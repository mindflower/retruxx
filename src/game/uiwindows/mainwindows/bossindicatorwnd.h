#pragma once
#include <ui/ui.h>

namespace ai
{
    class Obj;
}

namespace m3d
{
    namespace ui
    {
        class ProgressBarWnd;
        class ImageWnd;
    }
}

class BossIndicatorWnd :  public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_wndBossImageName;
        CStr m_wndBossNameName;
        CStr m_pbBossHealthName;
    };

public:
    virtual ~BossIndicatorWnd();
    virtual m3d::Class * GetClass() const ;
    ai::Obj const * GetBoss() const ;
    int SetupForBoss(int);
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
    int GetBossId() const ;
    virtual m3d::Object * Clone();

protected:
    void UpdateImage();
    BossIndicatorWnd(BossIndicatorWnd const &);
    BossIndicatorWnd();
    void FullUpdate();
    void OnNewFrame();
    virtual int GameDataUpdate(void *,int);
    void GetBossHealth(ai::Obj const *,float &,float &) const ;
    virtual int GameDataClear(bool);
    virtual int GameDataSetup();
    virtual int OnBeforeAddToWndStation();
    void UpdateHealth(bool);
    void UpdateName();

public:
    RT_CLASS_DECLARE(BossIndicatorWnd);

private:
    int m_bossId;
    m3d::ui::ImageWnd *m_wndBossImage;
    m3d::ui::Wnd *m_wndBossName;
    m3d::ui::ProgressBarWnd *m_pbBossHealth;
    BossIndicatorWnd::AuxInfo m_aif;
};
