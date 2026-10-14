#pragma once
#include "helpwnd.h"

class HelpSimpleMsgWnd : public HelpWnd
{
public:
    void SetupForHelp(CStr const& helpId, bool bAllowDisableAutoHelp);

    struct AuxInfo
    {
        CStr m_wndHelpTextName;
        CStr m_wndTitleName;
        AuxInfo(HelpSimpleMsgWnd::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;

    m3d::ui::Wnd* m_wndHelpText;
    m3d::ui::Wnd* m_wndTitle;
    HelpSimpleMsgWnd::AuxInfo m_aif;

    HelpSimpleMsgWnd();
    HelpSimpleMsgWnd(HelpSimpleMsgWnd const& rhs);

public:
    virtual ~HelpSimpleMsgWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(HelpSimpleMsgWnd);
};
