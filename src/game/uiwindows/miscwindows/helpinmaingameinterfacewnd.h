#pragma once

class HelpInMainGameInterfaceWnd :  public HelpWnd
{
public:
    static struct m3d::Class * GetBaseClass();
    static class m3d::Object * CreateObject();
    virtual class m3d::Object * Clone();
    virtual struct m3d::Class * GetClass() const ;
    virtual ~HelpInMainGameInterfaceWnd();
protected:
    virtual int GameDataSetup();
    HelpInMainGameInterfaceWnd(class HelpInMainGameInterfaceWnd const &);
    HelpInMainGameInterfaceWnd();
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    void HideHelp();
    void ShowHelp(enum HelpManager::HelpId);
    virtual int OnBeforeAddToWndStation();
    enum HelpManager::HelpId GetHelpIdByCtrlId(int) const ;
private:
    CreateObject();
    GetClass();
    m3d::ui::Wnd *m_wndHelpText;
    m3d::ui::Wnd *m_wndTitle;
    HelpInMainGameInterfaceWnd::AuxInfo m_aif;
};
