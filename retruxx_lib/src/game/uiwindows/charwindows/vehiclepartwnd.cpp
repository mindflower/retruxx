#include "vehiclepartwnd.h"

RT_CLASS_EXPORTS_BEGIN(VehiclePartWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(VehiclePartWnd);

VehiclePartWnd::~VehiclePartWnd()
{
    throw retruxx::logic_error("Not implemented");
}

m3d::Object* VehiclePartWnd::Clone()
{
    throw retruxx::logic_error("Not implemented");
}

m3d::Object* VehiclePartWnd::CreateObject()
{
    throw retruxx::logic_error("Not implemented");
}

m3d::Class* VehiclePartWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ItemWnd);
}

int VehiclePartWnd::GetResourceId() const
{
    throw retruxx::logic_error("Not implemented");
}

void VehiclePartWnd::SetVehicleId(int)
{
    throw retruxx::logic_error("Not implemented");
}

m3d::Class* VehiclePartWnd::GetClass() const
{
    throw retruxx::logic_error("Not implemented");
}

void VehiclePartWnd::SetPartName(CStr const&)
{
    throw retruxx::logic_error("Not implemented");
}

bool VehiclePartWnd::NeedUpdateInfoWnd(ItemInfoWnd*) const
{
    throw retruxx::logic_error("Not implemented");
}

void VehiclePartWnd::LaunchEventOnItemDeactivation() const
{
    throw retruxx::logic_error("Not implemented");
}

VehiclePartWnd::VehiclePartWnd()
{
    throw retruxx::logic_error("Not implemented");
}

VehiclePartWnd::VehiclePartWnd(VehiclePartWnd const&)
{
    throw retruxx::logic_error("Not implemented");
}

int VehiclePartWnd::GameDataUpdate(void*, int)
{
    throw retruxx::logic_error("Not implemented");
}

int VehiclePartWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    throw retruxx::logic_error("Not implemented");
}

void VehiclePartWnd::GetGunAngleEdges(retruxx::set<m3d::ui::Edges>&) const
{
    throw retruxx::logic_error("Not implemented");
}

bool VehiclePartWnd::CanContainItem() const
{
    throw retruxx::logic_error("Not implemented");
}

int VehiclePartWnd::OnMouseButton1(unsigned, PointBase<float> const&)
{
    throw retruxx::logic_error("Not implemented");
}

bool VehiclePartWnd::IsValid() const
{
    throw retruxx::logic_error("Not implemented");
}

void VehiclePartWnd::DrawGunAngleEdges(m3d::ui::DrawInfo const&)
{
    throw retruxx::logic_error("Not implemented");
}

bool VehiclePartWnd::CanChangeItem() const
{
    throw retruxx::logic_error("Not implemented");
}

void VehiclePartWnd::DrawGunAngleEdge(m3d::ui::Edges, m3d::ui::DrawInfo const&)
{
    throw retruxx::logic_error("Not implemented");
}

void VehiclePartWnd::UpdateItemValue()
{
    throw retruxx::logic_error("Not implemented");
}

void VehiclePartWnd::DrawGrid(m3d::ui::DrawInfo const&)
{
    throw retruxx::logic_error("Not implemented");
}

void VehiclePartWnd::UpdateGunAngleEdges()
{
    throw retruxx::logic_error("Not implemented");
}

bool VehiclePartWnd::SetItemObjId(int)
{
    throw retruxx::logic_error("Not implemented");
}

void VehiclePartWnd::UpdateBg()
{
    throw retruxx::logic_error("Not implemented");
}

void VehiclePartWnd::SetupInfoWnd(ItemInfoWnd*)
{
    throw retruxx::logic_error("Not implemented");
}
