#include "basketlist.h"

#include <core/kernel.h>
#include <core/log.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/basket.h>
#include <server/resourcemanager.h>
#include <server/server.h>

RT_CLASS_EXPORTS_BEGIN(BasketButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BasketButton);

RT_CLASS_EXPORTS_BEGIN(BasketList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BasketList);

BasketButton::BasketAuxInfo BasketButton::m_baif;

// ===========================================================================
//  BasketButton
// ===========================================================================

BasketButton::BasketAuxInfo::BasketAuxInfo()
{
    // RVA 0x435DD0
    m_wndRepositoryCapacityName = "wndRepositoryCapacityVal";
}

BasketButton::BasketAuxInfo::BasketAuxInfo(BasketButton::BasketAuxInfo const& rhs) :
    m_wndRepositoryCapacityName(rhs.m_wndRepositoryCapacityName)
{
}

BasketButton::BasketButton() : m_wndRepositoryCapacity(nullptr)
{
    // RVA 0x435E20
    m_type = TYPE_BASKET;
}

BasketButton::BasketButton(BasketButton const&) : BasketButton()
{
    // NOTE: the shipped copy ctor (RVA 0x435EA0) runs only the CBButton default
    // ctor and swaps the vtable, so it leaves m_type at NUM_TYPES and
    // m_wndRepositoryCapacity uninitialised. Delegating gives a usable object
    // while still copying nothing.
}

BasketButton::~BasketButton() = default;

m3d::Object* BasketButton::Clone()
{
    // RVA 0x435CE0
    return new BasketButton(*this);
}

m3d::Object* BasketButton::CreateObject()
{
    return new BasketButton;
}

m3d::Class* BasketButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(CBButton);
}

m3d::Class* BasketButton::GetClass() const
{
    return RT_CLASS_LOCAL(BasketButton);
}

int BasketButton::CreateChildren()
{
    // RVA 0x435EF0 - the basket row adds the repository-capacity column.
    if (!CBButton::CreateChildren())
    {
        return 0;
    }

    m3d::Object* child = GetChildByName(m_baif.m_wndRepositoryCapacityName);
    if (child && child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
    {
        m_wndRepositoryCapacity = static_cast<m3d::ui::Wnd*>(child);
        return 1;
    }

    M3D_LOG_INFO(
        "Get control error: control " + m_baif.m_wndRepositoryCapacityName + " is not found or incorrect type");
    return 0;
}

int BasketButton::FullUpdate()
{
    // RVA 0x4360B0
    if (!CBButton::FullUpdate())
    {
        return 0;
    }

    ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_cbId);
    if (obj && obj->IsKindOf(&ai::Basket::m_classBasket))
    {
        auto const* prototypeInfo = static_cast<ai::BasketPrototypeInfo const*>(obj->GetPrototypeInfo());
        if (prototypeInfo)
        {
            m_wndRepositoryCapacity->SetText(CStr(prototypeInfo->GetRepositoryCapacity()));
        }
    }
    return 1;
}

// ===========================================================================
//  BasketList
// ===========================================================================

BasketList::BasketList()
{
    // RVA 0x4361D0
    m_type = CBButton::TYPE_BASKET;
}

BasketList::BasketList(BasketList const&) : BasketList()
{
    // NOTE: the shipped copy ctor (RVA 0x436220) runs only the CBList default
    // ctor and swaps the vtable, leaving m_type at NUM_TYPES; delegating keeps
    // the list usable while still copying nothing.
}

BasketList::~BasketList() = default;

m3d::Object* BasketList::Clone()
{
    // RVA 0x435D10
    return new BasketList(*this);
}

m3d::Object* BasketList::CreateObject()
{
    return new BasketList;
}

m3d::Class* BasketList::GetBaseClass()
{
    return RT_CLASS_LOCAL(CBList);
}

m3d::Class* BasketList::GetClass() const
{
    return RT_CLASS_LOCAL(BasketList);
}

int BasketList::GetCBResourceId() const
{
    // RVA 0x436250
    return ai::theResourceManager->GetResourceId(CStr("BASKET"));
}

CBButton* BasketList::CreateItem() const
{
    // RVA 0x4362A0
    return static_cast<CBButton*>(M3D_KERNEL->New("BasketButton"));
}

void BasketList::PostTriggerEventOnBuyCb()
{
    // RVA 0x4362C0
    ai::pServer->PostPlayerEvent(ai::GE_TUTORIAL_BASKET_SELECT);
}
