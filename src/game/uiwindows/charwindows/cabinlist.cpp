#include "cabinlist.h"

RT_CLASS_EXPORTS_BEGIN(CabinButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CabinButton);

RT_CLASS_EXPORTS_BEGIN(CabinList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CabinList);

m3d::Object* CabinButton::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* CabinButton::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* CabinButton::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* CabinButton::Clone()
{
    throw std::logic_error("Not implemented");
}

CabinButton::~CabinButton()
{
    throw std::logic_error("Not implemented");
}

CabinButton::CabinButton(CabinButton const&)
{
    throw std::logic_error("Not implemented");
}

CabinButton::CabinButton()
{
    throw std::logic_error("Not implemented");
}

int CabinButton::FullUpdate()
{
    throw std::logic_error("Not implemented");
}

int CabinButton::CreateChildren()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* CabinList::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* CabinList::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* CabinList::Clone()
{
    throw std::logic_error("Not implemented");
}

CabinList::~CabinList()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* CabinList::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

CabinList::CabinList(CabinList const&)
{
    throw std::logic_error("Not implemented");
}

CabinList::CabinList()
{
    throw std::logic_error("Not implemented");
}

int CabinList::GetCBResourceId() const
{
    throw std::logic_error("Not implemented");
}

CBButton* CabinList::CreateItem() const
{
    throw std::logic_error("Not implemented");
}

void CabinList::PostTriggerEventOnBuyCb()
{
    throw std::logic_error("Not implemented");
}
