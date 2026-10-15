#pragma once
#include <ui/progressbarwnd.h>

namespace ai
{
    class Vehicle;
}

class HealthIndicatorWnd : public m3d::ui::ProgressBarWnd
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
    virtual int CreateFromPattern(m3d::ui::Wnd* p, bool deleteSrc);
    void SetVehicleId(int vehicleId);
    void SetType(HealthIndicatorWnd::Type type);

    enum Color
    {
        GREEN = 0,
        YELLOW = 1,
        RED = 2,
        NUM_COLORS = 3,
    };

    struct AuxInfo
    {
        CStr m_barTexNames[3][3];
        CStr m_strHealthId;
        AuxInfo(HealthIndicatorWnd::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int UpdateOnNewFrame();
    virtual void UpdateValue();
    virtual void UpdateBarTexture();
    virtual void UpdateTooltip();
    ai::Vehicle* GetVehicle() const;
    void SetBarTextures(CStr const& texGreenName, CStr const& texYellowName, CStr const& texRedName);
    int m_vehicleId;
    static inline HealthIndicatorWnd::AuxInfo m_aif;
    m3d::rend::TexHandle m_barTextures[3];
    HealthIndicatorWnd::Type m_type;
    CStr m_strHealth;
    HealthIndicatorWnd();
    HealthIndicatorWnd(HealthIndicatorWnd const& rhs);

public:
    virtual ~HealthIndicatorWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classHealthIndicatorWnd;
};
