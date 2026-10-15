#pragma once
// Ported from the original dxrender9/shaders/assembly/asm_shader.h: a shader assembled from
// vs_x_x / ps_x_x assembly with D3DXAssembleShader.
#include <d3d9.h>

#include <core/stringm3d.h>
#include <renderer/i_renderer.h>

class CDevice;

class AsmShaderImpl : public m3d::rend::IAsmShader
{
public:
    CStr m_fileName;
    Type m_type;
    union
    {
        void* m_shader;
        IDirect3DVertexShader9* m_vs;
        IDirect3DPixelShader9* m_ps;
    };

    static CDevice* m_dev;

    AsmShaderImpl();
    virtual ~AsmShaderImpl();

    bool LoadFromFile(char const* fileName, Type type);
    bool LoadFromString(char const* str, unsigned int length, Type type);

    // IRenderResource
    bool IsValid() const override;

    // IAsmShader
    void Apply() override;

    void OnDeviceReset();
    void OnDeviceRestore();
    void Invalidate();
    bool IsVertexShader() const;
};
