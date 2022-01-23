#pragma once
#include "advancedlist.h"

class RechargeList :  public AdvancedList
{
public:
    static m3d::Class * GetBaseClass();
    static m3d::Object * CreateObject();
    virtual m3d::Object * Clone();
    virtual m3d::Class * GetClass() const ;
    virtual ~RechargeList();

protected:
    RechargeList();
    RechargeList(RechargeList const &);
    virtual AdvancedButton * NewItem() const ;
    virtual std::vector<int, std::allocator<int> > GetObjIds() const ;
    virtual void BuyService(AdvancedButton const *);

public:
    RT_CLASS_DECLARE(RechargeList);
};

class RechargeButton :  public AdvancedButton
{
public:
    static m3d::Class * GetBaseClass();
    static m3d::Object * CreateObject();
    virtual m3d::Object * Clone();
    virtual m3d::Class * GetClass() const ;
    virtual ~RechargeButton();

protected:
    RechargeButton(RechargeButton const &);
    RechargeButton();
    virtual int GetMaxUnitsToBuy() const ;
    virtual CStr GetServiceName() const ;
    virtual CStr const & GetStrIdTooltipBuyNotNeed() const ;
    virtual float GetPriceForOneUnit() const ;
    virtual CStr const & GetStrIdTooltipBuyUnavailable() const ;
    virtual CStr const & GetStrIdTooltipBuyPartial() const ;
    virtual CStr const & GetStrIdTooltipBuy() const ;
    virtual CStr const & GetStrIdTooltipBuyFull() const ;
    virtual m3d::rend::TexHandle GetServiceIco() const ;

public:
    RT_CLASS_DECLARE(RechargeButton);
};
