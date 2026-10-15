#pragma once
#include "vehiclewnd.h"

class CharacteristicsWnd : public m3d::ui::Wnd
{
public:
    void SetupForVehicle(int vehicleId);
    void SetVehicleType(VehicleWnd::VehicleType vehicleType);

    struct CAuxInfo
    {
        CStr m_redColor;
        CStr m_greenColor;
        CAuxInfo(CharacteristicsWnd::CAuxInfo const&);
        CAuxInfo();
    };

protected:
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual void OnNewFrame();
    virtual void UpdateCharacteristics() = 0;
    virtual void ClearCharacteristics() = 0;
    ai::Vehicle const* GetVehicle() const;
    CStr GetColorByVal(int val, int baseVal, bool bInverted) const;
    CStr GetDiffValPrefix(int diff, bool bColorInverted) const;
    void UpdateValue(m3d::ui::Wnd* ctrl, int val, int baseVal, bool bColorInverted);
    void UpdateValueNormal(m3d::ui::Wnd* ctrl, int val, int baseVal, bool bColorInverted);
    void UpdateValueHacked(m3d::ui::Wnd* ctrl, int val, int baseVal, bool bColorInverted);
    CharacteristicsWnd::CAuxInfo m_caif;
    VehicleWnd::VehicleType m_vehicleType;
    int m_vehicleId;
    CharacteristicsWnd(CharacteristicsWnd const&);
    CharacteristicsWnd();

public:
    virtual ~CharacteristicsWnd() override;
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetRtClass() const;
    static m3d::Class m_classCharacteristicsWnd;
};
