#pragma once
#include "characteristicswnd.h"

namespace m3d
{
    namespace ui
    {
        class ImageWnd;
    }
}  // namespace m3d

namespace ai
{
    class VehiclePart;
}

class CBCharacteristicsWnd : public CharacteristicsWnd
{
public:
    void SetupForCB(int cbId);

    struct AuxInfo
    {
        CStr m_wndPiercingValName;
        CStr m_wndBlastValName;
        CStr m_wndEnergyValName;
        CStr m_wndMaxDurabilityValName;
        CStr m_wndWeightValName;
        CStr m_wndCBImageName;
        CStr m_wndCBNameName;
        AuxInfo(CBCharacteristicsWnd::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual void SetupForVehicle(int vehicleId);
    virtual void UpdateCharacteristics() override;
    virtual void ClearCharacteristics() override;
    void UpdateOnCBChanged();
    void UpdateCBImage();
    void UpdateCBName();
    ai::VehiclePart const* GetCB() const;
    m3d::ui::Wnd* m_wndPiercingVal;
    m3d::ui::Wnd* m_wndBlastVal;
    m3d::ui::Wnd* m_wndEnergyVal;
    m3d::ui::Wnd* m_wndMaxDurabilityVal;
    m3d::ui::Wnd* m_wndWeightVal;
    m3d::ui::ImageWnd* m_wndCBImage;
    m3d::ui::Wnd* m_wndCBName;
    CBCharacteristicsWnd::AuxInfo m_aif;
    int m_cbId;
    CBCharacteristicsWnd();
    CBCharacteristicsWnd(CBCharacteristicsWnd const& rhs);

public:
    virtual ~CBCharacteristicsWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classCBCharacteristicsWnd;
};
