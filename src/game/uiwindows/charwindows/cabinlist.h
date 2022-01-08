#pragma once

class CabinButton :  public CBButton
{
public:
    static class m3d::Object * CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    static struct m3d::Class * GetBaseClass();
    virtual class m3d::Object * Clone();
    virtual ~CabinButton();
protected:
    CabinButton(class CabinButton const &);
    CabinButton();
    virtual int FullUpdate();
    virtual int CreateChildren();
private:
    m3d::ui::Wnd *m_wndSpeed;
    m3d::ui::Wnd *m_wndTorque;
    m3d::ui::Wnd *m_wndControl;
};

class CabinList :  public CBList
{
public:
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * CreateObject();
    virtual class m3d::Object * Clone();
    virtual ~CabinList();
    static struct m3d::Class * GetBaseClass();
protected:
    CabinList(class CabinList const &);
    CabinList();
    virtual int GetCBResourceId() const ;
    virtual class CBButton * CreateItem() const ;
    virtual void PostTriggerEventOnBuyCb();
private:
};
