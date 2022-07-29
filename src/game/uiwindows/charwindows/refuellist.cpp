#include "refuellist.h"

RT_CLASS_EXPORTS_BEGIN(RefuelList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RefuelList);

RT_CLASS_EXPORTS_BEGIN(RefuelButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RefuelButton);

m3d::Object* RefuelButton::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* RefuelButton::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* RefuelButton::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

RefuelButton::~RefuelButton()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* RefuelButton::GetClass() const
{
    throw std::logic_error("Not implemented");
}

RefuelButton::RefuelButton()
{
    throw std::logic_error("Not implemented");
}

RefuelButton::RefuelButton(RefuelButton const&)
{
    throw std::logic_error("Not implemented");
}

CStr const& RefuelButton::GetStrIdTooltipBuy() const
{
    throw std::logic_error("Not implemented");
}

CStr const& RefuelButton::GetStrIdTooltipBuyFull() const
{
    throw std::logic_error("Not implemented");
}

CStr RefuelButton::GetServiceName() const
{
    throw std::logic_error("Not implemented");
}

float RefuelButton::GetPriceForOneUnit() const
{
    throw std::logic_error("Not implemented");
}

m3d::rend::TexHandle RefuelButton::GetServiceIco() const
{
    throw std::logic_error("Not implemented");
}

CStr const& RefuelButton::GetStrIdTooltipBuyPartial() const
{
    throw std::logic_error("Not implemented");
}

CStr const& RefuelButton::GetStrIdTooltipBuyNotNeed() const
{
    throw std::logic_error("Not implemented");
}

int RefuelButton::GetMaxUnitsToBuy() const
{
    throw std::logic_error("Not implemented");
}

CStr const& RefuelButton::GetStrIdTooltipBuyUnavailable() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* RefuelList::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* RefuelList::Clone()
{
    throw std::logic_error("Not implemented");
}

RefuelList::~RefuelList()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* RefuelList::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* RefuelList::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

AdvancedButton* RefuelList::NewItem() const
{
    throw std::logic_error("Not implemented");
}

RefuelList::RefuelList(RefuelList const&)
{
    throw std::logic_error("Not implemented");
}

RefuelList::RefuelList()
{
    throw std::logic_error("Not implemented");
}

std::vector<int, std::allocator<int>> RefuelList::GetObjIds() const
{
    throw std::logic_error("Not implemented");
}

void RefuelList::BuyService(AdvancedButton const*)
{
    throw std::logic_error("Not implemented");
}
