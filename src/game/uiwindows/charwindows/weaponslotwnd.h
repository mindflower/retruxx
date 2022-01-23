#pragma once
#include <core/ref_ptr.h>
#include <ui/ui.h>

namespace ai
{
    class Vehicle;
    class VehiclePart;
}

class WeaponGroupButtonList;

namespace m3d
{
    namespace ui
    {
        class ImageWnd;
    }
}

class WeaponSlotWnd :  public m3d::ui::Wnd
{
public:
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    virtual m3d::Object * Clone();
    int SetupForGunPart(CStr const &,int);
    virtual ~WeaponSlotWnd();
    static m3d::Class * GetBaseClass();

protected:
    virtual int GameDataUpdate(void *,int);
    bool IsSetForGunPart() const ;
    virtual int CreateFromPattern();
    bool NeedShowWeaponGroups() const ;
    void UpdateActiveState();
    void UpdateGunName(ai::VehiclePart const *);
    void OnVehiclePartChanged(void *);
    virtual int CreateChildren();
    void UpdateGunPartIco(ai::VehiclePart const *);
    void UpdateOnGunChanged();
    ai::Vehicle const * GetVehicle() const ;
    static int __fastcall LoadPattern(m3d::ui::Wnd *);
    void UpdateOnGunPartChanged();
    ai::VehiclePart const * GetGun() const ;
    void UpdateGunIco(ai::VehiclePart const *);
    WeaponSlotWnd(WeaponSlotWnd const &);
    WeaponSlotWnd();
    m3d::rend::TexHandle GetIcoByGunPartName(CStr const &,bool) const ;
    void ClearWeaponGroups();

public:
    RT_CLASS_DECLARE(WeaponSlotWnd);

private:
    CStr m_gunPartName;
    int m_vehicleId;
    m3d::ui::ImageWnd *m_wndGunPartIco;
    m3d::ui::Wnd *m_wndGunName;
    m3d::ui::ImageWnd *m_wndGunIco;
    ref_ptr<WeaponGroupButtonList> m_wndWeaponGroups;
    m3d::ui::Wnd *m_wndInactiveBg;
};
