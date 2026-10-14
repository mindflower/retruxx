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


class BossIndicatorWnd : public m3d::ui::Wnd
{
public:
    int SetupForBoss(int bossId);
    int GetBossId() const;
    const ai::Obj* GetBoss() const;

    struct AuxInfo
    {
        CStr m_wndBossImageName;
        CStr m_wndBossNameName;
        CStr m_pbBossHealthName;
        AuxInfo(const BossIndicatorWnd::AuxInfo&);
        AuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int OnBeforeAddToWndStation() override;
    void OnNewFrame();
    void FullUpdate();
    void UpdateImage();
    void UpdateName();
    void UpdateHealth(bool bForce);
    void GetBossHealth(const ai::Obj* boss, float& maxHealth, float& curHealth) const;
    int m_bossId;
    m3d::ui::ImageWnd* m_wndBossImage;
    m3d::ui::Wnd* m_wndBossName;
    m3d::ui::ProgressBarWnd* m_pbBossHealth;
    BossIndicatorWnd::AuxInfo m_aif;
    BossIndicatorWnd();
    BossIndicatorWnd(const BossIndicatorWnd& rhs);

public:
    virtual ~BossIndicatorWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classBossIndicatorWnd;
};
