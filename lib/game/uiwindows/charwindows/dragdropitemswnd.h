#pragma once
#include <server/geomrepositoryitem.h>
#include <ui/image.h>
#include <ui/ui.h>

class DragDropItemsWnd;

class ItemAcceptInfo
{
    friend class DragDropItemsWnd;
    friend class ItemWnd;
    friend class RepositoryWnd;
    // ShopWnd re-points m_eventSrcWnd at whichever tab accepted the drop.
    friend class ShopWnd;
    // WareWnd reads both fields when answering a quick drop.
    friend class WareWnd;

public:
    ItemAcceptInfo(m3d::ui::Wnd* eventSrcWnd, m3d::ui::Wnd* eventDstWnd, ai::GeomRepositoryItem const& item);
    ItemAcceptInfo(ItemAcceptInfo const&);

private:
    m3d::ui::Wnd* m_eventSrcWnd;
    m3d::ui::Wnd* m_eventDstWnd;
    ai::GeomRepositoryItem m_item;
};

class GeomSlot : public m3d::ui::ImageWnd
{
    // RepositoryWnd owns the slots it creates and touches their item / draw-style
    // fields directly (matches the shipped game).
    friend class RepositoryWnd;
    // WareWnd inspects the dragged item to decide whether it may be dropped.
    friend class WareWnd;

public:
    struct AuxInfo
    {
        /* 0x0000 */ CStr m_unsuitableTexName;
        /* 0x000c */ CStr m_tooRichTexName;
        /* 0x0018 */ PointBase<float> m_icoSz;
        /* 0x0020 */ float m_space;
        AuxInfo();
    };

    GeomSlot();
    void SetDrawStyle(int);
    int GetDrawStyle() const;
    virtual ~GeomSlot();
    void SetItem(ai::GeomRepositoryItem const&);
    ai::GeomRepositoryItem& GetItem();

protected:
    virtual int OnPaint(m3d::ui::DrawInfo const&);

    static GeomSlot::AuxInfo m_aif;
    static m3d::rend::TexHandle m_unsuitableTex;
    static m3d::rend::TexHandle m_tooRichTex;

private:
    ai::GeomRepositoryItem m_item;
    int m_gsStyle;
};

class DragSlot : public GeomSlot
{
    friend class DragDropItemsWnd;
    friend class ItemWnd;
    friend class RepositoryWnd;

public:
    DragSlot(DragDropItemsWnd*);
    virtual ~DragSlot();

protected:
    virtual int OnMouseButton0(unsigned int, class PointBase<float> const&);
    virtual int OnMouseMove(PointBase<float> const&, PointBase<float> const&);

private:
    DragDropItemsWnd* m_srcItemsWnd;
};

class DragDropItemsWnd : public m3d::ui::Wnd
{
public:
    enum DragStyle
    {
        DRAGSTYLE_VISIBLE_SRC = 0,
        DRAGSTYLE_HIDDEN_SRC = 1,
    };

public:
    static bool __fastcall IsDragging();
    static ai::GeomRepositoryItem __fastcall GetDragItem();
    virtual BoundsBase<float> GeomToWndBounds(BoundsBase<int> const&) = 0 /* 0x11c */;
    virtual void Enable(bool needEnable) /* 0x120 */;
    static void __fastcall RemoveDragSlot();
    virtual int CanAddDragItem(bool) = 0 /* 0x124 */;

protected:
    virtual int OnMouseButton0(unsigned int state, PointBase<float> const& at) override /* 0xa4 */;
    virtual int OnMouseMove(PointBase<float> const& pt, PointBase<float> const& deltas) override /* 0xa0 */;
    virtual int OnMouseOut() override /* 0x9c */;
    virtual int OnMouseIn() override /* 0x98 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int OnBeforeRemoveFromWndStation() override /* 0x70 */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override /* 0xc8 */;
    virtual int CreateDragSlotFromWndPt(PointBase<float> const& wndPt) /* 0x128 */;
    virtual int StartDrag() /* 0x12c */;
    virtual void Drag(PointBase<float> const& mousePt) /* 0x130 */;
    virtual int Drop(PointBase<float> const& mousePt) /* 0x134 */;
    virtual void UpdateDragSlotSize() /* 0x138 */;
    virtual void UpdateDragSlotPosition(PointBase<float> const& mousePt) /* 0x13c */;
    virtual void PlayStartDragSound() /* 0x140 */;
    virtual void PlayDropSound() /* 0x144 */;
    virtual ai::GeomRepositoryItem GetItemFromOrigin(PointBase<float> const&) = 0 /* 0x148 */;
    virtual int AddItem(ai::GeomRepositoryItem const&) = 0 /* 0x14c */;
    virtual void OnUpdateWhileDrag(PointBase<float> const&) = 0 /* 0x150 */;
    virtual void OnUpdateWhileNoDrag(PointBase<float> const&) = 0 /* 0x154 */;
    virtual void OnDragOut() = 0 /* 0x158 */;
    virtual int OnDragRemove() = 0 /* 0x15c */;
    virtual int GiveUpItem(ai::GeomRepositoryItem const&, m3d::ui::Wnd*) = 0 /* 0x160 */;
    virtual void HideDragSrc() = 0 /* 0x164 */;
    virtual void ShowDragSrc() = 0 /* 0x168 */;
    static DragSlot* m_dragSlot;
    /* 0x0220 */ DragDropItemsWnd::DragStyle m_dragStyle;
    DragDropItemsWnd(DragDropItemsWnd const&);
    DragDropItemsWnd();

public:
    virtual ~DragDropItemsWnd() override /* 0x00 */;
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetRtClass() const /* 0x16c */;
    static m3d::Class m_classDragDropItemsWnd;
}; /* size: 0x0224 */
