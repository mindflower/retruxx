#include "cbwnd.h"
#include "vehiclepartwnd.h"
#include "core/log.h"

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
    SetupChildVehicleParts();
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
    if ((this->m_gameDataFlags & 2) == 0)
    {
        // TODO: implement CBWnd::GameDataSetup
        //RETRUXX_NOT_IMPLEMENTED;
        return 1;
    }
    if ((this->m_gameDataFlags & 1) != 0)
        return 1;

    M3D_LOG_ERR("CBWnd: error - fail to init because of a bad resource");
    return 0;
}

void CBWnd::ClearChildVehicleParts()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CBWnd::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

int CBWnd::OnBeforeAddToWndStation()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        FullUpdate();
    }
    return Wnd::OnBeforeAddToWndStation();
}

int CBWnd::OnAfterRemoveFromWndStation()
{
    return m3d::ui::Wnd::OnAfterRemoveFromWndStation();
}

void CBWnd::OnFinishTrade()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CBWnd::FullUpdate()
{
    UpdateOnMainPartChanged();
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
