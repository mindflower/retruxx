#pragma once

class TargetCapturedCursorWnd :  public TargetCursorWnd
{
public:
    static class m3d::Object * CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    static struct m3d::Class * GetBaseClass();
    virtual class m3d::Object * Clone();
    virtual ~TargetCapturedCursorWnd();
protected:
    virtual void OnNewFrame();
    TargetCapturedCursorWnd(class TargetCapturedCursorWnd const &);
    TargetCapturedCursorWnd();
    bool bVisible() const ;
    virtual int GetTargetObjId() const ;
    void UpdateTexAngle();
    void UpdateCaptureTex();
    virtual void Show(bool);
    virtual int GameDataSetup();
private:
    m3d::rend::TexHandle m_texCapture;
    float m_texAngle;
};
