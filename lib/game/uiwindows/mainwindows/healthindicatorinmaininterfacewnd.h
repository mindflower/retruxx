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
        CStr m_wndLowHpLampName;
        CStr m_wndProgressBarName;
        CStr m_wndValueName;
        CStr m_wndOverlayName;
        CStr m_strHealthId;
        AuxInfo(HealthIndicatorInMainInterfaceWnd::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    void OnNewFrame();
    void FullUpdate(bool bForce);
    void UpdateProgressBar(float curHp, float maxHp);
    void UpdateLowHpLamp(float curHp, float maxHp);
    void UpdateValueWnd(float curHp);
    void UpdateTooltip(float curHp, float maxHp);
    ai::Vehicle const* GetVehicle() const;
    void GetHp(float& curHp, float& maxHp) const;
    HealthIndicatorInMainInterfaceWnd::Type m_type;
    TwinklingLampWnd* m_wndLowHpLamp;
    m3d::ui::ProgressBarWnd* m_wndProgressBar;
    ElectronicDigitalWnd* m_wndValue;
    int m_vehicleId;
    static inline HealthIndicatorInMainInterfaceWnd::AuxInfo m_aif;
    CStr m_strHealth;
    float m_prevCurVal;
    float m_prevMaxVal;
    HealthIndicatorInMainInterfaceWnd();
    HealthIndicatorInMainInterfaceWnd(HealthIndicatorInMainInterfaceWnd const& rhs);

public:
    virtual ~HealthIndicatorInMainInterfaceWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classHealthIndicatorInMainInterfaceWnd;
};
