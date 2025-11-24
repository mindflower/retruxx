#include "advancedlist.h"

RT_CLASS_EXPORTS_BEGIN(AdvancedButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(AdvancedButton);

int AdvancedButton::GetObjId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AdvancedButton::SetMode(Mode)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* AdvancedButton::GetRtClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int AdvancedButton::SetupForObj(int, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

AdvancedButton::~AdvancedButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* AdvancedButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int AdvancedButton::GetPrice() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int AdvancedButton::GetUnitsToBuy() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int AdvancedButton::SetupForWorkshop(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& AdvancedButton::GetStrIdTooltipBuyUnavailable() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AdvancedButton::UpdatePrevValues()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int AdvancedButton::CreateChildren()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& AdvancedButton::GetStrIdTooltipBuyPartial() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int AdvancedButton::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AdvancedButton::UpdatePriceControls(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AdvancedButton::OnLeft()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AdvancedButton::UpdateTooltip(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AdvancedButton::OnRight()
{
    RETRUXX_NOT_IMPLEMENTED;
}

AdvancedButton::AdvancedButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AdvancedButton::UpdateSlider(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& AdvancedButton::GetStrIdTooltipBuyNotNeed() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AdvancedButton::UpdateServiceName()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AdvancedButton::OnBuy()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int AdvancedButton::GetUnitsPrice(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& AdvancedButton::GetStrIdTooltipBuy() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AdvancedButton::UpdateServiceIco()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& AdvancedButton::GetStrIdTooltipBuyFull() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int AdvancedButton::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AdvancedButton::OnChangeMode()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AdvancedButton::UpdateLeftRightButtonsState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AdvancedButton::UpdateUnitsControls(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Workshop* AdvancedButton::GetWorkshop() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int AdvancedButton::LoadPattern(m3d::ui::Wnd*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int AdvancedButton::GetMaxPossibleUnitsToBuy() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AdvancedButton::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AdvancedButton::UpdateAdvanceButtonState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int AdvancedButton::CreateFromPattern()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AdvancedButton::UpdateControlsOnNewFrame(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

unsigned AdvancedButton::EnumColor2Color(help::Color) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AdvancedButton::UpdateValues()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AdvancedButton::OnBtnAdvanceClick()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AdvancedButton::RecalcLayot()
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(AdvancedList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(AdvancedList);

AdvancedList::AuxInfo::AuxInfo()
{
    m_space = 6.0;
}

void AdvancedList::SetSwitchWndBounds(BoundsBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* AdvancedList::GetRtClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* AdvancedList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int AdvancedList::SetupForWorkshop(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

AdvancedList::~AdvancedList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AdvancedList::OnVehiclePartChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

AdvancedList::AdvancedList()
{
    m_switchB.x0 = 0.0;
    m_switchB.y0 = 0.0;
    m_switchB.width = 0.0;
    m_switchB.height = 0.0;
}

int AdvancedList::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Workshop* AdvancedList::GetWorkshop() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int AdvancedList::CreateItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AdvancedList::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int AdvancedList::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AdvancedList::RecalcLayot()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int AdvancedList::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int AdvancedList::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AdvancedList::OnAdvancedModeChanged(m3d::ui::Wnd*, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int AdvancedList::GameDataSetup()
{
    // TODO: implement AdvancedList::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

int AdvancedList::AddItem(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AdvancedList::OnPlayerVehicleChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

AdvancedButton* AdvancedList::NewItem() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AdvancedList::SetAdvancedModeForButton(AdvancedButton*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void AdvancedList::ClearItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}
