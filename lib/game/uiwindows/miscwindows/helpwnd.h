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
        CStr m_checkDisableAutoHelpName;
        CStr m_wndCheckDisableAutoHelpEmbossName;
        CStr m_lblDisableAutoHelpName;
        AuxInfo(HelpWnd::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int OnBeforeAddToWndStation() override;
    virtual int OnAfterRemoveFromWndStation() override;
    void DisableAutoHelp(bool bDisable);
    void CheckAndShowAutoHelpControls();

    HelpWnd::AuxInfo m_aif;
    m3d::ui::CheckWnd* m_checkDisableAutoHelp;
    m3d::ui::Wnd* m_wndCheckDisableAutoHelpEmboss;
    m3d::ui::Wnd* m_lblDisableAutoHelp;
    bool m_bAllowDisableAutoHelp;

    HelpWnd();
    HelpWnd(HelpWnd const& rhs);

public:
    virtual ~HelpWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(HelpWnd);
};
