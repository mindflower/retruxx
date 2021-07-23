#pragma once

class VehicleCharacteristicsWnd :  public CharacteristicsWnd
{
public:
    virtual class m3d::Object * Clone();
    static class m3d::Object * __fastcall CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual ~VehicleCharacteristicsWnd();
protected:
    virtual void UpdateCharacteristics();
    virtual int GameDataSetup();
    virtual void ClearCharacteristics();
    VehicleCharacteristicsWnd();
    VehicleCharacteristicsWnd(class VehicleCharacteristicsWnd const &);
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
