#include "rechargelist.h"

RT_CLASS_EXPORTS_BEGIN(RechargeList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RechargeList);

RT_CLASS_EXPORTS_BEGIN(RechargeButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RechargeButton);

m3d::Class* RechargeList::GetBaseClass()
{
    return RT_CLASS_LOCAL(AdvancedList);
}

m3d::Object* RechargeList::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* RechargeList::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* RechargeList::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

RechargeList::~RechargeList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

RechargeList::RechargeList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

RechargeList::RechargeList(RechargeList const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

AdvancedButton* RechargeList::NewItem() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

std::vector<int, std::allocator<int>> RechargeList::GetObjIds() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RechargeList::BuyService(AdvancedButton const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* RechargeButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(AdvancedButton);
}

m3d::Object* RechargeButton::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* RechargeButton::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* RechargeButton::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

RechargeButton::~RechargeButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

RechargeButton::RechargeButton(RechargeButton const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RechargeButton::RechargeButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RechargeButton::GetMaxUnitsToBuy() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr RechargeButton::GetServiceName() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& RechargeButton::GetStrIdTooltipBuyNotNeed() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

float RechargeButton::GetPriceForOneUnit() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& RechargeButton::GetStrIdTooltipBuyUnavailable() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& RechargeButton::GetStrIdTooltipBuyPartial() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& RechargeButton::GetStrIdTooltipBuy() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& RechargeButton::GetStrIdTooltipBuyFull() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::rend::TexHandle RechargeButton::GetServiceIco() const
{
    RETRUXX_NOT_IMPLEMENTED;
}
