#pragma once
#include "smartcursorwnd.h"

class MainCursorWnd :  public SmartCursorWnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_texCapturingName;
        PointBase<float> m_texCapturingSz;
        float m_capturingRadius;
        int m_numCapturingSectors;
    };

public:
    bool NeedShow() const ;
    static m3d::Class * GetBaseClass();
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    virtual ~MainCursorWnd();
    virtual m3d::Object * Clone();

protected:
    void OnShowCursor(void *);
    void CheckAndShow();
    void FillCapturingDrawInfo(SmartCursorWnd::AuxDrawInfo *,int,bool) const ;
    void OnGameModeChanged(void *);
    void UpdateCapturingTex();
    virtual void OnNewFrame();
    virtual int GameDataSetup();
    virtual int GameDataUpdate(void *,int);
    MainCursorWnd();
    MainCursorWnd(MainCursorWnd const &);

public:
    RT_CLASS_DECLARE(MainCursorWnd);

private:
    MainCursorWnd::AuxInfo m_aif;
    m3d::rend::TexHandle m_texCapturing;
};
