#pragma once

class VehiclePartWnd :  public ItemWnd
{
public:
    virtual ~VehiclePartWnd();
    virtual class m3d::Object * Clone();
    static class m3d::Object * __fastcall CreateObject();
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual int GetResourceId() const ;
    virtual void SetVehicleId(int);
    virtual struct m3d::Class * GetClass() const ;
    virtual void SetPartName(CStr const &);
protected:
    virtual bool NeedUpdateInfoWnd(class ItemInfoWnd *) const ;
    virtual void LaunchEventOnItemDeactivation() const ;
    VehiclePartWnd();
    VehiclePartWnd(class VehiclePartWnd const &);
    virtual int GameDataUpdate(void *,int);
    virtual int OnPaint(struct m3d::ui::DrawInfo const &);
    void GetGunAngleEdges(class std::set<enum m3d::ui::Edges,struct std::less<enum m3d::ui::Edges>,class std::allocator<enum m3d::ui::Edges> > &) const ;
    virtual bool CanContainItem() const ;
    virtual int OnMouseButton1(unsigned int,class PointBase<float> const &);
    virtual bool IsValid() const ;
    void DrawGunAngleEdges(struct m3d::ui::DrawInfo const &);
    virtual bool CanChangeItem() const ;
    void DrawGunAngleEdge(enum m3d::ui::Edges,struct m3d::ui::DrawInfo const &);
    virtual void UpdateItemValue();
    virtual void DrawGrid(struct m3d::ui::DrawInfo const &);
    void UpdateGunAngleEdges();
    virtual bool SetItemObjId(int);
    void UpdateBg();
    virtual void SetupInfoWnd(class ItemInfoWnd *);
private:
    CStr m_partName;
    std::set<enum m3d::ui::Edges> m_gunAngleEdges;
};
