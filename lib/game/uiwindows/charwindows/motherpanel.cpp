#include "motherpanel.h"
#include "playermoneywnd.h"
#include "childpanel.h"
#include <core/log.h>
#include "ui/image.h"
#include "ui/button.h"
#include "motherpaneltabbutton.h"
#include <game/m3dgame.h>
#include <game/uimanager/uidefs.h>
#include <game/music/townmusicmanager.h>
#include <server/server.h>

RT_CLASS_EXPORT_METHOD_DEFINE(MotherPanel, LeaveTown)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(MotherPanel)
RT_CLASS_EXPORT(MotherPanel, m3d::METHOD, LeaveTown, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MotherPanel);

void MotherPanel::AuxSuspendedShow::Reset()
{
    RETRUXX_NOT_IMPLEMENTED;
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

void MotherPanel::LeaveTown(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* MotherPanel::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModalWnd);
}

bool MotherPanel::IsInTownRoot() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

MotherPanel::~MotherPanel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::UpdateTabButtonsOnEnterTown(ai::Town const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MotherPanel::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::OnBtnExitClick(m3d::ui::Wnd*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::ClearPanels(std::vector<ChildPanelId, std::allocator<ChildPanelId>> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::OnEscape()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MotherPanel::RemoveChildForce(m3d::Object*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

MotherPanel::ChildPanelId MotherPanel::GetCurrentPanelIdByGuiId(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::OnHidePanel(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    std::vector<std::pair<ChildPanelId, int>, std::allocator<std::pair<ChildPanelId, int>>> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::OnMap()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::OnAdditionalBuilding()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MotherPanel::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MotherPanel::OnAfterRemoveFromWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::OnShowPanel(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MotherPanel::AddChildPanel(ref_ptr<ChildPanel>, ChildPanelId)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::OnShop()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::OnPickUpAll()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::OnLocalMap(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::OnLeaveTown(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    std::vector<std::pair<ChildPanelId, int>, std::allocator<std::pair<ChildPanelId, int>>>,
    std::vector<ChildPanelId, std::allocator<ChildPanelId>> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Building* MotherPanel::GetBuildingForTab(Tab) const
{
    RETRUXX_NOT_IMPLEMENTED;
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

void MotherPanel::ShowTabButton(Tab, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::OnFinishTrade()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::OnEndWndAnimation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool MotherPanel::IsPanelPresent(int guiId) const
{
    ref_ptr wnd = M3D_APP->m_pInterfaceManager->GetWindow(guiId);
    return wnd && wnd->IsChildOf(this);
}

void MotherPanel::OnJournal()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool MotherPanel::CanChildPanelBeLaunchedNow(ChildPanelId) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

MotherPanel::Tab MotherPanel::GetTabForBuilding(ai::Building const*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::AdjustAnimationOnHidePanel(m3d::ui::Wnd*)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::OnQuestLog()
{
    RETRUXX_NOT_IMPLEMENTED;
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

int MotherPanel::RemoveChildPanelById(ChildPanelId)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MotherPanel::GetGuiIdByCurrentPanelId(ChildPanelId) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

MotherPanel::Tab MotherPanel::ValidateLastTab() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MotherPanel::RemoveChildPanel(ref_ptr<ChildPanel>)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::OnGlobalMap()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::OnCharacteristics()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Building const* MotherPanel::GetOnlyBuilding() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::AdjustChildOrder()
{
    RETRUXX_NOT_IMPLEMENTED;
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

MotherPanel::MotherPanel(MotherPanel const&)
{
    RETRUXX_NOT_IMPLEMENTED;
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

int MotherPanel::GameDataUpdate(void*, int dataType)
{
    switch (dataType)
    {
    case 2:
        // TODO: check this
        if (IsPanelPresent(IW_DLG_TALK_WITH_NPC) || (!IsChildOf(M3D_APP) && PickUpItemsFromGround()))
        {
            return 1;
        }
        ToggleTab(TAB_INVENTORY_VS_SHOP);
        return 1;

    default:
        RETRUXX_NOT_IMPLEMENTED;
    }
    // TODO: implement GameDataUpdate
    RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

void MotherPanel::Hide(bool, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::DestroyHackedWorkshopVehicle()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::Show()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MotherPanel::OnKey(unsigned short, unsigned char, unsigned)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::OnStartTrade(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::OnTabBtnClick(m3d::ui::Wnd*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
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

void MotherPanel::OnEnterTown(ai::Town const*)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}
