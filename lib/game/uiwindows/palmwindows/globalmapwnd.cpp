#include "globalmapwnd.h"

#include <algorithm>
#include <vector>

#include <client.h>
#include <config.h>
#include <core/kernel.h>
#include <core/log.h>
#include <game/m3dgame.h>
#include <game/uimisc/levelinfo.h>
#include <level.h>
#include <m3dapp.h>
#include <server/objects/location.h>
#include <ui/ui_srv.h>
#include <world.h>

namespace
{
    // The level icons are built out of the XML pattern windows exactly once, the
    // first time a level actually starts.
    bool g_bFirstTime = true;

    // Name of the level the player is standing on, or empty when there is no
    // level loaded yet.
    CStr CurrentLevelName()
    {
        if (m3d::pClient)
        {
            if (auto* level = m3d::pClient->GetWorld().m_level)
            {
                return level->GetLevelName();
            }
        }
        return {};
    }
}  // namespace

// ---------------------------------------------------------------------------
//  LevelConnectionsWnd
// ---------------------------------------------------------------------------

RT_CLASS_EXPORTS_BEGIN(LevelConnectionsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(LevelConnectionsWnd);

LevelConnectionsWnd::ConnectionDrawInfo::ConnectionDrawInfo(PointBase<float> const& pt0, PointBase<float> const& pt1) :
    m_pt0(pt0),
    m_pt1(pt1)
{
}

LevelConnectionsWnd::ConnectionDrawInfo::ConnectionDrawInfo(ConnectionDrawInfo const& that) :
    m_pt0(that.m_pt0),
    m_pt1(that.m_pt1)
{
}

LevelConnectionsWnd::AuxInfo::AuxInfo()
{
    // RVA 0x4DCA70
    m_connectionTexName = "Connection";
    m_connectionH = 6.0f;
}

LevelConnectionsWnd::AuxInfo::AuxInfo(AuxInfo const& rhs) :
    m_connectionTexName(rhs.m_connectionTexName),
    m_connectionH(rhs.m_connectionH)
{
}

LevelConnectionsWnd::LevelConnectionsWnd()
{
}

LevelConnectionsWnd::LevelConnectionsWnd(LevelConnectionsWnd const&) : LevelConnectionsWnd()
{
}

LevelConnectionsWnd::~LevelConnectionsWnd() = default;

m3d::Object* LevelConnectionsWnd::Clone()
{
    return new LevelConnectionsWnd(*this);
}

m3d::Object* LevelConnectionsWnd::CreateObject()
{
    return new LevelConnectionsWnd;
}

m3d::Class* LevelConnectionsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* LevelConnectionsWnd::GetClass() const
{
    return RT_CLASS_LOCAL(LevelConnectionsWnd);
}

void LevelConnectionsWnd::SetDrawInfo(
    std::vector<LevelConnectionsWnd::ConnectionDrawInfo, std::allocator<LevelConnectionsWnd::ConnectionDrawInfo>> const&
        connectionDrawInfo)
{
    m_connectionDrawInfo = connectionDrawInfo;
}

int LevelConnectionsWnd::OnPaint(m3d::ui::DrawInfo const& di)
{
    // RVA 0x4DCC80
    DrawConnections(di);
    return 1;
}

void LevelConnectionsWnd::DrawConnections(m3d::ui::DrawInfo const& di)
{
    // RVA 0x4DCCA0 - each connection is one stretched, rotated sprite laid along
    // the line between the two level icons.
    M3D_RENDERER->SetAlphaTest(M3D_ENGINE_CFG.m_alphaTestInterface.GetI());
    M3D_RENDERER->SetStageState(0, m3d::rend::BM_COLOR, m3d::rend::TS_MODULATE);
    M3D_RENDERER->SetStageState(0, m3d::rend::BM_ALPHA, m3d::rend::TS_MODULATE);
    M3D_RENDERER->SetStageState(1, m3d::rend::BM_COLOR, m3d::rend::TS_NONE);
    // NOTE: the shipped build sets stage 1's BM_COLOR state twice (the second
    // was plausibly meant to be BM_ALPHA, as in the stage-0 pair); preserved.
    M3D_RENDERER->SetStageState(1, m3d::rend::BM_COLOR, m3d::rend::TS_NONE);
    M3D_RENDERER->PushBlend(m3d::rend::BM_ALPHA);
    M3D_RENDERER->PushZbState(m3d::rend::ZB_DISABLE);

    float const halfH = m_aif.m_connectionH * 0.5f;
    float const xEdge = di.m_originalRect.x0;
    float const yEdge = di.m_originalRect.y0;

    for (unsigned i = 0; i < m_connectionDrawInfo.size(); ++i)
    {
        auto const& cdi = m_connectionDrawInfo[i];
        float const dx = cdi.m_pt0.x - cdi.m_pt1.x;
        float const dy = cdi.m_pt0.y - cdi.m_pt1.y;

        float const midX = cdi.m_pt0.x + (cdi.m_pt1.x - cdi.m_pt0.x) * 0.5f;
        float const midY = cdi.m_pt0.y + (cdi.m_pt1.y - cdi.m_pt0.y) * 0.5f;
        float const halfW = static_cast<float>(std::sqrt(dy * dy + dx * dx)) * 0.5f;
        float const angle = -static_cast<float>(std::atan2(dy, -dx));

        if (!m_connectionTex.IsValid())
        {
            M3D_RENDERER->SetWhiteTexture(0);
        }
        else
        {
            M3D_RENDERER->SetTexture(0, m_connectionTex, -1.0);
        }

        M3D_APP->PutSpriteRelRot(
            midX + xEdge, midY + yEdge, halfW, halfH, 0xFFFFFFFF, angle, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    }

    M3D_RENDERER->SetAlphaTest(0);
    M3D_RENDERER->PopBlend();
    M3D_RENDERER->PopZbState();
}

// ---------------------------------------------------------------------------
//  GlobalMapWnd
// ---------------------------------------------------------------------------

RT_CLASS_EXPORTS_BEGIN(GlobalMapWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(GlobalMapWnd);

GlobalMapWnd::AuxInfo::AuxInfo()
{
    // RVA 0x4DA8A0
    m_levelIcoName = "_ico";
    m_wndChartName = "wndChart";
}

GlobalMapWnd::AuxInfo::AuxInfo(AuxInfo const& rhs) :
    m_levelIcoName(rhs.m_levelIcoName),
    m_wndChartName(rhs.m_wndChartName)
{
}

GlobalMapWnd::GlobalMapWnd() : m_wndConnections(nullptr)
{
}

GlobalMapWnd::GlobalMapWnd(GlobalMapWnd const&) : GlobalMapWnd()
{
}

GlobalMapWnd::~GlobalMapWnd() = default;

m3d::Object* GlobalMapWnd::Clone()
{
    return new GlobalMapWnd(*this);
}

m3d::Object* GlobalMapWnd::CreateObject()
{
    return new GlobalMapWnd;
}

m3d::Class* GlobalMapWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ScreenWnd);
}

m3d::Class* GlobalMapWnd::GetClass() const
{
    return RT_CLASS_LOCAL(GlobalMapWnd);
}

int GlobalMapWnd::GameDataSetup()
{
    // RVA 0x4DABF0 - the level-icon patterns come from XML as ordinary child
    // windows named "<level>_ico"; hide them all, then build real LevelIcos out
    // of them lazily in InitLevelIcons.
    if ((m_gameDataFlags & 2) != 0)
    {
        return 1;
    }
    for (m3d::Object* child = GetFirstChild(); child; child = child->GetNextSibling())
    {
        if (child->IsKindOf(&m3d::ui::Wnd::m_classWnd) &&
            strstr(child->GetName(), m_aif.m_levelIcoName.c_str()) != nullptr)
        {
            static_cast<m3d::ui::Wnd*>(child)->ShowWindow(false);
        }
    }
    return CreateConnectionsWnd() & 1;
}

int GlobalMapWnd::GameDataClear(bool beforeContinuousLevel)
{
    // RVA 0x4DABD0 - a continuous level keeps the map it already drew.
    if (!beforeContinuousLevel)
    {
        ClearConnections();
    }
    return 1;
}

int GlobalMapWnd::GameDataUpdate(void*, int dataType)
{
    // RVA 0x4DB620
    if (dataType == 85)
    {
        OnStartLevel();
        return 1;
    }
    if (dataType == 71)
    {
        OnLocationStateChanged();
        return 1;
    }
    return 1;
}

void GlobalMapWnd::OnStartLevel()
{
    // RVA 0x4DB660
    if (g_bFirstTime)
    {
        InitLevelIcons();
        g_bFirstTime = false;
    }
    UpdateConnections();
}

void GlobalMapWnd::OnLocationStateChanged()
{
    // RVA 0x4DBE30
    UpdateConnections();
}

int GlobalMapWnd::InitLevelIcons()
{
    // RVA 0x4DACC0 - replace each "<level>_ico" pattern window with a real
    // LevelIco bound to that level.
    if ((m_gameDataFlags & 1) != 0)
    {
        return 0;
    }

    int res = 1;
    retruxx::vector<CStr> levelNames;
    M3D_APP->m_pInterfaceManager->GetLevelInfoManager()->GetAllLevelNames(levelNames);

    for (unsigned i = 0; i < levelNames.size(); ++i)
    {
        CStr const& levelName = levelNames[i];
        CStr const levelIcoName = levelName + m_aif.m_levelIcoName;

        m3d::Object* pattern = GetChildByName(levelIcoName);
        if (!pattern || !pattern->IsKindOf(&m3d::ui::ImageWnd::m_classImageWnd))
        {
            continue;
        }

        ref_ptr<LevelIco> levelIco = static_cast<LevelIco*>(m3d::g_Kernel->New("LevelIco"));
        if (!levelIco->CreateFromPattern(static_cast<m3d::ui::Wnd*>(pattern), true))
        {
            M3D_LOG_INFO("Make control error: cannot create " + levelIcoName + " from pattern class");
            res = 0;
        }

        if (!levelIco->SetUpForLevel(levelName))
        {
            continue;
        }

        int guiId = -1;
        if (!M3D_APP->m_pInterfaceManager->AddWindow(levelIco.get(), guiId, true, 0))
        {
            M3D_LOG_INFO("Make control error: cannot add " + levelIcoName + " to interface manager");
            res = 0;
            continue;
        }

        // 37 = location state changed, 85 = level started.
        std::vector<int> events;
        events.push_back(37);
        events.push_back(85);
        M3D_APP->m_pInterfaceManager->SetEventsForWindow(levelIco->GetGuiId(), events);

        m_levelIcons[levelName] = levelIco;
    }
    return res;
}

int GlobalMapWnd::CreateConnectionsWnd()
{
    // RVA 0x4DBF60 - one full-chart-sized window sitting behind the icons, on
    // which the connecting lines are drawn.
    if (m_wndConnections)
    {
        return 1;
    }

    m3d::Object* chart = GetChildByName(m_aif.m_wndChartName);
    if (!chart || !chart->IsKindOf(&m3d::ui::Wnd::m_classWnd))
    {
        return 0;
    }
    auto* chartWnd = static_cast<m3d::ui::Wnd*>(chart);

    m_wndConnections = static_cast<LevelConnectionsWnd*>(m3d::g_Kernel->New("LevelConnectionsWnd"));
    if (!m_wndConnections)
    {
        return 0;
    }

    // Positioned in this window's space, at the chart's offset.
    BoundsBase<float> b = chartWnd->GetBounds();
    auto const chartOrigin = chartWnd->GetOrigin();
    b.x0 += chartOrigin.x;
    b.y0 += chartOrigin.y;

    if (m_wndConnections->Create({}, 832, b, 0) == 0)
    {
        delete m_wndConnections;
        m_wndConnections = nullptr;
        return 0;
    }
    AddChild(m_wndConnections);
    MoveChildToFirstPosition(m_wndConnections);
    return 1;
}

void GlobalMapWnd::ClearConnections()
{
    // RVA 0x4DB680
    m_connections.clear();
    UpdateConnectionsDrawInfo();
}

bool GlobalMapWnd::ConnectionExists(CStr const& levelName0, CStr const& levelName1) const
{
    // RVA 0x4DBD30 - connections are undirected, so either ordering counts.
    if (levelName0.empty() || levelName1.empty())
    {
        return false;
    }
    for (unsigned i = 0; i < m_connections.size(); ++i)
    {
        auto const& c = m_connections[i];
        if ((c.m_levelNameFrom == levelName0 && c.m_levelNameTo == levelName1) ||
            (c.m_levelNameTo == levelName0 && c.m_levelNameFrom == levelName1))
        {
            return true;
        }
    }
    return false;
}

void GlobalMapWnd::UpdateConnections()
{
    // RVA 0x4DB6E0 - a connection exists wherever some level holds an active
    // location whose passage address points at another level.
    ClearConnections();

    LevelInfoManager* lim = M3D_APP->m_pInterfaceManager->GetLevelInfoManager();
    retruxx::vector<CStr> allLevels;
    lim->GetAllLevelNames(allLevels);

    for (unsigned i = 0; i < allLevels.size(); ++i)
    {
        CStr const& levelName = allLevels[i];
        auto const levelIt = lim->m_levelObjects.find(levelName);
        if (levelIt == lim->m_levelObjects.end())
        {
            continue;
        }

        for (auto objIt = levelIt->second.begin(); objIt != levelIt->second.end(); ++objIt)
        {
            ObjectInfo const* oi = objIt->second;
            if (!oi || !oi->IsLocation() || !oi->IsActive())
            {
                continue;
            }

            CStr const targetLevelName = ai::Location::GetLevelNameFromPassageAddress(*oi->GetPassageAddress());
            if (targetLevelName.empty() || ConnectionExists(levelName, targetLevelName))
            {
                continue;
            }

            Connection newConnection;
            newConnection.m_levelNameFrom = levelName;
            newConnection.m_levelNameTo = targetLevelName;
            m_connections.push_back(newConnection);
        }
    }
    UpdateConnectionsDrawInfo();
}

void GlobalMapWnd::UpdateConnectionsDrawInfo()
{
    // RVA 0x4DBA20 - turn each connection into a pair of points in the
    // connections window's own space, centred on the two level icons.
    if (!m_wndConnections)
    {
        return;
    }

    std::vector<LevelConnectionsWnd::ConnectionDrawInfo> connectionDrawInfo;
    for (unsigned i = 0; i < m_connections.size(); ++i)
    {
        auto const& c = m_connections[i];
        auto const it0 = m_levelIcons.find(c.m_levelNameFrom);
        auto const it1 = m_levelIcons.find(c.m_levelNameTo);
        if (it0 == m_levelIcons.end() || it1 == m_levelIcons.end())
        {
            continue;
        }

        ref_ptr<LevelIco> icoFrom = it0->second;
        ref_ptr<LevelIco> icoTo = it1->second;
        if (!icoFrom || !icoTo)
        {
            continue;
        }

        auto const bFrom = icoFrom->GetBounds();
        auto const bTo = icoTo->GetBounds();

        PointBase<float> pt0;
        pt0.x = ((bFrom.x0 * 2.0f) + bFrom.width) * 0.5f;
        pt0.y = ((bFrom.y0 * 2.0f) + bFrom.height) * 0.5f;
        PointBase<float> pt1;
        pt1.x = ((bTo.x0 * 2.0f) + bTo.width) * 0.5f;
        pt1.y = ((bTo.y0 * 2.0f) + bTo.height) * 0.5f;

        pt0 = m_wndConnections->ToWindow(ToScreen(pt0));
        pt1 = m_wndConnections->ToWindow(ToScreen(pt1));
        connectionDrawInfo.push_back(LevelConnectionsWnd::ConnectionDrawInfo(pt0, pt1));
    }
    m_wndConnections->SetDrawInfo(connectionDrawInfo);
}

void GlobalMapWnd::LaunchLocalMap()
{
    // RVA 0x4DBEB0 - open the local map for the level the player is on.
    M3D_APP->EnqueueMessage(65676, 0, 0, 0, 0, CurrentLevelName(), {});
}

int GlobalMapWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned idFrom, unsigned message, m3d::AIParam const& data)
{
    // RVA 0x4DBE40
    if ((m_style & 0x100000) != 0)
    {
        ReflectChildNotifyToParent(from, idFrom, message, data);
    }
    if (idFrom == 1)
    {
        if (message == 1)
        {
            OnExit();
            return 1;
        }
    }
    else if (idFrom == 300001 && message == 1)
    {
        LaunchLocalMap();
        return 1;
    }
    return 0;
}

int GlobalMapWnd::OnAfterAddToWndStation()
{
    // RVA 0x4DC080
    int const res = m3d::ui::Wnd::OnAfterAddToWndStation();
    M3D_APP->EnqueueMessage(65691, 0, 0, 0, 0, {}, {});
    return res;
}

// ---------------------------------------------------------------------------
//  LevelIco
// ---------------------------------------------------------------------------

RT_CLASS_EXPORTS_BEGIN(LevelIco)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(LevelIco);

LevelIco::AuxInfo LevelIco::m_aif;

LevelIco::AuxInfo::AuxInfo()
{
    // RVA 0x4DC170
    m_selFrameTexName = "MapSelFrame";
}

LevelIco::AuxInfo::AuxInfo(AuxInfo const& rhs) : m_selFrameTexName(rhs.m_selFrameTexName)
{
}

LevelIco::LevelIco() : m_state(STATE_INVISIBLE)
{
}

LevelIco::LevelIco(LevelIco const&) : LevelIco()
{
}

LevelIco::~LevelIco()
{
    M3D_RENDERER->ReleaseTexture(m_selFrameTex);
}

m3d::Object* LevelIco::Clone()
{
    return new LevelIco(*this);
}

m3d::Object* LevelIco::CreateObject()
{
    return new LevelIco;
}

m3d::Class* LevelIco::GetBaseClass()
{
    return RT_CLASS_LOCAL(ImageWnd);
}

m3d::Class* LevelIco::GetClass() const
{
    return RT_CLASS_LOCAL(LevelIco);
}

CStr const& LevelIco::GetLevelName() const
{
    return m_levelName;
}

bool LevelIco::IsValid() const
{
    // RVA 0x4DC2E0
    return (m_gameDataFlags & 1) != 0 && !m_levelName.empty();
}

int LevelIco::CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc)
{
    // RVA 0x4DC320
    if (!patternWnd || !patternWnd->IsKindOf(&m3d::ui::ImageWnd::m_classImageWnd))
    {
        M3D_LOG_INFO("LevelIco::CreateFromPattern error - invalid patternWnd");
        return 0;
    }

    unsigned const style = patternWnd->GetStyle();
    BoundsBase<float> const rc = patternWnd->GetBounds();
    int const id = patternWnd->GetId();

    m3d::Object* parent = patternWnd->GetParent();
    if (!parent)
    {
        M3D_LOG_INFO("LevelIco::CreateFromPattern error - cannot fnd parent for pattern wnd");
        return 0;
    }

    if (!CreateImageWnd(rc, m3d::rend::TexHandle{}))
    {
        M3D_LOG_INFO("LevelIco::CreateFromPattern error - cannot create");
        return 0;
    }

    m_style = style;
    m_id = id;
    SetPane(patternWnd->GetPaneName());
    SetPaneFlags(patternWnd->GetPaneFlags());
    SetColor(patternWnd->GetColor());
    parent->AddChild(this);
    parent->MoveChildToFirstPosition(this);
    if (deleteSrc)
    {
        parent->RemoveChild(patternWnd);
        patternWnd->DecRef();
    }

    m_selFrameTex = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_selFrameTexName, 0);
    M3D_RENDERER->ReferenceTexture(m_selFrameTex);

    m_gameDataFlags |= 1u;
    if (IsValid())
    {
        // Nothing is known about the level yet, so start hidden.
        m_state = STATE_INVISIBLE;
        ShowWindow(false);
    }
    return 1;
}

int LevelIco::SetUpForLevel(CStr const& levelName)
{
    // RVA 0x4DC560
    if ((m_gameDataFlags & 1) == 0 || levelName.empty())
    {
        return 0;
    }
    LevelInfo* li = M3D_APP->m_pInterfaceManager->GetLevelInfoManager()->GetLevelInfoByName(levelName);
    if (!li)
    {
        return 0;
    }
    m_levelName = levelName;
    SetImage(li->GetImage1());
    SetProperty(PROP_WND_TOOLTIP, const_cast<char*>(li->GetFullName().c_str()));
    return 1;
}

void LevelIco::UpdateState()
{
    // RVA 0x4DC5E0 - the icon shows the level the player is on, then levels they
    // have been to, then levels they have merely heard of; anything else is
    // hidden entirely.
    if ((m_gameDataFlags & 1) == 0 || m_levelName.empty())
    {
        return;
    }

    if (m_levelName == CurrentLevelName())
    {
        SetState(STATE_CURRENT);
        return;
    }

    State newState = STATE_INVISIBLE;
    LevelInfoManager* lim = M3D_APP->m_pInterfaceManager->GetLevelInfoManager();

    retruxx::vector<CStr> visitedLevels;
    lim->GetVisitedLevelNames(visitedLevels);
    if (std::find(visitedLevels.begin(), visitedLevels.end(), m_levelName) != visitedLevels.end())
    {
        newState = STATE_VISITED;
    }
    else
    {
        retruxx::vector<CStr> knownLevels;
        lim->GetKnownLevelNames(knownLevels);
        if (std::find(knownLevels.begin(), knownLevels.end(), m_levelName) != knownLevels.end())
        {
            newState = STATE_KNOWN;
        }
    }
    SetState(newState);
}

void LevelIco::SetState(LevelIco::State state)
{
    // RVA 0x4DC780
    if ((m_gameDataFlags & 1) == 0 || m_levelName.empty())
    {
        return;
    }
    m_state = state;
    switch (state)
    {
    case STATE_CURRENT:
        ShowWindow(true);
        SetColor(0);
        break;
    case STATE_VISITED:
    case STATE_KNOWN:
        ShowWindow(true);
        SetColor(3);
        break;
    case STATE_INVISIBLE:
        ShowWindow(false);
        break;
    default:
        break;
    }
}

int LevelIco::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x4DC820
    if ((m_gameDataFlags & 1) == 0 || m_levelName.empty())
    {
        return 0;
    }
    if (dataType == 37)
    {
        // A location changed state: only react when it is on our own level.
        if (data && static_cast<m3d::Event const*>(data)->m_strEv == m_levelName)
        {
            UpdateState();
        }
    }
    else if (dataType == 85)
    {
        UpdateState();
    }
    return 1;
}

void LevelIco::OnStartLevel()
{
    // RVA 0x4DC890
    UpdateState();
}

int LevelIco::OnPaint(m3d::ui::DrawInfo const& di)
{
    // RVA 0x4DC8A0
    ImageWnd::OnPaint(di);
    if (m_state <= STATE_VISITED)
    {
        BoundsBase<float> rect;
        rect.x0 = 0.0f;
        rect.y0 = 0.0f;
        rect.width = m_bounds.width;
        rect.height = m_bounds.height;
        GetGfxServer()->AddImagedRect(di, rect, 0xFFFFFFFF, m_selFrameTex);
    }
    return 1;
}

int LevelIco::OnMouseButton0(unsigned state, PointBase<float> const& at)
{
    // RVA 0x4DC920
    if (state)
    {
        if (m_state == STATE_CURRENT || m_state == STATE_VISITED)
        {
            // Somewhere the player has been, so its local map can be opened.
            M3D_APP->EnqueueMessage(65676, 0, 0, 0, 0, m_levelName, {});
        }
        else if (m_state == STATE_KNOWN)
        {
            M3D_APP->RunMsgBoxDlg({}, M3D_APP->GetStringByStringId0("MapUnavailable"), 1, 0);
        }
    }
    return m3d::ui::Wnd::OnMouseButton0(state, at);
}
