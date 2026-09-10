#include "shopwnd.h"

#include "dragdropitemswnd.h"
#include "repositorywnd.h"
#include "warewnd.h"

#include <core/aiparam.h>
#include <core/kernel.h>
#include <core/log.h>
#include <game/m3dgame.h>
#include <game/uimanager/gameuimanager.h>
#include <game/uimanager/truxxuimanager.h>
#include <m3dapp.h>
#include <ui/wndstation.h>

RT_CLASS_EXPORTS_BEGIN(ShopWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ShopWnd);

namespace
{
    // GameDataUpdate data types the shop reacts to.
    int const DATATYPE_DD_ITEM_QUICK_DROP = 99;
    int const DATATYPE_DD_DROP = 100;

    // Raised so the tutorial layer knows an item was dropped onto a tab.
    int const EVENT_ITEM_DROPPED_ON_TAB = 97;

    // Enqueued whenever the visible tab changes.
    int const MSG_TAB_SWITCHED = 65691;
}  // namespace

m3d::Class* ShopWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(SaleWnd);
}

m3d::Class* ShopWnd::GetClass() const
{
    return RT_CLASS_LOCAL(ShopWnd);
}

m3d::Object* ShopWnd::CreateObject()
{
    return new ShopWnd;
}

m3d::Object* ShopWnd::Clone()
{
    // RVA 0x472D40 - a fresh default-constructed window, not a copy of *this.
    return new ShopWnd;
}

ShopWnd::ShopWnd()
{
    // RVA 0x472DD0
    m_buildingType = ai::SHOP;
}

ShopWnd::ShopWnd(ShopWnd const&)
{
    // NOTE: the shipped copy ctor (RVA 0x472E20) chains to the SaleWnd ctor but,
    // unlike the default one above, never sets m_buildingType - so a copied shop
    // keeps SaleWnd's INVALID_BUILDINGTYPE. Kept as shipped; nothing in the exe
    // reaches this ctor.
}

ShopWnd::~ShopWnd() = default;

ai::BuildingType ShopWnd::GetBuildingType()
{
    // RVA 0x472F90
    return ai::SHOP;
}

void ShopWnd::SetupListOfTabTypes()
{
    // RVA 0x472F60 - note the goods tab is listed first, so it is the one that
    // ends up selected when CreateTabItems calls SetCurSel(0).
    m_tabTypes.push_back(TABTYPE_GOODS);
    m_tabTypes.push_back(TABTYPE_GUNS_AND_GADGETS);
}

int ShopWnd::GameDataSetup()
{
    // RVA 0x472E50
    if (!SaleWnd::GameDataSetup())
    {
        return 0;
    }
    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("ShopWnd: error - fail to init because of a bad resource");
    return 0;
}

int ShopWnd::SetUpForWorkshop(int shopId)
{
    // RVA 0x472FA0
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    return SaleWnd::SetUpForWorkshop(shopId) & 1;
}

int ShopWnd::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x472ED0
    if (dataType == DATATYPE_DD_ITEM_QUICK_DROP)
    {
        if (IsChildOf(M3D_APP))
        {
            OnDDItemQuickDrop(data);
        }
    }
    else if (dataType == DATATYPE_DD_DROP)
    {
        if (IsChildOf(M3D_APP))
        {
            OnDDDrop(data);
        }
    }
    else
    {
        SaleWnd::GameDataUpdate(data, dataType);
    }
    return 1;
}

int ShopWnd::OnAfterAddToWndStation()
{
    // RVA 0x473300
    int const res = m3d::ui::Wnd::OnAfterAddToWndStation();
    M3D_APP->EnqueueMessage(MSG_TAB_SWITCHED, 0, 0, 0, 0, CStr(), m3d::AIParam());
    return res;
}

void ShopWnd::OnDDDrop(void* data)
{
    // RVA 0x472FC0 - an item dropped onto one of this shop's own tabs is offered
    // to the first tab that is currently off-screen and will take it; that tab is
    // then brought to the front.
    if ((m_gameDataFlags & 1) == 0 || !data || !IsChildOf(M3D_APP))
    {
        return;
    }

    ItemAcceptInfo info(*static_cast<ItemAcceptInfo*>(data));
    if (!info.m_eventSrcWnd)
    {
        return;
    }

    // The drop only counts if it started on one of our own tab windows.
    bool fromOwnTab = false;
    for (TabItemsMap::iterator it = m_wndTabItems.begin(); it != m_wndTabItems.end(); ++it)
    {
        if (it->second && it->second.get() == info.m_eventSrcWnd)
        {
            fromOwnTab = true;
            break;
        }
    }
    if (!fromOwnTab)
    {
        return;
    }

    for (TabItemsMap::iterator it = m_wndTabItems.begin(); it != m_wndTabItems.end(); ++it)
    {
        m3d::ui::Wnd* tabWnd = it->second.get();
        if (!tabWnd)
        {
            continue;
        }
        // A tab that is already on screen is not a drop target.
        if (!tabWnd->IsKindOf(&DragDropItemsWnd::m_classDragDropItemsWnd) || tabWnd->IsChildOf(M3D_APP))
        {
            continue;
        }
        if (static_cast<DragDropItemsWnd*>(tabWnd)->CanAddDragItem(false))
        {
            info.m_eventSrcWnd = tabWnd;
            M3D_APP->m_pInterfaceManager->LaunchEvent(EVENT_ITEM_DROPPED_ON_TAB, GUI_EVENT_CUSTOM, &info);
            m_wndTab->SetCurSel(GetTabIdByTabType(it->first));
            return;
        }
    }
}

void ShopWnd::OnDDItemQuickDrop(void* data)
{
    // RVA 0x473190 - a shift-click sends the item to the first off-screen tab
    // that accepts it, and switches to that tab.
    if ((m_gameDataFlags & 1) == 0 || !data || !IsChildOf(M3D_APP))
    {
        return;
    }

    ItemAcceptInfo info(*static_cast<ItemAcceptInfo*>(data));
    if (!info.m_eventSrcWnd)
    {
        return;
    }

    for (TabItemsMap::iterator it = m_wndTabItems.begin(); it != m_wndTabItems.end(); ++it)
    {
        m3d::ui::Wnd* tabWnd = it->second.get();
        if (!tabWnd || tabWnd->IsChildOf(M3D_APP))
        {
            continue;
        }

        int accepted = 0;
        if (tabWnd->IsKindOf(&RepositoryWnd::m_classRepositoryWnd))
        {
            accepted = static_cast<RepositoryWnd*>(tabWnd)->UpdateOnItemQuickDrop(info);
        }
        else if (tabWnd->IsKindOf(&WareWnd::m_classWareWnd))
        {
            accepted = static_cast<WareWnd*>(tabWnd)->UpdateOnItemQuickDrop(info);
        }
        else
        {
            continue;
        }

        if (accepted)
        {
            m_wndTab->SetCurSel(GetTabIdByTabType(it->first));
            return;
        }
    }
}
