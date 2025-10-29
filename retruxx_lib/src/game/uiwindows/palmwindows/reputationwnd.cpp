#include "reputationwnd.h"

RT_CLASS_EXPORTS_BEGIN(ReputationButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ReputationButton);

m3d::Object* ReputationButton::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* ReputationButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(ButtonWnd);
}

int ReputationButton::GetClanBelong() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* ReputationButton::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ReputationButton::SetUpForClan(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* ReputationButton::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ReputationButton::~ReputationButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr ReputationButton::BWTolerance2StrId2(help::BlackWightTolerance) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr ReputationButton::BWTolerance2StrId1(help::BlackWightTolerance)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ReputationButton::ClearPattern()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ReputationButton::ReputationButton(ReputationButton const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ReputationButton::ReputationButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ReputationButton::CreateChildren()
{
    RETRUXX_NOT_IMPLEMENTED;
}

unsigned ReputationButton::BWTolerance2Color(help::BlackWightTolerance) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ReputationButton::UpdateTolerance()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ReputationButton::CreateFromPattern()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ReputationButton::UpdateToleranceIndicator(help::BlackWightTolerance)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ReputationButton::UpdateTooltip(help::BlackWightTolerance)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ReputationButton::LoadPattern(m3d::ui::Wnd*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ReputationButton* AuxRPanel::GetButtonBySlot(Slot) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ReputationButton* AuxRPanel::AddButton(int, m3d::ui::Wnd*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

AuxRPanel::AuxRPanel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

AuxRPanel::~AuxRPanel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool AuxRPanel::HasEmptySlot() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

Slot AuxRPanel::GetEmptySlotId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(ReputationList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ReputationList);

ReputationList::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ReputationList::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* ReputationList::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ReputationButton* ReputationList::AddButtonByClanBelong(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ReputationList::UpdateClans()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* ReputationList::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ReputationList::~ReputationList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ReputationList::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* ReputationList::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* ReputationList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

ReputationList::ReputationList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ReputationList::ReputationList(ReputationList const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

PointBase<int> ReputationList::GetButtonPanelCoord(ReputationButton const*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ReputationList::GetPanelIdFitToPlaceButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ReputationList::PlaceButton(ReputationButton*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ReputationList::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ReputationButton* ReputationList::GetButtonByClanBelong(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ReputationList::ClearClans()
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(ReputationWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ReputationWnd);

ReputationWnd::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* ReputationWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* ReputationWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* ReputationWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* ReputationWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ScreenWnd);
}

ReputationWnd::~ReputationWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ReputationWnd::ReputationWnd(ReputationWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ReputationWnd::ReputationWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ReputationWnd::OnKnownClansChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ReputationWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ReputationWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ReputationWnd::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ReputationWnd::OnAfterAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}
