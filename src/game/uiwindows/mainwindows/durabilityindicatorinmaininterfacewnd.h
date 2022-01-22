#pragma once
#include <ui/image.h>

namespace ai
{
    class VehiclePart;
    class Vehicle;
}

class DurabilityIndicatorInMainInterfaceWnd :  public m3d::ui::ImageWnd
{
public:
    enum Type
    {
        TYPE_IN_MAIN_INTERFACE = 0,
        TYPE_IN_CHARACTERISTIC_WND = 1,
        TYPE_NUM_TYPES = 2,
    };

public:
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    virtual int CreateFromPattern(m3d::ui::Wnd *,bool);
    void SetType(Type,CStr const &);
    virtual m3d::Object * Clone();
    void SetVehicleId(int);
    virtual ~DurabilityIndicatorInMainInterfaceWnd();
    static m3d::Class * GetBaseClass();

protected:
    unsigned int GetColorByValue(float,float) const ;
    virtual int GameDataUpdate(void *,int);
    virtual int GameDataClear(bool);
    ai::Vehicle const * GetVehicle() const ;
    DurabilityIndicatorInMainInterfaceWnd();
    DurabilityIndicatorInMainInterfaceWnd(class DurabilityIndicatorInMainInterfaceWnd const &);
    virtual void UpdateTooltip(float,float);
    void OnVehiclePartChanged(void *);
    void GetValue(float &,float &) const ;
    void FullUpdate(bool);
    void OnNewFrame();
    void UpdateColor(float,float);
    ai::VehiclePart const * GetVehiclePart() const ;

public:
    RT_CLASS_DECLARE(DurabilityIndicatorInMainInterfaceWnd);

private:
    int m_vehiclePartId;
    int m_vehicleId;
    CStr m_partName;
    DurabilityIndicatorInMainInterfaceWnd::Type m_type;
    CStr m_strCabinDurability;
    CStr m_strBasketDurability;
    float m_prevCurVal;
    float m_prevMaxVal;
};
