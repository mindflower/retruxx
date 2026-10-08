#include "motherpanel.h"
#include "playermoneywnd.h"
#include "childpanel.h"
#include <core/log.h>
#include "ui/image.h"
#include "ui/button.h"
#include "motherpaneltabbutton.h"
#include "groundwnd.h"
#include <i_event.h>
#include <game/uiwindows/palmwindows/localmapwnd.h>
#include <game/uimanager/uidefs.h>
#include <game/music/townmusicmanager.h>
#include <server/server.h>
#include "childpanel.h"
#include <game/uiwindows/townwindows/towndlg.h>
#include <game/uimisc/guihelper.h>
#include <server/objects/town.h>
#include <server/objects/player.h>
#include <server/objects/vehicle.h>
#include <server/objects/bar.h>
#include <server/objects/basket.h>
#include <server/objects/cabin.h>
#include <server/objects/workshop.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/physicbodies/vehiclepart.h>
#include <game/m3dgame.h>
#include <game/uiwindows/townwindows/bardlg.h>
#include <impulses/i_impulses.h>
#include <ui/ui_srv.h>
#include "znayukakprodatwnd.h"
#include "garagewnd.h"
#include "inventorywnd.h"
#include "salewnd.h"
#include "workshopvehiclewnd.h"

RT_CLASS_EXPORT_METHOD_DEFINE(MotherPanel, LeaveTown)
{
    auto* mp = static_cast<MotherPanel*>(context->asObject(0, "MotherPanel"));
    mp->LeaveTown(context->asBool(1));
    return 1;
}

RT_CLASS_EXPORTS_BEGIN(MotherPanel)
RT_CLASS_EXPORT(MotherPanel, m3d::METHOD, LeaveTown, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MotherPanel);

void MotherPanel::AuxSuspendedShow::Reset()
{
    m_previousPanelsToRemain.clear();
    m_suspendedPanels.clear();
}

MotherPanel::AuxSuspendedShow::AuxSuspendedShow() = default;

MotherPanel::AuxInfo::AuxInfo()
{
    m_tabBtnName = "tabBtn_";
    m_wndDecorName = "wndDecor";
    m_wndDecorBarName = "wndDecorBar";
    m_btnExitName = "btnExit";
    m_wndTopPanelName = "wndTopPanel";
    m_pickUpSoundName = "SOUND_PICKUP_ITEMS_FROM_GROUND";
}

void MotherPanel::LeaveTown(bool bQuick)
{
    if (M3D_APP->m_pInterfaceManager->GetCurrentTown() &&
        (IsInTownRoot() || M3D_APP->m_pInterfaceManager->IsWindowVisible(37)))
    {
        Hide(true, bQuick);
    }
}

CStr MotherPanel::Tab2Str(MotherPanel::Tab tabId)
{
    static retruxx::map<Tab, CStr> const converter = {
        {TAB_QUESTLOG, "Questlog"},
        {TAB_MAP, "Map"},
        {TAB_JOURNAL, "Journal"},
        {TAB_INVENTORY_VS_SHOP, "Inventory"},
        {TAB_CHARACTERISTIC_VS_WORKSHOP, "Characteristic"},
        {TAB_BAR, "Bar"},
        {TAB_ADDITIONAL_BUILDING, "AdditionalBuilding"},
    };

    auto const it = converter.find(tabId);
    if (it != converter.end())
    {
        return it->second;
    }
    return {};
}

m3d::Class* MotherPanel::GetClass() const
{
    return RT_CLASS_LOCAL(MotherPanel);
}

m3d::Object* MotherPanel::CreateObject()
{
    return new MotherPanel;
}

m3d::Object* MotherPanel::Clone()
{
    return new MotherPanel;
}

m3d::Class* MotherPanel::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModalWnd);
}

bool MotherPanel::IsInTownRoot() const
{
    return M3D_APP->m_pInterfaceManager->GetCurrentTown() && IsPanelPresent(4) && m_panels.size() == 1;
}

MotherPanel::~MotherPanel()
{
    for (auto* tabBtn : m_tabButtons)
    {
        delete tabBtn;
    }
}

void MotherPanel::UpdateTabButtonsOnEnterTown(ai::Town const* town)
{
    // RVA 0x45F720
    if ((m_gameDataFlags & 1) == 0 || !town)
    {
        return;
    }

    // SetMode is inlined in the original: only the building tabs (3..6) switch, and only when
    // the current town has that building.
    for (auto* tabButton : m_tabButtons)
    {
        if (tabButton)
        {
            tabButton->SetMode(MotherPanelTabButton::MODE_IN_TOWN);
        }
    }

    // ShowTabButton, inlined.
    if (help::GetBarWithBarmanForTown(town))
    {
        ShowTabButton(TAB_BAR, true);
    }
    if (help::GetBarWithoutBarmanForTown(town))
    {
        ShowTabButton(TAB_ADDITIONAL_BUILDING, true);
    }
}

int MotherPanel::OnBeforeAddToWndStation()
{
    return Wnd::OnBeforeAddToWndStation();
}

void MotherPanel::OnBtnExitClick(m3d::ui::Wnd*, int)
{
    OnEscape();
}

void MotherPanel::ClearPanels(std::vector<ChildPanelId> const& previousPanelsToRemain)
{
    // RVA 0x460020
    // The next node is taken first: RemoveChildPanel erases the current one from m_panels.
    for (auto it = m_panels.begin(); it != m_panels.end();)
    {
        auto const cur = it++;
        bool const remains = std::find(previousPanelsToRemain.begin(), previousPanelsToRemain.end(), cur->first) !=
                             previousPanelsToRemain.end();
        if (!remains && cur->second)
        {
            RemoveChildPanel(cur->second);
        }
    }
}

void MotherPanel::OnEscape()
{
    bool const hasOnlyBuilding = GetOnlyBuilding() != nullptr;

    if (IsPanelPresent(94))
    {
        ref_ptr wnd = M3D_APP->m_pInterfaceManager->GetWindow(94);
        if (auto* znayu = RT_DYNCAST(wnd.get(), ZnayuKakProdatWnd); znayu && znayu->IsChildOf(M3D_APP))
        {
            znayu->Cancel();
            return;
        }
        Hide(false, false);
        return;
    }

    if (IsPanelPresent(73))
    {
        OnWorkshop();
        return;
    }

    int msgArg;
    if (IsPanelPresent(2))
    {
        if (hasOnlyBuilding)
        {
            Hide(false, false);
            return;
        }
        msgArg = 2;
    }
    else if (IsPanelPresent(3))
    {
        if (hasOnlyBuilding)
        {
            Hide(false, false);
            return;
        }
        msgArg = 3;
    }
    else
    {
        if (IsPanelPresent(88))
        {
            return;
        }
        if (InTown() && m_curTabId != TAB_NUM_TABS && !hasOnlyBuilding)
        {
            OnTown();
            return;
        }
        Hide(false, false);
        return;
    }

    M3D_APP->EnqueueMessage(65673, msgArg, 0, 0, 0, {}, {});
}

int MotherPanel::RemoveChildForce(m3d::Object* wnd)
{
    // RVA 0x45E270 - the same as RemoveChild, over Wnd::RemoveChildForce.
    int const res = Wnd::RemoveChildForce(wnd);
    if (IS_KIND_OF(wnd, ChildPanel) && res)
    {
        for (auto it = m_panels.begin(); it != m_panels.end(); ++it)
        {
            if (it->second == wnd)
            {
                m_panels.erase(it);
                break;
            }
        }

        AdjustAnimationOnHidePanel(static_cast<ChildPanel*>(wnd));
        AdjustDecor();
    }
    return res;
}

MotherPanel::ChildPanelId MotherPanel::GetCurrentPanelIdByGuiId(int guiId) const
{
    // Iterate through all panels in the map
    for (auto const& panelPair : m_panels)
    {
        // Check if panel exists and has matching GUI ID
        if (panelPair.second && panelPair.second->GetGuiId() == guiId)
        {
            return panelPair.first;  // Return the panel ID
        }
    }

    // No panel found with this GUI ID
    return ChildPanelId::PANEL_INVALID;
}

void MotherPanel::OnHidePanel(void* data)
{
    if (!data)
    {
        return;
    }

    int const guiId = static_cast<m3d::Event*>(data)->m_intEv[0];
    ChildPanelId const panelId = GetCurrentPanelIdByGuiId(guiId);
    if (panelId == PANEL_INVALID)
    {
        return;
    }

    if (m_panels.size() == 1 || panelId == PANEL_FULLSCREEN || panelId == PANEL_TOWN)
    {
        Hide(false, false);
        return;
    }

    bool const removed = RemoveChildPanelById(panelId) != 0;
    if (InTown() && m_panels.size() == (removed ? 1u : 2u) && IsPanelPresent(4))
    {
        m_curTabId = TAB_INVALID;
        SelectTabButton(TAB_INVALID);
        M3D_APP->GetTownMusicManager()->StopAmbient();
    }
}

void MotherPanel::ToggleTab(Tab tabId)
{
    // RVA 0x460D30
    if (tabId != TAB_NUM_TABS)
    {
        if (m_curTabId == tabId && IsChildOf(M3D_APP))
        {
            if (M3D_APP->m_pInterfaceManager->GetCurrentTown())
            {
                OnTown();
            }
            else
            {
                Hide(false, false);
            }
        }
        else
        {
            SetCurTab(tabId, true);
        }
    }
}

void MotherPanel::AdjustAnimationOnShowPanels(
    std::vector<std::pair<ChildPanelId, int>, std::allocator<std::pair<ChildPanelId, int>>> const& panels)
{
    // RVA 0x4611C0
    // When one palm window replaces another, both switch without animation. Only the first
    // palm entry is looked at.
    for (auto const& [panelId, newGuiId] : panels)
    {
        if (panelId != PANEL_PALM)
        {
            continue;
        }

        if (newGuiId == -1)
        {
            return;
        }
        auto const it = m_panels.find(PANEL_PALM);
        if (it == m_panels.end() || !it->second)
        {
            return;
        }
        int const oldGuiId = it->second->GetGuiId();
        if (oldGuiId == -1)
        {
            return;
        }

        // NOTE: the original does not compare the two ids, so re-showing the palm window that is
        // already up also makes it skip both its show and hide animations.
        auto newPalmWnd = M3D_APP->m_pInterfaceManager->GetWindow(newGuiId);
        auto oldPalmWnd = M3D_APP->m_pInterfaceManager->GetWindow(oldGuiId);
        if (newPalmWnd)
        {
            newPalmWnd->SetOnShowAnimationImmediate(true);
        }
        if (oldPalmWnd)
        {
            oldPalmWnd->SetOnHideAnimationImmediate(true);
        }
        return;
    }
}

void MotherPanel::OnMap()
{
    // RVA 0x45ECD0
    // NOTE: tests the whole m_gameDataFlags word, like OnAdditionalBuilding.
    if (m_gameDataFlags != 0)
    {
        std::vector<std::pair<MotherPanel::ChildPanelId, int>> panels;
        if (M3D_APP->m_pInterfaceManager->GetCurrentTown() && !IsPanelPresent(4))
        {
            panels.push_back({PANEL_TOWN, 4});
        }

        panels.push_back({PANEL_PALM, m_bCurMapLocal ? IW_WND_LOCAL_MAP : IW_WND_GLOBAL_MAP});

        ai::pServer->PostPlayerEvent(ai::GE_TUTORIAL_MAP);
        ShowPanels(panels, {PANEL_TOWN});
    }
}

void MotherPanel::OnAdditionalBuilding()
{
    // RVA 0x45FDB0. The "additional building" tab is the town's second bar - the
    // one that has no barman - reusing the very same BarDlg window class.
    // NOTE: this and its sibling handlers below test the whole m_gameDataFlags
    // word rather than bit 1 as the rest of this file does; preserved as shipped.
    if (m_gameDataFlags == 0)
    {
        return;
    }

    ai::Town const* town = M3D_APP->m_pInterfaceManager->GetCurrentTown();
    if (!town)
    {
        return;
    }

    ai::Bar* bar = help::GetBarWithoutBarmanForTown(town);
    if (!bar)
    {
        return;
    }

    ref_ptr barWnd = M3D_APP->m_pInterfaceManager->GetWindow(3);
    auto* barDlg = RT_DYNCAST(barWnd.get(), BarDlg);
    if (!barDlg || !barDlg->SetUpForBar(bar->GetId()))
    {
        return;
    }

    RemoveChildPanelById(PANEL_RIGHT);

    std::vector<std::pair<ChildPanelId, int>> panels;
    if (!IsPanelPresent(4))
    {
        panels.push_back({PANEL_TOWN, 4});
    }
    panels.push_back({PANEL_RIGHT, 3});
    ShowPanels(panels, {PANEL_TOWN});
}

int MotherPanel::OnWndNotify(m3d::ui::Wnd* from, unsigned idFrom, unsigned message, m3d::AIParam const& data)
{
    if (!ModalWnd::OnWndNotify(from, idFrom, message, data))
    {
        if (idFrom != 800)
        {
            if (idFrom == 801 && message == 1)
            {
                OnEscape();
            }
            return 0;
        }
        if (message != 1)
        {
            return 0;
        }
        OnTabBtnClick(from, idFrom);
    }
    return 1;
}

int MotherPanel::OnAfterRemoveFromWndStation()
{
    int const res = m3d::ui::Wnd::OnAfterRemoveFromWndStation();
    m_lastTabId = m_curTabId;
    SetCurTab(TAB_NUM_TABS, true);
    if (m_hackedWorkshopVehicleId != -1)
    {
        help::DestroyVehicle(m_hackedWorkshopVehicleId);
        m_hackedWorkshopVehicleId = -1;
    }
    return res;
}

void MotherPanel::OnShowPanel(void* data)
{
    if (!data)
    {
        return;
    }

    switch (static_cast<m3d::Event*>(data)->m_intEv[0])
    {
    case 2:
        SetCurTab(TAB_BAR, true);
        break;
    case 3:
        SetCurTab(TAB_ADDITIONAL_BUILDING, true);
        break;
    case 4:
        OnTown();
        break;
    case 0x40:
    case 0x42:
        SetCurTab(TAB_INVENTORY_VS_SHOP, true);
        break;
    case 0x43:
    case 0x44:
    case 0x45:
        SetCurTab(TAB_CHARACTERISTIC_VS_WORKSHOP, true);
        break;
    case 0x49:
        OnBuyVehicle();
        break;
    case 0x58:
        OnTalkWithNpc();
        break;
    default:
        break;
    }
}

int MotherPanel::AddChildPanel(ref_ptr<ChildPanel> childPanel, ChildPanelId panelId)
{
    // RVA 0x45DF00
    if (!childPanel || panelId == PANEL_INVALID)
    {
        return 0;
    }

    // Already shown in this slot.
    if (GetCurrentPanelIdByGuiId(childPanel->GetGuiId()) == panelId)
    {
        return 1;
    }

    RemoveChildPanelById(panelId);
    m_panels.erase(panelId);
    m_panels.insert({panelId, childPanel});

    AddChild(childPanel.get());
    MoveChildToFirstPosition(childPanel.get());
    if (childPanel->GetStyle() & m3d::ui::WS_ACTIVATABLE)
    {
        GetStation()->Activate(childPanel.get());
    }
    return 1;
}

void MotherPanel::OnShop()
{
    // RVA 0x45E490
    if (m_gameDataFlags == 0)
    {
        return;
    }

    ai::Town const* town = M3D_APP->m_pInterfaceManager->GetCurrentTown();
    if (!town)
    {
        return;
    }

    ai::Building* shop = help::GetShopForTown(town);
    if (!shop)
    {
        return;
    }

    ref_ptr shopWnd = M3D_APP->m_pInterfaceManager->GetWindow(66);
    auto* saleWnd = RT_DYNCAST(shopWnd.get(), SaleWnd);
    if (!saleWnd || !saleWnd->SetUpForWorkshop(shop->GetId()))
    {
        return;
    }

    std::vector<std::pair<ChildPanelId, int>> panels;
    if (!IsPanelPresent(4))
    {
        panels.push_back({PANEL_TOWN, 4});
    }
    panels.push_back({PANEL_RIGHT, 66});
    panels.push_back({PANEL_LEFT, 64});
    panels.push_back({PANEL_VIDEO, 77});

    ai::pServer->PostPlayerEvent(ai::GE_TUTORIAL_SHOP);
    ShowPanels(panels, {PANEL_TOWN});
    M3D_APP->GetTownMusicManager()->LaunchAmbientShop();
}

void MotherPanel::OnPickUpAll()
{
    if (m_curTabId != TAB_INVENTORY_VS_SHOP || M3D_APP->m_pInterfaceManager->GetCurrentTown())
    {
        return;
    }

    ref_ptr groundWnd = M3D_APP->m_pInterfaceManager->GetWindow(63);
    if (auto* wnd = RT_DYNCAST(groundWnd.get(), GroundWnd); wnd && wnd->IsChildOf(M3D_APP))
    {
        wnd->PickUpAll();
    }
}

void MotherPanel::OnLocalMap(void* data)
{
    if (!data)
    {
        return;
    }

    ref_ptr wndLocalMap = M3D_APP->m_pInterfaceManager->GetWindow(82);
    auto* localMap = RT_DYNCAST(wndLocalMap.get(), LocalMapWnd);
    if (localMap && localMap->SetUpForLevel(static_cast<m3d::Event*>(data)->m_strEv))
    {
        m_bCurMapLocal = true;
        SetCurTab(TAB_MAP, true);
    }
}

void MotherPanel::OnLeaveTown(bool bQuick)
{
    M3D_APP->m_pInterfaceManager->OnLeaveTown(bQuick);
    UpdateTabButtonsOnLeaveTown();
}

bool MotherPanel::PickUpItemsFromGround()
{
    // RVA 0x461340
    if (!ai::thePlayer)
    {
        return false;
    }

    ai::Vehicle* vehicle = ai::thePlayer->GetVehicle();
    if (!vehicle)
    {
        return false;
    }

    unsigned int originalNumItems = 0;
    retruxx::vector<int> addedObjIds;
    vehicle->PickUpNearbyObjects(true, originalNumItems, addedObjIds);
    if (originalNumItems == 0 || addedObjIds.empty())
    {
        return false;
    }

    // Name every item that made it into the repository.
    for (int objId : addedObjIds)
    {
        if (ai::Obj const* obj = ai::theObjects->GetEntityByObjId(objId))
        {
            M3D_APP->EnqueueMessage(66563, 7, 0, 0, 0, obj->GetFullDescriptionWithAffixes(), {});
        }
    }

    bool bLooped = false;
    m_gfx->PlayControlSound(m_aif.m_pickUpSoundName, &bLooped);

    if (originalNumItems != addedObjIds.size())
    {
        // Some of what was lying there did not fit - tell the player which key
        // opens the repository so they can make room.
        M3D_APP->EnqueueMessage(66563, 14, 0, 0, 0, M3D_APP->m_pImpulses->GetImpulseNameById(42), {});
    }
    return true;
}

void MotherPanel::OnBuyVehicle()
{
    // RVA 0x45E950. Unlike its siblings this one has no m_gameDataFlags guard.
    ai::Workshop* workshop = M3D_APP->m_pInterfaceManager->GetCurrentWorkshop();
    if (!workshop)
    {
        return;
    }

    ref_ptr wshVehicleWnd = M3D_APP->m_pInterfaceManager->GetWindow(73);
    auto* vehicleWnd = RT_DYNCAST(wshVehicleWnd.get(), WorkshopVehicleWnd);
    if (!vehicleWnd)
    {
        return;
    }

    vehicleWnd->SetupForWorkshop(workshop->GetId());
    RemoveChildPanelById(PANEL_RIGHT);

    std::vector<std::pair<ChildPanelId, int>> panels;
    panels.push_back({PANEL_RIGHT, 73});
    if (!IsPanelPresent(68))
    {
        panels.push_back({PANEL_LEFT, 68});
    }
    ShowPanels(panels, {PANEL_TOWN, PANEL_LEFT});
}

void MotherPanel::ShowPanels(
    std::vector<std::pair<ChildPanelId, int>> panels,
    std::vector<ChildPanelId> const& previousPanelsToRemain)
{
    // RVA 0x45F1F0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    AdjustAnimationOnShowPanels(panels);
    ClearPanels(previousPanelsToRemain);

    // If any panel cannot come up yet because a conflicting panel is still registered, the whole
    // request is parked and replayed from OnEndWndAnimation.
    for (auto const& panel : panels)
    {
        if (!CanChildPanelBeLaunchedNow(panel.first))
        {
            m_suspendedShow.m_suspendedPanels = panels;
            m_suspendedShow.m_previousPanelsToRemain = previousPanelsToRemain;
            return;
        }
    }
    m_suspendedShow.m_suspendedPanels.clear();
    m_suspendedShow.m_previousPanelsToRemain.clear();

    for (auto const& [panelId, guiId] : panels)
    {
        auto wnd = M3D_APP->m_pInterfaceManager->GetWindow(guiId);
        if (auto* childPanel = RT_DYNCAST(wnd.get(), ChildPanel))
        {
            AddChildPanel(childPanel, panelId);
        }
    }

    AdjustDecor();
    AdjustChildOrder();
    if (!GetStation()->IsModal(this))
    {
        M3D_APP->m_pInterfaceManager->ShowWindow(IW_DLG_MOTHER_PANEL, true, false, false, true, nullptr);
    }
}

ai::Building* MotherPanel::GetBuildingForTab(Tab tabId) const
{
    ai::Town const* town = M3D_APP->m_pInterfaceManager->GetCurrentTown();
    if (tabId == TAB_BAR)
    {
        return help::GetBarWithBarmanForTown(town);
    }
    if (tabId == TAB_ADDITIONAL_BUILDING)
    {
        return help::GetBarWithoutBarmanForTown(town);
    }
    if (!town)
    {
        return nullptr;
    }
    if (tabId == TAB_INVENTORY_VS_SHOP)
    {
        auto const shops = town->GetBuildingByType(ai::SHOP);
        return shops.empty() ? nullptr : shops.front();
    }
    if (tabId == TAB_CHARACTERISTIC_VS_WORKSHOP)
    {
        auto const workshops = town->GetBuildingByType(ai::WORKSHOP);
        return workshops.empty() ? nullptr : workshops.front();
    }
    return nullptr;
}

int MotherPanel::RemoveChild(m3d::Object* w)
{
    // RVA 0x45E170
    int const res = Wnd::RemoveChild(w);
    if (IS_KIND_OF(w, ChildPanel) && res)
    {
        for (auto it = m_panels.begin(); it != m_panels.end(); ++it)
        {
            if (it->second == w)
            {
                m_panels.erase(it);
                break;
            }
        }

        // Both are inlined in the original.
        AdjustAnimationOnHidePanel(static_cast<ChildPanel*>(w));
        AdjustDecor();
    }
    return res;
}

void MotherPanel::ShowTabButton(Tab tabId, bool bShow)
{
    if ((m_gameDataFlags & 1) == 0 || tabId == TAB_INVALID)
    {
        return;
    }

    auto* btn = m_tabButtons[tabId];
    if (bShow)
    {
        if (!IsDirectChild(btn))
        {
            AddChild(btn);
        }
    }
    else if (IsDirectChild(btn))
    {
        RemoveChild(btn);
    }
}

void MotherPanel::OnFinishTrade()
{
    auto const panelsToRemove = m_secondPanelLevel;
    for (auto panelId : panelsToRemove)
    {
        RemoveChildPanelById(panelId);
    }
    DestroyHackedWorkshopVehicle();
}

void MotherPanel::OnEndWndAnimation()
{
    if (!m_suspendedShow.m_suspendedPanels.empty())
    {
        ShowPanels(m_suspendedShow.m_suspendedPanels, m_suspendedShow.m_previousPanelsToRemain);
    }
}

bool MotherPanel::IsPanelPresent(int guiId) const
{
    ref_ptr wnd = M3D_APP->m_pInterfaceManager->GetWindow(guiId);
    return wnd && wnd->IsChildOf(this);
}

void MotherPanel::OnJournal()
{
    // RVA 0x45EDE0
    // NOTE: tests the whole m_gameDataFlags word, like OnAdditionalBuilding.
    if (m_gameDataFlags != 0)
    {
        std::vector<std::pair<MotherPanel::ChildPanelId, int>> panels;
        if (M3D_APP->m_pInterfaceManager->GetCurrentTown() && !IsPanelPresent(4))
        {
            panels.push_back({PANEL_TOWN, 4});
        }

        panels.push_back({PANEL_PALM, 16});

        ai::pServer->PostPlayerEvent(ai::GE_TUTORIAL_JOURNAL);
        ShowPanels(panels, {PANEL_TOWN});
    }
}

bool MotherPanel::CanChildPanelBeLaunchedNow(ChildPanelId panelId) const
{
    // RVA 0x4602F0
    auto const isShown = [this](ChildPanelId id) { return m_panels.find(id) != m_panels.end(); };

    if (isShown(panelId))
    {
        return false;
    }

    switch (panelId)
    {
    case PANEL_LEFT:
    case PANEL_RIGHT:
    case PANEL_VIDEO:
    case PANEL_TRADE_RIGHT:
    case PANEL_TRADE_LEFT:
    case PANEL_TRADE_COMMON:
        return !isShown(PANEL_PALM) && !isShown(PANEL_CONVERSATION) && !isShown(PANEL_FULLSCREEN);

    case PANEL_FULLSCREEN:
        return m_panels.empty();

    case PANEL_TOWN:
        return !isShown(PANEL_FULLSCREEN);

    case PANEL_PALM:
    case PANEL_CONVERSATION:
        // NOTE: unlike the side panels, these are not blocked by PANEL_FULLSCREEN.
        return !isShown(PANEL_LEFT) && !isShown(PANEL_RIGHT) && !isShown(PANEL_VIDEO) &&
               !isShown(PANEL_TRADE_RIGHT) && !isShown(PANEL_TRADE_LEFT) && !isShown(PANEL_TRADE_COMMON);

    default:
        return true;
    }
}

MotherPanel::Tab MotherPanel::GetTabForBuilding(ai::Building const* building) const
{
    if (!building)
    {
        return TAB_INVALID;
    }

    ai::BuildingPrototypeInfo const* protoInfo = building->GetPrototypeInfo();
    int const buildingType =
        protoInfo ? static_cast<int>(protoInfo->m_buildingType) : static_cast<int>(ai::NUM_BUILDINGTYPES);

    switch (buildingType)
    {
    case ai::BAR:
        if (!building->IsKindOf(&ai::Bar::m_classBar))
        {
            return TAB_INVALID;
        }
        return static_cast<Tab>(6 - (static_cast<ai::Bar const*>(building)->bWithBarman() ? 1 : 0));
    case ai::SHOP:
        return TAB_INVENTORY_VS_SHOP;
    case ai::WORKSHOP:
        return TAB_CHARACTERISTIC_VS_WORKSHOP;
    default:
        return TAB_INVALID;
    }
}

void MotherPanel::AdjustAnimationOnHidePanel(m3d::ui::Wnd* panel)
{
    // RVA 0x4612E0
    if (panel)
    {
        panel->SetOnShowAnimationImmediate(false);
        panel->SetOnHideAnimationImmediate(false);
    }
}

void MotherPanel::UpdateTabButtonsOnLeaveTown()
{
    // RVA 0x45F860
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    // SetMode is inlined in the original: only the tabs that have a field mode (0..4) switch, and
    // only while there is no current town.
    for (auto* tabButton : m_tabButtons)
    {
        if (tabButton)
        {
            tabButton->SetMode(MotherPanelTabButton::MODE_IN_FIELD);
        }
    }

    ShowTabButton(TAB_BAR, false);
    ShowTabButton(TAB_ADDITIONAL_BUILDING, false);
}

void MotherPanel::OnTalkWithNpc()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    std::vector<std::pair<ChildPanelId, int>> panels;
    if (M3D_APP->m_pInterfaceManager->GetCurrentTown() && !IsPanelPresent(4))
    {
        panels.push_back({PANEL_TOWN, 4});
    }
    panels.push_back({PANEL_CONVERSATION, 88});
    ShowPanels(panels, {PANEL_TOWN});
}

void MotherPanel::OnQuestLog()
{
    // RVA 0x45EBD0
    // NOTE: tests the whole m_gameDataFlags word, like OnAdditionalBuilding.
    if (m_gameDataFlags != 0)
    {
        std::vector<std::pair<MotherPanel::ChildPanelId, int>> panels;
        if (M3D_APP->m_pInterfaceManager->GetCurrentTown() && !IsPanelPresent(4))
        {
            panels.push_back({PANEL_TOWN, 4});
        }

        panels.push_back({PANEL_PALM, 15});

        ai::pServer->PostPlayerEvent(ai::GE_TUTORIAL_QUESTLOG);
        ShowPanels(panels, {PANEL_TOWN});
    }
}

bool MotherPanel::InTown() const
{
    return M3D_APP->m_pInterfaceManager->GetCurrentTown() != nullptr;
}

void MotherPanel::OnInventory()
{
    // RVA 0x45E370
    if ((m_gameDataFlags & 1) != 0)
    {
        std::vector<std::pair<MotherPanel::ChildPanelId, int>> panels;
        if (M3D_APP->m_pInterfaceManager->GetCurrentTown() && !IsPanelPresent(IW_DLG_TOWN))
        {
            panels.push_back({PANEL_TOWN, IW_DLG_TOWN});
        }

        panels.push_back({PANEL_RIGHT, IW_WND_GROUND});
        panels.push_back({PANEL_LEFT, IW_WND_PLAYER_INVENTORY});
        panels.push_back({PANEL_VIDEO, IW_WND_VIDEO});

        ai::pServer->PostPlayerEvent(ai::GE_TUTORIAL_INVENTORY);
        ShowPanels(panels, {PANEL_TOWN});
    }
}

int MotherPanel::RemoveChildPanelById(ChildPanelId panelId)
{
    auto const it = m_panels.find(panelId);
    if (it == m_panels.end())
    {
        return 1;
    }

    return RemoveChild(it->second);
}

int MotherPanel::GetGuiIdByCurrentPanelId(ChildPanelId panelId) const
{
    auto it = m_panels.find(panelId);
    if (it == m_panels.end() || !it->second)
    {
        return -1;
    }
    return it->second->GetGuiId();
}

MotherPanel::Tab MotherPanel::ValidateLastTab() const
{
    if (m_lastTabId == (TAB_ADDITIONAL_BUILDING | TAB_MAP))
        return TAB_QUESTLOG;
    if (!M3D_APP->m_pInterfaceManager->GetCurrentTown() &&
        ((m_curTabId == TAB_BAR) || m_curTabId == TAB_ADDITIONAL_BUILDING))
    {
        return TAB_QUESTLOG;
    }
    else
    {
        return m_lastTabId;
    }
}

int MotherPanel::RemoveChildPanel(ref_ptr<ChildPanel> childPanel)
{
    // RVA 0x45E070
    if (!childPanel)
    {
        return 0;
    }

    auto const it = std::find_if(
        m_panels.begin(),
        m_panels.end(),
        [&childPanel](auto const& entry) { return entry.second.get() == childPanel.get(); });
    if (it == m_panels.end())
    {
        return 0;
    }

    // RemoveChild also erases the entry from m_panels.
    RemoveChild(it->second);
    return 1;
}

void MotherPanel::OnGlobalMap()
{
    m_bCurMapLocal = false;
    SetCurTab(TAB_MAP, true);
}

void MotherPanel::OnCharacteristics()
{
    // RVA 0x45EAC0
    // NOTE: tests the whole m_gameDataFlags word, like OnAdditionalBuilding.
    if (m_gameDataFlags != 0)
    {
        std::vector<std::pair<MotherPanel::ChildPanelId, int>> panels;
        if (M3D_APP->m_pInterfaceManager->GetCurrentTown() && !IsPanelPresent(4))
        {
            panels.push_back({PANEL_TOWN, 4});
        }

        panels.push_back({PANEL_LEFT, IW_WND_PLAYER_INVENTORY});
        panels.push_back({PANEL_RIGHT, IW_WND_CHARACTERISTICS_RIGHT});

        ai::pServer->PostPlayerEvent(ai::GE_TUTORIAL_VEHICLE);
        ShowPanels(panels, {PANEL_TOWN});
    }
}

ai::Building const* MotherPanel::GetOnlyBuilding() const
{
    ai::Town const* town = M3D_APP->m_pInterfaceManager->GetCurrentTown();
    if (!town)
    {
        return nullptr;
    }
    auto const& buildings = town->GetAllBuildings();
    if (buildings.size() != 1)
    {
        return nullptr;
    }
    return buildings.front();
}

void MotherPanel::AdjustChildOrder()
{
    // RVA 0x460D90
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    // Each call moves the child to the front, so the last one moved ends up on top: the frame
    // controls, then the tab buttons, then the conversation dialog.
    MoveChildToFirstPosition(m_wndTopPanel);
    MoveChildToFirstPosition(m_wndPlayerMoney);
    MoveChildToFirstPosition(m_btnExit);
    MoveChildToFirstPosition(m_wndDecor);
    MoveChildToFirstPosition(m_wndDecorBar);
    for (auto* tabButton : m_tabButtons)
    {
        if (IsDirectChild(tabButton))
        {
            MoveChildToFirstPosition(tabButton);
        }
    }

    if (IsPanelPresent(IW_DLG_TALK_WITH_NPC))
    {
        ref_ptr talkWnd = M3D_APP->m_pInterfaceManager->GetWindow(IW_DLG_TALK_WITH_NPC);
        MoveChildToFirstPosition(talkWnd.get());
    }
}

int MotherPanel::GameDataSetup()
{
    using namespace m3d::ui;

    if ((m_gameDataFlags & 2) == 0)
    {
        int res = 1;

        if (auto child = RT_DYNCAST(GetChildByName(m_aif.m_wndDecorName), Wnd))
        {
            m_wndDecor = child;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndDecorName + " is not found or incorrect type");
            res = 0;
        }

        if (auto child = RT_DYNCAST(GetChildByName(m_aif.m_wndDecorBarName), ImageWnd))
        {
            m_wndDecorBar = child;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndDecorBarName + " is not found or incorrect type");
            res = 0;
        }

        if (auto child = RT_DYNCAST(GetChildByName(m_aif.m_wndTopPanelName), Wnd))
        {
            m_wndTopPanel = child;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndTopPanelName + " is not found or incorrect type");
            res = 0;
        }

        if (auto child = RT_DYNCAST(GetChildByName(m_aif.m_btnExitName), ButtonWnd))
        {
            m_btnExit = child;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnExitName + " is not found or incorrect type");
            res = 0;
        }

        for (int i = TAB_QUESTLOG; i < TAB_NUM_TABS; ++i)
        {
            CStr const name = m_aif.m_tabBtnName + Tab2Str(static_cast<Tab>(i));
            if (auto child = RT_DYNCAST(GetChildByName(name), ButtonWnd))
            {
                m_tabButtons[i] = static_cast<MotherPanelTabButton*>(M3D_KERNEL->New("MotherPanelTabButton"));
                if (m_tabButtons[i])
                {
                    if (!m_tabButtons[i]->CreateFromPattern(child, true))
                    {
                        M3D_LOG_INFO("Make control error: cannot create " + name + " from pattern class");
                        res = 0;
                    }
                }
                else
                {
                    M3D_LOG_INFO(
                        "Make control error: cannot create " + name + " - cannot find rtti class MotherPanelTabButton");
                    res = 0;
                }
            }
            else
            {
                M3D_LOG_INFO("Get control error: control " + name + " is not found or incorrect type");
                res = 0;
            }

            if (m_tabButtons[i] != nullptr)
            {
                m_tabButtons[i]->SetupForTab(static_cast<Tab>(i));
                res &= 1u;
            }
        }

        ref_ptr<Wnd> moneyWnd = M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_PLAYER_MONEY);
        if (auto playerMoneyWnd = RT_DYNCAST(moneyWnd.get(), PlayerMoneyWnd))
        {
            m_wndPlayerMoney = playerMoneyWnd;
            AddChild(m_wndPlayerMoney);
            if (res)
            {
                m_gameDataFlags |= 1u;
                M3D_APP->m_pInterfaceManager->OnLeaveTown(true);
                UpdateTabButtonsOnLeaveTown();
            }
        }
    }
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    return 1;
}

void MotherPanel::OnTown()
{
    // RVA 0x45F980 - drops back to the town's root view.
    if (m_gameDataFlags == 0)
    {
        return;
    }

    ai::Building const* onlyBuilding = nullptr;
    ai::Town* town = M3D_APP->m_pInterfaceManager->GetCurrentTown();
    if (!town)
    {
        // Not in a town yet: the town dialog itself knows which one we just
        // walked into, so enter it now.
        ref_ptr wndTown = M3D_APP->m_pInterfaceManager->GetWindow(4);
        if (auto* townDlg = RT_DYNCAST(wndTown.get(), TownDlg))
        {
            town = townDlg->GetTown();
            if (town)
            {
                OnEnterTown(town);
                onlyBuilding = GetOnlyBuilding();
            }
        }
    }

    ClearPanels({PANEL_TOWN});
    m_curTabId = TAB_INVALID;
    SelectTabButton(TAB_INVALID);
    M3D_APP->GetTownMusicManager()->StopAmbient();

    if (town)
    {
        ai::thePlayer->CauseEvent(ai::GE_TUTORIAL_TOWN, 0.0f, m3d::AIParam(town->GetId()), m3d::AIParam());
        town->CauseEvent(ai::GE_TUTORIAL_TOWN, 0.0f, m3d::AIParam(), m3d::AIParam());
    }

    if (onlyBuilding)
    {
        // A town with a single building opens straight into that building's tab.
        SetCurTab(GetTabForBuilding(onlyBuilding), true);
    }
    else
    {
        std::vector<std::pair<ChildPanelId, int>> panels;
        panels.push_back({PANEL_TOWN, 4});
        ShowPanels(panels, {PANEL_TOWN});
    }
}

MotherPanel::MotherPanel(MotherPanel const&) : MotherPanel()
{
}

MotherPanel::MotherPanel()
{
    m_secondPanelLevel.push_back(PANEL_TRADE_RIGHT);
    m_secondPanelLevel.push_back(PANEL_TRADE_LEFT);
    m_secondPanelLevel.push_back(PANEL_TRADE_COMMON);
    m_hackedWorkshopVehicleId = -1;
    m_curTabId = TAB_NUM_TABS;
    m_lastTabId = TAB_NUM_TABS;
    m_wndDecor = 0;
    m_wndDecorBar = 0;
    m_btnExit = 0;
    m_wndTopPanel = 0;
    m_bCurMapLocal = 1;
    m_tabButtons.resize(7, nullptr);
}

int MotherPanel::GameDataUpdate(void* data, int dataType)
{
    switch (dataType)
    {
    case 2:
        if (IsPanelPresent(IW_DLG_TALK_WITH_NPC) || (!IsChildOf(M3D_APP) && PickUpItemsFromGround()))
            return 1;
        ToggleTab(TAB_INVENTORY_VS_SHOP);
        return 1;

    case 3:
        if (IsModal())
        {
            if (!InTown() && !IsPanelPresent(IW_DLG_TALK_WITH_NPC))
                Hide(/*bForce*/ false, /*bQuickLeaveTown*/ false);
        }
        else
        {
            SetCurTab(ValidateLastTab(), true);
        }
        return 1;

    case 4:
        if (!IsPanelPresent(IW_DLG_TALK_WITH_NPC))
            ToggleTab(TAB_QUESTLOG);
        return 1;
    case 5:
        if (!IsPanelPresent(IW_DLG_TALK_WITH_NPC))
            ToggleTab(TAB_JOURNAL);
        return 1;
    case 6:
        if (!IsPanelPresent(IW_DLG_TALK_WITH_NPC))
            ToggleTab(TAB_MAP);
        return 1;
    case 7:
        if (!IsPanelPresent(IW_DLG_TALK_WITH_NPC))
            ToggleTab(TAB_CHARACTERISTIC_VS_WORKSHOP);
        return 1;
    case 12:
        if (!IsPanelPresent(IW_DLG_TALK_WITH_NPC))
            ToggleTab(TAB_BAR);
        return 1;
    case 13:
        if (!IsPanelPresent(IW_DLG_TALK_WITH_NPC))
            ToggleTab(TAB_ADDITIONAL_BUILDING);
        return 1;

    case 14:
        OnPickUpAll();
        return 1;

    case 18:
        if (IsModal())
            OnEndWndAnimation();
        return 1;

    case 20:
    {
        if (!data)
            return 1;
        int objId = *reinterpret_cast<int*>(static_cast<char*>(data) + 0x34);
        ai::Obj* e = ai::theObjects->GetEntityByObjId(objId);
        if (e && e->IsKindOf(&ai::Bar::m_classBar))
            SetCurTab(static_cast<ai::Bar*>(e)->bWithBarman() ? TAB_BAR : TAB_ADDITIONAL_BUILDING, true);
        return 1;
    }

    case 21:
        SetCurTab(TAB_CHARACTERISTIC_VS_WORKSHOP, true);
        return 1;
    case 24:
        SetCurTab(TAB_INVENTORY_VS_SHOP, true);
        return 1;

    case 33:
        OnStartTrade(data);
        return 1;
    case 34:
        OnFinishTrade();
        return 1;
    case 35:
        OnHidePanel(data);
        return 1;
    case 36:
        OnShowPanel(data);
        return 1;
    case 38:
        OnLocalMap(data);
        return 1;
    case 39:
        OnGlobalMap();
        return 1;

    default:
        return 1;
    }
}

void MotherPanel::Hide(bool bForce, bool bQuickLeaveTown)
{
    // RVA 0x45F440
    // Unless forced, a town with a conditional closing rule for this level is not left: the town
    // gets GE_TOWN_CONDITIONAL_CLOSING and decides itself.
    if (!bForce)
    {
        if (auto* currentTown = M3D_APP->m_pInterfaceManager->GetCurrentTown())
        {
            ref_ptr townWnd = M3D_APP->m_pInterfaceManager->GetWindow(IW_DLG_TOWN);
            if (auto* townDlg = RT_DYNCAST(townWnd.get(), TownDlg))
            {
                if (townDlg->GetConditionalClosingInfoForTown(currentTown->GetName(), help::GetCurrentLevelName()))
                {
                    currentTown->CauseEvent(ai::GE_TOWN_CONDITIONAL_CLOSING, 0.0f, {}, {});
                    return;
                }
            }
        }
    }

    m_suspendedShow.m_suspendedPanels.clear();

    if (GetStation()->IsModal(this))
    {
        if (M3D_APP->m_pInterfaceManager->GetCurrentTown())
        {
            M3D_APP->m_pInterfaceManager->OnLeaveTown(bQuickLeaveTown);
            UpdateTabButtonsOnLeaveTown();
        }

        if (ai::thePlayer)
        {
            if (auto* vehicle = ai::thePlayer->GetVehicle())
            {
                vehicle->EnableSounds(true);
            }
        }

        M3D_APP->m_pInterfaceManager->ShowWindow(IW_DLG_MOTHER_PANEL, false, false, false, false, nullptr);
    }
}

void MotherPanel::DestroyHackedWorkshopVehicle()
{
    if (m_hackedWorkshopVehicleId != -1)
    {
        help::DestroyVehicle(m_hackedWorkshopVehicleId);
        m_hackedWorkshopVehicleId = -1;
    }
}

void MotherPanel::Show()
{
    if (!GetStation()->IsModal(this))
    {
        M3D_APP->m_pInterfaceManager->ShowWindow(7, true, false, false, true, nullptr);
    }
}

int MotherPanel::OnKey(unsigned short key, unsigned char scanCode, unsigned state)
{
    if (key != 1 || !state)
    {
        return ModalWnd::OnKey(key, scanCode, state);
    }
    OnEscape();
    return 1;
}

void MotherPanel::OnStartTrade(void* data)
{
    // RVA 0x45EEE0 - swaps the panel layout over to the trade triptych and points
    // it at the object being haggled over.
    if (!GetStation()->IsModal(this) || !M3D_APP->m_pInterfaceManager->IsInSaleMode())
    {
        return;
    }

    auto const* evt = static_cast<m3d::Event const*>(data);
    if (!evt)
    {
        return;
    }

    int const workshopId = evt->m_intEv[2];
    if (workshopId == -1)
    {
        return;
    }

    int tradeVehicleId = evt->m_intEv[0];
    if (tradeVehicleId == -1)
    {
        // No vehicle supplied: preview the goods on a scratch copy of the
        // player's own vehicle instead.
        if (!CreateHackedWorkshopVehicle())
        {
            return;
        }
        tradeVehicleId = m_hackedWorkshopVehicleId;
    }

    int const objToTradeId = evt->m_intEv[1];
    ai::Obj* objToTrade = ai::theObjects->GetEntityByObjId(objToTradeId);
    if (!objToTrade || !objToTrade->IsKindOf(&ai::Obj::m_classObj))
    {
        return;
    }

    ref_ptr inventoryWnd = M3D_APP->m_pInterfaceManager->GetWindow(65);
    auto* inventory = RT_DYNCAST(inventoryWnd.get(), InventoryWnd);
    if (!inventory)
    {
        return;
    }

    // NOTE: the cabin/basket branches below test the m_hackedWorkshopVehicleId
    // member rather than the tradeVehicleId local computed above, so when the
    // event carried its own vehicle id and no scratch vehicle was ever created,
    // the part swap is skipped entirely. Preserved as shipped.
    ZnayuKakProdatWnd::TradeType tradeType;
    if (objToTrade->IsKindOf(&ai::Vehicle::m_classVehicle))
    {
        tradeType = ZnayuKakProdatWnd::TRADETYPE_VEHICLE;
    }
    else if (objToTrade->IsKindOf(&ai::Cabin::m_classCabin))
    {
        if (m_hackedWorkshopVehicleId != -1)
        {
            ai::Obj* hacked = ai::theObjects->GetEntityByObjId(m_hackedWorkshopVehicleId);
            auto* hackedVehicle = RT_DYNCAST(hacked, ai::Vehicle);
            if (!hackedVehicle)
            {
                return;
            }
            help::RemoveAllPartsFromVehicle(hackedVehicle);
            hackedVehicle->SetCabin(static_cast<ai::VehiclePart*>(objToTrade));
        }
        tradeType = ZnayuKakProdatWnd::TRADETYPE_CABIN;
    }
    else if (objToTrade->IsKindOf(&ai::Basket::m_classBasket))
    {
        if (m_hackedWorkshopVehicleId != -1)
        {
            ai::Obj* hacked = ai::theObjects->GetEntityByObjId(m_hackedWorkshopVehicleId);
            auto* hackedVehicle = RT_DYNCAST(hacked, ai::Vehicle);
            if (!hackedVehicle)
            {
                return;
            }
            help::RemoveAllPartsFromVehicle(hackedVehicle);
            hackedVehicle->SetBasket(static_cast<ai::VehiclePart*>(objToTrade));
        }
        tradeType = ZnayuKakProdatWnd::TRADETYPE_BASKET;
    }
    else
    {
        return;
    }

    inventory->SetTradeVehicleId(tradeVehicleId, tradeType);

    ref_ptr znayuWnd = M3D_APP->m_pInterfaceManager->GetWindow(94);
    auto* znayu = RT_DYNCAST(znayuWnd.get(), ZnayuKakProdatWnd);
    if (!znayu)
    {
        return;
    }
    znayu->SetupForTrade(workshopId, objToTradeId, tradeVehicleId);

    std::vector<std::pair<ChildPanelId, int>> panels;
    panels.push_back({PANEL_TRADE_RIGHT, 65});
    panels.push_back({PANEL_TRADE_LEFT, 64});
    panels.push_back({PANEL_TRADE_COMMON, 94});
    ShowPanels(panels, {PANEL_TOWN, PANEL_LEFT, PANEL_RIGHT, PANEL_VIDEO});
}

void MotherPanel::OnTabBtnClick(m3d::ui::Wnd* wndFrom, int)
{
    if (auto* tab = RT_DYNCAST(wndFrom, MotherPanelTabButton); tab && !tab->IsSelected())
    {
        SetCurTab(tab->GetTabId(), true);
    }
}

int MotherPanel::CreateHackedWorkshopVehicle()
{
    // RVA 0x4607C0 - builds a throwaway clone of the player's vehicle that the
    // trade UI can mount candidate parts on without touching the real one. It is
    // spawned invisible and lives only until the trade finishes.
    DestroyHackedWorkshopVehicle();

    if (!ai::thePlayer)
    {
        return 0;
    }

    ai::Vehicle* playerVehicle = ai::thePlayer->GetVehicle();
    if (!playerVehicle)
    {
        return 0;
    }

    if (ai::Vehicle* vehicle = help::CreateVehicleFromPrototype(playerVehicle->GetPrototypeId()))
    {
        vehicle->SetInvisible();
        m_hackedWorkshopVehicleId = vehicle->GetId();
    }
    return m_hackedWorkshopVehicleId != -1;
}

void MotherPanel::OnWorkshop()
{
    // RVA 0x45E650
    if (m_gameDataFlags == 0)
    {
        return;
    }

    ai::Town const* town = M3D_APP->m_pInterfaceManager->GetCurrentTown();
    if (!town)
    {
        return;
    }

    ai::Building* workshop = help::GetWorkshopForTown(town);
    if (!workshop)
    {
        return;
    }

    ref_ptr wshWnd = M3D_APP->m_pInterfaceManager->GetWindow(67);
    auto* garageWnd = RT_DYNCAST(wshWnd.get(), GarageWnd);
    if (!garageWnd || !garageWnd->SetupForWorkshop(workshop->GetId()))
    {
        return;
    }

    std::vector<std::pair<ChildPanelId, int>> panels;
    if (!IsPanelPresent(4))
    {
        panels.push_back({PANEL_TOWN, 4});
    }
    panels.push_back({PANEL_RIGHT, 67});

    ai::thePlayer->CauseEvent(ai::GE_TUTORIAL_WORKSHOP, 0.0f, m3d::AIParam(workshop->GetId()), m3d::AIParam());
    workshop->CauseEvent(ai::GE_TUTORIAL_WORKSHOP, 0.0f, m3d::AIParam(), m3d::AIParam());

    std::vector<ChildPanelId> panelsToRemain;
    panelsToRemain.push_back(PANEL_TOWN);

    // Coming back from the buy-vehicle panel, that panel occupies the right slot.
    if (IsPanelPresent(73))
    {
        RemoveChildPanelById(PANEL_RIGHT);
    }

    ref_ptr wndCharacteristic = M3D_APP->m_pInterfaceManager->GetWindow(68);
    if (wndCharacteristic)
    {
        // Already up and settled? Leave it be rather than re-showing it.
        if (!wndCharacteristic->IsChildOf(M3D_APP) || wndCharacteristic->IsAnimatingNow())
        {
            panels.push_back({PANEL_LEFT, 68});
        }
        else
        {
            panelsToRemain.push_back(PANEL_LEFT);
        }
    }

    ShowPanels(panels, panelsToRemain);
    M3D_APP->GetTownMusicManager()->LaunchAmbientWorkshop();
}

void MotherPanel::SetCurTab(Tab tabId, bool bUpdatePanels)
{
    m_curTabId = tabId;
    SelectTabButton(tabId);
    M3D_APP->GetTownMusicManager()->StopAmbient();
    if (bUpdatePanels)
    {
        switch (m_curTabId)
        {
        case TAB_QUESTLOG:
            OnQuestLog();
            break;

        case TAB_MAP:
            OnMap();
            break;

        case TAB_JOURNAL:
            OnJournal();
            break;

        case TAB_INVENTORY_VS_SHOP:
            if (InTown())
            {
                if (!GetBuildingForTab(m_curTabId))
                {
                    OnCharacteristics();
                    break;
                }
                OnShop();
            }
            else
            {
                OnInventory();
            }
            break;

        case TAB_CHARACTERISTIC_VS_WORKSHOP:
            if (GetBuildingForTab(TAB_CHARACTERISTIC_VS_WORKSHOP))
            {
                OnWorkshop();
            }
            else
            {
                OnCharacteristics();
            }
            break;

        case TAB_BAR:
            if (GetBuildingForTab(TAB_BAR))
            {
                OnBar();
            }
            break;

        case TAB_ADDITIONAL_BUILDING:
            if (GetBuildingForTab(TAB_ADDITIONAL_BUILDING))
            {
                OnAdditionalBuilding();
            }
            break;

        default:
            ClearPanels({});
            break;
        }
    }
}

void MotherPanel::OnEnterTown(ai::Town const* town)
{
    if (town)
    {
        M3D_APP->m_pInterfaceManager->OnEnterTown(town->GetId());
        UpdateTabButtonsOnEnterTown(town);
    }
}

void MotherPanel::OnBar()
{
    // RVA 0x45FC20 - the bar tab proper, i.e. the town bar that has a barman.
    if (m_gameDataFlags == 0)
    {
        return;
    }

    ai::Town const* town = M3D_APP->m_pInterfaceManager->GetCurrentTown();
    if (!town)
    {
        return;
    }

    ai::Bar* bar = help::GetBarWithBarmanForTown(town);
    if (!bar)
    {
        return;
    }

    ref_ptr barWnd = M3D_APP->m_pInterfaceManager->GetWindow(2);
    auto* barDlg = RT_DYNCAST(barWnd.get(), BarDlg);
    if (!barDlg || !barDlg->SetUpForBar(bar->GetId()))
    {
        return;
    }

    RemoveChildPanelById(PANEL_RIGHT);

    std::vector<std::pair<ChildPanelId, int>> panels;
    if (!IsPanelPresent(4))
    {
        panels.push_back({PANEL_TOWN, 4});
    }
    panels.push_back({PANEL_RIGHT, 2});

    ai::pServer->PostPlayerEvent(ai::GE_TUTORIAL_BAR);
    ShowPanels(panels, {PANEL_TOWN});
}

void MotherPanel::SelectTabButton(Tab tabId)
{
    // RVA 0x460C30
    // m_tabButtons is indexed by Tab, so button i is the one for tab i.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    for (size_t i = 0; i < m_tabButtons.size(); ++i)
    {
        if (auto* tabButton = m_tabButtons[i])
        {
            tabButton->Select(tabId == static_cast<Tab>(i));
        }
    }
}

void MotherPanel::AdjustDecor()
{
    // RVA 0x460ED0
    if ((m_gameDataFlags & 1) != 0)
    {
        bool const bNeedShowDecorBar = IsPanelPresent(IW_DLG_BAR) || IsPanelPresent(IW_DLG_ADDITIONAL_BUILDING);
        m_wndDecorBar->ShowWindow(bNeedShowDecorBar);
    }
}
