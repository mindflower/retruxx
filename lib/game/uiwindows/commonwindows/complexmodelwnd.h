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
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x00 */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x00 */;
    virtual int OnBeforeAddToWndStation() override /* 0x00 */;
    virtual int OnAfterRemoveFromWndStation() override /* 0x00 */;
    virtual int OnMouseButton0(unsigned int state, PointBase<float> const& at) override /* 0x00 */;
    virtual int OnMouseMove(PointBase<float> const& pt, PointBase<float> const& deltas) override /* 0x00 */;
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

    /* 0x02a4 */ int m_objId;
    /* 0x02a8 */ PointBase<float> m_rotationAngle;
    /* 0x02b0 */ float m_rotationVelocity;
    /* 0x02b4 */ float m_rotationByHandVelocity;
    /* 0x02b8 */ bool m_bAllowRotate;
    /* 0x02b9 */ bool m_bAllowRotateByHandX;
    /* 0x02ba */ bool m_bAllowRotateByHandY;
    /* 0x02bb */ bool m_bInRotationByHandMode;
    /* 0x02bc */ float m_defaultRotationAngleX;
    /* 0x02c0 */ Aabb m_aabb;

    ComplexModelWnd();
    ComplexModelWnd(ComplexModelWnd const& rhs);

public:
    virtual ~ComplexModelWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classComplexModelWnd;
}; /* size: 0x02d8 */
