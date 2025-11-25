#include "itemwnd.h"

RT_CLASS_EXPORTS_BEGIN(ItemWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ItemWnd);

ItemWnd::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::GeomRepositoryItem ItemWnd::GetAsRepositoryItem() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemWnd::SetVehicleId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemWnd::CreateFromPattern(m3d::ui::Wnd const*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* ItemWnd::RepositoryClone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemWnd::GetDrawStyle() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* ItemWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(DragDropItemsWnd);
}

void ItemWnd::SetDrawStyle(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ItemWnd::~ItemWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Obj* ItemWnd::GetItem() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* ItemWnd::GetRtClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemWnd::Create(CStr const&, unsigned, BoundsBase<float> const&, unsigned)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemWnd::DrawSelection(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemWnd::CreateDragSlotFromWndPt(PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemWnd::DeactivateItem()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemWnd::UpdateItemValue()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemWnd::DrawItemIco(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemWnd::OnDistantEndDrag()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemWnd::DrawFrame(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemWnd::InitTextures()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemWnd::OnMouseOut()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemWnd::OnUpdateWhileDrag(PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ItemWnd::IsValid() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemWnd::UpdateItemIco()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemWnd::OnMouseButton1(unsigned, PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemWnd::ShowVideoWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemWnd::HideVideoWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ItemWnd::IsEmpty() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemWnd::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

bool ItemWnd::IsHidden() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Vehicle* ItemWnd::GetVehicle() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemWnd::OnUpdateWhileNoDrag(PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemWnd::GetItemPrototypeId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemWnd::SetupInfoWnd(ItemInfoWnd*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemWnd::RepositoryDeactivateItem()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemWnd::ShowDragSrc()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemWnd::HideDragSrc()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemWnd::OnDragOut()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemWnd::ActivateItem(ai::GeomRepositoryItem const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ItemWnd::NeedUpdateInfoWnd(ItemInfoWnd*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemWnd::StartDrag()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::GeomRepositoryItem ItemWnd::GetItemFromOrigin(PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ItemWnd::CanChangeItem() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemWnd::OnMouseIn()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemWnd::DrawItemInfo(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

BoundsBase<float> ItemWnd::GeomToWndBounds(BoundsBase<int> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ItemWnd::ItemWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemWnd::OnDistantStartDrag()
{
    RETRUXX_NOT_IMPLEMENTED;
}

PointBase<int> ItemWnd::GetCorrectGeomSize() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemWnd::Reset()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemWnd::DrawBg(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemWnd::OnAfterRemoveFromWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemWnd::OnDistantMouseIn(ai::GeomRepositoryItem const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemWnd::OnDragRemove()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemWnd::OnItemChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ItemWnd::CanContainItem() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemWnd::GiveUpItem(ai::GeomRepositoryItem const&, m3d::ui::Wnd*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemWnd::DrawGrid(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ItemWnd::IsDisabled() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemWnd::OnDistantMouseOut()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemWnd::GetItemObjId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemWnd::ShowInfoWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::rend::TexHandle ItemWnd::GetItemIco(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemWnd::LaunchEventOnItemDeactivation() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ItemWnd::CanAddDragItemDistant(ai::GeomRepositoryItem const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemWnd::HideInfoWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemWnd::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}
