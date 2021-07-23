#pragma once

class DamageInfoWnd :  public m3d::ui::Wnd
{
public:
    virtual class m3d::Object * Clone();
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * __fastcall CreateObject();
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual ~DamageInfoWnd();
protected:
    int UpdateOnPlayerVehicleChanged();
    virtual int GameDataUpdate(void *,int);
    virtual int GameDataSetup();
    DamageInfoWnd();
    DamageInfoWnd(class DamageInfoWnd const &);
private:
    ref_ptr<HealthIndicatorInMainInterfaceWnd> m_wndHealth;
    ref_ptr<DurabilityIndicatorInMainInterfaceWnd> m_wndCabinDurability;
    ref_ptr<DurabilityIndicatorInMainInterfaceWnd> m_wndBasketDurability;
    ref_ptr<FuelIndicatorInMainInterfaceWnd> m_wndFuel;
    DamageInfoWnd::AuxInfo m_aif;
};
