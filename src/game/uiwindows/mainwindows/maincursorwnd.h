#pragma once

class MainCursorWnd :  public SmartCursorWnd
{
public:
    bool NeedShow() const ;
    static struct m3d::Class * __fastcall GetBaseClass();
    static class m3d::Object * __fastcall CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    virtual ~MainCursorWnd();
    virtual class m3d::Object * Clone();
protected:
    void OnShowCursor(void *);
    void CheckAndShow();
    void FillCapturingDrawInfo(struct SmartCursorWnd::AuxDrawInfo *,int,bool) const ;
    void OnGameModeChanged(void *);
    void UpdateCapturingTex();
    virtual void OnNewFrame();
    virtual int GameDataSetup();
    virtual int GameDataUpdate(void *,int);
    MainCursorWnd();
    MainCursorWnd(class MainCursorWnd const &);
private:
    MainCursorWnd::AuxInfo m_aif;
    m3d::rend::TexHandle m_texCapturing;
};
