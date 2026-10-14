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
    virtual ~DemoSplashWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(DemoSplashWnd);
    virtual int GameDataSetup() override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override;

protected:
    virtual int OnPaint(m3d::ui::DrawInfo const& di) override;
    virtual int OnActivate(bool on) override;

    struct AuxInfo
    {
        CStr m_btnTargemName;
        AuxInfo(DemoSplashWnd::AuxInfo const&);
        AuxInfo();
    };

private:
    DemoSplashWnd::AuxInfo m_aif;
    m3d::ui::ButtonWnd* m_btnTargem;
};
