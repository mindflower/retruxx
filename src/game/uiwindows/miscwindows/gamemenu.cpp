#include "gamemenu.h"
#include "m3dapp.h"
#include "core/log.h"

RT_CLASS_EXPORTS_BEGIN(GameMenuWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(GameMenuWnd);

MenuItem::MenuItem(CStr const& name, unsigned id, ControlType ct)
{
    m_name = name;
    m_id = id;
    m_children.clear();
    m_parent = nullptr;
    m_disabled = false;
    m_ctrlType = ct;
    m_visible = true;
}

void MenuItem::AddChild(MenuItem* item)
{
    if (item)
    {
        item->m_parent = this;
        m_children.push_back(item);
    }
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
    return RT_CLASS_LOCAL(ModalWnd);
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
    return new GameMenuWnd;
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
    CreateGameMenuWnd();
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
    auto item = new MenuItem({}, 0, CT_BLANK);
    return item;
}

void GameMenuWnd::RecalcLayout()
{
    throw std::logic_error("Not implemented");
}

int GameMenuWnd::OnKey(unsigned short, unsigned char, unsigned)
{
    throw std::logic_error("Not implemented");
}

MenuItem* GameMenuWnd::CreateTextLabelMenuItem(CStr const& name) const
{
    auto item = new MenuItem(name, 0, CT_CENTERED_LABEL);
    return item;
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
    auto bounds = m3d::Application::g_pApp->GetBounds();
    BoundsBase<float> rc;
    rc.width = 320.0;
    rc.height = 448.0;
    rc.x0 = (bounds.width - 320.0) * 0.5;
    rc.y0 = (bounds.height - 448.0) * 0.5;
    if (!ModalWnd::Create({}, 0x4E00, rc, 3))
    {
        M3D_LOG_INFO("GameMenuWnd::CreateGameMenuWnd error - cannot create ModalWnd");
        return 0;
    }
    SetPane("PaneMenu");
    SetTextColor(0xFF404040);

    auto saveGameItem = new MenuItem(SAVE_GAME_MENUITEM_NAME, 103, CT_BUTTON);
    MenuItem::AddChild(saveGameItem);

    auto loadGameItem = new MenuItem("Load Game", 104, CT_BUTTON);
    MenuItem::AddChild(loadGameItem);

    auto optionsItem = new MenuItem("Options", 108, CT_BUTTON);
    MenuItem::AddChild(optionsItem);

    auto quitMenuItem = new MenuItem("Quit to Main Menu", 106, CT_BUTTON);
    MenuItem::AddChild(quitMenuItem);

    auto quitWindowsItem = new MenuItem("Quit to Windows", 107, CT_BUTTON);
    MenuItem::AddChild(quitWindowsItem);

    auto space = CreateBlankSpaceMenuItem();
    MenuItem::AddChild(space);

    auto line = CreateLineMenuItem();
    MenuItem::AddChild(line);

    //TODO: check this
    auto space2 = CreateBlankSpaceMenuItem();
    MenuItem::AddChild(space2);

    auto resumeItem = new MenuItem("ResumeGame", 109, CT_BUTTON);
    MenuItem::AddChild(resumeItem);

    auto deathItem = new MenuItem(DEATH_LEVEL_NAME, 101, CT_BUTTON);
    MenuItem::AddChild(deathItem);

    auto gameOverItem = CreateTextLabelMenuItem("GameOver");
    MenuItem::AddChild(gameOverItem);

    auto line2 = CreateLineMenuItem();
    MenuItem::AddChild(line2);

    auto space3 = CreateBlankSpaceMenuItem();
    MenuItem::AddChild(space3);

    auto lastGameItem = new MenuItem("Load Last Game", 105, CT_BUTTON);
    MenuItem::AddChild(lastGameItem);

    auto loadItem = new MenuItem("Load Game", 104, CT_BUTTON);
    MenuItem::AddChild(loadItem);

    auto startBeginItem = new MenuItem("Start Game From Begin", 110, CT_BUTTON);
    MenuItem::AddChild(startBeginItem);

    auto quitToMainMenu = new MenuItem("Quit to Main Menu", 106, CT_BUTTON);
    MenuItem::AddChild(quitToMainMenu);

    auto quitToWindows = new MenuItem("Quit to Windows", 107, CT_BUTTON);
    MenuItem::AddChild(quitToWindows);

    m_gameDataFlags |= 1u;
    m_curItem = this;
    m_minItem = this;
    return 1;
}

MenuItem* GameMenuWnd::CreateLineMenuItem() const
{
    auto item = new MenuItem({}, 0, CT_LINE);
    return item;
}

void GameMenuWnd::CloseCurrentMenuLevel(bool, bool, bool)
{
    throw std::logic_error("Not implemented");
}
