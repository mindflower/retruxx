#pragma once

class TargetInfoWnd :  public m3d::ui::Wnd
{
public:
    static class m3d::Object * CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    static bool __fastcall IsObjClassValidForInfo(struct m3d::Class const *);
    virtual class m3d::Object * Clone();
    int GetTargetObjId() const ;
    bool NeedShow() const ;
    virtual ~TargetInfoWnd();
    static struct m3d::Class * GetBaseClass();
protected:
    void UpdateName();
    void UpdateControlsOnNewFrame();
    virtual int OnBeforeRemoveFromWndStation();
    virtual int GameDataSetup();
    virtual int GameDataUpdate(void *,int);
    TargetInfoWnd();
    TargetInfoWnd(class TargetInfoWnd const &);
    void StopFade();
    void SetTargetObj(int);
    void UpdateToleranceColor();
    void UpdateDurability();
    void UpdateOnChangeTargetObj(int,int);
    virtual int OnAfterRemoveFromWndStation();
    virtual int OnBeforeAddToWndStation();
    void StartFade();
    class ai::Obj const * GetTargetObj() const ;
    void OnNewFrameForce();
    unsigned char CalcAlpha() const ;
    bool NeedUpdate() const ;
    void UpdateResistance();
    void CheckAndShow();
    bool IsFading() const ;
    void ProcessFade();
    void RestoreFromFade();
    void SetAlpha(unsigned char);
    void UpdateHealth();
    void UpdateDistance();
    void UpdateTargetObj();
private:
    GetClass();
    m3d::ui::ProgressBarWnd *m_pbHealth;
    m3d::ui::Wnd *m_lblHealth;
    m3d::ui::ProgressBarWnd *m_pbDurability;
    m3d::ui::Wnd *m_lblDurability;
    m3d::ui::Wnd *m_wndDistance;
    m3d::ui::Wnd *m_wndResistance[3];
    m3d::ui::Wnd *m_wndName;
    TargetInfoWnd::AuxInfo m_aif;
    int m_targetObjId;
    unsigned int m_fadeStartTime;
};
