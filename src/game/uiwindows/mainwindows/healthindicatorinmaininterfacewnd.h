#pragma once

class HealthIndicatorInMainInterfaceWnd :  public m3d::ui::Wnd
{
public:
    virtual class m3d::Object * Clone();
    void SetType(enum Type);
    int CreateFromPattern(class m3d::ui::Wnd *,bool);
    static class m3d::Object * CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    static struct m3d::Class * GetBaseClass();
    virtual ~HealthIndicatorInMainInterfaceWnd();
    void SetVehicleId(int);
protected:
    void UpdateTooltip(float,float);
    class ai::Vehicle const * GetVehicle() const ;
    virtual int GameDataClear(bool);
    void UpdateLowHpLamp(float,float);
    virtual int GameDataUpdate(void *,int);
    HealthIndicatorInMainInterfaceWnd();
    HealthIndicatorInMainInterfaceWnd(class HealthIndicatorInMainInterfaceWnd const &);
    void GetHp(float &,float &) const ;
    void OnNewFrame();
    void UpdateProgressBar(float,float);
    void UpdateValueWnd(float);
    void FullUpdate(bool);
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
