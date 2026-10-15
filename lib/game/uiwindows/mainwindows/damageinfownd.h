#pragma once
#include <core/ref_ptr.h>
#include <ui/ui.h>

class FuelIndicatorInMainInterfaceWnd;
class DurabilityIndicatorInMainInterfaceWnd;
class HealthIndicatorInMainInterfaceWnd;

class DamageInfoWnd : public m3d::ui::Wnd
{
public:
    struct AuxInfo
    {
        CStr m_wndHealthName;
        CStr m_wndCabinDurabilityName;
        CStr m_wndBasketDurabilityName;
        CStr m_wndFuelName;
        AuxInfo(DamageInfoWnd::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    int UpdateOnPlayerVehicleChanged();
    ref_ptr<HealthIndicatorInMainInterfaceWnd> m_wndHealth;
    ref_ptr<DurabilityIndicatorInMainInterfaceWnd> m_wndCabinDurability;
    ref_ptr<DurabilityIndicatorInMainInterfaceWnd> m_wndBasketDurability;
    ref_ptr<FuelIndicatorInMainInterfaceWnd> m_wndFuel;
    DamageInfoWnd::AuxInfo m_aif;
    DamageInfoWnd();
    DamageInfoWnd(DamageInfoWnd const& rhs);

public:
    virtual ~DamageInfoWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classDamageInfoWnd;
};
