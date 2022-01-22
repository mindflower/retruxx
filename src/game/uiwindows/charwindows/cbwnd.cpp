#include "cbwnd.h"
#include "vehiclepartwnd.h"

RT_CLASS_DEFINE(CBWnd);

CBWnd::CBAuxInfo::CBAuxInfo()
{
    throw std::logic_error("Not implemented");
}

void CBWnd::SetPartId(int)
{
    throw std::logic_error("Not implemented");
}

int CBWnd::GetPartId() const
{
    throw std::logic_error("Not implemented");
}

void CBWnd::SetVehicleId(int)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* CBWnd::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

CBWnd::~CBWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* CBWnd::GetRtClass() const
{
    throw std::logic_error("Not implemented");
}

InventoryWnd::VehicleType CBWnd::GetVehicleTypeByGuiId(int) const
{
    throw std::logic_error("Not implemented");
}

void CBWnd::UpdateOnVehiclepartChanged(CStr const&)
{
    throw std::logic_error("Not implemented");
}

void CBWnd::UpdateOnMainPartChanged()
{
    throw std::logic_error("Not implemented");
}

void CBWnd::SetupChildVehicleParts()
{
    throw std::logic_error("Not implemented");
}

int CBWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

int CBWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

void CBWnd::ClearChildVehicleParts()
{
    throw std::logic_error("Not implemented");
}

int CBWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

int CBWnd::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

int CBWnd::OnAfterRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

void CBWnd::OnFinishTrade()
{
    throw std::logic_error("Not implemented");
}

void CBWnd::FullUpdate()
{
    throw std::logic_error("Not implemented");
}

CBWnd::CBWnd()
{
    throw std::logic_error("Not implemented");
}

void CBWnd::SetDisabledLook(bool)
{
    throw std::logic_error("Not implemented");
}

int CBWnd::CreateChildVehiclePartWindow(CStr const&, PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}
