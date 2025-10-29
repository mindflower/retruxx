#include "inventorywnd.h"
#include "basketwnd.h"
#include "cabinwnd.h"

RT_CLASS_EXPORTS_BEGIN(InventoryWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(InventoryWnd);

InventoryWnd::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

InventoryWnd::ChildSaveInfo::ChildSaveInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* InventoryWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

InventoryWnd::~InventoryWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void InventoryWnd::SetVehicleId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* InventoryWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ChildPanel);
}

m3d::Object* InventoryWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void InventoryWnd::SetTradeVehicleId(int, ZnayuKakProdatWnd::TradeType)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* InventoryWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

InventoryWnd::InventoryWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

InventoryWnd::InventoryWnd(InventoryWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void InventoryWnd::RemoveCBWindows()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int InventoryWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

InventoryWnd::VehicleType InventoryWnd::GetVehicleTypeByGuiId(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int InventoryWnd::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int InventoryWnd::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int InventoryWnd::OnAfterRemoveFromWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void InventoryWnd::OnPlayerVehicleChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void InventoryWnd::AddCBWindows()
{
    RETRUXX_NOT_IMPLEMENTED;
}
