#pragma once
#include "cblist.h"

class BasketList :  public CBList
{
public:
    virtual m3d::Class * GetClass() const ;
    virtual m3d::Object * Clone();
    virtual ~BasketList();
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
protected:
    virtual int GetCBResourceId() const ;
    BasketList();
    BasketList(BasketList const &);
    virtual CBButton * CreateItem() const ;
    virtual void PostTriggerEventOnBuyCb();

public:
    RT_CLASS_DECLARE(BasketList);
};

class BasketButton :  public CBButton
{
public:
    virtual m3d::Object * Clone();
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
    virtual m3d::Class * GetClass() const ;
    virtual ~BasketButton();
protected:
    BasketButton(BasketButton const &);
    BasketButton();
    virtual int CreateChildren();
    virtual int FullUpdate();

public:
    RT_CLASS_DECLARE(BasketButton);

private:
    m3d::ui::Wnd *m_wndRepositoryCapacity;
};
