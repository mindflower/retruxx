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
        CStr m_wndIcoName;
        CStr m_wndNameName;
        CStr m_wndBuyPriceName;
        CStr m_wndSellPriceName;
        CStr m_wndAmountName;
        CStr m_wndCannotBuyIcoName;
        CStr m_wndCannotSellIcoName;
        CStr m_wndPatternName;
        CStr m_cannotSellStrId;
        CStr m_cannotBuyStrId;
        unsigned int m_cannotSellColor;
        unsigned int m_markByPrototypeColor;
        AuxInfo(WareItem::AuxInfo const& rhs);
        AuxInfo();
    };

    struct Pattern
    {
        m3d::ui::ButtonWnd* m_wndPattern;
        m3d::ui::ImageWnd* m_wndPatternIco;
        m3d::ui::Wnd* m_wndPatternName;
        m3d::ui::Wnd* m_wndPatternBuyPrice;
        m3d::ui::Wnd* m_wndPatternSellPrice;
        m3d::ui::Wnd* m_wndPatternAmount;
        m3d::ui::ImageWnd* m_wndPatternCannotBuyIco;
        m3d::ui::ImageWnd* m_wndPatternCannotSellIco;
        Pattern();
    };

protected:
    virtual int CreateFromPattern();
    virtual int CreateChildren();
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

    int m_warePrototypeId;
    int m_workshopId;
    int m_idx;
    m3d::ui::ImageWnd* m_wndIco;
    m3d::ui::Wnd* m_wndName;
    m3d::ui::Wnd* m_wndBuyPrice;
    m3d::ui::Wnd* m_wndSellPrice;
    m3d::ui::Wnd* m_wndAmount;
    m3d::ui::ImageWnd* m_wndCannotBuyIco;
    m3d::ui::ImageWnd* m_wndCannotSellIco;
    bool m_bMarkedByPrototype;
    static inline WareItem::AuxInfo m_aif;
    static inline WareItem::Pattern m_pattern;
    int m_buyPrice;
    int m_sellPrice;
    int m_amount;
    bool m_bActive;

    WareItem();
    WareItem(WareItem const& rhs);

public:
    virtual ~WareItem() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(WareItem);
};

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
        CStr m_wndItemPatternName;
        float m_space;
        unsigned int m_twinklePeriod;
        unsigned int m_numTwinkles;
        AuxInfo(WareList::AuxInfo const& rhs);
        AuxInfo();
    };

    struct TwinkleInfo
    {
        int m_itemPrototypeId;
        unsigned int m_startTime;
        TwinkleInfo(int itemPrototypeId);
        void Invalidate();
        bool IsValid() const;
    };

    using TwinkleInfoVector = std::vector<WareList::TwinkleInfo, std::allocator<WareList::TwinkleInfo>>;

protected:
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override
       ;
    virtual int OnBeforeAddToWndStation() override;
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

    WareItemVector m_items;
    int m_workshopId;
    WareList::AuxInfo m_aif;
    bool m_bMouseClickReact;
    TwinkleInfoVector m_twinkleItems;
    WareList::Mode m_curMode;

    WareList();
    WareList(WareList const& rhs);

public:
    virtual ~WareList() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(WareList);
};

class WareWnd : public DragDropItemsWnd
{
public:
    int CreateFromPattern(m3d::ui::Wnd* pattern, bool deleteSrc);
    virtual BoundsBase<float> GeomToWndBounds(BoundsBase<int> const& geomB) override;
    int SetupForWorkshop(int workshopId);
    virtual int CanAddDragItem(bool bSilent) override;
    int UpdateOnItemQuickDrop(ItemAcceptInfo const& info);
    int BuyWare(int wareObjId);

    struct AuxInfo
    {
        CStr m_wndWareListName;
        CStr m_wndSellPricesIcoName;
        CStr m_wndBuyPricesIcoName;
        CStr m_wndAmountsIcoName;
        CStr m_dragPaneName;
        CStr m_btnModeName;
        CStr m_wndEmbossBtnModeName;
        CStr m_toAdvancePaneName;
        CStr m_toSimplePaneName;
        CStr m_strIdShowAll;
        CStr m_strIdShowSelled;
        AuxInfo(WareWnd::AuxInfo const& rhs);
        AuxInfo();
    };

protected:
    virtual int OnMouseButton0(unsigned int state, PointBase<float> const& at) override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override
       ;
    virtual void OnPaintOverChildren(m3d::ui::DrawInfo const& di) override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual ai::GeomRepositoryItem GetItemFromOrigin(PointBase<float> const& wndPt) override;
    virtual int AddItem(ai::GeomRepositoryItem const& item) override;
    virtual void OnUpdateWhileDrag(PointBase<float> const& wndPt) override;
    virtual void OnUpdateWhileNoDrag(PointBase<float> const& wndPt) override;
    virtual void OnDragOut() override;
    virtual int OnDragRemove() override;
    virtual int GiveUpItem(ai::GeomRepositoryItem const& item, m3d::ui::Wnd* targetWnd) override;
    virtual void HideDragSrc() override;
    virtual void ShowDragSrc() override;
    void OnNewFrame();
    bool CanWindowQuickDropToMe(m3d::ui::Wnd const* srcWnd, ai::GeomRepositoryItem const& item) const;
    virtual bool IsItemCompatibleByResourceType(ai::GeomRepositoryItem const& item) const;
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

    ref_ptr<WareList> m_wndWareList;
    WareWnd::AuxInfo m_aif;
    int m_workshopId;
    bool m_bDragging;
    m3d::ui::ButtonWnd* m_btnMode;

    WareWnd();
    WareWnd(WareWnd const& rhs);

public:
    virtual ~WareWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(WareWnd);
};
