#pragma once
#include <core/ref_ptr.h>
#include <server/damageinfo.h>
#include <ui/ui.h>
#include <ui/wnd.h>

namespace ai
{
    class Obj;
    class Vehicle;
}

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
        class ImageWnd;
        class ProgressBarWnd;
    }
}

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
    const ai::Obj* GetGun() const;
    int GetGroupId() const;

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_wndPatternName;
        /* 0x000c */ CStr m_wndPatternIcoName;
        /* 0x0018 */ CStr m_wndPatternChargeName;
        /* 0x0024 */ CStr m_wndPatternNameName;
        /* 0x0030 */ CStr m_wndPatternAmmoName;
        /* 0x003c */ CStr m_wndPatternFrame0Name;
        /* 0x0048 */ CStr m_wndPatternFrame1Name;
        /* 0x0054 */ CStr m_wndPatternBgName;
        /* 0x0060 */ CStr m_wndPatternCanShotBgName;
        /* 0x006c */ CStr m_wndPatternGroupName;
        /* 0x0078 */ float m_height;
        /* 0x007c */ CStr m_paneName;
        /* 0x0088 */ float m_space;
        /* 0x008c */ float m_chargeBarW;
        /* 0x0090 */ CStr m_chargeBarTexName;
        /* 0x009c */ float m_lblReloadsW;
        /* 0x00a0 */ CStr m_weaponTypeTexNames[13];
        AuxInfo(const WeaponInfoWnd::AuxInfo&);
        AuxInfo();
    }; /* size: 0x013c */

    struct Pattern
    {
        /* 0x0000 */ m3d::ui::Wnd* m_wndPattern;
        /* 0x0004 */ m3d::ui::ImageWnd* m_wndPatternIco;
        /* 0x0008 */ m3d::ui::Wnd* m_wndPatternName;
        /* 0x000c */ m3d::ui::ProgressBarWnd* m_wndPatternCharge;
        /* 0x0010 */ m3d::ui::Wnd* m_wndPatternAmmo;
        /* 0x0014 */ m3d::ui::Wnd* m_wndPatternFrame0;
        /* 0x0018 */ m3d::ui::Wnd* m_wndPatternFrame1;
        /* 0x001c */ m3d::ui::ImageWnd* m_wndPatternBg;
        /* 0x0020 */ m3d::ui::ImageWnd* m_wndPatternCanShotBg;
        /* 0x0024 */ m3d::ui::Wnd* m_wndPatternGroup;
        Pattern();
    }; /* size: 0x0028 */

protected:
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    void UpdateOnNewFrame();
    void UpdateCharge();
    void UpdateAmmo();
    void UpdateIco();
    void UpdateCanShotBg();
    ai::DamageType GetWeaponTypeForGun(const ai::Obj* gun) const;
    int CreateFromPattern(WeaponInfoList::Type type);
    int CreateChildren(WeaponInfoList::Type type);
    static int __fastcall LoadPattern(m3d::ui::Wnd* pattern, WeaponInfoList::Type type);
    static void __fastcall ClearPattern(WeaponInfoList::Type type);
    m3d::rend::TexHandle GetIco() const;
    m3d::rend::TexHandle GetBarTexture(bool bReady) const;
    bool IsGunReady() const;
    int GetAmmo() const;
    /* 0x0220 */ int m_gunId;
    /* 0x0224 */ m3d::ui::ProgressBarWnd* m_wndCharge;
    /* 0x0228 */ m3d::ui::ImageWnd* m_wndIco;
    /* 0x022c */ m3d::ui::Wnd* m_wndName;
    /* 0x0230 */ m3d::ui::Wnd* m_wndAmmo;
    /* 0x0234 */ m3d::ui::Wnd* m_wndFrame0;
    /* 0x0238 */ m3d::ui::Wnd* m_wndFrame1;
    /* 0x023c */ m3d::ui::ImageWnd* m_wndBg;
    /* 0x0240 */ m3d::ui::ImageWnd* m_wndCanShotBg;
    /* 0x0244 */ m3d::ui::Wnd* m_wndGroup;
    static WeaponInfoWnd::AuxInfo m_aif;
    static WeaponInfoWnd::Pattern m_patterns[2];
    /* 0x0248 */ int m_groupId;
    /* 0x024c */ WeaponInfoList::Type m_type;
    static m3d::rend::TexHandle m_pbChargeTextureReady;
    static m3d::rend::TexHandle m_pbChargeTextureRecharging;
    WeaponInfoWnd();
    WeaponInfoWnd(const WeaponInfoWnd& rhs);

public:
    virtual ~WeaponInfoWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classWeaponInfoWnd;
}; /* size: 0x0250 */

class WeaponGroupChoiceDlg : public m3d::ui::ModalWnd
{
public:
    static CStr __fastcall CreateTooltipForWeaponGroup(int groupId);
    using ButtonVector = std::vector<m3d::ui::ButtonWnd*, std::allocator<m3d::ui::ButtonWnd*>>;

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_groupButtonName;
        /* 0x000c */ unsigned int m_selTextColor;
        AuxInfo(WeaponGroupChoiceDlg::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0010 */

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x00 */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override /* 0x00 */;
    virtual int OnKey(unsigned short key, unsigned char scanCode, unsigned int state) override /* 0x00 */;
    void OnKeyBindingsChanged();
    void SelectButton(int id);
    /* 0x0224 */ std::vector<m3d::ui::ButtonWnd*, std::allocator<m3d::ui::ButtonWnd*>> m_groupButons;
    /* 0x0234 */ WeaponGroupChoiceDlg::AuxInfo m_aif;
    WeaponGroupChoiceDlg();
    WeaponGroupChoiceDlg(WeaponGroupChoiceDlg const& rhs);

public:
    virtual ~WeaponGroupChoiceDlg() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    RT_CLASS_DECLARE(WeaponGroupChoiceDlg);
}; /* size: 0x0244 */
