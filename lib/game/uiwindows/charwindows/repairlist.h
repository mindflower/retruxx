#pragma once
#include "advancedlist.h"

class RepairButton : public AdvancedButton
{
    struct RprAuxInfo
    {
        CStr m_strIdTooltipRepair;
        CStr m_strIdTooltipRepairNotNeed;
        CStr m_strIdTooltipRepairUnavailable;
        CStr m_strIdTooltipRepairPartial;
        CStr m_strIdTooltipRepairFull;
        RprAuxInfo(RepairButton::RprAuxInfo const& rhs);
        RprAuxInfo();
    };

public:
    virtual m3d::Class* GetClass() const override;
    virtual ~RepairButton() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();

protected:
    virtual m3d::rend::TexHandle GetServiceIco() const override;
    RepairButton();
    RepairButton(RepairButton const& rhs);
    virtual CStr const& GetStrIdTooltipBuyNotNeed() const override;
    virtual CStr const& GetStrIdTooltipBuyPartial() const override;
    virtual int GetMaxUnitsToBuy() const override;
    virtual CStr const& GetStrIdTooltipBuyUnavailable() const override;
    virtual CStr const& GetStrIdTooltipBuyFull() const override;
    virtual CStr const& GetStrIdTooltipBuy() const override;
    virtual CStr GetServiceName() const override;
    virtual float GetPriceForOneUnit() const override;

    static inline RepairButton::RprAuxInfo m_rprAif;

public:
    RT_CLASS_DECLARE(RepairButton);
};

class RepairList : public AdvancedList
{
public:
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual ~RepairList() override;
    virtual m3d::Class* GetClass() const override;

protected:
    RepairList(RepairList const& rhs);
    RepairList();
    virtual void BuyService(AdvancedButton const* btn) override;
    virtual AdvancedButton* NewItem() const override;
    virtual std::vector<int, std::allocator<int>> GetObjIds() const override;

public:
    RT_CLASS_DECLARE(RepairList);
};
