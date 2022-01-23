#pragma once
#include "advancedlist.h"

class RepairButton : public AdvancedButton
{
public:
    virtual m3d::Class* GetClass() const;
    virtual ~RepairButton();
    virtual m3d::Object* Clone();
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
protected:
    virtual m3d::rend::TexHandle GetServiceIco() const;
    RepairButton();
    RepairButton(RepairButton const&);
    virtual CStr const& GetStrIdTooltipBuyNotNeed() const;
    virtual CStr const& GetStrIdTooltipBuyPartial() const;
    virtual int GetMaxUnitsToBuy() const;
    virtual CStr const& GetStrIdTooltipBuyUnavailable() const;
    virtual CStr const& GetStrIdTooltipBuyFull() const;
    virtual CStr const& GetStrIdTooltipBuy() const;
    virtual CStr GetServiceName() const;
    virtual float GetPriceForOneUnit() const;

public:
    RT_CLASS_DECLARE(RepairButton);
};

class RepairList :  public AdvancedList
{
public:
    virtual m3d::Object * Clone();
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
    virtual ~RepairList();
    virtual m3d::Class * GetClass() const ;

protected:
    RepairList(RepairList const &);
    RepairList();
    virtual void BuyService(AdvancedButton const *);
    virtual AdvancedButton * NewItem() const ;
    virtual std::vector<int, std::allocator<int> > GetObjIds() const ;

public:
    RT_CLASS_DECLARE(RepairList);
};
