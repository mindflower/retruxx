#pragma once
#include <math/aabb.h>
#include <ui/sgnodearraywnd.h>

namespace ai
{
    class Obj;
    class Vehicle;
    class CompoundGun;
    class Gun;
}

class ComplexModelWnd :  public m3d::ui::SgNodeArrayWnd
{
public:
    static m3d::Class * GetBaseClass();
    void SetDefaultRotationAngleX(float);
    virtual m3d::Object * Clone();
    float GetDefaultRotationAngleX() const ;
    int CreateFromPattern(m3d::ui::Wnd *,bool);
    bool IsAllowedRotateByHandX() const ;
    bool IsAllowedRotateByHandY() const ;
    void SetRotationByHandVelocity(float);
    void AllowRotate(bool);
    float GetRotationVelocity() const ;
    ai::Obj * GetObj() const ;
    static m3d::Object * CreateObject();
    bool IsAllowedRotate() const ;
    float GetRotationByHandVelocity() const ;
    void SetRotationVelocity(float);
    virtual m3d::Class * GetClass() const ;
    void AllowRotateByHandY(bool);
    void AllowRotateByHandX(bool);
    void SetObjId(int);
    virtual ~ComplexModelWnd();

protected:
    void SetRotationByHandMode(bool);
    ComplexModelWnd();
    ComplexModelWnd(ComplexModelWnd const &);
    void EmbraceCompoundGunAabb(Aabb &, ai::CompoundGun const *) const ;
    void UpdateCamera();
    void AddOneNode(m3d::SgNode *, CMatrix const &,bool,bool);
    void AddOneNode(m3d::SgNode *, CMatrix const &, Quaternion const &,bool);
    virtual int OnMouseMove(PointBase<float> const &, PointBase<float> const &);
    void SetupForVehicle(ai::Vehicle *);
    void UpdateRotationAngle();
    void SetupForCompoundGun(ai::CompoundGun *);
    virtual int GameDataClear(bool);
    virtual int OnMouseButton0(unsigned int, PointBase<float> const &);
    virtual int GameDataUpdate(void *,int);
    int UpdateOnVehiclePartChanged();
    virtual int OnAfterRemoveFromWndStation();
    bool IsDisabled() const ;
    void SetupForGun(ai::Gun *);
    virtual int OnBeforeAddToWndStation();
    CVector getGunSize(ai::Gun *);
    int UpdateOnNewFrame();
    void SetupModel();
    void EmbraceGunAabb(Aabb &, ai::Gun const *) const ;

public:
    RT_CLASS_DECLARE(ComplexModelWnd);

private:
    int m_objId;
    PointBase<float> m_rotationAngle;
    float m_rotationVelocity;
    float m_rotationByHandVelocity;
    bool m_bAllowRotate;
    bool m_bAllowRotateByHandX;
    bool m_bAllowRotateByHandY;
    bool m_bInRotationByHandMode;
    float m_defaultRotationAngleX;
    Aabb m_aabb;
};
