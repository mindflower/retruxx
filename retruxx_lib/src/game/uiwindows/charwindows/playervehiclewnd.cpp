#include "playervehiclewnd.h"
#include <game/uiwindows/mainwindows/damageinfownd.h>

RT_CLASS_EXPORTS_BEGIN(PlayerVehicleWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(PlayerVehicleWnd);

PlayerVehicleWnd::PAuxInfo::PAuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* PlayerVehicleWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

PlayerVehicleWnd::~PlayerVehicleWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* PlayerVehicleWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* PlayerVehicleWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* PlayerVehicleWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(VehicleWnd);
}

void PlayerVehicleWnd::UpdateTabButtonsState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

PlayerVehicleWnd::PlayerVehicleWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

PlayerVehicleWnd::PlayerVehicleWnd(PlayerVehicleWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int PlayerVehicleWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Vehicle* PlayerVehicleWnd::GetHackedVehicle() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void PlayerVehicleWnd::SetVehicleId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void PlayerVehicleWnd::DestroyHackedVehicle()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int PlayerVehicleWnd::CreateHackedVehicle()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int PlayerVehicleWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void PlayerVehicleWnd::OnTabBtnWeaponGroupsClick()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void PlayerVehicleWnd::OnTabBtnCBCharacteristicsClick()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void PlayerVehicleWnd::SetVehicleType(VehicleWnd::VehicleType)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void PlayerVehicleWnd::ShowTab(VehicleWnd::TabId)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int PlayerVehicleWnd::OnAfterAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void PlayerVehicleWnd::SelectTabButton(VehicleWnd::TabId, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int PlayerVehicleWnd::PrepareHackedVehicleForVehiclePart(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void PlayerVehicleWnd::OnTabBtnVehicleCharacteristicsClick()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int PlayerVehicleWnd::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int PlayerVehicleWnd::SetupForHackedVehiclePart(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}
