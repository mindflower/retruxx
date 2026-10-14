#pragma once
#include "helpwnd.h"
#include <game/uimisc/helpmanager.h>

class HelpInMainGameInterfaceWnd : public HelpWnd
{
    struct AuxInfo
    {
        CStr m_wndHelpTextName;
        CStr m_wndTitleName;
        AuxInfo(HelpInMainGameInterfaceWnd::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override;
    virtual int OnBeforeAddToWndStation() override;
    HelpManager::HelpId GetHelpIdByCtrlId(int ctrlId) const;
    void ShowHelp(HelpManager::HelpId helpId);
    void HideHelp();

    m3d::ui::Wnd* m_wndHelpText;
    m3d::ui::Wnd* m_wndTitle;
    HelpInMainGameInterfaceWnd::AuxInfo m_aif;

    HelpInMainGameInterfaceWnd();
    HelpInMainGameInterfaceWnd(HelpInMainGameInterfaceWnd const& rhs);

public:
    virtual ~HelpInMainGameInterfaceWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(HelpInMainGameInterfaceWnd);
};
