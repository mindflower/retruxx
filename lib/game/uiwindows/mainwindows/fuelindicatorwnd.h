#pragma once
#include <ui/ui.h>

namespace ai
{
    class Vehicle;
}

namespace m3d
{
    namespace ui
    {
        class ImageWnd;
        class ProgressBarWnd;
    }
}

class FuelIndicatorWnd :  public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_imgLowFuelIcoName;
        CStr m_wndProgressBarName;
    };

public:
    void SetVehicleId(int);
    virtual m3d::Class * GetClass() const ;
    virtual ~FuelIndicatorWnd();
    virtual m3d::Object * Clone();
    static m3d::Class * GetBaseClass();
    int CreateFromPattern(m3d::ui::Wnd *,bool);
    static m3d::Object * CreateObject();

protected:
    FuelIndicatorWnd(FuelIndicatorWnd const &);
    FuelIndicatorWnd();
    virtual int GameDataUpdate(void *,int);
    int UpdateProgressBar();
    ai::Vehicle * GetVehicle() const ;
    int UpdateLowFuelIco();
    int UpdateOnNewFrame();

public:
    RT_CLASS_DECLARE(FuelIndicatorWnd);

private:
    m3d::ui::ImageWnd *m_imgLowFuelIco;
    m3d::ui::ProgressBarWnd *m_wndProgressBar;
    int m_vehicleId;
    FuelIndicatorWnd::AuxInfo m_aif;
    float m_lowFuelThreshold;
    unsigned int m_lowFuelTwinklePeriod;
    unsigned int m_lowFuelTwinkleStart;
};
