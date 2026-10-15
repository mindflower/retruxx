#pragma once
// Ported from the original dxrender9/shaders/hlsl/hlsl_shader.h: a stand-alone HLSL vertex or
// pixel shader compiled with D3DXCompileShader.
#include <d3d9.h>
#include <d3dx9.h>

#include <vector>

#include <core/stringm3d.h>
#include <renderer/i_renderer.h>

#include "i_renderer_orig.h"

class CDevice;

class HlslShaderImpl : public m3d::rend::IHlslShader
{
public:
    unsigned int m_numConstants;
    CStr m_fileName;
    std::vector<m3d::rend::CompileParam> m_compileParams;
    CStr m_entryPoint;
    Profile m_profile;
    union
    {
        void* m_shader;
        IDirect3DVertexShader9* m_vs;
        IDirect3DPixelShader9* m_ps;
    };
    ID3DXConstantTable* m_constantTable;
    unsigned int m_numPrimitives;

    static CDevice* m_dev;

    HlslShaderImpl();
    virtual ~HlslShaderImpl();

    bool LoadFromFile(
        char const* fileName,
        char const* entryPoint,
        Profile profile,
        std::vector<m3d::rend::CompileParam> const& compileParams);
    bool LoadFromString(char const* str, unsigned int length, char const* entryPoint, Profile profile);

    // IRenderResource
    bool IsValid() const override;

    // IHlslShader
    unsigned int GetNumberOfParams() const override;
    ParameterHandle GetParamHandleByName(char const* name) override;
    void SetInt(ParameterHandle param, int val) override;
    void SetFloat(ParameterHandle param, float val) override;
    void SetVector4(ParameterHandle param, CVector4 const& val) override;
    void SetVector3(ParameterHandle param, CVector const& val) override;
    void SetFloat4(ParameterHandle param, nFloat4 const& val) override;
    void SetMatrix(ParameterHandle param, CMatrix const& val) override;
    void SetIntArray(ParameterHandle param, int const* vals, int count) override;
    void SetFloatArray(ParameterHandle param, float const* vals, int count) override;
    void SetFloat4Array(ParameterHandle param, nFloat4 const* vals, int count) override;
    void SetVector4Array(ParameterHandle param, CVector4 const* vals, int count) override;
    void SetMatrixArray(ParameterHandle param, CMatrix const* vals, int count) override;
    void SetMatrixPointerArray(ParameterHandle param, CMatrix const** vals, int count) override;
    void Apply() override;

    void UpdateShaderInfo();
    bool IsValidParam(ParameterHandle param) const;
    void ValidateEffect();
    void OnDeviceReset();
    void OnDeviceRestore();
    void Invalidate();
    bool IsVertexShader() const;
};
