#pragma once
#include "helpwnd.h"
#include <game/uimisc/helpmanager.h>

class HelpInMainGameInterfaceWnd :  public HelpWnd
{
public:
    static m3d::Class * GetBaseClass();
    static m3d::Object * CreateObject();
    virtual m3d::Object * Clone();
    virtual m3d::Class * GetClass() const ;
    virtual ~HelpInMainGameInterfaceWnd();

protected:
    virtual int GameDataSetup();
    HelpInMainGameInterfaceWnd(HelpInMainGameInterfaceWnd const &);
    HelpInMainGameInterfaceWnd();
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int, m3d::AIParam const &);
    void HideHelp();
    void ShowHelp(HelpManager::HelpId);
    virtual int OnBeforeAddToWndStation();
    HelpManager::HelpId GetHelpIdByCtrlId(int) const ;

public:
    RT_CLASS_DECLARE(HelpInMainGameInterfaceWnd);

private:
    m3d::ui::Wnd *m_wndHelpText;
    m3d::ui::Wnd *m_wndTitle;
    HelpInMainGameInterfaceWnd::AuxInfo m_aif;
};
