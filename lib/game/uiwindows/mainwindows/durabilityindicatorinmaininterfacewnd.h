#pragma once
#include <ui/image.h>

namespace ai
{
    class VehiclePart;
    class Vehicle;
}  // namespace ai

class DurabilityIndicatorInMainInterfaceWnd : public m3d::ui::ImageWnd
{
public:
    enum Type
    {
        TYPE_IN_MAIN_INTERFACE = 0,
        TYPE_IN_CHARACTERISTIC_WND = 1,
        TYPE_NUM_TYPES = 2,
    };

public:
    virtual int CreateFromPattern(m3d::ui::Wnd* pattern, bool deleteSrc) /* 0x124 */;
    void SetVehicleId(int vehicleId);
    void SetType(DurabilityIndicatorInMainInterfaceWnd::Type type, CStr const& partName);

    struct DurabilityIndicatorInMainInterfaceWnd::AuxInfo
    {
        /* 0x0000 */ CStr m_strCabinDurabilityId = "CabinDurability";
        /* 0x000c */ CStr m_strBasketDurabilityId = "BasketDurability";
    }; /* size: 0x0018 */

protected:
    virtual int GameDataClear(bool beforeContinuousLevle) override /* 0x00 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x00 */;
    void OnNewFrame();
    void OnVehiclePartChanged(void* data);
    void FullUpdate(bool bForce);
    void UpdateColor(float curVal, float maxVal);
    virtual void UpdateTooltip(float val, float maxVal) /* 0x128 */;
    void GetValue(float& curVal, float& maxVal) const;
    unsigned int GetColorByValue(float curVal, float maxVal) const;
    ai::Vehicle const* GetVehicle() const;
    ai::VehiclePart const* GetVehiclePart() const;
    /* 0x0234 */ int m_vehiclePartId;
    /* 0x0238 */ int m_vehicleId;
    /* 0x023c */ CStr m_partName;
    /* 0x0248 */ DurabilityIndicatorInMainInterfaceWnd::Type m_type;
    static inline DurabilityIndicatorInMainInterfaceWnd::AuxInfo m_aif;
    /* 0x024c */ CStr m_strCabinDurability;
    /* 0x0258 */ CStr m_strBasketDurability;
    /* 0x0264 */ float m_prevCurVal;
    /* 0x0268 */ float m_prevMaxVal;
    DurabilityIndicatorInMainInterfaceWnd();
    DurabilityIndicatorInMainInterfaceWnd(DurabilityIndicatorInMainInterfaceWnd const& rhs);

public:
    virtual ~DurabilityIndicatorInMainInterfaceWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classDurabilityIndicatorInMainInterfaceWnd;
}; /* size: 0x026c */
