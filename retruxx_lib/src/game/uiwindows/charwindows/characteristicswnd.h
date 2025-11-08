#pragma once
#include "vehiclewnd.h"

class CharacteristicsWnd : public m3d::ui::Wnd
{
public:
    class CAuxInfo
    {
    public:
        CAuxInfo();

    private:
        CStr m_redColor;
        CStr m_greenColor;
    };

public:
    virtual ~CharacteristicsWnd(void);
    virtual m3d::Class* GetRtClass(void) const;
    void SetVehicleType(VehicleWnd::VehicleType);

protected:
    CStr GetDiffValPrefix(int, bool) const;
    CharacteristicsWnd(void);
    void UpdateValue(m3d::ui::Wnd*, int, int, bool);
    virtual int GameDataClear(bool);
    CStr GetColorByVal(int, int, bool) const;
    virtual int GameDataUpdate(void*, int);
    virtual void OnNewFrame(void);
    void UpdateValueHacked(m3d::ui::Wnd*, int, int, bool);
    void UpdateValueNormal(m3d::ui::Wnd*, int, int, bool);
    ai::Vehicle const* GetVehicle(void) const;

public:
    RT_CLASS_DECLARE(CharacteristicsWnd);
   
private:
    CharacteristicsWnd::CAuxInfo m_caif;
    VehicleWnd::VehicleType m_vehicleType;
    int m_vehicleId;
};
