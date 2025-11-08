#pragma once
#include "itemwnd.h"

class RepositoryItemWnd :  public ItemWnd
{
public:
    virtual m3d::Object * Clone();
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    void SetUpForRepositoryItem(ai::GeomRepositoryItem const &);
    static m3d::Class * GetBaseClass();
    virtual ~RepositoryItemWnd();
    virtual int CanAddDragItem(bool);

protected:
    virtual int GetResourceId() const ;
    virtual bool NeedUpdateInfoWnd(ItemInfoWnd *) const ;
    virtual int DeactivateItem();
    virtual int OnMouseButton1(unsigned int, PointBase<float> const &);
    RepositoryItemWnd();
    RepositoryItemWnd(RepositoryItemWnd const &);
    virtual int ActivateItem(ai::GeomRepositoryItem const &);
    virtual void SetupInfoWnd(ItemInfoWnd *);
    virtual int GiveUpItem(ai::GeomRepositoryItem const &, m3d::ui::Wnd *);
    virtual bool SetItemObjId(int);
    virtual int AddItem(ai::GeomRepositoryItem const &);
    virtual void LaunchEventOnItemDeactivation() const ;
    virtual bool CanChangeItem() const ;
    virtual bool IsValid() const ;
    virtual ai::GeomRepositoryItem GetAsRepositoryItem() const ;
    virtual void SetVehicleId(int);
    virtual void UpdateItemValue();

public:
    RT_CLASS_DECLARE(RepositoryItemWnd);

private:
    ai::GeomRepository *m_repository;
};
