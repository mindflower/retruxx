#pragma once
#include <math/aabb.h>
#include <ui/sgnodearraywnd.h>

namespace ai
{
    class Obj;
    class Vehicle;
    class CompoundGun;
    class Gun;
}  // namespace ai

class ComplexModelWnd : public m3d::ui::SgNodeArrayWnd
{
public:
    void SetObjId(int objId);
    int CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc);
    ai::Obj* GetObj() const;
    void AllowRotate(bool bAllow);
    bool IsAllowedRotate() const;
    void SetRotationVelocity(float velocity);
    float GetRotationVelocity() const;
    void AllowRotateByHandX(bool bAllow);
    void AllowRotateByHandY(bool bAllow);
    bool IsAllowedRotateByHandX() const;
    bool IsAllowedRotateByHandY() const;
    void SetRotationByHandVelocity(float velocity);
    float GetRotationByHandVelocity() const;
    void SetDefaultRotationAngleX(float angle);
    float GetDefaultRotationAngleX() const;

protected:
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual int OnBeforeAddToWndStation() override;
    virtual int OnAfterRemoveFromWndStation() override;
    virtual int OnMouseButton0(unsigned int state, PointBase<float> const& at) override;
    virtual int OnMouseMove(PointBase<float> const& pt, PointBase<float> const& deltas) override;
    void SetupModel();
    void AddOneNode(m3d::SgNode* node, CMatrix const& InvMatr, Quaternion const& rot, bool withChilds);
    void AddOneNode(m3d::SgNode* node, CMatrix const& InvMatr, bool useRotation, bool withChilds);
    CVector getGunSize(ai::Gun* gun);
    void UpdateCamera();
    int UpdateOnNewFrame();
    int UpdateOnVehiclePartChanged();
    void SetupForVehicle(ai::Vehicle* vehicle);
    void SetupForGun(ai::Gun* gun);
    bool IsDisabled() const;
    void SetupForCompoundGun(ai::CompoundGun* cvp);
    void UpdateRotationAngle();
    void EmbraceGunAabb(Aabb& aabb, ai::Gun const* gun) const;
    void EmbraceCompoundGunAabb(Aabb& aabb, ai::CompoundGun const* cgun) const;
    void SetRotationByHandMode(bool bState);

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

    ComplexModelWnd();
    ComplexModelWnd(ComplexModelWnd const& rhs);

public:
    virtual ~ComplexModelWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classComplexModelWnd;
};
