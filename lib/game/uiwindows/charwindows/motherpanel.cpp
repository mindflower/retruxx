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
#include <game/m3dgame.h>
#include "znayukakprodatwnd.h"

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
    // TOOD: generated code MotherPanel::UpdateTabButtonsOnEnterTown
    // Only proceed if we have game data flag 1 and a valid town
    if ((m_gameDataFlags & 1) == 0 || !town)
    {
        return;
    }

    // Update tab buttons based on town availability
    for (size_t i = 0; i < m_tabButtons.size(); ++i)
    {
        MotherPanelTabButton* button = m_tabButtons[i];
        if (!button || button->GetMode() == MotherPanelTabButton::MODE_NUM_MODES)
        {
            continue;
        }

        // Only process specific tab types
        switch (button->GetTabId())
        {
        case TAB_INVENTORY_VS_SHOP:
        case TAB_CHARACTERISTIC_VS_WORKSHOP:
        case TAB_BAR:
        case TAB_ADDITIONAL_BUILDING:
            // If this building exists in the town, update button to in-town mode
            button->SetMode(MotherPanelTabButton::MODE_IN_TOWN);
            break;
        default:
            continue;
        }
    }

    // Handle bar with barman
    if (help::GetBarWithBarmanForTown(town) && (m_gameDataFlags & 1) != 0)
    {
        m3d::Object* barWithBarmanButton = reinterpret_cast<m3d::Object*>(m_tabButtons[5]);
        if (!m3d::Object::IsDirectChild(barWithBarmanButton))
        {
            AddChild(barWithBarmanButton);
        }
    }

    // Handle bar without barman
    if (help::GetBarWithoutBarmanForTown(town) && (m_gameDataFlags & 1) != 0)
    {
        m3d::Object* barWithoutBarmanButton = reinterpret_cast<m3d::Object*>(m_tabButtons[6]);
        if (!m3d::Object::IsDirectChild(barWithoutBarmanButton))
        {
            AddChild(barWithoutBarmanButton);
        }
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
    // TODO: generated code MotherPanel::ClearPanels
    // Iterate through all child panels
    for (auto it = m_panels.begin(); it != m_panels.end();)
    {
        ChildPanelId const& currentPanelId = it->first;

        // Check if this panel should be removed
        bool shouldRemove = true;

        // Look for current panel in the list of panels to keep
        for (auto const& panelIdToKeep : previousPanelsToRemain)
        {
            if (panelIdToKeep == currentPanelId)
            {
                shouldRemove = false;
                break;
            }
        }

        if (shouldRemove)
        {
            // Remove the child panel
            auto curIt = it++;
            RemoveChildPanel(curIt->second);
        }
        else
        {
            ++it;
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
    // TODO: generated code MotherPanel::RemoveChildForce
    // Store the window for later checks
    // First, try to remove the child from the basic Wnd hierarchy
    int const removalSuccess = m3d::ui::Wnd::RemoveChildForce(wnd);

    // Check if the removed window is actually a ChildPanel and if removal was successful
    if (wnd->IsKindOf(&ChildPanel::m_classChildPanel) && removalSuccess)
    {
        auto* childWnd = RT_DYNCAST(wnd, ChildPanel);
        // Need to also remove the child panel from our internal panels map
        // Search for this panel in our panels map
        auto panelIter = m_panels.begin();
        auto panelsEnd = m_panels.end();

        for (; panelIter != panelsEnd; ++panelIter)
        {
            // Get the panel reference from the iterator
            ref_ptr<ChildPanel> panelRef = panelIter->second;

            // Check if this panel matches the one being removed
            if (panelRef.get() == wnd)
            {
                // Found the panel in our map - remove it
                m_panels.erase(panelIter);
                break;
            }
        }

        // If we found and removed the panel, reset its animations
        if (wnd)
        {
            // Disable any show/hide animations for this panel
            childWnd->SetOnShowAnimationImmediate(false);
            childWnd->SetOnHideAnimationImmediate(false);
        }

        // Update UI state based on game data flags
        if ((m_gameDataFlags & 1) != 0)  // Check if first flag is set
        {
            bool shouldShowDecorBar = false;

            // Check if specific panels are still present
            if (IsPanelPresent(ChildPanelId::PANEL_VIDEO) || IsPanelPresent(ChildPanelId::PANEL_TRADE_RIGHT))
            {
                shouldShowDecorBar = true;
            }

            // Update the decoration bar visibility
            if (m_wndDecorBar)
            {
                m_wndDecorBar->ShowWindow(shouldShowDecorBar);
            }
        }
    }

    return removalSuccess;
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
    // TODO: check this
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
    // TODO: generated code MotherPanel::AdjustAnimationOnShowPanels
    // Look for PANEL_PALM in the incoming panels
    for (auto const& panelInfo : panels)
    {
        if (panelInfo.first == PANEL_PALM)
        {
            int newGuiId = panelInfo.second;

            // Only process if we're showing a valid GUI ID (not -1)
            if (newGuiId != -1)
            {
                // Check if we currently have a PANEL_PALM panel
                auto it = m_panels.find(PANEL_PALM);
                if (it != m_panels.end() && it->second)
                {
                    int currentGuiId = it->second->GetGuiId();

                    // If we have a different PANEL_PALM currently showing
                    if (currentGuiId != -1 && currentGuiId != newGuiId)
                    {
                        // Get the new panel window
                        auto newPalmWnd = M3D_APP->m_pInterfaceManager->GetWindow(newGuiId);

                        // Get the current panel window
                        auto oldPalmWnd = M3D_APP->m_pInterfaceManager->GetWindow(currentGuiId);

                        // Set immediate animation for transition
                        if (newPalmWnd)
                        {
                            newPalmWnd->SetOnShowAnimationImmediate(true);
                        }

                        if (oldPalmWnd)
                        {
                            oldPalmWnd->SetOnHideAnimationImmediate(true);
                        }
                    }
                }
            }
            return;  // Found PANEL_PALM, we're done
        }
    }

    // No PANEL_PALM in the panels to show
}

void MotherPanel::OnMap()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        std::vector<std::pair<MotherPanel::ChildPanelId, int>> panels;
        if (M3D_APP->m_pInterfaceManager->GetCurrentTown() && !IsPanelPresent(4))
        {
            panels.push_back({PANEL_TOWN, 4});
        }

        panels.push_back({PANEL_PALM, 82});

        // TODO: check this
        ai::pServer->PostPlayerEvent(ai::GE_TUTORIAL_MAP);
        ShowPanels(panels, {PANEL_TOWN});
    }
}

void MotherPanel::OnAdditionalBuilding()
{
    RETRUXX_NOT_IMPLEMENTED;
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
    // TODO: generated code MotherPanel::AddChildPanel
    // Validate input
    if (!childPanel.get())
    {
        return false;
    }

    if (panelId == ChildPanelId::PANEL_INVALID)
    {
        return false;
    }

    // Check if a panel with the same GUI ID already exists
    ChildPanelId existingPanelId = GetCurrentPanelIdByGuiId(childPanel->GetGuiId());
    if (existingPanelId == panelId)
    {
        // Already have this panel with the same ID
        return true;
    }

    // Remove any existing panel with this ID
    RemoveChildPanelById(panelId);

    // Insert the new panel into our map
    // Make sure to increment ref count before storing
    m_panels[panelId] = childPanel;

    // Add as a child window
    AddChild(childPanel.get());

    // Move to front
    MoveChildToFirstPosition(childPanel.get());

    // Activate the panel if it has the activation style flag
    if (childPanel->GetStyle() & m3d::ui::WS_ACTIVATABLE)
    {
        GetStation()->Activate(childPanel.get());
    }

    return true;
}

void MotherPanel::OnShop()
{
    RETRUXX_NOT_IMPLEMENTED;
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
    // TODO: implement MotherPanel::PickUpItemsFromGround
    // RETRUXX_NOT_IMPLEMENTED;
    return false;
}

void MotherPanel::OnBuyVehicle()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::ShowPanels(
    std::vector<std::pair<ChildPanelId, int>> panels,
    std::vector<ChildPanelId> const& previousPanelsToRemain)
{
    // TODO: generated code MotherPanel::ShowPanels
    // Check if game data flag 1 is set
    if ((m_gameDataFlags & 1) != 0)
    {
        AdjustAnimationOnShowPanels(panels);
        ClearPanels(previousPanelsToRemain);

        auto panelIt = panels.begin();
        auto panelEnd = panels.end();

        // Check if all panels can be launched now
        for (; panelIt != panelEnd; ++panelIt)
        {
            if (!CanChildPanelBeLaunchedNow(panelIt->first))
            {
                m_suspendedShow.m_suspendedPanels = std::move(panels);
                m_suspendedShow.m_previousPanelsToRemain = previousPanelsToRemain;
                return;
            }
        }

        // All panels can be launched - clear suspended state
        m_suspendedShow.m_suspendedPanels.clear();
        m_suspendedShow.m_previousPanelsToRemain.clear();

        // Launch each panel
        for (auto const& panelInfo : panels)
        {
            auto window = M3D_APP->m_pInterfaceManager->GetWindow(panelInfo.second);
            if (auto childPanel = RT_DYNCAST(window.get(), ChildPanel))
            {
                AddChildPanel(childPanel, panelInfo.first);
            }
        }

        // Update UI based on panel presence
        if ((m_gameDataFlags & 1) != 0)
        {
            bool showDecorBar = IsPanelPresent(PANEL_VIDEO) || IsPanelPresent(PANEL_TRADE_RIGHT);
            m_wndDecorBar->ShowWindow(showDecorBar);
        }

        AdjustChildOrder();

        // Handle window station and modal state
        auto station = GetStation();
        if (!station->IsModal(this))
        {
            M3D_APP->m_pInterfaceManager->ShowWindow(7, 1, 0, 0, 1, 0);
        }
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
    int const res = Wnd::RemoveChild(w);
    if (res && IS_KIND_OF(w, ChildPanel))
    {
        for (auto it = m_panels.begin(); it != m_panels.end(); ++it)
        {
            if (it->second == w)
            {
                m_panels.erase(it);
                break;
            }
        }

        auto* childPanel = RT_DYNCAST(w, ChildPanel);
        childPanel->SetOnShowAnimationImmediate(false);
        childPanel->SetOnHideAnimationImmediate(false);

        // Handle decor bar visibility based on game data flags
        if ((m_gameDataFlags & 1) != 0)
        {
            bool shouldShowDecorBar = false;

            // TODO: check this
            if (IsPanelPresent(2) || IsPanelPresent(3))
            {
                shouldShowDecorBar = true;
            }

            m_wndDecorBar->ShowWindow(shouldShowDecorBar);
        }
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
    if ((m_gameDataFlags & 1) != 0)
    {
        std::vector<std::pair<MotherPanel::ChildPanelId, int>> panels;
        if (M3D_APP->m_pInterfaceManager->GetCurrentTown() && !IsPanelPresent(4))
        {
            panels.push_back({PANEL_TOWN, 4});
        }

        panels.push_back({PANEL_PALM, 16});

        // TODO: check this
        ai::pServer->PostPlayerEvent(ai::GE_TUTORIAL_JOURNAL);
        ShowPanels(panels, {PANEL_TOWN});
    }
}

bool MotherPanel::CanChildPanelBeLaunchedNow(ChildPanelId panelId) const
{
    // TODO: generated code MotherPanel::CanChildPanelBeLaunchedNow(
    // First check if the panel is already active
    if (m_panels.find(panelId) != m_panels.end())
    {
        return false;
    }

    // Check specific panel type constraints
    switch (panelId)
    {
    case ChildPanelId::PANEL_LEFT:
    case ChildPanelId::PANEL_RIGHT:
    case ChildPanelId::PANEL_VIDEO:
    case ChildPanelId::PANEL_TRADE_RIGHT:
    case ChildPanelId::PANEL_TRADE_LEFT:
    case ChildPanelId::PANEL_TRADE_COMMON:
        // These panels cannot be launched if PALM or CONVERSATION panels are active
        if (m_panels.find(ChildPanelId::PANEL_PALM) != m_panels.end() ||
            m_panels.find(ChildPanelId::PANEL_CONVERSATION) != m_panels.end())
        {
            return false;
        }
        // These also cannot be launched if FULLSCREEN panel is active
        if (m_panels.find(ChildPanelId::PANEL_FULLSCREEN) != m_panels.end())
        {
            return false;
        }
        break;

    case ChildPanelId::PANEL_FULLSCREEN:
        // FULLSCREEN panel cannot be launched if any other panel is active
        if (!m_panels.empty())
        {
            return false;
        }
        break;

    case ChildPanelId::PANEL_TOWN:
        // TOWN panel cannot be launched if FULLSCREEN panel is active
        if (m_panels.find(ChildPanelId::PANEL_FULLSCREEN) != m_panels.end())
        {
            return false;
        }
        break;

    case ChildPanelId::PANEL_PALM:
    case ChildPanelId::PANEL_CONVERSATION:
        // These panels cannot be launched if any side panel is active
        if (m_panels.find(ChildPanelId::PANEL_LEFT) != m_panels.end() ||
            m_panels.find(ChildPanelId::PANEL_RIGHT) != m_panels.end() ||
            m_panels.find(ChildPanelId::PANEL_VIDEO) != m_panels.end() ||
            m_panels.find(ChildPanelId::PANEL_TRADE_RIGHT) != m_panels.end() ||
            m_panels.find(ChildPanelId::PANEL_TRADE_LEFT) != m_panels.end() ||
            m_panels.find(ChildPanelId::PANEL_TRADE_COMMON) != m_panels.end())
        {
            return false;
        }
        // These also cannot be launched if FULLSCREEN panel is active
        if (m_panels.find(ChildPanelId::PANEL_FULLSCREEN) != m_panels.end())
        {
            return false;
        }
        break;

    default:
        // For other panel types, just check if they're not already active
        break;
    }

    return true;
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
    if (panel)
    {
        panel->SetOnShowAnimationImmediate(false);
        panel->SetOnHideAnimationImmediate(false);
    }
}

void MotherPanel::UpdateTabButtonsOnLeaveTown()
{
    // TODO: generated code MotherPanel::UpdateTabButtonsOnLeaveTown
    // Check if the first game data flag is set
    if ((m_gameDataFlags & 1) != 0)
    {
        // Update tab button modes for specific tabs
        for (size_t i = 0; i < m_tabButtons.size(); ++i)
        {
            MotherPanelTabButton* tabButton = m_tabButtons[i];
            if (tabButton && tabButton->GetMode() != MotherPanelTabButton::MODE_NUM_MODES)
            {
                // Update specific tab types that should change when leaving town
                switch (tabButton->GetTabId())
                {
                case TAB_QUESTLOG:
                case TAB_MAP:
                case TAB_JOURNAL:
                case TAB_INVENTORY_VS_SHOP:
                case TAB_CHARACTERISTIC_VS_WORKSHOP:
                {
                    // Only update if we're not in a town
                    if (!M3D_APP->m_pInterfaceManager->GetCurrentTown())
                    {
                        tabButton->SetMode(MotherPanelTabButton::MODE_IN_FIELD);
                    }
                    break;
                }
                default:
                    // Other tab types don't need updating
                    continue;
                }
            }
        }

        // Additional cleanup logic
        if ((m_gameDataFlags & 1) != 0)
        {
            // Remove specific child tab buttons if they exist as direct children
            // Note: Index 5 and 6 likely refer to specific tab button indices
            if (m_tabButtons[5] && IsDirectChild(m_tabButtons[5]))
            {
                RemoveChild(m_tabButtons[5]);
            }

            if ((m_gameDataFlags & 1) != 0 && m_tabButtons[6] && IsDirectChild(m_tabButtons[6]))
            {
                RemoveChild(m_tabButtons[6]);
            }
        }
    }
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
    if ((m_gameDataFlags & 1) != 0)
    {
        std::vector<std::pair<MotherPanel::ChildPanelId, int>> panels;
        if (M3D_APP->m_pInterfaceManager->GetCurrentTown() && !IsPanelPresent(4))
        {
            panels.push_back({PANEL_TOWN, 4});
        }

        panels.push_back({PANEL_PALM, 15});

        // TODO: check this
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
    if ((m_gameDataFlags & 1) != 0)
    {
        std::vector<std::pair<MotherPanel::ChildPanelId, int>> panels;
        if (M3D_APP->m_pInterfaceManager->GetCurrentTown() && !IsPanelPresent(4))
        {
            panels.push_back({PANEL_TOWN, 4});
        }

        panels.push_back({PANEL_RIGHT, 63});
        panels.push_back({PANEL_LEFT, 64});
        panels.push_back({PANEL_VIDEO, 77});

        // TODO: check this
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
    // TODO: generated code MotherPanel::RemoveChildPanel
    if (!childPanel)
    {
        return 0;
    }

    // TODO: check this!
    // Find the child panel in our map
    auto it = std::find_if(
        m_panels.begin(),
        m_panels.end(),
        [&childPanel](std::pair<ChildPanelId, ref_ptr<ChildPanel>> const& entry)
        {
            return entry.second.get() == childPanel.get();
        });

    if (it == m_panels.end())
    {
        return 0;  // Panel not found
    }

    // Remove from UI hierarchy
    RemoveChild(childPanel);

    return 1;
}

void MotherPanel::OnGlobalMap()
{
    m_bCurMapLocal = false;
    SetCurTab(TAB_MAP, true);
}

void MotherPanel::OnCharacteristics()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        std::vector<std::pair<MotherPanel::ChildPanelId, int>> panels;
        if (M3D_APP->m_pInterfaceManager->GetCurrentTown() && !IsPanelPresent(4))
        {
            panels.push_back({PANEL_TOWN, 4});
        }

        panels.push_back({PANEL_RIGHT, 64});
        panels.push_back({PANEL_LEFT, 69});

        // TODO: check this
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
    // TODO: generated code MotherPanel::AdjustChildOrder
    // Only adjust order when a specific game data flag is set
    uint32_t const REORDER_FLAG = 1;  // Bit 0
    if (!(m_gameDataFlags & REORDER_FLAG))
    {
        return;  // No reordering needed
    }

    // Reorder children to bring specific elements to front (top of z-order)
    // This ensures they're drawn on top of other elements

    // 1. Move top panel to front
    MoveChildToFirstPosition(m_wndTopPanel);

    // 2. Move player money display to front
    if (m_wndPlayerMoney)
    {
        MoveChildToFirstPosition(m_wndPlayerMoney.get());
    }

    // 3. Move exit button to front
    MoveChildToFirstPosition(m_btnExit);

    // 4. Move decoration elements to front
    MoveChildToFirstPosition(m_wndDecor);
    MoveChildToFirstPosition(m_wndDecorBar);

    // 5. Move all tab buttons to front
    // m_tabButtons appears to be a std::vector<ref_ptr<MotherPanelTabButton>>
    for (size_t i = 0; i < m_tabButtons.size(); ++i)
    {
        auto* button = m_tabButtons[i];
        if (button && IsDirectChild(button))
        {
            MoveChildToFirstPosition(button);
        }
    }

    // 6. Special case: If panel with ID 88 (0x58) is present,
    // also move window ID 88 to front
    ChildPanelId const SPECIAL_PANEL_ID = static_cast<ChildPanelId>(0x58);  // 88 decimal

    if (IsPanelPresent(SPECIAL_PANEL_ID))
    {
        // Get the window from the interface manager
        ref_ptr<Wnd> specialWindow = M3D_APP->m_pInterfaceManager->GetWindow(88);
        if (specialWindow)
        {
            MoveChildToFirstPosition(specialWindow.get());
        }
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
        if (moneyWnd && moneyWnd->IsKindOf(RT_CLASS_LOCAL(PlayerMoneyWnd)))
        {
            AddChild(moneyWnd);
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
    RETRUXX_NOT_IMPLEMENTED;
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
    // TOOD: generated code MotherPanel::Hide
    // First, handle town conditional closing if applicable
    if (!bQuickLeaveTown)
    {
        auto* currentTown = M3D_APP->m_pInterfaceManager->GetCurrentTown();
        if (currentTown)
        {
            ref_ptr<Wnd> townWnd = M3D_APP->m_pInterfaceManager->GetWindow(4);

            if (auto* townWndCasted = RT_DYNCAST(townWnd.get(), TownDlg))
            {
                CStr townName = currentTown->GetName();
                CStr currentLevel = help::GetCurrentLevelName();

                // Check if town has conditional closing info
                bool hasConditionalClosing = townWndCasted->GetConditionalClosingInfoForTown(townName, currentLevel);

                if (hasConditionalClosing)
                {
                    currentTown->CauseEvent(ai::GE_TOWN_CONDITIONAL_CLOSING, 0.0f, {}, {});
                    return;  // Early return - town handles the closing
                }
            }
        }
    }

    // Clear suspended show panels
    m_suspendedShow.m_suspendedPanels.clear();

    if (GetStation()->IsModal(this))
    {
        // Handle town leaving
        if (M3D_APP->m_pInterfaceManager->GetCurrentTown())
        {
            M3D_APP->m_pInterfaceManager->OnLeaveTown(bQuickLeaveTown);
            UpdateTabButtonsOnLeaveTown();
        }

        // Re-enable vehicle sounds if player exists
        if (ai::thePlayer)
        {
            auto* vehicle = ai::thePlayer->GetVehicle();
            if (vehicle)
            {
                vehicle->EnableSounds(true);
            }
        }

        // Show window with ID 7 (likely game menu or main interface)
        M3D_APP->m_pInterfaceManager->ShowWindow(7, 0, 0, 0, 0, 0);
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

void MotherPanel::OnStartTrade(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::OnWorkshop()
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::SelectTabButton(Tab tabId)
{
    // TODO: generated code MotherPanel::SelectTabButton
    // Only process if the first game data flag is set
    if ((m_gameDataFlags & 1) == 0)
        return;

    // Iterate through all tab buttons
    for (size_t i = 0; i < m_tabButtons.size(); ++i)
    {
        MotherPanelTabButton* tabButton = m_tabButtons[i];

        // Skip null buttons
        if (!tabButton)
            continue;

        // Set selected state based on whether this button's index matches the requested tab
        // Note: The original code uses (tabId == i), suggesting tabId might actually be an index
        // rather than a Tab enum value. This is unusual - see analysis below.
        tabButton->Select(tabId == static_cast<MotherPanel::Tab>(i));
    }
}

void MotherPanel::AdjustDecor()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        bool const bNeedShowDecorBar = IsPanelPresent(PANEL_VIDEO) || IsPanelPresent(PANEL_TRADE_RIGHT);
        m_wndDecorBar->ShowWindow(bNeedShowDecorBar);
    }
}
