#pragma once
#include "helpwnd.h"

class HelpSimpleMsgWnd :  public HelpWnd
{
public:
    void SetupForHelp(CStr const &,bool);
    virtual m3d::Object * Clone();
    static m3d::Class * GetBaseClass();
    static m3d::Object * CreateObject();
    virtual ~HelpSimpleMsgWnd();
    virtual m3d::Class * GetClass() const ;

protected:
    HelpSimpleMsgWnd();
    HelpSimpleMsgWnd(HelpSimpleMsgWnd const &);
    virtual int GameDataSetup();
    virtual int GameDataClear(bool);

public:
    RT_CLASS_DECLARE(HelpSimpleMsgWnd);

private:
    m3d::ui::Wnd *m_wndHelpText;
    m3d::ui::Wnd *m_wndTitle;
    HelpSimpleMsgWnd::AuxInfo m_aif;
};
