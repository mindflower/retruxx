#include "cinemafadepanel.h"
#include <cinematic.h>
#include <m3dapp.h>

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
    throw std::logic_error("Not implemented");
}

m3d::Class* CinemaFadePanel::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

CinemaFadePanel::~CinemaFadePanel()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* CinemaFadePanel::Clone()
{
    throw std::logic_error("Not implemented");
}

void CinemaFadePanel::StartFadeCinematicUnrelated(float, int)
{
    throw std::logic_error("Not implemented");
}

float CinemaFadePanel::GetAlpha()
{
    throw std::logic_error("Not implemented");
}

int CinemaFadePanel::OnPaint(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

CinemaFadePanel::CinemaFadePanel()
{
    BoundsBase<float> rc{0.0, 0.0, 0.0, 0.0};
    rc.width = 1024.0;
    rc.height = 768.0;
    CreateWnd("", 0x300, rc, 0);
    SetCursorShow(false);
    m_fadePeriod = m3d::Application::g_pApp->m_cinematic->m_fadePeriod.GetF();
}

CinemaFadePanel::CinemaFadePanel(CinemaFadePanel const&)
{
    throw std::logic_error("Not implemented");
}
