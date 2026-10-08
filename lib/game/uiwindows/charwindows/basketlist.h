#pragma once
#include "cblist.h"

class BasketButton : public CBButton
{
    struct BasketAuxInfo
    {
        /* 0x0000 */ CStr m_wndRepositoryCapacityName;
        BasketAuxInfo(BasketButton::BasketAuxInfo const&);
        BasketAuxInfo();
    }; /* size: 0x000c */

protected:
    virtual int CreateChildren() override /* 0x120 */;
    virtual int FullUpdate() override /* 0x124 */;
    /* 0x0250 */ m3d::ui::Wnd* m_wndRepositoryCapacity;
    static BasketButton::BasketAuxInfo m_baif;
    BasketButton();
    BasketButton(BasketButton const& rhs);

public:
    virtual ~BasketButton() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    RT_CLASS_DECLARE(BasketButton);
}; /* size: 0x0254 */

class BasketList : public CBList
{
protected:
    virtual int GetCBResourceId() const override /* 0x11c */;
    virtual CBButton* CreateItem() const override /* 0x120 */;
    virtual void PostTriggerEventOnBuyCb() override /* 0x12c */;
    BasketList();
    BasketList(BasketList const& rhs);

public:
    virtual ~BasketList() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    RT_CLASS_DECLARE(BasketList);
}; /* size: 0x028c */
