#include "targetcapturingcursorwnd.h"

#include "config.h"
#include "core/kernel.h"
#include "core/log.h"
#include "m3dapp.h"
#include "core/timer.h"

#include "game/m3dgame.h"
#include "game/uimanager/truxxuimanager.h"

#include "server/objects/base/globalproperties.h"
#include "server/objects/player.h"
#include "server/objects/vehicle.h"

RT_CLASS_EXPORTS_BEGIN(TargetCapturingCursorWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(TargetCapturingCursorWnd);

TargetCapturingCursorWnd::AuxInfo::AuxInfo()
{
    // RVA 0x134330
    m_texName = "CursorCapture";
    m_texCaptureSzBig = PointBase<float>{140.0f, 140.0f};
    m_texCaptureSzSmall = PointBase<float>{36.0f, 36.0f};
}

m3d::Class* TargetCapturingCursorWnd::GetClass() const
{
    // RVA 0x1342B0
    return RT_CLASS_LOCAL(TargetCapturingCursorWnd);
}

TargetCapturingCursorWnd::~TargetCapturingCursorWnd()
{
    // RVA 0x134480 - TargetCursorWnd/SmartCursorWnd destructors run automatically.
    M3D_RENDERER->ReleaseTexture(m_texCapture);
}

CStr const& TargetCapturingCursorWnd::GetCaptureTexName()
{
    // RVA 0x134880
    return m_aif.m_texName;
}

float TargetCapturingCursorWnd::GetCapturingTexAngle()
{
    // RVA 0x134870
    return m_captureTexAngle;
}

m3d::Class* TargetCapturingCursorWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(TargetCursorWnd);
}

m3d::Object* TargetCapturingCursorWnd::CreateObject()
{
    // RVA 0x1342C0
    return new TargetCapturingCursorWnd;
}

PointBase<float> const& TargetCapturingCursorWnd::GetCaptureTexSmallSz()
{
    // RVA 0x134890
    return m_aif.m_texCaptureSzSmall;
}

m3d::Object* TargetCapturingCursorWnd::Clone()
{
    // RVA 0x134200
    return new TargetCapturingCursorWnd(*this);
}

int TargetCapturingCursorWnd::GetTargetObjId() const
{
    // RVA 0x134850
    if (ai::thePlayer && ai::thePlayer->GetVehicle())
    {
        return ai::thePlayer->GetVehicle()->GetToBeLockedObjId();
    }
    return -1;
}

TargetCapturingCursorWnd::TargetCapturingCursorWnd()
{
    // RVA 0x1343A0
}

TargetCapturingCursorWnd::TargetCapturingCursorWnd(TargetCapturingCursorWnd const&) : TargetCapturingCursorWnd()
{
    // Matches the original: the copy ctor default-constructs the base and an
    // invalid capture texture handle; nothing is copied from the source.
}

void TargetCapturingCursorWnd::UpdateCaptureTexAngle()
{
    // RVA 0x134780
    if (ai::thePlayer && ai::thePlayer->GetVehicle())
    {
        m_captureTexAngle += M3D_KERNEL->GetTimer().GetLastFrameTimeUnscaled() * CAPTURE_ROTATION_SPEED * 0.001f;
    }
}

void TargetCapturingCursorWnd::OnNewFrame()
{
    // RVA 0x134580
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

void TargetCapturingCursorWnd::UpdateCaptureTex()
{
    // RVA 0x1345D0
    // NOTE: the shipped build gates this behind a vtable call taking only
    // `this` and returning bool; NeedShow() is the only virtual on this
    // branch of the hierarchy matching that shape.
    if (NeedShow())
    {
        UpdateCaptureTexAngle();

        auto* aimDi = new SmartCursorWnd::AuxDrawInfo();
        aimDi->SetTexture(m_texCapture);
        aimDi->m_sz = GetCaptureTexSize();
        aimDi->m_angle = m_captureTexAngle;
        aimDi->m_color = (static_cast<unsigned>(GetCaptureTexAlpha()) << 24) | 0xFFFFFFu;
        m_drawInfo.push_back(aimDi);
    }
}

int TargetCapturingCursorWnd::GameDataSetup()
{
    // RVA 0x1344C0
    if ((m_gameDataFlags & 2) == 0)
    {
        m_texCapture = M3D_APP->m_pInterfaceManager->GetIcoByName(GetCaptureTexName(), 0);
        M3D_RENDERER->ReferenceTexture(m_texCapture);
        m_gameDataFlags |= 1u;
    }
    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("TargetCapturingCursorWnd: error - fail to init because of a bad resource");
    return 0;
}

PointBase<float> TargetCapturingCursorWnd::GetCaptureTexSize() const
{
    // RVA 0x1346C0
    if (!ai::thePlayer || !ai::thePlayer->GetVehicle())
    {
        return PointBase<float>{0.0f, 0.0f};
    }

    ai::Vehicle* vehicle = ai::thePlayer->GetVehicle();
    float const t = vehicle->GetTimeToLockTarget() / ai::theGlobProp.m_lockTimeout;

    float x = m_aif.m_texCaptureSzBig.x + ((m_aif.m_texCaptureSzSmall.x - m_aif.m_texCaptureSzBig.x) * t);
    float y = m_aif.m_texCaptureSzBig.y + ((m_aif.m_texCaptureSzSmall.y - m_aif.m_texCaptureSzBig.y) * t);

    if (m_aif.m_texCaptureSzSmall.x > x)
        x = m_aif.m_texCaptureSzSmall.x;
    if (x > m_aif.m_texCaptureSzBig.x)
        x = m_aif.m_texCaptureSzBig.x;
    if (m_aif.m_texCaptureSzSmall.y > y)
        y = m_aif.m_texCaptureSzSmall.y;
    if (y > m_aif.m_texCaptureSzBig.y)
        y = m_aif.m_texCaptureSzBig.y;

    return PointBase<float>{x, y};
}

unsigned char TargetCapturingCursorWnd::GetCaptureTexAlpha() const
{
    // RVA 0x1347D0
    if (!ai::thePlayer || !ai::thePlayer->GetVehicle())
    {
        return 0;
    }

    ai::Vehicle* vehicle = ai::thePlayer->GetVehicle();
    float const alpha = ((vehicle->GetTimeToLockTarget() / ai::theGlobProp.m_lockTimeout) * 254.0f) + 1.0f;
    if (alpha < 1.0f)
    {
        return 1;
    }
    if (alpha > 255.0f)
    {
        return 255;
    }
    return static_cast<unsigned char>(alpha);
}
