#pragma once

class DemoSplashWnd :  public m3d::ui::ModalWnd
{
public:
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * __fastcall CreateObject();
    virtual int GameDataSetup();
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    virtual class m3d::Object * Clone();
    virtual ~DemoSplashWnd();
    static struct m3d::Class * __fastcall GetBaseClass();
protected:
    DemoSplashWnd();
    DemoSplashWnd(class DemoSplashWnd const &);
    virtual int OnActivate(bool);
    virtual int OnPaint(struct m3d::ui::DrawInfo const &);
private:
    Clone();
    DemoSplashWnd::AuxInfo m_aif;
    m3d::ui::ButtonWnd *m_btnTargem;
};
