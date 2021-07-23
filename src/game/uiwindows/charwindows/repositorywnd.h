#pragma once

class RepositoryWnd :  public DragDropItemsWnd
{
public:
    static class m3d::Object * __fastcall IzvratCreateObject();
    class PointBase<int> WndToGeomPt(class PointBase<float> const &);
    virtual struct m3d::Class * IzvratGetClass() const ;
    void SetSpecialized(class std::vector<int,class std::allocator<int> > const &);
    struct BoundsBase<float> const & GetMaxBounds() const ;
    struct BoundsBase<float> IzvratGetMaxRepositoryBounds() const ;
    virtual class m3d::Object * Clone();
    virtual void Enable(bool);
    void SetCellSize(class PointBase<float> const &);
    virtual struct BoundsBase<float> GeomToWndBounds(struct BoundsBase<int> const &);
    virtual int CreateScrollWnd();
    class PointBase<int> GetGeomSize() const ;
    virtual void SetBounds(struct BoundsBase<float> const &,bool);
    void SetMaxBounds(struct BoundsBase<float> const &);
    virtual int CreateFromPattern(class m3d::ui::Wnd const *,bool);
    class PointBase<float> GetCellSize() const ;
    static class m3d::Object * __fastcall CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    virtual int IzvratSetIzvratRepository(class ai::IzvratRepository *,enum RepositoryType);
    int GetItemDrawStyleByResourceId(int) const ;
    enum RepositoryType GetRepositoryType() const ;
    virtual int CanAddDragItem(bool);
    static struct m3d::Class * __fastcall GetBaseClass();
    struct BoundsBase<int> WndToGeomBounds(struct BoundsBase<float> const &);
    static struct m3d::Class * __fastcall IzvratGetBaseClass();
    class ai::IzvratRepository * IzvratGetIzvratRepository() const ;
    virtual Izvrat~IzvratRepositoryWnd();
    virtual ~RepositoryWnd();
    bool IsValid() const ;
    void SetItemDrawStyleByResourceId(int,int);
    void SetItemToTwinkle(int);
    int UpdateOnItemQuickDrop(struct ItemAcceptInfo const &);
    virtual class m3d::Object * IzvratClone();
protected:
    virtual void DrawBunch(struct m3d::ui::DrawInfo const &);
    void OnSlotIn(int);
    struct GeomSlot * GetSlotByItemObjId(int) const ;
    virtual int IzvratCreateScrollWnd();
    int ShowVideo(int);
    void UpdateCellBunchWhileNoDrag(class PointBase<float> const &);
    void UpdateScrollWnd();
    virtual void SetSlotCompatibleWithVehicle(int,bool);
    void ScrollSlots();
    virtual void OnVehiclePartChanged(void *);
    void SetGeomSize(class PointBase<int> const &);
    virtual void OnDragOut();
    virtual void DrawGrid(struct m3d::ui::DrawInfo const &);
    virtual int OnDragRemove();
    int HideVideo();
    virtual void OnPlayerMoneyChanged();
    virtual int StartDrag();
    virtual int OnBeforeAddToWndStation();
    virtual int OnAfterRemoveFromWndStation();
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    void IzvratCalculateSegmentsBounds(class std::map<int,int,struct std::less<int>,class std::allocator<struct std::pair<int const ,int> > > const &,class std::vector<struct IzvratFrameSegment,class std::allocator<struct IzvratFrameSegment> > &);
    virtual void UpdateSlotsStylesCompatibleWithVehicle();
    virtual int GameDataUpdate(void *,int);
    virtual void OnUpdateWhileDrag(class PointBase<float> const &);
    void UpdateCellBunchWhileDrag();
    float GetMaxSlotY();
    virtual int GameDataClear(bool);
    virtual bool IsItemCompatibleByResourceType(struct ai::GeomRepositoryItem const &) const ;
    void AddInfoToEncyclopaedia();
    void AdjustWndToCells();
    virtual void IzvratDrawGrid(struct m3d::ui::DrawInfo const &);
    virtual void RemoveAllItems();
    virtual void UpdateSlotsStylesTooRich();
    virtual int OnMouseButton1(unsigned int,class PointBase<float> const &);
    void IzvratCalculateFrame();
    virtual int AddItem(struct ai::GeomRepositoryItem const &);
    void IzvratFindPerimeter(class std::map<int,int,struct std::less<int>,class std::allocator<struct std::pair<int const ,int> > > &) const ;
    virtual void DrawFrame(struct m3d::ui::DrawInfo const &);
    int HideInfo();
    void OnSlotOut();
    virtual void ShowDragSrc();
    virtual void TwinkleItem(struct TwinkleInfo &);
    bool CanWindowQuickDropToMe(class m3d::ui::Wnd const *,struct ai::GeomRepositoryItem const &) const ;
    virtual void OnPlayerVehicleChanged();
    void OnRepositoryChanged(void *);
    virtual void IzvratDrawFrame(struct m3d::ui::DrawInfo const &);
    struct GeomSlot * GetDraggingSlot() const ;
    virtual int OnPaint(struct m3d::ui::DrawInfo const &);
    virtual int GiveUpItem(struct ai::GeomRepositoryItem const &,class m3d::ui::Wnd *);
    void OnNewFrame();
    int ShowInfo(int);
    virtual void HideDragSrc();
    virtual void SetSlotTooRich(int,bool);
    virtual void OnUpdateWhileNoDrag(class PointBase<float> const &);
    virtual int IzvratSetRepository(class ai::GeomRepository *,enum RepositoryType);
    virtual int CreateItems();
    IzvratIzvratRepositoryWnd(class IzvratRepositoryWnd const &);
    IzvratIzvratRepositoryWnd();
    virtual struct ai::GeomRepositoryItem GetItemFromOrigin(class PointBase<float> const &);
    virtual void FullUpdate();
    void AdjustCellsToWnd();
    RepositoryWnd(class RepositoryWnd const &);
    RepositoryWnd();
    virtual void DrawUnsuitableBg(struct m3d::ui::DrawInfo const &);
    virtual void IzvratDrawBunch(struct m3d::ui::DrawInfo const &);
    virtual int OnMouseWheel(int,class PointBase<float> const &);
private:
    std::fill<IzvratFrameSegment *,IzvratFrameSegment>(IzvratFrameSegment *,IzvratFrameSegment *,IzvratFrameSegment const &);
    std::_Copy_backward_opt<TwinkleInfo *,TwinkleInfo *>(TwinkleInfo *,TwinkleInfo *,TwinkleInfo *,std::_Nonscalar_ptr_iterator_tag);
    std::copy<TwinkleInfo *,TwinkleInfo *>(TwinkleInfo *,TwinkleInfo *,TwinkleInfo *);
    std::_Ptr_cat<IzvratFrameSegment *,IzvratFrameSegment *>(IzvratFrameSegment * &,IzvratFrameSegment * &);
    IzvratSetRepository(ai::GeomRepository *,RepositoryType);
    std::_Copy_opt<TwinkleInfo *,TwinkleInfo *>(TwinkleInfo *,TwinkleInfo *,TwinkleInfo *,std::_Nonscalar_ptr_iterator_tag);
    std::_Allocate<IzvratFrameSegment>(uint,IzvratFrameSegment *);
    std::_Destroy<IzvratFrameSegment>(IzvratFrameSegment *);
    std::copy_backward<IzvratFrameSegment *,IzvratFrameSegment *>(IzvratFrameSegment *,IzvratFrameSegment *,IzvratFrameSegment *);
    std::_Destroy<TwinkleInfo>(TwinkleInfo *);
    std::_Copy_backward_opt<IzvratFrameSegment *,IzvratFrameSegment *>(IzvratFrameSegment *,IzvratFrameSegment *,IzvratFrameSegment *,std::_Nonscalar_ptr_iterator_tag);
    std::_Construct<TwinkleInfo,TwinkleInfo>(TwinkleInfo *,TwinkleInfo const &);
    std::_Construct<IzvratFrameSegment,IzvratFrameSegment>(IzvratFrameSegment *,IzvratFrameSegment const &);
    std::fill<TwinkleInfo *,TwinkleInfo>(TwinkleInfo *,TwinkleInfo *,TwinkleInfo const &);
    std::copy_backward<TwinkleInfo *,TwinkleInfo *>(TwinkleInfo *,TwinkleInfo *,TwinkleInfo *);
    std::allocator<IzvratFrameSegment>::allocator<IzvratFrameSegment>();
    std::allocator<IzvratFrameSegment>::allocator<IzvratFrameSegment>();
    std::swap<IzvratFrameSegment>(IzvratFrameSegment &,IzvratFrameSegment &);
    std::_Allocate<TwinkleInfo>(uint,TwinkleInfo *);
    std::_Ptr_cat<TwinkleInfo *,TwinkleInfo *>(TwinkleInfo * &,TwinkleInfo * &);
    std::allocator<TwinkleInfo>::allocator<TwinkleInfo>();
    std::allocator<TwinkleInfo>::allocator<TwinkleInfo>();
    bool m_isInited;
    PointBase<int> m_geomSize;
    std::vector<GeomSlot *> m_slots;
    PointBase<float> m_cellSize;
    BoundsBase<float> m_maxBounds;
    RepositoryWnd::CellBunch m_curBunch;
    ai::GeomRepository *m_repository;
    m3d::rend::TexHandle m_cellTextures[6];
    m3d::rend::TexHandle m_unsuitableBg;
    RepositoryWnd::RepositoryType m_type;
    RepositoryWnd::AuxInfo m_aif;
    bool m_isSpecialized;
    std::vector<int> m_compatibleResourceIds;
    std::map<int,int> m_itemStyles;
    int m_defaultItemStyle;
    int m_prevOverroledSlotId;
    std::vector<RepositoryWnd::TwinkleInfo> m_twinkleItems;
};

namespace RepositoryWnd
{
    class CellBunch
    {
    public:
        ~CellBunch();
        void Reset();
        CellBunch();
    protected:
    private:
        BoundsBase<int> m_bounds;
        SlotState m_state;
    };
}

namespace RepositoryWnd
{
    class TwinkleInfo
    {
    public:
        TwinkleInfo(int);
        void Invalidate();
        bool IsValid() const ;
    protected:
    private:
        int m_itemObjId;
        unsigned int m_startTime;
    };
}
