#pragma once

class ComplexModelWnd :  public m3d::ui::SgNodeArrayWnd
{
public:
    static struct m3d::Class * GetBaseClass();
    void SetDefaultRotationAngleX(float);
    virtual class m3d::Object * Clone();
    float GetDefaultRotationAngleX() const ;
    int CreateFromPattern(class m3d::ui::Wnd *,bool);
    bool IsAllowedRotateByHandX() const ;
    bool IsAllowedRotateByHandY() const ;
    void SetRotationByHandVelocity(float);
    void AllowRotate(bool);
    float GetRotationVelocity() const ;
    class ai::Obj * GetObj() const ;
    static class m3d::Object * CreateObject();
    bool IsAllowedRotate() const ;
    float GetRotationByHandVelocity() const ;
    void SetRotationVelocity(float);
    virtual struct m3d::Class * GetClass() const ;
    void AllowRotateByHandY(bool);
    void AllowRotateByHandX(bool);
    void SetObjId(int);
    virtual ~ComplexModelWnd();
protected:
    void SetRotationByHandMode(bool);
    ComplexModelWnd();
    ComplexModelWnd(class ComplexModelWnd const &);
    void EmbraceCompoundGunAabb(class Aabb &,class ai::CompoundGun const *) const ;
    void UpdateCamera();
    void AddOneNode(class m3d::SgNode *,struct CMatrix const &,bool,bool);
    void AddOneNode(class m3d::SgNode *,struct CMatrix const &,struct Quaternion const &,bool);
    virtual int OnMouseMove(class PointBase<float> const &,class PointBase<float> const &);
    void SetupForVehicle(class ai::Vehicle *);
    void UpdateRotationAngle();
    void SetupForCompoundGun(class ai::CompoundGun *);
    virtual int GameDataClear(bool);
    virtual int OnMouseButton0(unsigned int,class PointBase<float> const &);
    virtual int GameDataUpdate(void *,int);
    int UpdateOnVehiclePartChanged();
    virtual int OnAfterRemoveFromWndStation();
    bool IsDisabled() const ;
    void SetupForGun(class ai::Gun *);
    virtual int OnBeforeAddToWndStation();
    struct CVector getGunSize(class ai::Gun *);
    int UpdateOnNewFrame();
    void SetupModel();
    void EmbraceGunAabb(class Aabb &,class ai::Gun const *) const ;
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
