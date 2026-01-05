#include "complexmodelwnd.h"

RT_CLASS_EXPORTS_BEGIN(ComplexModelWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ComplexModelWnd);

m3d::Class* ComplexModelWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(SgNodeArrayWnd);
}

void ComplexModelWnd::SetDefaultRotationAngleX(float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* ComplexModelWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

float ComplexModelWnd::GetDefaultRotationAngleX() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ComplexModelWnd::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ComplexModelWnd::IsAllowedRotateByHandX() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ComplexModelWnd::IsAllowedRotateByHandY() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ComplexModelWnd::SetRotationByHandVelocity(float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ComplexModelWnd::AllowRotate(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

float ComplexModelWnd::GetRotationVelocity() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Obj* ComplexModelWnd::GetObj() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* ComplexModelWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ComplexModelWnd::IsAllowedRotate() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

float ComplexModelWnd::GetRotationByHandVelocity() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ComplexModelWnd::SetRotationVelocity(float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* ComplexModelWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ComplexModelWnd::AllowRotateByHandY(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ComplexModelWnd::AllowRotateByHandX(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ComplexModelWnd::SetObjId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ComplexModelWnd::~ComplexModelWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ComplexModelWnd::SetRotationByHandMode(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ComplexModelWnd::ComplexModelWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ComplexModelWnd::ComplexModelWnd(ComplexModelWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ComplexModelWnd::EmbraceCompoundGunAabb(Aabb&, ai::CompoundGun const*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ComplexModelWnd::UpdateCamera()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ComplexModelWnd::AddOneNode(m3d::SgNode*, CMatrix const&, bool, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ComplexModelWnd::AddOneNode(m3d::SgNode*, CMatrix const&, Quaternion const&, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ComplexModelWnd::OnMouseMove(PointBase<float> const&, PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ComplexModelWnd::SetupForVehicle(ai::Vehicle*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ComplexModelWnd::UpdateRotationAngle()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ComplexModelWnd::SetupForCompoundGun(ai::CompoundGun*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ComplexModelWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ComplexModelWnd::OnMouseButton0(unsigned, PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ComplexModelWnd::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

int ComplexModelWnd::UpdateOnVehiclePartChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ComplexModelWnd::OnAfterRemoveFromWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ComplexModelWnd::IsDisabled() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ComplexModelWnd::SetupForGun(ai::Gun*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ComplexModelWnd::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CVector ComplexModelWnd::getGunSize(ai::Gun*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ComplexModelWnd::UpdateOnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ComplexModelWnd::SetupModel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ComplexModelWnd::EmbraceGunAabb(Aabb&, ai::Gun const*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}
