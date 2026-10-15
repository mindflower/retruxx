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
        CStr m_wndBasketCapacityValName;
        BasketAuxInfo(BasketCharacteristicsWnd::BasketAuxInfo const&);
        BasketAuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    void SetupForCB(int cbId);
    virtual void UpdateCharacteristics() override;
    virtual void ClearCharacteristics() override;
    ai::Basket const* GetBasket() const;
    m3d::ui::Wnd* m_wndBasketCapacityVal;
    BasketCharacteristicsWnd::BasketAuxInfo m_basketAif;
    BasketCharacteristicsWnd();
    BasketCharacteristicsWnd(BasketCharacteristicsWnd const& rhs);

public:
    virtual ~BasketCharacteristicsWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classBasketCharacteristicsWnd;
};
