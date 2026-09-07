#include "repositoryitemwnd.h"

RT_CLASS_EXPORTS_BEGIN(RepositoryItemWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RepositoryItemWnd);

m3d::Object* RepositoryItemWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* RepositoryItemWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* RepositoryItemWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryItemWnd::SetUpForRepositoryItem(ai::GeomRepositoryItem const& item)
{
    // RVA 0x46A260
    m_repository = item.m_parentRepository;
    SetItemObjId(item.GetObjId());
}

m3d::Class* RepositoryItemWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ItemWnd);
}

RepositoryItemWnd::~RepositoryItemWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryItemWnd::CanAddDragItem(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryItemWnd::GetResourceId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool RepositoryItemWnd::NeedUpdateInfoWnd(ItemInfoWnd*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryItemWnd::DeactivateItem()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryItemWnd::OnMouseButton1(unsigned, PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RepositoryItemWnd::RepositoryItemWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

RepositoryItemWnd::RepositoryItemWnd(RepositoryItemWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryItemWnd::ActivateItem(ai::GeomRepositoryItem const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryItemWnd::SetupInfoWnd(ItemInfoWnd*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryItemWnd::GiveUpItem(ai::GeomRepositoryItem const&, m3d::ui::Wnd*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool RepositoryItemWnd::SetItemObjId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryItemWnd::AddItem(ai::GeomRepositoryItem const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryItemWnd::LaunchEventOnItemDeactivation() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool RepositoryItemWnd::CanChangeItem() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool RepositoryItemWnd::IsValid() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::GeomRepositoryItem RepositoryItemWnd::GetAsRepositoryItem() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryItemWnd::SetVehicleId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryItemWnd::UpdateItemValue()
{
    RETRUXX_NOT_IMPLEMENTED;
}
