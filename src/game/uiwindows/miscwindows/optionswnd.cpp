#include "optionswnd.h"

RT_CLASS_EXPORTS_BEGIN(OptionTabButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(OptionTabButton);

RT_CLASS_EXPORTS_BEGIN(OptionsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(OptionsWnd);

OptionsWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* OptionsWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* OptionsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModalWnd);
}

int OptionsWnd::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

OptionsWnd::OptionsWnd()
{
    throw std::logic_error("Not implemented");
}

OptionsWnd::OptionsWnd(OptionsWnd const&)
{
    throw std::logic_error("Not implemented");
}

void OptionsWnd::UpdateTabButtonsStates()
{
    throw std::logic_error("Not implemented");
}

int OptionsWnd::SetCurTab(Tab)
{
    throw std::logic_error("Not implemented");
}

void OptionsWnd::SelectTabButton(Tab)
{
    throw std::logic_error("Not implemented");
}

int OptionsWnd::ApplyTabChanges(Tab)
{
    throw std::logic_error("Not implemented");
}

int OptionsWnd::OnAfterRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

int OptionsWnd::ShowOptionWindowForTab(Tab)
{
    throw std::logic_error("Not implemented");
}

int OptionsWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

int OptionsWnd::GetOptionWindowGuiIdByTabId(Tab) const
{
    throw std::logic_error("Not implemented");
}

int OptionsWnd::CanClose()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* OptionTabButton::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* OptionTabButton::CreateObject()
{
    throw std::logic_error("Not implemented");
}

OptionsWnd::Tab OptionTabButton::GetTabId() const
{
    throw std::logic_error("Not implemented");
}

int OptionTabButton::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    throw std::logic_error("Not implemented");
}

void OptionTabButton::Select(bool)
{
    throw std::logic_error("Not implemented");
}

int OptionTabButton::SetupForTab(OptionsWnd::Tab)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* OptionTabButton::Clone()
{
    throw std::logic_error("Not implemented");
}

OptionTabButton::~OptionTabButton()
{
    throw std::logic_error("Not implemented");
}

bool OptionTabButton::IsSelected() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* OptionTabButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(ButtonWnd);
}

OptionTabButton::OptionTabButton(OptionTabButton const&)
{
    throw std::logic_error("Not implemented");
}

OptionTabButton::OptionTabButton()
{
    throw std::logic_error("Not implemented");
}

void OptionTabButton::UpdatePane()
{
    throw std::logic_error("Not implemented");
}
