#pragma once

class BossIndicatorWnd :  public m3d::ui::Wnd
{
public:
    virtual ~BossIndicatorWnd();
    virtual struct m3d::Class * GetClass() const ;
    class ai::Obj const * GetBoss() const ;
    int SetupForBoss(int);
    static class m3d::Object * CreateObject();
    static struct m3d::Class * GetBaseClass();
    int GetBossId() const ;
    virtual class m3d::Object * Clone();
protected:
    void UpdateImage();
    BossIndicatorWnd(class BossIndicatorWnd const &);
    BossIndicatorWnd();
    void FullUpdate();
    void OnNewFrame();
    virtual int GameDataUpdate(void *,int);
    void GetBossHealth(class ai::Obj const *,float &,float &) const ;
    virtual int GameDataClear(bool);
    virtual int GameDataSetup();
    virtual int OnBeforeAddToWndStation();
    void UpdateHealth(bool);
    void UpdateName();
private:
    int m_bossId;
    m3d::ui::ImageWnd *m_wndBossImage;
    m3d::ui::Wnd *m_wndBossName;
    m3d::ui::ProgressBarWnd *m_pbBossHealth;
    BossIndicatorWnd::AuxInfo m_aif;
};
