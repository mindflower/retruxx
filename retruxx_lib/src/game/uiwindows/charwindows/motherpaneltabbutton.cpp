#include "motherpaneltabbutton.h"

RT_CLASS_EXPORTS_BEGIN(MotherPanelTabButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MotherPanelTabButton);

m3d::rend::TexHandle MotherPanelTabButton::PerModeInfo::GetUnselTex() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

MotherPanelTabButton::PerModeInfo::PerModeInfo(m3d::rend::TexHandle, m3d::rend::TexHandle)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::rend::TexHandle MotherPanelTabButton::PerModeInfo::GetSelTex() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

MotherPanelTabButton::PerModeInfo::~PerModeInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* MotherPanelTabButton::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

MotherPanel::Tab MotherPanelTabButton::GetTabId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanelTabButton::Select(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool MotherPanelTabButton::IsSelected() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* MotherPanelTabButton::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

MotherPanelTabButton::Mode MotherPanelTabButton::GetMode() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* MotherPanelTabButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(ButtonWnd);
}

MotherPanelTabButton::~MotherPanelTabButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MotherPanelTabButton::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MotherPanelTabButton::SetupForTab(MotherPanel::Tab)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanelTabButton::SetMode(Mode)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* MotherPanelTabButton::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanelTabButton::UpdateTex()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanelTabButton::ClearInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr MotherPanelTabButton::Mode2Str(Mode)
{
    RETRUXX_NOT_IMPLEMENTED;
}

MotherPanelTabButton::MotherPanelTabButton(MotherPanelTabButton const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

MotherPanelTabButton::MotherPanelTabButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanelTabButton::UpdateTooltip()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool MotherPanelTabButton::CanApplyMode(Mode) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool MotherPanelTabButton::HasMode(Mode) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MotherPanelTabButton::InitInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Building const* MotherPanelTabButton::GetBuilding(Mode) const
{
    RETRUXX_NOT_IMPLEMENTED;
}
