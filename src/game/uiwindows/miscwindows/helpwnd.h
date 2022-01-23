#pragma once
#include <ui/wnd.h>

namespace m3d
{
    namespace ui
    {
        class CheckWnd;
    }
}

class HelpWnd :  public m3d::ui::ModalWnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_checkDisableAutoHelpName;
        CStr m_wndCheckDisableAutoHelpEmbossName;
        CStr m_lblDisableAutoHelpName;
    };

public:
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    static m3d::Class * GetBaseClass();
    virtual m3d::Object * Clone();
    virtual ~HelpWnd();
    void AllowDisableAutoHelp(bool);

protected:
    virtual int OnAfterRemoveFromWndStation();
    virtual int OnBeforeAddToWndStation();
    void DisableAutoHelp(bool);
    void CheckAndShowAutoHelpControls();
    HelpWnd();
    HelpWnd(HelpWnd const &);
    virtual int GameDataSetup();

public:
    RT_CLASS_DECLARE(HelpWnd);

private:
    HelpWnd::AuxInfo m_aif;
    m3d::ui::CheckWnd *m_checkDisableAutoHelp;
    m3d::ui::Wnd *m_wndCheckDisableAutoHelpEmboss;
    m3d::ui::Wnd *m_lblDisableAutoHelp;
    bool m_bAllowDisableAutoHelp;
};
