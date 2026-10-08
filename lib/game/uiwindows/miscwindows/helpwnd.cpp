#include "helpwnd.h"

#include <core/log.h>
#include <game/m3dgame.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimisc/helpmanager.h>
#include <m3dapp.h>
#include <ui/button.h>

RT_CLASS_EXPORTS_BEGIN(HelpWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(HelpWnd);

HelpWnd::AuxInfo::AuxInfo()
{
    // RVA 0x55B6F0
    m_checkDisableAutoHelpName = "checkDisableAutoHelp";
    m_wndCheckDisableAutoHelpEmbossName = "checkDisableAutoHelp_emboss";
    m_lblDisableAutoHelpName = "lblDisableAutoHelp";
}

HelpWnd::HelpWnd()
{
    // RVA 0x55B7D0
    m_checkDisableAutoHelp = nullptr;
    m_wndCheckDisableAutoHelpEmboss = nullptr;
    m_lblDisableAutoHelp = nullptr;
    m_bAllowDisableAutoHelp = false;
}

HelpWnd::HelpWnd(HelpWnd const&)
{
    // RVA 0x55B8C0 - the shipped copy constructor copies nothing from rhs and,
    // unlike the default one, leaves the three control pointers and the
    // "may be disabled" flag uninitialised.
}

HelpWnd::~HelpWnd()
{
    // RVA 0x55B8E0 - only the aux info and the ModalWnd base need unwinding.
}

m3d::Object* HelpWnd::Clone()
{
    // RVA 0x55B630
    return new HelpWnd(*this);
}

m3d::Object* HelpWnd::CreateObject()
{
    return new HelpWnd;
}

m3d::Class* HelpWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModalWnd);
}

m3d::Class* HelpWnd::GetClass() const
{
    return RT_CLASS_LOCAL(HelpWnd);
}

int HelpWnd::GameDataSetup()
{
    // RVA 0x55B900
    if ((m_gameDataFlags & 2) == 0)
    {
        int res = 1;

        m3d::Object* checkDisableAutoHelp = GetChildByName(m_aif.m_checkDisableAutoHelpName);
        if (checkDisableAutoHelp && checkDisableAutoHelp->IsKindOf(&m3d::ui::CheckWnd::m_classCheckWnd))
        {
            m_checkDisableAutoHelp = static_cast<m3d::ui::CheckWnd*>(checkDisableAutoHelp);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_checkDisableAutoHelpName +
                         " is not found or incorrect type");
            res = 0;
        }

        m3d::Object* emboss = GetChildByName(m_aif.m_wndCheckDisableAutoHelpEmbossName);
        if (emboss && emboss->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_wndCheckDisableAutoHelpEmboss = static_cast<m3d::ui::Wnd*>(emboss);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndCheckDisableAutoHelpEmbossName +
                         " is not found or incorrect type");
            res = 0;
        }

        m3d::Object* lblDisableAutoHelp = GetChildByName(m_aif.m_lblDisableAutoHelpName);
        if (lblDisableAutoHelp && lblDisableAutoHelp->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_lblDisableAutoHelp = static_cast<m3d::ui::Wnd*>(lblDisableAutoHelp);
            if (res)
            {
                m_gameDataFlags |= 1u;
            }
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_lblDisableAutoHelpName +
                         " is not found or incorrect type");
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }

    M3D_LOG_INFO("HelpWnd: error - fail to init because of a bad resource");
    return 0;
}

int HelpWnd::OnBeforeAddToWndStation()
{
    // RVA 0x55BD20
    CheckAndShowAutoHelpControls();
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

int HelpWnd::OnAfterRemoveFromWndStation()
{
    // RVA 0x55BD30 - the checkbox only counts while it is actually on screen;
    // a hidden one must not silently turn the auto help back on.
    int const res = m3d::ui::Wnd::OnAfterRemoveFromWndStation();
    if ((m_gameDataFlags & 1) != 0 && (m_checkDisableAutoHelp->GetStyle() & m3d::ui::WS_IS_VISIBLE) != 0)
    {
        M3D_APP->m_pInterfaceManager->GetHelpManager()->EnableAutoHelp(m_checkDisableAutoHelp->GetCheck() == 0);
    }
    return res;
}

void HelpWnd::DisableAutoHelp(bool bDisable)
{
    // RVA 0x55BCF0
    M3D_APP->m_pInterfaceManager->GetHelpManager()->EnableAutoHelp(!bDisable);
}

void HelpWnd::CheckAndShowAutoHelpControls()
{
    // RVA 0x55BD90 - the "do not show this again" trio is only offered while the
    // auto help is still on and this particular window allows switching it off.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    if (M3D_APP->m_pInterfaceManager->GetHelpManager()->IsAutoHelpEnabled() && m_bAllowDisableAutoHelp)
    {
        m_checkDisableAutoHelp->ShowWindow(true);
        m_wndCheckDisableAutoHelpEmboss->ShowWindow(true);
        m_lblDisableAutoHelp->ShowWindow(true);
        m_checkDisableAutoHelp->SetCheck(0);
    }
    else
    {
        m_checkDisableAutoHelp->ShowWindow(false);
        m_wndCheckDisableAutoHelpEmboss->ShowWindow(false);
        m_lblDisableAutoHelp->ShowWindow(false);
    }
}

void HelpWnd::AllowDisableAutoHelp(bool bAllow)
{
    // RVA 0x55BE40
    m_bAllowDisableAutoHelp = bAllow;
    if (IsChildOf(M3D_APP))
    {
        CheckAndShowAutoHelpControls();
    }
}
