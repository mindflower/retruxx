#include "inventorywnd.h"
#include "basketwnd.h"
#include "cabinwnd.h"

RT_CLASS_DEFINE(InventoryWnd);

InventoryWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

InventoryWnd::ChildSaveInfo::ChildSaveInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* InventoryWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

InventoryWnd::~InventoryWnd()
{
    throw std::logic_error("Not implemented");
}

void InventoryWnd::SetVehicleId(int)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* InventoryWnd::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* InventoryWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

void InventoryWnd::SetTradeVehicleId(int, ZnayuKakProdatWnd::TradeType)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* InventoryWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

InventoryWnd::InventoryWnd()
{
    throw std::logic_error("Not implemented");
}

InventoryWnd::InventoryWnd(InventoryWnd const&)
{
    throw std::logic_error("Not implemented");
}

void InventoryWnd::RemoveCBWindows()
{
    throw std::logic_error("Not implemented");
}

int InventoryWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

InventoryWnd::VehicleType InventoryWnd::GetVehicleTypeByGuiId(int) const
{
    throw std::logic_error("Not implemented");
}

int InventoryWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

int InventoryWnd::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

int InventoryWnd::OnAfterRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

void InventoryWnd::OnPlayerVehicleChanged()
{
    throw std::logic_error("Not implemented");
}

void InventoryWnd::AddCBWindows()
{
    throw std::logic_error("Not implemented");
}
