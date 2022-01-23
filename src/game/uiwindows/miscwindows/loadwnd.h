#pragma once
#include "lswnd.h"

class LoadWnd :  public LSWnd
{
public:
    class LAuxInfo
    {
    public:
        LAuxInfo();

    private:
        CStr m_btnLoadName;
    };

public:
    virtual m3d::Object * Clone();
    static m3d::Class * GetBaseClass();
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    virtual ~LoadWnd();

protected:
    virtual int GameDataUpdate(void *,int);
    virtual int GameDataSetup();
    virtual void OnEnter();
    LoadWnd(LoadWnd const &);
    LoadWnd();
    virtual void OnSaveSelectionChange();
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int, m3d::AIParam const &);
    void OnLoad();

public:
    RT_CLASS_DECLARE(LoadWnd);

private:
    m3d::ui::ButtonWnd *m_btnLoad;
    LoadWnd::LAuxInfo m_laif;
};
