#pragma once
#include "advancedlist.h"

class RefuelButton :  public AdvancedButton
{
public:
    virtual m3d::Object * Clone();
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
    virtual ~RefuelButton();
    virtual m3d::Class * GetClass() const ;

protected:
    RefuelButton();
    RefuelButton(RefuelButton const &);
    virtual CStr const & GetStrIdTooltipBuy() const ;
    virtual CStr const & GetStrIdTooltipBuyFull() const ;
    virtual CStr GetServiceName() const ;
    virtual float GetPriceForOneUnit() const ;
    virtual m3d::rend::TexHandle GetServiceIco() const ;
    virtual CStr const & GetStrIdTooltipBuyPartial() const ;
    virtual CStr const & GetStrIdTooltipBuyNotNeed() const ;
    virtual int GetMaxUnitsToBuy() const ;
    virtual CStr const & GetStrIdTooltipBuyUnavailable() const ;

public:
    RT_CLASS_DECLARE(RefuelButton);
};

class RefuelList :  public AdvancedList
{
public:
    virtual m3d::Class * GetClass() const ;
    virtual m3d::Object * Clone();
    virtual ~RefuelList();
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();

protected:
    virtual AdvancedButton * NewItem() const ;
    RefuelList(RefuelList const &);
    RefuelList();
    virtual std::vector<int, std::allocator<int> > GetObjIds() const ;
    virtual void BuyService(AdvancedButton const *);

public:
    RT_CLASS_DECLARE(RefuelList);
};
