#include "vehiclepartwnd.h"

RT_CLASS_EXPORTS_BEGIN(VehiclePartWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(VehiclePartWnd);

VehiclePartWnd::~VehiclePartWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* VehiclePartWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* VehiclePartWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* VehiclePartWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ItemWnd);
}

int VehiclePartWnd::GetResourceId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void VehiclePartWnd::SetVehicleId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* VehiclePartWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void VehiclePartWnd::SetPartName(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool VehiclePartWnd::NeedUpdateInfoWnd(ItemInfoWnd*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void VehiclePartWnd::LaunchEventOnItemDeactivation() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

VehiclePartWnd::VehiclePartWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

VehiclePartWnd::VehiclePartWnd(VehiclePartWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int VehiclePartWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int VehiclePartWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void VehiclePartWnd::GetGunAngleEdges(retruxx::set<m3d::ui::Edges>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool VehiclePartWnd::CanContainItem() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int VehiclePartWnd::OnMouseButton1(unsigned, PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool VehiclePartWnd::IsValid() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void VehiclePartWnd::DrawGunAngleEdges(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool VehiclePartWnd::CanChangeItem() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void VehiclePartWnd::DrawGunAngleEdge(m3d::ui::Edges, m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void VehiclePartWnd::UpdateItemValue()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void VehiclePartWnd::DrawGrid(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void VehiclePartWnd::UpdateGunAngleEdges()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool VehiclePartWnd::SetItemObjId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void VehiclePartWnd::UpdateBg()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void VehiclePartWnd::SetupInfoWnd(ItemInfoWnd*)
{
    RETRUXX_NOT_IMPLEMENTED;
}
