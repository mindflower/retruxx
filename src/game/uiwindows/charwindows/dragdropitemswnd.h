#pragma once

class ItemAcceptInfo
{
public:
    ItemAcceptInfo(struct ItemAcceptInfo const &);
protected:
private:
    m3d::ui::Wnd *m_eventSrcWnd;
    m3d::ui::Wnd *m_eventDstWnd;
    ai::GeomRepositoryItem m_item;
};

class DragSlot :  public GeomSlot
{
public:
    DragSlot(class DragDropItemsWnd *);
    virtual ~DragSlot();
protected:
    virtual int OnMouseButton0(unsigned int,class PointBase<float> const &);
    virtual int OnMouseMove(class PointBase<float> const &,class PointBase<float> const &);
private:
    DragDropItemsWnd *m_srcItemsWnd;
};
