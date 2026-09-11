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

    void overlayStop()
    {
        // RVA 0x75EDB0 - the exact counterpart of overlayStart. NOTE: the pops
        // are NOT in reverse order of the pushes - ZFunc and ZbState are
        // swapped - but each renderer state has its own stack, so the pairing
        // still works out.
        M3D_RENDERER->PopZFunc();
        M3D_RENDERER->PopZbState();
        M3D_RENDERER->PopZBiasSlopeScale();
        M3D_RENDERER->PopZBias();
    }
}  // namespace m3d
