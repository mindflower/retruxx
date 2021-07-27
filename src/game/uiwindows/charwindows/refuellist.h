#pragma once

class RefuelButton :  public AdvancedButton
{
public:
    virtual class m3d::Object * Clone();
    static class m3d::Object * __fastcall CreateObject();
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual ~RefuelButton();
    virtual struct m3d::Class * GetClass() const ;
protected:
    RefuelButton();
    RefuelButton(class RefuelButton const &);
    virtual CStr const & GetStrIdTooltipBuy() const ;
    virtual CStr const & GetStrIdTooltipBuyFull() const ;
    virtual CStr GetServiceName() const ;
    virtual float GetPriceForOneUnit() const ;
    virtual class m3d::rend::TexHandle GetServiceIco() const ;
    virtual CStr const & GetStrIdTooltipBuyPartial() const ;
    virtual CStr const & GetStrIdTooltipBuyNotNeed() const ;
    virtual int GetMaxUnitsToBuy() const ;
    virtual CStr const & GetStrIdTooltipBuyUnavailable() const ;
private:
};

class RefuelList :  public AdvancedList
{
public:
    virtual struct m3d::Class * GetClass() const ;
    virtual class m3d::Object * Clone();
    virtual ~RefuelList();
    static class m3d::Object * __fastcall CreateObject();
    static struct m3d::Class * __fastcall GetBaseClass();
protected:
    virtual class AdvancedButton * NewItem() const ;
    RefuelList(class RefuelList const &);
    RefuelList();
    virtual class std::vector<int,class std::allocator<int> > GetObjIds() const ;
    virtual void BuyService(class AdvancedButton const *);
private:
};
