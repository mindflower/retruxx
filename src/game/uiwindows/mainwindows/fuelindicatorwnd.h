#pragma once

class FuelIndicatorWnd :  public m3d::ui::Wnd
{
public:
    void SetVehicleId(int);
    virtual struct m3d::Class * GetClass() const ;
    virtual ~FuelIndicatorWnd();
    virtual class m3d::Object * Clone();
    static struct m3d::Class * __fastcall GetBaseClass();
    int CreateFromPattern(class m3d::ui::Wnd *,bool);
    static class m3d::Object * __fastcall CreateObject();
protected:
    FuelIndicatorWnd(class FuelIndicatorWnd const &);
    FuelIndicatorWnd();
    virtual int GameDataUpdate(void *,int);
    int UpdateProgressBar();
    class ai::Vehicle * GetVehicle() const ;
    int UpdateLowFuelIco();
    int UpdateOnNewFrame();
private:
    m3d::ui::ImageWnd *m_imgLowFuelIco;
    m3d::ui::ProgressBarWnd *m_wndProgressBar;
    int m_vehicleId;
    FuelIndicatorWnd::AuxInfo m_aif;
    float m_lowFuelThreshold;
    unsigned int m_lowFuelTwinklePeriod;
    unsigned int m_lowFuelTwinkleStart;
};
