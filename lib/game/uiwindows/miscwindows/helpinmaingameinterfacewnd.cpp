#include "helpinmaingameinterfacewnd.h"

#include <core/log.h>
#include <game/m3dgame.h>
#include <game/uimanager/truxxuimanager.h>
#include <m3dapp.h>

namespace
{
    // Control ids of the main-game-interface widgets this window explains. They
    // are laid out in the same order as the help ids they map onto.
    int const ID_CTRL_FIRST = 70000;
}  // namespace

RT_CLASS_EXPORTS_BEGIN(HelpInMainGameInterfaceWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(HelpInMainGameInterfaceWnd);

HelpInMainGameInterfaceWnd::AuxInfo::AuxInfo()
{
    // RVA 0x4B8D90
    m_wndHelpTextName = "wndHelpText";
    m_wndTitleName = "wndTitle";
}

HelpInMainGameInterfaceWnd::HelpInMainGameInterfaceWnd()
{
    // RVA 0x4B8E20 - NOTE: unlike HelpSimpleMsgWnd the shipped constructor does
    // not null m_wndHelpText / m_wndTitle; they only become defined once
    // GameDataSetup has run.
}

HelpInMainGameInterfaceWnd::HelpInMainGameInterfaceWnd(HelpInMainGameInterfaceWnd const&)
{
    // RVA 0x4B8EF0 - shares its body with the default constructor and copies
    // nothing from rhs.
}

HelpInMainGameInterfaceWnd::~HelpInMainGameInterfaceWnd()
{
    // RVA 0x4B8F40 - only the aux info and the HelpWnd base need unwinding.
}

m3d::Object* HelpInMainGameInterfaceWnd::Clone()
{
    // RVA 0x4B8D10
    return new HelpInMainGameInterfaceWnd(*this);
}

m3d::Object* HelpInMainGameInterfaceWnd::CreateObject()
{
    return new HelpInMainGameInterfaceWnd;
}

m3d::Class* HelpInMainGameInterfaceWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(HelpWnd);
}

m3d::Class* HelpInMainGameInterfaceWnd::GetClass() const
{
    return RT_CLASS_LOCAL(HelpInMainGameInterfaceWnd);
}

int HelpInMainGameInterfaceWnd::GameDataSetup()
{
    // RVA 0x4B8FC0
    if (!HelpWnd::GameDataSetup())
    {
        return 0;
    }

    if ((m_gameDataFlags & 2) == 0)
    {
        int res = 1;

        m3d::Object* wndHelpText = GetChildByName(m_aif.m_wndHelpTextName);
        if (wndHelpText && wndHelpText->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_wndHelpText = static_cast<m3d::ui::Wnd*>(wndHelpText);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndHelpTextName + " is not found or incorrect type");
            res = 0;
        }

        m3d::Object* wndTitle = GetChildByName(m_aif.m_wndTitleName);
        if (wndTitle && wndTitle->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_wndTitle = static_cast<m3d::ui::Wnd*>(wndTitle);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndTitleName + " is not found or incorrect type");
            res = 0;
        }

        if (!res)
        {
            // The shipped code clears the ready bit HelpWnd set rather than
            // tracking the failure separately.
            m_gameDataFlags &= ~1u;
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }

    M3D_LOG_INFO("HelpInMainGameInterfaceWnd: error - fail to init because of a bad resource");
    return 0;
}

int HelpInMainGameInterfaceWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned id, unsigned msg, m3d::AIParam const& data)
{
    // RVA 0x4B91E0 - hovering one of the interface widgets shows its help text,
    // leaving it again blanks the panel.
    if (ModalWnd::OnWndNotify(from, id, msg, data))
    {
        return 1;
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        if (msg == 7)
        {
            HelpManager::HelpId const helpId = GetHelpIdByCtrlId(static_cast<int>(id));
            if (helpId != HelpManager::NUM_HELP_IDS)
            {
                ShowHelp(helpId);
                return 1;
            }
        }
        else if (msg == 8 && GetHelpIdByCtrlId(static_cast<int>(id)) != HelpManager::NUM_HELP_IDS)
        {
            HideHelp();
            return 1;
        }
    }
    return 0;
}

int HelpInMainGameInterfaceWnd::OnBeforeAddToWndStation()
{
    // RVA 0x4B94A0 - the panel opens on its own "how to use this help" page.
    ShowHelp(HelpManager::HELP_ID_HOW_TO_USE_MAIN_GAME_INTERFACE_HELP);
    CheckAndShowAutoHelpControls();
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

void HelpInMainGameInterfaceWnd::ShowHelp(HelpManager::HelpId helpId)
{
    // RVA 0x4B9260
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    CStr const strHelpId = HelpManager::Enumered2StrHelpId(helpId);
    m_wndHelpText->SetText(M3D_APP->m_pInterfaceManager->GetHelpManager()->GetHelpMsgByHelpId(strHelpId));
    m_wndTitle->SetText(M3D_APP->m_pInterfaceManager->GetHelpManager()->GetHelpTitleByHelpId(strHelpId));
}

void HelpInMainGameInterfaceWnd::HideHelp()
{
    // RVA 0x4B9390
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndHelpText->SetText(CStr());
        m_wndTitle->SetText(CStr());
    }
}

HelpManager::HelpId HelpInMainGameInterfaceWnd::GetHelpIdByCtrlId(int ctrlId) const
{
    // RVA 0x4B9410
    switch (ctrlId)
    {
    case ID_CTRL_FIRST + 0:
        return HelpManager::HELP_ID_RADAR;
    case ID_CTRL_FIRST + 1:
        return HelpManager::HELP_ID_SPEEDOMETER;
    case ID_CTRL_FIRST + 2:
        return HelpManager::HELP_ID_HP;
    case ID_CTRL_FIRST + 3:
        return HelpManager::HELP_ID_FUEL;
    case ID_CTRL_FIRST + 4:
        return HelpManager::HELP_ID_DURABILITY;
    case ID_CTRL_FIRST + 5:
        return HelpManager::HELP_ID_TIMER;
    case ID_CTRL_FIRST + 6:
        return HelpManager::HELP_ID_WEAPON;
    case ID_CTRL_FIRST + 7:
        return HelpManager::HELP_ID_FADING_MESSAGES;
    case ID_CTRL_FIRST + 8:
        return HelpManager::HELP_ID_IMPORTANT_FADING_MESSAGES;
    case ID_CTRL_FIRST + 9:
        return HelpManager::HELP_ID_TARGET_INFO;
    case ID_CTRL_FIRST + 10:
        return HelpManager::HELP_ID_TARGET_CAPTURE;
    case ID_CTRL_FIRST + 11:
        return HelpManager::HELP_ID_ACTION_ICONS;
    default:
        return HelpManager::NUM_HELP_IDS;
    }
}
