#pragma once
#include <core/ref_ptr.h>
#include <ui/ui.h>

namespace ai
{
    class Vehicle;
}

class WeaponSlotWnd;

class WeaponSlotList :  public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        float m_space;
    };

public:
    virtual m3d::Class * GetClass() const ;
    virtual ~WeaponSlotList();
    int SetupForVehicle(int);
    static m3d::Class * GetBaseClass();
    static m3d::Object * CreateObject();
    virtual m3d::Object * Clone();

protected:
    void OnVehiclePartChanged(void *);
    virtual int OnBeforeAddToWndStation();
    WeaponSlotList();
    WeaponSlotList(WeaponSlotList const &);
    void GetGunPartNames(int, std::vector<CStr, std::allocator<CStr> > &) const ;
    virtual int GameDataClear(bool);
    virtual int OnAfterRemoveFromWndStation();
    void ClearItems();
    int AddItem(CStr const &);
    void CreateItems();
    ai::Vehicle const * GetVehicle() const ;
    void SortGunPartNames(std::vector<CStr, std::allocator<CStr> > &, ai::Vehicle const *) const ;
    virtual int GameDataSetup();
    void FullUpdate();
    virtual int GameDataUpdate(void *,int);
    void RecalcLayot();

public:
    RT_CLASS_DECLARE(WeaponSlotList);

private:
    int m_vehicleId;
    std::vector<ref_ptr<WeaponSlotWnd>> m_items;
    WeaponSlotList::AuxInfo m_aif;
};
