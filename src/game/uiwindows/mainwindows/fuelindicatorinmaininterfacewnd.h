#pragma once
#include <ui/ui.h>

namespace ai
{
    class Vehicle;
}

class ElectronicDigitalWnd;

namespace m3d
{
    namespace ui
    {
        class ProgressBarWnd;
    }
}

class TwinklingLampWnd;

class FuelIndicatorInMainInterfaceWnd :  public m3d::ui::Wnd
{
public:
    enum Type
    {
        TYPE_IN_MAIN_INTERFACE = 0,
        TYPE_IN_CHARACTERISTIC_WND = 1,
        TYPE_NUM_TYPES = 2,
    };

public:
    void SetType(Type);
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    static m3d::Class * GetBaseClass();
    virtual m3d::Object * Clone();
    void SetVehicleId(int);
    virtual ~FuelIndicatorInMainInterfaceWnd();
    int CreateFromPattern(m3d::ui::Wnd *,bool);

protected:
    void UpdateProgressBar(float,float);
    void GetFuel(float &,float &) const ;
    void OnNewFrame();
    void FullUpdate(bool);
    void UpdateValueWnd(float);
    ai::Vehicle const * GetVehicle() const ;
    void UpdateTooltip(float,float);
    FuelIndicatorInMainInterfaceWnd();
    FuelIndicatorInMainInterfaceWnd(class FuelIndicatorInMainInterfaceWnd const &);
    virtual int GameDataClear(bool);
    virtual int GameDataUpdate(void *,int);
    void UpdateLowFuelLamp(float,float);

public:
    RT_CLASS_DECLARE(FuelIndicatorInMainInterfaceWnd);

private:
    FuelIndicatorInMainInterfaceWnd::Type m_type;
    TwinklingLampWnd *m_wndLowFuelLamp;
    m3d::ui::ProgressBarWnd *m_wndProgressBar;
    ElectronicDigitalWnd *m_wndValue;
    int m_vehicleId;
    CStr m_strFuel;
    float m_prevCurVal;
    float m_prevMaxVal;
};
