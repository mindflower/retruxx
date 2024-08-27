#include "motherpaneltabbutton.h"

RT_CLASS_EXPORTS_BEGIN(MotherPanelTabButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MotherPanelTabButton);

m3d::rend::TexHandle MotherPanelTabButton::PerModeInfo::GetUnselTex() const
{
    throw std::logic_error("Not implemented");
}

MotherPanelTabButton::PerModeInfo::PerModeInfo(m3d::rend::TexHandle, m3d::rend::TexHandle)
{
    throw std::logic_error("Not implemented");
}

m3d::rend::TexHandle MotherPanelTabButton::PerModeInfo::GetSelTex() const
{
    throw std::logic_error("Not implemented");
}

MotherPanelTabButton::PerModeInfo::~PerModeInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* MotherPanelTabButton::GetClass() const
{
    throw std::logic_error("Not implemented");
}

MotherPanel::Tab MotherPanelTabButton::GetTabId() const
{
    throw std::logic_error("Not implemented");
}

void MotherPanelTabButton::Select(bool)
{
    throw std::logic_error("Not implemented");
}

bool MotherPanelTabButton::IsSelected() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* MotherPanelTabButton::Clone()
{
    throw std::logic_error("Not implemented");
}

MotherPanelTabButton::Mode MotherPanelTabButton::GetMode() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* MotherPanelTabButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(ButtonWnd);
}

MotherPanelTabButton::~MotherPanelTabButton()
{
    throw std::logic_error("Not implemented");
}

int MotherPanelTabButton::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    throw std::logic_error("Not implemented");
}

int MotherPanelTabButton::SetupForTab(MotherPanel::Tab)
{
    throw std::logic_error("Not implemented");
}

void MotherPanelTabButton::SetMode(Mode)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* MotherPanelTabButton::CreateObject()
{
    throw std::logic_error("Not implemented");
}

void MotherPanelTabButton::UpdateTex()
{
    throw std::logic_error("Not implemented");
}

void MotherPanelTabButton::ClearInfo()
{
    throw std::logic_error("Not implemented");
}

CStr MotherPanelTabButton::Mode2Str(Mode)
{
    throw std::logic_error("Not implemented");
}

MotherPanelTabButton::MotherPanelTabButton(MotherPanelTabButton const&)
{
    throw std::logic_error("Not implemented");
}

MotherPanelTabButton::MotherPanelTabButton()
{
    throw std::logic_error("Not implemented");
}

void MotherPanelTabButton::UpdateTooltip()
{
    throw std::logic_error("Not implemented");
}

bool MotherPanelTabButton::CanApplyMode(Mode) const
{
    throw std::logic_error("Not implemented");
}

bool MotherPanelTabButton::HasMode(Mode) const
{
    throw std::logic_error("Not implemented");
}

void MotherPanelTabButton::InitInfo()
{
    throw std::logic_error("Not implemented");
}

ai::Building const* MotherPanelTabButton::GetBuilding(Mode) const
{
    throw std::logic_error("Not implemented");
}
