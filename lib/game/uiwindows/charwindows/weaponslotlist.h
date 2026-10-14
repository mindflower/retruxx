#pragma once
#include <core/ref_ptr.h>
#include <ui/ui.h>

namespace ai
{
    class Vehicle;
}

class WeaponSlotWnd;

class WeaponSlotList : public m3d::ui::Wnd
{
public:
    int SetupForVehicle(int vehicleId);

    using WeaponSlotWndVector = std::vector<ref_ptr<WeaponSlotWnd>, std::allocator<ref_ptr<WeaponSlotWnd>>>;

    struct WeaponSlotList::AuxInfo
    {
        float m_space;
        AuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int OnBeforeAddToWndStation() override;
    virtual int OnAfterRemoveFromWndStation() override;
    ai::Vehicle const* GetVehicle() const;
    void FullUpdate();
    void CreateItems();
    void ClearItems();
    int AddItem(CStr const& gunPartName);
    void GetGunPartNames(int vehicleId, std::vector<CStr, std::allocator<CStr>>& gunPartNames) const;
    void SortGunPartNames(std::vector<CStr, std::allocator<CStr>>& gunPartNames, ai::Vehicle const* vehicle) const;
    void RecalcLayot();
    void OnVehiclePartChanged(void* data);

    int m_vehicleId;
    std::vector<ref_ptr<WeaponSlotWnd>, std::allocator<ref_ptr<WeaponSlotWnd>>> m_items;
    WeaponSlotList::AuxInfo m_aif;

    WeaponSlotList();
    WeaponSlotList(WeaponSlotList const& rhs);

public:
    virtual ~WeaponSlotList() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classWeaponSlotList;
};
