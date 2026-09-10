#pragma once
#include <ui/ui.h>

namespace ai
{
    class Vehicle;
}

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
    }
}

class SkinSwitcher
{
    enum Skin
    {
        SKIN_COMMON_MIN = 0x0,
        SKIN_COMMON_0 = 0x0,
        SKIN_COMMON_1 = 0x1,
        SKIN_COMMON_2 = 0x2,
        SKIN_COMMON_MAX = 0x2,
        SKIN_DEATH = 0x8,
    };

public:
    SkinSwitcher();
    ~SkinSwitcher();
    int SetUp(int vehicleId, m3d::ui::ButtonWnd* btnNext, m3d::ui::ButtonWnd* btnPrev);
    void Reset();
    void ShowNextSkin();
    void ShowPrevSkin();
    int GetCurSkin() const;
    static int __fastcall GetMaxSkinCount(ai::Vehicle* vehicle);
    static bool __fastcall IsSkinApplicable(int skin, ai::Vehicle* vehicle);
    static int __fastcall ApplySkin(int skin, ai::Vehicle* vehicle);
    static int __fastcall GetVehicleSkin(ai::Vehicle* vehicle);

private:
    void UpdateSkinButtonsStates();
    int SetCurSkin(int skin);
    int GetNextSkin() const;
    int GetPrevSkin() const;
    ai::Vehicle* GetVehicle() const;
    bool IsValid() const;

    /* 0x0000 */ int m_vehicleId;
    /* 0x0004 */ m3d::ui::ButtonWnd* m_btnSkinNext;
    /* 0x0008 */ m3d::ui::ButtonWnd* m_btnSkinPrev;
    /* 0x000c */ int m_curSkin;
}; /* size: 0x0010 */

class SkinsWnd : public m3d::ui::Wnd
{
public:
    int SetupForVehicle(int vehicleId);

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_btnPrevName;
        /* 0x000c */ CStr m_btnNextName;
        /* 0x0018 */ CStr m_btnBuyName;
        /* 0x0024 */ CStr m_wndPriceName;
        AuxInfo(SkinsWnd::AuxInfo const& rhs);
        AuxInfo();
    }; /* size: 0x0030 */

protected:
    virtual int GameDataSetup() override /* 0x104 */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override
        /* 0xc8 */;
    virtual int OnBeforeAddToWndStation() override /* 0x68 */;
    virtual int OnAfterRemoveFromWndStation() override /* 0x74 */;
    void BuySkin();
    int GetSkinPrice(int vehicleId, int skinId) const;
    ai::Vehicle* GetVehicle() const;
    void FullUpdate();
    void ShowNextSkin();
    void ShowPrevSkin();
    void OnChangeSkin();

    /* 0x0220 */ SkinSwitcher* m_skinSwitcher;
    /* 0x0224 */ m3d::ui::ButtonWnd* m_btnPrev;
    /* 0x0228 */ m3d::ui::ButtonWnd* m_btnNext;
    /* 0x022c */ m3d::ui::ButtonWnd* m_btnBuy;
    /* 0x0230 */ m3d::ui::Wnd* m_wndPrice;
    /* 0x0234 */ SkinsWnd::AuxInfo m_aif;
    /* 0x0264 */ int m_vehicleId;
    /* 0x0268 */ int m_savedSkinId;

    SkinsWnd();
    SkinsWnd(SkinsWnd const& rhs);

public:
    virtual ~SkinsWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(SkinsWnd);
}; /* size: 0x026c */
