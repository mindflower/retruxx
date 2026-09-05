#pragma once
#include "vehiclewnd.h"

class CharacteristicsWnd : public m3d::ui::Wnd
{
public:
    void SetupForVehicle(int vehicleId);
    void SetVehicleType(VehicleWnd::VehicleType vehicleType);

    struct CAuxInfo
    {
        /* 0x0000 */ CStr m_redColor;
        /* 0x000c */ CStr m_greenColor;
        CAuxInfo(const CharacteristicsWnd::CAuxInfo&);
        CAuxInfo();
    }; /* size: 0x0018 */

protected:
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual void OnNewFrame() /* 0x11c */;
    virtual void UpdateCharacteristics() = 0 /* 0x120 */;
    virtual void ClearCharacteristics() = 0 /* 0x124 */;
    const ai::Vehicle* GetVehicle() const;
    CStr GetColorByVal(int val, int baseVal, bool bInverted) const;
    CStr GetDiffValPrefix(int diff, bool bColorInverted) const;
    void UpdateValue(m3d::ui::Wnd* ctrl, int val, int baseVal, bool bColorInverted);
    void UpdateValueNormal(m3d::ui::Wnd* ctrl, int val, int baseVal, bool bColorInverted);
    void UpdateValueHacked(m3d::ui::Wnd* ctrl, int val, int baseVal, bool bColorInverted);
    /* 0x0220 */ CharacteristicsWnd::CAuxInfo m_caif;
    /* 0x0238 */ VehicleWnd::VehicleType m_vehicleType;
    /* 0x023c */ int m_vehicleId;
    CharacteristicsWnd(const CharacteristicsWnd&);
    CharacteristicsWnd();

public:
    virtual ~CharacteristicsWnd() override /* 0x00 */;
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetRtClass() const /* 0x128 */;
    static m3d::Class m_classCharacteristicsWnd;
}; /* size: 0x0240 */
