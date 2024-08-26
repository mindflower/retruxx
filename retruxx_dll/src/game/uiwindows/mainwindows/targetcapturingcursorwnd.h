#pragma once
#include "targetcursorwnd.h"

class TargetCapturingCursorWnd :  public TargetCursorWnd
{
public:
    virtual m3d::Class * GetClass() const ;
    virtual ~TargetCapturingCursorWnd();
    static CStr const & __fastcall GetCaptureTexName();
    static float __fastcall GetCapturingTexAngle();
    static m3d::Class * GetBaseClass();
    static m3d::Object * CreateObject();
    static PointBase<float> const & __fastcall GetCaptureTexSmallSz();
    virtual m3d::Object * Clone();

protected:
    virtual int GetTargetObjId() const ;
    TargetCapturingCursorWnd();
    TargetCapturingCursorWnd(TargetCapturingCursorWnd const &);
    void UpdateCaptureTexAngle();
    virtual void OnNewFrame();
    void UpdateCaptureTex();
    virtual int GameDataSetup();
    PointBase<float> GetCaptureTexSize() const ;
    unsigned char GetCaptureTexAlpha() const ;

public:
    RT_CLASS_DECLARE(TargetCapturingCursorWnd);

private:
    const float CAPTURE_ROTATION_SPEED = 2.3561945;
    m3d::rend::TexHandle m_texCapture;
};
