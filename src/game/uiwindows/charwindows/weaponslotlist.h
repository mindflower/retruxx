#pragma once

class WeaponSlotList :  public m3d::ui::Wnd
{
public:
    virtual struct m3d::Class * GetClass() const ;
    virtual ~WeaponSlotList();
    int SetupForVehicle(int);
    static struct m3d::Class * GetBaseClass();
    static class m3d::Object * CreateObject();
    virtual class m3d::Object * Clone();
protected:
    void OnVehiclePartChanged(void *);
    virtual int OnBeforeAddToWndStation();
    WeaponSlotList();
    WeaponSlotList(class WeaponSlotList const &);
    void GetGunPartNames(int,class std::vector<CStr,class std::allocator<CStr> > &) const ;
    virtual int GameDataClear(bool);
    virtual int OnAfterRemoveFromWndStation();
    void ClearItems();
    int AddItem(CStr const &);
    void CreateItems();
    class ai::Vehicle const * GetVehicle() const ;
    void SortGunPartNames(class std::vector<CStr,class std::allocator<CStr> > &,class ai::Vehicle const *) const ;
    virtual int GameDataSetup();
    void FullUpdate();
    virtual int GameDataUpdate(void *,int);
    void RecalcLayot();
private:
    int m_vehicleId;
    std::vector<ref_ptr<WeaponSlotWnd>> m_items;
    WeaponSlotList::AuxInfo m_aif;
};
