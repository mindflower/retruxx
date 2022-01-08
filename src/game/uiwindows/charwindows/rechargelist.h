#pragma once

class RechargeList :  public AdvancedList
{
public:
    static struct m3d::Class * GetBaseClass();
    static class m3d::Object * CreateObject();
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
    static struct m3d::Class * GetBaseClass();
    static class m3d::Object * CreateObject();
    virtual class m3d::Object * Clone();
    virtual struct m3d::Class * GetClass() const ;
    virtual ~RechargeButton();
protected:
    RechargeButton(class RechargeButton const &);
    RechargeButton();
    virtual int GetMaxUnitsToBuy() const ;
    virtual CStr GetServiceName() const ;
    virtual CStr const & GetStrIdTooltipBuyNotNeed() const ;
    virtual float GetPriceForOneUnit() const ;
    virtual CStr const & GetStrIdTooltipBuyUnavailable() const ;
    virtual CStr const & GetStrIdTooltipBuyPartial() const ;
    virtual CStr const & GetStrIdTooltipBuy() const ;
    virtual CStr const & GetStrIdTooltipBuyFull() const ;
    virtual class m3d::rend::TexHandle GetServiceIco() const ;
private:
};
