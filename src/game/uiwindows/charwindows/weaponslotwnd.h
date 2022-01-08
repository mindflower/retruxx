#pragma once

class WeaponSlotWnd :  public m3d::ui::Wnd
{
public:
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * CreateObject();
    virtual class m3d::Object * Clone();
    int SetupForGunPart(CStr const &,int);
    virtual ~WeaponSlotWnd();
    static struct m3d::Class * GetBaseClass();
protected:
    virtual int GameDataUpdate(void *,int);
    bool IsSetForGunPart() const ;
    virtual int CreateFromPattern();
    bool NeedShowWeaponGroups() const ;
    void UpdateActiveState();
    void UpdateGunName(class ai::VehiclePart const *);
    void OnVehiclePartChanged(void *);
    virtual int CreateChildren();
    void UpdateGunPartIco(class ai::VehiclePart const *);
    void UpdateOnGunChanged();
    class ai::Vehicle const * GetVehicle() const ;
    static int __fastcall LoadPattern(class m3d::ui::Wnd *);
    void UpdateOnGunPartChanged();
    class ai::VehiclePart const * GetGun() const ;
    void UpdateGunIco(class ai::VehiclePart const *);
    WeaponSlotWnd(class WeaponSlotWnd const &);
    WeaponSlotWnd();
    class m3d::rend::TexHandle GetIcoByGunPartName(CStr const &,bool) const ;
    void ClearWeaponGroups();
private:
    CStr m_gunPartName;
    int m_vehicleId;
    m3d::ui::ImageWnd *m_wndGunPartIco;
    m3d::ui::Wnd *m_wndGunName;
    m3d::ui::ImageWnd *m_wndGunIco;
    ref_ptr<WeaponGroupButtonList> m_wndWeaponGroups;
    m3d::ui::Wnd *m_wndInactiveBg;
};
