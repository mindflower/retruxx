#pragma once
#include "childpanel.h"
#include <core/ref_ptr.h>

namespace ai
{
    class GeomRepository;
    class Workshop;
}  // namespace ai

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
    }
}  // namespace m3d

class SkinSwitcher;
class WeaponSlotList;
class BasketCharacteristicsWnd;
class CabinCharacteristicsWnd;
class VehicleCharacteristicsWnd;
class ComplexModelWnd;

class VehicleWnd : public ChildPanel
{
public:
    enum VehicleType
    {
        VEHICLE_NORMAL = 0,
        VEHICLE_HACKED = 1,
    };

public:
    virtual void SetVehicleId(int vehicleId) override /* 0x11c */;

    enum TabId
    {
        TAB_ID_VEHICLE_CHARACTERISTICS = 0,
        TAB_ID_CB_CHARACTERISTICS = 1,
        TAB_ID_WEAPON_GROUPS = 2,
        TAB_ID_NUM_TAB_IDS = 3,
    };

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_wndTruckPictureName;
        /* 0x000c */ CStr m_lblNameName;
        /* 0x0018 */ CStr m_redColor;
        /* 0x0024 */ CStr m_greenColor;
        /* 0x0030 */ CStr m_wndCBCharacteristicsName;

        AuxInfo(VehicleWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x003c */

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual void OnVehiclePartChanged(void* data) /* 0x128 */;
    virtual void SetVehicleType(VehicleWnd::VehicleType type) /* 0x12c */;
    void UpdateVehicleName();
    virtual void OnCharacteristicTabSelChanged(void* data) /* 0x130 */;
    virtual void ShowTab(VehicleWnd::TabId tabId) /* 0x134 */;

    /* 0x0224 */ VehicleWnd::AuxInfo m_aif;
    /* 0x0260 */ ref_ptr<ComplexModelWnd> m_wndVehicleModel;
    /* 0x0264 */ m3d::ui::Wnd* m_lblName;
    /* 0x0268 */ ref_ptr<m3d::ui::Wnd> m_wndCBCharacteristics;
    /* 0x026c */ ref_ptr<VehicleCharacteristicsWnd> m_wndVehicleCharacteristics;
    /* 0x0270 */ ref_ptr<CabinCharacteristicsWnd> m_wndCabinCharacteristics;
    /* 0x0274 */ ref_ptr<BasketCharacteristicsWnd> m_wndBasketCharacteristics;
    /* 0x0278 */ ref_ptr<WeaponSlotList> m_wndWeaponGroups;
    /* 0x027c */ VehicleWnd::VehicleType m_vehicleType;
    /* 0x0280 */ ref_ptr<m3d::ui::Wnd> m_tabItems[3];
    /* 0x028c */ VehicleWnd::TabId m_curTab;

    VehicleWnd();
    VehicleWnd(VehicleWnd const& rhs);

public:
    virtual ~VehicleWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classVehicleWnd;
}; /* size: 0x0290 */
