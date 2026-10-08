#pragma once
#include "helpwnd.h"

class HelpSimpleMsgWnd : public HelpWnd
{
public:
    void SetupForHelp(CStr const& helpId, bool bAllowDisableAutoHelp);

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_wndHelpTextName;
        /* 0x000c */ CStr m_wndTitleName;
        AuxInfo(HelpSimpleMsgWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0018 */

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x00 */;

    /* 0x0258 */ m3d::ui::Wnd* m_wndHelpText;
    /* 0x025c */ m3d::ui::Wnd* m_wndTitle;
    /* 0x0260 */ HelpSimpleMsgWnd::AuxInfo m_aif;

    HelpSimpleMsgWnd();
    HelpSimpleMsgWnd(HelpSimpleMsgWnd const& rhs);

public:
    virtual ~HelpSimpleMsgWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    RT_CLASS_DECLARE(HelpSimpleMsgWnd);
}; /* size: 0x0278 */
