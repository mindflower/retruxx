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
        /* 0x0000 */ CStr m_wndLowFuelLampName = "wndLowFuelLamp";
        /* 0x000c */ CStr m_wndProgressBarName = "wndFuelProgressBar";
        /* 0x0018 */ CStr m_wndValueName = "wndFuelValue";
        /* 0x0024 */ CStr m_wndOverlayName = "wndFuelProgressBarOverlay";
        /* 0x0030 */ CStr m_strFuelId = "Fuel";
    }; /* size: 0x003c */

protected:
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    void OnNewFrame();
    void FullUpdate(bool bForce);
    void UpdateProgressBar(float curFuel, float maxFuel);
    void UpdateLowFuelLamp(float curFuel, float maxFuel);
    void UpdateValueWnd(float curFuel);
    void UpdateTooltip(float curFuel, float maxFuel);
    ai::Vehicle const* GetVehicle() const;
    void GetFuel(float& curFuel, float& maxFuel) const;
    /* 0x0220 */ FuelIndicatorInMainInterfaceWnd::Type m_type;
    /* 0x0224 */ TwinklingLampWnd* m_wndLowFuelLamp;
    /* 0x0228 */ m3d::ui::ProgressBarWnd* m_wndProgressBar;
    /* 0x022c */ ElectronicDigitalWnd* m_wndValue;
    /* 0x0230 */ int m_vehicleId;
    static inline FuelIndicatorInMainInterfaceWnd::AuxInfo m_aif;
    /* 0x0234 */ CStr m_strFuel;
    /* 0x0240 */ float m_prevCurVal;
    /* 0x0244 */ float m_prevMaxVal;
    FuelIndicatorInMainInterfaceWnd();
    FuelIndicatorInMainInterfaceWnd(FuelIndicatorInMainInterfaceWnd const& ths);

public:
    virtual ~FuelIndicatorInMainInterfaceWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classFuelIndicatorInMainInterfaceWnd;
}; /* size: 0x0248 */
