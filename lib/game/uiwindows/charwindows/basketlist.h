#pragma once
#include "cblist.h"

class BasketButton : public CBButton
{
    struct BasketAuxInfo
    {
        CStr m_wndRepositoryCapacityName;
        BasketAuxInfo(BasketButton::BasketAuxInfo const&);
        BasketAuxInfo();
    };

protected:
    virtual int CreateChildren() override;
    virtual int FullUpdate() override;
    m3d::ui::Wnd* m_wndRepositoryCapacity;
    static BasketButton::BasketAuxInfo m_baif;
    BasketButton();
    BasketButton(BasketButton const& rhs);

public:
    virtual ~BasketButton() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(BasketButton);
};

class BasketList : public CBList
{
protected:
    virtual int GetCBResourceId() const override;
    virtual CBButton* CreateItem() const override;
    virtual void PostTriggerEventOnBuyCb() override;
    BasketList();
    BasketList(BasketList const& rhs);

public:
    virtual ~BasketList() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(BasketList);
};
