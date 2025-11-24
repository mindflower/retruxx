#include "refuellist.h"

RT_CLASS_EXPORTS_BEGIN(RefuelList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RefuelList);

RT_CLASS_EXPORTS_BEGIN(RefuelButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RefuelButton);

m3d::Object* RefuelButton::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* RefuelButton::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* RefuelButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(AdvancedButton);
}

RefuelButton::~RefuelButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* RefuelButton::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

RefuelButton::RefuelButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

RefuelButton::RefuelButton(RefuelButton const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& RefuelButton::GetStrIdTooltipBuy() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& RefuelButton::GetStrIdTooltipBuyFull() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr RefuelButton::GetServiceName() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

float RefuelButton::GetPriceForOneUnit() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::rend::TexHandle RefuelButton::GetServiceIco() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& RefuelButton::GetStrIdTooltipBuyPartial() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& RefuelButton::GetStrIdTooltipBuyNotNeed() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RefuelButton::GetMaxUnitsToBuy() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& RefuelButton::GetStrIdTooltipBuyUnavailable() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* RefuelList::GetClass() const
{
    return RT_CLASS_LOCAL(RefuelList);
}

m3d::Object* RefuelList::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

RefuelList::~RefuelList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* RefuelList::CreateObject()
{
    return new RefuelList;
}

m3d::Class* RefuelList::GetBaseClass()
{
    return RT_CLASS_LOCAL(AdvancedList);
}

AdvancedButton* RefuelList::NewItem() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

RefuelList::RefuelList(RefuelList const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RefuelList::RefuelList() = default;

std::vector<int, std::allocator<int>> RefuelList::GetObjIds() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RefuelList::BuyService(AdvancedButton const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}
