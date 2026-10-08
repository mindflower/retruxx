#include "helpsimplemsgwnd.h"

#include <core/log.h>
#include <game/m3dgame.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimisc/helpmanager.h>
#include <m3dapp.h>

RT_CLASS_EXPORTS_BEGIN(HelpSimpleMsgWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(HelpSimpleMsgWnd);

HelpSimpleMsgWnd::AuxInfo::AuxInfo()
{
    // RVA 0x4B9540
    m_wndHelpTextName = "wndHelpText";
    m_wndTitleName = "wndTitle";
}

HelpSimpleMsgWnd::HelpSimpleMsgWnd()
{
    // RVA 0x4B95D0
    m_wndHelpText = nullptr;
    m_wndTitle = nullptr;
}

HelpSimpleMsgWnd::HelpSimpleMsgWnd(HelpSimpleMsgWnd const&)
{
    // RVA 0x4B96B0 - the shipped copy constructor copies nothing from rhs and,
    // unlike the default one, leaves the two child window pointers
    // uninitialised.
}

HelpSimpleMsgWnd::~HelpSimpleMsgWnd()
{
    // RVA 0x4B9700 - only the aux info and the HelpWnd base need unwinding.
}

m3d::Object* HelpSimpleMsgWnd::Clone()
{
    // RVA 0x4B94C0
    return new HelpSimpleMsgWnd(*this);
}

m3d::Object* HelpSimpleMsgWnd::CreateObject()
{
    return new HelpSimpleMsgWnd;
}

m3d::Class* HelpSimpleMsgWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(HelpWnd);
}

m3d::Class* HelpSimpleMsgWnd::GetClass() const
{
    return RT_CLASS_LOCAL(HelpSimpleMsgWnd);
}

int HelpSimpleMsgWnd::GameDataSetup()
{
    // RVA 0x4B9780
    if (HelpWnd::GameDataSetup() && (m_gameDataFlags & 2) == 0)
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

    M3D_LOG_INFO("HelpSimpleMsgWnd: error - fail to init because of a bad resource");
    return 0;
}

int HelpSimpleMsgWnd::GameDataClear(bool)
{
    // RVA 0x4B99A0
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndHelpText->SetText(CStr());
        m_wndTitle->SetText(CStr());
    }
    return 1;
}

void HelpSimpleMsgWnd::SetupForHelp(CStr const& helpId, bool bAllowDisableAutoHelp)
{
    // RVA 0x4B9A30
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    m_bAllowDisableAutoHelp = bAllowDisableAutoHelp;
    if (IsChildOf(M3D_APP))
    {
        CheckAndShowAutoHelpControls();
    }

    m_wndHelpText->SetText(M3D_APP->m_pInterfaceManager->GetHelpManager()->GetHelpMsgByHelpId(helpId));
    m_wndTitle->SetText(M3D_APP->m_pInterfaceManager->GetHelpManager()->GetHelpTitleByHelpId(helpId));
}
