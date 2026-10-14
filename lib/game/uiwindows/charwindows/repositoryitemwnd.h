#pragma once
#include "itemwnd.h"

class RepositoryItemWnd : public ItemWnd
{
public:
    void SetUpForRepositoryItem(ai::GeomRepositoryItem const& repositoryItem);
    virtual int CanAddDragItem(bool bSilent) override;

protected:
    virtual void SetVehicleId(int VehicleId) override;
    virtual ai::GeomRepositoryItem GetAsRepositoryItem() const override;
    virtual int GiveUpItem(ai::GeomRepositoryItem const& item, m3d::ui::Wnd* targetWnd) override;
    virtual bool SetItemObjId(int objId) override;
    virtual int AddItem(ai::GeomRepositoryItem const& item) override;
    virtual int DeactivateItem() override;
    virtual int ActivateItem(ai::GeomRepositoryItem const& item) override;
    virtual bool IsValid() const override;
    virtual int GetResourceId() const override;
    virtual void SetupInfoWnd(ItemInfoWnd* infoWnd) override;
    virtual bool NeedUpdateInfoWnd(ItemInfoWnd* infoWnd) const override;
    virtual void LaunchEventOnItemDeactivation() const override;
    virtual void UpdateItemValue() override;
    virtual int OnMouseButton1(unsigned int state, PointBase<float> const& at) override;
    virtual bool CanChangeItem() const override;

    ai::GeomRepository* m_repository;

    RepositoryItemWnd();
    RepositoryItemWnd(RepositoryItemWnd const& rhs);

public:
    virtual ~RepositoryItemWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(RepositoryItemWnd);
};
