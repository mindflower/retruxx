#pragma once
#include "characteristicswnd.h"

namespace m3d
{
    namespace ui
    {
        class ImageWnd;
    }
}

namespace ai
{
    class VehiclePart;
}

class CBCharacteristicsWnd :  public CharacteristicsWnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_wndPiercingValName;
        CStr m_wndBlastValName;
        CStr m_wndEnergyValName;
        CStr m_wndMaxDurabilityValName;
        CStr m_wndWeightValName;
        CStr m_wndCBImageName;
        CStr m_wndCBNameName;
    };

public:
    virtual m3d::Object * Clone();
    virtual ~CBCharacteristicsWnd();
    static m3d::Class * GetBaseClass();
    void SetupForCB(int);
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;

protected:
    CBCharacteristicsWnd();
    void UpdateOnCBChanged();
    virtual void ClearCharacteristics();
    void UpdateCBName();
    virtual int GameDataSetup();
    virtual void UpdateCharacteristics();
    ai::VehiclePart const * GetCB() const ;
    virtual int GameDataClear(bool);
    void UpdateCBImage();
    virtual void SetupForVehicle(int);

public:
    RT_CLASS_DECLARE(CBCharacteristicsWnd);

private:
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
