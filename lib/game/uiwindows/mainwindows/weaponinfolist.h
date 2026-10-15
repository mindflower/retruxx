#pragma once
#include <core/ref_ptr.h>
#include <server/damageinfo.h>
#include <ui/ui.h>
#include <ui/wnd.h>

namespace ai
{
    class Obj;
    class Vehicle;
}  // namespace ai

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
        class ImageWnd;
        class ProgressBarWnd;
    }  // namespace ui
}  // namespace m3d

class WeaponInfoList : public m3d::ui::Wnd
{
public:
    enum Type
    {
        TYPE_IN_MAIN_INTERFACE = 0,
        TYPE_IN_CHARACTERISTIC_WND = 1,
        TYPE_NUM_TYPES = 2,
    };

    class AuxInfo
    {
    public:
        AuxInfo();

    public:
        float m_smallSpace;
        float m_bigSpace;
        float m_bottom;
        float m_groupInMainUIW;
        float m_groupInCharWndW;
        float m_groupInFignyaWndW;
        float m_groupInCharWndSpace;
        CStr m_groupTexInMainUi;
        CStr m_groupTexInCharWnd;
        CStr m_groupTexInFignyaWnd;
        CStr m_wndDecorTopName;
        CStr m_wndDecorBottomName;
    };

public:
    static m3d::Class* GetBaseClass();
    static m3d::Object* CreateObject();
    int CreateFromPattern(m3d::ui::Wnd*, bool);
    virtual m3d::Object* Clone();
    virtual m3d::Class* GetClass() const;
    virtual ~WeaponInfoList();

protected:
    void UpdateOnKeyBindingsChanged();
    int RecalcLayotInMainInterface();
    virtual int GameDataSetup();
    virtual int OnWndNotify(m3d::ui::Wnd*, unsigned int, unsigned int, m3d::AIParam const&);
    WeaponInfoList();
    WeaponInfoList(WeaponInfoList const&);
    void GetItemsByGroup(int, std::vector<ref_ptr<class WeaponInfoWnd>>&) const;
    ai::Vehicle* GetVehicle() const;
    int GetNumOfGroups() const;
    int CreateItems();
    int RecalcLayot();
    virtual int GameDataUpdate(void*, int);
    int AddGroupButton(int, BoundsBase<float> const&, WeaponInfoWnd const*, int);
    bool CanGunShotToTarget(ai::Obj const*) const;
    int UpdateOnVehiclePartChanged(void*);
    void UpdateOnWeaponGroupChanged();
    m3d::rend::TexHandle GetGroupTextureForWeaponBtn(WeaponInfoWnd const*, int);
    int RecalcLayotInCharacteristicWnd();
    int GetGroupByVehiclePartName(CStr const&) const;
    virtual int OnBeforeAddToWndStation();
    int UpdateOnPlayerVehicleChanged();
    int AddItem(int, int);
    void InitGroupTextures();
    void ReleaseGroupTextures();
    void SetTooltipForGroupButton(m3d::ui::Wnd*);
    virtual int GameDataClear(bool);

public:
    RT_CLASS_DECLARE(WeaponInfoList);

private:
    std::vector<ref_ptr<WeaponInfoWnd>> m_items;
    std::vector<m3d::ui::Wnd*> m_groupButtons;
    int m_vehicleId;
    WeaponInfoList::AuxInfo m_aif;
    WeaponInfoList::Type m_type;
    std::vector<std::pair<m3d::rend::TexHandle, m3d::rend::TexHandle>> m_groupTextures;
    m3d::ui::ImageWnd* m_wndDecorTop;
    m3d::ui::ImageWnd* m_wndDecorBottom;
};

class WeaponInfoWnd : public m3d::ui::Wnd
{
    friend class WeaponInfoList;

public:
    int SetupForGun(int gunId, int groupId, WeaponInfoList::Type type);
    ai::Obj const* GetGun() const;
    int GetGroupId() const;

    struct AuxInfo
    {
        CStr m_wndPatternName;
        CStr m_wndPatternIcoName;
        CStr m_wndPatternChargeName;
        CStr m_wndPatternNameName;
        CStr m_wndPatternAmmoName;
        CStr m_wndPatternFrame0Name;
        CStr m_wndPatternFrame1Name;
        CStr m_wndPatternBgName;
        CStr m_wndPatternCanShotBgName;
        CStr m_wndPatternGroupName;
        float m_height;
        CStr m_paneName;
        float m_space;
        float m_chargeBarW;
        CStr m_chargeBarTexName;
        float m_lblReloadsW;
        CStr m_weaponTypeTexNames[13];
        AuxInfo(WeaponInfoWnd::AuxInfo const&);
        AuxInfo();
    };

    struct Pattern
    {
        m3d::ui::Wnd* m_wndPattern;
        m3d::ui::ImageWnd* m_wndPatternIco;
        m3d::ui::Wnd* m_wndPatternName;
        m3d::ui::ProgressBarWnd* m_wndPatternCharge;
        m3d::ui::Wnd* m_wndPatternAmmo;
        m3d::ui::Wnd* m_wndPatternFrame0;
        m3d::ui::Wnd* m_wndPatternFrame1;
        m3d::ui::ImageWnd* m_wndPatternBg;
        m3d::ui::ImageWnd* m_wndPatternCanShotBg;
        m3d::ui::Wnd* m_wndPatternGroup;
        Pattern();
    };

protected:
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    void UpdateOnNewFrame();
    void UpdateCharge();
    void UpdateAmmo();
    void UpdateIco();
    void UpdateCanShotBg();
    ai::DamageType GetWeaponTypeForGun(ai::Obj const* gun) const;
    int CreateFromPattern(WeaponInfoList::Type type);
    int CreateChildren(WeaponInfoList::Type type);
    static int LoadPattern(m3d::ui::Wnd* pattern, WeaponInfoList::Type type);
    static void ClearPattern(WeaponInfoList::Type type);
    m3d::rend::TexHandle GetIco() const;
    m3d::rend::TexHandle GetBarTexture(bool bReady) const;
    bool IsGunReady() const;
    int GetAmmo() const;
    int m_gunId;
    m3d::ui::ProgressBarWnd* m_wndCharge;
    m3d::ui::ImageWnd* m_wndIco;
    m3d::ui::Wnd* m_wndName;
    m3d::ui::Wnd* m_wndAmmo;
    m3d::ui::Wnd* m_wndFrame0;
    m3d::ui::Wnd* m_wndFrame1;
    m3d::ui::ImageWnd* m_wndBg;
    m3d::ui::ImageWnd* m_wndCanShotBg;
    m3d::ui::Wnd* m_wndGroup;
    static WeaponInfoWnd::AuxInfo m_aif;
    static WeaponInfoWnd::Pattern m_patterns[2];
    int m_groupId;
    WeaponInfoList::Type m_type;
    static m3d::rend::TexHandle m_pbChargeTextureReady;
    static m3d::rend::TexHandle m_pbChargeTextureRecharging;
    WeaponInfoWnd();
    WeaponInfoWnd(WeaponInfoWnd const& rhs);

public:
    virtual ~WeaponInfoWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classWeaponInfoWnd;
};

class WeaponGroupChoiceDlg : public m3d::ui::ModalWnd
{
public:
    static CStr CreateTooltipForWeaponGroup(int groupId);
    using ButtonVector = std::vector<m3d::ui::ButtonWnd*, std::allocator<m3d::ui::ButtonWnd*>>;

    struct AuxInfo
    {
        CStr m_groupButtonName;
        unsigned int m_selTextColor;
        AuxInfo(WeaponGroupChoiceDlg::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override;
    virtual int OnKey(unsigned short key, unsigned char scanCode, unsigned int state) override;
    void OnKeyBindingsChanged();
    void SelectButton(int id);
    std::vector<m3d::ui::ButtonWnd*, std::allocator<m3d::ui::ButtonWnd*>> m_groupButons;
    WeaponGroupChoiceDlg::AuxInfo m_aif;
    WeaponGroupChoiceDlg();
    WeaponGroupChoiceDlg(WeaponGroupChoiceDlg const& rhs);

public:
    virtual ~WeaponGroupChoiceDlg() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(WeaponGroupChoiceDlg);
};
