#include "dragdropitemswnd.h"

RT_CLASS_EXPORTS_BEGIN(DragDropItemsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(DragDropItemsWnd);

ItemAcceptInfo::ItemAcceptInfo(ItemAcceptInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

GeomSlot::GeomSlot()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GeomSlot::SetDrawStyle(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GeomSlot::GetDrawStyle() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

GeomSlot::~GeomSlot()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GeomSlot::SetItem(ai::GeomRepositoryItem const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::GeomRepositoryItem& GeomSlot::GetItem()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GeomSlot::OnPaint(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

DragSlot::DragSlot(DragDropItemsWnd*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

DragSlot::~DragSlot()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int DragSlot::OnMouseButton0(unsigned, PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int DragSlot::OnMouseMove(PointBase<float> const&, PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* DragDropItemsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* DragDropItemsWnd::GetRtClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

DragDropItemsWnd::~DragDropItemsWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void DragDropItemsWnd::Enable(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::GeomRepositoryItem DragDropItemsWnd::GetDragItem()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool DragDropItemsWnd::IsDragging()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void DragDropItemsWnd::RemoveDragSlot()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int DragDropItemsWnd::OnDragRemove()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void DragDropItemsWnd::Drag(PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

DragDropItemsWnd::DragDropItemsWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void DragDropItemsWnd::PlayDropSound()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void DragDropItemsWnd::UpdateDragSlotSize()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void DragDropItemsWnd::PlayStartDragSound()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int DragDropItemsWnd::CreateDragSlotFromWndPt(PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int DragDropItemsWnd::OnMouseIn()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void DragDropItemsWnd::UpdateDragSlotPosition(PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int DragDropItemsWnd::Drop(PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int DragDropItemsWnd::OnMouseButton0(unsigned, PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int DragDropItemsWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int DragDropItemsWnd::OnBeforeRemoveFromWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int DragDropItemsWnd::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

int DragDropItemsWnd::OnMouseOut()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int DragDropItemsWnd::StartDrag()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int DragDropItemsWnd::OnMouseMove(PointBase<float> const&, PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
