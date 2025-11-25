#include "localchartwnd.h"

RT_CLASS_EXPORTS_BEGIN(LocalChartWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(LocalChartWnd);

LocalChartWnd::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalChartWnd::EnableObjectNames(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* LocalChartWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ImageWnd);
}

int LocalChartWnd::SetUpForMap(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* LocalChartWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalChartWnd::SelectMapMarkIcoForObjectInfo(ObjectInfo const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* LocalChartWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool LocalChartWnd::IsObjectNamesEnabled() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalChartWnd::CreateFromPattern(m3d::ui::Wnd const*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

LocalChartWnd::~LocalChartWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CVector LocalChartWnd::WndPtToWorldPos(PointBase<float> const&, CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* LocalChartWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalChartWnd::UpdateMapBg()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalChartWnd::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalChartWnd::ClearMapMarkIcons()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalChartWnd::OnAddNavPoint(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

BoundsBase<float> LocalChartWnd::GetCellWndBounds(CStr const&, int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalChartWnd::UpdateMapMarkIcons()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalChartWnd::DeleteNavPointMark(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool LocalChartWnd::IsObjectToBeShown(ObjectInfo const*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalChartWnd::DrawCells(m3d::ui::DrawInfo const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalChartWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalChartWnd::UpdateMapMarkNamesVisibility()
{
    RETRUXX_NOT_IMPLEMENTED;
}

BoundsBase<float> LocalChartWnd::GetChartRect() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalChartWnd::AddNavPointMark(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalChartWnd::AddChild(m3d::Object*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalChartWnd::AdjustChildrenOrder()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalChartWnd::ClearMapMarkNames()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalChartWnd::UpdateMapMarks()
{
    RETRUXX_NOT_IMPLEMENTED;
}

PointBase<float> LocalChartWnd::WorldPosToWndPt(CVector const&, CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalChartWnd::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

int LocalChartWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

NavPointMarkWnd* LocalChartWnd::GetNavPointMarkByNavPointId(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalChartWnd::OnCurProfileChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalChartWnd::OnMouseButton0(unsigned, PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalChartWnd::OnMouseButton1(unsigned, PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalChartWnd::UpdatePlayerMark()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalChartWnd::DrawVisibilityMap(m3d::ui::DrawInfo const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalChartWnd::UpdateMapMarkNames()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalChartWnd::ClearNavPointMarks()
{
    RETRUXX_NOT_IMPLEMENTED;
}

LocalChartWnd::LocalChartWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

LocalChartWnd::LocalChartWnd(LocalChartWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalChartWnd::ClearMapMarks()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool LocalChartWnd::HandleMouseClickOnMapMark(PointBase<float> const&, std::vector<MapMarkWnd*, std::allocator<MapMarkWnd*>> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalChartWnd::GetGridSizeForLevel(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalChartWnd::UpdateNavPointMarks()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ObjectInfo const* LocalChartWnd::GetCurrentObjectInfo() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CVector LocalChartWnd::GetNorth(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LocalChartWnd::OnDeleteNavPoint(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool LocalChartWnd::IsObjectOnKnownZone(ObjectInfo const*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LocalChartWnd::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}
