#pragma once

class TargetCapturingCursorWnd :  public TargetCursorWnd
{
public:
    virtual struct m3d::Class * GetClass() const ;
    virtual ~TargetCapturingCursorWnd();
    static class CStr const & __fastcall GetCaptureTexName();
    static float __fastcall GetCapturingTexAngle();
    static struct m3d::Class * __fastcall GetBaseClass();
    static class m3d::Object * __fastcall CreateObject();
    static class PointBase<float> const & __fastcall GetCaptureTexSmallSz();
    virtual class m3d::Object * Clone();
protected:
    virtual int GetTargetObjId() const ;
    TargetCapturingCursorWnd();
    TargetCapturingCursorWnd(class TargetCapturingCursorWnd const &);
    void UpdateCaptureTexAngle();
    virtual void OnNewFrame();
    void UpdateCaptureTex();
    virtual int GameDataSetup();
    class PointBase<float> GetCaptureTexSize() const ;
    unsigned char GetCaptureTexAlpha() const ;
private:
    const float CAPTURE_ROTATION_SPEED;
    m3d::rend::TexHandle m_texCapture;
};
