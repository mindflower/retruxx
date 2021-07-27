#pragma once

class LocalChartWnd :  public m3d::ui::ImageWnd
{
public:
    void EnableObjectNames(bool);
    static struct m3d::Class * __fastcall GetBaseClass();
    int SetUpForMap(CStr const &);
    static class m3d::Object * __fastcall CreateObject();
    void SelectMapMarkIcoForObjectInfo(class ObjectInfo const *);
    virtual struct m3d::Class * GetClass() const ;
    bool IsObjectNamesEnabled() const ;
    int CreateFromPattern(class m3d::ui::Wnd const *,bool);
    virtual ~LocalChartWnd();
    struct CVector WndPtToWorldPos(class PointBase<float> const &,CStr const &) const ;
    virtual class m3d::Object * Clone();
protected:
    int UpdateMapBg();
    int FullUpdate();
    void ClearMapMarkIcons();
    void OnAddNavPoint(void *);
    struct BoundsBase<float> GetCellWndBounds(CStr const &,int) const ;
    int UpdateMapMarkIcons();
    int DeleteNavPointMark(int);
    bool IsObjectToBeShown(class ObjectInfo const *) const ;
    void DrawCells(struct m3d::ui::DrawInfo const &) const ;
    virtual int OnPaint(struct m3d::ui::DrawInfo const &);
    void UpdateMapMarkNamesVisibility();
    struct BoundsBase<float> GetChartRect() const ;
    int AddNavPointMark(int);
    virtual int AddChild(class m3d::Object *);
    void AdjustChildrenOrder();
    void ClearMapMarkNames();
    int UpdateMapMarks();
    class PointBase<float> WorldPosToWndPt(struct CVector const &,CStr const &) const ;
    virtual int GameDataUpdate(void *,int);
    virtual int GameDataClear(bool);
    class NavPointMarkWnd * GetNavPointMarkByNavPointId(int) const ;
    void OnCurProfileChanged();
    virtual int OnMouseButton0(unsigned int,class PointBase<float> const &);
    virtual int OnMouseButton1(unsigned int,class PointBase<float> const &);
    int UpdatePlayerMark();
    void DrawVisibilityMap(struct m3d::ui::DrawInfo const &) const ;
    int UpdateMapMarkNames();
    void ClearNavPointMarks();
    LocalChartWnd();
    LocalChartWnd(class LocalChartWnd const &);
    void ClearMapMarks();
    bool HandleMouseClickOnMapMark(class PointBase<float> const &,class std::vector<class MapMarkWnd *,class std::allocator<class MapMarkWnd *> > const &);
    int GetGridSizeForLevel(CStr const &) const ;
    int UpdateNavPointMarks();
    class ObjectInfo const * GetCurrentObjectInfo() const ;
    struct CVector GetNorth(CStr const &) const ;
    void OnDeleteNavPoint(void *);
    bool IsObjectOnKnownZone(class ObjectInfo const *) const ;
    virtual int OnBeforeAddToWndStation();
private:
    std::vector<MapMarkWnd *> m_mapMarkIcons;
    std::vector<MapMarkWnd *> m_mapMarkNames;
    CStr m_levelName;
    LocalChartWnd::AuxInfo m_aif;
    m3d::rend::TexHandle m_cellBgTex;
    m3d::rend::TexHandle m_cellLineTex;
    PlayerMarkWnd *m_playerMark;
    std::vector<NavPointMarkWnd *> m_navPointMarks;
    bool m_bObjectNamesEnabled;
};
