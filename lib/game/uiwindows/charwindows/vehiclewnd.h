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
    virtual void SetVehicleId(int vehicleId) override;

    enum TabId
    {
        TAB_ID_VEHICLE_CHARACTERISTICS = 0,
        TAB_ID_CB_CHARACTERISTICS = 1,
        TAB_ID_WEAPON_GROUPS = 2,
        TAB_ID_NUM_TAB_IDS = 3,
    };

    struct AuxInfo
    {
        CStr m_wndTruckPictureName;
        CStr m_lblNameName;
        CStr m_redColor;
        CStr m_greenColor;
        CStr m_wndCBCharacteristicsName;

        AuxInfo(VehicleWnd::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual void OnVehiclePartChanged(void* data);
    virtual void SetVehicleType(VehicleWnd::VehicleType type);
    void UpdateVehicleName();
    virtual void OnCharacteristicTabSelChanged(void* data);
    virtual void ShowTab(VehicleWnd::TabId tabId);

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

    VehicleWnd();
    VehicleWnd(VehicleWnd const& rhs);

public:
    virtual ~VehicleWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classVehicleWnd;
};
