#pragma once
#include "cbcharacteristicswnd.h"

namespace ai
{
    class Basket;
}

class BasketCharacteristicsWnd :  public CBCharacteristicsWnd
{
public:
    class BasketAuxInfo
    {
    public:
        BasketAuxInfo();

    private:
        CStr m_wndBasketCapacityValName;
    };

public:
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
    virtual m3d::Object * Clone();
    void SetupForBasket(int);
    virtual ~BasketCharacteristicsWnd();
    virtual m3d::Class * GetClass() const ;

protected:
    void SetupForCB(int);
    virtual int GameDataSetup();
    virtual void UpdateCharacteristics();
    BasketCharacteristicsWnd(BasketCharacteristicsWnd const &);
    BasketCharacteristicsWnd();
    virtual void ClearCharacteristics();
    ai::Basket const * GetBasket() const ;

public:
    RT_CLASS_DECLARE(BasketCharacteristicsWnd);

private:
    m3d::ui::Wnd *m_wndBasketCapacityVal;
    BasketCharacteristicsWnd::BasketAuxInfo m_basketAif;
};
