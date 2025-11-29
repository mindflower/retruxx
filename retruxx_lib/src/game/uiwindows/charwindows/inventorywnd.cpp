#include "inventorywnd.h"
#include "basketwnd.h"
#include "cabinwnd.h"

RT_CLASS_EXPORTS_BEGIN(InventoryWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(InventoryWnd);

InventoryWnd::AuxInfo::AuxInfo()
{
    m_wndCabinName = "wndCabin";
    m_wndBasketName = "wndBasket";
}

InventoryWnd::ChildSaveInfo::ChildSaveInfo()
{
    m_bBasketOnShowAnimationEnabled = 0;
    m_bBasketOnHideAnimationEnabled = 0;
    m_bCabinOnShowAnimationEnabled = 0;
    m_bCabinOnHideAnimationEnabled = 0;
    m_basketB.x0 = 0.0;
    m_basketB.y0 = 0.0;
    m_basketB.width = 0.0;
    m_basketB.height = 0.0;
    m_cabinB.x0 = 0.0;
    m_cabinB.y0 = 0.0;
    m_cabinB.width = 0.0;
    m_cabinB.height = 0.0;
}

m3d::Class* InventoryWnd::GetClass() const
{
    return RT_CLASS_LOCAL(InventoryWnd);
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
    return new InventoryWnd;
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
    m_vehicleType = VEHICLETYPE_INVALID;
    m_tradeType = ZnayuKakProdatWnd::TRADETYPE_NUM_TRADETYPES;
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
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

InventoryWnd::VehicleType InventoryWnd::GetVehicleTypeByGuiId(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int InventoryWnd::GameDataSetup()
{
    // TODO: implement InventoryWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
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
