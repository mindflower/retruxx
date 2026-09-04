#include "localchartwnd.h"

#include <cmath>

#include <config.h>
#include <core/kernel.h>
#include <game/m3dgame.h>
#include "game/profile.h"
#include "game/uimisc/guihelper.h"
#include "game/uimisc/levelinfo.h"
#include "game/uimisc/navpoint.h"
#include "game/uimisc/visibilitymap.h"
#include "game/uimanager/truxxuimanager.h"
#include "localmapwnd.h"
#include "mapmarkwnd.h"
#include "server/objects/player.h"
#include "server/objects/vehicle.h"

RT_CLASS_EXPORTS_BEGIN(LocalChartWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(LocalChartWnd);

int const LocalChartWnd::CELL_SIZE = 512;

LocalChartWnd::AuxInfo::AuxInfo()
{
    // RVA 0x4E3040
    m_cellBgTexName = "MapCellBg";
    m_cellLineTexName = "MapCellLine";
    m_cellLineThickW = 2.0f;
    m_cellLineThinW = 2.0f;
    m_nameColor = 0xFF404040u;
    m_nameShadowColor = 0xFF000000u;
    m_nameSpaceX = 2.0f;
    m_nameSpaceY = 0.0f;
    m_namePane = "PaneSmallTitle";
}

void LocalChartWnd::EnableObjectNames(bool bEnable)
{
    // RVA 0x4E56A0
    m_bObjectNamesEnabled = bEnable;

    m3d::AIParam data;
    CallParentNotify(0x1B, data, false);

    if (Profile* profile = M3D_APP->GetProfileManager()->GetCurProfile())
    {
        m3d::AIParam param{m_bObjectNamesEnabled ? 1 : 0};
        profile->SetParam(PP_MINIMAP_ENABLE_OBJECT_NAMES, param);
        UpdateMapMarkNamesVisibility();
    }
}

m3d::Class* LocalChartWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ImageWnd);
}

int LocalChartWnd::SetUpForMap(CStr const& levelName)
{
    // RVA 0x4E3950
    m_levelName = levelName;
    return m_levelName.empty() ? GameDataClear(false) : FullUpdate();
}

m3d::Object* LocalChartWnd::CreateObject()
{
    // RVA 0x4E3010
    return new LocalChartWnd;
}

void LocalChartWnd::SelectMapMarkIcoForObjectInfo(ObjectInfo const* objectInfo)
{
    // RVA 0x4E5800
    for (auto* mark : m_mapMarkIcons)
    {
        if (auto* ico = RT_DYNCAST(mark, MapMarkIcoWnd))
        {
            ico->Select(false);
        }
    }
    if (!objectInfo || objectInfo->GetLevelName() != m_levelName)
    {
        return;
    }
    for (auto* mark : m_mapMarkIcons)
    {
        auto* ico = RT_DYNCAST(mark, MapMarkIcoWnd);
        if (!ico)
        {
            continue;
        }
        ObjectInfo const* oi = ico->GetObjectInfo();
        if (oi && oi->GetName() == objectInfo->GetName())
        {
            ico->Select(true);
            return;
        }
    }
}

m3d::Class* LocalChartWnd::GetClass() const
{
    // RVA 0x4E3000
    return RT_CLASS_LOCAL(LocalChartWnd);
}

bool LocalChartWnd::IsObjectNamesEnabled() const
{
    // RVA 0x4E5760
    return m_bObjectNamesEnabled;
}

int LocalChartWnd::CreateFromPattern(m3d::ui::Wnd const* patternWnd, bool deleteSrc)
{
    // RVA 0x4E3440
    if (!patternWnd)
    {
        return 0;
    }

    // The shipped build copies the global "errormsg" placeholder as the window
    // text; it is a default-constructed CStr with no writer anywhere in the exe.
    if (!m3d::ui::Wnd::Create({}, patternWnd->GetStyle(), patternWnd->GetBounds(), patternWnd->GetId()))
    {
        return 0;
    }

    // Copy the state / appearance from the pattern control.
    SetStyle(patternWnd->GetStyle());
    SetText(patternWnd->GetText());
    SetId(patternWnd->GetId());
    SetName(patternWnd->GetName());
    SetBounds(patternWnd->GetBounds(), true);
    SetDefaultFont(patternWnd->GetDefaultFont());
    SetWrapMode(patternWnd->GetWrapMode());
    SetFormatMode(patternWnd->GetFormatMode());
    SetColor(patternWnd->GetColor());
    SetTextColor(patternWnd->GetTextColor());
    SetTextColorDisabled(patternWnd->GetTextColorDisabled());
    SetClientEdges(patternWnd->GetClientEdges());
    SetPane(patternWnd->GetPaneName());
    SetPaneFlags(patternWnd->GetPaneFlags());
    SetScrollPane(patternWnd->GetScrollPaneName());
    SetBackground(patternWnd->GetBackground());

    CStr tooltip;
    patternWnd->GetProperty(PROP_WND_TOOLTIP, &tooltip);
    SetProperty(PROP_WND_TOOLTIP, &tooltip);

    SetOnShowAnimation(patternWnd->GetOnShowAnimation());
    SetOnHideAnimation(patternWnd->GetOnHideAnimation());

    // Take the pattern's place in the window tree, then optionally destroy it.
    m3d::Object* parent = patternWnd->GetParent();
    if (!parent)
    {
        return 0;
    }
    parent->AddChild(this);
    if (deleteSrc)
    {
        delete const_cast<m3d::ui::Wnd*>(patternWnd);
    }

    ITruxxUiManager* uiMgr = M3D_APP->m_pInterfaceManager;
    m_cellBgTex = uiMgr->GetIcoByName(m_aif.m_cellBgTexName, 0);
    if (m_cellBgTex.IsValid())
    {
        M3D_RENDERER->ReferenceTexture(m_cellBgTex);
    }
    m_cellLineTex = uiMgr->GetIcoByName(m_aif.m_cellLineTexName, 0);
    if (m_cellLineTex.IsValid())
    {
        M3D_RENDERER->ReferenceTexture(m_cellLineTex);
    }

    m_gameDataFlags |= 1u;
    OnCurProfileChanged();
    return 1;
}

LocalChartWnd::~LocalChartWnd()
{
    // RVA 0x4E3310
    ClearMapMarkIcons();
    ClearMapMarkNames();
    ClearNavPointMarks();
    delete m_playerMark;
    if (m_cellBgTex.IsValid())
    {
        M3D_RENDERER->ReleaseTexture(m_cellBgTex);
    }
    if (m_cellLineTex.IsValid())
    {
        M3D_RENDERER->ReleaseTexture(m_cellLineTex);
    }
}

CVector LocalChartWnd::WndPtToWorldPos(PointBase<float> const& wndPt, CStr const& levelName) const
{
    // RVA 0x4E4420
    //
    // Maps a chart pixel back to a world position. NOTE: in the shipped build
    // this is *not* a clean inverse of WorldPosToWndPt - for the north.z
    // orientations it uses the opposite sign/origin and routes the value derived
    // from screen X into world X (rather than world Z), so a world->window->world
    // round trip is mirrored and axis-swapped. Reproduced faithfully; the only
    // caller is the right-click "drop user waypoint" path in
    // LocalMapWnd::OnWndNotify.
    BoundsBase<float> const chartRect = GetClientBounds();
    if (wndPt.x < chartRect.x0 || chartRect.x0 + chartRect.width <= wndPt.x || wndPt.y < chartRect.y0 ||
        chartRect.y0 + chartRect.height <= wndPt.y)
    {
        return CVector{0.0f, 0.0f, 0.0f};
    }

    float const clientX = wndPt.x - chartRect.x0;
    float const clientY = wndPt.y - chartRect.y0;
    CVector const north = GetNorth(levelName);
    float const levelSize = M3D_APP->m_pInterfaceManager->GetLevelInfoManager()->GetLevelSize(levelName);

    // Orientation table (its own, distinct from WorldPosToWndPt's).
    float originX = chartRect.width;
    float signX = -1.0f;
    float originY = 0.0f;
    float signY = 1.0f;
    if (north.z == -1.0f)  // north points -Z (also the fallback)
    {
        originX = chartRect.width;
        signX = -1.0f;
        originY = 0.0f;
        signY = 1.0f;
    }
    else if (north.z == 1.0f)  // north points +Z
    {
        originX = 0.0f;
        signX = 1.0f;
        originY = chartRect.height;
        signY = -1.0f;
    }
    else if (north.x == 1.0f)  // north points +X
    {
        originX = chartRect.width;
        signX = -1.0f;
        originY = chartRect.height;
        signY = -1.0f;
    }
    else if (north.x == -1.0f)  // north points -X
    {
        originX = 0.0f;
        signX = 1.0f;
        originY = 0.0f;
        signY = 1.0f;
    }

    float const along = chartRect.width != 0.0f ? (clientX * signX + originX) * levelSize / chartRect.width : 0.0f;
    float const across = chartRect.height != 0.0f ? (clientY * signY + originY) * levelSize / chartRect.height : 0.0f;

    CVector out{0.0f, 0.0f, 0.0f};
    if (north.x != 0.0f)
    {
        out.x = across;
        out.z = along;
    }
    else
    {
        out.x = along;
        out.z = across;
    }
    return out;
}

m3d::Object* LocalChartWnd::Clone()
{
    // RVA 0x4E2FC0
    return new LocalChartWnd(*this);
}

int LocalChartWnd::UpdateMapBg()
{
    // RVA 0x4E4050
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    LevelInfoManager* lim = M3D_APP->m_pInterfaceManager->GetLevelInfoManager();
    if (LevelInfo* li = lim->GetLevelInfoById(lim->GetLevelInfoId(m_levelName)))
    {
        SetImage(li->GetImage0());
        return 1;
    }
    SetImage(m3d::rend::TexHandle());
    return 0;
}

int LocalChartWnd::FullUpdate()
{
    // RVA 0x4E39A0
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    int res = 1;
    auto* vismap = M3D_APP->m_pInterfaceManager->GetLevelInfoManager()->GetVisibilityMapForLevel(m_levelName);
    if (vismap)
    {
        vismap->UpdateBg();
    }
    else
    {
        res = 0;
    }
    res &= UpdateMapBg();
    res &= UpdatePlayerMark();
    int const marks = UpdateMapMarkIcons() & 1;
    int const names = UpdateMapMarkNames() & marks;
    return res & names & UpdateNavPointMarks();
}

void LocalChartWnd::ClearMapMarkIcons()
{
    // RVA 0x4E3790
    for (auto* mark : m_mapMarkIcons)
    {
        delete mark;
    }
    m_mapMarkIcons.clear();
}

void LocalChartWnd::OnAddNavPoint(void* data)
{
    // RVA 0x4E4FF0
    if (data)
    {
        AddNavPointMark(static_cast<int*>(data)[13]);
    }
}

BoundsBase<float> LocalChartWnd::GetCellWndBounds(CStr const& levelName, int cellId) const
{
    // RVA 0x4E4E40
    int const grid = GetGridSizeForLevel(levelName);
    if (grid == 0)
    {
        return BoundsBase<float>{0.0f, 0.0f, 0.0f, 0.0f};
    }
    int const col = cellId % grid;
    int const row = cellId / grid;
    float const cell = static_cast<float>(CELL_SIZE);
    CVector const nearCorner{static_cast<float>(col) * cell, 0.0f, static_cast<float>(row) * cell};
    CVector const farCorner{static_cast<float>(col + 1) * cell, 0.0f, static_cast<float>(row + 1) * cell};

    PointBase<float> const farPt = WorldPosToWndPt(farCorner, levelName);
    PointBase<float> const nearPt = WorldPosToWndPt(nearCorner, levelName);

    BoundsBase<float> b;
    b.x0 = nearPt.x;
    b.y0 = farPt.y;
    b.width = farPt.x - nearPt.x;
    b.height = nearPt.y - farPt.y;
    return b;
}

int LocalChartWnd::UpdateMapMarkIcons()
{
    // RVA 0x4E3A40
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    ClearMapMarkIcons();

    LevelInfoManager* lim = M3D_APP->m_pInterfaceManager->GetLevelInfoManager();
    auto* objects = lim->GetObjectsForLevel(m_levelName);
    if (!objects)
    {
        return 0;
    }
    for (auto const& kv : *objects)
    {
        ObjectInfo* oi = kv.second;
        if (!oi || !IsObjectToBeShown(oi))
        {
            continue;
        }
        auto* ico = static_cast<MapMarkIcoWnd*>(M3D_KERNEL->New("MapMarkIcoWnd"));
        if (!ico)
        {
            continue;
        }
        PointBase<float> const pt = WorldPosToWndPt(oi->GetPosition(), m_levelName);
        if (ico->SetUp(oi, pt))
        {
            m_mapMarkIcons.push_back(ico);
            AddChild(ico);
        }
        else
        {
            delete ico;
        }
    }
    SelectMapMarkIcoForObjectInfo(GetCurrentObjectInfo());
    return 1;
}

int LocalChartWnd::DeleteNavPointMark(int npId)
{
    // RVA 0x4E51A0
    for (auto it = m_navPointMarks.begin(); it != m_navPointMarks.end(); ++it)
    {
        if (*it && (*it)->GetNavPointId() == npId)
        {
            delete *it;
            m_navPointMarks.erase(it);
            return 1;
        }
    }
    return 0;
}

bool LocalChartWnd::IsObjectToBeShown(ObjectInfo const* objectInfo) const
{
    // RVA 0x4E4200
    if (!objectInfo)
    {
        return false;
    }
    auto* vismap =
        M3D_APP->m_pInterfaceManager->GetLevelInfoManager()->GetVisibilityMapForLevel(objectInfo->GetLevelName());
    if (!vismap || !vismap->IsWorldPositionVisible(objectInfo->GetPosition()))
    {
        return false;
    }
    // Inactive locations stay hidden.
    return !objectInfo->IsLocation() || objectInfo->IsActive();
}

void LocalChartWnd::DrawCells(m3d::ui::DrawInfo const& di) const
{
    // RVA 0x4E4600
    int const grid = GetGridSizeForLevel(m_levelName);
    int const numCells = grid * grid;
    if (numCells <= 0)
    {
        return;
    }

    float const gridF = static_cast<float>(grid);
    // The line width alternates between the "thick" and "thin" AuxInfo values
    // each time a divider is emitted. NOTE: the shipped AuxInfo ships both as
    // 2.0f, so the toggle has no visible effect.
    float horLineW = m_aif.m_cellLineThickW;
    float vertLineW = m_aif.m_cellLineThickW;

    for (int i = 0; i < numCells; ++i)
    {
        BoundsBase<float> const cellB = GetCellWndBounds(m_levelName, i);
        int const row = i / grid;
        float const span = gridF * cellB.width;

        // One horizontal divider per row, along the row's far edge (skip the
        // last row). Emitted while walking the row's first column.
        if (i % grid == 0 && row < grid - 1)
        {
            horLineW = (horLineW == m_aif.m_cellLineThickW) ? m_aif.m_cellLineThinW : m_aif.m_cellLineThickW;

            BoundsBase<float> lineB;
            lineB.x0 = cellB.x0 + cellB.width - span;
            lineB.y0 = cellB.y0 + cellB.height;
            lineB.width = span;
            lineB.height = horLineW;
            GetGfxServer()->AddImagedRect(di, lineB, 0xFFFFFFFFu, m_cellLineTex);
        }

        // One vertical divider per column, along the column's near edge (skip
        // the last column). Emitted while walking the first row.
        if (row == 0 && i % grid < grid - 1)
        {
            vertLineW = (vertLineW == m_aif.m_cellLineThickW) ? m_aif.m_cellLineThinW : m_aif.m_cellLineThickW;

            BoundsBase<float> rect;
            rect.x0 = cellB.x0;
            rect.y0 = cellB.y0;
            rect.width = vertLineW;
            rect.height = span;
            GetGfxServer()->AddImagedRect(di, rect, 0xFFFFFFFFu, m_cellLineTex);
        }
    }
}

int LocalChartWnd::OnPaint(m3d::ui::DrawInfo const& di)
{
    // RVA 0x4E45D0
    ImageWnd::OnPaint(di);
    DrawVisibilityMap(di);
    DrawCells(di);
    return 1;
}

void LocalChartWnd::UpdateMapMarkNamesVisibility()
{
    // RVA 0x4E3C80
    for (auto* nameMark : m_mapMarkNames)
    {
        if (nameMark)
        {
            nameMark->ShowWindow(m_bObjectNamesEnabled);
        }
    }
}

BoundsBase<float> LocalChartWnd::GetChartRect() const
{
    // RVA 0x4E45C0
    return GetClientBounds();
}

int LocalChartWnd::AddNavPointMark(int npId)
{
    // RVA 0x4E5010
    if (m_levelName != help::GetCurrentLevelName())
    {
        return 1;
    }
    if (GetNavPointMarkByNavPointId(npId))
    {
        return 0;
    }

    NavPointManager* npm = M3D_APP->m_pInterfaceManager->GetNavPointManager();
    NavPoint const* np = npm->GetNavPointById(npId);
    if (!np || !np->IsValid() || np->GetLevelName() != m_levelName)
    {
        return 0;
    }
    CVector const* coord = np->GetCoordinate();
    if (!coord)
    {
        return 0;
    }

    auto* mark = static_cast<NavPointMarkWnd*>(M3D_KERNEL->New("NavPointMarkWnd"));
    if (!mark)
    {
        return 0;
    }
    PointBase<float> const pt = WorldPosToWndPt(*coord, m_levelName);
    if (!mark->CreateNavPointMarkWnd(pt, npId))
    {
        delete mark;
        return 0;
    }
    m_navPointMarks.push_back(mark);
    AddChild(mark);
    MoveChildToFirstPosition(mark);
    return 1;
}

int LocalChartWnd::AddChild(m3d::Object* node)
{
    // RVA 0x4E59E0
    int const res = m3d::ui::Wnd::AddChild(node);
    AdjustChildrenOrder();
    return res;
}

void LocalChartWnd::AdjustChildrenOrder()
{
    // RVA 0x4E5A00
    // Keep the object-name labels drawn on top of the icons.
    for (auto* nameMark : m_mapMarkNames)
    {
        if (nameMark && IsDirectChild(nameMark))
        {
            MoveChildToFirstPosition(nameMark);
        }
    }
}

void LocalChartWnd::ClearMapMarkNames()
{
    // RVA 0x4E3820
    for (auto* mark : m_mapMarkNames)
    {
        delete mark;
    }
    m_mapMarkNames.clear();
}

int LocalChartWnd::UpdateMapMarks()
{
    // RVA 0x4E3A20
    return (UpdateMapMarkIcons() & 1) & UpdateMapMarkNames();
}

PointBase<float> LocalChartWnd::WorldPosToWndPt(CVector const& worldPos, CStr const& levelName) const
{
    // RVA 0x4E4290
    //
    // Projects a world position onto the chart in client pixels. The shipped
    // build inlines an orientation table (keyed on the level's north vector)
    // that mirrors/rotates the projection so "north" points up on screen.
    // NOTE: the Hex-Rays output for this __userpurge came out with its return
    // struct field indices cross-wired; the reconstruction keeps the
    // dimensionally coherent wiring (screen X from the x0/width terms, screen Y
    // from the y0/height terms) - the orientation GetCellWndBounds relies on.
    BoundsBase<float> const chartRect = GetClientBounds();
    CVector const north = GetNorth(levelName);
    float const levelSize = M3D_APP->m_pInterfaceManager->GetLevelInfoManager()->GetLevelSize(levelName);
    float const invSize = levelSize != 0.0f ? 1.0f / levelSize : 0.0f;

    // Orientation: origin corner offset (client px) + per-axis sign (+1/-1).
    float originX = 0.0f;
    float signX = 1.0f;
    float originY = chartRect.height;
    float signY = -1.0f;
    if (north.z == -1.0f)  // north points -Z (also the fallback)
    {
        originX = 0.0f;
        signX = 1.0f;
        originY = chartRect.height;
        signY = -1.0f;
    }
    else if (north.z == 1.0f)  // north points +Z
    {
        originX = chartRect.width;
        signX = -1.0f;
        originY = 0.0f;
        signY = 1.0f;
    }
    else if (north.x == 1.0f)  // north points +X
    {
        originX = chartRect.width;
        signX = -1.0f;
        originY = chartRect.height;
        signY = -1.0f;
    }
    else if (north.x == -1.0f)  // north points -X
    {
        originX = 0.0f;
        signX = 1.0f;
        originY = 0.0f;
        signY = 1.0f;
    }

    // When north lies along world X, the X/Z axes feeding screen X/Y swap.
    bool const axesSwapped = north.x != 0.0f;
    float const feedX = axesSwapped ? worldPos.x : worldPos.z;
    float const feedY = axesSwapped ? worldPos.z : worldPos.x;

    PointBase<float> out;
    out.x = chartRect.x0 + (invSize * chartRect.width * feedX - originX) / signX;
    out.y = chartRect.y0 + (invSize * chartRect.height * feedY - originY) / signY;
    return out;
}

int LocalChartWnd::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x4E4F50
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    switch (dataType)
    {
    case 28:
        if (IsChildOf(M3D_APP) && data)
        {
            AddNavPointMark(static_cast<int*>(data)[13]);
        }
        break;
    case 29:
        if (IsChildOf(M3D_APP) && data)
        {
            DeleteNavPointMark(static_cast<int*>(data)[13]);
            return 1;
        }
        break;
    case 40:
        OnCurProfileChanged();
        return 1;
    }
    return 1;
}

int LocalChartWnd::GameDataClear(bool)
{
    // RVA 0x4E3740
    ClearMapMarkIcons();
    ClearMapMarkNames();
    ClearNavPointMarks();
    delete m_playerMark;
    m_playerMark = nullptr;
    return 1;
}

NavPointMarkWnd* LocalChartWnd::GetNavPointMarkByNavPointId(int npId) const
{
    // RVA 0x4E5210
    for (auto* mark : m_navPointMarks)
    {
        if (mark && mark->GetNavPointId() == npId)
        {
            return mark;
        }
    }
    return nullptr;
}

void LocalChartWnd::OnCurProfileChanged()
{
    // RVA 0x4E5620
    Profile* profile = M3D_APP->GetProfileManager()->GetCurProfile();
    if (!profile)
    {
        return;
    }
    m3d::AIParam param;
    if (profile->GetParam(PP_MINIMAP_ENABLE_OBJECT_NAMES, param))
    {
        EnableObjectNames(param.GetAsID() != 0);
    }
}

int LocalChartWnd::OnMouseButton0(unsigned state, PointBase<float> const& at)
{
    // RVA 0x4E52F0
    m3d::ui::Wnd::OnMouseButton0(state, at);
    if (state && !HandleMouseClickOnMapMark(at, m_mapMarkIcons))
    {
        HandleMouseClickOnMapMark(at, m_mapMarkNames);
    }
    return 1;
}

int LocalChartWnd::OnMouseButton1(unsigned state, PointBase<float> const& at)
{
    // RVA 0x4E5330
    m3d::ui::Wnd::OnMouseButton1(state, at);
    if (m_levelName != help::GetCurrentLevelName() || !state)
    {
        return 1;
    }

    for (auto* mark : m_navPointMarks)
    {
        if (!mark)
        {
            continue;
        }
        BoundsBase<float> const b = mark->GetBounds();
        if (at.x >= b.x0 && b.x0 + b.width > at.x && at.y >= b.y0 && b.y0 + b.height > at.y)
        {
            m3d::AIParam data{mark->GetNavPointId()};
            CallParentNotify(24, data, false);
            return 1;
        }
    }

    BoundsBase<float> const cb = GetClientBounds();
    if (at.x >= cb.x0 && cb.x0 + cb.width > at.x && at.y >= cb.y0 && cb.y0 + cb.height > at.y)
    {
        // NOTE: the shipped build sends an AIPARAM_RANGE payload that packs the
        // click's y coordinate into the z slot (offset 8); retruxx uses the
        // natural CVector2 packing since nothing consumes this yet.
        m3d::AIParam data{CVector2{at.x, at.y}};
        CallParentNotify(23, data, false);
    }
    return 1;
}

int LocalChartWnd::UpdatePlayerMark()
{
    // RVA 0x4E3E60
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    ai::Player* player = ai::thePlayer;
    bool const onThisLevel = player && player->GetVehicle() && m_levelName == help::GetCurrentLevelName();

    if (!onThisLevel)
    {
        delete m_playerMark;
        m_playerMark = nullptr;
        return 1;
    }

    if (!m_playerMark)
    {
        m_playerMark = static_cast<PlayerMarkWnd*>(M3D_KERNEL->New("PlayerMarkWnd"));
        if (!m_playerMark)
        {
            return 0;
        }
        if (!m_playerMark->CreatePlayerMarkWnd())
        {
            delete m_playerMark;
            m_playerMark = nullptr;
            return 0;
        }
        AddChild(m_playerMark);
    }

    if (ai::Vehicle* vehicle = ai::thePlayer ? ai::thePlayer->GetVehicle() : nullptr)
    {
        CVector const pos = vehicle->GetPosition();
        m_playerMark->SetImageCoords(WorldPosToWndPt(pos, m_levelName));

        // NOTE: the shipped build first rotates the normalized direction by the
        // level's north vector (anonymous-namespace HackedConvertCoordinate0)
        // before taking the heading; that conversion is folded into the chart
        // geometry TODO cluster (WorldPosToWndPt / GetNorth).
        CVector const dir = vehicle->GetDirection().getNormalized();
        m_playerMark->SetImageAngle(std::atan2(dir.x, -dir.z));
    }
    return 1;
}

void LocalChartWnd::DrawVisibilityMap(m3d::ui::DrawInfo const& di) const
{
    // RVA 0x4E47E0
    namespace rend = m3d::rend;

    VisibilityMap* vismap = M3D_APP->m_pInterfaceManager->GetLevelInfoManager()->GetVisibilityMapForLevel(m_levelName);
    if (!vismap)
    {
        return;
    }

    M3D_RENDERER->SetAlphaTest(M3D_ENGINE_CFG.m_alphaTestInterface.GetI());
    M3D_RENDERER->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
    M3D_RENDERER->SetStageState(0, rend::BM_ALPHA, rend::TS_MODULATE);
    M3D_RENDERER->SetStageState(1, rend::BM_COLOR, rend::TS_NONE);
    // NOTE: the shipped build repeats the previous call verbatim here (likely a
    // copy/paste of a BM_ALPHA line); reproduced as-is.
    M3D_RENDERER->SetStageState(1, rend::BM_COLOR, rend::TS_NONE);
    M3D_RENDERER->PushBlend(rend::BM_ALPHA);
    M3D_RENDERER->PushZbState(rend::ZB_DISABLE);

    BoundsBase<float> const clientB = GetClientBounds();
    m3d::rend::TexHandle visibilityTex = vismap->GetBg();
    CVector const north = GetNorth(m_levelName);

    float cx = (di.m_originalRect.x0 * 2.0f + di.m_originalRect.width) * 0.5f;
    float cy = (di.m_originalRect.y0 * 2.0f + di.m_originalRect.height) * 0.5f;

    float const hw = clientB.width * 0.5f;
    float const hh = clientB.height * 0.5f;

    // Quad corners, centred on the origin.
    float x0 = -hw, y0 = -hh;
    float x1 = hw, y1 = -hh;
    float x2 = hw, y2 = hh;
    float x3 = -hw, y3 = hh;

    float u0 = 0.0f, v0 = 0.0f;
    float u1 = 1.0f, v1 = 0.0f;
    float u2 = 1.0f, v2 = 1.0f;
    float u3 = 0.0f, v3 = 1.0f;

    // NOTE: the shipped rotation angle is float(pi/2) negated, and cos of it is
    // a tiny non-zero float, so the rotated quad carries a small skew - kept.
    float const kQuarterTurn = -1.5707963705f;
    auto const rotate = [kQuarterTurn](float& px, float& py)
    {
        float const s = std::sin(kQuarterTurn);
        float const c = std::cos(kQuarterTurn);
        float const nx = c * px - s * py;
        float const ny = s * px + c * py;
        px = nx;
        py = ny;
    };

    if (north.z == 1.0f)
    {
        v0 = 1.0f;
        v1 = 1.0f;
        v2 = 0.0f;
        v3 = 0.0f;
    }
    else if (north.z == -1.0f)
    {
        u0 = 1.0f;
        u1 = 0.0f;
        u2 = 0.0f;
        u3 = 1.0f;
    }
    else if (north.x == 1.0f)
    {
        v0 = 1.0f;
        v1 = 1.0f;
        v2 = 0.0f;
        v3 = 0.0f;
        rotate(x0, y0);
        rotate(x1, y1);
        rotate(x2, y2);
        rotate(x3, y3);
    }
    else if (north.x == -1.0f)
    {
        rotate(x0, y0);
        rotate(x1, y1);
        rotate(x2, y2);
        rotate(x3, y3);
        u0 = 1.0f;
        u1 = 0.0f;
        u2 = 0.0f;
        u3 = 1.0f;
    }

    M3D_RENDERER->RelToAbs(cx, cy);
    M3D_RENDERER->RelToAbs(x0, y0);
    M3D_RENDERER->RelToAbs(x1, y1);
    M3D_RENDERER->RelToAbs(x2, y2);
    M3D_RENDERER->RelToAbs(x3, y3);

    if (!visibilityTex.IsValid())
    {
        M3D_RENDERER->SetWhiteTexture(0);
    }
    else
    {
        M3D_RENDERER->SetTexture(0, visibilityTex, -1.0);
    }

    M3D_APP->PutSprite2Abs(
        x0 + cx,
        y0 + cy,
        x1 + cx,
        y1 + cy,
        x3 + cx,
        y3 + cy,
        x2 + cx,
        y2 + cy,
        u0,
        v0,
        u1,
        v1,
        u3,
        v3,
        u2,
        v2,
        0.0f,
        0xFFFFFFFFu);

    M3D_RENDERER->SetAlphaTest(0);
    M3D_RENDERER->PopBlend();
    M3D_RENDERER->PopZbState();
}

int LocalChartWnd::UpdateMapMarkNames()
{
    // RVA 0x4E3B50
    BoundsBase<float> const clientB = GetClientBounds();
    int res = 1;
    ClearMapMarkNames();

    for (auto* icon : m_mapMarkIcons)
    {
        if (!icon)
        {
            res = 0;
            continue;
        }
        ObjectInfo* oi = icon->GetObjectInfo();
        if (!oi)
        {
            res = 0;
            continue;
        }
        auto* name = static_cast<MapMarkNameWnd*>(M3D_KERNEL->New("MapMarkNameWnd"));
        if (!name)
        {
            res = 0;
            continue;
        }
        BoundsBase<float> const iconB = icon->GetBounds();
        if (name->SetUp(oi, iconB, clientB))
        {
            m_mapMarkNames.push_back(name);
            AddChild(name);
        }
        else
        {
            res = 0;
            delete name;
        }
    }
    UpdateMapMarkNamesVisibility();
    return res;
}

void LocalChartWnd::ClearNavPointMarks()
{
    // RVA 0x4E38B0
    for (auto* mark : m_navPointMarks)
    {
        delete mark;
    }
    m_navPointMarks.clear();
}

LocalChartWnd::LocalChartWnd()
{
    // RVA 0x4E3150
    m_playerMark = nullptr;
    m_bObjectNamesEnabled = true;
    m_bDrawPane = true;
}

LocalChartWnd::LocalChartWnd(LocalChartWnd const& rhs) : ImageWnd(rhs)
{
    // RVA 0x4E3290: fresh mark/nav-point vectors and invalid cell textures.
    // NOTE: the shipped copy ctor also leaves m_playerMark, m_bObjectNamesEnabled
    // and m_bDrawPane uninitialised; set to safe values here.
    m_cellBgTex.SetInvalid();
    m_cellLineTex.SetInvalid();
    m_playerMark = nullptr;
    m_bObjectNamesEnabled = true;
}

void LocalChartWnd::ClearMapMarks()
{
    // RVA 0x4E3780
    ClearMapMarkIcons();
    ClearMapMarkNames();
}

bool LocalChartWnd::HandleMouseClickOnMapMark(
    PointBase<float> const& pt,
    std::vector<MapMarkWnd*, std::allocator<MapMarkWnd*>> const& mapMarks)
{
    // RVA 0x4E5520 (inlined into OnMouseButton0 in the shipped build)
    for (auto* mark : mapMarks)
    {
        if (!mark)
        {
            continue;
        }
        BoundsBase<float> const b = mark->GetBounds();
        if (pt.x >= b.x0 && b.x0 + b.width > pt.x && pt.y >= b.y0 && b.y0 + b.height > pt.y)
        {
            if ((mark->GetStyle() & 0x200) != 0)
            {
                // The shipped build forwards the ObjectInfo pointer as an id payload.
                m3d::AIParam data{reinterpret_cast<int>(mark->GetObjectInfo())};
                CallParentNotify(0x19, data, false);
            }
            return true;
        }
    }
    return false;
}

int LocalChartWnd::GetGridSizeForLevel(CStr const& levelName) const
{
    // RVA 0x4E5270
    float const levelSize = M3D_APP->m_pInterfaceManager->GetLevelInfoManager()->GetLevelSize(levelName);
    return static_cast<int>(levelSize * (1.0f / static_cast<float>(CELL_SIZE)));
}

int LocalChartWnd::UpdateNavPointMarks()
{
    // RVA 0x4E40E0
    int res = 1;
    ClearNavPointMarks();
    if (m_levelName != help::GetCurrentLevelName())
    {
        return 1;
    }
    NavPointManager* npm = M3D_APP->m_pInterfaceManager->GetNavPointManager();
    std::vector<int> const navPointIds = npm->GetNavPointsForLevel(m_levelName);
    for (int npId : navPointIds)
    {
        if (!AddNavPointMark(npId))
        {
            res = 0;
        }
    }
    return res;
}

ObjectInfo const* LocalChartWnd::GetCurrentObjectInfo() const
{
    // RVA 0x4E5940
    ref_ptr<m3d::ui::Wnd> wndLocalMap = M3D_APP->m_pInterfaceManager->GetWindow(82);
    if (!wndLocalMap)
    {
        return nullptr;
    }
    auto* localMap = RT_DYNCAST(wndLocalMap.get(), LocalMapWnd);
    if (!localMap)
    {
        return nullptr;
    }
    return localMap->GetCurrentObjectInfo();
}

CVector LocalChartWnd::GetNorth(CStr const& levelName) const
{
    // RVA 0x4E5770
    LevelInfoManager* lim = M3D_APP->m_pInterfaceManager->GetLevelInfoManager();
    if (LevelInfo* li = lim->GetLevelInfoById(lim->GetLevelInfoId(levelName)))
    {
        return li->GetNorth();
    }
    return CVector{0.0f, 0.0f, -1.0f};
}

void LocalChartWnd::OnDeleteNavPoint(void* data)
{
    // RVA 0x4E5000
    if (data)
    {
        DeleteNavPointMark(static_cast<int*>(data)[13]);
    }
}

bool LocalChartWnd::IsObjectOnKnownZone(ObjectInfo const* objectInfo) const
{
    // RVA 0x4E4250
    if (!objectInfo)
    {
        return false;
    }
    auto* vismap =
        M3D_APP->m_pInterfaceManager->GetLevelInfoManager()->GetVisibilityMapForLevel(objectInfo->GetLevelName());
    return vismap && vismap->IsWorldPositionVisible(objectInfo->GetPosition());
}

int LocalChartWnd::OnBeforeAddToWndStation()
{
    // RVA 0x4E3940
    FullUpdate();
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}
