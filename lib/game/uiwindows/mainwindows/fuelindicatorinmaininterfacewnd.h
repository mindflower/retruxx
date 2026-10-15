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
}  // namespace m3d

class TwinklingLampWnd;

class FuelIndicatorInMainInterfaceWnd : public m3d::ui::Wnd
{
public:
    enum Type
    {
        TYPE_IN_MAIN_INTERFACE = 0,
        TYPE_IN_CHARACTERISTIC_WND = 1,
        TYPE_NUM_TYPES = 2,
    };

public:
    int CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc);
    void SetVehicleId(int vehicleId);
    void SetType(FuelIndicatorInMainInterfaceWnd::Type type);

    struct AuxInfo
    {
        CStr m_wndLowFuelLampName = "wndLowFuelLamp";
        CStr m_wndProgressBarName = "wndFuelProgressBar";
        CStr m_wndValueName = "wndFuelValue";
        CStr m_wndOverlayName = "wndFuelProgressBarOverlay";
        CStr m_strFuelId = "Fuel";
    };

protected:
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    void OnNewFrame();
    void FullUpdate(bool bForce);
    void UpdateProgressBar(float curFuel, float maxFuel);
    void UpdateLowFuelLamp(float curFuel, float maxFuel);
    void UpdateValueWnd(float curFuel);
    void UpdateTooltip(float curFuel, float maxFuel);
    ai::Vehicle const* GetVehicle() const;
    void GetFuel(float& curFuel, float& maxFuel) const;
    FuelIndicatorInMainInterfaceWnd::Type m_type;
    TwinklingLampWnd* m_wndLowFuelLamp;
    m3d::ui::ProgressBarWnd* m_wndProgressBar;
    ElectronicDigitalWnd* m_wndValue;
    int m_vehicleId;
    static inline FuelIndicatorInMainInterfaceWnd::AuxInfo m_aif;
    CStr m_strFuel;
    float m_prevCurVal;
    float m_prevMaxVal;
    FuelIndicatorInMainInterfaceWnd();
    FuelIndicatorInMainInterfaceWnd(FuelIndicatorInMainInterfaceWnd const& ths);

public:
    virtual ~FuelIndicatorInMainInterfaceWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classFuelIndicatorInMainInterfaceWnd;
};
