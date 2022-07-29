#include "dragdropitemswnd.h"

RT_CLASS_EXPORTS_BEGIN(DragDropItemsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(DragDropItemsWnd);

ItemAcceptInfo::ItemAcceptInfo(ItemAcceptInfo const&)
{
    throw std::logic_error("Not implemented");
}

GeomSlot::GeomSlot()
{
    throw std::logic_error("Not implemented");
}

void GeomSlot::SetDrawStyle(int)
{
    throw std::logic_error("Not implemented");
}

int GeomSlot::GetDrawStyle() const
{
    throw std::logic_error("Not implemented");
}

GeomSlot::~GeomSlot()
{
    throw std::logic_error("Not implemented");
}

void GeomSlot::SetItem(ai::GeomRepositoryItem const&)
{
    throw std::logic_error("Not implemented");
}

ai::GeomRepositoryItem& GeomSlot::GetItem()
{
    throw std::logic_error("Not implemented");
}

int GeomSlot::OnPaint(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

DragSlot::DragSlot(DragDropItemsWnd*)
{
    throw std::logic_error("Not implemented");
}

DragSlot::~DragSlot()
{
    throw std::logic_error("Not implemented");
}

int DragSlot::OnMouseButton0(unsigned, PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

int DragSlot::OnMouseMove(PointBase<float> const&, PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* DragDropItemsWnd::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* DragDropItemsWnd::GetRtClass() const
{
    throw std::logic_error("Not implemented");
}

DragDropItemsWnd::~DragDropItemsWnd()
{
    throw std::logic_error("Not implemented");
}

void DragDropItemsWnd::Enable(bool)
{
    throw std::logic_error("Not implemented");
}

ai::GeomRepositoryItem DragDropItemsWnd::GetDragItem()
{
    throw std::logic_error("Not implemented");
}

bool DragDropItemsWnd::IsDragging()
{
    throw std::logic_error("Not implemented");
}

void DragDropItemsWnd::RemoveDragSlot()
{
    throw std::logic_error("Not implemented");
}

int DragDropItemsWnd::OnDragRemove()
{
    throw std::logic_error("Not implemented");
}

void DragDropItemsWnd::Drag(PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

DragDropItemsWnd::DragDropItemsWnd()
{
    throw std::logic_error("Not implemented");
}

void DragDropItemsWnd::PlayDropSound()
{
    throw std::logic_error("Not implemented");
}

void DragDropItemsWnd::UpdateDragSlotSize()
{
    throw std::logic_error("Not implemented");
}

void DragDropItemsWnd::PlayStartDragSound()
{
    throw std::logic_error("Not implemented");
}

int DragDropItemsWnd::CreateDragSlotFromWndPt(PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

int DragDropItemsWnd::OnMouseIn()
{
    throw std::logic_error("Not implemented");
}

void DragDropItemsWnd::UpdateDragSlotPosition(PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

int DragDropItemsWnd::Drop(PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

int DragDropItemsWnd::OnMouseButton0(unsigned, PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

int DragDropItemsWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

int DragDropItemsWnd::OnBeforeRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

int DragDropItemsWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

int DragDropItemsWnd::OnMouseOut()
{
    throw std::logic_error("Not implemented");
}

int DragDropItemsWnd::StartDrag()
{
    throw std::logic_error("Not implemented");
}

int DragDropItemsWnd::OnMouseMove(PointBase<float> const&, PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}
