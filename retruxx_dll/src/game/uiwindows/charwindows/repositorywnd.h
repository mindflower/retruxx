#pragma once
#include "dragdropitemswnd.h"
#include <map>

enum SlotState
{
    SLOTSTATE_UNSELECTED = 0x0,
    SLOTSTATE_SELECTED = 0x1,
    SLOTSTATE_SUITABLE = 0x2,
    SLOTSTATE_UNSUITABLE = 0x3,
    SLOTSTATE_OWN_PLACE_IN_HIDDEN_MODE = 0x4,
    SLOTSTATE_SUITABLE_DISTANT = 0x5,
    SLOTSTATE_NUM_SLOTSTATES = 0x6,
};

class RepositoryWnd :  public DragDropItemsWnd
{
public:
    class CellBunch
    {
    public:
        ~CellBunch();
        void Reset();
        CellBunch();

    private:
        BoundsBase<int> m_bounds;
        SlotState m_state;
    };

    class TwinkleInfo
    {
    public:
        TwinkleInfo(int);
        void Invalidate();
        bool IsValid() const;

    private:
        int m_itemObjId;
        unsigned int m_startTime;
    };

    enum RepositoryType
    {
        REPOSITORYTYPE_PLAYER = 0x0,
        REPOSITORYTYPE_GROUND = 0x1,
        REPOSITORYTYPE_SHOP = 0x2,
        REPOSITORYTYPE_WORKSHOP_VEHICLE = 0x3,
    };

    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_cellTexNames[6];
        CStr m_unsuitableBgName;
        unsigned int m_twinklePeriod;
        unsigned int m_numTwinkles;
    };

public:
    PointBase<int> WndToGeomPt(PointBase<float> const &);
    void SetSpecialized(std::vector<int> const &);
    BoundsBase<float> const & GetMaxBounds() const ;
    virtual m3d::Object * Clone();
    virtual void Enable(bool);
    void SetCellSize(PointBase<float> const &);
    virtual BoundsBase<float> GeomToWndBounds(BoundsBase<int> const &);
    virtual int CreateScrollWnd();
    PointBase<int> GetGeomSize() const ;
    virtual void SetBounds(BoundsBase<float> const &,bool);
    void SetMaxBounds(BoundsBase<float> const &);
    virtual int CreateFromPattern(m3d::ui::Wnd const *,bool);
    PointBase<float> GetCellSize() const ;
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    int GetItemDrawStyleByResourceId(int) const ;
    RepositoryType GetRepositoryType() const ;
    virtual int CanAddDragItem(bool);
    static m3d::Class * GetBaseClass();
    BoundsBase<int> WndToGeomBounds(BoundsBase<float> const &);
    virtual ~RepositoryWnd();
    bool IsValid() const ;
    void SetItemDrawStyleByResourceId(int,int);
    void SetItemToTwinkle(int);
    int UpdateOnItemQuickDrop(ItemAcceptInfo const &);

protected:
    virtual void DrawBunch(m3d::ui::DrawInfo const &);
    void OnSlotIn(int);
    GeomSlot * GetSlotByItemObjId(int) const ;
    int ShowVideo(int);
    void UpdateCellBunchWhileNoDrag(PointBase<float> const &);
    void UpdateScrollWnd();
    virtual void SetSlotCompatibleWithVehicle(int,bool);
    void ScrollSlots();
    virtual void OnVehiclePartChanged(void *);
    void SetGeomSize(PointBase<int> const &);
    virtual void OnDragOut();
    virtual void DrawGrid(m3d::ui::DrawInfo const &);
    virtual int OnDragRemove();
    int HideVideo();
    virtual void OnPlayerMoneyChanged();
    virtual int StartDrag();
    virtual int OnBeforeAddToWndStation();
    virtual int OnAfterRemoveFromWndStation();
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int,m3d::AIParam const &);
    virtual void UpdateSlotsStylesCompatibleWithVehicle();
    virtual int GameDataUpdate(void *,int);
    virtual void OnUpdateWhileDrag(PointBase<float> const &);
    void UpdateCellBunchWhileDrag();
    float GetMaxSlotY();
    virtual int GameDataClear(bool);
    virtual bool IsItemCompatibleByResourceType(ai::GeomRepositoryItem const &) const ;
    void AddInfoToEncyclopaedia();
    void AdjustWndToCells();
    virtual void RemoveAllItems();
    virtual void UpdateSlotsStylesTooRich();
    virtual int OnMouseButton1(unsigned int,PointBase<float> const &);
    virtual int AddItem(ai::GeomRepositoryItem const &);
    virtual void DrawFrame(m3d::ui::DrawInfo const &);
    int HideInfo();
    void OnSlotOut();
    virtual void ShowDragSrc();
    virtual void TwinkleItem(TwinkleInfo &);
    bool CanWindowQuickDropToMe(m3d::ui::Wnd const *,ai::GeomRepositoryItem const &) const ;
    virtual void OnPlayerVehicleChanged();
    void OnRepositoryChanged(void *);
    GeomSlot * GetDraggingSlot() const ;
    virtual int OnPaint(m3d::ui::DrawInfo const &);
    virtual int GiveUpItem(ai::GeomRepositoryItem const &,m3d::ui::Wnd *);
    void OnNewFrame();
    int ShowInfo(int);
    virtual void HideDragSrc();
    virtual void SetSlotTooRich(int,bool);
    virtual void OnUpdateWhileNoDrag(PointBase<float> const &);
    virtual int CreateItems();
    virtual ai::GeomRepositoryItem GetItemFromOrigin(PointBase<float> const &);
    virtual void FullUpdate();
    void AdjustCellsToWnd();
    RepositoryWnd(RepositoryWnd const &);
    RepositoryWnd();
    virtual void DrawUnsuitableBg(m3d::ui::DrawInfo const &);
    virtual int OnMouseWheel(int,PointBase<float> const &);

public:
    RT_CLASS_DECLARE(RepositoryWnd);

private:
    bool m_isInited;
    PointBase<int> m_geomSize;
    std::vector<GeomSlot *> m_slots;
    PointBase<float> m_cellSize;
    BoundsBase<float> m_maxBounds;
    CellBunch m_curBunch;
    ai::GeomRepository *m_repository;
    m3d::rend::TexHandle m_cellTextures[6];
    m3d::rend::TexHandle m_unsuitableBg;
    RepositoryType m_type;
    AuxInfo m_aif;
    bool m_isSpecialized;
    std::vector<int> m_compatibleResourceIds;
    std::map<int,int> m_itemStyles;
    int m_defaultItemStyle;
    int m_prevOverroledSlotId;
    std::vector<TwinkleInfo> m_twinkleItems;
};
