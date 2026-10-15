#pragma once
#include "advancedlist.h"

class RechargeList : public AdvancedList
{
public:
    static m3d::Class* GetBaseClass();
    static m3d::Object* CreateObject();
    virtual m3d::Object* Clone() override;
    virtual m3d::Class* GetClass() const override;
    virtual ~RechargeList() override;

protected:
    RechargeList();
    RechargeList(RechargeList const& rhs);
    virtual AdvancedButton* NewItem() const override;
    virtual std::vector<int, std::allocator<int>> GetObjIds() const override;
    virtual void BuyService(AdvancedButton const* btn) override;

public:
    RT_CLASS_DECLARE(RechargeList);
};

class RechargeButton : public AdvancedButton
{
    struct RchrAuxInfo
    {
        CStr m_strIdTooltipRecharge;
        CStr m_strIdTooltipRechargeNotNeed;
        CStr m_strIdTooltipRechargeUnavailable;
        CStr m_strIdTooltipRechargePartial;
        CStr m_strIdTooltipRechargeFull;
        RchrAuxInfo(RechargeButton::RchrAuxInfo const& rhs);
        RchrAuxInfo();
    };

public:
    static m3d::Class* GetBaseClass();
    static m3d::Object* CreateObject();
    virtual m3d::Object* Clone() override;
    virtual m3d::Class* GetClass() const override;
    virtual ~RechargeButton() override;

protected:
    RechargeButton(RechargeButton const& rhs);
    RechargeButton();
    virtual int GetMaxUnitsToBuy() const override;
    virtual CStr GetServiceName() const override;
    virtual CStr const& GetStrIdTooltipBuyNotNeed() const override;
    virtual float GetPriceForOneUnit() const override;
    virtual CStr const& GetStrIdTooltipBuyUnavailable() const override;
    virtual CStr const& GetStrIdTooltipBuyPartial() const override;
    virtual CStr const& GetStrIdTooltipBuy() const override;
    virtual CStr const& GetStrIdTooltipBuyFull() const override;
    virtual m3d::rend::TexHandle GetServiceIco() const override;

    static inline RechargeButton::RchrAuxInfo m_rchrAif;

public:
    RT_CLASS_DECLARE(RechargeButton);
};
