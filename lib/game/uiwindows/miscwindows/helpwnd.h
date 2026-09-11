#pragma once
#include <ui/wnd.h>

namespace m3d
{
    namespace ui
    {
        class CheckWnd;
    }
}

class HelpWnd : public m3d::ui::ModalWnd
{
public:
    void AllowDisableAutoHelp(bool bAllow);

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_checkDisableAutoHelpName;
        /* 0x000c */ CStr m_wndCheckDisableAutoHelpEmbossName;
        /* 0x0018 */ CStr m_lblDisableAutoHelpName;
        AuxInfo(HelpWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0024 */

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int OnBeforeAddToWndStation() override /* 0x00 */;
    virtual int OnAfterRemoveFromWndStation() override /* 0x00 */;
    void DisableAutoHelp(bool bDisable);
    void CheckAndShowAutoHelpControls();

    /* 0x0224 */ HelpWnd::AuxInfo m_aif;
    /* 0x0248 */ m3d::ui::CheckWnd* m_checkDisableAutoHelp;
    /* 0x024c */ m3d::ui::Wnd* m_wndCheckDisableAutoHelpEmboss;
    /* 0x0250 */ m3d::ui::Wnd* m_lblDisableAutoHelp;
    /* 0x0254 */ bool m_bAllowDisableAutoHelp;

    HelpWnd();
    HelpWnd(HelpWnd const& rhs);

public:
    virtual ~HelpWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    RT_CLASS_DECLARE(HelpWnd);
}; /* size: 0x0258 */
