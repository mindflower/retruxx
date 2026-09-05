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
        /* 0x0000 */ CStr m_wndPiercingValName;
        /* 0x000c */ CStr m_wndBlastValName;
        /* 0x0018 */ CStr m_wndEnergyValName;
        /* 0x0024 */ CStr m_wndMaxDurabilityValName;
        /* 0x0030 */ CStr m_wndWeightValName;
        /* 0x003c */ CStr m_wndCBImageName;
        /* 0x0048 */ CStr m_wndCBNameName;
        AuxInfo(CBCharacteristicsWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0054 */

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    virtual void SetupForVehicle(int vehicleId) /* 0x12c */;
    virtual void UpdateCharacteristics() override /* 0x120 */;
    virtual void ClearCharacteristics() override /* 0x124 */;
    void UpdateOnCBChanged();
    void UpdateCBImage();
    void UpdateCBName();
    ai::VehiclePart const* GetCB() const;
    /* 0x0240 */ m3d::ui::Wnd* m_wndPiercingVal;
    /* 0x0244 */ m3d::ui::Wnd* m_wndBlastVal;
    /* 0x0248 */ m3d::ui::Wnd* m_wndEnergyVal;
    /* 0x024c */ m3d::ui::Wnd* m_wndMaxDurabilityVal;
    /* 0x0250 */ m3d::ui::Wnd* m_wndWeightVal;
    /* 0x0254 */ m3d::ui::ImageWnd* m_wndCBImage;
    /* 0x0258 */ m3d::ui::Wnd* m_wndCBName;
    /* 0x025c */ CBCharacteristicsWnd::AuxInfo m_aif;
    /* 0x02b0 */ int m_cbId;
    CBCharacteristicsWnd();
    CBCharacteristicsWnd(CBCharacteristicsWnd const& rhs);

public:
    virtual ~CBCharacteristicsWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    static m3d::Class m_classCBCharacteristicsWnd;
}; /* size: 0x02b4 */
