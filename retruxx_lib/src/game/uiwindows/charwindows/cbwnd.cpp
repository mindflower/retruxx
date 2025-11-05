#include "cbwnd.h"
#include "vehiclepartwnd.h"

RT_CLASS_EXPORTS_BEGIN(CBWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CBWnd);

CBWnd::CBAuxInfo::CBAuxInfo()
{
    m_vehiclePartPaneName = "PaneGun";
    m_wndHidePictureBgName = "wndHidePictureBg";
    m_wndDisabledBgName = "wndDisabledBg";
}

void CBWnd::SetPartId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CBWnd::GetPartId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CBWnd::SetVehicleId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* CBWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ChildPanel);
}

CBWnd::~CBWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* CBWnd::GetRtClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

InventoryWnd::VehicleType CBWnd::GetVehicleTypeByGuiId(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CBWnd::UpdateOnVehiclepartChanged(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CBWnd::UpdateOnMainPartChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CBWnd::SetupChildVehicleParts()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CBWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CBWnd::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CBWnd::ClearChildVehicleParts()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CBWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CBWnd::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CBWnd::OnAfterRemoveFromWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CBWnd::OnFinishTrade()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CBWnd::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CBWnd::CBWnd()
{
    m_wndHidePictureBg = 0;
    m_wndDisabledBg = 0;
    m_vehicleType = InventoryWnd::VEHICLETYPE_INVALID;
    m_mainPartId = -1;
}

void CBWnd::SetDisabledLook(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CBWnd::CreateChildVehiclePartWindow(CStr const&, PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
