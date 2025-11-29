#include "basketlist.h"

RT_CLASS_EXPORTS_BEGIN(BasketButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BasketButton);

RT_CLASS_EXPORTS_BEGIN(BasketList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BasketList);

m3d::Class* BasketList::GetClass() const
{
    return RT_CLASS_LOCAL(BasketList);
}

m3d::Object* BasketList::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

BasketList::~BasketList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* BasketList::CreateObject()
{
    return new BasketList;
}

m3d::Class* BasketList::GetBaseClass()
{
    return RT_CLASS_LOCAL(CBList);
}

int BasketList::GetCBResourceId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

BasketList::BasketList()
{
    m_type = CBButton::TYPE_BASKET;
}

BasketList::BasketList(BasketList const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CBButton* BasketList::CreateItem() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void BasketList::PostTriggerEventOnBuyCb()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* BasketButton::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* BasketButton::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* BasketButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(CBButton);
}

m3d::Class* BasketButton::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

BasketButton::~BasketButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

BasketButton::BasketButton(BasketButton const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

BasketButton::BasketButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BasketButton::CreateChildren()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BasketButton::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
}
