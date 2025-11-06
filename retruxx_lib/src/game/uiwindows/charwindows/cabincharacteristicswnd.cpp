#include "cabincharacteristicswnd.h"

RT_CLASS_EXPORTS_BEGIN(CabinCharacteristicsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CabinCharacteristicsWnd);

CabinCharacteristicsWnd::CabinAuxInfo::CabinAuxInfo()
{
    m_wndMaxSpeedValName = "wndMaxSpeedVal";
    m_wndTorqueValName = "wndTorqueVal";
    m_wndControlValName = "wndControlVal";
}

void CabinCharacteristicsWnd::SetupForCabin(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* CabinCharacteristicsWnd::CreateObject()
{
    return new CabinCharacteristicsWnd;
}

m3d::Class* CabinCharacteristicsWnd::GetClass() const
{
    return RT_CLASS_LOCAL(CabinCharacteristicsWnd);
}

m3d::Object* CabinCharacteristicsWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* CabinCharacteristicsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(CBCharacteristicsWnd);
}

CabinCharacteristicsWnd::~CabinCharacteristicsWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CabinCharacteristicsWnd::SetupForCB(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Cabin const* CabinCharacteristicsWnd::GetCabin() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CabinCharacteristicsWnd::CabinCharacteristicsWnd()
{
    m_wndMaxSpeedVal = 0;
    m_wndTorqueVal = 0;
    m_wndControlVal = 0;
}

CabinCharacteristicsWnd::CabinCharacteristicsWnd(CabinCharacteristicsWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CabinCharacteristicsWnd::ClearCharacteristics()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CabinCharacteristicsWnd::UpdateCharacteristics()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CabinCharacteristicsWnd::GameDataSetup()
{
    // TODO: implement CabinCharacteristicsWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}
