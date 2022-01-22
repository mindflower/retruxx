#include "localchartwnd.h"

RT_CLASS_DEFINE(LocalChartWnd);

LocalChartWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

void LocalChartWnd::EnableObjectNames(bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* LocalChartWnd::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

int LocalChartWnd::SetUpForMap(CStr const&)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* LocalChartWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

void LocalChartWnd::SelectMapMarkIcoForObjectInfo(ObjectInfo const*)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* LocalChartWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

bool LocalChartWnd::IsObjectNamesEnabled() const
{
    throw std::logic_error("Not implemented");
}

int LocalChartWnd::CreateFromPattern(m3d::ui::Wnd const*, bool)
{
    throw std::logic_error("Not implemented");
}

LocalChartWnd::~LocalChartWnd()
{
    throw std::logic_error("Not implemented");
}

CVector LocalChartWnd::WndPtToWorldPos(PointBase<float> const&, CStr const&) const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* LocalChartWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

int LocalChartWnd::UpdateMapBg()
{
    throw std::logic_error("Not implemented");
}

int LocalChartWnd::FullUpdate()
{
    throw std::logic_error("Not implemented");
}

void LocalChartWnd::ClearMapMarkIcons()
{
    throw std::logic_error("Not implemented");
}

void LocalChartWnd::OnAddNavPoint(void*)
{
    throw std::logic_error("Not implemented");
}

BoundsBase<float> LocalChartWnd::GetCellWndBounds(CStr const&, int) const
{
    throw std::logic_error("Not implemented");
}

int LocalChartWnd::UpdateMapMarkIcons()
{
    throw std::logic_error("Not implemented");
}

int LocalChartWnd::DeleteNavPointMark(int)
{
    throw std::logic_error("Not implemented");
}

bool LocalChartWnd::IsObjectToBeShown(ObjectInfo const*) const
{
    throw std::logic_error("Not implemented");
}

void LocalChartWnd::DrawCells(m3d::ui::DrawInfo const&) const
{
    throw std::logic_error("Not implemented");
}

int LocalChartWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

void LocalChartWnd::UpdateMapMarkNamesVisibility()
{
    throw std::logic_error("Not implemented");
}

BoundsBase<float> LocalChartWnd::GetChartRect() const
{
    throw std::logic_error("Not implemented");
}

int LocalChartWnd::AddNavPointMark(int)
{
    throw std::logic_error("Not implemented");
}

int LocalChartWnd::AddChild(m3d::Object*)
{
    throw std::logic_error("Not implemented");
}

void LocalChartWnd::AdjustChildrenOrder()
{
    throw std::logic_error("Not implemented");
}

void LocalChartWnd::ClearMapMarkNames()
{
    throw std::logic_error("Not implemented");
}

int LocalChartWnd::UpdateMapMarks()
{
    throw std::logic_error("Not implemented");
}

PointBase<float> LocalChartWnd::WorldPosToWndPt(CVector const&, CStr const&) const
{
    throw std::logic_error("Not implemented");
}

int LocalChartWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

int LocalChartWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

NavPointMarkWnd* LocalChartWnd::GetNavPointMarkByNavPointId(int) const
{
    throw std::logic_error("Not implemented");
}

void LocalChartWnd::OnCurProfileChanged()
{
    throw std::logic_error("Not implemented");
}

int LocalChartWnd::OnMouseButton0(unsigned, PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

int LocalChartWnd::OnMouseButton1(unsigned, PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

int LocalChartWnd::UpdatePlayerMark()
{
    throw std::logic_error("Not implemented");
}

void LocalChartWnd::DrawVisibilityMap(m3d::ui::DrawInfo const&) const
{
    throw std::logic_error("Not implemented");
}

int LocalChartWnd::UpdateMapMarkNames()
{
    throw std::logic_error("Not implemented");
}

void LocalChartWnd::ClearNavPointMarks()
{
    throw std::logic_error("Not implemented");
}

LocalChartWnd::LocalChartWnd()
{
    throw std::logic_error("Not implemented");
}

LocalChartWnd::LocalChartWnd(LocalChartWnd const&)
{
    throw std::logic_error("Not implemented");
}

void LocalChartWnd::ClearMapMarks()
{
    throw std::logic_error("Not implemented");
}

bool LocalChartWnd::HandleMouseClickOnMapMark(PointBase<float> const&, std::vector<MapMarkWnd*, std::allocator<MapMarkWnd*>> const&)
{
    throw std::logic_error("Not implemented");
}

int LocalChartWnd::GetGridSizeForLevel(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

int LocalChartWnd::UpdateNavPointMarks()
{
    throw std::logic_error("Not implemented");
}

ObjectInfo const* LocalChartWnd::GetCurrentObjectInfo() const
{
    throw std::logic_error("Not implemented");
}

CVector LocalChartWnd::GetNorth(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

void LocalChartWnd::OnDeleteNavPoint(void*)
{
    throw std::logic_error("Not implemented");
}

bool LocalChartWnd::IsObjectOnKnownZone(ObjectInfo const*) const
{
    throw std::logic_error("Not implemented");
}

int LocalChartWnd::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}
