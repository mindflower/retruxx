#include "itemmodelwnd.h"

RT_CLASS_DEFINE(ItemModelWnd);

bool ItemModelWnd::IsAllowedRotateByHandX() const
{
    throw std::logic_error("Not implemented");
}

bool ItemModelWnd::IsAllowedRotateByHandY() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* ItemModelWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

void ItemModelWnd::AllowRotate(bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ItemModelWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModelWnd);
}

float ItemModelWnd::GetRotationVelocity() const
{
    throw std::logic_error("Not implemented");
}

ItemModelWnd::~ItemModelWnd()
{
    throw std::logic_error("Not implemented");
}

bool ItemModelWnd::IsAutosized() const
{
    throw std::logic_error("Not implemented");
}

void ItemModelWnd::SetRotationVelocity(float)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* ItemModelWnd::CreateObject()
{
    return new ItemModelWnd;
}

int ItemModelWnd::SetModelByName(CStr const&, unsigned, unsigned)
{
    throw std::logic_error("Not implemented");
}

int ItemModelWnd::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    throw std::logic_error("Not implemented");
}

void ItemModelWnd::SetRotationByHandVelocity(float)
{
    throw std::logic_error("Not implemented");
}

float ItemModelWnd::GetRotationByHandVelocity() const
{
    throw std::logic_error("Not implemented");
}

void ItemModelWnd::AllowRotateByHandX(bool)
{
    throw std::logic_error("Not implemented");
}

CVector const& ItemModelWnd::GetDefaultTranslation() const
{
    throw std::logic_error("Not implemented");
}

void ItemModelWnd::SetAutosized(bool)
{
    throw std::logic_error("Not implemented");
}

void ItemModelWnd::AllowRotateByHandY(bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ItemModelWnd::GetClass() const
{
    return RT_CLASS_LOCAL(ItemModelWnd);
}

void ItemModelWnd::SetDefaultRotationAngleX(float)
{
    throw std::logic_error("Not implemented");
}

void ItemModelWnd::SetDefaultTranslation(CVector const&)
{
    throw std::logic_error("Not implemented");
}

bool ItemModelWnd::IsAllowedRotate() const
{
    throw std::logic_error("Not implemented");
}

float ItemModelWnd::GetDefaultRotationAngleX() const
{
    throw std::logic_error("Not implemented");
}

int ItemModelWnd::OnAfterRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

void ItemModelWnd::UpdateCamera()
{
    throw std::logic_error("Not implemented");
}

bool ItemModelWnd::IsDisabled() const
{
    throw std::logic_error("Not implemented");
}

ItemModelWnd::ItemModelWnd(ItemModelWnd const&)
{
    throw std::logic_error("Not implemented");
}

ItemModelWnd::ItemModelWnd()
{
}

int ItemModelWnd::OnMouseButton0(unsigned, PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

bool ItemModelWnd::IsValid() const
{
    throw std::logic_error("Not implemented");
}

void ItemModelWnd::SetRotationByHandMode(bool)
{
    throw std::logic_error("Not implemented");
}

void ItemModelWnd::CalcAutosizeTranslation(CVector&) const
{
    throw std::logic_error("Not implemented");
}

int ItemModelWnd::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

int ItemModelWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

int ItemModelWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

int ItemModelWnd::OnMouseMove(PointBase<float> const&, PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

int ItemModelWnd::OnNewFrame()
{
    throw std::logic_error("Not implemented");
}

void ItemModelWnd::UpdateRotationAngle()
{
    throw std::logic_error("Not implemented");
}
