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
public:
    static m3d::Object* CreateObject();
    bool IsMarkedByPrototype() const;
    static m3d::Class* GetBaseClass();
    virtual m3d::Object* Clone();
    virtual ~WareItem();
    int SetUp(int, int, int);
    virtual m3d::Class* GetClass() const;
    void MarkByPrototype(bool);
    void OnNewFrame();
    int GetWarePrototypeId() const;

protected:
    void UpdateBuyPrice(bool);
    int GetTownId() const;
    WareItem();
    WareItem(WareItem const&);
    virtual int CreateFromPattern();
    void UpdateSellPrice(bool);
    virtual int CreateChildren();
    void GetBtnImages(bool, m3d::rend::TexHandle&, m3d::rend::TexHandle&, m3d::rend::TexHandle&, m3d::rend::TexHandle&) const;
    static void __fastcall ClearPattern();
    void UpdateActiveState();
    virtual void OnPaintOverChildren(m3d::ui::DrawInfo const&);
    void UpdateName();
    static int __fastcall LoadPattern(m3d::ui::Wnd*);
    void UpdateIco();
    void UpdateTooltip();
    ai::Workshop* GetWorkshop() const;
    void SetActive(bool);
    CStr GetFullNameForWare(int) const;
    void FullUpdate();
    m3d::rend::TexHandle GetIcoForWare(int) const;
    void UpdateAmount(bool);

public:
    RT_CLASS_DECLARE(WareItem);

private:
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
    int m_buyPrice;
    int m_sellPrice;
    int m_amount;
    bool m_bActive;
};

class WareList : public m3d::ui::Wnd
{
public:
    class TwinkleInfo
    {
    public:
        void Invalidate();
        TwinkleInfo(int);
        bool IsValid() const;

    private:
        int m_itemPrototypeId;
        unsigned int m_startTime;
    };

    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_wndItemPatternName;
        float m_space;
        unsigned int m_twinklePeriod;
        unsigned int m_numTwinkles;
    };

    enum Mode
    {
        MODE_FULL = 0x0,
        MODE_ONLY_SELLED = 0x1,
    };

public:
    int SetupForWorkshop(int);
    virtual ~WareList();
    void ReactOnMouseClick(bool);
    virtual m3d::Object* Clone();
    void MarkItemByPrototypeId(int);
    static m3d::Class* GetBaseClass();
    void SetItemToTwinkle(int);
    static m3d::Object* CreateObject();
    int CreateFromPattern(m3d::ui::Wnd*, bool);
    int BuyWare(int);
    void OnNewFrame();
    Mode GetCurMode() const;
    int CreateItems();
    virtual m3d::Class* GetClass() const;
    void SetCurMode(Mode);

protected:
    void ClearItems();
    int AddItem(int, int);
    void OnSelectItem(WareItem*);
    WareList();
    WareList(WareList const&);
    int SellWare(int);
    virtual int OnBeforeAddToWndStation();
    void TwinkleItem(TwinkleInfo&);
    void RecalcLayot();
    void ShowVideo(WareItem*);
    void HideVideo();
    virtual int OnWndNotify(m3d::ui::Wnd*, unsigned int, unsigned int, m3d::AIParam const&);
    ai::Workshop* GetWorkshop() const;
    void FullUpdate();
    WareItem* GetItemByWarePrototypeId(int) const;
    int EnsureVisibleWare(int);

public:
    RT_CLASS_DECLARE(WareList);

private:
    std::vector<WareItem*> m_items;
    int m_workshopId;
    WareList::AuxInfo m_aif;
    bool m_bMouseClickReact;
    std::vector<WareList::TwinkleInfo> m_twinkleItems;
    WareList::Mode m_curMode;
};

class WareWnd :  public DragDropItemsWnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo(void);
    protected:
    private:
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
    };

public:
    virtual int CanAddDragItem(bool);
    int UpdateOnItemQuickDrop(ItemAcceptInfo const &);
    virtual m3d::Class * GetClass() const ;
    virtual BoundsBase<float> GeomToWndBounds(BoundsBase<int> const &);
    int BuyWare(int);
    static m3d::Object * CreateObject();
    int SetupForWorkshop(int);
    virtual m3d::Object * Clone();
    static m3d::Class * GetBaseClass();
    int CreateFromPattern(m3d::ui::Wnd*, bool);
    virtual ~WareWnd();

protected:
    bool CanAddDragItemDistant(ai::GeomRepositoryItem const &) const ;
    void SetCurMode(WareList::Mode);
    void UpdateModeButtonState();
    void OnDistantMouseOut();
    void OnCurProfileParamChanged(void *);
    void OnNewFrame();
    virtual int GiveUpItem(ai::GeomRepositoryItem const &, m3d::ui::Wnd *);
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int, m3d::AIParam const &);
    bool CanWindowQuickDropToMe(m3d::ui::Wnd const *, ai::GeomRepositoryItem const &) const ;
    virtual int OnDragRemove();
    void OnDistantMouseIn(ai::GeomRepositoryItem const &);
    WareList::Mode GetCurMode() const ;
    void OnCurProfileChanged();
    virtual int AddItem(ai::GeomRepositoryItem const &);
    virtual ai::GeomRepositoryItem GetItemFromOrigin(PointBase<float> const &);
    virtual void OnDragOut();
    void OnModeBtnClicked();
    virtual void HideDragSrc();
    virtual void ShowDragSrc();
    void UpdateModeTooltip();
    void OnChangeCurMode();
    virtual void OnUpdateWhileNoDrag(PointBase<float> const &);
    virtual int GameDataUpdate(void *,int);
    virtual bool IsItemCompatibleByResourceType(ai::GeomRepositoryItem const &) const ;
    virtual int OnMouseButton0(unsigned int, PointBase<float> const &);
    virtual void OnUpdateWhileDrag(PointBase<float> const &);
    ai::Workshop * GetWorkshop() const ;
    virtual void OnPaintOverChildren(m3d::ui::DrawInfo const &);
    WareWnd(WareWnd const &);
    WareWnd();

public:
    RT_CLASS_DECLARE(WareWnd);

private:
    ref_ptr<WareList> m_wndWareList;
    WareWnd::AuxInfo m_aif;
    int m_workshopId;
    bool m_bDragging;
    m3d::ui::ButtonWnd *m_btnMode;
};

