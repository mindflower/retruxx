#pragma once
#include <game/uiwindows/charwindows/childpanel.h>

class ScreenWnd : public ChildPanel
{
public:
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    virtual m3d::Object * Clone();
    static m3d::Class * GetBaseClass();
    virtual ~ScreenWnd();

protected:
    ScreenWnd();
    ScreenWnd(ScreenWnd const &);
    virtual int GameDataSetup();
    virtual int AddChild(m3d::Object *);

public:
    RT_CLASS_DECLARE(ScreenWnd);
};
