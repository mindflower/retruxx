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
}  // namespace m3d

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
    static int GetMaxSkinCount(ai::Vehicle* vehicle);
    static bool IsSkinApplicable(int skin, ai::Vehicle* vehicle);
    static int ApplySkin(int skin, ai::Vehicle* vehicle);
    static int GetVehicleSkin(ai::Vehicle* vehicle);

private:
    void UpdateSkinButtonsStates();
    int SetCurSkin(int skin);
    int GetNextSkin() const;
    int GetPrevSkin() const;
    ai::Vehicle* GetVehicle() const;
    bool IsValid() const;

    int m_vehicleId;
    m3d::ui::ButtonWnd* m_btnSkinNext;
    m3d::ui::ButtonWnd* m_btnSkinPrev;
    int m_curSkin;
};

class SkinsWnd : public m3d::ui::Wnd
{
public:
    int SetupForVehicle(int vehicleId);

    struct AuxInfo
    {
        CStr m_btnPrevName;
        CStr m_btnNextName;
        CStr m_btnBuyName;
        CStr m_wndPriceName;
        AuxInfo(SkinsWnd::AuxInfo const& rhs);
        AuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override;
    virtual int OnBeforeAddToWndStation() override;
    virtual int OnAfterRemoveFromWndStation() override;
    void BuySkin();
    int GetSkinPrice(int vehicleId, int skinId) const;
    ai::Vehicle* GetVehicle() const;
    void FullUpdate();
    void ShowNextSkin();
    void ShowPrevSkin();
    void OnChangeSkin();

    SkinSwitcher* m_skinSwitcher;
    m3d::ui::ButtonWnd* m_btnPrev;
    m3d::ui::ButtonWnd* m_btnNext;
    m3d::ui::ButtonWnd* m_btnBuy;
    m3d::ui::Wnd* m_wndPrice;
    SkinsWnd::AuxInfo m_aif;
    int m_vehicleId;
    int m_savedSkinId;

    SkinsWnd();
    SkinsWnd(SkinsWnd const& rhs);

public:
    virtual ~SkinsWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(SkinsWnd);
};
