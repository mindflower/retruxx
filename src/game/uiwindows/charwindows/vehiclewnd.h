#pragma once
#include "childpanel.h"
#include <core/ref_ptr.h>

namespace ai
{
    class GeomRepository;
    class Workshop;
}

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
    }
}

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
        VEHICLE_NORMAL = 0x0,
        VEHICLE_HACKED = 0x1,
    };

    enum TabId
    {
        TAB_ID_VEHICLE_CHARACTERISTICS = 0x0,
        TAB_ID_CB_CHARACTERISTICS = 0x1,
        TAB_ID_WEAPON_GROUPS = 0x2,
        TAB_ID_NUM_TAB_IDS = 0x3,
    };

    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_wndTruckPictureName;
        CStr m_lblNameName;
        CStr m_redColor;
        CStr m_greenColor;
        CStr m_wndCBCharacteristicsName;
    };

public:
    virtual void SetVehicleId(int);
    virtual ~VehicleWnd(void);

protected:
    VehicleWnd(VehicleWnd const&);
    VehicleWnd(void);
    void UpdateVehicleName(void);
    virtual void OnVehiclePartChanged(void*);
    virtual void ShowTab(TabId);
    virtual void OnCharacteristicTabSelChanged(void*);

public:
    RT_CLASS_DECLARE(VehicleWnd);

private:
    VehicleWnd::AuxInfo m_aif;
    ref_ptr<ComplexModelWnd> m_wndVehicleModel;
    m3d::ui::Wnd* m_lblName;
    ref_ptr<m3d::ui::Wnd> m_wndCBCharacteristics;
    ref_ptr<VehicleCharacteristicsWnd> m_wndVehicleCharacteristics;
    ref_ptr<CabinCharacteristicsWnd> m_wndCabinCharacteristics;
    ref_ptr<BasketCharacteristicsWnd> m_wndBasketCharacteristics;
    ref_ptr<WeaponSlotList> m_wndWeaponGroups;
    VehicleWnd::VehicleType m_vehicleType;
    ref_ptr<m3d::ui::Wnd> m_tabItems[3];
    VehicleWnd::TabId m_curTab;
};

