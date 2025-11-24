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
        /* 0x0000 */ CStr m_wndBossImageName;
        /* 0x000c */ CStr m_wndBossNameName;
        /* 0x0018 */ CStr m_pbBossHealthName;
        AuxInfo(const BossIndicatorWnd::AuxInfo&);
        AuxInfo();
    }; /* size: 0x0024 */

protected:
    virtual int GameDataSetup() override /* 0x104 */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int OnBeforeAddToWndStation() override /* 0x68 */;
    void OnNewFrame();
    void FullUpdate();
    void UpdateImage();
    void UpdateName();
    void UpdateHealth(bool bForce);
    void GetBossHealth(const ai::Obj* boss, float& maxHealth, float& curHealth) const;
    /* 0x0220 */ int m_bossId;
    /* 0x0224 */ m3d::ui::ImageWnd* m_wndBossImage;
    /* 0x0228 */ m3d::ui::Wnd* m_wndBossName;
    /* 0x022c */ m3d::ui::ProgressBarWnd* m_pbBossHealth;
    /* 0x0230 */ BossIndicatorWnd::AuxInfo m_aif;
    BossIndicatorWnd();
    BossIndicatorWnd(const BossIndicatorWnd& rhs);

public:
    virtual ~BossIndicatorWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classBossIndicatorWnd;
}; /* size: 0x0254 */
