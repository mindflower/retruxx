#include "mapmarkwnd.h"

#include <cmath>

#include <config.h>
#include <core/kernel.h>

#include <game/m3dgame.h>
#include "game/uimanager/truxxuimanager.h"
#include "game/uimisc/levelinfo.h"
#include "game/uimisc/questinfo.h"

#include "server/quest.h"
#include "server/relationship.h"
#include "server/server.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/dynamicquest.h"
#include "server/objects/player.h"
#include "server/objects/vehicle.h"

#include <ui/frame.h>
#include <ui/ui_srv.h>

// ============================================================================
//  MapMarkWnd
// ============================================================================

RT_CLASS_EXPORTS_BEGIN(MapMarkWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MapMarkWnd);

MapMarkWnd::MapMarkWnd()
{
    m_objectInfo = nullptr;
}

MapMarkWnd::MapMarkWnd(MapMarkWnd const& rhs) : Wnd(rhs)
{
    m_objectInfo = nullptr;
}

MapMarkWnd::~MapMarkWnd()
{
}

m3d::Object* MapMarkWnd::Clone()
{
    // RVA 0xEFD50
    return new MapMarkWnd(*this);
}

m3d::Object* MapMarkWnd::CreateObject()
{
    // RVA 0xEFEA0
    return new MapMarkWnd;
}

m3d::Class* MapMarkWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* MapMarkWnd::GetClass() const
{
    // RVA 0xEFE90
    return RT_CLASS_LOCAL(MapMarkWnd);
}

ObjectInfo* MapMarkWnd::GetObjectInfo() const
{
    // RVA 0xEFF70
    return m_objectInfo;
}

void MapMarkWnd::FullUpdate()
{
    // RVA 0xEFF80 - the base does nothing.
}

int MapMarkWnd::SetUpForObjectInfo(ObjectInfo* objectInfo)
{
    // RVA 0xEFF50
    m_objectInfo = objectInfo;
    if (!objectInfo)
    {
        return 0;
    }
    FullUpdate();
    return 1;
}

// ============================================================================
//  MapMarkIcoWnd
// ============================================================================

RT_CLASS_EXPORTS_BEGIN(MapMarkIcoWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MapMarkIcoWnd);

MapMarkIcoWnd::AuxInfo::AuxInfo()
{
    // RVA 0xF0030
    m_size = PointBase<float>{32.0f, 32.0f};
    m_sizeSelected = PointBase<float>{64.0f, 32.0f};
    m_colorEnemy = 0xFF880000u;
    m_colorNeutral = 0xFFCCCCCCu;
    m_colorFriend = 0xFF008040u;
    m_texIdSelection = "MapMarkSelection";
}

MapMarkIcoWnd::MapMarkIcoWnd()
{
    m_bSelected = false;
    m_texSelection.SetInvalid();
}

MapMarkIcoWnd::MapMarkIcoWnd(MapMarkIcoWnd const& rhs) : MapMarkWnd(rhs)
{
    m_bSelected = false;
    m_texSelection.SetInvalid();
}

MapMarkIcoWnd::~MapMarkIcoWnd()
{
    // NOTE: the shipped destructor is inlined to nothing, so the SetUp
    // ReferenceTexture leaks; retruxx releases the selection texture here.
    if (m_texSelection.IsValid())
    {
        M3D_RENDERER->ReleaseTexture(m_texSelection);
    }
}

m3d::Object* MapMarkIcoWnd::Clone()
{
    // RVA 0xEFD80
    return new MapMarkIcoWnd(*this);
}

m3d::Object* MapMarkIcoWnd::CreateObject()
{
    // RVA 0xEFFB0
    return new MapMarkIcoWnd;
}

m3d::Class* MapMarkIcoWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(MapMarkWnd);
}

m3d::Class* MapMarkIcoWnd::GetClass() const
{
    // RVA 0xEFFA0
    return RT_CLASS_LOCAL(MapMarkIcoWnd);
}

bool MapMarkIcoWnd::IsSelected() const
{
    // RVA 0xF0490
    return m_bSelected;
}

void MapMarkIcoWnd::Select(bool bSelect)
{
    // RVA 0xF0480
    m_bSelected = bSelect;
}

int MapMarkIcoWnd::SetUp(ObjectInfo* objectInfo, PointBase<float> const& objWndOrigin)
{
    // RVA 0xF01B0
    if (Valid())
    {
        return 0;
    }
    float const halfW = m_aif.m_sizeSelected.x * 0.5f;
    float const halfH = m_aif.m_sizeSelected.y * 0.5f;
    BoundsBase<float> const b{
        objWndOrigin.x - halfW, objWndOrigin.y - halfH, m_aif.m_sizeSelected.x, m_aif.m_sizeSelected.y};
    if (!m3d::ui::Wnd::Create({}, 0x220u, b, 0x193u))
    {
        return 0;
    }

    m_texSelection = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_texIdSelection, 0);
    M3D_RENDERER->ReferenceTexture(m_texSelection);

    m_objectInfo = objectInfo;
    if (!objectInfo)
    {
        return 0;
    }
    FullUpdate();
    return 1;
}

void MapMarkIcoWnd::FullUpdate()
{
    // RVA 0xF02F0
    m3d::rend::TexHandle tex;
    if (m_objectInfo)
    {
        tex = M3D_APP->m_pInterfaceManager->GetIcoByName(m_objectInfo->GetPrototypeName(), 1);
    }
    SetBackground(tex);
    if (m_objectInfo && m_objectInfo->IsTown())
    {
        UpdateToleranceColor();
    }
}

void MapMarkIcoWnd::UpdateIcon()
{
    // RVA 0xF0350
    m3d::rend::TexHandle tex;
    if (m_objectInfo)
    {
        tex = M3D_APP->m_pInterfaceManager->GetIcoByName(m_objectInfo->GetPrototypeName(), 1);
    }
    SetBackground(tex);
}

void MapMarkIcoWnd::UpdateTooltip()
{
    // RVA 0xF0390
    CStr tooltip;
    if (m_objectInfo)
    {
        tooltip = m_objectInfo->GetFullName();
    }
    SetProperty(PROP_WND_TOOLTIP, &tooltip);
}

void MapMarkIcoWnd::UpdateToleranceColor()
{
    // RVA 0xF03F0
    m_curClr = 0xFFFFFFFFu;
    if (!m_objectInfo || !ai::thePlayer)
    {
        return;
    }
    ai::Vehicle* vehicle = ai::thePlayer->GetVehicle();
    if (!vehicle)
    {
        return;
    }
    switch (ai::pServer->CheckTolerance(m_objectInfo->GetBelong(), vehicle->GetBelong()))
    {
    case ai::RS_ENEMY:
        m_curClr = m_aif.m_colorEnemy;
        break;
    case ai::RS_NEUTRAL:
        m_curClr = m_aif.m_colorNeutral;
        break;
    case ai::RS_ALLY:
    case ai::RS_OWN:
        m_curClr = m_aif.m_colorFriend;
        break;
    default:
        break;
    }
}

int MapMarkIcoWnd::OnPaint(m3d::ui::DrawInfo const& di)
{
    // RVA 0xF04A0
    float const dx = (m_aif.m_sizeSelected.x - m_aif.m_size.x) * 0.5f;
    float const dy = (m_aif.m_sizeSelected.y - m_aif.m_size.y) * 0.5f;
    BoundsBase<float> const b = GetBounds();

    BoundsBase<float> const icoB{dx, dy, b.width - dx * 2.0f, b.height - dy * 2.0f};
    GetGfxServer()->AddImagedRect(di, icoB, m_curClr, m_bgTexture);

    if (m_bSelected)
    {
        BoundsBase<float> const selRect{0.0f, 0.0f, b.width, b.height};
        GetGfxServer()->AddImagedRect(di, selRect, 0xFFFFFFFFu, m_texSelection);
    }
    return 1;
}

// ============================================================================
//  PlayerMarkWnd
// ============================================================================

RT_CLASS_EXPORTS_BEGIN(PlayerMarkWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(PlayerMarkWnd);

PlayerMarkWnd::AuxInfo::AuxInfo()
{
    // RVA 0xF09E0
    m_defaultSize = PointBase<float>{16.0f, 16.0f};
    m_texName = "PlayerMark";
}

PlayerMarkWnd::PlayerMarkWnd()
{
    m_wndCoords = PointBase<float>{0.0f, 0.0f};
    m_angle = 0.0f;
    m_size = m_aif.m_defaultSize;
}

PlayerMarkWnd::PlayerMarkWnd(PlayerMarkWnd const& rhs) : ImageWnd(rhs)
{
    m_wndCoords = PointBase<float>{0.0f, 0.0f};
    m_angle = 0.0f;
    m_size = m_aif.m_defaultSize;
}

PlayerMarkWnd::~PlayerMarkWnd()
{
}

m3d::Object* PlayerMarkWnd::Clone()
{
    // RVA 0xEFE10
    return new PlayerMarkWnd(*this);
}

m3d::Object* PlayerMarkWnd::CreateObject()
{
    // RVA 0xF0970
    return new PlayerMarkWnd;
}

m3d::Class* PlayerMarkWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ImageWnd);
}

m3d::Class* PlayerMarkWnd::GetClass() const
{
    // RVA 0xF0960
    return RT_CLASS_LOCAL(PlayerMarkWnd);
}

int PlayerMarkWnd::CreatePlayerMarkWnd()
{
    // RVA 0xF0B80
    BoundsBase<float> const rc{0.0f, 0.0f, 0.0f, 0.0f};
    m3d::rend::TexHandle const tex = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_texName, 0);
    if (!CreateImageWnd(rc, tex))
    {
        return 0;
    }
    m_style |= 0x100u;

    float const dim = 2.0f * ((m_size.x > m_size.y) ? m_size.x : m_size.y);
    m_bounds.width = dim;
    m_bounds.height = dim;
    m_bounds.x0 = m_wndCoords.x - dim * 0.5f;
    m_bounds.y0 = m_wndCoords.y - m_bounds.height * 0.5f;
    return 1;
}

void PlayerMarkWnd::SetImageCoords(PointBase<float> const& wndCoords)
{
    // RVA 0xF0CC0
    m_wndCoords = wndCoords;
    m_bounds.x0 = m_wndCoords.x - m_bounds.width * 0.5f;
    m_bounds.y0 = m_wndCoords.y - m_bounds.height * 0.5f;
}

void PlayerMarkWnd::SetImageAngle(float angle)
{
    // RVA 0xF0D20
    m_angle = angle;
}

void PlayerMarkWnd::SetImageSize(PointBase<float> const& size)
{
    // RVA 0xF0D30
    m_size = size;
    float const dim = 2.0f * ((size.x > size.y) ? size.x : size.y);
    m_bounds.width = dim;
    m_bounds.height = dim;
    m_bounds.x0 = m_wndCoords.x - dim * 0.5f;
    m_bounds.y0 = m_wndCoords.y - m_bounds.height * 0.5f;
}

int PlayerMarkWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    // RVA 0xF0DB0
    namespace rend = m3d::rend;

    float const halfW = m_size.x * 0.5f;
    float const halfH = m_size.y * 0.5f;

    if (!m_texture.IsValid())
    {
        M3D_RENDERER->SetWhiteTexture(0);
    }
    else
    {
        M3D_RENDERER->SetTexture(0, m_texture, -1.0);
    }

    M3D_RENDERER->SetAlphaTest(M3D_ENGINE_CFG.m_alphaTestInterface.GetI());
    M3D_RENDERER->SetStageState(0, rend::BM_COLOR, rend::TS_TEXTURE);
    M3D_RENDERER->SetStageState(0, rend::BM_ALPHA, rend::TS_TEXTURE);
    M3D_RENDERER->SetStageState(1, rend::BM_COLOR, rend::TS_NONE);
    M3D_RENDERER->SetStageState(1, rend::BM_COLOR, rend::TS_NONE);
    M3D_RENDERER->PushBlend(rend::BM_ALPHA);
    M3D_RENDERER->PushZbState(rend::ZB_DISABLE);

    auto* parent = RT_DYNCAST(GetParent(), m3d::ui::Wnd);
    if (!parent)
    {
        return 0;
    }
    PointBase<float> const screenPt = parent->ToScreen(m_wndCoords);
    // NOTE: the shipped build passes a garbage float bit-pattern for the sy
    // argument; retruxx passes the real half-height.
    M3D_APP->PutSpriteRelRot(screenPt.x, screenPt.y, halfW, halfH, 0xFFFFFFFFu, m_angle, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

    M3D_RENDERER->SetAlphaTest(0);
    M3D_RENDERER->PopBlend();
    M3D_RENDERER->PopZbState();
    return 1;
}

// ============================================================================
//  NavPointMarkWnd
// ============================================================================

RT_CLASS_EXPORTS_BEGIN(NavPointMarkWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(NavPointMarkWnd);

NavPointMarkWnd::AuxInfo::AuxInfo()
{
    // RVA 0xF1060
    m_locationTooltipStrName = "NavPoint";
    m_texNames[0] = "MapMarkMainQuest";
    m_texNames[1] = "MapMarkUserQuest";
    m_texNames[2] = "MapMarkUserLocation";
    m_size = PointBase<float>{16.0f, 16.0f};
}

NavPointMarkWnd::NavPointMarkWnd()
{
    m_navPointId = -1;
}

NavPointMarkWnd::NavPointMarkWnd(NavPointMarkWnd const& rhs) : ImageWnd(rhs)
{
    m_navPointId = -1;
}

NavPointMarkWnd::~NavPointMarkWnd()
{
}

m3d::Object* NavPointMarkWnd::Clone()
{
    // RVA 0xEFE50
    return new NavPointMarkWnd(*this);
}

m3d::Object* NavPointMarkWnd::CreateObject()
{
    // RVA 0xF0FC0
    return new NavPointMarkWnd;
}

m3d::Class* NavPointMarkWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ImageWnd);
}

m3d::Class* NavPointMarkWnd::GetClass() const
{
    // RVA 0xF0FB0
    return RT_CLASS_LOCAL(NavPointMarkWnd);
}

int NavPointMarkWnd::GetNavPointId() const
{
    // RVA 0xF1370
    return m_navPointId;
}

m3d::rend::TexHandle NavPointMarkWnd::GetNpIcoByType(NavPoint::NavPointType npType) const
{
    // RVA 0xF1380
    if (npType == NavPoint::NAVPOINT_TYPE_NUM_NAVPOINT_TYPES)
    {
        return m3d::rend::TexHandle();
    }
    return M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_texNames[npType], 0);
}

CStr NavPointMarkWnd::GetTooltipForNp(int npId) const
{
    // RVA 0xF13B0
    NavPoint const* np = M3D_APP->m_pInterfaceManager->GetNavPointManager()->GetNavPointById(npId);
    if (!np)
    {
        return {};
    }

    if (np->GetObjectType() == NavPoint::OBJECT_TYPE_STATIC_QUEST)
    {
        QuestInfoManager* qim = M3D_APP->m_pInterfaceManager->GetQuestInfoManager();
        if (ai::Quest* quest = ai::theQuestManager->GetQuestById(np->GetObjectId()))
        {
            if (QuestInfo const* qi = qim->GetQuestInfoByQuestInfoName(quest->GetName()))
            {
                return qi->GetBriefDiz();
            }
        }
    }
    else if (np->GetObjectType() == NavPoint::OBJECT_TYPE_DYNAMIC_QUEST)
    {
        ai::Obj* obj = ai::theObjects->GetEntityByObjId(np->GetObjectId());
        if (obj && obj->IsKindOf(&ai::DynamicQuest::m_classDynamicQuest))
        {
            QuestInfoManager* qim = M3D_APP->m_pInterfaceManager->GetQuestInfoManager();
            QuestInfo const* qi = qim->GetQuestInfoForDynamicQuest(np->GetObjectId());
            return qi ? qi->GetBriefDiz() : CStr{};
        }
    }
    else if (np->GetNavPointType() == NavPoint::NAVPOINT_TYPE_USER_LOCATION)
    {
        return M3D_APP->GetStringByStringId0(m_aif.m_locationTooltipStrName);
    }
    return {};
}

int NavPointMarkWnd::CreateNavPointMarkWnd(PointBase<float> const& wndCoords, int navPointId)
{
    // RVA 0xF11F0
    if (navPointId == -1)
    {
        return 0;
    }
    NavPoint const* np = M3D_APP->m_pInterfaceManager->GetNavPointManager()->GetNavPointById(navPointId);
    if (!np)
    {
        return 0;
    }
    m3d::rend::TexHandle const tex = GetNpIcoByType(np->GetNavPointType());

    // NOTE: the shipped build uses the x half-size for both axes (m_size is square).
    float const halfSizeX = m_aif.m_size.x * 0.5f;
    float const dim = (m_aif.m_size.x + m_aif.m_size.y) * 0.5f;
    BoundsBase<float> const rc{wndCoords.x - halfSizeX, wndCoords.y - halfSizeX, dim, dim};
    if (!CreateImageWnd(rc, tex))
    {
        return 0;
    }

    m_style = (m_style & ~0x100u) | 0x20u;
    m_id = 408;

    CStr tooltip = GetTooltipForNp(navPointId);
    SetProperty(PROP_WND_TOOLTIP, &tooltip);
    m_navPointId = navPointId;
    return 1;
}

// ============================================================================
//  MapMarkNameWnd
// ============================================================================

RT_CLASS_EXPORTS_BEGIN(MapMarkNameWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MapMarkNameWnd);

MapMarkNameWnd::NameAuxInfo::NameAuxInfo()
{
    // RVA 0xF0650
    m_paneName = "PaneSmallTitle";
    m_space = PointBase<float>{2.0f, 0.0f};
    m_textColor = 0xFF404040u;
}

MapMarkNameWnd::MapMarkNameWnd()
{
}

MapMarkNameWnd::MapMarkNameWnd(MapMarkNameWnd const& rhs) : MapMarkWnd(rhs)
{
}

MapMarkNameWnd::~MapMarkNameWnd()
{
}

m3d::Object* MapMarkNameWnd::Clone()
{
    // RVA 0xEFDD0
    return new MapMarkNameWnd(*this);
}

m3d::Object* MapMarkNameWnd::CreateObject()
{
    // RVA 0xF0610
    return new MapMarkNameWnd;
}

m3d::Class* MapMarkNameWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(MapMarkWnd);
}

m3d::Class* MapMarkNameWnd::GetClass() const
{
    // RVA 0xF0600
    return RT_CLASS_LOCAL(MapMarkNameWnd);
}

int MapMarkNameWnd::SetUp(ObjectInfo* objectInfo, BoundsBase<float> const& mapMarkIcoB, BoundsBase<float> const& clipB)
{
    // RVA 0xF0740
    if (Valid())
    {
        return 0;
    }
    m_objectInfo = objectInfo;
    if (!objectInfo)
    {
        return 0;
    }
    FullUpdate();

    CStr const& fullName = objectInfo->GetFullName();
    PointBase<float> const nameSz = GetGfxServer()->MeasureText(fullName, 1, m3d::TW_NOWRAP, 1024.0f);

    float barWidth = 0.0f;
    if (m3d::ui::Pane* pane = GetGfxServer()->GetPane(m_nAif.m_paneName))
    {
        if (m3d::ui::Frame* frame = pane->m_frame[0])
        {
            barWidth = static_cast<float>(frame->m_barUsedWidth);
        }
    }

    // Centre the name horizontally over the icon and place it above, then clamp
    // the whole rect into the clip region.
    float x = ((mapMarkIcoB.x0 * 2.0f + mapMarkIcoB.width) * 0.5f) - nameSz.x * 0.5f;
    float y = (mapMarkIcoB.y0 - m_nAif.m_space.y * 2.0f - nameSz.y) - barWidth;

    float const minX = clipB.x0 + m_nAif.m_space.x + barWidth;
    float const maxX = (clipB.x0 + clipB.width) - nameSz.x - m_nAif.m_space.x - barWidth;
    float const minY = clipB.y0 + m_nAif.m_space.y + barWidth;
    float const maxY = (clipB.y0 + clipB.height) - nameSz.y - m_nAif.m_space.y - barWidth;
    if (minX > x)
    {
        x = minX;
    }
    if (x > maxX)
    {
        x = maxX;
    }
    if (minY > y)
    {
        y = minY;
    }
    if (y > maxY)
    {
        y = maxY;
    }

    BoundsBase<float> b;
    b.x0 = x - m_nAif.m_space.x - barWidth;
    b.y0 = y - m_nAif.m_space.y - barWidth;
    b.width = (m_nAif.m_space.x + barWidth) * 2.0f + nameSz.x;
    b.height = (m_nAif.m_space.y + barWidth) * 2.0f + nameSz.y;
    if (!Create(fullName, 544u, b, 0u))
    {
        return 0;
    }

    SetPane(m_nAif.m_paneName);
    SetDefaultFont(1);
    SetWrapMode(m3d::TW_NOWRAP);
    SetTextColor(m_nAif.m_textColor);
    SetClientEdges(m_nAif.m_space.x, m_nAif.m_space.y, m_nAif.m_space.x, m_nAif.m_space.y);
    return 1;
}
