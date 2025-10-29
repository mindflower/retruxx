#include "basketwnd.h"
#include "izvratrepositorywnd.h"

RT_CLASS_EXPORTS_BEGIN(BasketWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BasketWnd);

BasketWnd::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* BasketWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* BasketWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* BasketWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* BasketWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(CBWnd);
}

BasketWnd::~BasketWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void BasketWnd::SetupInventory()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BasketWnd::OnAfterRemoveFromWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void BasketWnd::UpdateOnMainPartChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BasketWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void BasketWnd::SetupChildVehicleParts()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BasketWnd::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BasketWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

BasketWnd::BasketWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

BasketWnd::BasketWnd(BasketWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

InventoryWnd::VehicleType BasketWnd::GetVehicleTypeByGuiId(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void BasketWnd::UpdateOnSaleTabChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}
