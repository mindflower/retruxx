#include "statswnd.h"
#include <game/uiwindows/commonwindows/itemmodelwnd.h>

RT_CLASS_EXPORTS_BEGIN(StatsButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(StatsButton);

int StatsButton::SetUpForStats(CStr const&, PointBase<float> const&, float)
{
    throw std::logic_error("Not implemented");
}

void StatsButton::ClearValue()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* StatsButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(ButtonWnd);
}

int StatsButton::UpdateValue()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* StatsButton::Clone()
{
    throw std::logic_error("Not implemented");
}

void StatsButton::SetBounds(BoundsBase<float> const&, BoundsBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

CStr const& StatsButton::GetStatsName() const
{
    throw std::logic_error("Not implemented");
}

BoundsBase<float> const& StatsButton::GetVirtualBounds() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* StatsButton::CreateObject()
{
    throw std::logic_error("Not implemented");
}

StatsButton::~StatsButton()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* StatsButton::GetClass() const
{
    throw std::logic_error("Not implemented");
}

void StatsButton::SetBounds(BoundsBase<float> const&, bool)
{
    throw std::logic_error("Not implemented");
}

StatsButton::StatsButton(StatsButton const&)
{
    throw std::logic_error("Not implemented");
}

StatsButton::StatsButton()
{
    throw std::logic_error("Not implemented");
}

CStr StatsButton::GetStatsFullName() const
{
    throw std::logic_error("Not implemented");
}

CStr StatsButton::GetHackedDefaultValueForStats(CStr const&) const
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
}

m3d::Class* StatsList::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* StatsList::CreateObject()
{
    throw std::logic_error("Not implemented");
}

void StatsList::ClearStats()
{
    throw std::logic_error("Not implemented");
}

int StatsList::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    throw std::logic_error("Not implemented");
}

void StatsList::UpdateStats()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* StatsList::Clone()
{
    throw std::logic_error("Not implemented");
}

StatsList::~StatsList()
{
    throw std::logic_error("Not implemented");
}

int StatsList::AddButtonByStatsName(CStr const&)
{
    throw std::logic_error("Not implemented");
}

StatsList::StatsList(StatsList const&)
{
    throw std::logic_error("Not implemented");
}

StatsList::StatsList()
{
    throw std::logic_error("Not implemented");
}

int StatsList::MeasureItem(int, BoundsBase<float>&) const
{
    throw std::logic_error("Not implemented");
}

int StatsList::RenderItem(int, PointBase<float> const&, m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

int StatsList::CompareItem(int, int)
{
    throw std::logic_error("Not implemented");
}

int StatsList::InitStats()
{
    throw std::logic_error("Not implemented");
}

int StatsList::DeleteItem(int)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORTS_BEGIN(StatsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(StatsWnd);

StatsWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* StatsWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* StatsWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

StatsWnd::~StatsWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* StatsWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* StatsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ScreenWnd);
}

StatsWnd::StatsWnd(StatsWnd const&)
{
    throw std::logic_error("Not implemented");
}

StatsWnd::StatsWnd()
{
    throw std::logic_error("Not implemented");
}

void StatsWnd::OnNewFrame()
{
    throw std::logic_error("Not implemented");
}

int StatsWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

int StatsWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

void StatsWnd::OnStartLevel()
{
    throw std::logic_error("Not implemented");
}

void StatsWnd::UpdatePlayerPortrait()
{
    throw std::logic_error("Not implemented");
}

void StatsWnd::UpdatePlayerDiz()
{
    throw std::logic_error("Not implemented");
}

void StatsWnd::UpdatePlayerPortraitAnmation()
{
    throw std::logic_error("Not implemented");
}

int StatsWnd::OnAfterAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

int StatsWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

void StatsWnd::UpdateStats()
{
    throw std::logic_error("Not implemented");
}
