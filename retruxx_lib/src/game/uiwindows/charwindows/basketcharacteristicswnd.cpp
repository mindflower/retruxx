#include "basketcharacteristicswnd.h"

RT_CLASS_EXPORTS_BEGIN(BasketCharacteristicsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BasketCharacteristicsWnd);

BasketCharacteristicsWnd::BasketAuxInfo::BasketAuxInfo()
{
    m_wndBasketCapacityValName = "wndBasketCapacityVal";
}

m3d::Object* BasketCharacteristicsWnd::CreateObject()
{
    return new BasketCharacteristicsWnd;
}

m3d::Class* BasketCharacteristicsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(CBCharacteristicsWnd);
}

m3d::Object* BasketCharacteristicsWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void BasketCharacteristicsWnd::SetupForBasket(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

BasketCharacteristicsWnd::~BasketCharacteristicsWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* BasketCharacteristicsWnd::GetClass() const
{
    return RT_CLASS_LOCAL(BasketCharacteristicsWnd);
}

void BasketCharacteristicsWnd::SetupForCB(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BasketCharacteristicsWnd::GameDataSetup()
{
    // TODO: implement BasketCharacteristicsWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

void BasketCharacteristicsWnd::UpdateCharacteristics()
{
    RETRUXX_NOT_IMPLEMENTED;
}

BasketCharacteristicsWnd::BasketCharacteristicsWnd(BasketCharacteristicsWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

BasketCharacteristicsWnd::BasketCharacteristicsWnd()
{
    m_wndBasketCapacityVal = nullptr;
}

void BasketCharacteristicsWnd::ClearCharacteristics()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Basket const* BasketCharacteristicsWnd::GetBasket() const
{
    RETRUXX_NOT_IMPLEMENTED;
}
