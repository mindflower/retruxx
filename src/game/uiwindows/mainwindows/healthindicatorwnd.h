#pragma once
#include <ui/progressbarwnd.h>

namespace ai
{
    class Vehicle;
}

class HealthIndicatorWnd :  public m3d::ui::ProgressBarWnd
{
public:
    enum Type
    {
        TYPE_IN_MAIN_INTERFACE = 0,
        TYPE_IN_CHARACTERISTIC_WND = 1,
        TYPE_IN_FIGNYA_WND = 2,
        TYPE_NUM_TYPES = 3,
    };

public:
    virtual ~HealthIndicatorWnd();
    void SetType(Type);
    virtual m3d::Object * Clone();
    virtual int CreateFromPattern(m3d::ui::Wnd *,bool);
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
    void SetVehicleId(int);
    virtual m3d::Class * GetClass() const ;

protected:
    virtual int GameDataUpdate(void *,int);
    virtual void UpdateBarTexture();
    HealthIndicatorWnd();
    HealthIndicatorWnd(HealthIndicatorWnd const &);
    ai::Vehicle * GetVehicle() const ;
    void SetBarTextures(CStr const &,CStr const &,CStr const &);
    virtual int UpdateOnNewFrame();
    virtual void UpdateValue();
    virtual void UpdateTooltip();

public:
    RT_CLASS_DECLARE(HealthIndicatorWnd);

private:
    int m_vehicleId;
    m3d::rend::TexHandle m_barTextures[3];
    HealthIndicatorWnd::Type m_type;
    CStr m_strHealth;
};
