#pragma once

class HelpSimpleMsgWnd :  public HelpWnd
{
public:
    void SetupForHelp(CStr const &,bool);
    virtual class m3d::Object * Clone();
    static struct m3d::Class * __fastcall GetBaseClass();
    static class m3d::Object * __fastcall CreateObject();
    virtual ~HelpSimpleMsgWnd();
    virtual struct m3d::Class * GetClass() const ;
protected:
    HelpSimpleMsgWnd();
    HelpSimpleMsgWnd(class HelpSimpleMsgWnd const &);
    virtual int GameDataSetup();
    virtual int GameDataClear(bool);
private:
    m3d::ui::Wnd *m_wndHelpText;
    m3d::ui::Wnd *m_wndTitle;
    HelpSimpleMsgWnd::AuxInfo m_aif;
};
