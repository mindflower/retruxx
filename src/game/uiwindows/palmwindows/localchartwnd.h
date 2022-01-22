#pragma once
#include <ui/image.h>

class PlayerMarkWnd;

class LocalChartWnd :  public m3d::ui::ImageWnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_cellBgTexName;
        CStr m_cellLineTexName;
        float m_cellLineThickW;
        float m_cellLineThinW;
        unsigned int m_nameColor;
        unsigned int m_nameShadowColor;
        float m_nameSpaceX;
        float m_nameSpaceY;
        CStr m_namePane;
    };

public:
    void EnableObjectNames(bool);
    static m3d::Class * GetBaseClass();
    int SetUpForMap(CStr const &);
    static m3d::Object * CreateObject();
    void SelectMapMarkIcoForObjectInfo(class ObjectInfo const *);
    virtual m3d::Class * GetClass() const ;
    bool IsObjectNamesEnabled() const ;
    int CreateFromPattern(m3d::ui::Wnd const *,bool);
    virtual ~LocalChartWnd();
    CVector WndPtToWorldPos(PointBase<float> const &,CStr const &) const ;
    virtual m3d::Object * Clone();

protected:
    int UpdateMapBg();
    int FullUpdate();
    void ClearMapMarkIcons();
    void OnAddNavPoint(void *);
    BoundsBase<float> GetCellWndBounds(CStr const &,int) const ;
    int UpdateMapMarkIcons();
    int DeleteNavPointMark(int);
    bool IsObjectToBeShown(ObjectInfo const *) const ;
    void DrawCells(m3d::ui::DrawInfo const &) const ;
    virtual int OnPaint(m3d::ui::DrawInfo const &);
    void UpdateMapMarkNamesVisibility();
    BoundsBase<float> GetChartRect() const ;
    int AddNavPointMark(int);
    virtual int AddChild(m3d::Object *);
    void AdjustChildrenOrder();
    void ClearMapMarkNames();
    int UpdateMapMarks();
    PointBase<float> WorldPosToWndPt(CVector const &,CStr const &) const ;
    virtual int GameDataUpdate(void *,int);
    virtual int GameDataClear(bool);
    class NavPointMarkWnd * GetNavPointMarkByNavPointId(int) const ;
    void OnCurProfileChanged();
    virtual int OnMouseButton0(unsigned int, PointBase<float> const &);
    virtual int OnMouseButton1(unsigned int, PointBase<float> const &);
    int UpdatePlayerMark();
    void DrawVisibilityMap(m3d::ui::DrawInfo const &) const ;
    int UpdateMapMarkNames();
    void ClearNavPointMarks();
    LocalChartWnd();
    LocalChartWnd(LocalChartWnd const &);
    void ClearMapMarks();
    bool HandleMouseClickOnMapMark(PointBase<float> const &, std::vector<class MapMarkWnd *, std::allocator<MapMarkWnd *> > const &);
    int GetGridSizeForLevel(CStr const &) const ;
    int UpdateNavPointMarks();
    ObjectInfo const * GetCurrentObjectInfo() const ;
    CVector GetNorth(CStr const &) const ;
    void OnDeleteNavPoint(void *);
    bool IsObjectOnKnownZone(ObjectInfo const *) const ;
    virtual int OnBeforeAddToWndStation();

public:
    RT_CLASS_DECLARE(LocalChartWnd);

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
