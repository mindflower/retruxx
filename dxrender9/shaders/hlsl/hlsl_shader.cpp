// Ported from the original dxrender9/shaders/hlsl/hlsl_shader.cpp: a stand-alone HLSL vertex or
// pixel shader compiled with D3DXCompileShader, its parameters set through the D3DX constant table,
// and the CDevice side of its cache.
#include "shaders/hlsl/hlsl_shader.h"

#include <algorithm>
#include <cstring>
#include <string>

#include <core/kernel.h>
#include <core/scoped_ptr.h>
#include <file/fileserver.h>
#include <file/filestream.h>

#include "device.h"
#include "log.h"
#include "shaders/shader_compat.h"
#include "shaders/shader_include.h"

CDevice* HlslShaderImpl::m_dev = 0;

// ProfileStringTable @ 0x85280c: the D3DX profile name of each IHlslShader::Profile. The original hands
// "unsupported" to D3DXCompileShader for VS_3_0 and PS_3_0.
static const char* ProfileStringTable[9] = {
    "vs_1_1",      // VS_1_1
    "vs_2_0",      // VS_2_0
    "unsupported", // VS_3_0
    "ps_1_1",      // PS_1_1
    "ps_1_3",      // PS_1_3
    "ps_1_4",      // PS_1_4
    "ps_2_0",      // PS_2_0
    "ps_2_a",      // PS_2_a
    "unsupported", // PS_3_0
};

// orig 0x64b760 hlsl_shader.cpp:39
HlslShaderImpl::HlslShaderImpl()
{
    m_shader = 0;
    m_constantTable = 0;
    m_numConstants = 0;
}

// orig 0x64c420 hlsl_shader.cpp:46
HlslShaderImpl::~HlslShaderImpl()
{
    if (m_shader)
    {
        if (IsVertexShader())
            m_vs->Release();
        else
            m_ps->Release();
    }

    if (m_constantTable)
    {
        m_constantTable->Release();
        m_constantTable = 0;
    }

    m_dev->OnHlslShaderDestructor(this);
}

// orig 0x64ad10 hlsl_shader.cpp:64
bool HlslShaderImpl::IsVertexShader() const
{
    return m_profile >= VS_1_1 && m_profile <= VS_3_0;
}

// orig 0x64b850 hlsl_shader.cpp:94
bool HlslShaderImpl::LoadFromFile(const char* fileName, const char* entryFunc, Profile profile,
                                  std::vector<CompileParam> const& compileParams)
{
    m_fileName = fileName;
    m_compileParams = compileParams;
    m_entryPoint = entryFunc;
    m_profile = profile;

    scoped_ptr<m3d::fs::FileStream> stream(g_kernel->GetFileServer().CreateFileStream());

    if (!stream->Open(fileName, m3d::fs::IStream::OPEN_READ))
    {
        LogMsg(CStr::format_("HlslShaderImpl: could not open shader file '%s'", fileName));
        return false;
    }

    unsigned int fileSize = stream->GetSize();

    char* buffer = new char[fileSize];

    stream->ReadBytes(buffer, fileSize);

    ID3DXBuffer* shaderCode;
    ID3DXBuffer* errorBuffer = 0;
    auxShaderInclude includeHandler;

    // ps_2_0 shaders are compiled for the ps_2_a profile on NV3x
    if (m_dev->IsNV3x() && profile == PS_2_0)
        profile = PS_2_a;

    // the compile parameters as macros, followed by the device's macro list (with its terminator)
    std::vector<D3DXMACRO> ownMacros(m_compileParams.size());
    for (unsigned int i = 0; i < ownMacros.size(); i++)
    {
        ownMacros[i].Name = m_dev->CompileParamToString(m_compileParams[i]);
        ownMacros[i].Definition = "true";
    }

    ownMacros.insert(ownMacros.end(), m_dev->m_d3dxMacros.begin(), m_dev->m_d3dxMacros.end());
    D3DXMACRO* macros = ownMacros.size() > 1 ? &ownMacros[0] : 0;

    // retruxx adaptation: the original passes the file to D3DXCompileShader with flags 0 and the D3DX of its
    // own SDK. With d3dx9_43 the shaders are compiled by that era's compiler (d3dx9_31.dll) through
    // D3DXSHADER_USE_LEGACY_D3DX9_31_DLL, which yields the original ps_1_x / vs_1_x code; without
    // that DLL the fallback is the backwards-compatibility mode (ps_1_x as ps_2_0) on a source
    // preprocessed in memory with its samplers bound in declaration order. See shader_compat.h.
    HRESULT hr;
    ID3DXBuffer* preprocessed = 0;
    std::string rewritten;
    if (hlsl_compat::LegacyCompilerAvailable())
    {
        hr = D3DXCompileShader(buffer, fileSize, macros, &includeHandler, entryFunc, ProfileStringTable[profile],
                               D3DXSHADER_USE_LEGACY_D3DX9_31_DLL, &shaderCode, &errorBuffer, &m_constantTable);
    }
    else if (SUCCEEDED(D3DXPreprocessShader(buffer, fileSize, macros, &includeHandler, &preprocessed, 0)) &&
             preprocessed)
    {
        const char* text = (const char*)preprocessed->GetBufferPointer();
        rewritten.assign(text, strnlen(text, preprocessed->GetBufferSize()));
        preprocessed->Release();
        if (hlsl_compat::AssignSamplerRegistersInDeclarationOrder(rewritten) > 0)
        {
            LogMsg(CStr::format_("HlslShaderImpl: sampler registers of shader file '%s' assigned in declaration order",
                                 fileName));
        }
        hr = D3DXCompileShader(rewritten.c_str(), (UINT)rewritten.size(), 0, &includeHandler, entryFunc,
                               ProfileStringTable[profile], D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY, &shaderCode,
                               &errorBuffer, &m_constantTable);
    }
    else
    {
        hr = D3DXCompileShader(buffer, fileSize, macros, &includeHandler, entryFunc, ProfileStringTable[profile],
                               D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY, &shaderCode, &errorBuffer,
                               &m_constantTable);
    }

    delete[] buffer;

    if (FAILED(hr))
    {
        LogMsg(CStr::format_("HlslShaderImpl: failed to load shader file '%s' with: %s", fileName,
                             errorBuffer ? (const char*)errorBuffer->GetBufferPointer() : "No D3DX error message."));

        if (errorBuffer)
        {
            errorBuffer->Release();
            errorBuffer = 0;
        }

        return false;
    }

    if (IsVertexShader())
    {
        m_dev->m_pd3dDevice->CreateVertexShader((DWORD*)shaderCode->GetBufferPointer(), &m_vs);
    }
    else
    {
        m_dev->m_pd3dDevice->CreatePixelShader((DWORD*)shaderCode->GetBufferPointer(), &m_ps);
    }

    UpdateShaderInfo();

    return true;
}

// orig 0x64ad30 hlsl_shader.cpp:200
bool HlslShaderImpl::LoadFromString(const char* shaderBuf, unsigned int dataLen, const char* entryFunc,
                                    Profile profile)
{
    return false;
}

// orig 0x64ad40 hlsl_shader.cpp:207
bool HlslShaderImpl::IsValid() const
{
    return false;
}

// orig 0x64b190 hlsl_shader.cpp:214
CStr getParamRegisterSetName(D3DXREGISTER_SET a0)
{
    switch (a0)
    {
    case D3DXRS_BOOL:
        return CStr("D3DXRS_BOOL");
    case D3DXRS_INT4:
        return CStr("D3DXRS_INT4");
    case D3DXRS_FLOAT4:
        return CStr("D3DXRS_FLOAT4");
    case D3DXRS_SAMPLER:
        return CStr("D3DXRS_SAMPLER");
    default:
        return CStr("Unknown!");
    }
}

// orig 0x64b200 hlsl_shader.cpp:237
CStr getParamClassName(D3DXPARAMETER_CLASS a0)
{
    switch (a0)
    {
    case D3DXPC_SCALAR:
        return CStr("D3DXPC_SCALAR");
    case D3DXPC_VECTOR:
        return CStr("D3DXPC_VECTOR");
    case D3DXPC_MATRIX_ROWS:
        return CStr("D3DXPC_MATRIX_ROWS");
    case D3DXPC_MATRIX_COLUMNS:
        return CStr("D3DXPC_MATRIX_COLUMNS");
    case D3DXPC_OBJECT:
        return CStr("D3DXPC_OBJECT");
    case D3DXPC_STRUCT:
        return CStr("D3DXPC_STRUCT");
    default:
        return CStr("Unknown!");
    }
}

// orig 0x64b2a0 hlsl_shader.cpp:266
CStr getParamTypeName(D3DXPARAMETER_TYPE a0)
{
    switch (a0)
    {
    case D3DXPT_VOID:
        return CStr("D3DXPT_VOID");
    case D3DXPT_BOOL:
        return CStr("D3DXPT_BOOL");
    case D3DXPT_INT:
        return CStr("D3DXPT_INT");
    case D3DXPT_FLOAT:
        return CStr("D3DXPT_FLOAT");
    case D3DXPT_STRING:
        return CStr("D3DXPT_STRING");
    case D3DXPT_TEXTURE:
        return CStr("D3DXPT_TEXTURE");
    case D3DXPT_TEXTURE1D:
        return CStr("D3DXPT_TEXTURE1D");
    case D3DXPT_TEXTURE2D:
        return CStr("D3DXPT_TEXTURE2D");
    case D3DXPT_TEXTURE3D:
        return CStr("D3DXPT_TEXTURE3D");
    case D3DXPT_TEXTURECUBE:
        return CStr("D3DXPT_TEXTURECUBE");
    case D3DXPT_SAMPLER:
        return CStr("D3DXPT_SAMPLER");
    case D3DXPT_SAMPLER1D:
        return CStr("D3DXPT_SAMPLER1D");
    case D3DXPT_SAMPLER2D:
        return CStr("D3DXPT_SAMPLER2D");
    case D3DXPT_SAMPLER3D:
        return CStr("D3DXPT_SAMPLER3D");
    case D3DXPT_SAMPLERCUBE:
        return CStr("D3DXPT_SAMPLERCUBE");
    case D3DXPT_PIXELSHADER:
        return CStr("D3DXPT_PIXELSHADER");
    case D3DXPT_VERTEXSHADER:
        return CStr("D3DXPT_VERTEXSHADER");
    case D3DXPT_PIXELFRAGMENT:
        return CStr("D3DXPT_PIXELFRAGMENT");
    case D3DXPT_VERTEXFRAGMENT:
        return CStr("D3DXPT_VERTEXFRAGMENT");
    default:
        return CStr("Unknown!");
    }
}

// orig 0x64ad50 hlsl_shader.cpp:334
void HlslShaderImpl::UpdateShaderInfo()
{
    D3DXCONSTANTTABLE_DESC desc;
    m_constantTable->GetDesc(&desc);
    m_numConstants = desc.Constants;

    m_constantTable->SetDefaults(m_dev->m_pd3dDevice);
}

// orig 0x64ad90 hlsl_shader.cpp:372
bool HlslShaderImpl::IsValidParam(unsigned int p) const
{
    return true;
}

// orig 0x64ada0 hlsl_shader.cpp:382
unsigned int HlslShaderImpl::GetNumberOfParams() const
{
    return m_numConstants;
}

// orig 0x64adb0 hlsl_shader.cpp:389
unsigned int HlslShaderImpl::GetParamHandleByName(const char* paramName)
{
    return (unsigned int)(size_t)m_constantTable->GetConstantByName(0, paramName);
}

// orig 0x64add0 hlsl_shader.cpp:396
void HlslShaderImpl::SetInt(unsigned int p, int val)
{
    m_constantTable->SetInt(m_dev->m_pd3dDevice, (D3DXHANDLE)(size_t)p, val);
}

// orig 0x64ae00 hlsl_shader.cpp:406
void HlslShaderImpl::SetIntArray(unsigned int p, const int* array, int count)
{
    m_constantTable->SetIntArray(m_dev->m_pd3dDevice, (D3DXHANDLE)(size_t)p, array, count);
}

// orig 0x64ae30 hlsl_shader.cpp:416
void HlslShaderImpl::SetFloat(unsigned int p, float val)
{
    m_constantTable->SetFloat(m_dev->m_pd3dDevice, (D3DXHANDLE)(size_t)p, val);
}

// orig 0x64ae60 hlsl_shader.cpp:426
void HlslShaderImpl::SetFloatArray(unsigned int p, const float* array, int count)
{
    m_constantTable->SetFloatArray(m_dev->m_pd3dDevice, (D3DXHANDLE)(size_t)p, array, count);
}

// orig 0x64ae90 hlsl_shader.cpp:440
void HlslShaderImpl::SetVector4(unsigned int p, CVector4 const& val)
{
    m_constantTable->SetVector(m_dev->m_pd3dDevice, (D3DXHANDLE)(size_t)p, (const D3DXVECTOR4*)&val);
}

// orig 0x64b440 hlsl_shader.cpp:451
void HlslShaderImpl::SetVector3(unsigned int p, CVector const& val)
{
    static D3DXVECTOR4 v4;
    v4.x = val.x;
    v4.y = val.y;
    v4.z = val.z;
    v4.w = 1.0f;

    m_constantTable->SetVector(m_dev->m_pd3dDevice, (D3DXHANDLE)(size_t)p, &v4);
}

// orig 0x64aec0 hlsl_shader.cpp:466
void HlslShaderImpl::SetFloat4(unsigned int p, nFloat4 const& val)
{
    m_constantTable->SetVector(m_dev->m_pd3dDevice, (D3DXHANDLE)(size_t)p, (const D3DXVECTOR4*)&val);
}

// orig 0x64aef0 hlsl_shader.cpp:481
void HlslShaderImpl::SetFloat4Array(unsigned int p, const nFloat4* array, int count)
{
    m_constantTable->SetVectorArray(m_dev->m_pd3dDevice, (D3DXHANDLE)(size_t)p, (const D3DXVECTOR4*)array, count);
}

// orig 0x64af20 hlsl_shader.cpp:491
void HlslShaderImpl::SetVector4Array(unsigned int p, const CVector4* array, int count)
{
    m_constantTable->SetVectorArray(m_dev->m_pd3dDevice, (D3DXHANDLE)(size_t)p, (const D3DXVECTOR4*)array, count);
}

// orig 0x64af50 hlsl_shader.cpp:501
void HlslShaderImpl::SetMatrix(unsigned int p, CMatrix const& val)
{
    m_constantTable->SetMatrix(m_dev->m_pd3dDevice, (D3DXHANDLE)(size_t)p, (const D3DXMATRIX*)&val);
}

// orig 0x64af80 hlsl_shader.cpp:516
void HlslShaderImpl::SetMatrixArray(unsigned int p, const CMatrix* array, int count)
{
    m_constantTable->SetMatrixArray(m_dev->m_pd3dDevice, (D3DXHANDLE)(size_t)p, (const D3DXMATRIX*)array, count);
}

// orig 0x64afb0 hlsl_shader.cpp:526
void HlslShaderImpl::SetMatrixPointerArray(unsigned int p, const CMatrix** array, int count)
{
    m_constantTable->SetMatrixPointerArray(m_dev->m_pd3dDevice, (D3DXHANDLE)(size_t)p, (const D3DXMATRIX**)array,
                                           count);
}

// orig 0x64afe0 hlsl_shader.cpp:539
void HlslShaderImpl::ValidateEffect()
{
}

// orig 0x64aff0 hlsl_shader.cpp:547
void HlslShaderImpl::Invalidate()
{
    if (m_shader)
    {
        if (IsVertexShader())
            m_vs->Release();
        else
            m_ps->Release();
    }

    m_shader = 0;

    if (m_constantTable)
    {
        m_constantTable->Release();
        m_constantTable = 0;
    }
}

// orig 0x64b040 hlsl_shader.cpp:564
void HlslShaderImpl::OnDeviceReset()
{
}

// orig 0x64b050 hlsl_shader.cpp:570
void HlslShaderImpl::OnDeviceRestore()
{
}

// orig 0x64b060 hlsl_shader.cpp:576
void HlslShaderImpl::Apply()
{
    if (IsVertexShader())
        m_dev->setVertexShader(m_vs);
    else
        m_dev->setPixelShader(m_ps);
}

// orig 0x64c140 hlsl_shader.cpp:589
IHlslShader* CDevice::NewHlslShader(const char* fileName, const char* entryFunc, IHlslShader::Profile profile,
                                    std::vector<CompileParam> const& compileParams)
{
    ShaderIdData shaderId(CStr(fileName), compileParams);

    UnifyFileName(shaderId.filename);

    std::sort(shaderId.compileParams.begin(), shaderId.compileParams.end());
    shaderId.compileParams.erase(std::unique(shaderId.compileParams.begin(), shaderId.compileParams.end()),
                                 shaderId.compileParams.end());

    std::map<ShaderIdData, HlslShaderImpl*>::iterator it = m_HlslShaders.find(shaderId);
    if (it != m_HlslShaders.end())
    {
        HlslShaderImpl* shader = it->second;

        shader->AddRef();
        return shader;
    }

    HlslShaderImpl* shader = new HlslShaderImpl();
    if (!shader->LoadFromFile(fileName, entryFunc, profile, shaderId.compileParams))
    {
        delete shader;
        return 0;
    }

    shader->AddRef();
    m_HlslShaders[shaderId] = shader;
    return shader;
}

// HTA-only wrapper, no original body
IHlslShader* CDevice::NewHlslShader(const char* fileName, const char* entryFunc, IHlslShader::Profile profile)
{
    return NewHlslShader(fileName, entryFunc, profile, std::vector<CompileParam>());
}

// orig 0x64c380 hlsl_shader.cpp:631
void CDevice::OnHlslShaderDestructor(HlslShaderImpl* shader)
{
    ShaderIdData shaderId(shader->m_fileName, shader->m_compileParams);
    UnifyFileName(shaderId.filename);
    m_HlslShaders.erase(shaderId);
}

// orig 0x64b6b0 hlsl_shader.cpp:640
void CDevice::ReleaseHlslShaders()
{
    for (std::map<ShaderIdData, HlslShaderImpl*>::iterator it = m_HlslShaders.begin(); it != m_HlslShaders.end();
         ++it)
    {
        HlslShaderImpl* shader = it->second;

        LogMsg(CStr::format_("Warning: Hlsl shader '%s' is not released (refs = %d)", shader->m_fileName.c_str(),
                             shader->GetRefCount()));
    }
}
