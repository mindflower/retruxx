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
    virtual int CreateFromPattern(m3d::ui::Wnd* pattern, bool deleteSrc);
    void SetVehicleId(int vehicleId);
    void SetType(DurabilityIndicatorInMainInterfaceWnd::Type type, CStr const& partName);

    struct DurabilityIndicatorInMainInterfaceWnd::AuxInfo
    {
        CStr m_strCabinDurabilityId = "CabinDurability";
        CStr m_strBasketDurabilityId = "BasketDurability";
    };

protected:
    virtual int GameDataClear(bool beforeContinuousLevle) override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    void OnNewFrame();
    void OnVehiclePartChanged(void* data);
    void FullUpdate(bool bForce);
    void UpdateColor(float curVal, float maxVal);
    virtual void UpdateTooltip(float val, float maxVal);
    void GetValue(float& curVal, float& maxVal) const;
    unsigned int GetColorByValue(float curVal, float maxVal) const;
    ai::Vehicle const* GetVehicle() const;
    ai::VehiclePart const* GetVehiclePart() const;
    int m_vehiclePartId;
    int m_vehicleId;
    CStr m_partName;
    DurabilityIndicatorInMainInterfaceWnd::Type m_type;
    static inline DurabilityIndicatorInMainInterfaceWnd::AuxInfo m_aif;
    CStr m_strCabinDurability;
    CStr m_strBasketDurability;
    float m_prevCurVal;
    float m_prevMaxVal;
    DurabilityIndicatorInMainInterfaceWnd();
    DurabilityIndicatorInMainInterfaceWnd(DurabilityIndicatorInMainInterfaceWnd const& rhs);

public:
    virtual ~DurabilityIndicatorInMainInterfaceWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classDurabilityIndicatorInMainInterfaceWnd;
};
