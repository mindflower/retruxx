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
    virtual int CreateFromPattern(m3d::ui::Wnd* p, bool deleteSrc) /* 0x128 */;
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
        /* 0x0000 */ CStr m_barTexNames[3][3];
        /* 0x006c */ CStr m_strHealthId;
        AuxInfo(HealthIndicatorWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0078 */

protected:
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x00 */;
    virtual int UpdateOnNewFrame() /* 0x12c */;
    virtual void UpdateValue() /* 0x130 */;
    virtual void UpdateBarTexture() /* 0x134 */;
    virtual void UpdateTooltip() /* 0x138 */;
    ai::Vehicle* GetVehicle() const;
    void SetBarTextures(CStr const& texGreenName, CStr const& texYellowName, CStr const& texRedName);
    /* 0x0244 */ int m_vehicleId;
    static inline HealthIndicatorWnd::AuxInfo m_aif;
    /* 0x0248 */ m3d::rend::TexHandle m_barTextures[3];
    /* 0x0254 */ HealthIndicatorWnd::Type m_type;
    /* 0x0258 */ CStr m_strHealth;
    HealthIndicatorWnd();
    HealthIndicatorWnd(HealthIndicatorWnd const& rhs);

public:
    virtual ~HealthIndicatorWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classHealthIndicatorWnd;
}; /* size: 0x0264 */
