#include "vehiclepartwnd.h"

RT_CLASS_EXPORTS_BEGIN(VehiclePartWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(VehiclePartWnd);

VehiclePartWnd::~VehiclePartWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* VehiclePartWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* VehiclePartWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* VehiclePartWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ItemWnd);
}

int VehiclePartWnd::GetResourceId() const
{
    throw std::logic_error("Not implemented");
}

void VehiclePartWnd::SetVehicleId(int)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* VehiclePartWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

void VehiclePartWnd::SetPartName(CStr const&)
{
    throw std::logic_error("Not implemented");
}

bool VehiclePartWnd::NeedUpdateInfoWnd(ItemInfoWnd*) const
{
    throw std::logic_error("Not implemented");
}

void VehiclePartWnd::LaunchEventOnItemDeactivation() const
{
    throw std::logic_error("Not implemented");
}

VehiclePartWnd::VehiclePartWnd()
{
    throw std::logic_error("Not implemented");
}

VehiclePartWnd::VehiclePartWnd(VehiclePartWnd const&)
{
    throw std::logic_error("Not implemented");
}

int VehiclePartWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

int VehiclePartWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

void VehiclePartWnd::GetGunAngleEdges(std::set<m3d::ui::Edges>&) const
{
    throw std::logic_error("Not implemented");
}

bool VehiclePartWnd::CanContainItem() const
{
    throw std::logic_error("Not implemented");
}

int VehiclePartWnd::OnMouseButton1(unsigned, PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

bool VehiclePartWnd::IsValid() const
{
    throw std::logic_error("Not implemented");
}

void VehiclePartWnd::DrawGunAngleEdges(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

bool VehiclePartWnd::CanChangeItem() const
{
    throw std::logic_error("Not implemented");
}

void VehiclePartWnd::DrawGunAngleEdge(m3d::ui::Edges, m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

void VehiclePartWnd::UpdateItemValue()
{
    throw std::logic_error("Not implemented");
}

void VehiclePartWnd::DrawGrid(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

void VehiclePartWnd::UpdateGunAngleEdges()
{
    throw std::logic_error("Not implemented");
}

bool VehiclePartWnd::SetItemObjId(int)
{
    throw std::logic_error("Not implemented");
}

void VehiclePartWnd::UpdateBg()
{
    throw std::logic_error("Not implemented");
}

void VehiclePartWnd::SetupInfoWnd(ItemInfoWnd*)
{
    throw std::logic_error("Not implemented");
}
