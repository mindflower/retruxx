#pragma once
#include <core/ref_ptr.h>
#include <ui/ui.h>

class SpeedometerWnd;
class FuelIndicatorWnd;

class VehicleInfoPanel :  public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        // NOTE: not a friend in the header this class was extracted from,
        // but VehicleInfoPanel::GameDataSetup needs to read this name.
        friend class VehicleInfoPanel;
        CStr m_wndFuelName;
    };

public:
    static m3d::Class * GetBaseClass();
    virtual ~VehicleInfoPanel();
    static m3d::Object * CreateObject();
    virtual m3d::Object * Clone();
    virtual m3d::Class * GetClass() const ;

protected:
    VehicleInfoPanel(VehicleInfoPanel const &);
    VehicleInfoPanel();
    virtual int GameDataSetup();
    void OnPlayerVehicleChanged();
    virtual int GameDataUpdate(void *,int);

public:
    RT_CLASS_DECLARE(VehicleInfoPanel);

private:
    ref_ptr<FuelIndicatorWnd> m_wndFuel;
    ref_ptr<SpeedometerWnd> m_wndSpeedometer;
    VehicleInfoPanel::AuxInfo m_aif;
};
