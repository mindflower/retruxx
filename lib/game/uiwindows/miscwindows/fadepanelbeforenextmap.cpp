#include "fadepanelbeforenextmap.h"

#include <config.h>
#include <core/kernel.h>
#include <core/timer.h>
#include <game/m3dgame.h>
#include <game/uimanager/truxxuimanager.h>
#include <m3dapp.h>
#include <ui/wndstation.h>

namespace
{
    // GameDataUpdate data types this panel listens to.
    int const DATATYPE_PLAYER_PASS_TO_MAP_FADING = 81;
    int const DATATYPE_NEW_FRAME = 89;
}  // namespace

RT_CLASS_EXPORTS_BEGIN(FadePanelBeforeNextMap)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(FadePanelBeforeNextMap);

FadePanelBeforeNextMap::FadePanelBeforeNextMap()
{
    // RVA 0x4B46B0
    m_alpha = 0.0f;
    m_fadeStartTime = 0;
    m_fadePeriod = M3D_ENGINE_CFG.m_fadingTimeBeforeNextMap.GetF();
    m_blackColor = 0xFF000000;
}

FadePanelBeforeNextMap::FadePanelBeforeNextMap(FadePanelBeforeNextMap const&)
{
    // RVA 0x4B4740 - the shipped copy constructor copies nothing from rhs and,
    // unlike the default one, leaves every member uninitialised.
}

FadePanelBeforeNextMap::~FadePanelBeforeNextMap()
{
    // RVA 0x4B4750 - the panel owns nothing of its own.
}

m3d::Object* FadePanelBeforeNextMap::Clone()
{
    // RVA 0x4B4630
    return new FadePanelBeforeNextMap(*this);
}

m3d::Object* FadePanelBeforeNextMap::CreateObject()
{
    return new FadePanelBeforeNextMap;
}

m3d::Class* FadePanelBeforeNextMap::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* FadePanelBeforeNextMap::GetClass() const
{
    return RT_CLASS_LOCAL(FadePanelBeforeNextMap);
}

int FadePanelBeforeNextMap::GameDataUpdate(void*, int dataType)
{
    // RVA 0x4B4760
    if (dataType == DATATYPE_PLAYER_PASS_TO_MAP_FADING)
    {
        OnPlayerPassToMapFading();
    }
    else if (dataType == DATATYPE_NEW_FRAME)
    {
        OnNewFrame();
    }
    return 1;
}

void FadePanelBeforeNextMap::OnPlayerPassToMapFading()
{
    // RVA 0x4B4790 - the shipped code inlines StartFading() here.
    M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, true, false, false, false, nullptr);
    StartFading();
}

void FadePanelBeforeNextMap::StartFading()
{
    // RVA 0x4B48A0
    m_fadeStartTime = M3D_KERNEL->GetTimer().GetCurTime();
    m_alpha = 0.0f;
    if (GetParent())
    {
        GetParent()->MoveChildToFirstPosition(this);
    }
    GetStation()->SetCursorShow(false);
}

void FadePanelBeforeNextMap::OnNewFrame()
{
    // RVA 0x4B4800 - folded onto UpdateAlpha in the shipped build.
    UpdateAlpha();
}

void FadePanelBeforeNextMap::UpdateAlpha()
{
    // RVA 0x4B4850 - no guard against m_fadePeriod being zero; the clamp in
    // CalcCurColor deals with whatever comes out.
    m_alpha = static_cast<float>(
        static_cast<double>(M3D_KERNEL->GetTimer().GetCurTime() - m_fadeStartTime) * 0.001 / m_fadePeriod * 255.0);
}

unsigned FadePanelBeforeNextMap::CalcCurColor() const
{
    // RVA 0x4B4950 - the fade colour with the current alpha in its top byte.
    int alpha = static_cast<int>(m_alpha);
    if (alpha < 1)
    {
        // NOTE: the lower bound really is 1, not 0 - the panel never goes fully
        // transparent on its own.
        alpha = 1;
    }
    else if (alpha > 255)
    {
        alpha = 255;
    }
    return (m_blackColor & 0x00FFFFFFu) | (static_cast<unsigned>(alpha) << 24);
}

int FadePanelBeforeNextMap::OnPaint(m3d::ui::DrawInfo const& di)
{
    // RVA 0x4B48F0
    BoundsBase<float> rect;
    rect.x0 = 0.0f;
    rect.y0 = 0.0f;
    rect.width = m_bounds.width;
    rect.height = m_bounds.height;
    GetGfxServer()->AddFlatAxialQuad(di, rect, CalcCurColor());
    return 1;
}
