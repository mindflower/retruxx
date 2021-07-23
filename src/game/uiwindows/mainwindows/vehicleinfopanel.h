#pragma once

class VehicleInfoPanel :  public m3d::ui::Wnd
{
public:
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual ~VehicleInfoPanel();
    static class m3d::Object * __fastcall CreateObject();
    virtual class m3d::Object * Clone();
    virtual struct m3d::Class * GetClass() const ;
protected:
    VehicleInfoPanel(class VehicleInfoPanel const &);
    VehicleInfoPanel();
    virtual int GameDataSetup();
    void OnPlayerVehicleChanged();
    virtual int GameDataUpdate(void *,int);
private:
    ref_ptr<FuelIndicatorWnd> m_wndFuel;
    ref_ptr<SpeedometerWnd> m_wndSpeedometer;
    VehicleInfoPanel::AuxInfo m_aif;
};
