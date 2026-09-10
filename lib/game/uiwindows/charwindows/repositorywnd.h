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
        /* 0x0000 */ BoundsBase<int> m_bounds;
        /* 0x0010 */ SlotState m_state;
    }; /* size: 0x0014 */

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
        /* 0x0000 */ CStr m_cellTexNames[6];
        /* 0x0048 */ CStr m_unsuitableBgName;
        /* 0x0054 */ unsigned int m_twinklePeriod;
        /* 0x0058 */ unsigned int m_numTwinkles;
        AuxInfo(RepositoryWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x005c */

public:
    virtual int CreateFromPattern(m3d::ui::Wnd const* patternWnd, bool deleteSrc) /* 0x170 */;
    ai::GeomRepository* GetRepository() const;
    virtual int SetRepository(ai::GeomRepository* repository, RepositoryWnd::RepositoryType type) /* 0x174 */;
    PointBase<int> GetGeomSize() const;
    PointBase<float> GetCellSize() const;
    void SetCellSize(PointBase<float> const& size);
    bool IsValid() const;
    PointBase<int> WndToGeomPt(PointBase<float> const& wndPt);
    virtual BoundsBase<float> GeomToWndBounds(BoundsBase<int> const& geomB) override /* 0x11c */;
    BoundsBase<int> WndToGeomBounds(BoundsBase<float> const& wndB);
    virtual void Enable(bool needEnable) override /* 0x120 */;
    virtual void SetBounds(BoundsBase<float> const& bounds, bool bUpdateBaseOrigin) override /* 0x00 */;
    void SetMaxBounds(BoundsBase<float> const& maxBounds);
    BoundsBase<float> const& GetMaxBounds() const;
    void SetSpecialized(std::vector<int, std::allocator<int>> const& compatibleRcIds);
    void SetItemDrawStyleByResourceId(int style, int rcId);
    int GetItemDrawStyleByResourceId(int rcId) const;
    RepositoryWnd::RepositoryType GetRepositoryType() const;
    virtual int CreateScrollWnd() /* 0x178 */;
    virtual int CanAddDragItem(bool bSilent) override /* 0x124 */;
    int UpdateOnItemQuickDrop(ItemAcceptInfo const& info);
    void SetItemToTwinkle(int itemObjId);

    struct TwinkleInfo
    {
        /* 0x0000 */ int m_itemObjId;
        /* 0x0004 */ unsigned int m_startTime;
        TwinkleInfo(int itemObjId);
        void Invalidate();
        bool IsValid() const;
    }; /* size: 0x0008 */

protected:
    void SetGeomSize(PointBase<int> const& geomSize);
    void AdjustWndToCells();
    void AdjustCellsToWnd();
    virtual int CreateItems() /* 0x17c */;
    virtual void RemoveAllItems() /* 0x180 */;
    float GetMaxSlotY();
    void UpdateScrollWnd();
    void ScrollSlots();
    void OnRepositoryChanged(void* data);
    int ShowInfo(int itemId);
    int HideInfo();
    int ShowVideo(int itemId);
    int HideVideo();
    virtual void DrawBunch(m3d::ui::DrawInfo const& di) /* 0x184 */;
    virtual void DrawGrid(m3d::ui::DrawInfo const& di) /* 0x188 */;
    virtual void DrawFrame(m3d::ui::DrawInfo const& di) /* 0x18c */;
    virtual void DrawUnsuitableBg(m3d::ui::DrawInfo const& di) /* 0x190 */;
    bool CanWindowQuickDropToMe(m3d::ui::Wnd const* srcWnd, ai::GeomRepositoryItem const& item) const;
    GeomSlot* GetDraggingSlot() const;
    virtual void OnPlayerVehicleChanged() /* 0x194 */;
    virtual void OnVehiclePartChanged(void* data) /* 0x198 */;
    virtual void OnPlayerMoneyChanged() /* 0x19c */;
    virtual void UpdateSlotsStylesCompatibleWithVehicle() /* 0x1a0 */;
    virtual void UpdateSlotsStylesTooRich() /* 0x1a4 */;
    virtual void SetSlotCompatibleWithVehicle(int slotIdx, bool bCompatible) /* 0x1a8 */;
    virtual void SetSlotTooRich(int slotIdx, bool bTooRich) /* 0x1ac */;
    virtual void FullUpdate() /* 0x1b0 */;
    virtual void TwinkleItem(RepositoryWnd::TwinkleInfo& twinkleInfo) /* 0x1b4 */;
    void OnNewFrame();
    GeomSlot* GetSlotByItemObjId(int itemObjId) const;
    virtual int OnPaint(m3d::ui::DrawInfo const& di) override /* 0x00 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x00 */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override /* 0xc8 */;
    virtual int OnMouseWheel(int ticks, PointBase<float> const& at) override /* 0x00 */;
    virtual int OnMouseButton1(unsigned int state, PointBase<float> const& at) override /* 0x00 */;
    virtual int OnBeforeAddToWndStation() override /* 0x00 */;
    virtual int OnAfterRemoveFromWndStation() override /* 0x00 */;
    void UpdateCellBunchWhileDrag();
    void UpdateCellBunchWhileNoDrag(PointBase<float> const& mousePt);
    virtual int GiveUpItem(ai::GeomRepositoryItem const& item, m3d::ui::Wnd* targetWnd) override /* 0x160 */;
    virtual ai::GeomRepositoryItem GetItemFromOrigin(PointBase<float> const& wndPt) override /* 0x148 */;
    virtual int AddItem(ai::GeomRepositoryItem const& item) override /* 0x14c */;
    virtual void OnUpdateWhileDrag(PointBase<float> const& wndPt) override /* 0x150 */;
    virtual void OnUpdateWhileNoDrag(PointBase<float> const& wndPt) override /* 0x154 */;
    virtual void OnDragOut() override /* 0x158 */;
    virtual int OnDragRemove() override /* 0x15c */;
    virtual bool IsItemCompatibleByResourceType(ai::GeomRepositoryItem const& item) const /* 0x1b8 */;
    virtual int StartDrag() override /* 0x12c */;
    virtual void HideDragSrc() override /* 0x164 */;
    virtual void ShowDragSrc() override /* 0x168 */;
    void AddInfoToEncyclopaedia();
    void OnSlotOut();
    void OnSlotIn(int slotId);

    using SlotVector = std::vector<GeomSlot*, std::allocator<GeomSlot*>>;
    using ItemStyleMap = std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int>>>;
    using ItemStylePair = std::pair<int, int>;
    using TwinkleInfoVector = std::vector<RepositoryWnd::TwinkleInfo, std::allocator<RepositoryWnd::TwinkleInfo>>;

protected:
    /* 0x0224 */ bool m_isInited;
    /* 0x0225 */ char Padding_114[3];
    /* 0x0228 */ PointBase<int> m_geomSize;
    /* 0x0230 */ std::vector<GeomSlot*, std::allocator<GeomSlot*>> m_slots;
    /* 0x0240 */ PointBase<float> m_cellSize;
    /* 0x0248 */ BoundsBase<float> m_maxBounds;
    /* 0x0258 */ RepositoryWnd::CellBunch m_curBunch;
    /* 0x026c */ ai::GeomRepository* m_repository;
    /* 0x0270 */ m3d::rend::TexHandle m_cellTextures[6];
    /* 0x0288 */ m3d::rend::TexHandle m_unsuitableBg;
    /* 0x028c */ RepositoryWnd::RepositoryType m_type;
    /* 0x0290 */ RepositoryWnd::AuxInfo m_aif;
    /* 0x02ec */ bool m_isSpecialized;
    /* 0x02ed */ char Padding_115[3];
    /* 0x02f0 */ std::vector<int, std::allocator<int>> m_compatibleResourceIds;
    /* 0x0300 */ std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int>>> m_itemStyles;
    /* 0x030c */ int m_defaultItemStyle;
    /* 0x0310 */ int m_prevOverroledSlotId;
    /* 0x0314 */ std::vector<RepositoryWnd::TwinkleInfo, std::allocator<RepositoryWnd::TwinkleInfo>> m_twinkleItems;
    RepositoryWnd();
    RepositoryWnd(RepositoryWnd const& rhs);

public:
    virtual ~RepositoryWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    static m3d::Class m_classRepositoryWnd;
}; /* size: 0x0324 */
