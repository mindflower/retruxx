#include "targetcapturedcursorwnd.h"

#include "config.h"
#include "core/kernel.h"
#include "core/log.h"
#include "m3dapp.h"

#include "game/m3dgame.h"
#include "game/uimanager/truxxuimanager.h"
#include "targetcapturingcursorwnd.h"

#include "server/objects/player.h"
#include "server/objects/vehicle.h"

RT_CLASS_EXPORTS_BEGIN(TargetCapturedCursorWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(TargetCapturedCursorWnd);

m3d::Object* TargetCapturedCursorWnd::CreateObject()
{
    // RVA 0x133E20
    return new TargetCapturedCursorWnd;
}

m3d::Class* TargetCapturedCursorWnd::GetClass() const
{
    // RVA 0x133E10
    return RT_CLASS_LOCAL(TargetCapturedCursorWnd);
}

m3d::Class* TargetCapturedCursorWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(TargetCursorWnd);
}

m3d::Object* TargetCapturedCursorWnd::Clone()
{
    // RVA 0x133DA0
    return new TargetCapturedCursorWnd(*this);
}

TargetCapturedCursorWnd::~TargetCapturedCursorWnd()
{
    // RVA 0x133F50 - TargetCursorWnd/SmartCursorWnd destructors run automatically.
    M3D_RENDERER->ReleaseTexture(m_texCapture);
}

void TargetCapturedCursorWnd::OnNewFrame()
{
    // RVA 0x134070
    ClearDrawInfo();
    if (NeedShow())
    {
        Show(true);
        AccidentValidateBounds();
    }
    else
    {
        Show(false);
    }
    UpdateCaptureTex();
}

TargetCapturedCursorWnd::TargetCapturedCursorWnd(TargetCapturedCursorWnd const&) : TargetCapturedCursorWnd()
{
    // Matches the original: the copy ctor default-constructs the base and an
    // invalid capture texture handle; nothing is copied from the source.
}

TargetCapturedCursorWnd::TargetCapturedCursorWnd()
{
    // RVA 0x133E90
    // NOTE: the shipped ctor leaves m_texAngle uninitialized; it is only ever
    // read after Show(true) or UpdateTexAngle() have run, so 0 is harmless.
    m_texAngle = 0.0f;
}

bool TargetCapturedCursorWnd::bVisible() const
{
    // RVA 0x1341F0
    return (m_style & 0x200) != 0;
}

int TargetCapturedCursorWnd::GetTargetObjId() const
{
    // RVA 0x134050
    if (ai::thePlayer && ai::thePlayer->GetVehicle())
    {
        return ai::thePlayer->GetVehicle()->GetLockedObjId();
    }
    return -1;
}

void TargetCapturedCursorWnd::UpdateTexAngle()
{
    // RVA 0x1341D0
    m_texAngle = TargetCapturingCursorWnd::GetCapturingTexAngle();
}

void TargetCapturedCursorWnd::UpdateCaptureTex()
{
    // RVA 0x1340C0
    // NOTE: the shipped build gates this behind a vtable call taking only
    // `this` and returning bool; NeedShow() is the only virtual on this
    // branch of the hierarchy matching that shape.
    if (NeedShow())
    {
        auto* aimDi = new SmartCursorWnd::AuxDrawInfo();
        aimDi->SetTexture(m_texCapture);
        aimDi->m_sz = TargetCapturingCursorWnd::GetCaptureTexSmallSz();
        aimDi->m_angle = m_texAngle;
        m_drawInfo.push_back(aimDi);
    }
}

void TargetCapturedCursorWnd::Show(bool bShow)
{
    // RVA 0x134190
    if (bShow && (m_style & 0x200) == 0)
    {
        m_texAngle = TargetCapturingCursorWnd::GetCapturingTexAngle();
    }
    ShowWindow(bShow);
}

int TargetCapturedCursorWnd::GameDataSetup()
{
    // RVA 0x133F90
    if ((m_gameDataFlags & 2) == 0)
    {
        m_texCapture = M3D_APP->m_pInterfaceManager->GetIcoByName(TargetCapturingCursorWnd::GetCaptureTexName(), 0);
        M3D_RENDERER->ReferenceTexture(m_texCapture);
        m_gameDataFlags |= 1u;
    }
    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("TargetCapturedCursorWnd: error - fail to init because of a bad resource");
    return 0;
}
