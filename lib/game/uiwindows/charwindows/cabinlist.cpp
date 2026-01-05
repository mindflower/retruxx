#include "cabinlist.h"

RT_CLASS_EXPORTS_BEGIN(CabinButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CabinButton);

RT_CLASS_EXPORTS_BEGIN(CabinList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CabinList);

m3d::Object* CabinButton::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* CabinButton::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* CabinButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(CBButton);
}

m3d::Object* CabinButton::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CabinButton::~CabinButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CabinButton::CabinButton(CabinButton const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CabinButton::CabinButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CabinButton::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CabinButton::CreateChildren()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* CabinList::GetClass() const
{
    return RT_CLASS_LOCAL(CabinList);
}

m3d::Object* CabinList::CreateObject()
{
    return new CabinList;
}

m3d::Object* CabinList::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CabinList::~CabinList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* CabinList::GetBaseClass()
{
    return RT_CLASS_LOCAL(CBList);
}

CabinList::CabinList(CabinList const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CabinList::CabinList()
{
    m_type = CBButton::TYPE_CABIN;
}

int CabinList::GetCBResourceId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CBButton* CabinList::CreateItem() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CabinList::PostTriggerEventOnBuyCb()
{
    RETRUXX_NOT_IMPLEMENTED;
}
