#pragma once
#include "characteristicswnd.h"

class VehicleCharacteristicsWnd :  public CharacteristicsWnd
{
    // VehicleWnd::SetVehicleId invokes the protected virtual ClearCharacteristics().
    friend class VehicleWnd;

public:
    class AuxInfo
    {
    public:
        AuxInfo();

        CStr m_wndMaxHealthValName;
        CStr m_wndMaxFuelValName;
        CStr m_wndPiercingValName;
        CStr m_wndBlastValName;
        CStr m_wndEnergyValName;
        CStr m_wndMaxDurabilityValName;
        CStr m_wndBasketCapacityValName;
        CStr m_wndMaxSpeedValName;
        CStr m_wndTorqueValName;
        CStr m_wndWeightValName;
        CStr m_wndControlValName;
    };

public:
    virtual m3d::Object * Clone();
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    static m3d::Class * GetBaseClass();
    virtual ~VehicleCharacteristicsWnd();

protected:
    virtual void UpdateCharacteristics();
    virtual int GameDataSetup();
    virtual void ClearCharacteristics();
    VehicleCharacteristicsWnd();
    VehicleCharacteristicsWnd(VehicleCharacteristicsWnd const &);

public:
    RT_CLASS_DECLARE(VehicleCharacteristicsWnd);

private:
    m3d::ui::Wnd *m_wndMaxHealthVal;
    m3d::ui::Wnd *m_wndMaxFuelVal;
    m3d::ui::Wnd *m_wndPiercingVal;
    m3d::ui::Wnd *m_wndBlastVal;
    m3d::ui::Wnd *m_wndEnergyVal;
    m3d::ui::Wnd *m_wndMaxDurabilityVal;
    m3d::ui::Wnd *m_wndBasketCapacityVal;
    m3d::ui::Wnd *m_wndMaxSpeedVal;
    m3d::ui::Wnd *m_wndTorqueVal;
    m3d::ui::Wnd *m_wndWeightVal;
    m3d::ui::Wnd *m_wndControlVal;
    VehicleCharacteristicsWnd::AuxInfo m_aif;
};
