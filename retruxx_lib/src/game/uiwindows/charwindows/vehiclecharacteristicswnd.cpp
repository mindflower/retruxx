#include "vehiclecharacteristicswnd.h"

RT_CLASS_EXPORTS_BEGIN(VehicleCharacteristicsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(VehicleCharacteristicsWnd);

VehicleCharacteristicsWnd::AuxInfo::AuxInfo()
{
    m_wndMaxHealthValName = "wndMaxHealthVal";
    m_wndMaxFuelValName = "wndMaxFuelVal";
    m_wndPiercingValName = "wndPiercingVal";
    m_wndBlastValName = "wndBlastVal";
    m_wndEnergyValName = "wndEnergyVal";
    m_wndMaxDurabilityValName = "wndMaxDurabilityVal";
    m_wndBasketCapacityValName = "wndBasketCapacityVal";
    m_wndMaxSpeedValName = "wndMaxSpeedVal";
    m_wndTorqueValName = "wndTorqueVal";
    m_wndWeightValName = "wndWeightVal";
    m_wndControlValName = "wndControlVal";
}

m3d::Object* VehicleCharacteristicsWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* VehicleCharacteristicsWnd::CreateObject()
{
    return new VehicleCharacteristicsWnd;
}

m3d::Class* VehicleCharacteristicsWnd::GetClass() const
{
    return RT_CLASS_LOCAL(VehicleCharacteristicsWnd);
}

m3d::Class* VehicleCharacteristicsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(CharacteristicsWnd);
}

VehicleCharacteristicsWnd::~VehicleCharacteristicsWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void VehicleCharacteristicsWnd::UpdateCharacteristics()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int VehicleCharacteristicsWnd::GameDataSetup()
{
    // TODO: implement VehicleCharacteristicsWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

void VehicleCharacteristicsWnd::ClearCharacteristics()
{
    RETRUXX_NOT_IMPLEMENTED;
}

VehicleCharacteristicsWnd::VehicleCharacteristicsWnd()
{
    m_wndMaxHealthVal = 0;
    m_wndMaxFuelVal = 0;
    m_wndPiercingVal = 0;
    m_wndBlastVal = 0;
    m_wndEnergyVal = 0;
    m_wndMaxDurabilityVal = 0;
    m_wndBasketCapacityVal = 0;
    m_wndMaxSpeedVal = 0;
    m_wndTorqueVal = 0;
    m_wndWeightVal = 0;
    m_wndControlVal = 0;
}

VehicleCharacteristicsWnd::VehicleCharacteristicsWnd(VehicleCharacteristicsWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
