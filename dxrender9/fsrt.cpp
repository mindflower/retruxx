// The full-screen render target and the full-screen quad (the original dxrender9/fsrt.cpp).
//
// In the original the full-screen render target code (CreateFsRt / ReleaseFsRt / DrawFsRt, the m_fsRt*
// members) is compiled out: the three functions are empty, and the file statics below are never
// referenced.
#include <renderer/i_renderer_vertex.h>

#include "device.h"
#include "log.h"

namespace
{
    // orig data 0x89c004 / 0x852710 (fsrt.cpp file statics, unused)
    IDirect3DVertexDeclaration9* m_vdRtTex;
    TexHandle g_Noise;

    // The original's m3d::rend::VertexXYZT1 (engine/renderer/i_renderer_vertex.h). This project's
    // i_renderer_vertex.h has the VERTEX_XYZT1 type but no struct for it, so the vertex layout the
    // quad is written with lives here.
    struct VertexXYZT1
    {
        float x;
        float y;
        float z;
        float tu;
        float tv;

        // orig 0x634060 i_renderer_vertex.h:58
        void xyz(float xx, float yy, float zz)
        {
            x = xx;
            y = yy;
            z = zz;
        }

        // orig 0x634090 i_renderer_vertex.h:60
        void uv0(float u, float v)
        {
            tu = u;
            tv = v;
        }
    };
}

// orig 0x6340b0 fsrt.cpp:50
void CDevice::CreateFsRt()
{
}

// orig 0x6340c0 fsrt.cpp:197
void CDevice::ReleaseFsRt()
{
}

// orig 0x634400 fsrt.cpp:212
void CDevice::initFullScreenQuad()
{
    m_fsQuadVb = AddVbStreaming(VERTEX_XYZT1, 4, 0);

    m_fsQuadIb = AddIb(4, false);
    unsigned short* ind = (unsigned short*)LockIb(m_fsQuadIb, 0, 0, 0);
    ind[0] = 0;
    ind[1] = 1;
    ind[2] = 2;
    ind[3] = 3;
    UnlockIb(m_fsQuadIb);
}

// orig 0x6340d0 fsrt.cpp:265
void CDevice::doneFullScreenQuadStuff()
{
    ReleaseVb(m_fsQuadVb);
    ReleaseIb(m_fsQuadIb);
}

// orig 0x634100 fsrt.cpp:276
void CDevice::DrawFullScreenQuad(bool useShader)
{
    float du = 0.5f / m_curViewportD3D.Width;
    float dv = 0.5f / m_curViewportD3D.Height;

    int ofs;
    VertexXYZT1* v = (VertexXYZT1*)LockVbStreaming(m_fsQuadVb, 4, ofs, 0);

    v[0].xyz(1.0f, -1.0f, 0.0f);
    v[0].uv0(du + 1.0f, dv + 1.0f);

    v[1].xyz(1.0f, 1.0f, 0.0f);
    v[1].uv0(du + 1.0f, dv);

    v[2].xyz(-1.0f, -1.0f, 0.0f);
    v[2].uv0(du, dv + 1.0f);

    v[3].xyz(-1.0f, 1.0f, 0.0f);
    v[3].uv0(du, dv);

    UnlockVb(m_fsQuadVb);

    SetToStream0(m_fsQuadVb);
    SetIndices(m_fsQuadIb, 0);

    if (useShader)
    {
        DrawIndexedPrimitiveShader(M3DPT_TRIANGLESTRIP, 0, 4, 0, 2);
    }
    else
    {
        DrawIndexedPrimitive(M3DPT_TRIANGLESTRIP, 0, 4, 0, 2);
    }
}

// orig 0x634280 fsrt.cpp:316
void CDevice::DrawFullScreenQuad(IEffect* shader)
{
    float du = 0.5f / m_curViewportD3D.Width;
    float dv = 0.5f / m_curViewportD3D.Height;

    int ofs;
    VertexXYZT1* v = (VertexXYZT1*)LockVbStreaming(m_fsQuadVb, 4, ofs, 0);

    v[0].xyz(1.0f, -1.0f, 0.0f);
    v[0].uv0(du + 1.0f, dv + 1.0f);

    v[1].xyz(1.0f, 1.0f, 0.0f);
    v[1].uv0(du + 1.0f, dv);

    v[2].xyz(-1.0f, -1.0f, 0.0f);
    v[2].uv0(du, dv + 1.0f);

    v[3].xyz(-1.0f, 1.0f, 0.0f);
    v[3].uv0(du, dv);

    UnlockVb(m_fsQuadVb);

    SetToStream0(m_fsQuadVb);
    SetIndices(m_fsQuadIb, 0);

    DrawIndexedPrimitiveEffect(M3DPT_TRIANGLESTRIP, shader, 0, 4, 0, 2);
}

// HTA-only wrapper, no original body: the engine applies its vertex/pixel shaders before calling it.
void CDevice::DrawFullScreenQuad()
{
    DrawFullScreenQuad(true);
}

// orig 0x6343f0 fsrt.cpp:351
void CDevice::DrawFsRt()
{
}
