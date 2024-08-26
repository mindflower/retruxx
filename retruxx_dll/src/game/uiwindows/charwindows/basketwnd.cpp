#include "basketwnd.h"
#include "izvratrepositorywnd.h"

RT_CLASS_EXPORTS_BEGIN(BasketWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BasketWnd);

BasketWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* BasketWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* BasketWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* BasketWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* BasketWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(CBWnd);
}

BasketWnd::~BasketWnd()
{
    throw std::logic_error("Not implemented");
}

void BasketWnd::SetupInventory()
{
    throw std::logic_error("Not implemented");
}

int BasketWnd::OnAfterRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

void BasketWnd::UpdateOnMainPartChanged()
{
    throw std::logic_error("Not implemented");
}

int BasketWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

void BasketWnd::SetupChildVehicleParts()
{
    throw std::logic_error("Not implemented");
}

int BasketWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

int BasketWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

BasketWnd::BasketWnd()
{
    throw std::logic_error("Not implemented");
}

BasketWnd::BasketWnd(BasketWnd const&)
{
    throw std::logic_error("Not implemented");
}

InventoryWnd::VehicleType BasketWnd::GetVehicleTypeByGuiId(int) const
{
    throw std::logic_error("Not implemented");
}

void BasketWnd::UpdateOnSaleTabChanged(void*)
{
    throw std::logic_error("Not implemented");
}
