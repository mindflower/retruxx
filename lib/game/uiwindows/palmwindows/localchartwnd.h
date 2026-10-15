#pragma once
#include <ui/image.h>

class PlayerMarkWnd;
class MapMarkWnd;
class NavPointMarkWnd;
class ObjectInfo;

class LocalChartWnd : public m3d::ui::ImageWnd
{
public:
    int CreateFromPattern(m3d::ui::Wnd const* patternWnd, bool deleteSrc);
    int SetUpForMap(CStr const& levelName);
    CVector WndPtToWorldPos(PointBase<float> const& wndPt, CStr const& levelName) const;
    void EnableObjectNames(bool bEnable);
    bool IsObjectNamesEnabled() const;
    void SelectMapMarkIcoForObjectInfo(ObjectInfo const* objectInfo);

    using MapMarkVector = std::vector<MapMarkWnd*, std::allocator<MapMarkWnd*>>;
    using NavPointMarkVector = std::vector<NavPointMarkWnd*, std::allocator<NavPointMarkWnd*>>;

    struct AuxInfo
    {
        CStr m_cellBgTexName;
        CStr m_cellLineTexName;
        float m_cellLineThickW;
        float m_cellLineThinW;
        unsigned int m_nameColor;
        unsigned int m_nameShadowColor;
        float m_nameSpaceX;
        float m_nameSpaceY;
        CStr m_namePane;
        AuxInfo(LocalChartWnd::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int OnBeforeAddToWndStation() override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual int OnPaint(m3d::ui::DrawInfo const& di) override;
    virtual int OnMouseButton0(unsigned int state, PointBase<float> const& at) override;
    virtual int OnMouseButton1(unsigned int state, PointBase<float> const& at) override;
    virtual int AddChild(m3d::Object* node) override;
    int FullUpdate();
    int UpdateMapMarks();
    int UpdateMapMarkIcons();
    int UpdateMapMarkNames();
    int UpdatePlayerMark();
    int UpdateNavPointMarks();
    int UpdateMapBg();
    bool IsObjectToBeShown(ObjectInfo const* objectInfo) const;
    bool IsObjectOnKnownZone(ObjectInfo const* objectInfo) const;
    void ClearMapMarks();
    void ClearMapMarkIcons();
    void ClearMapMarkNames();
    void UpdateMapMarkNamesVisibility();
    void ClearNavPointMarks();
    PointBase<float> WorldPosToWndPt(CVector const& worldPos, CStr const& levelName) const;
    BoundsBase<float> GetChartRect() const;
    void DrawCells(m3d::ui::DrawInfo const& di) const;
    void DrawVisibilityMap(m3d::ui::DrawInfo const& di) const;
    BoundsBase<float> GetCellWndBounds(CStr const& levelName, int cellId) const;
    void OnAddNavPoint(void* data);
    void OnDeleteNavPoint(void* data);
    int AddNavPointMark(int npId);
    int DeleteNavPointMark(int npId);
    NavPointMarkWnd* GetNavPointMarkByNavPointId(int npId) const;
    int GetGridSizeForLevel(CStr const& levelName) const;
    void OnCurProfileChanged();
    CVector GetNorth(CStr const& levelName) const;
    ObjectInfo const* GetCurrentObjectInfo() const;
    bool HandleMouseClickOnMapMark(
        PointBase<float> const& pt,
        std::vector<MapMarkWnd*, std::allocator<MapMarkWnd*>> const& mapMarks);
    void AdjustChildrenOrder();
    static int const CELL_SIZE;
    std::vector<MapMarkWnd*, std::allocator<MapMarkWnd*>> m_mapMarkIcons;
    std::vector<MapMarkWnd*, std::allocator<MapMarkWnd*>> m_mapMarkNames;
    CStr m_levelName;
    LocalChartWnd::AuxInfo m_aif;
    m3d::rend::TexHandle m_cellBgTex;
    m3d::rend::TexHandle m_cellLineTex;
    PlayerMarkWnd* m_playerMark;
    std::vector<NavPointMarkWnd*, std::allocator<NavPointMarkWnd*>> m_navPointMarks;
    bool m_bObjectNamesEnabled;
    LocalChartWnd();
    LocalChartWnd(LocalChartWnd const& rhs);

public:
    virtual ~LocalChartWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classLocalChartWnd;
};
