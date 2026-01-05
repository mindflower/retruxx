#include "cbcharacteristicswnd.h"

RT_CLASS_EXPORTS_BEGIN(CBCharacteristicsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CBCharacteristicsWnd);

CBCharacteristicsWnd::AuxInfo::AuxInfo()
{
    m_wndPiercingValName = "wndPiercingVal";
    m_wndBlastValName = "wndBlastVal";
    m_wndEnergyValName = "wndEnergyVal";
    m_wndMaxDurabilityValName = "wndMaxDurabilityVal";
    m_wndWeightValName = "wndWeightVal";
    m_wndCBImageName = "wndCBIco";
    m_wndCBNameName = "wndCBName";
}

m3d::Object* CBCharacteristicsWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CBCharacteristicsWnd::~CBCharacteristicsWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* CBCharacteristicsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(CharacteristicsWnd);
}

void CBCharacteristicsWnd::SetupForCB(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* CBCharacteristicsWnd::CreateObject()
{
    return new CBCharacteristicsWnd;
}

m3d::Class* CBCharacteristicsWnd::GetClass() const
{
    return RT_CLASS_LOCAL(CBCharacteristicsWnd);
}

CBCharacteristicsWnd::CBCharacteristicsWnd()
{
    m_wndPiercingVal = 0;
    m_wndBlastVal = 0;
    m_wndEnergyVal = 0;
    m_wndMaxDurabilityVal = 0;
    m_wndWeightVal = 0;
    m_wndCBImage = 0;
    m_wndCBName = 0;
    m_cbId = -1;
}

void CBCharacteristicsWnd::UpdateOnCBChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CBCharacteristicsWnd::ClearCharacteristics()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CBCharacteristicsWnd::UpdateCBName()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CBCharacteristicsWnd::GameDataSetup()
{
    // TODO: implement CBCharacteristicsWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

void CBCharacteristicsWnd::UpdateCharacteristics()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::VehiclePart const* CBCharacteristicsWnd::GetCB() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CBCharacteristicsWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CBCharacteristicsWnd::UpdateCBImage()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CBCharacteristicsWnd::SetupForVehicle(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}
