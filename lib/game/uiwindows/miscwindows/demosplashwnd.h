#pragma once
#include <ui/wnd.h>

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
    }
}  // namespace m3d

class DemoSplashWnd : public m3d::ui::ModalWnd
{
protected:
    DemoSplashWnd();
    DemoSplashWnd(DemoSplashWnd const& rhs);

public:
    virtual ~DemoSplashWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    RT_CLASS_DECLARE(DemoSplashWnd);
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override /* 0x00 */;

protected:
    virtual int OnPaint(m3d::ui::DrawInfo const& di) override /* 0x00 */;
    virtual int OnActivate(bool on) override /* 0x00 */;

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_btnTargemName;
        AuxInfo(DemoSplashWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x000c */

private:
    /* 0x0224 */ DemoSplashWnd::AuxInfo m_aif;
    /* 0x0230 */ m3d::ui::ButtonWnd* m_btnTargem;
}; /* size: 0x0234 */
