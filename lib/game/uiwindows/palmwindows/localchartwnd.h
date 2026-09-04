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
        /* 0x0000 */ CStr m_cellBgTexName;
        /* 0x000c */ CStr m_cellLineTexName;
        /* 0x0018 */ float m_cellLineThickW;
        /* 0x001c */ float m_cellLineThinW;
        /* 0x0020 */ unsigned int m_nameColor;
        /* 0x0024 */ unsigned int m_nameShadowColor;
        /* 0x0028 */ float m_nameSpaceX;
        /* 0x002c */ float m_nameSpaceY;
        /* 0x0030 */ CStr m_namePane;
        AuxInfo(LocalChartWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x003c */

protected:
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x00 */;
    virtual int OnBeforeAddToWndStation() override /* 0x00 */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x00 */;
    virtual int OnPaint(m3d::ui::DrawInfo const& di) override /* 0x88 */;
    virtual int OnMouseButton0(unsigned int state, PointBase<float> const& at) override /* 0x00 */;
    virtual int OnMouseButton1(unsigned int state, PointBase<float> const& at) override /* 0x00 */;
    virtual int AddChild(m3d::Object* node) override /* 0x00 */;
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
    /* 0x0234 */ std::vector<MapMarkWnd*, std::allocator<MapMarkWnd*>> m_mapMarkIcons;
    /* 0x0244 */ std::vector<MapMarkWnd*, std::allocator<MapMarkWnd*>> m_mapMarkNames;
    /* 0x0254 */ CStr m_levelName;
    /* 0x0260 */ LocalChartWnd::AuxInfo m_aif;
    /* 0x029c */ m3d::rend::TexHandle m_cellBgTex;
    /* 0x02a0 */ m3d::rend::TexHandle m_cellLineTex;
    /* 0x02a4 */ PlayerMarkWnd* m_playerMark;
    /* 0x02a8 */ std::vector<NavPointMarkWnd*, std::allocator<NavPointMarkWnd*>> m_navPointMarks;
    /* 0x02b8 */ bool m_bObjectNamesEnabled;
    LocalChartWnd();
    LocalChartWnd(LocalChartWnd const& rhs);

public:
    virtual ~LocalChartWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classLocalChartWnd;
}; /* size: 0x02bc */
