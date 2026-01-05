#pragma once
#include "cblist.h"

class CabinButton :  public CBButton
{
public:
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    static m3d::Class * GetBaseClass();
    virtual m3d::Object * Clone();
    virtual ~CabinButton();
protected:
    CabinButton(CabinButton const &);
    CabinButton();
    virtual int FullUpdate();
    virtual int CreateChildren();

public:
    RT_CLASS_DECLARE(CabinButton);

private:
    m3d::ui::Wnd *m_wndSpeed;
    m3d::ui::Wnd *m_wndTorque;
    m3d::ui::Wnd *m_wndControl;
};

class CabinList :  public CBList
{
public:
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    virtual m3d::Object * Clone();
    virtual ~CabinList();
    static m3d::Class * GetBaseClass();
protected:
    CabinList(CabinList const &);
    CabinList();
    virtual int GetCBResourceId() const ;
    virtual CBButton * CreateItem() const ;
    virtual void PostTriggerEventOnBuyCb();

public:
    RT_CLASS_DECLARE(CabinList);
};
