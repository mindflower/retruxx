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
    throw std::logic_error("Not implemented");
}

m3d::Object* ComplexModelWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

float ComplexModelWnd::GetDefaultRotationAngleX() const
{
    throw std::logic_error("Not implemented");
}

int ComplexModelWnd::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    throw std::logic_error("Not implemented");
}

bool ComplexModelWnd::IsAllowedRotateByHandX() const
{
    throw std::logic_error("Not implemented");
}

bool ComplexModelWnd::IsAllowedRotateByHandY() const
{
    throw std::logic_error("Not implemented");
}

void ComplexModelWnd::SetRotationByHandVelocity(float)
{
    throw std::logic_error("Not implemented");
}

void ComplexModelWnd::AllowRotate(bool)
{
    throw std::logic_error("Not implemented");
}

float ComplexModelWnd::GetRotationVelocity() const
{
    throw std::logic_error("Not implemented");
}

ai::Obj* ComplexModelWnd::GetObj() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* ComplexModelWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

bool ComplexModelWnd::IsAllowedRotate() const
{
    throw std::logic_error("Not implemented");
}

float ComplexModelWnd::GetRotationByHandVelocity() const
{
    throw std::logic_error("Not implemented");
}

void ComplexModelWnd::SetRotationVelocity(float)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ComplexModelWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

void ComplexModelWnd::AllowRotateByHandY(bool)
{
    throw std::logic_error("Not implemented");
}

void ComplexModelWnd::AllowRotateByHandX(bool)
{
    throw std::logic_error("Not implemented");
}

void ComplexModelWnd::SetObjId(int)
{
    throw std::logic_error("Not implemented");
}

ComplexModelWnd::~ComplexModelWnd()
{
    throw std::logic_error("Not implemented");
}

void ComplexModelWnd::SetRotationByHandMode(bool)
{
    throw std::logic_error("Not implemented");
}

ComplexModelWnd::ComplexModelWnd()
{
    throw std::logic_error("Not implemented");
}

ComplexModelWnd::ComplexModelWnd(ComplexModelWnd const&)
{
    throw std::logic_error("Not implemented");
}

void ComplexModelWnd::EmbraceCompoundGunAabb(Aabb&, ai::CompoundGun const*) const
{
    throw std::logic_error("Not implemented");
}

void ComplexModelWnd::UpdateCamera()
{
    throw std::logic_error("Not implemented");
}

void ComplexModelWnd::AddOneNode(m3d::SgNode*, CMatrix const&, bool, bool)
{
    throw std::logic_error("Not implemented");
}

void ComplexModelWnd::AddOneNode(m3d::SgNode*, CMatrix const&, Quaternion const&, bool)
{
    throw std::logic_error("Not implemented");
}

int ComplexModelWnd::OnMouseMove(PointBase<float> const&, PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

void ComplexModelWnd::SetupForVehicle(ai::Vehicle*)
{
    throw std::logic_error("Not implemented");
}

void ComplexModelWnd::UpdateRotationAngle()
{
    throw std::logic_error("Not implemented");
}

void ComplexModelWnd::SetupForCompoundGun(ai::CompoundGun*)
{
    throw std::logic_error("Not implemented");
}

int ComplexModelWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

int ComplexModelWnd::OnMouseButton0(unsigned, PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

int ComplexModelWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

int ComplexModelWnd::UpdateOnVehiclePartChanged()
{
    throw std::logic_error("Not implemented");
}

int ComplexModelWnd::OnAfterRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

bool ComplexModelWnd::IsDisabled() const
{
    throw std::logic_error("Not implemented");
}

void ComplexModelWnd::SetupForGun(ai::Gun*)
{
    throw std::logic_error("Not implemented");
}

int ComplexModelWnd::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

CVector ComplexModelWnd::getGunSize(ai::Gun*)
{
    throw std::logic_error("Not implemented");
}

int ComplexModelWnd::UpdateOnNewFrame()
{
    throw std::logic_error("Not implemented");
}

void ComplexModelWnd::SetupModel()
{
    throw std::logic_error("Not implemented");
}

void ComplexModelWnd::EmbraceGunAabb(Aabb&, ai::Gun const*) const
{
    throw std::logic_error("Not implemented");
}
