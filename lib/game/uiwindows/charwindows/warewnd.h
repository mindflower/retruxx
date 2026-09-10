#pragma once
#include "dragdropitemswnd.h"
#include <core/ref_ptr.h>
#include <ui/button.h>

namespace ai
{
    class Workshop;
}

namespace m3d
{
    namespace ui
    {
        class ImageWnd;
    }
}

class WareItem : public m3d::ui::ButtonWnd
{
    // WareList loads and clears the shared row pattern on the item's behalf.
    friend class WareList;

public:
    int SetUp(int warePrototypeId, int workshopId, int idx);
    int GetWarePrototypeId() const;
    void MarkByPrototype(bool bMark);
    bool IsMarkedByPrototype() const;
    void OnNewFrame();

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_wndIcoName;
        /* 0x000c */ CStr m_wndNameName;
        /* 0x0018 */ CStr m_wndBuyPriceName;
        /* 0x0024 */ CStr m_wndSellPriceName;
        /* 0x0030 */ CStr m_wndAmountName;
        /* 0x003c */ CStr m_wndCannotBuyIcoName;
        /* 0x0048 */ CStr m_wndCannotSellIcoName;
        /* 0x0054 */ CStr m_wndPatternName;
        /* 0x0060 */ CStr m_cannotSellStrId;
        /* 0x006c */ CStr m_cannotBuyStrId;
        /* 0x0078 */ unsigned int m_cannotSellColor;
        /* 0x007c */ unsigned int m_markByPrototypeColor;
        AuxInfo(WareItem::AuxInfo const& rhs);
        AuxInfo();
    }; /* size: 0x0080 */

    struct Pattern
    {
        /* 0x0000 */ m3d::ui::ButtonWnd* m_wndPattern;
        /* 0x0004 */ m3d::ui::ImageWnd* m_wndPatternIco;
        /* 0x0008 */ m3d::ui::Wnd* m_wndPatternName;
        /* 0x000c */ m3d::ui::Wnd* m_wndPatternBuyPrice;
        /* 0x0010 */ m3d::ui::Wnd* m_wndPatternSellPrice;
        /* 0x0014 */ m3d::ui::Wnd* m_wndPatternAmount;
        /* 0x0018 */ m3d::ui::ImageWnd* m_wndPatternCannotBuyIco;
        /* 0x001c */ m3d::ui::ImageWnd* m_wndPatternCannotSellIco;
        Pattern();
    }; /* size: 0x0020 */

protected:
    virtual int CreateFromPattern() /* 0x11c */;
    virtual int CreateChildren() /* 0x120 */;
    static int __fastcall LoadPattern(m3d::ui::Wnd* pattern);
    static void __fastcall ClearPattern();
    virtual void OnPaintOverChildren(m3d::ui::DrawInfo const& di) override;
    ai::Workshop* GetWorkshop() const;
    m3d::rend::TexHandle GetIcoForWare(int warePrototypeId) const;
    CStr GetFullNameForWare(int warePrototypeId) const;
    void FullUpdate();
    void UpdateName();
    void UpdateIco();
    void UpdateSellPrice(bool bForce);
    void UpdateBuyPrice(bool bForce);
    void UpdateAmount(bool bForce);
    int GetTownId() const;
    void UpdateTooltip();
    void UpdateActiveState();
    void SetActive(bool bActive);
    void GetBtnImages(
        bool bActive,
        m3d::rend::TexHandle& imgOut,
        m3d::rend::TexHandle& imgOver,
        m3d::rend::TexHandle& imgDown,
        m3d::rend::TexHandle& imgDisabled) const;

    /* 0x023c */ int m_warePrototypeId;
    /* 0x0240 */ int m_workshopId;
    /* 0x0244 */ int m_idx;
    /* 0x0248 */ m3d::ui::ImageWnd* m_wndIco;
    /* 0x024c */ m3d::ui::Wnd* m_wndName;
    /* 0x0250 */ m3d::ui::Wnd* m_wndBuyPrice;
    /* 0x0254 */ m3d::ui::Wnd* m_wndSellPrice;
    /* 0x0258 */ m3d::ui::Wnd* m_wndAmount;
    /* 0x025c */ m3d::ui::ImageWnd* m_wndCannotBuyIco;
    /* 0x0260 */ m3d::ui::ImageWnd* m_wndCannotSellIco;
    /* 0x0264 */ bool m_bMarkedByPrototype;
    static inline WareItem::AuxInfo m_aif;
    static inline WareItem::Pattern m_pattern;
    /* 0x0268 */ int m_buyPrice;
    /* 0x026c */ int m_sellPrice;
    /* 0x0270 */ int m_amount;
    /* 0x0274 */ bool m_bActive;

    WareItem();
    WareItem(WareItem const& rhs);

public:
    virtual ~WareItem() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(WareItem);
}; /* size: 0x0278 */

class WareList : public m3d::ui::Wnd
{
public:
    enum Mode
    {
        MODE_FULL = 0,
        MODE_ONLY_SELLED = 1,
    };

public:
    int SetupForWorkshop(int workshopId);
    int CreateFromPattern(m3d::ui::Wnd* pattern, bool deleteSrc);
    void OnNewFrame();
    int CreateItems();
    int BuyWare(int wareObjId);
    void ReactOnMouseClick(bool bReact);
    void MarkItemByPrototypeId(int warePrototypeId);
    void SetItemToTwinkle(int warePrototypeId);
    WareList::Mode GetCurMode() const;
    void SetCurMode(WareList::Mode mode);

    using WareItemVector = std::vector<WareItem*, std::allocator<WareItem*>>;

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_wndItemPatternName;
        /* 0x000c */ float m_space;
        /* 0x0010 */ unsigned int m_twinklePeriod;
        /* 0x0014 */ unsigned int m_numTwinkles;
        AuxInfo(WareList::AuxInfo const& rhs);
        AuxInfo();
    }; /* size: 0x0018 */

    struct TwinkleInfo
    {
        /* 0x0000 */ int m_itemPrototypeId;
        /* 0x0004 */ unsigned int m_startTime;
        TwinkleInfo(int itemPrototypeId);
        void Invalidate();
        bool IsValid() const;
    }; /* size: 0x0008 */

    using TwinkleInfoVector = std::vector<WareList::TwinkleInfo, std::allocator<WareList::TwinkleInfo>>;

protected:
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override
        /* 0xc8 */;
    virtual int OnBeforeAddToWndStation() override /* 0x68 */;
    void ClearItems();
    ai::Workshop* GetWorkshop() const;
    int AddItem(int warePrototypeId, int idx);
    int SellWare(int warePrototypeId);
    void OnSelectItem(WareItem* item);
    void ShowVideo(WareItem* item);
    void HideVideo();
    void RecalcLayot();
    void TwinkleItem(WareList::TwinkleInfo& twinkleInfo);
    WareItem* GetItemByWarePrototypeId(int warePrototypeId) const;
    void FullUpdate();
    int EnsureVisibleWare(int warePrototypeId);

    /* 0x0220 */ WareItemVector m_items;
    /* 0x0230 */ int m_workshopId;
    /* 0x0234 */ WareList::AuxInfo m_aif;
    /* 0x024c */ bool m_bMouseClickReact;
    /* 0x0250 */ TwinkleInfoVector m_twinkleItems;
    /* 0x0260 */ WareList::Mode m_curMode;

    WareList();
    WareList(WareList const& rhs);

public:
    virtual ~WareList() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(WareList);
}; /* size: 0x0264 */

class WareWnd : public DragDropItemsWnd
{
public:
    int CreateFromPattern(m3d::ui::Wnd* pattern, bool deleteSrc);
    virtual BoundsBase<float> GeomToWndBounds(BoundsBase<int> const& geomB) override /* 0x11c */;
    int SetupForWorkshop(int workshopId);
    virtual int CanAddDragItem(bool bSilent) override /* 0x124 */;
    int UpdateOnItemQuickDrop(ItemAcceptInfo const& info);
    int BuyWare(int wareObjId);

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_wndWareListName;
        /* 0x000c */ CStr m_wndSellPricesIcoName;
        /* 0x0018 */ CStr m_wndBuyPricesIcoName;
        /* 0x0024 */ CStr m_wndAmountsIcoName;
        /* 0x0030 */ CStr m_dragPaneName;
        /* 0x003c */ CStr m_btnModeName;
        /* 0x0048 */ CStr m_wndEmbossBtnModeName;
        /* 0x0054 */ CStr m_toAdvancePaneName;
        /* 0x0060 */ CStr m_toSimplePaneName;
        /* 0x006c */ CStr m_strIdShowAll;
        /* 0x0078 */ CStr m_strIdShowSelled;
        AuxInfo(WareWnd::AuxInfo const& rhs);
        AuxInfo();
    }; /* size: 0x0084 */

protected:
    virtual int OnMouseButton0(unsigned int state, PointBase<float> const& at) override /* 0xa4 */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override
        /* 0xc8 */;
    virtual void OnPaintOverChildren(m3d::ui::DrawInfo const& di) override;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual ai::GeomRepositoryItem GetItemFromOrigin(PointBase<float> const& wndPt) override /* 0x148 */;
    virtual int AddItem(ai::GeomRepositoryItem const& item) override /* 0x14c */;
    virtual void OnUpdateWhileDrag(PointBase<float> const& wndPt) override /* 0x150 */;
    virtual void OnUpdateWhileNoDrag(PointBase<float> const& wndPt) override /* 0x154 */;
    virtual void OnDragOut() override /* 0x158 */;
    virtual int OnDragRemove() override /* 0x15c */;
    virtual int GiveUpItem(ai::GeomRepositoryItem const& item, m3d::ui::Wnd* targetWnd) override /* 0x160 */;
    virtual void HideDragSrc() override /* 0x164 */;
    virtual void ShowDragSrc() override /* 0x168 */;
    void OnNewFrame();
    bool CanWindowQuickDropToMe(m3d::ui::Wnd const* srcWnd, ai::GeomRepositoryItem const& item) const;
    virtual bool IsItemCompatibleByResourceType(ai::GeomRepositoryItem const& item) const /* 0x170 */;
    ai::Workshop* GetWorkshop() const;
    void OnDistantMouseIn(ai::GeomRepositoryItem const& item);
    void OnDistantMouseOut();
    bool CanAddDragItemDistant(ai::GeomRepositoryItem const& item) const;
    void OnModeBtnClicked();
    void SetCurMode(WareList::Mode mode);
    void OnChangeCurMode();
    void UpdateModeButtonState();
    void UpdateModeTooltip();
    WareList::Mode GetCurMode() const;
    void OnCurProfileChanged();
    void OnCurProfileParamChanged(void* data);

    /* 0x0224 */ ref_ptr<WareList> m_wndWareList;
    /* 0x0228 */ WareWnd::AuxInfo m_aif;
    /* 0x02ac */ int m_workshopId;
    /* 0x02b0 */ bool m_bDragging;
    /* 0x02b4 */ m3d::ui::ButtonWnd* m_btnMode;

    WareWnd();
    WareWnd(WareWnd const& rhs);

public:
    virtual ~WareWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(WareWnd);
}; /* size: 0x02b8 */
