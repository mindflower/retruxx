#pragma once
#include "advancedlist.h"

class RefuelButton : public AdvancedButton
{
    struct RflAuxInfo
    {
        CStr m_strIdTooltipRefuel;
        CStr m_strIdTooltipRefuelNotNeed;
        CStr m_strIdTooltipRefuelUnavailable;
        CStr m_strIdTooltipRefuelPartial;
        CStr m_strIdTooltipRefuelFull;
        RflAuxInfo(RefuelButton::RflAuxInfo const& rhs);
        RflAuxInfo();
    };

public:
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual ~RefuelButton() override;
    virtual m3d::Class* GetClass() const override;

protected:
    RefuelButton();
    RefuelButton(RefuelButton const& rhs);
    virtual CStr const& GetStrIdTooltipBuy() const override;
    virtual CStr const& GetStrIdTooltipBuyFull() const override;
    virtual CStr GetServiceName() const override;
    virtual float GetPriceForOneUnit() const override;
    virtual m3d::rend::TexHandle GetServiceIco() const override;
    virtual CStr const& GetStrIdTooltipBuyPartial() const override;
    virtual CStr const& GetStrIdTooltipBuyNotNeed() const override;
    virtual int GetMaxUnitsToBuy() const override;
    virtual CStr const& GetStrIdTooltipBuyUnavailable() const override;

    static inline RefuelButton::RflAuxInfo m_rflAif;

public:
    RT_CLASS_DECLARE(RefuelButton);
};

class RefuelList : public AdvancedList
{
public:
    virtual m3d::Class* GetClass() const override;
    virtual m3d::Object* Clone() override;
    virtual ~RefuelList() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();

protected:
    virtual AdvancedButton* NewItem() const override;
    RefuelList(RefuelList const& rhs);
    RefuelList();
    virtual std::vector<int, std::allocator<int>> GetObjIds() const override;
    virtual void BuyService(AdvancedButton const* btn) override;

public:
    RT_CLASS_DECLARE(RefuelList);
};
