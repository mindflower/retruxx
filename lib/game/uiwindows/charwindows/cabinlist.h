#pragma once
#include "cblist.h"

class CabinButton : public CBButton
{
    struct CabinAuxInfo
    {
        CStr m_wndSpeedName;
        CStr m_wndTorqueName;
        CStr m_wndControlName;
        CabinAuxInfo(CabinButton::CabinAuxInfo const&);
        CabinAuxInfo();
    };

protected:
    virtual int CreateChildren() override;
    virtual int FullUpdate() override;
    m3d::ui::Wnd* m_wndSpeed;
    m3d::ui::Wnd* m_wndTorque;
    m3d::ui::Wnd* m_wndControl;
    static CabinButton::CabinAuxInfo m_caif;
    CabinButton();
    CabinButton(CabinButton const& rhs);

public:
    virtual ~CabinButton() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(CabinButton);
};

class CabinList : public CBList
{
protected:
    virtual int GetCBResourceId() const override;
    virtual CBButton* CreateItem() const override;
    virtual void PostTriggerEventOnBuyCb() override;
    CabinList();
    CabinList(CabinList const& rhs);

public:
    virtual ~CabinList() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(CabinList);
};
