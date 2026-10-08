#pragma once
#include "targetcursorwnd.h"

class TargetCapturingCursorWnd :  public TargetCursorWnd
{
public:
    // NOTE: not present in the header this class was extracted from, but
    // referenced from TargetCapturingCursorWnd's own methods as well as from
    // TargetCapturedCursorWnd (which shares the same capture texture/sizes).
    struct AuxInfo
    {
        AuxInfo();
        /* 0x0000 */ CStr m_texName;
        /* 0x000c */ PointBase<float> m_texCaptureSzBig;
        /* 0x0014 */ PointBase<float> m_texCaptureSzSmall;
    }; /* size: 0x001c */

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
    static inline TargetCapturingCursorWnd::AuxInfo m_aif;
    // NOTE: shared, running rotation angle for the capture-in-progress ring;
    // accumulates unboundedly frame over frame (a shipped quirk - it is only
    // ever used through sin/cos-style consumers that don't care about wrap).
    static inline float m_captureTexAngle = 0.0f;
};
