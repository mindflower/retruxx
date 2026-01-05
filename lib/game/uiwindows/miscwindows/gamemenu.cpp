#include "gamemenu.h"
#include "game/m3dgame.h"
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
    for (auto const& child : m_children)
    {
        delete child;
    }
}

MenuItem* MenuItem::GetMenuItemViaName(CStr const& name)
{
    if (!CStr::my_strcmp(GameMenuWnd::ROOT_LEVEL_NAME.c_str(), name.c_str()))
    {
        return this;
    }
    for (auto* child : m_children)
    {
        if (!CStr::my_strcmp(child->m_name.c_str(), name.c_str()))
        {
            return child;
        }
    }
    for (auto* child : m_children)
    {
        auto* result = child->GetMenuItemViaName(name);
        if (result)
        {
            return result;
        }
    }
    return nullptr;
}

MenuItem const* MenuItem::GetMenuItemViaName(CStr const& name) const
{
    return const_cast<MenuItem*>(this)->GetMenuItemViaName(name);
}

m3d::Class* GameMenuWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModalWnd);
}

CStr GameMenuWnd::GetCurrentLevelName() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* GameMenuWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr GameMenuWnd::GetCurrentRootLevelName() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

GameMenuWnd::~GameMenuWnd()
{
}

m3d::Object* GameMenuWnd::CreateObject()
{
    return new GameMenuWnd;
}

m3d::Class* GameMenuWnd::GetClass() const
{
    return RT_CLASS_LOCAL(GameMenuWnd);
}

bool GameMenuWnd::SetMenuLevel(CStr const& levelName, CStr const& rootLevelName)
{
    auto* menuItemViaName = GetMenuItemViaName(levelName);
    if (!menuItemViaName)
    {
        return false;
    }

    if (menuItemViaName->m_children.empty())
    {
        return false;
    }
    MenuItem* minItem = menuItemViaName;
    if (!rootLevelName.empty())
    {
        while (CStr::my_strcmp(minItem->m_name.c_str(), rootLevelName.c_str()))
        {
            minItem = minItem->m_parent;
            if (!minItem)
            {
                return false;
            }
        }
    }
    m_curItem = menuItemViaName;
    m_minItem = minItem;
    RecalcLayout();
    return true;
}

GameMenuWnd::GameMenuWnd() : MenuItem(ROOT_LEVEL_NAME, 101, CT_BUTTON)
{
    CreateGameMenuWnd();
}

GameMenuWnd::GameMenuWnd(GameMenuWnd const&) : MenuItem(ROOT_LEVEL_NAME, 101, CT_BUTTON)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GameMenuWnd::OnInitModal()
{
    Valid();
    RecalcLayout();
    return 1;
}

MenuItem* GameMenuWnd::CreateBlankSpaceMenuItem() const
{
    auto item = new MenuItem({}, 0, CT_BLANK);
    return item;
}

void GameMenuWnd::RecalcLayout()
{
    // TODO: implement GameMenuWnd::RecalcLayout
    // RETRUXX_NOT_IMPLEMENTED;
}

int GameMenuWnd::OnKey(unsigned short key, unsigned char scanCode, unsigned state)
{
    if (key != 1 || state != 1)
    {
        return ModalWnd::OnKey(key, scanCode, state);
    }

    if (CStr::my_strcmp(m_curItem->m_name.c_str(), GameMenuWnd::DEATH_LEVEL_NAME.c_str()))
    {
        if (m_curItem != m_minItem)
        {
            m_curItem = m_curItem->m_parent;
            RecalcLayout();
            return 1;
        }

        M3D_APP->EnqueueMessage(UM_GAME_MENU_MODE_EXIT, 0, 0, 0, 0, {}, {});
        M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, 0, 0, 0, 0, 0);
    }
    return 1;
}

MenuItem* GameMenuWnd::CreateTextLabelMenuItem(CStr const& name) const
{
    auto item = new MenuItem(name, 0, CT_CENTERED_LABEL);
    return item;
}

int GameMenuWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GameMenuWnd::OnBeforeAddToWndStation()
{
    M3D_APP->EnqueueMessage(UM_GAME_MENU_MODE_ENTER, 0, 0, 0, 0, {}, {});
    return Wnd::OnBeforeAddToWndStation();
}

int GameMenuWnd::OnActivate(bool on)
{
    Valid();
    GetStation()->CaptureFocus(on ? this : nullptr);
    return 1;
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
    RETRUXX_NOT_IMPLEMENTED;
}
