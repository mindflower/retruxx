#include "repairlist.h"

RT_CLASS_EXPORTS_BEGIN(RepairButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RepairButton);

RT_CLASS_EXPORTS_BEGIN(RepairList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RepairList);

m3d::Class* RepairButton::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

RepairButton::~RepairButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* RepairButton::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* RepairButton::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* RepairButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(AdvancedButton);
}

m3d::rend::TexHandle RepairButton::GetServiceIco() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

RepairButton::RepairButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

RepairButton::RepairButton(RepairButton const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& RepairButton::GetStrIdTooltipBuyNotNeed() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& RepairButton::GetStrIdTooltipBuyPartial() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepairButton::GetMaxUnitsToBuy() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& RepairButton::GetStrIdTooltipBuyUnavailable() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& RepairButton::GetStrIdTooltipBuyFull() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& RepairButton::GetStrIdTooltipBuy() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr RepairButton::GetServiceName() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

float RepairButton::GetPriceForOneUnit() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* RepairList::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* RepairList::CreateObject()
{
    return new RepairList;
}

m3d::Class* RepairList::GetBaseClass()
{
    return RT_CLASS_LOCAL(AdvancedList);
}

RepairList::~RepairList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* RepairList::GetClass() const
{
    return RT_CLASS_LOCAL(RepairList);
}

RepairList::RepairList(RepairList const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RepairList::RepairList() = default;

void RepairList::BuyService(AdvancedButton const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

AdvancedButton* RepairList::NewItem() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

std::vector<int, std::allocator<int>> RepairList::GetObjIds() const
{
    RETRUXX_NOT_IMPLEMENTED;
}
