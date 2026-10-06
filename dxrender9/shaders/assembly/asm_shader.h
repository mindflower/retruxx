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
    /* 0x0008 */ CStr m_fileName;
    /* 0x0014 */ Type m_type;
    union
    {
        /* 0x0018 */ void* m_shader;
        /* 0x0018 */ IDirect3DVertexShader9* m_vs;
        /* 0x0018 */ IDirect3DPixelShader9* m_ps;
    };

    static CDevice* m_dev;

    AsmShaderImpl();
    virtual ~AsmShaderImpl();

    bool LoadFromFile(const char* fileName, Type type);
    bool LoadFromString(const char* str, unsigned int length, Type type);

    // IRenderResource
    bool IsValid() const override;

    // IAsmShader
    void Apply() override;

    void OnDeviceReset();
    void OnDeviceRestore();
    void Invalidate();
    bool IsVertexShader() const;
}; /* size: 0x001c */
