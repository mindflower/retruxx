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

class RepositoryWnd : public DragDropItemsWnd
{
    // SaleWnd::CreateTabItem snaps a freshly built repository tab to the cell
    // grid before handing it to the tab control.
    friend class SaleWnd;
    // WareWnd checks the source repository's type before taking a quick drop.
    friend class WareWnd;

public:
    struct CellBunch
    {
        CellBunch(RepositoryWnd::CellBunch const&);
        CellBunch();
        ~CellBunch();
        void Reset();
        BoundsBase<int> m_bounds;
        SlotState m_state;
    };

    enum ItemsWndAdjustFlag
    {
        ITEMSWND_ADJUST_NO = 0,
        ITEMSWND_ADJUST_WND = 1,
        ITEMSWND_ADJUST_CELLS = 2,
    };

    enum RepositoryType
    {
        REPOSITORYTYPE_PLAYER = 0,
        REPOSITORYTYPE_GROUND = 1,
        REPOSITORYTYPE_SHOP = 2,
        REPOSITORYTYPE_WORKSHOP_VEHICLE = 3,
    };

    struct AuxInfo
    {
        CStr m_cellTexNames[6];
        CStr m_unsuitableBgName;
        unsigned int m_twinklePeriod;
        unsigned int m_numTwinkles;
        AuxInfo(RepositoryWnd::AuxInfo const&);
        AuxInfo();
    };

public:
    virtual int CreateFromPattern(m3d::ui::Wnd const* patternWnd, bool deleteSrc);
    ai::GeomRepository* GetRepository() const;
    virtual int SetRepository(ai::GeomRepository* repository, RepositoryWnd::RepositoryType type);
    PointBase<int> GetGeomSize() const;
    PointBase<float> GetCellSize() const;
    void SetCellSize(PointBase<float> const& size);
    bool IsValid() const;
    PointBase<int> WndToGeomPt(PointBase<float> const& wndPt);
    virtual BoundsBase<float> GeomToWndBounds(BoundsBase<int> const& geomB) override;
    BoundsBase<int> WndToGeomBounds(BoundsBase<float> const& wndB);
    virtual void Enable(bool needEnable) override;
    virtual void SetBounds(BoundsBase<float> const& bounds, bool bUpdateBaseOrigin) override;
    void SetMaxBounds(BoundsBase<float> const& maxBounds);
    BoundsBase<float> const& GetMaxBounds() const;
    void SetSpecialized(std::vector<int, std::allocator<int>> const& compatibleRcIds);
    void SetItemDrawStyleByResourceId(int style, int rcId);
    int GetItemDrawStyleByResourceId(int rcId) const;
    RepositoryWnd::RepositoryType GetRepositoryType() const;
    virtual int CreateScrollWnd();
    virtual int CanAddDragItem(bool bSilent) override;
    int UpdateOnItemQuickDrop(ItemAcceptInfo const& info);
    void SetItemToTwinkle(int itemObjId);

    struct TwinkleInfo
    {
        int m_itemObjId;
        unsigned int m_startTime;
        TwinkleInfo(int itemObjId);
        void Invalidate();
        bool IsValid() const;
    };

protected:
    void SetGeomSize(PointBase<int> const& geomSize);
    void AdjustWndToCells();
    void AdjustCellsToWnd();
    virtual int CreateItems();
    virtual void RemoveAllItems();
    float GetMaxSlotY();
    void UpdateScrollWnd();
    void ScrollSlots();
    void OnRepositoryChanged(void* data);
    int ShowInfo(int itemId);
    int HideInfo();
    int ShowVideo(int itemId);
    int HideVideo();
    virtual void DrawBunch(m3d::ui::DrawInfo const& di);
    virtual void DrawGrid(m3d::ui::DrawInfo const& di);
    virtual void DrawFrame(m3d::ui::DrawInfo const& di);
    virtual void DrawUnsuitableBg(m3d::ui::DrawInfo const& di);
    bool CanWindowQuickDropToMe(m3d::ui::Wnd const* srcWnd, ai::GeomRepositoryItem const& item) const;
    GeomSlot* GetDraggingSlot() const;
    virtual void OnPlayerVehicleChanged();
    virtual void OnVehiclePartChanged(void* data);
    virtual void OnPlayerMoneyChanged();
    virtual void UpdateSlotsStylesCompatibleWithVehicle();
    virtual void UpdateSlotsStylesTooRich();
    virtual void SetSlotCompatibleWithVehicle(int slotIdx, bool bCompatible);
    virtual void SetSlotTooRich(int slotIdx, bool bTooRich);
    virtual void FullUpdate();
    virtual void TwinkleItem(RepositoryWnd::TwinkleInfo& twinkleInfo);
    void OnNewFrame();
    GeomSlot* GetSlotByItemObjId(int itemObjId) const;
    virtual int OnPaint(m3d::ui::DrawInfo const& di) override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override;
    virtual int OnMouseWheel(int ticks, PointBase<float> const& at) override;
    virtual int OnMouseButton1(unsigned int state, PointBase<float> const& at) override;
    virtual int OnBeforeAddToWndStation() override;
    virtual int OnAfterRemoveFromWndStation() override;
    void UpdateCellBunchWhileDrag();
    void UpdateCellBunchWhileNoDrag(PointBase<float> const& mousePt);
    virtual int GiveUpItem(ai::GeomRepositoryItem const& item, m3d::ui::Wnd* targetWnd) override;
    virtual ai::GeomRepositoryItem GetItemFromOrigin(PointBase<float> const& wndPt) override;
    virtual int AddItem(ai::GeomRepositoryItem const& item) override;
    virtual void OnUpdateWhileDrag(PointBase<float> const& wndPt) override;
    virtual void OnUpdateWhileNoDrag(PointBase<float> const& wndPt) override;
    virtual void OnDragOut() override;
    virtual int OnDragRemove() override;
    virtual bool IsItemCompatibleByResourceType(ai::GeomRepositoryItem const& item) const;
    virtual int StartDrag() override;
    virtual void HideDragSrc() override;
    virtual void ShowDragSrc() override;
    void AddInfoToEncyclopaedia();
    void OnSlotOut();
    void OnSlotIn(int slotId);

    using SlotVector = std::vector<GeomSlot*, std::allocator<GeomSlot*>>;
    using ItemStyleMap = std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int>>>;
    using ItemStylePair = std::pair<int, int>;
    using TwinkleInfoVector = std::vector<RepositoryWnd::TwinkleInfo, std::allocator<RepositoryWnd::TwinkleInfo>>;

protected:
    bool m_isInited;
    PointBase<int> m_geomSize;
    std::vector<GeomSlot*, std::allocator<GeomSlot*>> m_slots;
    PointBase<float> m_cellSize;
    BoundsBase<float> m_maxBounds;
    RepositoryWnd::CellBunch m_curBunch;
    ai::GeomRepository* m_repository;
    m3d::rend::TexHandle m_cellTextures[6];
    m3d::rend::TexHandle m_unsuitableBg;
    RepositoryWnd::RepositoryType m_type;
    RepositoryWnd::AuxInfo m_aif;
    bool m_isSpecialized;
    std::vector<int, std::allocator<int>> m_compatibleResourceIds;
    std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int>>> m_itemStyles;
    int m_defaultItemStyle;
    int m_prevOverroledSlotId;
    std::vector<RepositoryWnd::TwinkleInfo, std::allocator<RepositoryWnd::TwinkleInfo>> m_twinkleItems;
    RepositoryWnd();
    RepositoryWnd(RepositoryWnd const& rhs);

public:
    virtual ~RepositoryWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classRepositoryWnd;
};
