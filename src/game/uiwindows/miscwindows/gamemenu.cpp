#include "gamemenu.h"

RT_CLASS_DEFINE(GameMenuWnd);

MenuItem::MenuItem(CStr const&, unsigned, ControlType)
{
    throw std::logic_error("Not implemented");
}

void MenuItem::AddChild(MenuItem*)
{
    throw std::logic_error("Not implemented");
}

MenuItem::~MenuItem()
{
    throw std::logic_error("Not implemented");
}

MenuItem* MenuItem::GetMenuItemViaName(CStr const&)
{
    throw std::logic_error("Not implemented");
}

MenuItem const* MenuItem::GetMenuItemViaName(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* GameMenuWnd::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

CStr GameMenuWnd::GetCurrentLevelName() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* GameMenuWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

CStr GameMenuWnd::GetCurrentRootLevelName() const
{
    throw std::logic_error("Not implemented");
}

GameMenuWnd::~GameMenuWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* GameMenuWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* GameMenuWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

bool GameMenuWnd::SetMenuLevel(CStr const&, CStr const&)
{
    throw std::logic_error("Not implemented");
}

GameMenuWnd::GameMenuWnd() : MenuItem(ROOT_LEVEL_NAME, 101, CT_BUTTON)
{
    throw std::logic_error("Not implemented");
}

GameMenuWnd::GameMenuWnd(GameMenuWnd const&) : MenuItem(ROOT_LEVEL_NAME, 101, CT_BUTTON)
{
    throw std::logic_error("Not implemented");
}

int GameMenuWnd::OnInitModal()
{
    throw std::logic_error("Not implemented");
}

MenuItem* GameMenuWnd::CreateBlankSpaceMenuItem() const
{
    throw std::logic_error("Not implemented");
}

void GameMenuWnd::RecalcLayout()
{
    throw std::logic_error("Not implemented");
}

int GameMenuWnd::OnKey(unsigned short, unsigned char, unsigned)
{
    throw std::logic_error("Not implemented");
}

MenuItem* GameMenuWnd::CreateTextLabelMenuItem(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

int GameMenuWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

int GameMenuWnd::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

int GameMenuWnd::OnActivate(bool)
{
    throw std::logic_error("Not implemented");
}

int GameMenuWnd::CreateGameMenuWnd()
{
    throw std::logic_error("Not implemented");
}

MenuItem* GameMenuWnd::CreateLineMenuItem() const
{
    throw std::logic_error("Not implemented");
}

void GameMenuWnd::CloseCurrentMenuLevel(bool, bool, bool)
{
    throw std::logic_error("Not implemented");
}
