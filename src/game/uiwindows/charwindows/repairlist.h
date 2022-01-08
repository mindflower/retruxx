#pragma once

class RepairList :  public AdvancedList
{
public:
    virtual class m3d::Object * Clone();
    static class m3d::Object * CreateObject();
    static struct m3d::Class * GetBaseClass();
    virtual ~RepairList();
    virtual struct m3d::Class * GetClass() const ;
protected:
    RepairList(class RepairList const &);
    RepairList();
    virtual void BuyService(class AdvancedButton const *);
    virtual class AdvancedButton * NewItem() const ;
    virtual class std::vector<int,class std::allocator<int> > GetObjIds() const ;
private:
};

class RepairButton :  public AdvancedButton
{
public:
    virtual struct m3d::Class * GetClass() const ;
    virtual ~RepairButton();
    virtual class m3d::Object * Clone();
    static class m3d::Object * CreateObject();
    static struct m3d::Class * GetBaseClass();
protected:
    virtual class m3d::rend::TexHandle GetServiceIco() const ;
    RepairButton();
    RepairButton(class RepairButton const &);
    virtual CStr const & GetStrIdTooltipBuyNotNeed() const ;
    virtual CStr const & GetStrIdTooltipBuyPartial() const ;
    virtual int GetMaxUnitsToBuy() const ;
    virtual CStr const & GetStrIdTooltipBuyUnavailable() const ;
    virtual CStr const & GetStrIdTooltipBuyFull() const ;
    virtual CStr const & GetStrIdTooltipBuy() const ;
    virtual CStr GetServiceName() const ;
    virtual float GetPriceForOneUnit() const ;
private:
};
