#include "vehiclewnd.h"
#include <game/uiwindows/mainwindows/damageinfownd.h>
#include "weaponslotlist.h"
#include "basketcharacteristicswnd.h"
#include "cabincharacteristicswnd.h"
#include "vehiclecharacteristicswnd.h"
#include <game/uiwindows/commonwindows/complexmodelwnd.h>

RT_CLASS_EXPORTS_BEGIN(VehicleWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(VehicleWnd);

VehicleWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

void VehicleWnd::SetVehicleId(int)
{
    throw std::logic_error("Not implemented");
}

VehicleWnd::~VehicleWnd()
{
    throw std::logic_error("Not implemented");
}

VehicleWnd::VehicleWnd(VehicleWnd const&)
{
    throw std::logic_error("Not implemented");
}

VehicleWnd::VehicleWnd()
{
    throw std::logic_error("Not implemented");
}

void VehicleWnd::UpdateVehicleName()
{
    throw std::logic_error("Not implemented");
}

void VehicleWnd::OnVehiclePartChanged(void*)
{
    throw std::logic_error("Not implemented");
}

void VehicleWnd::ShowTab(TabId)
{
    throw std::logic_error("Not implemented");
}

void VehicleWnd::OnCharacteristicTabSelChanged(void*)
{
    throw std::logic_error("Not implemented");
}
