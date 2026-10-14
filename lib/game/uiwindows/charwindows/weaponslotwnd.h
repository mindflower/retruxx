#pragma once
#include <core/ref_ptr.h>
#include <ui/ui.h>

namespace ai
{
    class Vehicle;
    class VehiclePart;
}  // namespace ai

class WeaponGroupButtonList;

namespace m3d
{
    namespace ui
    {
        class ImageWnd;
    }
}  // namespace m3d

class WeaponSlotWnd : public m3d::ui::Wnd
{
    // WeaponSlotList owns the shared pattern's lifetime and builds the slots.
    friend class WeaponSlotList;

public:
    int SetupForGunPart(CStr const& gunPartName, int vehicleId);
    CStr const& GetGunPartName() const;

    struct WeaponSlotWnd::AuxInfo
    {
        CStr m_wndPatternName;
        CStr m_wndPatternGunPartIcoName;
        CStr m_wndPatternGunNameName;
        CStr m_wndPatternGunIcoName;
        CStr m_wndPatternInactiveBgName;
        CStr m_texIdGunPart;
        CStr m_texIdGunPartNone;
        CStr m_strPadding;
        CStr m_strIdGunNone;
        unsigned int m_colorGunNone;

        AuxInfo(WeaponSlotWnd::AuxInfo const&);
        AuxInfo();
    };

    struct WeaponSlotWnd::Pattern
    {
        Pattern();
        ~Pattern();
        void IncRef();
        void DecRef();
        int GetRef() const;

        m3d::ui::Wnd* m_wndPattern;
        m3d::ui::ImageWnd* m_wndPatternGunPartIco;
        m3d::ui::Wnd* m_wndPatternGunName;
        m3d::ui::ImageWnd* m_wndPatternGunIco;
        m3d::ui::Wnd* m_wndPatternWeaponGroups;
        m3d::ui::Wnd* m_wndPatternInactiveBg;

        void Clear();
        int m_ref;
    };

protected:
    virtual int CreateFromPattern();
    virtual int CreateChildren();
    static int __fastcall LoadPattern(m3d::ui::Wnd* pattern);
    virtual int GameDataUpdate(void* data, int dataType) override;
    void OnVehiclePartChanged(void* data);
    void UpdateOnGunPartChanged();
    void UpdateActiveState();
    void UpdateOnGunChanged();
    void UpdateGunPartIco(ai::VehiclePart const* gun);
    void UpdateGunName(ai::VehiclePart const* gun);
    void UpdateGunIco(ai::VehiclePart const* gun);
    bool IsSetForGunPart() const;
    ai::VehiclePart const* GetGun() const;
    ai::Vehicle const* GetVehicle() const;
    m3d::rend::TexHandle GetIcoByGunPartName(CStr const& gunPartName, bool bGunInstalled) const;
    bool NeedShowWeaponGroups() const;
    void ClearWeaponGroups();

    CStr m_gunPartName;
    int m_vehicleId;
    m3d::ui::ImageWnd* m_wndGunPartIco;
    m3d::ui::Wnd* m_wndGunName;
    m3d::ui::ImageWnd* m_wndGunIco;
    ref_ptr<WeaponGroupButtonList> m_wndWeaponGroups;
    m3d::ui::Wnd* m_wndInactiveBg;

    static inline WeaponSlotWnd::AuxInfo m_aif;
    static inline WeaponSlotWnd::Pattern m_pattern;

    WeaponSlotWnd();
    WeaponSlotWnd(WeaponSlotWnd const& rhs);

public:
    virtual ~WeaponSlotWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classWeaponSlotWnd;
};
