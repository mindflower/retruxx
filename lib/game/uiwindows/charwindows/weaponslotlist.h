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
        /* 0x0000 */ float m_space;
        AuxInfo();
    }; /* size: 0x0004 */

protected:
    virtual int GameDataSetup() override /* 0x104 */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int OnBeforeAddToWndStation() override /* 0x68 */;
    virtual int OnAfterRemoveFromWndStation() override /* 0x74 */;
    ai::Vehicle const* GetVehicle() const;
    void FullUpdate();
    void CreateItems();
    void ClearItems();
    int AddItem(CStr const& gunPartName);
    void GetGunPartNames(int vehicleId, std::vector<CStr, std::allocator<CStr>>& gunPartNames) const;
    void SortGunPartNames(std::vector<CStr, std::allocator<CStr>>& gunPartNames, ai::Vehicle const* vehicle) const;
    void RecalcLayot();
    void OnVehiclePartChanged(void* data);

    /* 0x0220 */ int m_vehicleId;
    /* 0x0224 */ std::vector<ref_ptr<WeaponSlotWnd>, std::allocator<ref_ptr<WeaponSlotWnd>>> m_items;
    /* 0x0234 */ WeaponSlotList::AuxInfo m_aif;

    WeaponSlotList();
    WeaponSlotList(WeaponSlotList const& rhs);

public:
    virtual ~WeaponSlotList() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classWeaponSlotList;
}; /* size: 0x0238 */
