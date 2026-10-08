#pragma once
#include "advancedlist.h"

class RepairButton : public AdvancedButton
{
    struct RprAuxInfo
    {
        /* 0x0000 */ CStr m_strIdTooltipRepair;
        /* 0x000c */ CStr m_strIdTooltipRepairNotNeed;
        /* 0x0018 */ CStr m_strIdTooltipRepairUnavailable;
        /* 0x0024 */ CStr m_strIdTooltipRepairPartial;
        /* 0x0030 */ CStr m_strIdTooltipRepairFull;
        RprAuxInfo(RepairButton::RprAuxInfo const& rhs);
        RprAuxInfo();
    }; /* size: 0x003c */

public:
    virtual m3d::Class* GetClass() const override;
    virtual ~RepairButton() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();

protected:
    virtual m3d::rend::TexHandle GetServiceIco() const override /* 0x16c */;
    RepairButton();
    RepairButton(RepairButton const& rhs);
    virtual CStr const& GetStrIdTooltipBuyNotNeed() const override /* 0x17c */;
    virtual CStr const& GetStrIdTooltipBuyPartial() const override /* 0x184 */;
    virtual int GetMaxUnitsToBuy() const override /* 0x154 */;
    virtual CStr const& GetStrIdTooltipBuyUnavailable() const override /* 0x180 */;
    virtual CStr const& GetStrIdTooltipBuyFull() const override /* 0x188 */;
    virtual CStr const& GetStrIdTooltipBuy() const override /* 0x178 */;
    virtual CStr GetServiceName() const override /* 0x168 */;
    virtual float GetPriceForOneUnit() const override /* 0x160 */;

    static inline RepairButton::RprAuxInfo m_rprAif;

public:
    RT_CLASS_DECLARE(RepairButton);
}; /* size: 0x0280 */

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
    virtual void BuyService(AdvancedButton const* btn) override /* 0x120 */;
    virtual AdvancedButton* NewItem() const override /* 0x12c */;
    virtual std::vector<int, std::allocator<int>> GetObjIds() const override /* 0x13c */;

public:
    RT_CLASS_DECLARE(RepairList);
}; /* size: 0x0248 */
