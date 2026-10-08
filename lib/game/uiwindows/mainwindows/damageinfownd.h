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
        /* 0x0000 */ CStr m_wndHealthName;
        /* 0x000c */ CStr m_wndCabinDurabilityName;
        /* 0x0018 */ CStr m_wndBasketDurabilityName;
        /* 0x0024 */ CStr m_wndFuelName;
        AuxInfo(DamageInfoWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0030 */

protected:
    virtual int GameDataSetup() override /* 0x104 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    int UpdateOnPlayerVehicleChanged();
    /* 0x0220 */ ref_ptr<HealthIndicatorInMainInterfaceWnd> m_wndHealth;
    /* 0x0224 */ ref_ptr<DurabilityIndicatorInMainInterfaceWnd> m_wndCabinDurability;
    /* 0x0228 */ ref_ptr<DurabilityIndicatorInMainInterfaceWnd> m_wndBasketDurability;
    /* 0x022c */ ref_ptr<FuelIndicatorInMainInterfaceWnd> m_wndFuel;
    /* 0x0230 */ DamageInfoWnd::AuxInfo m_aif;
    DamageInfoWnd();
    DamageInfoWnd(DamageInfoWnd const& rhs);

public:
    virtual ~DamageInfoWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classDamageInfoWnd;
}; /* size: 0x0260 */
