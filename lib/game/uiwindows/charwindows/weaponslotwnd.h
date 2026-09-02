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
public:
    int SetupForGunPart(CStr const& gunPartName, int vehicleId);
    CStr const& GetGunPartName() const;

    struct WeaponSlotWnd::AuxInfo
    {
        /* 0x0000 */ CStr m_wndPatternName;
        /* 0x000c */ CStr m_wndPatternGunPartIcoName;
        /* 0x0018 */ CStr m_wndPatternGunNameName;
        /* 0x0024 */ CStr m_wndPatternGunIcoName;
        /* 0x0030 */ CStr m_wndPatternInactiveBgName;
        /* 0x003c */ CStr m_texIdGunPart;
        /* 0x0048 */ CStr m_texIdGunPartNone;
        /* 0x0054 */ CStr m_strPadding;
        /* 0x0060 */ CStr m_strIdGunNone;
        /* 0x006c */ unsigned int m_colorGunNone;

        AuxInfo(WeaponSlotWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0070 */

    struct WeaponSlotWnd::Pattern
    {
        Pattern();
        ~Pattern();
        void IncRef();
        void DecRef();
        int GetRef() const;

        /* 0x0000 */ m3d::ui::Wnd* m_wndPattern;
        /* 0x0004 */ m3d::ui::ImageWnd* m_wndPatternGunPartIco;
        /* 0x0008 */ m3d::ui::Wnd* m_wndPatternGunName;
        /* 0x000c */ m3d::ui::ImageWnd* m_wndPatternGunIco;
        /* 0x0010 */ m3d::ui::Wnd* m_wndPatternWeaponGroups;
        /* 0x0014 */ m3d::ui::Wnd* m_wndPatternInactiveBg;

        void Clear();
        /* 0x0018 */ int m_ref;
    }; /* size: 0x001c */

protected:
    virtual int CreateFromPattern() /* 0x11c */;
    virtual int CreateChildren() /* 0x120 */;
    static int __fastcall LoadPattern(m3d::ui::Wnd* pattern);
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
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

    /* 0x0220 */ CStr m_gunPartName;
    /* 0x022c */ int m_vehicleId;
    /* 0x0230 */ m3d::ui::ImageWnd* m_wndGunPartIco;
    /* 0x0234 */ m3d::ui::Wnd* m_wndGunName;
    /* 0x0238 */ m3d::ui::ImageWnd* m_wndGunIco;
    /* 0x023c */ ref_ptr<WeaponGroupButtonList> m_wndWeaponGroups;
    /* 0x0240 */ m3d::ui::Wnd* m_wndInactiveBg;

    static inline WeaponSlotWnd::AuxInfo m_aif;
    static inline WeaponSlotWnd::Pattern m_pattern;

    WeaponSlotWnd();
    WeaponSlotWnd(WeaponSlotWnd const& rhs);

public:
    virtual ~WeaponSlotWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classWeaponSlotWnd;
}; /* size: 0x0244 */
