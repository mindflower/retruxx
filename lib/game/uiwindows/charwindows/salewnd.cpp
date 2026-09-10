#include "salewnd.h"

#include "repositorywnd.h"
#include "warewnd.h"

#include <core/aiparam.h>
#include <core/kernel.h>
#include <core/log.h>
#include <game/m3dgame.h>
#include <game/uimanager/gameuimanager.h>
#include <game/uimanager/truxxuimanager.h>
#include <m3dapp.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/player.h>
#include <server/objects/town.h>
#include <server/objects/vehicle.h>
#include <server/resourcemanager.h>
#include <server/server.h>
#include <ui/image.h>
#include <ui/wndstation.h>

RT_CLASS_EXPORTS_BEGIN(SaleWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SaleWnd);

SaleWnd::AuxInfo SaleWnd::m_aif;

namespace
{
    // Ids the XML gives the tab control and the town button.
    unsigned const ID_WND_TAB = 300000;
    unsigned const ID_BTN_TOWN = 300003;

    // TabWnd reports a selection change with this message; the AIParam id list
    // carries {oldTabIdx, newTabIdx, selectionType}.
    unsigned const MSG_TAB_SEL_CHANGED = 6;

    // Application messages this window enqueues.
    int const MSG_LAUNCH_TOWN_DLG = 66540;
    int const MSG_TAB_SWITCHED = 65691;

    // Interface-manager events raised for the tutorial / sound layer.
    int const EVENT_TAB_WORK_WND_SHOWN = 102;
    int const EVENT_ITEM_DROPPED_ON_TAB = 97;

    // GameDataUpdate data types.
    int const DATATYPE_PLAYER_VEHICLE_CHANGED = 64;
}  // namespace

// ===========================================================================
//  SaleWnd::AuxInfo
// ===========================================================================

SaleWnd::AuxInfo::AuxInfo() :
    m_wndTabName("wndTab"),
    m_tabNames{CStr("GunsAndGadgets"), CStr("Goods")},
    m_wndRepositoryTabItemName("wndRepositoryTabItem"),
    m_wndGoodsTabItemName("wndGoodsTabItem"),
    m_wndTabItemsIds{0x20, 0x23},
    m_tabBtnSz(96.0f, 48.0f),
    m_tabBtnSpace(10.0f),
    m_wndTownPictureName("wndTownPicture"),
    m_townPictureTexName("InventoryTown")
{
    // RVA 0x46FA00
}

SaleWnd::AuxInfo::AuxInfo(SaleWnd::AuxInfo const& rhs) :
    m_wndTabName(rhs.m_wndTabName),
    m_tabNames{rhs.m_tabNames[0], rhs.m_tabNames[1]},
    m_wndRepositoryTabItemName(rhs.m_wndRepositoryTabItemName),
    m_wndGoodsTabItemName(rhs.m_wndGoodsTabItemName),
    m_wndTabItemsIds{rhs.m_wndTabItemsIds[0], rhs.m_wndTabItemsIds[1]},
    m_tabBtnSz(rhs.m_tabBtnSz),
    m_tabBtnSpace(rhs.m_tabBtnSpace),
    m_wndTownPictureName(rhs.m_wndTownPictureName),
    m_townPictureTexName(rhs.m_townPictureTexName)
{
}

// ===========================================================================
//  SaleWnd - construction
// ===========================================================================

m3d::Class* SaleWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ChildPanel);
}

m3d::Class* SaleWnd::GetRtClass() const
{
    return RT_CLASS_LOCAL(SaleWnd);
}

SaleWnd::SaleWnd() :
    m_wndTab(nullptr),
    m_workshopId(-1),
    m_buildingType(ai::INVALID_BUILDINGTYPE),
    m_wndTownPicture(nullptr)
{
    // RVA 0x46FC20 - m_vehicleId is reset to -1 here too, on top of what the
    // ChildPanel base ctor already did.
    m_vehicleId = -1;
}

SaleWnd::SaleWnd(SaleWnd const&) :
    // NOTE: the shipped copy ctor (RVA 0x46FC20 shares its body with the default
    // one) initialises everything the same way and copies nothing from rhs.
    m_wndTab(nullptr),
    m_workshopId(-1),
    m_buildingType(ai::INVALID_BUILDINGTYPE),
    m_wndTownPicture(nullptr)
{
    m_vehicleId = -1;
}

SaleWnd::~SaleWnd()
{
    // RVA 0x46FD50 - the two maps and the tab-type vector free themselves.
}

// ===========================================================================
//  SaleWnd - setup
// ===========================================================================

int SaleWnd::GameDataSetup()
{
    // RVA 0x46FE00
    if ((m_gameDataFlags & 2) == 0)
    {
        int res = 1;

        m3d::Object* wndTownPicture = GetChildByName(m_aif.m_wndTownPictureName);
        if (wndTownPicture && wndTownPicture->IsKindOf(&m3d::ui::ImageWnd::m_classImageWnd))
        {
            m_wndTownPicture = static_cast<m3d::ui::ImageWnd*>(wndTownPicture);
        }
        else
        {
            M3D_LOG_INFO(
                "Get control error: control " + m_aif.m_wndTownPictureName + " is not found or incorrect type");
            res = 0;
        }

        m3d::Object* wndTab = GetChildByName(m_aif.m_wndTabName);
        if (wndTab && wndTab->IsKindOf(&m3d::ui::TabWnd::m_classTabWnd))
        {
            m_wndTab = static_cast<m3d::ui::TabWnd*>(wndTab);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndTabName + " is not found or incorrect type");
            res = 0;
        }

        if (m_wndTab)
        {
            m3d::ui::TabButtonInfo btnInfo;
            btnInfo.SetIzvrat(
                m_aif.m_tabBtnSz.x, m_aif.m_tabBtnSz.y, m_aif.m_tabBtnSpace, m3d::ui::TabButtonInfo::ALIGNMENT_LEFT);
            m_wndTab->SetTabButtonInfo(btnInfo);
        }

        // The concrete window decides which tabs it wants; each one is built up
        // front and parked in m_allTabItems, and CreateTabItems later moves the
        // ones the current workshop actually has into m_wndTabItems.
        SetupListOfTabTypes();
        for (int i = 0; i < static_cast<int>(m_tabTypes.size()); ++i)
        {
            m3d::ui::Wnd* tabItem = CreateTabItem(m_tabTypes[i]);
            if (tabItem)
            {
                m_allTabItems.insert(TabItemsPair(m_tabTypes[i], ref_ptr<m3d::ui::Wnd>(tabItem)));
            }
            else
            {
                res = 0;
            }
        }

        // Nothing under a sale window may take the input focus, so the whole
        // subtree is stripped of both activation styles.
        std::vector<m3d::Object*> stack;
        stack.push_back(this);
        while (!stack.empty())
        {
            m3d::Object* wnd = stack.back();
            stack.pop_back();
            for (m3d::Object* child = wnd->GetFirstChild(); child; child = child->GetNextSibling())
            {
                assert(child && child->IsKindOf(&m3d::ui::Wnd::m_classWnd));
                auto* childWnd = static_cast<m3d::ui::Wnd*>(child);
                childWnd->SetStyle(
                    childWnd->GetStyle() &
                    ~static_cast<unsigned int>(
                        m3d::ui::WS_ACTIVATION_CAPTURES_FOCUS | m3d::ui::WS_ACTIVATABLE));
                if (child->GetFirstChild())
                {
                    stack.push_back(child);
                }
            }
        }

        if (res)
        {
            m_gameDataFlags |= 1u;
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("SaleWnd::GameDataSetup error");
    return 0;
}

int SaleWnd::GameDataClear(bool)
{
    // RVA 0x4703D0
    ClearTabItems();
    return 1;
}

int SaleWnd::GameDataUpdate(void*, int dataType)
{
    // RVA 0x470320
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == DATATYPE_PLAYER_VEHICLE_CHANGED)
    {
        ai::Vehicle* vehicle = ai::thePlayer ? ai::thePlayer->GetVehicle() : nullptr;
        SetVehicleId(vehicle ? vehicle->GetId() : -1);
    }
    return 1;
}

int SaleWnd::OnBeforeAddToWndStation()
{
    // RVA 0x4704E0 - re-fire the tab notification so the visible tab's work
    // window gets its "shown" event even though nothing was clicked.
    if ((m_gameDataFlags & 1) != 0)
    {
        int const curSel = m_wndTab->GetCurSel();
        if (static_cast<unsigned int>(curSel) <= 1)
        {
            UpdateOnChangeTabSel(curSel, -1, m3d::ui::TabWnd::BY_PROGRAM);
        }
    }
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

int SaleWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data)
{
    // RVA 0x470730
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (ChildPanel::OnWndNotify(from, id, msg, data))
    {
        return 1;
    }

    if (id == ID_WND_TAB)
    {
        if (msg != MSG_TAB_SEL_CHANGED)
        {
            return 0;
        }
        retruxx::vector<int> const params = data.GetAsIdList();
        if (params.size() != 3)
        {
            return 0;
        }
        UpdateOnChangeTabSel(params[1], params[0], static_cast<m3d::ui::TabWnd::SelectionType>(params[2]));
        return 1;
    }

    if (id == ID_BTN_TOWN && msg == 1)
    {
        LaunchTownDlg();
        return 1;
    }
    return 0;
}

int SaleWnd::SetUpForWorkshop(int workshopId)
{
    // RVA 0x470380
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    m_workshopId = workshopId;
    ClearTabItems();
    if (m_workshopId != -1)
    {
        CreateTabItems();
    }
    UpdateTownPicture();
    return 1;
}

int SaleWnd::LaunchTownDlg()
{
    // RVA 0x470430
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (ai::Workshop* workshop = GetWorkshop())
    {
        M3D_APP->EnqueueMessage(MSG_LAUNCH_TOWN_DLG, workshop->GetParentId(), 0, 0, 0, CStr(), m3d::AIParam());
    }
    return 1;
}

ai::Workshop* SaleWnd::GetWorkshop() const
{
    // RVA 0x4716B0
    if (m_workshopId == -1)
    {
        return nullptr;
    }
    return RT_DYNCAST(ai::theObjects->GetEntityByObjId(m_workshopId), ai::Workshop);
}

// ===========================================================================
//  SaleWnd - tabs
// ===========================================================================

ai::WorkshopRepositoryType SaleWnd::GetWorkshopRepositoryType(SaleWnd::WorkshopTabType tabType)
{
    // RVA 0x470520 - only the weapons tab is backed by a workshop repository;
    // the goods tab draws its contents from the ware list instead.
    if (tabType == TABTYPE_GUNS_AND_GADGETS)
    {
        return ai::WORKSHOP_GUNS_AND_GADGETS;
    }
    M3D_LOG_INFO("Error: invalid workshop tab type: " + CStr(static_cast<int>(tabType)));
    return ai::WORKSHOP_NUM_TYPES;
}

bool SaleWnd::IsTabCreatedFromRepositorySource(SaleWnd::WorkshopTabType tabType) const
{
    // RVA 0x471690
    return tabType == TABTYPE_GUNS_AND_GADGETS;
}

std::vector<int, std::allocator<int>> SaleWnd::GetResourceIdsForTabItem(SaleWnd::WorkshopTabType tabType)
{
    // RVA 0x470600
    std::vector<int> resourceIds;
    if (tabType == TABTYPE_GUNS_AND_GADGETS)
    {
        resourceIds.push_back(ai::theResourceManager->GetResourceId(CStr("GUN")));
        resourceIds.push_back(ai::theResourceManager->GetResourceId(CStr("GADGET")));
    }
    else if (tabType == TABTYPE_GOODS)
    {
        resourceIds.push_back(ai::theResourceManager->GetResourceId(CStr("GOODS")));
    }
    return resourceIds;
}

void SaleWnd::SetupListOfTabTypes()
{
    // RVA 0x4716A0 - the base class asks for no tabs at all; ShopWnd overrides
    // this. NOTE: the PDB marks this pure virtual, but the shipped binary does
    // emit an empty body for it, so it is defined here rather than declared = 0.
}

m3d::ui::Wnd* SaleWnd::CreateTabItem(SaleWnd::WorkshopTabType type)
{
    // RVA 0x4709D0 - each tab's work window is cloned from a pattern child of
    // this window, detached from its parent, positioned in the tab's client area
    // and handed to the interface manager under a fixed id.
    m3d::Object* item = nullptr;
    m3d::ui::Wnd* pattern = nullptr;
    CStr const* patternName = nullptr;

    if (type == TABTYPE_GUNS_AND_GADGETS)
    {
        patternName = &m_aif.m_wndRepositoryTabItemName;
        m3d::Object* child = GetChildByName(*patternName);
        if (!child || !child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            M3D_LOG_INFO("Make control error: control " + *patternName + " is not found or incorrect type");
        }
        else
        {
            pattern = static_cast<m3d::ui::Wnd*>(child);
            item = M3D_KERNEL->New("RepositoryWnd");
            if (!item)
            {
                M3D_LOG_INFO(
                    "Make control error: cannot create " + *patternName + " - cannot find rtti class " +
                    CStr("RepositoryWnd"));
            }
            else if (!static_cast<RepositoryWnd*>(item)->CreateFromPattern(pattern, true))
            {
                M3D_LOG_INFO("Make control error: cannot create " + *patternName + " from pattern class");
                item = nullptr;
            }
        }
    }
    else if (type == TABTYPE_GOODS)
    {
        patternName = &m_aif.m_wndGoodsTabItemName;
        m3d::Object* child = GetChildByName(*patternName);
        if (!child || !child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            M3D_LOG_INFO("Make control error: control " + *patternName + " is not found or incorrect type");
        }
        else
        {
            pattern = static_cast<m3d::ui::Wnd*>(child);
            item = M3D_KERNEL->New("WareWnd");
            if (!item)
            {
                M3D_LOG_INFO(
                    "Make control error: cannot create " + *patternName + " - cannot find rtti class " +
                    CStr("WareWnd"));
            }
            else if (!static_cast<WareWnd*>(item)->CreateFromPattern(pattern, true))
            {
                M3D_LOG_INFO("Make control error: cannot create " + *patternName + " from pattern class");
                item = nullptr;
            }
        }
    }

    if (!item)
    {
        M3D_LOG_INFO("SaleWnd::CreateTabItem error - cannot create item");
        return nullptr;
    }

    auto* itemWnd = static_cast<m3d::ui::Wnd*>(item);
    itemWnd->SetStyle(itemWnd->GetStyle() | m3d::ui::WS_IS_VISIBLE);

    auto* parent = RT_DYNCAST(itemWnd->GetParent(), m3d::ui::Wnd);
    if (parent)
    {
        parent->RemoveChild(itemWnd);
    }
    if (parent)
    {
        // The pattern's bounds are in the pattern's parent space; re-express them
        // in the tab control's space so the work window lands inside the tab.
        BoundsBase<float> const patB = itemWnd->GetBounds();
        PointBase<float> const origin = parent->ToScreen(PointBase<float>(patB.x0, patB.y0));
        BoundsBase<float> newB(
            origin.x, origin.y, origin.x + patB.width, origin.y + patB.height);
        m_wndTab->ToWindow(newB);

        if (itemWnd->IsKindOf(&RepositoryWnd::m_classRepositoryWnd))
        {
            auto* repositoryWnd = static_cast<RepositoryWnd*>(itemWnd);
            repositoryWnd->SetBounds(newB, false);
            repositoryWnd->AdjustWndToCells();
        }
        else
        {
            itemWnd->SetBounds(newB, true);
        }
    }

    if (M3D_APP->m_pInterfaceManager->AddWindowById(itemWnd, m_aif.m_wndTabItemsIds[type], 1, 0))
    {
        return itemWnd;
    }
    return nullptr;
}

int SaleWnd::SetupTabInfo(m3d::ui::Wnd* itemWnd, SaleWnd::WorkshopTabType type)
{
    // RVA 0x4712B0 - gives the tab its caption and its two icons, then records
    // the work window as the live item for that tab type.
    if (!itemWnd || type == TABTYPE_NUM_TABTYPES)
    {
        return 0;
    }

    m3d::ui::TabItemInfo tabInfo;
    CStr diz;
    if (M3D_APP->GetStringByStringId(diz, m_aif.m_tabNames[type]))
    {
        tabInfo.m_diz = diz;
    }
    tabInfo.m_image = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_tabNames[type], 0);
    tabInfo.m_imageDisabled = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_tabNames[type], 1);

    itemWnd->SetInt(type);
    if (m_wndTab->AddItem(ref_ptr<m3d::ui::Wnd>(itemWnd), tabInfo) == -1)
    {
        return 0;
    }
    m_wndTabItems.insert(TabItemsPair(type, ref_ptr<m3d::ui::Wnd>(itemWnd)));
    return 1;
}

void SaleWnd::AddTabItem(SaleWnd::WorkshopTabType tabType)
{
    // RVA 0x4714F0 - the weapons tab only appears when the workshop actually has
    // a guns-and-gadgets repository; the goods tab is always allowed.
    ai::Workshop* workshop = GetWorkshop();
    if (!workshop)
    {
        return;
    }
    if (tabType == TABTYPE_GUNS_AND_GADGETS &&
        !workshop->GetRepositoryByType(GetWorkshopRepositoryType(TABTYPE_GUNS_AND_GADGETS)))
    {
        return;
    }

    TabItemsMap::iterator it = m_allTabItems.find(tabType);
    if (it == m_allTabItems.end())
    {
        return;
    }
    m3d::ui::Wnd* itemWnd = it->second.get();
    if (!itemWnd)
    {
        return;
    }
    SetupTabInfo(itemWnd, tabType);
    SetupTabItem(itemWnd, tabType);
}

void SaleWnd::CreateTabItems()
{
    // RVA 0x4714A0
    for (int i = 0; i < static_cast<int>(m_tabTypes.size()); ++i)
    {
        AddTabItem(m_tabTypes[i]);
    }
    m_wndTab->SetCurSel(0);
}

int SaleWnd::SetupTabItem(m3d::ui::Wnd* tabItem, SaleWnd::WorkshopTabType tabType)
{
    // RVA 0x471580
    if (!tabItem || tabType == TABTYPE_NUM_TABTYPES)
    {
        return 0;
    }
    ai::Workshop* workshop = GetWorkshop();
    if (!workshop)
    {
        return 0;
    }

    if (tabItem->IsKindOf(&WareWnd::m_classWareWnd))
    {
        static_cast<WareWnd*>(tabItem)->SetupForWorkshop(m_workshopId);
        return 1;
    }
    if (!tabItem->IsKindOf(&RepositoryWnd::m_classRepositoryWnd))
    {
        return 0;
    }

    ai::GeomRepository* repository = workshop->GetRepositoryByType(GetWorkshopRepositoryType(tabType));
    if (!repository)
    {
        return 0;
    }
    auto* repositoryWnd = static_cast<RepositoryWnd*>(tabItem);
    repositoryWnd->SetRepository(repository, static_cast<RepositoryWnd::RepositoryType>(2));

    // Everything is drawn with style 12 by default, and the resources this tab
    // sells are then switched back to the plain style 4.
    std::vector<int> const rcIds = GetResourceIdsForTabItem(tabType);
    repositoryWnd->SetSpecialized(rcIds);
    repositoryWnd->SetItemDrawStyleByResourceId(12, -1);
    for (int i = 0; i < static_cast<int>(rcIds.size()); ++i)
    {
        repositoryWnd->SetItemDrawStyleByResourceId(4, rcIds[i]);
    }
    return 1;
}

void SaleWnd::ClearTabItems()
{
    // RVA 0x4703E0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    m_wndTabItems.clear();
    m_wndTab->RemoveAllItems();
}

void SaleWnd::UpdateOnChangeTabSel(int newTabIdx, int, m3d::ui::TabWnd::SelectionType selectionType)
{
    // RVA 0x470800
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    ref_ptr<m3d::ui::Wnd> workWnd = GetWorkWndForTabItem(newTabIdx);
    if (workWnd)
    {
        M3D_APP->m_pInterfaceManager->LaunchEvent(
            EVENT_TAB_WORK_WND_SHOWN, GUI_EVENT_CUSTOM, reinterpret_cast<void*>(workWnd->GetInt()));
    }

    // Only a deliberate switch feeds the tutorial; a programmatic one does not.
    if (selectionType == m3d::ui::TabWnd::BY_MOUSE || selectionType == m3d::ui::TabWnd::BY_KB)
    {
        SaleWnd::WorkshopTabType const tabType = GetTabTypeByTabId(newTabIdx);
        if (tabType == TABTYPE_GUNS_AND_GADGETS)
        {
            ai::pServer->PostPlayerEvent(ai::GE_TUTORIAL_TAB_WEAPON);
            M3D_APP->EnqueueMessage(MSG_TAB_SWITCHED, 0, 0, 0, 0, CStr(), m3d::AIParam());
        }
        else if (tabType == TABTYPE_GOODS)
        {
            ai::pServer->PostPlayerEvent(ai::GE_TUTORIAL_TAB_GOODS);
            M3D_APP->EnqueueMessage(MSG_TAB_SWITCHED, 0, 0, 0, 0, CStr(), m3d::AIParam());
        }
    }
}

void SaleWnd::UpdateTownPicture()
{
    // RVA 0x471740 - the picture is looked up by the workshop's own name first,
    // then by the town's name, and finally falls back to a generic image.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    ai::Workshop* workshop = GetWorkshop();
    if (!workshop)
    {
        m_wndTownPicture->ShowWindow(false);
        return;
    }

    m_wndTownPicture->ShowWindow(true);
    m3d::rend::TexHandle tex = M3D_APP->m_pInterfaceManager->GetIcoByName(workshop->GetName(), 0);
    if (!tex.IsValid())
    {
        auto* town = RT_DYNCAST(workshop->GetParent(), ai::Town);
        if (town)
        {
            tex = M3D_APP->m_pInterfaceManager->GetIcoByName(town->GetName(), 0);
            if (!tex.IsValid())
            {
                tex = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_townPictureTexName, 0);
            }
        }
    }
    m_wndTownPicture->SetImage(tex);
}

ref_ptr<m3d::ui::Wnd> SaleWnd::GetWorkWndForTabItem(int tabId) const
{
    // RVA 0x471710
    m3d::ui::TabItem const* item = m_wndTab->GetItem(tabId);
    return item ? item->GetWorkWnd() : ref_ptr<m3d::ui::Wnd>();
}

int SaleWnd::GetTabIdByTabType(SaleWnd::WorkshopTabType tabType) const
{
    // RVA 0x471870 - the map says which window belongs to the type; the tab
    // control is then scanned for the tab holding that same window.
    if ((m_gameDataFlags & 1) == 0)
    {
        return -1;
    }
    TabItemsMap::const_iterator it = m_wndTabItems.find(tabType);
    if (it == m_wndTabItems.end())
    {
        return -1;
    }
    ref_ptr<m3d::ui::Wnd> wanted = it->second;
    if (!wanted)
    {
        return -1;
    }

    for (int i = 0; i < m_wndTab->GetItemsCount(); ++i)
    {
        ref_ptr<m3d::ui::Wnd> workWnd = GetWorkWndForTabItem(i);
        if (workWnd.get() == wanted.get())
        {
            return i;
        }
    }
    return -1;
}

SaleWnd::WorkshopTabType SaleWnd::GetTabTypeByTabId(int tabId) const
{
    // RVA 0x471970 - the inverse of GetTabIdByTabType.
    ref_ptr<m3d::ui::Wnd> workWnd = GetWorkWndForTabItem(tabId);
    if (!workWnd)
    {
        return TABTYPE_INVALID;
    }
    for (TabItemsMap::const_iterator it = m_wndTabItems.begin(); it != m_wndTabItems.end(); ++it)
    {
        if (it->second.get() == workWnd.get())
        {
            return it->first;
        }
    }
    return TABTYPE_INVALID;
}
