#pragma once
#include "cbcharacteristicswnd.h"

namespace ai
{
    class Cabin;
}

class CabinCharacteristicsWnd : public CBCharacteristicsWnd
{
public:
    void SetupForCabin(int cabinId);

    struct CabinAuxInfo
    {
        CStr m_wndMaxSpeedValName;
        CStr m_wndTorqueValName;
        CStr m_wndControlValName;
        CabinAuxInfo(const CabinCharacteristicsWnd::CabinAuxInfo&);
        CabinAuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    void SetupForCB(int cbId);
    virtual void UpdateCharacteristics() override;
    virtual void ClearCharacteristics() override;
    const ai::Cabin* GetCabin() const;
    m3d::ui::Wnd* m_wndMaxSpeedVal;
    m3d::ui::Wnd* m_wndTorqueVal;
    m3d::ui::Wnd* m_wndControlVal;
    CabinCharacteristicsWnd::CabinAuxInfo m_cabinAif;
    CabinCharacteristicsWnd();
    CabinCharacteristicsWnd(const CabinCharacteristicsWnd& rhs);

public:
    virtual ~CabinCharacteristicsWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classCabinCharacteristicsWnd;
};
