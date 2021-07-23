#pragma once

class LoadWnd :  public LSWnd
{
public:
    virtual class m3d::Object * Clone();
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * __fastcall CreateObject();
    virtual ~LoadWnd();
protected:
    virtual int GameDataUpdate(void *,int);
    virtual int GameDataSetup();
    virtual void OnEnter();
    LoadWnd(class LoadWnd const &);
    LoadWnd();
    virtual void OnSaveSelectionChange();
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    void OnLoad();
private:
    m3d::ui::ButtonWnd *m_btnLoad;
    LoadWnd::LAuxInfo m_laif;
};
