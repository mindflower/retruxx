#pragma once

class CBCharacteristicsWnd :  public CharacteristicsWnd
{
public:
    virtual class m3d::Object * Clone();
    virtual ~CBCharacteristicsWnd();
    static struct m3d::Class * GetBaseClass();
    void SetupForCB(int);
    static class m3d::Object * CreateObject();
    virtual struct m3d::Class * GetClass() const ;
protected:
    CBCharacteristicsWnd();
    CBCharacteristicsWnd(class CBCharacteristicsWnd const &);
    void UpdateOnCBChanged();
    virtual void ClearCharacteristics();
    void UpdateCBName();
    virtual int GameDataSetup();
    virtual void UpdateCharacteristics();
    class ai::VehiclePart const * GetCB() const ;
    virtual int GameDataClear(bool);
    void UpdateCBImage();
    virtual void SetupForVehicle(int);
private:
    SetupForVehicle(int);
    m3d::ui::Wnd *m_wndPiercingVal;
    m3d::ui::Wnd *m_wndBlastVal;
    m3d::ui::Wnd *m_wndEnergyVal;
    m3d::ui::Wnd *m_wndMaxDurabilityVal;
    m3d::ui::Wnd *m_wndWeightVal;
    m3d::ui::ImageWnd *m_wndCBImage;
    m3d::ui::Wnd *m_wndCBName;
    CBCharacteristicsWnd::AuxInfo m_aif;
    int m_cbId;
};
