// Ported from the original dxrender9/shaders/effects/effect.cpp: EffectImpl, the ID3DXEffect
// wrapper CDevice::NewEffect hands out, plus the effect cache members of CDevice.
//
// The original was built against an older d3dx9effect.h whose ID3DXBaseEffect has two more slots between
// GetTexture and GetPool than the June 2010 header (all ID3DXEffect calls from SetTechnique on sit
// 8 bytes higher in the original code); the methods are called by name here, so the vtable of the
// d3dx9_43 the port links against is the one that matters.
#include "shaders/effects/effect.h"

#include <algorithm>
#include <cstring>
#include <map>
#include <vector>

#include <config.h>
#include <core/console/cvar.h>
#include <core/ini.h>
#include <core/kernel.h>
#include <core/scoped_ptr.h>
#include <core/stringm3d.h>
#include <file/fileserver.h>
#include <file/filestream.h>
#include <math/vector.h>

#include "device.h"
#include "log.h"
#include "shaders/shader_compat.h"
#include "shaders/shader_include.h"

CDevice* EffectImpl::m_dev;

// orig 0x89c014: the scratch vector SetVector3 hands to D3DX (lives right after m_dev in .bss)
static CVector4 _v;

// orig 0x64a010 effect.cpp:45
EffectImpl::EffectImpl(bool bApplyGlobalParams)
    : m_bApplyGlobalParams(bApplyGlobalParams),
      m_effect(0),
      m_hasBeenValidated(false),
      m_didNotValidate(false),
      m_curTechnique(-1)
{
    memset(m_parameterHandles, 0, sizeof(m_parameterHandles));
}

// orig 0x64a960 effect.cpp:52
EffectImpl::~EffectImpl()
{
    Invalidate();
    m_dev->OnEffectDestructor(this);
}

// orig 0x64a9d0 effect.cpp:62
bool EffectImpl::LoadFromFile(const char* fileName, std::vector<CompileParam> const& compileParams)
{
    m_fileName = fileName;
    m_compileParams = compileParams;

    scoped_ptr<m3d::fs::FileStream> file(g_kernel->GetFileServer().CreateFileStream());

    if (!file->Open(fileName, m3d::fs::IStream::OPEN_READ))
    {
        LogMsg(CStr::format_("EffectImpl: could not open shader file '%s'", fileName));
        return false;
    }

    unsigned int fileSize = file->GetSize();

    // The original allocates through the kernel's allocator (AllocMem(size, 0, 0) / FreeMem(p, 0, 0)).
    uint8_t* buffer = (uint8_t*)g_kernel->g_mar.AllocMem(fileSize, 0, 0);

    file->ReadBytes(buffer, fileSize);

    std::vector<D3DXMACRO> ownMacros(m_compileParams.size());
    for (unsigned int i = 0; i < ownMacros.size(); i++)
    {
        ownMacros[i].Name = m_dev->CompileParamToString(m_compileParams[i]);
        ownMacros[i].Definition = "true";
    }

    ownMacros.insert(ownMacros.end(), m_dev->m_d3dxMacros.begin(), m_dev->m_d3dxMacros.end());
    D3DXMACRO* macros = ownMacros.size() > 1 ? &ownMacros[0] : NULL;

    ID3DXBuffer* errorBuffer = NULL;
    auxShaderInclude includeHandler;

    // retruxx adaptation: the original passes only D3DXSHADER_NO_PRESHADER and uses the D3DX of its own SDK.
    // With d3dx9_43 the effect is compiled by that era's compiler (d3dx9_31.dll) through
    // D3DXSHADER_USE_LEGACY_D3DX9_31_DLL, which keeps the ps_1_x techniques and their semantics;
    // without that DLL the fallback is the backwards-compatibility mode (ps_1_x as ps_2_0). See
    // shaders/shader_compat.h.
    DWORD const compileFlags = D3DXSHADER_NO_PRESHADER | (hlsl_compat::LegacyCompilerAvailable()
                                                              ? D3DXSHADER_USE_LEGACY_D3DX9_31_DLL
                                                              : D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY);
    HRESULT hr = D3DXCreateEffect(m_dev->GetDevice(), buffer, fileSize, macros, &includeHandler, compileFlags,
                                  m_dev->m_globalFxPool, &m_effect, &errorBuffer);

    if (buffer)
        g_kernel->g_mar.FreeMem(buffer, 0, 0);

    if (FAILED(hr))
    {
        LogMsg(CStr::format_("EffectImpl: failed to load fx file '%s' with: %s", fileName,
                             errorBuffer ? (const char*)errorBuffer->GetBufferPointer() : "No D3DX error message."));

        if (errorBuffer)
        {
            errorBuffer->Release();
            errorBuffer = NULL;
        }

        return false;
    }

    m_effect->SetStateManager(&m_dev->m_stateManager);

    m_hasBeenValidated = false;
    m_didNotValidate = false;
    ValidateEffect();

    return true;
}

// orig 0x6456e0 effect.cpp:159
bool EffectImpl::LoadFromString(const char* shaderBuf, unsigned int dataLen)
{
    return false;
}

// orig 0x6456f0 effect.cpp:166
bool EffectImpl::IsValid() const
{
    return m_hasBeenValidated;
}

// orig 0x645700 effect.cpp:173
void EffectImpl::SetInt(Parameter p, int val)
{
    m_curParams.SetInt(p, val);
    m_effect->SetInt(m_parameterHandles[p].d3dxHandle, val);
}

// orig 0x645740 effect.cpp:187
void EffectImpl::SetIntArray(Parameter p, const int* array, int count)
{
    m_effect->SetIntArray(m_parameterHandles[p].d3dxHandle, array, count);
}

// orig 0x645770 effect.cpp:197
void EffectImpl::SetFloat(Parameter p, float val)
{
    m_curParams.SetFloat(p, val);
    m_effect->SetFloat(m_parameterHandles[p].d3dxHandle, val);
}

// orig 0x6457c0 effect.cpp:211
void EffectImpl::SetFloatArray(Parameter p, const float* array, int count)
{
    m_effect->SetFloatArray(m_parameterHandles[p].d3dxHandle, array, count);
}

// orig 0x6457f0 effect.cpp:221
void EffectImpl::SetVector4(Parameter p, CVector4 const& val)
{
    m_curParams.SetVector4(p, val);
    m_effect->SetVector(m_parameterHandles[p].d3dxHandle, reinterpret_cast<D3DXVECTOR4 const*>(&val));
}

// orig 0x645860 effect.cpp:235
void EffectImpl::SetVector3(Parameter p, CVector const& val)
{
    _v.x = val.x;
    _v.y = val.y;
    _v.z = val.z;
    _v.w = 1.0f;
    m_effect->SetVector(m_parameterHandles[p].d3dxHandle, reinterpret_cast<D3DXVECTOR4 const*>(&_v));
}

// orig 0x6458c0 effect.cpp:250
void EffectImpl::SetFloat4(Parameter p, nFloat4 const& val)
{
    m_effect->SetVector(m_parameterHandles[p].d3dxHandle, reinterpret_cast<D3DXVECTOR4 const*>(&val));
}

// orig 0x6458f0 effect.cpp:261
void EffectImpl::SetFloat4Array(Parameter p, const nFloat4* array, int count)
{
    m_effect->SetVectorArray(m_parameterHandles[p].d3dxHandle, reinterpret_cast<D3DXVECTOR4 const*>(array), count);
}

// orig 0x645920 effect.cpp:271
void EffectImpl::SetVector4Array(Parameter p, const CVector4* array, int count)
{
    m_effect->SetVectorArray(m_parameterHandles[p].d3dxHandle, reinterpret_cast<D3DXVECTOR4 const*>(array), count);
}

// orig 0x645950 effect.cpp:281
void EffectImpl::SetMatrix(Parameter p, CMatrix const& val)
{
    m_effect->SetMatrix(m_parameterHandles[p].d3dxHandle, reinterpret_cast<D3DXMATRIX const*>(&val));
}

// orig 0x645980 effect.cpp:292
void EffectImpl::SetMatrixArray(Parameter p, const CMatrix* array, int count)
{
    m_effect->SetMatrixArray(m_parameterHandles[p].d3dxHandle, reinterpret_cast<D3DXMATRIX const*>(array), count);
}

// orig 0x6459b0 effect.cpp:302
void EffectImpl::SetMatrixPointerArray(Parameter p, const CMatrix** array, int count)
{
    m_effect->SetMatrixPointerArray(m_parameterHandles[p].d3dxHandle, reinterpret_cast<D3DXMATRIX const**>(array),
                                    count);
}

// orig 0x647510 effect.cpp:312
void EffectImpl::SetTexture(Parameter p, TexHandle* tex)
{
    CDevice::CTexture& texture = m_dev->m_textures[TexId(*tex)];

    // The original passes the tsc as a double (-1.0, the callee returns with `ret 0xc`).
    int frame = m_dev->GetTextureCurrentFrame(&texture, -1.0);

    m_effect->SetTexture(m_parameterHandles[p].d3dxHandle, texture.m_maps[frame]->m_pTex);
}

// orig 0x647570 effect.cpp:339
void EffectImpl::SetParams(nShaderParams& params)
{
    m_effect->BeginParameterBlock();

    bool changes = false;
    for (int i = 0; i < NumParameters; i++)
    {
        if (params.IsParameterValid((Parameter)i))
        {
            D3DXHANDLE handle = m_parameterHandles[i].d3dxHandle;
            if (handle)
            {
                nShaderArg const& arg = params.GetArg((Parameter)i);
                if (!(arg == m_curParams.GetArg((Parameter)i)))
                {
                    changes = true;
                    m_curParams.SetArg((Parameter)i, arg);

                    switch (arg.GetType())
                    {
                    case nShaderArg::Int:
                        m_effect->SetInt(handle, arg.GetInt());
                        break;

                    case nShaderArg::Float:
                        m_effect->SetFloat(handle, arg.GetFloat());
                        break;

                    case nShaderArg::Float4:
                        m_effect->SetVector(handle, reinterpret_cast<D3DXVECTOR4 const*>(&arg.GetFloat4()));
                        break;

                    case nShaderArg::Matrix44:
                        m_effect->SetMatrix(handle, reinterpret_cast<D3DXMATRIX const*>(arg.GetMatrix44()));
                        break;

                    case nShaderArg::Texture:
                        m_effect->SetTexture(handle, m_dev->m_textures[TexId(*arg.GetTexture())].m_maps[0]->m_pTex);
                        break;
                    }
                }
            }
        }
    }

    D3DXHANDLE block = m_effect->EndParameterBlock();

    if (changes)
    {
        m_effect->ApplyParameterBlock(block);
    }
}

// orig 0x6460c0 effect.cpp:412
void EffectImpl::UpdateParameterHandles()
{
    memset(m_parameterHandles, 0, sizeof(m_parameterHandles));

    m_effect->GetCurrentTechnique();

    for (int param = 0; param < NumParameters; param++)
    {
        const char* paramName = m_dev->EffectParameterToString((Parameter)param);

        // the parameters are bound by semantic, not by name
        m_parameterHandles[param].d3dxHandle = m_effect->GetParameterBySemantic(NULL, paramName);

        if (m_parameterHandles[param].d3dxHandle)
        {
            D3DXPARAMETER_DESC paramDesc;
            HRESULT hr = m_effect->GetParameterDesc(m_parameterHandles[param].d3dxHandle, &paramDesc);
            if (SUCCEEDED(hr))
            {
                m_parameterHandles[param].isShared = (paramDesc.Flags & D3DX_PARAMETER_SHARED) != 0;

                for (unsigned int i = 0; i < paramDesc.Annotations; i++)
                {
                    D3DXHANDLE annot = m_effect->GetAnnotation(m_parameterHandles[param].d3dxHandle, i);

                    D3DXPARAMETER_DESC annotDesc;
                    hr = m_effect->GetParameterDesc(annot, &annotDesc);
                    if (SUCCEEDED(hr))
                    {
                        CStr annotName(annotDesc.Name);
                        annotName.toLower();

                        if (annotName == "space" && annotDesc.Class == D3DXPC_SCALAR && annotDesc.Type == D3DXPT_INT)
                        {
                            m_effect->GetInt(annot, &m_parameterHandles[param].space);
                        }
                    }
                }
            }
            else
            {
                LogMsg(CStr::format_(
                    "EffectImpl() error: Could not get param description for: shader = '%s', param = '%s'",
                    m_fileName.c_str(), paramName));
            }
        }
    }
}

// orig 0x6459e0 effect.cpp:490
bool EffectImpl::getBoolAnnotation(D3DXHANDLE techHandle, const char* annotName, bool defaultValue)
{
    D3DXHANDLE annot = m_effect->GetAnnotationByName(techHandle, annotName);
    if (annot)
    {
        D3DXPARAMETER_DESC annotDesc;
        m_effect->GetParameterDesc(annot, &annotDesc);

        if (annotDesc.Type == D3DXPT_BOOL)
        {
            BOOL b;
            m_effect->GetBool(annot, &b);
            return b == TRUE;
        }
    }

    return defaultValue;
}

// orig 0x645a50 effect.cpp:513
int EffectImpl::getIntAnnotation(D3DXHANDLE techHandle, const char* annotName, int defaultValue)
{
    D3DXHANDLE annot = m_effect->GetAnnotationByName(techHandle, annotName);
    if (annot)
    {
        D3DXPARAMETER_DESC annotDesc;
        m_effect->GetParameterDesc(annot, &annotDesc);

        if (annotDesc.Type == D3DXPT_INT)
        {
            int i;
            m_effect->GetInt(annot, &i);
            return i;
        }
    }

    return defaultValue;
}

// orig 0x646270 effect.cpp:536
CStr EffectImpl::getStringAnnotation(D3DXHANDLE techHandle, const char* annotName, CStr const& defaultValue)
{
    CStr returnValue(defaultValue);

    D3DXHANDLE annot = m_effect->GetAnnotationByName(techHandle, annotName);
    if (annot)
    {
        D3DXPARAMETER_DESC annotDesc;
        m_effect->GetParameterDesc(annot, &annotDesc);

        if (annotDesc.Type == D3DXPT_STRING)
        {
            const char* s;
            m_effect->GetString(annot, &s);
            returnValue = s;
        }
    }

    return returnValue;
}

// orig 0x648e60 effect.cpp:559
void EffectImpl::getUserRenderParams(CStr const& userParamsStr, std::vector<TechniqueDesc::UserParam>& userParams)
{
    userParams.clear();
    if (userParamsStr.length() != 0)
    {
        std::vector<CStr> tokens;
        m3d::Tokenize(userParamsStr, tokens, ",");

        for (unsigned int i = 0; i < tokens.size(); i++)
        {
            std::vector<CStr> paramData;
            m3d::Tokenize(tokens[i], paramData, " /t");

            TechniqueDesc::UserParam param;
            param.name = paramData[0];

            if (paramData[1] == "MATRIX")
                param.type = TechniqueDesc::UPT_MATRIX;
            else if (paramData[1] == "VECTOR4")
                param.type = TechniqueDesc::UPT_VECTOR4;
            else if (paramData[1] == "VECTOR")
                param.type = TechniqueDesc::UPT_VECTOR;
            else if (paramData[1] == "FLOAT")
                param.type = TechniqueDesc::UPT_FLOAT;

            userParams.push_back(param);
        }
    }
}

// orig 0x64a0a0 effect.cpp:602
void EffectImpl::ValidateEffect()
{
    D3DXEFFECT_DESC effectDesc;
    m_effect->GetDesc(&effectDesc);

    m_numTechniques = effectDesc.Techniques;

    for (unsigned int i = 0; i < m_numTechniques; i++)
    {
        D3DXHANDLE techHandle = m_effect->GetTechnique(i);

        D3DXTECHNIQUE_DESC techDesc;
        m_effect->GetTechniqueDesc(techHandle, &techDesc);

        if (m_effect->ValidateTechnique(techHandle) != D3D_OK)
        {
            LogMsg(CStr::format_("EffectImpl() error: effect = '%s', could not validate technique '%s', skipping it...",
                                 m_fileName.c_str(), techDesc.Name));
            m_numTechniques--;
            continue;
        }

        TechniqueDesc desc;
        desc.name = techDesc.Name;
        desc.numPasses = techDesc.Passes;
        desc.briefDesc = getStringAnnotation(techHandle, "Description", "N/A");
        desc.tangentSpaceUsed = getBoolAnnotation(techHandle, "ComputeTangentSpace", false);
        desc.isDefault = getBoolAnnotation(techHandle, "Default", false);
        desc.isPS20 = getBoolAnnotation(techHandle, "IsPs20", false);
        desc.useAlpha = getBoolAnnotation(techHandle, "UseAlpha", true);
        desc.maxInstances = getIntAnnotation(techHandle, "MaxInstances", 0);
        getUserRenderParams(getStringAnnotation(techHandle, "UserParams", ""), desc.userParams);

        if (desc.isPS20)
        {
            if (!g_kernel->GetEngineCfg().m_r_allowPS20.GetB()
                || (m_dev->m_isNV30 && !g_kernel->GetEngineCfg().m_r_allowPS20ForNV30.GetB())
                || !m_dev->IsFeatureSupported(FEATURE_PS_2_0))
            {
                m_numTechniques--;
                continue;
            }
        }

        desc.vertexFormatStr = getStringAnnotation(techHandle, "VertexFormat", "");

        if (desc.vertexFormatStr == "VERTEX_XYZCT1")
            desc.vertexFormat = VERTEX_XYZCT1;
        else if (desc.vertexFormatStr == "VERTEX_XYZNT1")
            desc.vertexFormat = VERTEX_XYZNT1;
        else if (desc.vertexFormatStr == "VERTEX_XYZNC")
            desc.vertexFormat = VERTEX_XYZNC;
        else if (desc.vertexFormatStr == "VERTEX_XYZC")
            desc.vertexFormat = VERTEX_XYZC;
        else if (desc.vertexFormatStr == "VERTEX_XYZWCT1")
            desc.vertexFormat = VERTEX_XYZWCT1;
        else if (desc.vertexFormatStr == "VERTEX_XYZNCT1")
            desc.vertexFormat = VERTEX_XYZNCT1;
        else if (desc.vertexFormatStr == "VERTEX_XYZNCT2")
            desc.vertexFormat = VERTEX_XYZNCT2;
        else if (desc.vertexFormatStr == "VERTEX_XYZNT2")
            desc.vertexFormat = VERTEX_XYZNT2;
        else if (desc.vertexFormatStr == "VERTEX_XYZNT3")
            desc.vertexFormat = VERTEX_XYZNT3;
        else if (desc.vertexFormatStr == "VERTEX_XYZCT2")
            desc.vertexFormat = VERTEX_XYZCT2;
        else if (desc.vertexFormatStr == "VERTEX_XYZNT1T")
            desc.vertexFormat = VERTEX_XYZNT1T;
        else if (desc.vertexFormatStr == "VERTEX_XYZNCT1T")
            desc.vertexFormat = VERTEX_XYZNCT1T;
        else if (desc.vertexFormatStr == "VERTEX_XYZNCT1_UV2_S1")
            desc.vertexFormat = VERTEX_XYZNCT1_UV2_S1;
        else if (desc.vertexFormatStr == "VERTEX_STREAM_UV_S1")
            desc.vertexFormat = VERTEX_STREAM_UV_S1;
        else if (desc.vertexFormatStr == "VERTEX_XYZ")
            desc.vertexFormat = VERTEX_XYZ;
        else
        {
            LogMsg(CStr::format_(
                "EffectImpl() error: effect = '%s', unsupported vertex format '%s' in technique '%s', skipping it...",
                m_fileName.c_str(), desc.vertexFormatStr.c_str(), techDesc.Name));
            m_numTechniques--;
            continue;
        }

        TechniqueDescInternal internalDesc;
        internalDesc.publicDesc = desc;

        internalDesc.handle = techHandle;
        m_techDescs.push_back(internalDesc);
    }

    m_defaultTechnique = -1;
    m_defaultPS20Technique = -1;
    for (unsigned int i = 0; i < m_numTechniques; i++)
    {
        if (m_techDescs[i].publicDesc.isDefault)
        {
            m_defaultTechnique = i;

            if (m_techDescs[i].publicDesc.isPS20)
                m_defaultPS20Technique = i;
        }
    }

    if (m_defaultTechnique == -1)
    {
        LogMsg(CStr::format_(
            "EffectImpl() warning: effect = '%s', no technique found as default, setting first one ('%s')...",
            m_fileName.c_str(), m_techDescs[0].publicDesc.name.c_str()));
        m_defaultTechnique = 0;
    }

    m_didNotValidate = false;
    m_hasBeenValidated = true;
}

// orig 0x645ae0 effect.cpp:793
unsigned int EffectImpl::GetNumTechniques() const
{
    return m_numTechniques;
}

// orig 0x6476e0 effect.cpp:803
IEffect::TechniqueDesc const& EffectImpl::GetTechniqueDesc(unsigned int techId) const
{
    return m_techDescs[techId].publicDesc;
}

// orig 0x647700 effect.cpp:814
void EffectImpl::SetCurTechnique(unsigned int techId)
{
    if (techId != m_curTechnique)
    {
        m_effect->SetTechnique(m_techDescs[techId].handle);
        UpdateParameterHandles();
        m_curTechnique = techId;
    }
}

// orig 0x647740 effect.cpp:831
void EffectImpl::SetCurTechniqueByName(const char* techName)
{
    for (unsigned int i = 0; i < m_numTechniques; i++)
    {
        if (m_techDescs[i].publicDesc.name == techName)
        {
            SetCurTechnique(i);
        }
    }
}

// orig 0x645af0 effect.cpp:848
void EffectImpl::SetDefaultTechnique(bool preferPS20)
{
    if (preferPS20 && m_defaultPS20Technique >= 0)
        SetCurTechnique(m_defaultPS20Technique);
    else
        SetCurTechnique(m_defaultTechnique);
}

// orig 0x645b20 effect.cpp:859
unsigned int EffectImpl::GetCurTechnique() const
{
    return m_curTechnique;
}

// orig 0x647790 effect.cpp:866
const char* EffectImpl::GetCurTechniqueName() const
{
    if (m_curTechnique >= 0 && m_curTechnique < (int)m_numTechniques)
    {
        return m_techDescs[m_curTechnique].publicDesc.name.c_str();
    }

    return "";
}

// orig 0x645b30 effect.cpp:881
bool EffectImpl::IsParameterUsed(Parameter p)
{
    return m_parameterHandles[p].d3dxHandle != 0;
}

// orig 0x646330 effect.cpp:892
void EffectImpl::ApplyGlobalFxParams()
{
    if (m_parameterHandles[World].d3dxHandle)
    {
        m_effect->SetMatrix(m_parameterHandles[World].d3dxHandle,
                            reinterpret_cast<D3DXMATRIX const*>(&m_dev->GetModelMatrix()));
    }

    if (m_parameterHandles[IEffect::InvWorld].d3dxHandle)
    {
        CMatrix InvWorld = m_dev->GetModelMatrix().getInverseSimple();
        m_effect->SetMatrix(m_parameterHandles[IEffect::InvWorld].d3dxHandle,
                            reinterpret_cast<D3DXMATRIX const*>(&InvWorld));
    }

    if (m_parameterHandles[View].d3dxHandle)
    {
        m_effect->SetMatrix(m_parameterHandles[View].d3dxHandle, reinterpret_cast<D3DXMATRIX const*>(&m_dev->MatGet()));
    }

    if (m_parameterHandles[ModelView].d3dxHandle)
    {
        CMatrix modelView = m_dev->MatGetWorld() * m_dev->MatGet();
        m_effect->SetMatrix(m_parameterHandles[ModelView].d3dxHandle, reinterpret_cast<D3DXMATRIX const*>(&modelView));
    }

    if (m_parameterHandles[Projection].d3dxHandle)
    {
        m_effect->SetMatrix(m_parameterHandles[Projection].d3dxHandle,
                            reinterpret_cast<D3DXMATRIX const*>(&m_dev->MatGetProj()));
    }

    if (m_parameterHandles[ModelViewProjection].d3dxHandle)
    {
        m_effect->SetMatrix(m_parameterHandles[ModelViewProjection].d3dxHandle,
                            reinterpret_cast<D3DXMATRIX const*>(m_dev->GetModelViewProjMatrix()));
    }

    if (m_parameterHandles[TmpLight0Dir].d3dxHandle)
    {
        CVector normLightDir(m_dev->m_lights[0].Direction.x, m_dev->m_lights[0].Direction.y,
                             m_dev->m_lights[0].Direction.z);

        if (m_parameterHandles[TmpLight0Dir].space == 1)
        {
            normLightDir = m_dev->MatGetWorld().vecRotBack(normLightDir);
        }

        if (m_parameterHandles[TmpLight0Dir].space == 2)
        {
            normLightDir = m_dev->GetModelMatrix().vecRot(normLightDir);
        }

        m_effect->SetValue(m_parameterHandles[TmpLight0Dir].d3dxHandle, &normLightDir, sizeof(CVector));
    }

    if (m_parameterHandles[ViewPos].d3dxHandle)
    {
        CVector const& viewOrigin = m_dev->GetViewOrigin();

        CVector4 vp(viewOrigin.x, viewOrigin.y, viewOrigin.z, 1.0f);
        if (m_parameterHandles[ViewPos].space == 1)
        {
            CMatrix invWorldMatrix = m_dev->GetModelMatrix().getInverseSimple();
            vp = invWorldMatrix.vecMul(vp);
        }

        m_effect->SetVector(m_parameterHandles[ViewPos].d3dxHandle, reinterpret_cast<D3DXVECTOR4 const*>(&vp));
    }

    if (m_parameterHandles[NormalizationCubemap].d3dxHandle)
    {
        m_effect->SetTexture(m_parameterHandles[NormalizationCubemap].d3dxHandle, m_dev->GetNormalCubemap());
    }
}

// orig 0x646aa0 effect.cpp:1004
int EffectImpl::Begin()
{
    if (m_didNotValidate)
    {
        return 0;
    }

    if (m_bApplyGlobalParams)
        ApplyGlobalFxParams();

    m_dev->PushBlend();
    m_dev->PushAlphaTest();

    UINT numPasses;
    m_effect->Begin(&numPasses, D3DXFX_DONOTSAVESTATE | D3DXFX_DONOTSAVESHADERSTATE);

    return numPasses;
}

// orig 0x645b50 effect.cpp:1033
void EffectImpl::BeginPass(unsigned int pass)
{
    m_effect->BeginPass(pass);
}

// orig 0x645b70 effect.cpp:1043
void EffectImpl::EndPass()
{
    m_effect->EndPass();
}

// orig 0x645b80 effect.cpp:1053
void EffectImpl::CommitChanges()
{
    m_effect->CommitChanges();
}

// orig 0x645b90 effect.cpp:1063
void EffectImpl::End()
{
    if (!m_didNotValidate)
    {
        m_effect->End();

        m_dev->PopBlend();
        m_dev->PopAlphaTest();
    }
}

// orig 0x64a760 effect.cpp:1080
void EffectImpl::Invalidate()
{
    if (m_effect)
    {
        m_effect->Release();
        m_effect = 0;
    }
    m_hasBeenValidated = false; m_didNotValidate = false; m_curTechnique = -1; m_techDescs.clear();
    m_curParams.Reset();
}

// orig 0x645bc0 effect.cpp:1092
void EffectImpl::OnDeviceReset()
{
    if (m_effect)
        m_effect->OnLostDevice();
}

// orig 0x645be0 effect.cpp:1100
void EffectImpl::OnDeviceRestore()
{
    if (m_effect)
        m_effect->OnResetDevice();
}

// orig 0x64a7c0 effect.cpp:1108
IEffect* CDevice::NewEffect(const char* fileName, bool bApplyGlobalParams, std::vector<CompileParam> const& compileParams)
{
    ShaderIdData fxId(fileName, compileParams);
    UnifyFileName(fxId.filename);

    std::sort(fxId.compileParams.begin(), fxId.compileParams.end());
    fxId.compileParams.erase(std::unique(fxId.compileParams.begin(), fxId.compileParams.end()),
                             fxId.compileParams.end());

    std::map<ShaderIdData, EffectImpl*>::iterator it = m_effects.find(fxId);
    if (it != m_effects.end())
    {
        EffectImpl* fx = it->second;
        fx->AddRef();
        return fx;
    }

    EffectImpl* fx = new EffectImpl(bApplyGlobalParams);
    if (!fx->LoadFromFile(fxId.filename.c_str(), fxId.compileParams))
    {
        delete fx;
        return 0;
    }

    fx->AddRef();
    m_effects[fxId] = fx;
    return fx;
}

// HTA-only wrapper, no original body
IEffect* CDevice::NewEffect(const char* fileName, bool bApplyGlobalParams)
{
    return NewEffect(fileName, bApplyGlobalParams, std::vector<CompileParam>());
}

// orig 0x649ec0 effect.cpp:1149
void CDevice::OnEffectDestructor(EffectImpl* effect)
{
    ShaderIdData fxId(effect->m_fileName, effect->m_compileParams);
    UnifyFileName(fxId.filename);
    m_effects.erase(fxId);
}

// orig 0x647cc0 effect.cpp:1157
void CDevice::ReleaseEffects()
{
    for (std::map<ShaderIdData, EffectImpl*>::iterator it = m_effects.begin(); it != m_effects.end(); ++it)
    {
        EffectImpl* fx = it->second;

        LogMsg(CStr::format_("Warning: fx shader '%s' is not released (refs = %d)", fx->m_fileName.c_str(),
                             fx->GetRefCount()));
    }
}

// orig 0x6477c0 effect.cpp:1168
bool CDevice::ShaderIdData::operator<(ShaderIdData const& a) const
{
    if (filename == a.filename)
        return compileParams < a.compileParams;

    return filename < a.filename;
}
