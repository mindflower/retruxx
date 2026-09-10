#pragma once
#include "itemwnd.h"

class RepositoryItemWnd : public ItemWnd
{
public:
    void SetUpForRepositoryItem(ai::GeomRepositoryItem const& repositoryItem);
    virtual int CanAddDragItem(bool bSilent) override /* 0x124 */;

protected:
    virtual void SetVehicleId(int VehicleId) override /* 0x170 */;
    virtual ai::GeomRepositoryItem GetAsRepositoryItem() const override /* 0x178 */;
    virtual int GiveUpItem(ai::GeomRepositoryItem const& item, m3d::ui::Wnd* targetWnd) override /* 0x160 */;
    virtual bool SetItemObjId(int objId) override /* 0x194 */;
    virtual int AddItem(ai::GeomRepositoryItem const& item) override /* 0x14c */;
    virtual int DeactivateItem() override /* 0x1ac */;
    virtual int ActivateItem(ai::GeomRepositoryItem const& item) override /* 0x1a8 */;
    virtual bool IsValid() const override /* 0x1b0 */;
    virtual int GetResourceId() const override /* 0x17c */;
    virtual void SetupInfoWnd(ItemInfoWnd* infoWnd) override /* 0x180 */;
    virtual bool NeedUpdateInfoWnd(ItemInfoWnd* infoWnd) const override /* 0x184 */;
    virtual void LaunchEventOnItemDeactivation() const override /* 0x188 */;
    virtual void UpdateItemValue() override /* 0x18c */;
    virtual int OnMouseButton1(unsigned int state, PointBase<float> const& at) override;
    virtual bool CanChangeItem() const override /* 0x1b4 */;

    /* 0x02b0 */ ai::GeomRepository* m_repository;

    RepositoryItemWnd();
    RepositoryItemWnd(RepositoryItemWnd const& rhs);

public:
    virtual ~RepositoryItemWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(RepositoryItemWnd);
}; /* size: 0x02b4 */
