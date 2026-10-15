#pragma once
#include "lswnd.h"

class LoadWnd : public LSWnd
{
public:
    struct LAuxInfo
    {
        CStr m_btnLoadName;
        LAuxInfo(LoadWnd::LAuxInfo const&);
        LAuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override;
    virtual void OnSaveSelectionChange() override;
    virtual void OnEnter() override;
    void OnLoad();

    m3d::ui::ButtonWnd* m_btnLoad;
    LoadWnd::LAuxInfo m_laif;

    LoadWnd();
    LoadWnd(LoadWnd const& rhs);

public:
    virtual ~LoadWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(LoadWnd);
};
