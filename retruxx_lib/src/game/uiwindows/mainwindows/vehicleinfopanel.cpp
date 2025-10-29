#include "vehicleinfopanel.h"
#include "speedometerwnd.h"
#include "fuelindicatorwnd.h"

RT_CLASS_EXPORTS_BEGIN(VehicleInfoPanel)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(VehicleInfoPanel);

VehicleInfoPanel::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* VehicleInfoPanel::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

VehicleInfoPanel::~VehicleInfoPanel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* VehicleInfoPanel::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* VehicleInfoPanel::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* VehicleInfoPanel::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

VehicleInfoPanel::VehicleInfoPanel(VehicleInfoPanel const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

VehicleInfoPanel::VehicleInfoPanel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int VehicleInfoPanel::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void VehicleInfoPanel::OnPlayerVehicleChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int VehicleInfoPanel::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}
