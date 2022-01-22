#pragma once
#include "itemwnd.h"

class VehiclePartWnd :  public ItemWnd
{
public:
    virtual ~VehiclePartWnd();
    virtual m3d::Object * Clone();
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
    virtual int GetResourceId() const ;
    virtual void SetVehicleId(int);
    virtual m3d::Class * GetClass() const ;
    virtual void SetPartName(CStr const &);

protected:
    virtual bool NeedUpdateInfoWnd(ItemInfoWnd *) const ;
    virtual void LaunchEventOnItemDeactivation() const ;
    VehiclePartWnd();
    VehiclePartWnd(VehiclePartWnd const &);
    virtual int GameDataUpdate(void *,int);
    virtual int OnPaint(m3d::ui::DrawInfo const &);
    void GetGunAngleEdges(std::set<m3d::ui::Edges> &) const ;
    virtual bool CanContainItem() const ;
    virtual int OnMouseButton1(unsigned int,class PointBase<float> const &);
    virtual bool IsValid() const ;
    void DrawGunAngleEdges(m3d::ui::DrawInfo const &);
    virtual bool CanChangeItem() const ;
    void DrawGunAngleEdge(m3d::ui::Edges,m3d::ui::DrawInfo const &);
    virtual void UpdateItemValue();
    virtual void DrawGrid(m3d::ui::DrawInfo const &);
    void UpdateGunAngleEdges();
    virtual bool SetItemObjId(int);
    void UpdateBg();
    virtual void SetupInfoWnd(ItemInfoWnd *);

public:
    RT_CLASS_DECLARE(VehiclePartWnd);

private:
    CStr m_partName;
    std::set<m3d::ui::Edges> m_gunAngleEdges;
};
