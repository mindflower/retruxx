#include "cinemafadepanel.h"

#include <algorithm>
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
    // TODO: check this!!!
    int fadeStartTime = 0;
    int playTime = 0;
    if (m_isCinematicRelated)
    {
        fadeStartTime = M3D_APP->m_cinematic->m_fadeStartTime;
        playTime = m3d::Application::g_pApp->m_cinematic->m_playTime;
    }
    else
    {
        fadeStartTime = m_fadeStart;
        playTime = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
    }

    unsigned int const timeDiff = static_cast<unsigned int>(playTime) - static_cast<unsigned int>(fadeStartTime);

    float const duration = m_fadePeriod;
    if (duration <= 0.0f)
    {
        if (m_isCinematicRelated)
        {
            return (M3D_APP->m_cinematic->m_state == 1 || M3D_APP->m_cinematic->m_state == 4) ? 0.0f : 255.0f;
        }
        return m_state == FS_FADING_UP ? 0.0f : 255.0f;
    }

    float calculatedAlpha = static_cast<float>(timeDiff) / (duration * 1000.0f);

    calculatedAlpha *= 255.0f;

    float alpha = 0.0;
    // Apply inversion logic based on conditions
    if (m_isCinematicRelated)
    {
        // Using application timer path
        int state = M3D_APP->m_cinematic->m_state;
        if (state == 1 || state == 4)
        {
            alpha = 255.0f - calculatedAlpha;  // Invert alpha
        }
        else
        {
            alpha = calculatedAlpha;
        }
    }
    else
    {
        // Using internal timer path
        if (m_state == 0)
        {
            alpha = 255.0f - calculatedAlpha;  // Invert alpha
        }
        else
        {
            alpha = calculatedAlpha;
        }
    }

    alpha = std::clamp(alpha, 0.0f, 255.0f);

    return alpha;
}

int CinemaFadePanel::OnPaint(m3d::ui::DrawInfo const& di)
{
    // TODO: check this!!!
    m_curAlpha = GetAlpha();
    auto const curTime = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
    auto const fadeStartTime = static_cast<unsigned int>(m_fadeStart);
    auto const fadeDuration = static_cast<double>(m_fadePeriod) * 1000.0;
    auto const fadeComplete = m_fadePeriod <= 0.0f || static_cast<double>(curTime - fadeStartTime) >= fadeDuration;
    if (fadeComplete && m_isFading && !m_isCinematicRelated)
    {
        m_isFading = false;
        M3D_APP->m_pInterfaceManager->ShowWindow(19, false, false, false, false, nullptr);
        GetStation()->SetCursorShow(true);
        GetStation()->CaptureMouse(nullptr);
        return 1;
    }

    BoundsBase<float> rect;
    rect.x0 = 0.0;
    rect.y0 = 0.0;
    rect.width = this->m_bounds.width;
    rect.height = this->m_bounds.height;
    GetGfxServer()->AddFlatAxialQuad(di, rect, (int)m_curAlpha << 24);
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
