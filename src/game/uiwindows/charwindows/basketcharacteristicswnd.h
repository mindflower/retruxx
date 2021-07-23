#pragma once

class BasketCharacteristicsWnd :  public CBCharacteristicsWnd
{
public:
    static class m3d::Object * __fastcall CreateObject();
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual class m3d::Object * Clone();
    void SetupForBasket(int);
    virtual ~BasketCharacteristicsWnd();
    virtual struct m3d::Class * GetClass() const ;
protected:
    void SetupForCB(int);
    virtual int GameDataSetup();
    virtual void UpdateCharacteristics();
    BasketCharacteristicsWnd(class BasketCharacteristicsWnd const &);
    BasketCharacteristicsWnd();
    virtual void ClearCharacteristics();
    class ai::Basket const * GetBasket() const ;
private:
    m3d::ui::Wnd *m_wndBasketCapacityVal;
    BasketCharacteristicsWnd::BasketAuxInfo m_basketAif;
};
