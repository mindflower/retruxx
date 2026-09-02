#include "gamemenu.h"
#include "game/m3dgame.h"
#include "core/log.h"

#include "config.h"
#include "core/console/console.h"
#include "core/kernel.h"
#include "game/uimanager/uidefs.h"
#include "i_event.h"
#include "ui/frame.h"
#include "ui/image.h"
#include "ui/msgbox.h"
#include "ui/ui_srv.h"

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
    return m_curItem ? m_curItem->m_name : CStr();
}

m3d::Object* GameMenuWnd::Clone()
{
    // The shipped game allocates a GameMenuWnd and runs the copy constructor,
    // which rebuilds only an empty root - it does not clone the menu tree.
    return new GameMenuWnd(*this);
}

CStr GameMenuWnd::GetCurrentRootLevelName() const
{
    return m_minItem ? m_minItem->m_name : CStr();
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
    // Matches the shipped copy constructor: it default-constructs the ModalWnd
    // base and re-initialises the MenuItem root ("main", id 101, CT_BUTTON) but
    // deliberately does NOT copy the menu tree or call CreateGameMenuWnd.
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
    // Ported from ExMachina 1.02 NoCD RVA 0x4B61F0.
    Valid();
    RemoveAllChildren();

    // --- Logo -----------------------------------------------------------------
    auto* logo = static_cast<m3d::ui::ImageWnd*>(M3D_KERNEL->New("ImageWnd"));
    bool logoOk = false;
    if (logo)
    {
        BoundsBase<float> logoRc;
        logoRc.x0 = 54.0f;
        logoRc.y0 = 8.0f;
        logoRc.width = 256.0f;
        logoRc.height = 64.0f;
        const m3d::rend::TexHandle ico = M3D_APP->m_pInterfaceManager->GetIcoByName("ExMachinaLogo", 0);
        logoOk = logo->CreateImageWnd(logoRc, ico) != 0;
    }
    if (logoOk)
    {
        Wnd::AddChild(logo);
    }

    // --- Layout metrics from the menu pane frame -----------------------------
    float frameW = 0.0f;
    if (auto* pane = GetGfxServer()->GetPane(m_paneName))
    {
        if (pane->m_frame[0])
        {
            frameW = static_cast<float>(pane->m_frame[0]->m_barUsedWidth);
        }
    }

    const float baseX0 = frameW + 18.0f;
    const float baseWidth = GetBounds().width - baseX0 * 2.0f;
    float y = 88.0f;  // running vertical cursor

    // --- One widget per visible child MenuItem -----------------------------
    const auto& items = m_curItem->m_children;
    for (size_t j = 0; j < items.size(); ++j)
    {
        MenuItem* item = items[j];
        if (!item->m_visible)
        {
            continue;
        }

        m3d::ui::Wnd* w = nullptr;
        switch (item->m_ctrlType)
        {
        case CT_BUTTON:
            w = static_cast<m3d::ui::Wnd*>(M3D_KERNEL->New("ButtonWnd"));
            break;
        case CT_CHECK:
            w = static_cast<m3d::ui::Wnd*>(M3D_KERNEL->New("CheckWnd"));
            break;
        case CT_SLIDER:
            w = static_cast<m3d::ui::Wnd*>(M3D_KERNEL->New("SliderWnd"));
            break;
        case CT_CENTERED_LABEL:
        case CT_BLANK:
            w = static_cast<m3d::ui::Wnd*>(M3D_KERNEL->New("Wnd"));
            break;
        case CT_LINE:
            w = static_cast<m3d::ui::Wnd*>(M3D_KERNEL->New("LineWnd"));
            break;
        }

        float x0 = baseX0;
        float width = baseWidth;
        float height = 32.0f;

        if (w)
        {
            // Shipped style masks: 0xE02 (WS_DISABLE|WS_IS_VISIBLE|WS_TEXT_CENTERED)
            // for disabled items, WS_EMPTY otherwise.
            unsigned flags = item->m_disabled
                                 ? (m3d::ui::WS_DISABLE | m3d::ui::WS_IS_VISIBLE | m3d::ui::WS_TEXT_CENTERED)
                                 : 0u;

            switch (item->m_ctrlType)
            {
            case CT_BLANK:
                height = 24.0f;
                flags = m3d::ui::WS_NOFRAME;  // 0x40
                break;
            case CT_CENTERED_LABEL:
            {
                flags |= m3d::ui::WS_NOFRAME | m3d::ui::WS_TRANSPARENT | m3d::ui::WS_IS_VISIBLE |
                         m3d::ui::WS_TEXT_CENTERED;  // |= 0xF40
                const CStr label = M3D_APP->GetStringByStringId0(item->m_name);
                const PointBase<float> sz = GetGfxServer()->MeasureText(label, 0, m3d::TW_NOWRAP, 0.0f);
                width = sz.x + 2.0f;
                x0 = (baseWidth - sz.x) * 0.5f + baseX0;
                height = sz.y;
                break;
            }
            case CT_SLIDER:
                height = static_cast<float>(GetGfxServer()->GetSliderHeight());
                break;
            default:
                break;
            }

            if (item->m_ctrlType == CT_BUTTON)
            {
                w->SetPane("PaneBtnGray1");

                // Emboss panel drawn behind the button.
                if (auto* emboss = static_cast<m3d::ui::Wnd*>(M3D_KERNEL->New("Wnd")))
                {
                    BoundsBase<float> embossB;
                    embossB.x0 = x0 - 2.0f;
                    embossB.y0 = y - 2.0f;
                    embossB.width = width + 4.0f;
                    embossB.height = height + 4.0f;
                    if (emboss->Create(CStr(), m3d::ui::WS_TRANSPARENT | m3d::ui::WS_IS_VISIBLE, embossB, 0))  // 0x300
                    {
                        emboss->SetPane("PaneEmboss");
                        Wnd::AddChild(emboss);
                    }
                }
            }
            else if (item->m_ctrlType == CT_LINE)
            {
                const CStr linePaneName = "PaneLineMenu";
                if (auto* linePane = GetGfxServer()->GetPane(linePaneName))
                {
                    w->SetPane(linePaneName);
                    if (linePane->m_frame[0])
                    {
                        height = static_cast<float>(linePane->m_frame[0]->m_barTexWidth);
                    }
                }
                x0 = frameW;
                width = m_bounds.width - frameW * 2.0f;
            }

            BoundsBase<float> rc;
            rc.x0 = x0;
            rc.y0 = y;
            rc.width = width;
            rc.height = height;
            w->Create(M3D_APP->GetStringByStringId0(item->m_name), flags, rc, item->m_id);
            w->SetTextColor(0xFF404040);
            w->SetInt(static_cast<unsigned>(j));
            OnInitDlgItem(w, item->m_id);
            Wnd::AddChild(w);
            MoveChildToFirstPosition(w);
        }

        // Advance the cursor; 16px gap between two consecutive non-blank items.
        float gap = 0.0f;
        if (item->m_ctrlType != CT_BLANK && j + 1 < items.size() && items[j + 1]->m_ctrlType != CT_BLANK)
        {
            gap = 16.0f;
        }
        y += gap + height;
    }

    // --- "Back" button (skip at the death level and at the root level) -------
    if (CStr::my_strcmp(m_curItem->m_name.c_str(), DEATH_LEVEL_NAME.c_str()) && m_curItem != m_minItem)
    {
        const CStr backLabel = M3D_APP->GetStringByStringId0("Back");
        if (!m_curItem->m_children.empty())
        {
            y += 32.0f;
        }
        if (auto* back = static_cast<m3d::ui::Wnd*>(M3D_KERNEL->New("ButtonWnd")))
        {
            BoundsBase<float> rc;
            rc.x0 = baseX0;
            rc.y0 = y;
            rc.width = baseWidth;
            rc.height = 32.0f;
            back->Create(backLabel, 0, rc, 102);
            back->SetPane("PaneBtnGray1");
            back->SetTextColor(0xFF404040);
            Wnd::AddChild(back);
        }
    }

    // --- Resize the window to fit its content and recentre it ---------------
    float contentBottom = 0.0f;
    for (m3d::Object* child = GetFirstChild(); child; child = child->GetNextSibling())
    {
        if (child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            const BoundsBase<float> cb = static_cast<m3d::ui::Wnd*>(child)->GetBounds();
            if (cb.y0 + cb.height > contentBottom)
            {
                contentBottom = cb.y0 + cb.height;
            }
        }
    }
    if (contentBottom != 0.0f)
    {
        contentBottom += frameW + 30.0f;
    }

    m_bounds.height = contentBottom;
    m_bounds.x0 = (1024.0f - m_bounds.width) * 0.5f;
    m_bounds.y0 = (768.0f - m_bounds.height) * 0.5f;
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

int GameMenuWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned id, unsigned msg, m3d::AIParam const& data)
{
    Valid();
    if (ModalWnd::OnWndNotify(from, id, msg, data))
    {
        return 1;
    }

    if (msg != 1 && msg != 5)
    {
        return 0;
    }

    int handled = 0;
    switch (id)
    {
    case 102:  // "Back"
    {
        if (m_curItem == m_minItem)
        {
            M3D_APP->EnqueueMessage(UM_GAME_MENU_MODE_EXIT, 0, 0, 0, 0, {}, {});
            M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, false, false, false, false, nullptr);
        }
        else
        {
            m_curItem = m_curItem->m_parent;
            RecalcLayout();
        }
        // The shipped build additionally pins m_minItem to the new level, but only
        // for the "Player Menu" level while an EngineConfig start-level string is
        // empty. That config field is not mapped in retruxx yet, so the pin (which
        // would otherwise trap the user at this level) is skipped.
        // TODO(ExMachina 1.02 NoCD RVA 0x4B5610, case 'f'): m_minItem = m_curItem;
        handled = 1;
        break;
    }
    case 103:  // "SaveGame"
        M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, false, false, false, false, nullptr);
        M3D_APP->EnqueueMessage(UM_SAVEGAME, 0, 0, 0, 0, {}, {});
        handled = 1;
        break;
    case 104:  // "Load Game"
        M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, false, false, false, false, nullptr);
        M3D_APP->EnqueueMessage(UM_LOADGAME, 0, 0, 0, 0, {}, {});
        handled = 1;
        break;
    case 105:  // "Load Last Game"
        m_curItem = m_minItem;
        M3D_APP->EnqueueMessage(UM_GAME_MENU_MODE_EXIT, 0, 0, 0, 0, {}, {});
        M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, false, false, false, false, nullptr);
        M3D_APP->EnqueueMessage(UM_LOAD_LAST_GAME, 0, 0, 0, 0, {}, {});
        handled = 1;
        break;
    case 106:  // "Quit to Main Menu"
        if (CStr::my_strcmp(m_curItem->m_name.c_str(), DEATH_LEVEL_NAME.c_str()))
        {
            if (M3D_APP->RunMsgBoxDlg("", M3D_APP->GetStringByStringId0("ExitCurGame"), 2u, false) !=
                m3d::ui::MBX_RET_OK)
            {
                return 1;
            }
        }
        m_curItem = m_minItem;
        M3D_APP->EnqueueMessage(UM_GAME_MENU_MODE_EXIT, 0, 0, 0, 0, {}, {});
        M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, false, false, false, false, nullptr);
        M3D_APP->EnqueueMessage(UM_QUIT_TO_MAINMENU, 0, 0, 0, 0, {}, {});
        handled = 1;
        break;
    case 107:  // "Quit to Windows"
        if (CStr::my_strcmp(m_curItem->m_name.c_str(), DEATH_LEVEL_NAME.c_str()))
        {
            if (M3D_APP->RunMsgBoxDlg("", M3D_APP->GetStringByStringId0("quitGameWithSaveWarn"), 2u, false) !=
                m3d::ui::MBX_RET_OK)
            {
                return 1;
            }
        }
        m_curItem = m_minItem;
        M3D_APP->EnqueueMessage(UM_GAME_MENU_MODE_EXIT, 0, 0, 0, 0, {}, {});
        M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, false, false, false, false, nullptr);
        M3D_APP->ClearViewportToBlack();
        M3D_APP->m_pInterfaceManager->ShowWindow(IW_WND_DEMO_SPLASH, true, true, true, true, nullptr);
        M3D_APP->EnqueueMessage(1, 0, 0, 0, 0, {}, {});
        M3D_LOG_INFO("Quit to Windows");
        handled = 1;
        break;
    case 108:  // "Options"
        M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, false, false, false, false, nullptr);
        M3D_APP->EnqueueMessage(UM_OPTIONS, 0, 0, 0, 0, {}, {});
        handled = 1;
        break;
    case 109:  // "ResumeGame"
        m_curItem = m_minItem;
        M3D_APP->EnqueueMessage(UM_GAME_MENU_MODE_EXIT, 0, 0, 0, 0, {}, {});
        M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, false, false, false, false, nullptr);
        handled = 1;
        break;
    case 110:  // "Start Game From Begin"
    {
        m_curItem = m_minItem;
        M3D_APP->EnqueueMessage(UM_GAME_MENU_MODE_EXIT, 0, 0, 0, 0, {}, {});
        M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, false, false, false, false, nullptr);
        CStr firstLevel = M3D_ENGINE_CFG.m_firstLevel.GetS();
        M3D_ENGINE_CFG.m_console->executeCommand("/map " + firstLevel);
        handled = 1;
        break;
    }
    default:
        // Navigate into the sub-menu identified by the notifying button's index.
        if (m_curItem && static_cast<int>(from->GetInt()) >= 0)
        {
            int const childCount = static_cast<int>(m_curItem->m_children.size());
            if (static_cast<int>(from->GetInt()) < childCount)
            {
                m_curItem = m_curItem->m_children[from->GetInt()];
                RecalcLayout();
            }
        }
        return 1;
    }

    return handled;
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

    // The "DeathMenu" level is an invisible sub-menu of the root; the items below
    // are its children, not root siblings.
    auto deathItem = new MenuItem(DEATH_LEVEL_NAME, 101, CT_BUTTON);
    deathItem->m_visible = false;

    deathItem->AddChild(CreateTextLabelMenuItem("GameOver"));
    deathItem->AddChild(CreateLineMenuItem());
    deathItem->AddChild(CreateBlankSpaceMenuItem());
    deathItem->AddChild(new MenuItem("Load Last Game", 105, CT_BUTTON));
    deathItem->AddChild(new MenuItem("Load Game", 104, CT_BUTTON));
    deathItem->AddChild(new MenuItem("Start Game From Begin", 110, CT_BUTTON));
    deathItem->AddChild(new MenuItem("Quit to Main Menu", 106, CT_BUTTON));
    deathItem->AddChild(new MenuItem("Quit to Windows", 107, CT_BUTTON));

    MenuItem::AddChild(deathItem);

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

void GameMenuWnd::CloseCurrentMenuLevel(bool bForceClose, bool bResetLevel, bool bExitGameMenuModeOnHide)
{
    if (bForceClose)
    {
        if (bResetLevel)
        {
            m_curItem = m_minItem;
        }
    }
    else if (m_curItem != m_minItem)
    {
        m_curItem = m_curItem->m_parent;
        RecalcLayout();
        return;
    }

    if (bExitGameMenuModeOnHide)
    {
        M3D_APP->EnqueueMessage(UM_GAME_MENU_MODE_EXIT, 0, 0, 0, 0, {}, {});
    }
    M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, false, false, false, false, nullptr);
}
