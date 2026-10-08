#pragma once
#include "cblist.h"

class CabinButton : public CBButton
{
    struct CabinAuxInfo
    {
        /* 0x0000 */ CStr m_wndSpeedName;
        /* 0x000c */ CStr m_wndTorqueName;
        /* 0x0018 */ CStr m_wndControlName;
        CabinAuxInfo(CabinButton::CabinAuxInfo const&);
        CabinAuxInfo();
    }; /* size: 0x0024 */

protected:
    virtual int CreateChildren() override /* 0x120 */;
    virtual int FullUpdate() override /* 0x124 */;
    /* 0x0250 */ m3d::ui::Wnd* m_wndSpeed;
    /* 0x0254 */ m3d::ui::Wnd* m_wndTorque;
    /* 0x0258 */ m3d::ui::Wnd* m_wndControl;
    static CabinButton::CabinAuxInfo m_caif;
    CabinButton();
    CabinButton(CabinButton const& rhs);

public:
    virtual ~CabinButton() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    RT_CLASS_DECLARE(CabinButton);
}; /* size: 0x025c */

class CabinList : public CBList
{
protected:
    virtual int GetCBResourceId() const override /* 0x11c */;
    virtual CBButton* CreateItem() const override /* 0x120 */;
    virtual void PostTriggerEventOnBuyCb() override /* 0x12c */;
    CabinList();
    CabinList(CabinList const& rhs);

public:
    virtual ~CabinList() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    RT_CLASS_DECLARE(CabinList);
}; /* size: 0x028c */
