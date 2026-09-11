#include "cinemafadepanel.h"

#include <cinematic.h>
#include <core/kernel.h>
#include <core/timer.h>
#include <game/m3dgame.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimanager/uidefs.h>
#include <m3dapp.h>
#include <ui/wndstation.h>

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

m3d::Class* CinemaFadePanel::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

CinemaFadePanel::CinemaFadePanel()
{
    // RVA 0x4ACFC0
    BoundsBase<float> rc{0.0f, 0.0f, 1024.0f, 768.0f};
    CreateWnd(CStr(), 0x300, rc, 0);
    SetCursorShow(false);
    m_fadePeriod = M3D_APP->m_cinematic->m_fadePeriod.GetF();
    m_isCinematicRelated = true;
    m_fadeStart = -1;
    m_curAlpha = 255.0f;
    m_state = FS_IS_WHITE;
    m_isFading = false;
}

CinemaFadePanel::CinemaFadePanel(CinemaFadePanel const&)
{
    // RVA 0x4AD0E0 - the shipped copy constructor is an assert(0); the panel is
    // a singleton owned by the interface manager and cannot be duplicated.
    RETRUXX_NOT_IMPLEMENTED;
}

CinemaFadePanel::~CinemaFadePanel()
{
    // RVA 0x4AD110 - the panel owns nothing of its own.
}

m3d::Object* CinemaFadePanel::Clone()
{
    // RVA 0x4ACF20 - allocates, runs the asserting copy constructor and returns
    // null; cloning this panel is not supported.
    RETRUXX_NOT_IMPLEMENTED;
}

float CinemaFadePanel::GetAlpha()
{
    // RVA 0x4AD120 - how far the fade has run, expressed as an alpha in
    // [1, 255]. A cinematic-driven fade follows the cinematic clock and inverts
    // while the cinematic is fading in; a standalone fade follows the unscaled
    // engine clock and inverts while fading up.
    int fadeStartTime = 0;
    unsigned playTime = 0;
    if (m_isCinematicRelated)
    {
        fadeStartTime = M3D_APP->m_cinematic->m_fadeStartTime;
        playTime = static_cast<unsigned>(M3D_APP->m_cinematic->m_playTime);
    }
    else
    {
        fadeStartTime = m_fadeStart;
        playTime = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
    }

    // NOTE: no guard against m_fadePeriod being zero - the shipped code divides
    // regardless and lets the clamp below deal with the result.
    float const elapsed = static_cast<float>(
        static_cast<double>(playTime - static_cast<unsigned>(fadeStartTime)) / (m_fadePeriod * 1000.0f) * 255.0);

    float alpha = elapsed;
    if (m_isCinematicRelated)
    {
        m3d::CinematicState const state = M3D_APP->m_cinematic->m_state;
        if (state == m3d::CINEMATIC_ENTER_FADE_IN || state == m3d::CINEMATIC_EXIT_FADE_IN)
        {
            alpha = 255.0f - elapsed;
        }
    }
    else if (m_state == FS_FADING_UP)
    {
        alpha = 255.0f - elapsed;
    }

    // NOTE: the lower bound really is 1, not 0 - the panel never goes fully
    // transparent on its own.
    if (1.0f > alpha)
    {
        return 1.0f;
    }
    if (alpha > 255.0f)
    {
        alpha = 255.0f;
    }
    return alpha;
}

int CinemaFadePanel::OnPaint(m3d::ui::DrawInfo const& di)
{
    // RVA 0x4AD200
    int const fadeStartTime = m_isCinematicRelated ? M3D_APP->m_cinematic->m_fadeStartTime : m_fadeStart;
    unsigned const curTime = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();

    float const alpha = GetAlpha();
    m_curAlpha = alpha;

    bool draw = alpha > 1.0f && alpha < 255.0f;
    if (!draw)
    {
        draw = curTime == static_cast<unsigned>(fadeStartTime);
        if (!draw)
        {
            // The fade has run its course.
            m_isFading = false;
            draw = alpha > 1.0f || m_isCinematicRelated;
        }
    }

    if (!draw)
    {
        // A standalone fade that has faded all the way back up takes the panel
        // off the screen and hands the mouse back.
        M3D_APP->m_pInterfaceManager->ShowWindow(IW_WND_CINEMA_FADE_PANEL, false, false, false, false, nullptr);
        GetStation()->SetCursorShow(true);
        GetStation()->CaptureMouse(nullptr);
        return 1;
    }

    BoundsBase<float> rect;
    rect.x0 = 0.0f;
    rect.y0 = 0.0f;
    rect.width = m_bounds.width;
    rect.height = m_bounds.height;
    GetGfxServer()->AddFlatAxialQuad(di, rect, static_cast<int>(alpha) << 24);
    return 1;
}

void CinemaFadePanel::AttachToScreenCinematicRelated()
{
    // RVA 0x4AD350
    M3D_APP->m_pInterfaceManager->ShowWindow(IW_WND_CINEMA_FADE_PANEL, true, true, false, false, nullptr);
    if (GetParent())
    {
        GetParent()->MoveChildToFirstPosition(this);
    }
    m_isCinematicRelated = true;
    m_fadePeriod = M3D_APP->m_cinematic->m_fadePeriod.GetF();
    m_fadeStart = M3D_APP->m_cinematic->m_fadeStartTime;
    m_isFading = true;
}

void CinemaFadePanel::StartFadeCinematicUnrelated(float fadePeriod, int direction)
{
    // RVA 0x4AD3C0 - a fade that is not driven by a cinematic; it grabs the
    // mouse and hides the cursor for its duration.
    if (direction == 0)
    {
        m_state = FS_FADING_UP;
    }
    else if (direction == 1)
    {
        m_state = FS_FADING_DOWN;
    }

    M3D_APP->m_pInterfaceManager->ShowWindow(IW_WND_CINEMA_FADE_PANEL, true, true, false, false, nullptr);
    m_isCinematicRelated = false;
    m_fadePeriod = fadePeriod;
    m_fadeStart = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
    m_isFading = true;
    GetStation()->CaptureMouse(M3D_APP);
    GetStation()->SetCursorShow(false);
}

void CinemaFadePanel::FadeUp(float fadePeriod)
{
    // NOTE: declared by the shipped headers but eliminated from the binary as
    // unreferenced; reconstructed from the direction constants that
    // StartFadeCinematicUnrelated maps onto FS_FADING_UP / FS_FADING_DOWN.
    StartFadeCinematicUnrelated(fadePeriod, FS_FADING_UP);
}

void CinemaFadePanel::FadeDown(float fadePeriod)
{
    // See the note on FadeUp.
    StartFadeCinematicUnrelated(fadePeriod, FS_FADING_DOWN);
}

bool CinemaFadePanel::IsFading()
{
    // Also eliminated from the shipped binary as unreferenced.
    return m_isFading;
}
