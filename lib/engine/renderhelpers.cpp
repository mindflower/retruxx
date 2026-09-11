#include <m3dapp.h>
#include <renderer/i_renderer.h>

namespace m3d
{
    void overlayStart()
    {
        // RVA 0x75ED50
        M3D_RENDERER->PushZbState(rend::ZB_NOWRITE);
        M3D_RENDERER->PushZFunc(rend::M3DCMP_LESSEQUAL);
        M3D_RENDERER->PushZBiasSlopeScale(-0.1f);
        M3D_RENDERER->PushZBias(0.0f);
    }
}  // namespace m3d
