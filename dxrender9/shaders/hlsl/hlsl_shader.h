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
    /* 0x0008 */ unsigned int m_numConstants;
    /* 0x000c */ CStr m_fileName;
    /* 0x0018 */ std::vector<m3d::rend::CompileParam> m_compileParams;
    /* 0x0028 */ CStr m_entryPoint;
    /* 0x0034 */ Profile m_profile;
    union
    {
        /* 0x0038 */ void* m_shader;
        /* 0x0038 */ IDirect3DVertexShader9* m_vs;
        /* 0x0038 */ IDirect3DPixelShader9* m_ps;
    };
    /* 0x003c */ ID3DXConstantTable* m_constantTable;
    /* 0x0040 */ unsigned int m_numPrimitives;

    static CDevice* m_dev;

    HlslShaderImpl();
    virtual ~HlslShaderImpl();

    bool LoadFromFile(const char* fileName, const char* entryPoint, Profile profile,
                      std::vector<m3d::rend::CompileParam> const& compileParams);
    bool LoadFromString(const char* str, unsigned int length, const char* entryPoint, Profile profile);

    // IRenderResource
    bool IsValid() const override;

    // IHlslShader
    unsigned int GetNumberOfParams() const override;
    ParameterHandle GetParamHandleByName(const char* name) override;
    void SetInt(ParameterHandle param, int val) override;
    void SetFloat(ParameterHandle param, float val) override;
    void SetVector4(ParameterHandle param, CVector4 const& val) override;
    void SetVector3(ParameterHandle param, CVector const& val) override;
    void SetFloat4(ParameterHandle param, nFloat4 const& val) override;
    void SetMatrix(ParameterHandle param, CMatrix const& val) override;
    void SetIntArray(ParameterHandle param, const int* vals, int count) override;
    void SetFloatArray(ParameterHandle param, const float* vals, int count) override;
    void SetFloat4Array(ParameterHandle param, const nFloat4* vals, int count) override;
    void SetVector4Array(ParameterHandle param, const CVector4* vals, int count) override;
    void SetMatrixArray(ParameterHandle param, const CMatrix* vals, int count) override;
    void SetMatrixPointerArray(ParameterHandle param, const CMatrix** vals, int count) override;
    void Apply() override;

    void UpdateShaderInfo();
    bool IsValidParam(ParameterHandle param) const;
    void ValidateEffect();
    void OnDeviceReset();
    void OnDeviceRestore();
    void Invalidate();
    bool IsVertexShader() const;
}; /* size: 0x0044 */
