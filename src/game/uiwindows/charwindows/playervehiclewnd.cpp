#include "playervehiclewnd.h"
#include <game/uiwindows/mainwindows/damageinfownd.h>

RT_CLASS_EXPORTS_BEGIN(PlayerVehicleWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(PlayerVehicleWnd);

PlayerVehicleWnd::PAuxInfo::PAuxInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* PlayerVehicleWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

PlayerVehicleWnd::~PlayerVehicleWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* PlayerVehicleWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* PlayerVehicleWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* PlayerVehicleWnd::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

void PlayerVehicleWnd::UpdateTabButtonsState()
{
    throw std::logic_error("Not implemented");
}

PlayerVehicleWnd::PlayerVehicleWnd()
{
    throw std::logic_error("Not implemented");
}

PlayerVehicleWnd::PlayerVehicleWnd(PlayerVehicleWnd const&)
{
    throw std::logic_error("Not implemented");
}

int PlayerVehicleWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

ai::Vehicle* PlayerVehicleWnd::GetHackedVehicle() const
{
    throw std::logic_error("Not implemented");
}

void PlayerVehicleWnd::SetVehicleId(int)
{
    throw std::logic_error("Not implemented");
}

void PlayerVehicleWnd::DestroyHackedVehicle()
{
    throw std::logic_error("Not implemented");
}

int PlayerVehicleWnd::CreateHackedVehicle()
{
    throw std::logic_error("Not implemented");
}

int PlayerVehicleWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

void PlayerVehicleWnd::OnTabBtnWeaponGroupsClick()
{
    throw std::logic_error("Not implemented");
}

void PlayerVehicleWnd::OnTabBtnCBCharacteristicsClick()
{
    throw std::logic_error("Not implemented");
}

void PlayerVehicleWnd::SetVehicleType(VehicleWnd::VehicleType)
{
    throw std::logic_error("Not implemented");
}

void PlayerVehicleWnd::ShowTab(VehicleWnd::TabId)
{
    throw std::logic_error("Not implemented");
}

int PlayerVehicleWnd::OnAfterAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

void PlayerVehicleWnd::SelectTabButton(VehicleWnd::TabId, bool)
{
    throw std::logic_error("Not implemented");
}

int PlayerVehicleWnd::PrepareHackedVehicleForVehiclePart(int)
{
    throw std::logic_error("Not implemented");
}

void PlayerVehicleWnd::OnTabBtnVehicleCharacteristicsClick()
{
    throw std::logic_error("Not implemented");
}

int PlayerVehicleWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

int PlayerVehicleWnd::SetupForHackedVehiclePart(int)
{
    throw std::logic_error("Not implemented");
}
