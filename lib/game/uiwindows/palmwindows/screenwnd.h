#pragma once
#include <game/uiwindows/charwindows/childpanel.h>

class ScreenWnd : public ChildPanel
{
protected:
    virtual int AddChild(m3d::Object* w) override;
    virtual int GameDataSetup() override;
    ScreenWnd();
    ScreenWnd(ScreenWnd const& rhs);

public:
    virtual ~ScreenWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(ScreenWnd);
};
