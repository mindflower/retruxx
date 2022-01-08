#pragma once

class FuelIndicatorInMainInterfaceWnd :  public m3d::ui::Wnd
{
public:
    void SetType(enum Type);
    static class m3d::Object * CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    static struct m3d::Class * GetBaseClass();
    virtual class m3d::Object * Clone();
    void SetVehicleId(int);
    virtual ~FuelIndicatorInMainInterfaceWnd();
    int CreateFromPattern(class m3d::ui::Wnd *,bool);
protected:
    void UpdateProgressBar(float,float);
    void GetFuel(float &,float &) const ;
    void OnNewFrame();
    void FullUpdate(bool);
    void UpdateValueWnd(float);
    class ai::Vehicle const * GetVehicle() const ;
    void UpdateTooltip(float,float);
    FuelIndicatorInMainInterfaceWnd();
    FuelIndicatorInMainInterfaceWnd(class FuelIndicatorInMainInterfaceWnd const &);
    virtual int GameDataClear(bool);
    virtual int GameDataUpdate(void *,int);
    void UpdateLowFuelLamp(float,float);
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
