#include "cinemafadepanel.h"
#include <cinematic.h>
#include <m3dapp.h>
#include <core/kernel.h>
#include <core/timer.h>
#include <game/m3dgame.h>

RT_CLASS_EXPORTS_BEGIN(CinemaFadePanel)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CinemaFadePanel);

m3d::Object* CinemaFadePanel::CreateObject()
{
    return new CinemaFadePanel;
}

m3d::Class* CinemaFadePanel::GetClass() const
{
    return RT_CLASS_LOCAL(CinemaFadePanel);
}

void CinemaFadePanel::AttachToScreenCinematicRelated()
{
    M3D_APP->m_pInterfaceManager->ShowWindow(19, true, true, false, false, nullptr);
    if (GetParent())
    {
        GetParent()->MoveChildToFirstPosition(this);
    }
    m_isCinematicRelated = true;

    auto fadePeriod = M3D_APP->m_cinematic->m_fadePeriod.GetF();
    m_fadePeriod = fadePeriod;
    m_fadeStart = M3D_APP->m_cinematic->m_fadeStartTime;
    m_isFading = 1;
}

m3d::Class* CinemaFadePanel::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

CinemaFadePanel::~CinemaFadePanel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* CinemaFadePanel::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaFadePanel::StartFadeCinematicUnrelated(float, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

float CinemaFadePanel::GetAlpha()
{
    int fadeStartTime = 0;
    if (m_isCinematicRelated)
    {
        fadeStartTime = M3D_APP->m_cinematic->m_fadeStartTime;
    }
    else
    {
        fadeStartTime = m_fadeStart;
    }

    int playTime = 0;
    if (m_isCinematicRelated)
    {
        playTime = m3d::Application::g_pApp->m_cinematic->m_playTime;
    }
    else
    {
        playTime = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
    }
    return (double)(unsigned int)(playTime - fadeStartTime) / (m_fadePeriod * 1000.0) * 255.0;
}

int CinemaFadePanel::OnPaint(m3d::ui::DrawInfo const& di)
{
    int fadeStartTime = 0;
    if (m_isCinematicRelated)
    {
        fadeStartTime = M3D_APP->m_cinematic->m_fadeStartTime;
    }
    else
    {
        fadeStartTime = m_fadeStart;
    }

    auto curTime = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
    this->m_curAlpha = GetAlpha();
    if (m_curAlpha > 1.0 && m_curAlpha < 255.0
        || curTime == fadeStartTime
        || (this->m_isFading = 0, m_curAlpha > 1.0)
        || this->m_isCinematicRelated)
    {
        BoundsBase<float> rect;
        rect.x0 = 0.0;
        rect.y0 = 0.0;
        rect.width = this->m_bounds.width;
        rect.height = this->m_bounds.height;
        GetGfxServer()->AddFlatAxialQuad(di, rect, (int)m_curAlpha << 24);
    }
    else
    {
        M3D_APP->m_pInterfaceManager->ShowWindow(19, false, false, false, false, nullptr);
        GetStation()->SetCursorShow(true);
        GetStation()->CaptureMouse(nullptr);
    }
    return 1;
}

CinemaFadePanel::CinemaFadePanel()
{
    BoundsBase<float> rc{0.0, 0.0, 0.0, 0.0};
    rc.width = 1024.0;
    rc.height = 768.0;
    CreateWnd("", 0x300, rc, 0);
    SetCursorShow(false);
    m_fadePeriod = m3d::Application::g_pApp->m_cinematic->m_fadePeriod.GetF();
    m_isCinematicRelated = 1;
    m_fadeStart = -1;
    m_curAlpha = 255.0;
    m_state = FS_IS_WHITE;
    m_isFading = 0;
}

CinemaFadePanel::CinemaFadePanel(CinemaFadePanel const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
