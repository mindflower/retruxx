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
        CStr m_unsuitableTexName;
        CStr m_tooRichTexName;
        PointBase<float> m_icoSz;
        float m_space;
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
    static bool IsDragging();
    static ai::GeomRepositoryItem GetDragItem();
    virtual BoundsBase<float> GeomToWndBounds(BoundsBase<int> const&) = 0;
    virtual void Enable(bool needEnable);
    static void RemoveDragSlot();
    virtual int CanAddDragItem(bool) = 0;

protected:
    virtual int OnMouseButton0(unsigned int state, PointBase<float> const& at) override;
    virtual int OnMouseMove(PointBase<float> const& pt, PointBase<float> const& deltas) override;
    virtual int OnMouseOut() override;
    virtual int OnMouseIn() override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int OnBeforeRemoveFromWndStation() override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override;
    virtual int CreateDragSlotFromWndPt(PointBase<float> const& wndPt);
    virtual int StartDrag();
    virtual void Drag(PointBase<float> const& mousePt);
    virtual int Drop(PointBase<float> const& mousePt);
    virtual void UpdateDragSlotSize();
    virtual void UpdateDragSlotPosition(PointBase<float> const& mousePt);
    virtual void PlayStartDragSound();
    virtual void PlayDropSound();
    virtual ai::GeomRepositoryItem GetItemFromOrigin(PointBase<float> const&) = 0;
    virtual int AddItem(ai::GeomRepositoryItem const&) = 0;
    virtual void OnUpdateWhileDrag(PointBase<float> const&) = 0;
    virtual void OnUpdateWhileNoDrag(PointBase<float> const&) = 0;
    virtual void OnDragOut() = 0;
    virtual int OnDragRemove() = 0;
    virtual int GiveUpItem(ai::GeomRepositoryItem const&, m3d::ui::Wnd*) = 0;
    virtual void HideDragSrc() = 0;
    virtual void ShowDragSrc() = 0;
    static DragSlot* m_dragSlot;
    DragDropItemsWnd::DragStyle m_dragStyle;
    DragDropItemsWnd(DragDropItemsWnd const&);
    DragDropItemsWnd();

public:
    virtual ~DragDropItemsWnd() override;
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetRtClass() const;
    static m3d::Class m_classDragDropItemsWnd;
};
