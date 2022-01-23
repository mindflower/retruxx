#pragma once

class DemoSplashWnd : public m3d::ui::ModalWnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_btnTargemName;
    };

public:
    virtual m3d::Class* GetClass() const;
    static m3d::Object* CreateObject();
    virtual int GameDataSetup();
    virtual int OnWndNotify(m3d::ui::Wnd*, unsigned int, unsigned int, m3d::AIParam const&);
    virtual m3d::Object* Clone();
    virtual ~DemoSplashWnd();
    static m3d::Class* GetBaseClass();

protected:
    DemoSplashWnd();
    DemoSplashWnd(DemoSplashWnd const&);
    virtual int OnActivate(bool);
    virtual int OnPaint(m3d::ui::DrawInfo const&);

public:
    RT_CLASS_DECLARE(DemoSplashWnd);

private:
    DemoSplashWnd::AuxInfo m_aif;
    m3d::ui::ButtonWnd* m_btnTargem;
};
