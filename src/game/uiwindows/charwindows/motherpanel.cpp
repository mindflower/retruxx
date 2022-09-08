#include "motherpanel.h"
#include "playermoneywnd.h"
#include "childpanel.h"

RT_CLASS_EXPORT_METHOD_DEFINE(MotherPanel, LeaveTown)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORTS_BEGIN(MotherPanel)
	RT_CLASS_EXPORT(MotherPanel, m3d::METHOD, LeaveTown, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MotherPanel);

void MotherPanel::AuxSuspendedShow::Reset()
{
    throw std::logic_error("Not implemented");
}

MotherPanel::AuxSuspendedShow::AuxSuspendedShow()
{
    throw std::logic_error("Not implemented");
}

MotherPanel::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::LeaveTown(bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* MotherPanel::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* MotherPanel::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* MotherPanel::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* MotherPanel::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModalWnd);
}

bool MotherPanel::IsInTownRoot() const
{
    throw std::logic_error("Not implemented");
}

MotherPanel::~MotherPanel()
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::UpdateTabButtonsOnEnterTown(ai::Town const*)
{
    throw std::logic_error("Not implemented");
}

int MotherPanel::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::OnBtnExitClick(m3d::ui::Wnd*, int)
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::ClearPanels(std::vector<ChildPanelId, std::allocator<ChildPanelId>> const&)
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::OnEscape()
{
    throw std::logic_error("Not implemented");
}

int MotherPanel::RemoveChildForce(m3d::Object*)
{
    throw std::logic_error("Not implemented");
}

MotherPanel::ChildPanelId MotherPanel::GetCurrentPanelIdByGuiId(int) const
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::OnHidePanel(void*)
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::ToggleTab(Tab)
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::AdjustAnimationOnShowPanels(std::vector<std::pair<ChildPanelId, int>, std::allocator<std::pair<ChildPanelId, int>>> const&)
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::OnMap()
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::OnAdditionalBuilding()
{
    throw std::logic_error("Not implemented");
}

int MotherPanel::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

int MotherPanel::OnAfterRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::OnShowPanel(void*)
{
    throw std::logic_error("Not implemented");
}

int MotherPanel::AddChildPanel(ref_ptr<ChildPanel>, ChildPanelId)
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::OnShop()
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::OnPickUpAll()
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::OnLocalMap(void*)
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::OnLeaveTown(bool)
{
    throw std::logic_error("Not implemented");
}

bool MotherPanel::PickUpItemsFromGround()
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::OnBuyVehicle()
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::ShowPanels(std::vector<std::pair<ChildPanelId, int>, std::allocator<std::pair<ChildPanelId, int>>>, std::vector<ChildPanelId, std::allocator<ChildPanelId>> const&)
{
    throw std::logic_error("Not implemented");
}

ai::Building* MotherPanel::GetBuildingForTab(Tab) const
{
    throw std::logic_error("Not implemented");
}

int MotherPanel::RemoveChild(m3d::Object*)
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::ShowTabButton(Tab, bool)
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::OnFinishTrade()
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::OnEndWndAnimation()
{
    throw std::logic_error("Not implemented");
}

bool MotherPanel::IsPanelPresent(int) const
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::OnJournal()
{
    throw std::logic_error("Not implemented");
}

bool MotherPanel::CanChildPanelBeLaunchedNow(ChildPanelId) const
{
    throw std::logic_error("Not implemented");
}

MotherPanel::Tab MotherPanel::GetTabForBuilding(ai::Building const*) const
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::AdjustAnimationOnHidePanel(m3d::ui::Wnd*)
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::UpdateTabButtonsOnLeaveTown()
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::OnTalkWithNpc()
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::OnQuestLog()
{
    throw std::logic_error("Not implemented");
}

bool MotherPanel::InTown() const
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::OnInventory()
{
    throw std::logic_error("Not implemented");
}

int MotherPanel::RemoveChildPanelById(ChildPanelId)
{
    throw std::logic_error("Not implemented");
}

int MotherPanel::GetGuiIdByCurrentPanelId(ChildPanelId) const
{
    throw std::logic_error("Not implemented");
}

MotherPanel::Tab MotherPanel::ValidateLastTab() const
{
    throw std::logic_error("Not implemented");
}

int MotherPanel::RemoveChildPanel(ref_ptr<ChildPanel>)
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::OnGlobalMap()
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::OnCharacteristics()
{
    throw std::logic_error("Not implemented");
}

ai::Building const* MotherPanel::GetOnlyBuilding() const
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::AdjustChildOrder()
{
    throw std::logic_error("Not implemented");
}

int MotherPanel::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::OnTown()
{
    throw std::logic_error("Not implemented");
}

MotherPanel::MotherPanel(MotherPanel const&)
{
    throw std::logic_error("Not implemented");
}

MotherPanel::MotherPanel()
{
    throw std::logic_error("Not implemented");
}

int MotherPanel::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::Hide(bool, bool)
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::DestroyHackedWorkshopVehicle()
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::Show()
{
    throw std::logic_error("Not implemented");
}

int MotherPanel::OnKey(unsigned short, unsigned char, unsigned)
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::OnStartTrade(void*)
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::OnTabBtnClick(m3d::ui::Wnd*, int)
{
    throw std::logic_error("Not implemented");
}

int MotherPanel::CreateHackedWorkshopVehicle()
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::OnWorkshop()
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::SetCurTab(Tab, bool)
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::OnEnterTown(ai::Town const*)
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::OnBar()
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::SelectTabButton(Tab)
{
    throw std::logic_error("Not implemented");
}

void MotherPanel::AdjustDecor()
{
    throw std::logic_error("Not implemented");
}
