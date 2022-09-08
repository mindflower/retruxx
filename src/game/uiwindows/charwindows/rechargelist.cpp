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
    throw std::logic_error("Not implemented");
}

m3d::Object* RechargeList::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* RechargeList::GetClass() const
{
    throw std::logic_error("Not implemented");
}

RechargeList::~RechargeList()
{
    throw std::logic_error("Not implemented");
}

RechargeList::RechargeList()
{
    throw std::logic_error("Not implemented");
}

RechargeList::RechargeList(RechargeList const&)
{
    throw std::logic_error("Not implemented");
}

AdvancedButton* RechargeList::NewItem() const
{
    throw std::logic_error("Not implemented");
}

std::vector<int, std::allocator<int>> RechargeList::GetObjIds() const
{
    throw std::logic_error("Not implemented");
}

void RechargeList::BuyService(AdvancedButton const*)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* RechargeButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(AdvancedButton);
}

m3d::Object* RechargeButton::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* RechargeButton::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* RechargeButton::GetClass() const
{
    throw std::logic_error("Not implemented");
}

RechargeButton::~RechargeButton()
{
    throw std::logic_error("Not implemented");
}

RechargeButton::RechargeButton(RechargeButton const&)
{
    throw std::logic_error("Not implemented");
}

RechargeButton::RechargeButton()
{
    throw std::logic_error("Not implemented");
}

int RechargeButton::GetMaxUnitsToBuy() const
{
    throw std::logic_error("Not implemented");
}

CStr RechargeButton::GetServiceName() const
{
    throw std::logic_error("Not implemented");
}

CStr const& RechargeButton::GetStrIdTooltipBuyNotNeed() const
{
    throw std::logic_error("Not implemented");
}

float RechargeButton::GetPriceForOneUnit() const
{
    throw std::logic_error("Not implemented");
}

CStr const& RechargeButton::GetStrIdTooltipBuyUnavailable() const
{
    throw std::logic_error("Not implemented");
}

CStr const& RechargeButton::GetStrIdTooltipBuyPartial() const
{
    throw std::logic_error("Not implemented");
}

CStr const& RechargeButton::GetStrIdTooltipBuy() const
{
    throw std::logic_error("Not implemented");
}

CStr const& RechargeButton::GetStrIdTooltipBuyFull() const
{
    throw std::logic_error("Not implemented");
}

m3d::rend::TexHandle RechargeButton::GetServiceIco() const
{
    throw std::logic_error("Not implemented");
}
