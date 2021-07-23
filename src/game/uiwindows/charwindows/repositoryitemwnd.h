#pragma once

class RepositoryItemWnd :  public ItemWnd
{
public:
    virtual class m3d::Object * Clone();
    static class m3d::Object * __fastcall CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    void SetUpForRepositoryItem(struct ai::GeomRepositoryItem const &);
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual ~RepositoryItemWnd();
    virtual int CanAddDragItem(bool);
protected:
    virtual int GetResourceId() const ;
    virtual bool NeedUpdateInfoWnd(class ItemInfoWnd *) const ;
    virtual int DeactivateItem();
    virtual int OnMouseButton1(unsigned int,class PointBase<float> const &);
    RepositoryItemWnd();
    RepositoryItemWnd(class RepositoryItemWnd const &);
    virtual int ActivateItem(struct ai::GeomRepositoryItem const &);
    virtual void SetupInfoWnd(class ItemInfoWnd *);
    virtual int GiveUpItem(struct ai::GeomRepositoryItem const &,class m3d::ui::Wnd *);
    virtual bool SetItemObjId(int);
    virtual int AddItem(struct ai::GeomRepositoryItem const &);
    virtual void LaunchEventOnItemDeactivation() const ;
    virtual bool CanChangeItem() const ;
    virtual bool IsValid() const ;
    virtual struct ai::GeomRepositoryItem GetAsRepositoryItem() const ;
    virtual void SetVehicleId(int);
    virtual void UpdateItemValue();
private:
    SetItemObjId(int);
    AddItem(ai::GeomRepositoryItem const &);
    CanAddDragItem(bool);
    ai::GeomRepository *m_repository;
};
