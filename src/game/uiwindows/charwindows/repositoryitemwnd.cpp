#include "repositoryitemwnd.h"

RT_CLASS_EXPORTS_BEGIN(RepositoryItemWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RepositoryItemWnd);

m3d::Object* RepositoryItemWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* RepositoryItemWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* RepositoryItemWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

void RepositoryItemWnd::SetUpForRepositoryItem(ai::GeomRepositoryItem const&)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* RepositoryItemWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ItemWnd);
}

RepositoryItemWnd::~RepositoryItemWnd()
{
    throw std::logic_error("Not implemented");
}

int RepositoryItemWnd::CanAddDragItem(bool)
{
    throw std::logic_error("Not implemented");
}

int RepositoryItemWnd::GetResourceId() const
{
    throw std::logic_error("Not implemented");
}

bool RepositoryItemWnd::NeedUpdateInfoWnd(ItemInfoWnd*) const
{
    throw std::logic_error("Not implemented");
}

int RepositoryItemWnd::DeactivateItem()
{
    throw std::logic_error("Not implemented");
}

int RepositoryItemWnd::OnMouseButton1(unsigned, PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

RepositoryItemWnd::RepositoryItemWnd()
{
    throw std::logic_error("Not implemented");
}

RepositoryItemWnd::RepositoryItemWnd(RepositoryItemWnd const&)
{
    throw std::logic_error("Not implemented");
}

int RepositoryItemWnd::ActivateItem(ai::GeomRepositoryItem const&)
{
    throw std::logic_error("Not implemented");
}

void RepositoryItemWnd::SetupInfoWnd(ItemInfoWnd*)
{
    throw std::logic_error("Not implemented");
}

int RepositoryItemWnd::GiveUpItem(ai::GeomRepositoryItem const&, m3d::ui::Wnd*)
{
    throw std::logic_error("Not implemented");
}

bool RepositoryItemWnd::SetItemObjId(int)
{
    throw std::logic_error("Not implemented");
}

int RepositoryItemWnd::AddItem(ai::GeomRepositoryItem const&)
{
    throw std::logic_error("Not implemented");
}

void RepositoryItemWnd::LaunchEventOnItemDeactivation() const
{
    throw std::logic_error("Not implemented");
}

bool RepositoryItemWnd::CanChangeItem() const
{
    throw std::logic_error("Not implemented");
}

bool RepositoryItemWnd::IsValid() const
{
    throw std::logic_error("Not implemented");
}

ai::GeomRepositoryItem RepositoryItemWnd::GetAsRepositoryItem() const
{
    throw std::logic_error("Not implemented");
}

void RepositoryItemWnd::SetVehicleId(int)
{
    throw std::logic_error("Not implemented");
}

void RepositoryItemWnd::UpdateItemValue()
{
    throw std::logic_error("Not implemented");
}
