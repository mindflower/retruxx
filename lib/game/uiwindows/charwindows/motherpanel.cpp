#include "motherpanel.h"
#include "playermoneywnd.h"
#include "childpanel.h"

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

void MotherPanel::ToggleTab(Tab)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
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

int MotherPanel::RemoveChild(m3d::Object*)
{
    RETRUXX_NOT_IMPLEMENTED;
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

bool MotherPanel::IsPanelPresent(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::OnInventory()
{
    RETRUXX_NOT_IMPLEMENTED;
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
    // TODO: implement MotherPanel::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
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
}

int MotherPanel::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
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

void MotherPanel::SetCurTab(Tab, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::OnEnterTown(ai::Town const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::OnBar()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::SelectTabButton(Tab)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanel::AdjustDecor()
{
    RETRUXX_NOT_IMPLEMENTED;
}
