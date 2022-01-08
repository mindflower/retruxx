#pragma once

class HelpWnd :  public m3d::ui::ModalWnd
{
public:
    static class m3d::Object * CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    static struct m3d::Class * GetBaseClass();
    virtual class m3d::Object * Clone();
    virtual ~HelpWnd();
    void AllowDisableAutoHelp(bool);
protected:
    virtual int OnAfterRemoveFromWndStation();
    virtual int OnBeforeAddToWndStation();
    void DisableAutoHelp(bool);
    void CheckAndShowAutoHelpControls();
    HelpWnd();
    HelpWnd(class HelpWnd const &);
    virtual int GameDataSetup();
private:
    HelpWnd::AuxInfo m_aif;
    m3d::ui::CheckWnd *m_checkDisableAutoHelp;
    m3d::ui::Wnd *m_wndCheckDisableAutoHelpEmboss;
    m3d::ui::Wnd *m_lblDisableAutoHelp;
    bool m_bAllowDisableAutoHelp;
};
