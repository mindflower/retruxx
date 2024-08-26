#pragma once
#include "targetcursorwnd.h"

class TargetCapturedCursorWnd :  public TargetCursorWnd
{
public:
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    static m3d::Class * GetBaseClass();
    virtual m3d::Object * Clone();
    virtual ~TargetCapturedCursorWnd();

protected:
    virtual void OnNewFrame();
    TargetCapturedCursorWnd(TargetCapturedCursorWnd const &);
    TargetCapturedCursorWnd();
    bool bVisible() const ;
    virtual int GetTargetObjId() const ;
    void UpdateTexAngle();
    void UpdateCaptureTex();
    virtual void Show(bool);
    virtual int GameDataSetup();

public:
    RT_CLASS_DECLARE(TargetCapturedCursorWnd);

private:
    m3d::rend::TexHandle m_texCapture;
    float m_texAngle;
};
