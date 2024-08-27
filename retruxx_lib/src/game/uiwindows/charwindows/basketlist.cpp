#include "basketlist.h"

RT_CLASS_EXPORTS_BEGIN(BasketButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BasketButton);

RT_CLASS_EXPORTS_BEGIN(BasketList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BasketList);

m3d::Class* BasketList::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* BasketList::Clone()
{
    throw std::logic_error("Not implemented");
}

BasketList::~BasketList()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* BasketList::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* BasketList::GetBaseClass()
{
    return RT_CLASS_LOCAL(CBList);
}

int BasketList::GetCBResourceId() const
{
    throw std::logic_error("Not implemented");
}

BasketList::BasketList()
{
    throw std::logic_error("Not implemented");
}

BasketList::BasketList(BasketList const&)
{
    throw std::logic_error("Not implemented");
}

CBButton* BasketList::CreateItem() const
{
    throw std::logic_error("Not implemented");
}

void BasketList::PostTriggerEventOnBuyCb()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* BasketButton::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* BasketButton::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* BasketButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(CBButton);
}

m3d::Class* BasketButton::GetClass() const
{
    throw std::logic_error("Not implemented");
}

BasketButton::~BasketButton()
{
    throw std::logic_error("Not implemented");
}

BasketButton::BasketButton(BasketButton const&)
{
    throw std::logic_error("Not implemented");
}

BasketButton::BasketButton()
{
    throw std::logic_error("Not implemented");
}

int BasketButton::CreateChildren()
{
    throw std::logic_error("Not implemented");
}

int BasketButton::FullUpdate()
{
    throw std::logic_error("Not implemented");
}
