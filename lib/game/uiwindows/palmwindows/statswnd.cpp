#include "statswnd.h"
#include <game/uiwindows/commonwindows/itemmodelwnd.h>

RT_CLASS_EXPORTS_BEGIN(StatsButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(StatsButton);

int StatsButton::SetUpForStats(CStr const&, PointBase<float> const&, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void StatsButton::ClearValue()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* StatsButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(ButtonWnd);
}

int StatsButton::UpdateValue()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* StatsButton::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void StatsButton::SetBounds(BoundsBase<float> const&, BoundsBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& StatsButton::GetStatsName() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

BoundsBase<float> const& StatsButton::GetVirtualBounds() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* StatsButton::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

StatsButton::~StatsButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* StatsButton::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void StatsButton::SetBounds(BoundsBase<float> const&, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

StatsButton::StatsButton(StatsButton const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

StatsButton::StatsButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr StatsButton::GetStatsFullName() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr StatsButton::GetHackedDefaultValueForStats(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(StatsList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(StatsList);

m3d::Class* StatsList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int StatsList::GetButtonIdByStatsName(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* StatsList::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* StatsList::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void StatsList::ClearStats()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int StatsList::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void StatsList::UpdateStats()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* StatsList::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

StatsList::~StatsList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int StatsList::AddButtonByStatsName(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

StatsList::StatsList(StatsList const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

StatsList::StatsList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int StatsList::MeasureItem(int, BoundsBase<float>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int StatsList::RenderItem(int, PointBase<float> const&, m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int StatsList::CompareItem(int, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int StatsList::InitStats()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int StatsList::DeleteItem(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(StatsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(StatsWnd);

StatsWnd::AuxInfo::AuxInfo()
{
    m_statsListName = "statsList";
    m_wndPlayerDizName = "wndPlayerDiz";
    m_wndPlayerPortraitName = "wndPlayerPortrait";
}

m3d::Class* StatsWnd::GetClass() const
{
    return RT_CLASS_LOCAL(StatsWnd);
}

m3d::Object* StatsWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

StatsWnd::~StatsWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* StatsWnd::CreateObject()
{
    return new StatsWnd;
}

m3d::Class* StatsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ScreenWnd);
}

StatsWnd::StatsWnd(StatsWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

StatsWnd::StatsWnd()
{
    m_statsList = 0;
    m_wndPlayerDiz = 0;
}

void StatsWnd::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int StatsWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int StatsWnd::GameDataUpdate(void*, int)
{
    // TODO: implement StatsWnd::GameDataUpdate
    // RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

void StatsWnd::OnStartLevel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void StatsWnd::UpdatePlayerPortrait()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void StatsWnd::UpdatePlayerDiz()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void StatsWnd::UpdatePlayerPortraitAnmation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int StatsWnd::OnAfterAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int StatsWnd::GameDataSetup()
{
    // TODO: implement StatsWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

void StatsWnd::UpdateStats()
{
    RETRUXX_NOT_IMPLEMENTED;
}
