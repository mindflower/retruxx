#include "itemwnd.h"

RT_CLASS_EXPORTS_BEGIN(ItemWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ItemWnd);

ItemWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

ai::GeomRepositoryItem ItemWnd::GetAsRepositoryItem() const
{
    throw std::logic_error("Not implemented");
}

void ItemWnd::SetVehicleId(int)
{
    throw std::logic_error("Not implemented");
}

int ItemWnd::CreateFromPattern(m3d::ui::Wnd const*, bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* ItemWnd::RepositoryClone()
{
    throw std::logic_error("Not implemented");
}

int ItemWnd::GetDrawStyle() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ItemWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(DragDropItemsWnd);
}

void ItemWnd::SetDrawStyle(int)
{
    throw std::logic_error("Not implemented");
}

ItemWnd::~ItemWnd()
{
    throw std::logic_error("Not implemented");
}

ai::Obj* ItemWnd::GetItem() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ItemWnd::GetRtClass() const
{
    throw std::logic_error("Not implemented");
}

int ItemWnd::Create(CStr const&, unsigned, BoundsBase<float> const&, unsigned)
{
    throw std::logic_error("Not implemented");
}

void ItemWnd::DrawSelection(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

int ItemWnd::CreateDragSlotFromWndPt(PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

int ItemWnd::DeactivateItem()
{
    throw std::logic_error("Not implemented");
}

void ItemWnd::UpdateItemValue()
{
    throw std::logic_error("Not implemented");
}

void ItemWnd::DrawItemIco(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

void ItemWnd::OnDistantEndDrag()
{
    throw std::logic_error("Not implemented");
}

void ItemWnd::DrawFrame(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

void ItemWnd::InitTextures()
{
    throw std::logic_error("Not implemented");
}

int ItemWnd::OnMouseOut()
{
    throw std::logic_error("Not implemented");
}

void ItemWnd::OnUpdateWhileDrag(PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

bool ItemWnd::IsValid() const
{
    throw std::logic_error("Not implemented");
}

void ItemWnd::UpdateItemIco()
{
    throw std::logic_error("Not implemented");
}

int ItemWnd::OnMouseButton1(unsigned, PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

int ItemWnd::ShowVideoWnd()
{
    throw std::logic_error("Not implemented");
}

int ItemWnd::HideVideoWnd()
{
    throw std::logic_error("Not implemented");
}

bool ItemWnd::IsEmpty() const
{
    throw std::logic_error("Not implemented");
}

int ItemWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

bool ItemWnd::IsHidden() const
{
    throw std::logic_error("Not implemented");
}

ai::Vehicle* ItemWnd::GetVehicle() const
{
    throw std::logic_error("Not implemented");
}

void ItemWnd::OnUpdateWhileNoDrag(PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

int ItemWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

int ItemWnd::GetItemPrototypeId() const
{
    throw std::logic_error("Not implemented");
}

void ItemWnd::SetupInfoWnd(ItemInfoWnd*)
{
    throw std::logic_error("Not implemented");
}

int ItemWnd::RepositoryDeactivateItem()
{
    throw std::logic_error("Not implemented");
}

void ItemWnd::ShowDragSrc()
{
    throw std::logic_error("Not implemented");
}

void ItemWnd::HideDragSrc()
{
    throw std::logic_error("Not implemented");
}

void ItemWnd::OnDragOut()
{
    throw std::logic_error("Not implemented");
}

int ItemWnd::ActivateItem(ai::GeomRepositoryItem const&)
{
    throw std::logic_error("Not implemented");
}

bool ItemWnd::NeedUpdateInfoWnd(ItemInfoWnd*) const
{
    throw std::logic_error("Not implemented");
}

int ItemWnd::StartDrag()
{
    throw std::logic_error("Not implemented");
}

ai::GeomRepositoryItem ItemWnd::GetItemFromOrigin(PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

bool ItemWnd::CanChangeItem() const
{
    throw std::logic_error("Not implemented");
}

int ItemWnd::OnMouseIn()
{
    throw std::logic_error("Not implemented");
}

void ItemWnd::DrawItemInfo(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

BoundsBase<float> ItemWnd::GeomToWndBounds(BoundsBase<int> const&)
{
    throw std::logic_error("Not implemented");
}

ItemWnd::ItemWnd()
{
    throw std::logic_error("Not implemented");
}

void ItemWnd::OnDistantStartDrag()
{
    throw std::logic_error("Not implemented");
}

PointBase<int> ItemWnd::GetCorrectGeomSize() const
{
    throw std::logic_error("Not implemented");
}

void ItemWnd::Reset()
{
    throw std::logic_error("Not implemented");
}

void ItemWnd::DrawBg(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

int ItemWnd::OnAfterRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

void ItemWnd::OnDistantMouseIn(ai::GeomRepositoryItem const&)
{
    throw std::logic_error("Not implemented");
}

int ItemWnd::OnDragRemove()
{
    throw std::logic_error("Not implemented");
}

void ItemWnd::OnItemChanged()
{
    throw std::logic_error("Not implemented");
}

bool ItemWnd::CanContainItem() const
{
    throw std::logic_error("Not implemented");
}

int ItemWnd::GiveUpItem(ai::GeomRepositoryItem const&, m3d::ui::Wnd*)
{
    throw std::logic_error("Not implemented");
}

void ItemWnd::DrawGrid(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

bool ItemWnd::IsDisabled() const
{
    throw std::logic_error("Not implemented");
}

void ItemWnd::OnDistantMouseOut()
{
    throw std::logic_error("Not implemented");
}

int ItemWnd::GetItemObjId() const
{
    throw std::logic_error("Not implemented");
}

int ItemWnd::ShowInfoWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::rend::TexHandle ItemWnd::GetItemIco(int) const
{
    throw std::logic_error("Not implemented");
}

void ItemWnd::LaunchEventOnItemDeactivation() const
{
    throw std::logic_error("Not implemented");
}

bool ItemWnd::CanAddDragItemDistant(ai::GeomRepositoryItem const&) const
{
    throw std::logic_error("Not implemented");
}

int ItemWnd::HideInfoWnd()
{
    throw std::logic_error("Not implemented");
}

int ItemWnd::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}
