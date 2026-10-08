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
        /* 0x0000 */ CStr m_wndMaxSpeedValName;
        /* 0x000c */ CStr m_wndTorqueValName;
        /* 0x0018 */ CStr m_wndControlValName;
        CabinAuxInfo(const CabinCharacteristicsWnd::CabinAuxInfo&);
        CabinAuxInfo();
    }; /* size: 0x0024 */

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    void SetupForCB(int cbId);
    virtual void UpdateCharacteristics() override /* 0x120 */;
    virtual void ClearCharacteristics() override /* 0x124 */;
    const ai::Cabin* GetCabin() const;
    /* 0x02b4 */ m3d::ui::Wnd* m_wndMaxSpeedVal;
    /* 0x02b8 */ m3d::ui::Wnd* m_wndTorqueVal;
    /* 0x02bc */ m3d::ui::Wnd* m_wndControlVal;
    /* 0x02c0 */ CabinCharacteristicsWnd::CabinAuxInfo m_cabinAif;
    CabinCharacteristicsWnd();
    CabinCharacteristicsWnd(const CabinCharacteristicsWnd& rhs);

public:
    virtual ~CabinCharacteristicsWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    static m3d::Class m_classCabinCharacteristicsWnd;
}; /* size: 0x02e4 */
