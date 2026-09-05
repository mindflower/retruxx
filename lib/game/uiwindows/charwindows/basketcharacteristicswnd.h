#pragma once
#include "cbcharacteristicswnd.h"

namespace ai
{
    class Basket;
}

class BasketCharacteristicsWnd : public CBCharacteristicsWnd
{
public:
    void SetupForBasket(int basketId);

    struct BasketAuxInfo
    {
        /* 0x0000 */ CStr m_wndBasketCapacityValName;
        BasketAuxInfo(const BasketCharacteristicsWnd::BasketAuxInfo&);
        BasketAuxInfo();
    }; /* size: 0x000c */

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    void SetupForCB(int cbId);
    virtual void UpdateCharacteristics() override /* 0x120 */;
    virtual void ClearCharacteristics() override /* 0x124 */;
    const ai::Basket* GetBasket() const;
    /* 0x02b4 */ m3d::ui::Wnd* m_wndBasketCapacityVal;
    /* 0x02b8 */ BasketCharacteristicsWnd::BasketAuxInfo m_basketAif;
    BasketCharacteristicsWnd();
    BasketCharacteristicsWnd(const BasketCharacteristicsWnd& rhs);

public:
    virtual ~BasketCharacteristicsWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    static m3d::Class m_classBasketCharacteristicsWnd;
}; /* size: 0x02c4 */
