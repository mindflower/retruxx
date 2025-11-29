#pragma once
#include <core/ref_ptr.h>
#include <ui/ui.h>

class FuelIndicatorInMainInterfaceWnd;
class DurabilityIndicatorInMainInterfaceWnd;
class HealthIndicatorInMainInterfaceWnd;

class DamageInfoWnd :  public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();
        CStr m_wndHealthName;
        CStr m_wndCabinDurabilityName;
        CStr m_wndBasketDurabilityName;
        CStr m_wndFuelName;
    };

public:
    virtual m3d::Object * Clone();
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
    virtual ~DamageInfoWnd();

protected:
    int UpdateOnPlayerVehicleChanged();
    virtual int GameDataUpdate(void *,int);
    virtual int GameDataSetup();
    DamageInfoWnd();
    DamageInfoWnd(DamageInfoWnd const &);

public:
    RT_CLASS_DECLARE(DamageInfoWnd);

private:
    ref_ptr<HealthIndicatorInMainInterfaceWnd> m_wndHealth;
    ref_ptr<DurabilityIndicatorInMainInterfaceWnd> m_wndCabinDurability;
    ref_ptr<DurabilityIndicatorInMainInterfaceWnd> m_wndBasketDurability;
    ref_ptr<FuelIndicatorInMainInterfaceWnd> m_wndFuel;
    DamageInfoWnd::AuxInfo m_aif;
};
