#pragma once
#include <game/uiwindows/charwindows/childpanel.h>

class ScreenWnd : public ChildPanel
{
protected:
    virtual int AddChild(m3d::Object* w) override /* 0x00 */;
    virtual int GameDataSetup() override /* 0x00 */;
    ScreenWnd();
    ScreenWnd(ScreenWnd const& rhs);

public:
    virtual ~ScreenWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    RT_CLASS_DECLARE(ScreenWnd);
}; /* size: 0x0224 */
