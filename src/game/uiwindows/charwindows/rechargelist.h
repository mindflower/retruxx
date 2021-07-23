#pragma once

class RechargeList :  public AdvancedList
{
public:
    static struct m3d::Class * __fastcall GetBaseClass();
    static class m3d::Object * __fastcall CreateObject();
    virtual class m3d::Object * Clone();
    virtual struct m3d::Class * GetClass() const ;
    virtual ~RechargeList();
protected:
    RechargeList();
    RechargeList(class RechargeList const &);
    virtual class AdvancedButton * NewItem() const ;
    virtual class std::vector<int,class std::allocator<int> > GetObjIds() const ;
    virtual void BuyService(class AdvancedButton const *);
private:
};

class RechargeButton :  public AdvancedButton
{
public:
    static struct m3d::Class * __fastcall GetBaseClass();
    static class m3d::Object * __fastcall CreateObject();
    virtual class m3d::Object * Clone();
    virtual struct m3d::Class * GetClass() const ;
    virtual ~RechargeButton();
protected:
    RechargeButton(class RechargeButton const &);
    RechargeButton();
    virtual int GetMaxUnitsToBuy() const ;
    virtual class CStr GetServiceName() const ;
    virtual class CStr const & GetStrIdTooltipBuyNotNeed() const ;
    virtual float GetPriceForOneUnit() const ;
    virtual class CStr const & GetStrIdTooltipBuyUnavailable() const ;
    virtual class CStr const & GetStrIdTooltipBuyPartial() const ;
    virtual class CStr const & GetStrIdTooltipBuy() const ;
    virtual class CStr const & GetStrIdTooltipBuyFull() const ;
    virtual class m3d::rend::TexHandle GetServiceIco() const ;
private:
};
