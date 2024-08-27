#include "cabincharacteristicswnd.h"

RT_CLASS_EXPORTS_BEGIN(CabinCharacteristicsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CabinCharacteristicsWnd);

CabinCharacteristicsWnd::CabinAuxInfo::CabinAuxInfo()
{
    throw std::logic_error("Not implemented");
}

void CabinCharacteristicsWnd::SetupForCabin(int)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* CabinCharacteristicsWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* CabinCharacteristicsWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* CabinCharacteristicsWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* CabinCharacteristicsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(CBCharacteristicsWnd);
}

CabinCharacteristicsWnd::~CabinCharacteristicsWnd()
{
    throw std::logic_error("Not implemented");
}

void CabinCharacteristicsWnd::SetupForCB(int)
{
    throw std::logic_error("Not implemented");
}

ai::Cabin const* CabinCharacteristicsWnd::GetCabin() const
{
    throw std::logic_error("Not implemented");
}

CabinCharacteristicsWnd::CabinCharacteristicsWnd()
{
    throw std::logic_error("Not implemented");
}

CabinCharacteristicsWnd::CabinCharacteristicsWnd(CabinCharacteristicsWnd const&)
{
    throw std::logic_error("Not implemented");
}

void CabinCharacteristicsWnd::ClearCharacteristics()
{
    throw std::logic_error("Not implemented");
}

void CabinCharacteristicsWnd::UpdateCharacteristics()
{
    throw std::logic_error("Not implemented");
}

int CabinCharacteristicsWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}
