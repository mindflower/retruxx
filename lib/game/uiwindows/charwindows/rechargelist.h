#pragma once
#include "advancedlist.h"

class RechargeList : public AdvancedList
{
public:
    static m3d::Class* __fastcall GetBaseClass();
    static m3d::Object* __fastcall CreateObject();
    virtual m3d::Object* Clone() override;
    virtual m3d::Class* GetClass() const override;
    virtual ~RechargeList() override;

protected:
    RechargeList();
    RechargeList(RechargeList const& rhs);
    virtual AdvancedButton* NewItem() const override /* 0x12c */;
    virtual std::vector<int, std::allocator<int>> GetObjIds() const override /* 0x13c */;
    virtual void BuyService(AdvancedButton const* btn) override /* 0x120 */;

public:
    RT_CLASS_DECLARE(RechargeList);
}; /* size: 0x0248 */

class RechargeButton : public AdvancedButton
{
    struct RchrAuxInfo
    {
        /* 0x0000 */ CStr m_strIdTooltipRecharge;
        /* 0x000c */ CStr m_strIdTooltipRechargeNotNeed;
        /* 0x0018 */ CStr m_strIdTooltipRechargeUnavailable;
        /* 0x0024 */ CStr m_strIdTooltipRechargePartial;
        /* 0x0030 */ CStr m_strIdTooltipRechargeFull;
        RchrAuxInfo(RechargeButton::RchrAuxInfo const& rhs);
        RchrAuxInfo();
    }; /* size: 0x003c */

public:
    static m3d::Class* __fastcall GetBaseClass();
    static m3d::Object* __fastcall CreateObject();
    virtual m3d::Object* Clone() override;
    virtual m3d::Class* GetClass() const override;
    virtual ~RechargeButton() override;

protected:
    RechargeButton(RechargeButton const& rhs);
    RechargeButton();
    virtual int GetMaxUnitsToBuy() const override /* 0x154 */;
    virtual CStr GetServiceName() const override /* 0x168 */;
    virtual CStr const& GetStrIdTooltipBuyNotNeed() const override /* 0x17c */;
    virtual float GetPriceForOneUnit() const override /* 0x160 */;
    virtual CStr const& GetStrIdTooltipBuyUnavailable() const override /* 0x180 */;
    virtual CStr const& GetStrIdTooltipBuyPartial() const override /* 0x184 */;
    virtual CStr const& GetStrIdTooltipBuy() const override /* 0x178 */;
    virtual CStr const& GetStrIdTooltipBuyFull() const override /* 0x188 */;
    virtual m3d::rend::TexHandle GetServiceIco() const override /* 0x16c */;

    static inline RechargeButton::RchrAuxInfo m_rchrAif;

public:
    RT_CLASS_DECLARE(RechargeButton);
}; /* size: 0x0280 */
