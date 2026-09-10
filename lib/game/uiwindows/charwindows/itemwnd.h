#pragma once
#include "dragdropitemswnd.h"
#include "repositorywnd.h"

class ItemInfoWnd;

class ItemWnd : public DragDropItemsWnd
{
    // GadgetWnd and VehiclePartWnd read/write ItemWnd's item/geom/state/aif
    // fields directly (both are lean specialisations that reuse ItemWnd's
    // drawing + item plumbing).
    friend class GadgetWnd;
    friend class VehiclePartWnd;
    // RepositoryItemWnd re-derives m_itemId from its repository in
    // SetItemObjId / IsValid / GetAsRepositoryItem.
    friend class RepositoryItemWnd;

public:
    class AuxInfo
    {
        friend class ItemWnd;
        friend class GadgetWnd;
        friend class VehiclePartWnd;

    public:
        AuxInfo();

    private:
        CStr m_cellTexNames[6];
        CStr m_suitableDistantPane;
    };

public:
    virtual ai::GeomRepositoryItem GetAsRepositoryItem() const;
    virtual void SetVehicleId(int);
    virtual int CreateFromPattern(m3d::ui::Wnd const*, bool);
    virtual m3d::Object* RepositoryClone();
    int GetDrawStyle() const;
    static m3d::Class* GetBaseClass();
    void SetDrawStyle(int);
    virtual ~ItemWnd();
    ai::Obj* GetItem() const;
    virtual m3d::Class* GetRtClass() const;
    virtual int Create(CStr const&, unsigned int,  BoundsBase<float> const&, unsigned int);

protected:
    virtual void DrawSelection(m3d::ui::DrawInfo const&);
    virtual int CreateDragSlotFromWndPt(PointBase<float> const&);
    virtual int DeactivateItem();
    virtual void UpdateItemValue();
    virtual void DrawItemIco(m3d::ui::DrawInfo const&);
    virtual void OnDistantEndDrag();
    virtual void DrawFrame(m3d::ui::DrawInfo const&);
    virtual void InitTextures();
    virtual int OnMouseOut();
    virtual void OnUpdateWhileDrag(PointBase<float> const&);
    virtual bool IsValid() const;
    virtual void UpdateItemIco();
    virtual int OnMouseButton1(unsigned int, PointBase<float> const&);
    virtual int ShowVideoWnd();
    virtual int HideVideoWnd();
    virtual bool IsEmpty() const;
    virtual int GameDataUpdate(void*, int);
    virtual int CanAddDragItem(bool) override;
    virtual int AddItem(ai::GeomRepositoryItem const&) override;
    virtual int GetResourceId() const;
    bool IsHidden() const;
    class ai::Vehicle* GetVehicle() const;
    virtual void OnUpdateWhileNoDrag( PointBase<float> const&);
    virtual int OnPaint( m3d::ui::DrawInfo const&);
    virtual int GetItemPrototypeId() const;
    virtual void SetupInfoWnd(ItemInfoWnd*);
    virtual int RepositoryDeactivateItem();
    virtual void ShowDragSrc();
    virtual void HideDragSrc();
    virtual void OnDragOut();
    virtual int ActivateItem(ai::GeomRepositoryItem const&);
    virtual bool NeedUpdateInfoWnd(ItemInfoWnd*) const;
    virtual int StartDrag();
    virtual ai::GeomRepositoryItem GetItemFromOrigin(PointBase<float> const&);
    virtual bool CanChangeItem() const;
    virtual int OnMouseIn();
    virtual void DrawItemInfo(m3d::ui::DrawInfo const&);
    virtual BoundsBase<float> GeomToWndBounds(BoundsBase<int> const&);
    ItemWnd();
    virtual void OnDistantStartDrag();
    virtual class PointBase<int> GetCorrectGeomSize() const;
    virtual void Reset();
    virtual void DrawBg( m3d::ui::DrawInfo const&);
    virtual int OnAfterRemoveFromWndStation();
    virtual void OnDistantMouseIn(ai::GeomRepositoryItem const&);
    virtual int OnDragRemove();
    virtual void OnItemChanged();
    virtual bool CanContainItem() const;
    virtual int GiveUpItem(ai::GeomRepositoryItem const&, m3d::ui::Wnd*);
    virtual void DrawGrid(m3d::ui::DrawInfo const&);
    bool IsDisabled() const;
    virtual void OnDistantMouseOut();
    virtual int GetItemObjId() const;
    virtual bool SetItemObjId(int);
    virtual int ShowInfoWnd();
    virtual m3d::rend::TexHandle GetItemIco(int) const;
    virtual void LaunchEventOnItemDeactivation() const;
    virtual bool CanAddDragItemDistant(ai::GeomRepositoryItem const&) const;
    virtual int HideInfoWnd();
    virtual int OnBeforeAddToWndStation();

public:
    RT_CLASS_DECLARE(ItemWnd);

private:
    int m_vehicleId;
    m3d::rend::TexHandle m_itemIco;
    PointBase<int> m_geomSize;
    SlotState m_state;
    m3d::rend::TexHandle m_cellTextures[6];
    AuxInfo m_aif;
    bool m_isDragging;
    int m_drawStyle;
    int m_itemId;
};
