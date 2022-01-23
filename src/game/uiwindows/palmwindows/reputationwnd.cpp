#include "reputationwnd.h"

RT_CLASS_DEFINE(ReputationButton);

m3d::Object* ReputationButton::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ReputationButton::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

int ReputationButton::GetClanBelong() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* ReputationButton::CreateObject()
{
    throw std::logic_error("Not implemented");
}

int ReputationButton::SetUpForClan(int)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ReputationButton::GetClass() const
{
    throw std::logic_error("Not implemented");
}

ReputationButton::~ReputationButton()
{
    throw std::logic_error("Not implemented");
}

CStr ReputationButton::BWTolerance2StrId2(help::BlackWightTolerance) const
{
    throw std::logic_error("Not implemented");
}

CStr ReputationButton::BWTolerance2StrId1(help::BlackWightTolerance)
{
    throw std::logic_error("Not implemented");
}

void ReputationButton::ClearPattern()
{
    throw std::logic_error("Not implemented");
}

ReputationButton::ReputationButton(ReputationButton const&)
{
    throw std::logic_error("Not implemented");
}

ReputationButton::ReputationButton()
{
    throw std::logic_error("Not implemented");
}

int ReputationButton::CreateChildren()
{
    throw std::logic_error("Not implemented");
}

unsigned ReputationButton::BWTolerance2Color(help::BlackWightTolerance) const
{
    throw std::logic_error("Not implemented");
}

void ReputationButton::UpdateTolerance()
{
    throw std::logic_error("Not implemented");
}

int ReputationButton::CreateFromPattern()
{
    throw std::logic_error("Not implemented");
}

void ReputationButton::UpdateToleranceIndicator(help::BlackWightTolerance)
{
    throw std::logic_error("Not implemented");
}

void ReputationButton::UpdateTooltip(help::BlackWightTolerance)
{
    throw std::logic_error("Not implemented");
}

int ReputationButton::LoadPattern(m3d::ui::Wnd*)
{
    throw std::logic_error("Not implemented");
}

ReputationButton* AuxRPanel::GetButtonBySlot(Slot) const
{
    throw std::logic_error("Not implemented");
}

ReputationButton* AuxRPanel::AddButton(int, m3d::ui::Wnd*)
{
    throw std::logic_error("Not implemented");
}

AuxRPanel::AuxRPanel()
{
    throw std::logic_error("Not implemented");
}

AuxRPanel::~AuxRPanel()
{
    throw std::logic_error("Not implemented");
}

bool AuxRPanel::HasEmptySlot() const
{
    throw std::logic_error("Not implemented");
}

Slot AuxRPanel::GetEmptySlotId() const
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_DEFINE(ReputationList);

ReputationList::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

int ReputationList::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ReputationList::GetClass() const
{
    throw std::logic_error("Not implemented");
}

ReputationButton* ReputationList::AddButtonByClanBelong(int)
{
    throw std::logic_error("Not implemented");
}

void ReputationList::UpdateClans()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* ReputationList::Clone()
{
    throw std::logic_error("Not implemented");
}

ReputationList::~ReputationList()
{
    throw std::logic_error("Not implemented");
}

int ReputationList::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* ReputationList::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ReputationList::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

ReputationList::ReputationList()
{
    throw std::logic_error("Not implemented");
}

ReputationList::ReputationList(ReputationList const&)
{
    throw std::logic_error("Not implemented");
}

PointBase<int> ReputationList::GetButtonPanelCoord(ReputationButton const*) const
{
    throw std::logic_error("Not implemented");
}

int ReputationList::GetPanelIdFitToPlaceButton()
{
    throw std::logic_error("Not implemented");
}

void ReputationList::PlaceButton(ReputationButton*)
{
    throw std::logic_error("Not implemented");
}

int ReputationList::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

ReputationButton* ReputationList::GetButtonByClanBelong(int) const
{
    throw std::logic_error("Not implemented");
}

void ReputationList::ClearClans()
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_DEFINE(ReputationWnd);

ReputationWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* ReputationWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ReputationWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* ReputationWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ReputationWnd::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

ReputationWnd::~ReputationWnd()
{
    throw std::logic_error("Not implemented");
}

ReputationWnd::ReputationWnd(ReputationWnd const&)
{
    throw std::logic_error("Not implemented");
}

ReputationWnd::ReputationWnd()
{
    throw std::logic_error("Not implemented");
}

void ReputationWnd::OnKnownClansChanged()
{
    throw std::logic_error("Not implemented");
}

int ReputationWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

int ReputationWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

int ReputationWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

int ReputationWnd::OnAfterAddToWndStation()
{
    throw std::logic_error("Not implemented");
}
