#include "repairlist.h"

RT_CLASS_EXPORTS_BEGIN(RepairButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RepairButton);

RT_CLASS_EXPORTS_BEGIN(RepairList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RepairList);

m3d::Class* RepairButton::GetClass() const
{
    throw std::logic_error("Not implemented");
}

RepairButton::~RepairButton()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* RepairButton::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* RepairButton::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* RepairButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(AdvancedButton);
}

m3d::rend::TexHandle RepairButton::GetServiceIco() const
{
    throw std::logic_error("Not implemented");
}

RepairButton::RepairButton()
{
    throw std::logic_error("Not implemented");
}

RepairButton::RepairButton(RepairButton const&)
{
    throw std::logic_error("Not implemented");
}

CStr const& RepairButton::GetStrIdTooltipBuyNotNeed() const
{
    throw std::logic_error("Not implemented");
}

CStr const& RepairButton::GetStrIdTooltipBuyPartial() const
{
    throw std::logic_error("Not implemented");
}

int RepairButton::GetMaxUnitsToBuy() const
{
    throw std::logic_error("Not implemented");
}

CStr const& RepairButton::GetStrIdTooltipBuyUnavailable() const
{
    throw std::logic_error("Not implemented");
}

CStr const& RepairButton::GetStrIdTooltipBuyFull() const
{
    throw std::logic_error("Not implemented");
}

CStr const& RepairButton::GetStrIdTooltipBuy() const
{
    throw std::logic_error("Not implemented");
}

CStr RepairButton::GetServiceName() const
{
    throw std::logic_error("Not implemented");
}

float RepairButton::GetPriceForOneUnit() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* RepairList::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* RepairList::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* RepairList::GetBaseClass()
{
    return RT_CLASS_LOCAL(AdvancedList);
}

RepairList::~RepairList()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* RepairList::GetClass() const
{
    throw std::logic_error("Not implemented");
}

RepairList::RepairList(RepairList const&)
{
    throw std::logic_error("Not implemented");
}

RepairList::RepairList()
{
    throw std::logic_error("Not implemented");
}

void RepairList::BuyService(AdvancedButton const*)
{
    throw std::logic_error("Not implemented");
}

AdvancedButton* RepairList::NewItem() const
{
    throw std::logic_error("Not implemented");
}

std::vector<int, std::allocator<int>> RepairList::GetObjIds() const
{
    throw std::logic_error("Not implemented");
}
