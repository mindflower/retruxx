#pragma once

namespace WareList
{
    class TwinkleInfo
    {
    public:
        void Invalidate();
        TwinkleInfo(int);
        bool IsValid() const ;
    protected:
    private:
        int m_itemPrototypeId;
        unsigned int m_startTime;
    };
}

class WareWnd :  public DragDropItemsWnd
{
public:
    virtual int CanAddDragItem(bool);
    int UpdateOnItemQuickDrop(struct ItemAcceptInfo const &);
    virtual struct m3d::Class * GetClass() const ;
    virtual struct BoundsBase<float> GeomToWndBounds(struct BoundsBase<int> const &);
    int BuyWare(int);
    static class m3d::Object * __fastcall CreateObject();
    int SetupForWorkshop(int);
    virtual class m3d::Object * Clone();
    static struct m3d::Class * __fastcall GetBaseClass();
    `public: int CreateFromPattern(class m3d::ui::Wnd *,bool)'::`54'::__unnamed::~__unnamed();
    virtual ~WareWnd();
protected:
    bool CanAddDragItemDistant(struct ai::GeomRepositoryItem const &) const ;
    void SetCurMode(enum WareList::Mode);
    void UpdateModeButtonState();
    void OnDistantMouseOut();
    void OnCurProfileParamChanged(void *);
    void OnNewFrame();
    virtual int GiveUpItem(struct ai::GeomRepositoryItem const &,class m3d::ui::Wnd *);
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    bool CanWindowQuickDropToMe(class m3d::ui::Wnd const *,struct ai::GeomRepositoryItem const &) const ;
    virtual int OnDragRemove();
    void OnDistantMouseIn(struct ai::GeomRepositoryItem const &);
    enum WareList::Mode GetCurMode() const ;
    void OnCurProfileChanged();
    virtual int AddItem(struct ai::GeomRepositoryItem const &);
    virtual struct ai::GeomRepositoryItem GetItemFromOrigin(class PointBase<float> const &);
    virtual void OnDragOut();
    void OnModeBtnClicked();
    virtual void HideDragSrc();
    virtual void ShowDragSrc();
    void UpdateModeTooltip();
    void OnChangeCurMode();
    virtual void OnUpdateWhileNoDrag(class PointBase<float> const &);
    virtual int GameDataUpdate(void *,int);
    virtual bool IsItemCompatibleByResourceType(struct ai::GeomRepositoryItem const &) const ;
    virtual int OnMouseButton0(unsigned int,class PointBase<float> const &);
    virtual void OnUpdateWhileDrag(class PointBase<float> const &);
    class ai::Workshop * GetWorkshop() const ;
    virtual void OnPaintOverChildren(struct m3d::ui::DrawInfo const &);
    WareWnd(class WareWnd const &);
    WareWnd();
private:
    ref_ptr<WareList> m_wndWareList;
    WareWnd::AuxInfo m_aif;
    int m_workshopId;
    bool m_bDragging;
    m3d::ui::ButtonWnd *m_btnMode;
};

class WareItem :  public m3d::ui::ButtonWnd
{
public:
    static class m3d::Object * __fastcall CreateObject();
    bool IsMarkedByPrototype() const ;
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual class m3d::Object * Clone();
    virtual ~WareItem();
    int SetUp(int,int,int);
    virtual struct m3d::Class * GetClass() const ;
    void MarkByPrototype(bool);
    void OnNewFrame();
    int GetWarePrototypeId() const ;
protected:
    void UpdateBuyPrice(bool);
    int GetTownId() const ;
    WareItem();
    WareItem(class WareItem const &);
    virtual int CreateFromPattern();
    void UpdateSellPrice(bool);
    virtual int CreateChildren();
    void GetBtnImages(bool,class m3d::rend::TexHandle &,class m3d::rend::TexHandle &,class m3d::rend::TexHandle &,class m3d::rend::TexHandle &) const ;
    static void __fastcall ClearPattern();
    void UpdateActiveState();
    virtual void OnPaintOverChildren(struct m3d::ui::DrawInfo const &);
    void UpdateName();
    static int __fastcall LoadPattern(class m3d::ui::Wnd *);
    void UpdateIco();
    void UpdateTooltip();
    class ai::Workshop * GetWorkshop() const ;
    void SetActive(bool);
    class CStr GetFullNameForWare(int) const ;
    void FullUpdate();
    class m3d::rend::TexHandle GetIcoForWare(int) const ;
    void UpdateAmount(bool);
private:
    int m_warePrototypeId;
    int m_workshopId;
    int m_idx;
    m3d::ui::ImageWnd *m_wndIco;
    m3d::ui::Wnd *m_wndName;
    m3d::ui::Wnd *m_wndBuyPrice;
    m3d::ui::Wnd *m_wndSellPrice;
    m3d::ui::Wnd *m_wndAmount;
    m3d::ui::ImageWnd *m_wndCannotBuyIco;
    m3d::ui::ImageWnd *m_wndCannotSellIco;
    bool m_bMarkedByPrototype;
    int m_buyPrice;
    int m_sellPrice;
    int m_amount;
    bool m_bActive;
};

class WareList :  public m3d::ui::Wnd
{
public:
    int SetupForWorkshop(int);
    virtual ~WareList();
    void ReactOnMouseClick(bool);
    virtual class m3d::Object * Clone();
    void MarkItemByPrototypeId(int);
    static struct m3d::Class * __fastcall GetBaseClass();
    void SetItemToTwinkle(int);
    static class m3d::Object * __fastcall CreateObject();
    int CreateFromPattern(class m3d::ui::Wnd *,bool);
    int BuyWare(int);
    void OnNewFrame();
    enum Mode GetCurMode() const ;
    int CreateItems();
    virtual struct m3d::Class * GetClass() const ;
    void SetCurMode(enum Mode);
protected:
    void ClearItems();
    int AddItem(int,int);
    void OnSelectItem(class WareItem *);
    WareList();
    WareList(class WareList const &);
    int SellWare(int);
    virtual int OnBeforeAddToWndStation();
    void TwinkleItem(struct TwinkleInfo &);
    void RecalcLayot();
    void ShowVideo(class WareItem *);
    void HideVideo();
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    class ai::Workshop * GetWorkshop() const ;
    void FullUpdate();
    class WareItem * GetItemByWarePrototypeId(int) const ;
    int EnsureVisibleWare(int);
private:
    std::fill<TwinkleInfo *,TwinkleInfo>(TwinkleInfo *,TwinkleInfo *,TwinkleInfo const &);
    std::_Ptr_cat<TwinkleInfo *,TwinkleInfo *>(TwinkleInfo * &,TwinkleInfo * &);
    std::copy_backward<TwinkleInfo *,TwinkleInfo *>(TwinkleInfo *,TwinkleInfo *,TwinkleInfo *);
    std::copy<TwinkleInfo *,TwinkleInfo *>(TwinkleInfo *,TwinkleInfo *,TwinkleInfo *);
    std::_Copy_backward_opt<TwinkleInfo *,TwinkleInfo *>(TwinkleInfo *,TwinkleInfo *,TwinkleInfo *,std::_Nonscalar_ptr_iterator_tag);
    std::_Construct<TwinkleInfo,TwinkleInfo>(TwinkleInfo *,TwinkleInfo const &);
    std::allocator<TwinkleInfo>::allocator<TwinkleInfo>();
    std::allocator<TwinkleInfo>::allocator<TwinkleInfo>();
    std::_Allocate<TwinkleInfo>(uint,TwinkleInfo *);
    std::_Destroy<TwinkleInfo>(TwinkleInfo *);
    std::_Copy_opt<TwinkleInfo *,TwinkleInfo *>(TwinkleInfo *,TwinkleInfo *,TwinkleInfo *,std::_Nonscalar_ptr_iterator_tag);
    std::vector<WareItem *> m_items;
    int m_workshopId;
    WareList::AuxInfo m_aif;
    bool m_bMouseClickReact;
    std::vector<WareList::TwinkleInfo> m_twinkleItems;
    WareList::Mode m_curMode;
};
