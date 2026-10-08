#pragma once
#include "helpwnd.h"
#include <game/uimisc/helpmanager.h>

class HelpInMainGameInterfaceWnd : public HelpWnd
{
    struct AuxInfo
    {
        /* 0x0000 */ CStr m_wndHelpTextName;
        /* 0x000c */ CStr m_wndTitleName;
        AuxInfo(HelpInMainGameInterfaceWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0018 */

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override
        /* 0x00 */;
    virtual int OnBeforeAddToWndStation() override /* 0x00 */;
    HelpManager::HelpId GetHelpIdByCtrlId(int ctrlId) const;
    void ShowHelp(HelpManager::HelpId helpId);
    void HideHelp();

    /* 0x0258 */ m3d::ui::Wnd* m_wndHelpText;
    /* 0x025c */ m3d::ui::Wnd* m_wndTitle;
    /* 0x0260 */ HelpInMainGameInterfaceWnd::AuxInfo m_aif;

    HelpInMainGameInterfaceWnd();
    HelpInMainGameInterfaceWnd(HelpInMainGameInterfaceWnd const& rhs);

public:
    virtual ~HelpInMainGameInterfaceWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    RT_CLASS_DECLARE(HelpInMainGameInterfaceWnd);
}; /* size: 0x0278 */
