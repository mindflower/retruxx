// Ported from the original dxrender9/shaders/shaders.cpp: the shader constant setters, the shader
// macro set (the user macros kept in the engine configuration's r_shadersMacros cvars plus the
// device dependent ones), the shader reload and the current vertex / pixel shader latches of
// CDevice.
#include "device.h"

#include <algorithm>
#include <string.h>

#include <config.h>
#include <core/console/cvar.h>
#include <core/ini.h>
#include <core/kernel.h>

#include "log.h"
#include "shaders/assembly/asm_shader.h"
#include "shaders/effects/effect.h"
#include "shaders/hlsl/hlsl_shader.h"

// The number of r_shadersMacros cvars (m3d::EngineConfig) the user macros are kept in.
static const unsigned int SHADERS_MACROS_NUM = 8;

// The original asserts through m3d::Kernel::SysError(assertion, file, line). The HTA kernel's SysError takes
// (whence, descr); lib/include/core/kernel.h's M3D_ASSERT builds them this way, but through the
// m3d::g_Kernel global, which the DLL does not define, so the same macro is spelled here over
// g_kernel.
#define DX_ASSERT(cond)                                                       \
    do                                                                        \
    {                                                                         \
        if (!(cond))                                                          \
            g_kernel->SysError(CStr(__FILE__ ":") + CStr(__LINE__), (#cond)); \
    } while (0)

// orig 0x639f00 shaders.cpp:32
void CDevice::SetVsFloatConst(unsigned int RegisterIndex, const float* pConstantData, unsigned int RegisterCount)
{
    m_lastResult = m_pd3dDevice->SetVertexShaderConstantF(RegisterIndex, pConstantData, RegisterCount);
}

// orig 0x639f30 shaders.cpp:41
void CDevice::SetVsIntConst(unsigned int RegisterIndex, const int* pConstantData, unsigned int RegisterCount)
{
    m_lastResult = m_pd3dDevice->SetVertexShaderConstantI(RegisterIndex, pConstantData, RegisterCount);
}

// orig 0x639f60 shaders.cpp:50
void CDevice::SetVsBoolConst(unsigned int RegisterIndex, const int* pConstantData, unsigned int RegisterCount)
{
    m_lastResult = m_pd3dDevice->SetVertexShaderConstantB(RegisterIndex, pConstantData, RegisterCount);
}

// orig 0x639f90 shaders.cpp:59
void CDevice::SetPsFloatConst(unsigned int RegisterIndex, const float* pConstantData, unsigned int RegisterCount)
{
    m_lastResult = m_pd3dDevice->SetPixelShaderConstantF(RegisterIndex, pConstantData, RegisterCount);
}

// orig 0x639fc0 shaders.cpp:68
void CDevice::SetPsIntConst(unsigned int RegisterIndex, const int* pConstantData, unsigned int RegisterCount)
{
    m_lastResult = m_pd3dDevice->SetPixelShaderConstantI(RegisterIndex, pConstantData, RegisterCount);
}

// orig 0x639ff0 shaders.cpp:77
void CDevice::SetPsBoolConst(unsigned int RegisterIndex, const int* pConstantData, unsigned int RegisterCount)
{
    m_lastResult = m_pd3dDevice->SetPixelShaderConstantB(RegisterIndex, pConstantData, RegisterCount);
}

// orig 0x63b770 shaders.cpp:86
void CDevice::rstShadersPrepareFor()
{
    for (std::map<ShaderIdData, EffectImpl*>::iterator it = m_effects.begin(); it != m_effects.end(); ++it)
    {
        it->second->OnDeviceReset();
    }
}

// orig 0x63b7e0 shaders.cpp:97
void CDevice::rstShadersRestoreAfter()
{
    for (std::map<ShaderIdData, EffectImpl*>::iterator it = m_effects.begin(); it != m_effects.end(); ++it)
    {
        it->second->OnDeviceRestore();
    }
}

// orig 0x63c070 shaders.cpp:108
bool CDevice::ReloadShaders()
{
    bool res = true;

    AsmShaderImpl* shaderAsmTmp = new AsmShaderImpl();

    for (std::map<CStr, AsmShaderImpl*>::iterator it = m_AsmShaders.begin(); it != m_AsmShaders.end(); ++it)
    {
        AsmShaderImpl* shader = it->second;
        CStr const& fileName = shader->m_fileName;
        IAsmShader::Type type = shader->m_type;

        // the temporary shader checks whether the file compiles before the live one is touched
        if (!shaderAsmTmp->LoadFromFile(fileName.c_str(), type))
        {
            shaderAsmTmp->Invalidate();
            res = false;

            continue;
        }

        shaderAsmTmp->Invalidate();

        shader->Invalidate();

        DX_ASSERT(shader->LoadFromFile( fileName.c_str(), type ));
    }

    shaderAsmTmp->LoadFromFile("no file", IAsmShader::VERTEX_SHADER);
    delete shaderAsmTmp;

    HlslShaderImpl* shaderHlslTmp = new HlslShaderImpl();

    for (std::map<ShaderIdData, HlslShaderImpl*>::iterator it = m_HlslShaders.begin(); it != m_HlslShaders.end();
         ++it)
    {
        HlslShaderImpl* shader = it->second;
        CStr const& fileName = shader->m_fileName;
        CStr const& entryName = shader->m_entryPoint;
        IHlslShader::Profile profile = shader->m_profile;
        std::vector<CompileParam> const& compileParams = shader->m_compileParams;

        if (!shaderHlslTmp->LoadFromFile(fileName.c_str(), entryName.c_str(), profile, compileParams))
        {
            shaderHlslTmp->Invalidate();
            res = false;

            continue;
        }

        shaderHlslTmp->Invalidate();

        shader->Invalidate();

        DX_ASSERT(shader->LoadFromFile( fileName.c_str(), entryName.c_str(), profile, compileParams ));
    }

    shaderHlslTmp->LoadFromFile("no file", "with this name", IHlslShader::PS_2_0, std::vector<CompileParam>());
    delete shaderHlslTmp;

    EffectImpl* effectTmp = new EffectImpl(true);

    for (std::map<ShaderIdData, EffectImpl*>::iterator it = m_effects.begin(); it != m_effects.end(); ++it)
    {
        EffectImpl* effect = it->second;
        CStr const& fileName = effect->m_fileName;
        std::vector<CompileParam> const& compileParams = effect->m_compileParams;
        unsigned int curTech = effect->GetCurTechnique();

        if (!effectTmp->LoadFromFile(fileName.c_str(), compileParams))
        {
            effectTmp->Invalidate();
            res = false;
        }
        else
        {
            effectTmp->Invalidate();

            effect->Invalidate();

            DX_ASSERT(effect->LoadFromFile( fileName.c_str(), compileParams ));
        }

        effect->SetCurTechnique(curTech);
    }

    effectTmp->LoadFromFile("no file", std::vector<CompileParam>());
    delete effectTmp;

    if (res)
    {
        LogMsg("d3d: Reloaded shaders ok");
    }

    return res;
}

// orig 0x63c5d0 shaders.cpp:218
void CDevice::createD3DXMacros()
{
    m_d3dxMacros.resize(m_shadersMacros.size());
    for (unsigned int i = 0; i < m_shadersMacros.size(); i++)
    {
        m_d3dxMacros[i].Name = m_shadersMacros[i].macro.name.c_str();
        m_d3dxMacros[i].Definition = m_shadersMacros[i].macro.definition.c_str();
    }

    D3DXMACRO finishTag;
    finishTag.Name = 0;
    finishTag.Definition = 0;
    m_d3dxMacros.push_back(finishTag);
}

// orig 0x63c860 shaders.cpp:233
void CDevice::addMacro(CStr const& name, CStr const& definition, bool userDefined)
{
    MacroData md;
    md.macro.name = name;
    md.macro.definition = definition;
    md.userDefined = userDefined;
    m_shadersMacros.push_back(md);
}

// orig 0x63c910 shaders.cpp:243
void CDevice::loadShadersMacros()
{
    m_shadersMacros.clear();

    for (unsigned int i = 0; i < SHADERS_MACROS_NUM; i++)
    {
        CStr macro(g_kernel->GetEngineCfg().m_r_shadersMacros[i].GetS());
        if (!macro.empty())
        {
            std::vector<CStr> macroTokens;
            m3d::Tokenize(macro, macroTokens, "(), ;\t");

            if (macroTokens.size() == 2)
                addMacro(macroTokens[0], macroTokens[1], true);
        }
    }

    if (IsNV3x())
        addMacro("NV3x", "true", false);

    if (IsFeatureSupported((DeviceFeature)FEATURE_DEPTH_TEXTURES))
        addMacro("SUPPORT_DEPTH_TEXTURES", "true", false);

    addMacro("MAX_ANISOTROPY", CStr(GetMaxAnisotropy()), false);
    addMacro("MAX_INSTANCES", CStr((GetMaxVertexShaderConst() - 20) / 9), false);

    createD3DXMacros();
}

// orig 0x63a7b0 shaders.cpp:277
void CDevice::saveShadersMacros()
{
    unsigned int i = 0;
    for (unsigned int j = 0; j < m_shadersMacros.size(); j++)
    {
        if (m_shadersMacros[j].userDefined && !m_shadersMacros[j].macro.name.empty())
        {
            DX_ASSERT(i < SHADERS_MACROS_NUM);
            CStr macro = m_shadersMacros[j].macro.name + " " + m_shadersMacros[j].macro.definition;
            g_kernel->GetEngineCfg().m_r_shadersMacros[i].Set(macro.c_str(), false);

            i++;
        }
    }

    for (; i < SHADERS_MACROS_NUM; i++)
        g_kernel->GetEngineCfg().m_r_shadersMacros[i].Set("", false);
}

// orig 0x63cc50 shaders.cpp:297
void CDevice::AddChangeShaderMacro(ShaderMacro const* macro)
{
    unsigned int i;

    for (i = 0; i < m_shadersMacros.size(); i++)
    {
        if (macro->name == m_shadersMacros[i].macro.name)
            break;
    }

    if (i >= m_shadersMacros.size())
    {
        // a new macro
        addMacro(macro->name, macro->definition, true);
    }
    else
    {
        // only a user defined macro changes, and only to a different definition
        if (m_shadersMacros[i].userDefined && macro->definition != m_shadersMacros[i].macro.definition)
        {
            m_shadersMacros[i].macro.definition = macro->definition;
        }
        else
            return;
    }

    createD3DXMacros();

    saveShadersMacros();
}

// orig 0x63c6f0 shaders.cpp:333
void CDevice::DeleteShaderMacro(CStr const* macroName)
{
    unsigned int i;

    for (i = 0; i < m_shadersMacros.size(); i++)
    {
        if (*macroName == m_shadersMacros[i].macro.name)
            break;
    }

    if (i < m_shadersMacros.size() && m_shadersMacros[i].userDefined)
    {
        m_shadersMacros.erase(m_shadersMacros.begin() + i);

        createD3DXMacros();

        saveShadersMacros();
    }
}

// orig 0x63a020 shaders.cpp:358
HRESULT CDevice::setVertexShader(IDirect3DVertexShader9* shader)
{
    if (shader == m_curVertexShader)
        return S_OK;

    m_stats.swVertexShaders++;
    m_curVertexShader = shader;
    return m_pd3dDevice->SetVertexShader(shader);
}

// orig 0x63a060 shaders.cpp:373
HRESULT CDevice::setPixelShader(IDirect3DPixelShader9* shader)
{
    if (shader == m_curPixelShader)
        return S_OK;

    m_stats.swPixelShaders++;
    m_curPixelShader = shader;
    return m_pd3dDevice->SetPixelShader(shader);
}
