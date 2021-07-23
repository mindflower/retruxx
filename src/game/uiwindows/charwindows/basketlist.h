#pragma once

class BasketList :  public CBList
{
public:
    virtual struct m3d::Class * GetClass() const ;
    virtual class m3d::Object * Clone();
    virtual ~BasketList();
    static class m3d::Object * __fastcall CreateObject();
    static struct m3d::Class * __fastcall GetBaseClass();
protected:
    virtual int GetCBResourceId() const ;
    BasketList();
    BasketList(class BasketList const &);
    virtual class CBButton * CreateItem() const ;
    virtual void PostTriggerEventOnBuyCb();
private:
};

class BasketButton :  public CBButton
{
public:
    virtual class m3d::Object * Clone();
    static class m3d::Object * __fastcall CreateObject();
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual struct m3d::Class * GetClass() const ;
    virtual ~BasketButton();
protected:
    BasketButton(class BasketButton const &);
    BasketButton();
    virtual int CreateChildren();
    virtual int FullUpdate();
private:
    m3d::ui::Wnd *m_wndRepositoryCapacity;
};
