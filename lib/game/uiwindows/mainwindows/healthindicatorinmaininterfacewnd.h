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

class HealthIndicatorInMainInterfaceWnd : public m3d::ui::Wnd
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
    void SetType(HealthIndicatorInMainInterfaceWnd::Type type);

    struct HealthIndicatorInMainInterfaceWnd::AuxInfo
    {
        /* 0x0000 */ CStr m_wndLowHpLampName;
        /* 0x000c */ CStr m_wndProgressBarName;
        /* 0x0018 */ CStr m_wndValueName;
        /* 0x0024 */ CStr m_wndOverlayName;
        /* 0x0030 */ CStr m_strHealthId;
        AuxInfo(HealthIndicatorInMainInterfaceWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x003c */

protected:
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    void OnNewFrame();
    void FullUpdate(bool bForce);
    void UpdateProgressBar(float curHp, float maxHp);
    void UpdateLowHpLamp(float curHp, float maxHp);
    void UpdateValueWnd(float curHp);
    void UpdateTooltip(float curHp, float maxHp);
    ai::Vehicle const* GetVehicle() const;
    void GetHp(float& curHp, float& maxHp) const;
    /* 0x0220 */ HealthIndicatorInMainInterfaceWnd::Type m_type;
    /* 0x0224 */ TwinklingLampWnd* m_wndLowHpLamp;
    /* 0x0228 */ m3d::ui::ProgressBarWnd* m_wndProgressBar;
    /* 0x022c */ ElectronicDigitalWnd* m_wndValue;
    /* 0x0230 */ int m_vehicleId;
    static inline HealthIndicatorInMainInterfaceWnd::AuxInfo m_aif;
    /* 0x0234 */ CStr m_strHealth;
    /* 0x0240 */ float m_prevCurVal;
    /* 0x0244 */ float m_prevMaxVal;
    HealthIndicatorInMainInterfaceWnd();
    HealthIndicatorInMainInterfaceWnd(HealthIndicatorInMainInterfaceWnd const& rhs);

public:
    virtual ~HealthIndicatorInMainInterfaceWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classHealthIndicatorInMainInterfaceWnd;
}; /* size: 0x0248 */
