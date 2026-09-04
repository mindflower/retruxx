#pragma once
#include <ui/ui.h>

namespace ai
{
    class VehiclePart;
    class Vehicle;
}

class DurabilityIndicatorWnd :  public m3d::ui::Wnd
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
    // NOTE: not present in the header this class was extracted from.
    struct AuxInfo
    {
        AuxInfo();
        /* green/yellow/red/dead durability texture names, one quad per Type */
        CStr m_cabinDurTexNames[TYPE_NUM_TYPES][4];
        CStr m_basketDurTexNames[TYPE_NUM_TYPES][4];
        CStr m_strCabinDurabilityId;
        CStr m_strBasketDurabilityId;
    };

    static m3d::Class * GetBaseClass();
    static m3d::Object * CreateObject();
    virtual int CreateFromPattern(m3d::ui::Wnd *,bool);
    virtual m3d::Object * Clone();
    void SetType(Type,CStr const &);
    virtual m3d::Class * GetClass() const ;
    void SetVehicleId(int);
    virtual ~DurabilityIndicatorWnd();

protected:
    m3d::rend::TexHandle GetImageByDurabilityRelValue(float) const ;
    virtual int UpdateOnNewFrame();
    DurabilityIndicatorWnd();
    DurabilityIndicatorWnd(DurabilityIndicatorWnd const &);
    virtual BoundsBase<float> GetRect() const ;
    ai::Vehicle * GetVehicle() const ;
    void SetDurTextures(CStr const &,CStr const &,CStr const &,CStr const &);
    virtual int OnPaint(m3d::ui::DrawInfo const &);
    virtual int GameDataUpdate(void *,int);
    virtual int UpdateOnVehiclePartChanged();
    virtual void UpdateTooltip(float,float);
    ai::VehiclePart * GetVehiclePart() const ;
    void UpdateVehiclePartImage();
    virtual void UpdateValue(bool);

public:
    RT_CLASS_DECLARE(DurabilityIndicatorWnd);

private:
    int m_vehiclePartId;
    int m_vehicleId;
    CStr m_partName;
    m3d::rend::TexHandle m_durTextures[4];
    DurabilityIndicatorWnd::Type m_type;
    CStr m_strCabinDurability;
    CStr m_strBasketDurability;
    float m_prevCurVal;
    float m_prevMaxVal;
    // NOTE: not present in the header this class was extracted from.
    static inline DurabilityIndicatorWnd::AuxInfo m_aif;
};
