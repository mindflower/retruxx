#pragma once
#include "cbcharacteristicswnd.h"

namespace ai
{
    class Cabin;
}

class CabinCharacteristicsWnd :  public CBCharacteristicsWnd
{
public:
    class CabinAuxInfo
    {
    public:
        CabinAuxInfo();

    private:
        CStr m_wndMaxSpeedValName;
        CStr m_wndTorqueValName;
        CStr m_wndControlValName;
    };

public:
    void SetupForCabin(int);
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    virtual m3d::Object * Clone();
    static m3d::Class * GetBaseClass();
    virtual ~CabinCharacteristicsWnd();

protected:
    void SetupForCB(int);
    ai::Cabin const * GetCabin() const ;
    CabinCharacteristicsWnd();
    CabinCharacteristicsWnd(CabinCharacteristicsWnd const &);
    virtual void ClearCharacteristics();
    virtual void UpdateCharacteristics();
    virtual int GameDataSetup();

public:
    RT_CLASS_DECLARE(CabinCharacteristicsWnd);

private:
    m3d::ui::Wnd *m_wndMaxSpeedVal;
    m3d::ui::Wnd *m_wndTorqueVal;
    m3d::ui::Wnd *m_wndControlVal;
    CabinCharacteristicsWnd::CabinAuxInfo m_cabinAif;
};
