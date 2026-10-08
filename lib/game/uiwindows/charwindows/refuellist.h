#pragma once
#include "advancedlist.h"

class RefuelButton : public AdvancedButton
{
    struct RflAuxInfo
    {
        /* 0x0000 */ CStr m_strIdTooltipRefuel;
        /* 0x000c */ CStr m_strIdTooltipRefuelNotNeed;
        /* 0x0018 */ CStr m_strIdTooltipRefuelUnavailable;
        /* 0x0024 */ CStr m_strIdTooltipRefuelPartial;
        /* 0x0030 */ CStr m_strIdTooltipRefuelFull;
        RflAuxInfo(RefuelButton::RflAuxInfo const& rhs);
        RflAuxInfo();
    }; /* size: 0x003c */

public:
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual ~RefuelButton() override;
    virtual m3d::Class* GetClass() const override;

protected:
    RefuelButton();
    RefuelButton(RefuelButton const& rhs);
    virtual CStr const& GetStrIdTooltipBuy() const override /* 0x178 */;
    virtual CStr const& GetStrIdTooltipBuyFull() const override /* 0x188 */;
    virtual CStr GetServiceName() const override /* 0x168 */;
    virtual float GetPriceForOneUnit() const override /* 0x160 */;
    virtual m3d::rend::TexHandle GetServiceIco() const override /* 0x16c */;
    virtual CStr const& GetStrIdTooltipBuyPartial() const override /* 0x184 */;
    virtual CStr const& GetStrIdTooltipBuyNotNeed() const override /* 0x17c */;
    virtual int GetMaxUnitsToBuy() const override /* 0x154 */;
    virtual CStr const& GetStrIdTooltipBuyUnavailable() const override /* 0x180 */;

    static inline RefuelButton::RflAuxInfo m_rflAif;

public:
    RT_CLASS_DECLARE(RefuelButton);
}; /* size: 0x0280 */

class RefuelList : public AdvancedList
{
public:
    virtual m3d::Class* GetClass() const override;
    virtual m3d::Object* Clone() override;
    virtual ~RefuelList() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();

protected:
    virtual AdvancedButton* NewItem() const override /* 0x12c */;
    RefuelList(RefuelList const& rhs);
    RefuelList();
    virtual std::vector<int, std::allocator<int>> GetObjIds() const override /* 0x13c */;
    virtual void BuyService(AdvancedButton const* btn) override /* 0x120 */;

public:
    RT_CLASS_DECLARE(RefuelList);
}; /* size: 0x0248 */
