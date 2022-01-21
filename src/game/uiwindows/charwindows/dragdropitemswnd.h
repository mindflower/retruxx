#pragma once
#include <server/geomrepositoryitem.h>
#include <ui/image.h>
#include <ui/ui.h>

class DragDropItemsWnd;

class ItemAcceptInfo
{
public:
    ItemAcceptInfo(ItemAcceptInfo const &);

private:
    m3d::ui::Wnd *m_eventSrcWnd;
    m3d::ui::Wnd *m_eventDstWnd;
    ai::GeomRepositoryItem m_item;
};

class GeomSlot : public m3d::ui::ImageWnd
{
public:
    GeomSlot();
    void SetDrawStyle(int);
    int GetDrawStyle() const;
    virtual ~GeomSlot();
    void SetItem(ai::GeomRepositoryItem const&);
    ai::GeomRepositoryItem& GetItem();

protected:
    virtual int OnPaint(m3d::ui::DrawInfo const&);

private:
    ai::GeomRepositoryItem m_item;
    int m_gsStyle;
};

class DragSlot :  public GeomSlot
{
public:
    DragSlot(DragDropItemsWnd *);
    virtual ~DragSlot();

protected:
    virtual int OnMouseButton0(unsigned int,class PointBase<float> const &);
    virtual int OnMouseMove(PointBase<float> const &,PointBase<float> const &);

private:
    DragDropItemsWnd *m_srcItemsWnd;
};

class DragDropItemsWnd : public m3d::ui::Wnd
{
public:
    enum DragStyle
    {
        DRAGSTYLE_VISIBLE_SRC = 0x0,
        DRAGSTYLE_HIDDEN_SRC = 0x1,
    };

public:
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetRtClass() const;
    virtual ~DragDropItemsWnd();
    virtual void Enable(bool);
    static ai::GeomRepositoryItem __fastcall GetDragItem();
    static bool __fastcall IsDragging();
    static void __fastcall RemoveDragSlot();

protected:
    virtual int OnDragRemove();
    virtual void Drag(PointBase<float> const&);
    DragDropItemsWnd();
    virtual void PlayDropSound();
    virtual void UpdateDragSlotSize();
    virtual void PlayStartDragSound();
    virtual int CreateDragSlotFromWndPt(PointBase<float> const&);
    virtual int OnMouseIn();
    virtual void UpdateDragSlotPosition(PointBase<float> const&);
    virtual int Drop(PointBase<float> const&);
    virtual int OnMouseButton0(unsigned int, PointBase<float> const&);
    virtual int OnWndNotify(m3d::ui::Wnd*, unsigned int, unsigned int, m3d::AIParam const&);
    virtual int OnBeforeRemoveFromWndStation();
    virtual int GameDataUpdate(void*, int);
    virtual int OnMouseOut();
    virtual int StartDrag();
    virtual int OnMouseMove(PointBase<float> const&, PointBase<float> const&);

public:
    RT_CLASS_INLINE_DECLARE(DragDropItemsWnd);

private:
    DragStyle m_dragStyle;
};
