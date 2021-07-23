#pragma once

class ScreenWnd :  public ChildPanel
{
public:
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * __fastcall CreateObject();
    virtual class m3d::Object * Clone();
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual ~ScreenWnd();
protected:
    ScreenWnd();
    ScreenWnd(class ScreenWnd const &);
    virtual int GameDataSetup();
    virtual int AddChild(class m3d::Object *);
private:
};
