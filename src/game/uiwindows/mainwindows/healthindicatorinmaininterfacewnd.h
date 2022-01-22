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

class HealthIndicatorInMainInterfaceWnd :  public m3d::ui::Wnd
{
public:
    enum Type
    {
        TYPE_IN_MAIN_INTERFACE = 0,
        TYPE_IN_CHARACTERISTIC_WND = 1,
        TYPE_NUM_TYPES = 2,
    };

public:
    virtual m3d::Object * Clone();
    void SetType(Type);
    int CreateFromPattern(m3d::ui::Wnd *,bool);
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    static m3d::Class * GetBaseClass();
    virtual ~HealthIndicatorInMainInterfaceWnd();
    void SetVehicleId(int);

protected:
    void UpdateTooltip(float,float);
    ai::Vehicle const * GetVehicle() const ;
    virtual int GameDataClear(bool);
    void UpdateLowHpLamp(float,float);
    virtual int GameDataUpdate(void *,int);
    HealthIndicatorInMainInterfaceWnd();
    HealthIndicatorInMainInterfaceWnd(HealthIndicatorInMainInterfaceWnd const &);
    void GetHp(float &,float &) const ;
    void OnNewFrame();
    void UpdateProgressBar(float,float);
    void UpdateValueWnd(float);
    void FullUpdate(bool);

public:
    RT_CLASS_DECLARE(HealthIndicatorInMainInterfaceWnd);

private:
    HealthIndicatorInMainInterfaceWnd::Type m_type;
    TwinklingLampWnd *m_wndLowHpLamp;
    m3d::ui::ProgressBarWnd *m_wndProgressBar;
    ElectronicDigitalWnd *m_wndValue;
    int m_vehicleId;
    CStr m_strHealth;
    float m_prevCurVal;
    float m_prevMaxVal;
};
