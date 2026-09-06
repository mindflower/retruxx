#include "radarwnd.h"

#include <cmath>

#include "config.h"
#include "electronicdigitalwnd.h"
#include "core/kernel.h"
#include "core/log.h"
#include "core/console/console.h"
#include "core/ini.h"
#include <game/m3dgame.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimisc/guihelper.h>
#include <game/uimisc/levelinfo.h>
#include <game/uimisc/objectcollection.h>
#include <math/vector.h>
#include "server/server.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/player.h"
#include "server/objects/vehicle.h"
#include "server/objects/staticautogun.h"

RT_CLASS_EXPORT_METHOD_DEFINE(RadarWnd, AllowNavPoints)
{
    auto radar = (RadarWnd*)context->asObject(0, "RadarWnd");
    bool allow = context->asBool(1);
    radar->AllowNavPoints(allow);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(RadarWnd, AllowVehicles)
{
    auto radar = (RadarWnd*)context->asObject(0, "RadarWnd");
    bool allow = context->asBool(1);
    radar->AllowVehicles(allow);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(RadarWnd, AllowTurrets)
{
    auto radar = (RadarWnd*)context->asObject(0, "RadarWnd");
    bool allow = context->asBool(1);
    radar->AllowTurrets(allow);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(RadarWnd, AllowDistances)
{
    auto radar = (RadarWnd*)context->asObject(0, "RadarWnd");
    bool allow = context->asBool(1);
    radar->AllowDistances(allow);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(RadarWnd, SetScanRadius)
{
    auto radar = (RadarWnd*)context->asObject(0, "RadarWnd");
    bool radius = context->asFloat(1);
    radar->SetScanRadius(radius);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(RadarWnd, SetTest)
{
    auto radar = (RadarWnd*)context->asObject(0, "RadarWnd");
    radar->SetTest(context->asInt(1));
    return 1;
}

RT_CLASS_EXPORTS_BEGIN(RadarWnd)
RT_CLASS_EXPORT(RadarWnd, m3d::METHOD, AllowNavPoints, "", "", "")
RT_CLASS_EXPORT(RadarWnd, m3d::METHOD, AllowVehicles, "", "", "")
RT_CLASS_EXPORT(RadarWnd, m3d::METHOD, AllowTurrets, "", "", "")
RT_CLASS_EXPORT(RadarWnd, m3d::METHOD, AllowDistances, "", "", "")
RT_CLASS_EXPORT(RadarWnd, m3d::METHOD, SetScanRadius, "", "", "")
RT_CLASS_EXPORT(RadarWnd, m3d::METHOD, SetTest, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RadarWnd);

// ===========================================================================
//  RadarWnd::NpDistance
// ===========================================================================

RadarWnd::NpDistance::~NpDistance()
{
}

RadarWnd::NpDistance::NpDistance(ElectronicDigitalWnd* wndDigital, NavPoint::NavPointType npType)
{
    m_wndDigital = wndDigital;
    m_navPointType = npType;
    m_navPointId = -1;
    Show(false);
}

int RadarWnd::NpDistance::GetNavPointId() const
{
    return m_navPointId;
}

NavPoint::NavPointType RadarWnd::NpDistance::GetNavPointType() const
{
    return m_navPointType;
}

bool RadarWnd::NpDistance::IsValid() const
{
    return m_wndDigital != nullptr && m_navPointType != NavPoint::NAVPOINT_TYPE_NUM_NAVPOINT_TYPES;
}

int RadarWnd::NpDistance::SetNavPointId(int npId)
{
    if (!m_wndDigital || m_navPointType == NavPoint::NAVPOINT_TYPE_NUM_NAVPOINT_TYPES)
    {
        return 0;
    }

    m_navPointId = npId;
    if (npId == -1)
    {
        m_wndDigital->Clear();
        Show(false);
    }
    else
    {
        Show(true);
    }
    return 1;
}

int RadarWnd::NpDistance::SetDistance(float distance)
{
    if (!m_wndDigital || m_navPointType == NavPoint::NAVPOINT_TYPE_NUM_NAVPOINT_TYPES)
    {
        return 0;
    }

    if (distance == -1.0f)
    {
        m_wndDigital->Clear();
        return 1;
    }

    if (static_cast<int>(distance) < 1000)
    {
        m_wndDigital->ShowNumber(static_cast<int>(distance), false, 3, false);
        m_wndDigital->ShowSymbol(ElectronicDigitalWnd::SYMBOL_METER, true, 1, false);
    }
    else
    {
        m_wndDigital->ShowNumber(static_cast<int>(distance * 0.001f), false, 3, false);
        m_wndDigital->ShowSymbol(ElectronicDigitalWnd::SYMBOL_KILOMETER, true, 1, false);
    }
    return 1;
}

void RadarWnd::NpDistance::Show(bool bShow)
{
    if (!m_wndDigital || m_navPointType == NavPoint::NAVPOINT_TYPE_NUM_NAVPOINT_TYPES)
    {
        return;
    }

    m3d::ui::Wnd::AnimationInfo animationInfo(
        bShow ? m_wndDigital->GetOnShowAnimation() : m_wndDigital->GetOnHideAnimation());

    if (m_wndDigital->IsChildOf(m3d::Application::g_pApp))
    {
        animationInfo.m_bEnabled = true;
        animationInfo.m_purpose = m3d::ui::Wnd::AnimationInfo::PURPOSE_UNKNOWN;
        m_wndDigital->StartAnimation(animationInfo, true);
    }
    else
    {
        m_wndDigital->SetBaseOrigin(animationInfo.m_endPt);
    }
}

// ===========================================================================
//  RadarWnd::RadarItem
// ===========================================================================

RadarWnd::RadarItem::RadarItem(
    m3d::rend::TexHandle tex,
    PointBase<float> const& size,
    PointBase<float> const& coords,
    float angle)
{
    m_texture = tex;
    if (m_texture.IsValid())
    {
        M3D_RENDERER->ReferenceTexture(m_texture);
    }
    m_coords = coords;
    m_angle = angle;
    m_size = size;
}

RadarWnd::RadarItem::~RadarItem()
{
    if (m_texture.IsValid())
    {
        M3D_RENDERER->ReleaseTexture(m_texture);
    }
}

PointBase<float> const& RadarWnd::RadarItem::GetCoords() const
{
    return m_coords;
}

PointBase<float> const& RadarWnd::RadarItem::GetSize() const
{
    return m_size;
}

float RadarWnd::RadarItem::GetAngle() const
{
    return m_angle;
}

m3d::rend::TexHandle RadarWnd::RadarItem::GetTexture() const
{
    return m_texture;
}

BoundsBase<float> RadarWnd::RadarItem::GetBounds() const
{
    float const halfW = m_size.x * 0.5f;
    float const halfH = m_size.y * 0.5f;
    BoundsBase<float> bounds;
    bounds.x0 = m_coords.x - halfW;
    bounds.y0 = m_coords.y - halfH;
    bounds.width = (m_coords.x + halfW) - bounds.x0;
    bounds.height = (m_coords.y + halfH) - bounds.y0;
    return bounds;
}

void RadarWnd::RadarItem::SetCoords(PointBase<float> const& coords)
{
    m_coords = coords;
}

void RadarWnd::RadarItem::SetSize(PointBase<float> const& size)
{
    m_size = size;
}

void RadarWnd::RadarItem::SetAngle(float angle)
{
    m_angle = angle;
}

void RadarWnd::RadarItem::SetTexture(m3d::rend::TexHandle texture)
{
    if (m_texture.IsValid())
    {
        M3D_RENDERER->ReleaseTexture(m_texture);
    }
    m_texture = texture;
    if (m_texture.IsValid())
    {
        M3D_RENDERER->ReferenceTexture(m_texture);
    }
}

void RadarWnd::RadarItem::Draw(m3d::ui::DrawInfo const& di) const
{
    float const halfW = m_size.x * 0.5f;
    float const halfH = m_size.y * 0.5f;
    float const xEdge = di.m_originalRect.x0;
    float const yEdge = di.m_originalRect.y0;

    if (!m_texture.IsValid())
    {
        M3D_RENDERER->SetWhiteTexture(0);
    }
    else
    {
        M3D_RENDERER->SetTexture(0, m_texture, -1.0);
    }

    M3D_APP->PutSpriteRelRot(
        m_coords.x + xEdge, m_coords.y + yEdge, halfW, halfH, 0xFFFFFFFF, m_angle, 0.0, 0.0, 0.0, 0.0, 0.0);
}

// ===========================================================================
//  RadarWnd::AuxInfo
// ===========================================================================

RadarWnd::AuxInfo::AuxInfo()
{
    // RVA 0x52BE60 - verified field by field against the shipped constructor.
    // Initialize radar icon names
    char const* iconNames[] = {
        "RadarVehicleOwnSmall",     "RadarVehicleOwnLarge",     "RadarVehicleEnemySmall", "RadarVehicleEnemyLarge",
        "RadarVehicleNeutralSmall", "RadarVehicleNeutralLarge", "RadarVehicleAllySmall",  "RadarVehicleAllyLarge",
        "RadarTurretOwnSmall",      "RadarTurretOwnLarge",      "RadarTurretEnemySmall",  "RadarTurretEnemyLarge",
        "RadarTurretNeutralSmall",  "RadarTurretNeutralLarge",  "RadarTurretAllySmall",   "RadarTurretAllyLarge",
        "RadarQuestMainFar",        "RadarQuestMainNear",       "RadarQuestUserFar",      "RadarQuestUserNear",
        "RadarNavPointFar",         "RadarNavPointNear",        "RadarWorldsideNord",     "RadarWorldsideSouth"};

    for (int i = 0; i < 24; ++i)
    {
        m_icoNames[i] = iconNames[i];
    }

    // Initialize icon sizes
    m_icoSizeVehicleSmall = {12.0f, 12.0f};
    m_icoSizeVehicleLarge = {16.0f, 16.0f};
    m_icoSizeTurretSmall = {14.0f, 14.0f};
    m_icoSizeTurretLarge = {14.0f, 14.0f};
    m_icoSizeMainQuestFar = {16.0f, 16.0f};
    m_icoSizeMainQuestNear = {12.0f, 12.0f};
    m_icoSizeUserQuestFar = {16.0f, 16.0f};
    m_icoSizeUserQuestNear = {12.0f, 12.0f};
    m_icoSizeUserLocationFar = {16.0f, 16.0f};
    m_icoSizeUserLocationNear = {12.0f, 12.0f};
    m_icoSizeWorldside = {8.0f, 16.0f};

    // Initialize radar geometry
    m_rotationCenter = {65.0f, 65.0f};
    m_innerRadius = 49.0f;
    m_outerRadius = 49.0f;

    // Initialize texture names
    m_cameraSightTexName = ("CameraSight");
    m_cameraSightSz = {128.0f, 64.0f};

    m_playerVehicleTexName = ("RadarPlayerVehicle");
    m_playerVehicleSz = {16.0f, 16.0f};

    m_highlightTexName = ("RadarHighlight");
    m_highlightB = {5.0f, 5.0f, 123.0f, 123.0f};

    // Initialize window distance names
    char const* distanceNames[] = {"wndDistance_MainQuest", "wndDistance_UserQuest", "wndDistance_UserLocation"};

    for (int i = 0; i < 3; ++i)
    {
        m_wndsDistancesNames[i] = (distanceNames[i]);
    }
}

// ===========================================================================
//  RadarWnd
// ===========================================================================

void RadarWnd::ClearRadarItemMap(std::map<int, std::vector<RadarWnd::RadarItem*>>& itemsMap)
{
    for (auto& [key, items] : itemsMap)
    {
        for (auto* item : items)
        {
            delete item;
        }
    }
    itemsMap.clear();
}

void RadarWnd::ClearVehicleItems()
{
    ClearRadarItemMap(m_vehicleItems);
}

void RadarWnd::ClearTurretItems()
{
    ClearRadarItemMap(m_turretItems);
}

void RadarWnd::ClearWorldsideItems()
{
    ClearRadarItemMap(m_worldsideItems);
}

void RadarWnd::ClearNavPointItems()
{
    ClearRadarItemMap(m_navPointItems);

    for (auto& distance : m_distances)
    {
        if (distance.m_wndDigital && distance.m_navPointType != NavPoint::NAVPOINT_TYPE_NUM_NAVPOINT_TYPES)
        {
            distance.m_navPointId = -1;
            distance.m_wndDigital->Clear();
            distance.Show(false);
        }
    }
}

void RadarWnd::Reset()
{
    m_bNavPointsAllowed = true;
    m_bVehiclesAllowed = false;
    m_bTurretsAllowed = false;
    m_bDistancesAllowed = false;
    m_bNavPointsEnabled = true;
    m_bVehiclesEnabled = true;
    m_bTurretsEnabled = true;
    m_bDistancesEnabled = true;
    m_scanRadius = m_cvDefaultRadarScanRadius.GetF();
}

float RadarWnd::GetScanRadius() const
{
    return m_scanRadius;
}

float RadarWnd::GetDefaultScanRadius() const
{
    return m_cvDefaultRadarScanRadius.GetF();
}

void RadarWnd::SetScanRadius(float radius)
{
    m_scanRadius = radius;
}

void RadarWnd::SetTest(int test)
{
    m_test = test;
}

void RadarWnd::AllowVehicles(bool bAllow)
{
    m_bVehiclesAllowed = bAllow;
}

void RadarWnd::AllowTurrets(bool bAllow)
{
    m_bTurretsAllowed = bAllow;
}

void RadarWnd::AllowNavPoints(bool bAllow)
{
    // RVA 0x52F8D0 - note the rebuild runs unconditionally, so disallowing nav
    // points here still repopulates m_navPointItems; as shipped.
    m_bNavPointsAllowed = bAllow;
    CreateNavPoints();
}

void RadarWnd::AllowDistances(bool bAllow)
{
    // RVA 0x52F900
    m_bDistancesAllowed = bAllow;
    if (bAllow)
    {
        for (auto const& [npId, items] : m_navPointItems)
        {
            AddDistance(npId);
        }
        return;
    }

    for (auto& distance : m_distances)
    {
        if (distance.m_wndDigital && distance.m_navPointType != NavPoint::NAVPOINT_TYPE_NUM_NAVPOINT_TYPES)
        {
            distance.m_navPointId = -1;
            distance.m_wndDigital->Clear();
            distance.Show(false);
        }
    }
}

float RadarWnd::GetIcoRotationAngle(PointBase<float> const& wndCoords) const
{
    return -std::atan2(m_aif.m_rotationCenter.x - wndCoords.x, m_aif.m_rotationCenter.y - wndCoords.y);
}

bool RadarWnd::IsPositionOutsideScanRadius(CVector const& worldPos) const
{
    if (!ai::thePlayer)
    {
        return false;
    }
    ai::Vehicle const* vehicle = ai::thePlayer->GetVehicle();
    if (!vehicle)
    {
        return false;
    }

    CVector const playerCoords = vehicle->GetPosition();
    float const dx = playerCoords.x - worldPos.x;
    float const dy = playerCoords.y - worldPos.y;
    float const dz = playerCoords.z - worldPos.z;
    return std::sqrt(dz * dz + dy * dy + dx * dx) > m_scanRadius;
}

CVector RadarWnd::FixCoord(CVector const& originalNpCoord) const
{
    CVector fixed = originalNpCoord;
    if (ai::thePlayer)
    {
        if (ai::Vehicle const* vehicle = ai::thePlayer->GetVehicle())
        {
            fixed.y = vehicle->GetPosition().y;
        }
    }
    return fixed;
}

m3d::rend::TexHandle RadarWnd::GetIcoForWorldside(Worldside side) const
{
    if (side == WORLDSIDE_NORD)
    {
        return m_icoTextures[22];
    }
    if (side == WORLDSIDE_SOUTH)
    {
        return m_icoTextures[23];
    }
    return m3d::rend::TexHandle{};
}

void RadarWnd::GetIcoForNavPoint(
    NavPoint::NavPointType npType,
    CVector const& worldCoord,
    m3d::rend::TexHandle& tex,
    PointBase<float>& icoSize) const
{
    tex.SetInvalid();
    icoSize.x = 0.0f;
    icoSize.y = 0.0f;

    bool const outside = IsPositionOutsideScanRadius(worldCoord);
    int texIndex;
    PointBase<float> size;

    switch (npType)
    {
    case NavPoint::NAVPOINT_TYPE_MAIN_QUEST:
        texIndex = outside ? 16 : 17;
        size = outside ? m_aif.m_icoSizeMainQuestFar : m_aif.m_icoSizeMainQuestNear;
        break;
    case NavPoint::NAVPOINT_TYPE_USER_QUEST:
        texIndex = outside ? 18 : 19;
        size = outside ? m_aif.m_icoSizeUserQuestFar : m_aif.m_icoSizeUserQuestNear;
        break;
    case NavPoint::NAVPOINT_TYPE_USER_LOCATION:
        texIndex = outside ? 20 : 21;
        size = outside ? m_aif.m_icoSizeUserLocationFar : m_aif.m_icoSizeUserLocationNear;
        break;
    default:
        return;
    }

    icoSize = size;
    tex = m_icoTextures[texIndex];
}

void RadarWnd::GetIcoForVehicle(ai::Vehicle const* v, m3d::rend::TexHandle& tex, PointBase<float>& icoSize) const
{
    tex.SetInvalid();
    icoSize.x = 0.0f;
    icoSize.y = 0.0f;
    if (!v || !ai::thePlayer)
    {
        return;
    }

    ai::Vehicle const* playerVehicle = ai::thePlayer->GetVehicle();
    if (!playerVehicle)
    {
        return;
    }

    int const objId = v->GetId();
    bool const targeted = playerVehicle->GetInfoObjId() == objId || playerVehicle->GetLockedObjId() == objId ||
        playerVehicle->GetToBeLockedObjId() == objId;

    int base;
    switch (ai::pServer->CheckTolerance(v->GetBelong(), playerVehicle->GetBelong()))
    {
    case ai::RS_OWN:
        base = 0;
        break;
    case ai::RS_ENEMY:
        base = 2;
        break;
    case ai::RS_NEUTRAL:
        base = 4;
        break;
    case ai::RS_ALLY:
        base = 6;
        break;
    default:
        return;
    }

    tex = m_icoTextures[base + (targeted ? 1 : 0)];
    icoSize = targeted ? m_aif.m_icoSizeVehicleLarge : m_aif.m_icoSizeVehicleSmall;
}

void RadarWnd::GetIcoForTurret(ai::StaticAutoGun const* t, m3d::rend::TexHandle& tex, PointBase<float>& icoSize) const
{
    tex.SetInvalid();
    icoSize.x = 0.0f;
    icoSize.y = 0.0f;
    if (!t || !ai::thePlayer)
    {
        return;
    }

    ai::Vehicle const* playerVehicle = ai::thePlayer->GetVehicle();
    if (!playerVehicle)
    {
        return;
    }

    int const objId = t->GetId();
    bool const targeted = playerVehicle->GetInfoObjId() == objId || playerVehicle->GetLockedObjId() == objId ||
        playerVehicle->GetToBeLockedObjId() == objId;

    int base;
    switch (ai::pServer->CheckTolerance(t->GetBelong(), playerVehicle->GetBelong()))
    {
    case ai::RS_OWN:
        base = 8;
        break;
    case ai::RS_ENEMY:
        base = 10;
        break;
    case ai::RS_NEUTRAL:
        base = 12;
        break;
    case ai::RS_ALLY:
        base = 14;
        break;
    default:
        return;
    }

    tex = m_icoTextures[base + (targeted ? 1 : 0)];
    icoSize = targeted ? m_aif.m_icoSizeTurretLarge : m_aif.m_icoSizeTurretSmall;
}

void RadarWnd::GetIcoForObject(ai::Obj const* o, m3d::rend::TexHandle& tex, PointBase<float>& icoSize) const
{
    tex.SetInvalid();
    icoSize.x = 0.0f;
    icoSize.y = 0.0f;
    if (!o)
    {
        return;
    }

    if (o->IsKindOf(&ai::Vehicle::m_classVehicle))
    {
        GetIcoForVehicle(static_cast<ai::Vehicle const*>(o), tex, icoSize);
    }
    else if (o->IsKindOf(&ai::StaticAutoGun::m_classStaticAutoGun))
    {
        GetIcoForTurret(static_cast<ai::StaticAutoGun const*>(o), tex, icoSize);
    }
}

PointBase<float> RadarWnd::WorldToRadarCoords(CVector const& wrldCoords, ItemType type) const
{
    PointBase<float> result{};

    if (!ai::thePlayer)
    {
        return result;
    }
    ai::Vehicle const* vehicle = ai::thePlayer->GetVehicle();
    if (!vehicle)
    {
        return result;
    }

    CVector const playerCoords = vehicle->GetPosition();
    float const scanRadius = m_scanRadius;
    float wndRadarRadius = m_aif.m_innerRadius;
    float edgeX = m_aif.m_rotationCenter.x - wndRadarRadius;
    float edgeY = m_aif.m_rotationCenter.y - wndRadarRadius;

    CVector c;
    c.x = wrldCoords.x - playerCoords.x;
    c.y = wrldCoords.y - playerCoords.y;
    c.z = wrldCoords.z - playerCoords.z;

    bool clampToEdge = (type == ITEMTYPE_WORLDSIDE);
    if (type == ITEMTYPE_NAVPOINT && std::sqrt(c.z * c.z + c.x * c.x + c.y * c.y) > scanRadius)
    {
        clampToEdge = true;
    }
    if (clampToEdge)
    {
        CVector const n = c.getNormalized();
        c.x = n.x * scanRadius;
        c.y = n.y * scanRadius;
        c.z = n.z * scanRadius;
        wndRadarRadius = m_aif.m_outerRadius;
        edgeX = m_aif.m_rotationCenter.x - wndRadarRadius;
        edgeY = m_aif.m_rotationCenter.y - wndRadarRadius;
    }

    CVector const dir = vehicle->GetDirection();
    float const invLen = 1.0f / std::sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z + 0.00000011920929f);
    float const yaw = std::atan2(invLen * dir.x, invLen * dir.z);
    float const cs = std::cos(yaw);
    float const sn = std::sin(yaw);

    float const rotatedZ = c.z * cs + c.x * sn;
    result.x = ((c.x * cs - c.z * sn) / scanRadius) * wndRadarRadius + (edgeX + wndRadarRadius);
    result.y = ((-1.0f / scanRadius) * rotatedZ) * wndRadarRadius + (edgeY + wndRadarRadius);
    return result;
}

void RadarWnd::DrawBg(m3d::ui::DrawInfo const& di)
{
    m3d::ui::Wnd::OnPaint(di);
}

void RadarWnd::DrawPlayerVehicle(m3d::ui::DrawInfo const& di) const
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    float const halfW = m_aif.m_playerVehicleSz.x * 0.5f;
    float const halfH = m_aif.m_playerVehicleSz.y * 0.5f;
    float const y = m_aif.m_rotationCenter.y;
    float const x0 = m_aif.m_rotationCenter.x - halfW;

    BoundsBase<float> playerVehicleB;
    playerVehicleB.x0 = x0;
    playerVehicleB.y0 = y - halfH;
    playerVehicleB.width = (m_aif.m_rotationCenter.x + halfW) - x0;
    playerVehicleB.height = (y + halfH) - (y - halfH);
    m3d::ui::Wnd::m_gfx->AddImagedRect(di, playerVehicleB, 0xFFFFFFFF, m_playerVehicleTex);
}

void RadarWnd::DrawHighlight(m3d::ui::DrawInfo const& di) const
{
    if ((m_gameDataFlags & 1) != 0)
    {
        m3d::ui::Wnd::m_gfx->AddImagedRect(di, m_aif.m_highlightB, 0xFFFFFFFF, m_highlightTex);
    }
}

int RadarWnd::GameDataClear(bool beforeContinuousLevel)
{
    ClearRadarItemMap(m_vehicleItems);
    ClearRadarItemMap(m_turretItems);
    ClearNavPointItems();
    ClearRadarItemMap(m_worldsideItems);
    if (!beforeContinuousLevel)
    {
        Reset();
    }
    return 1;
}

m3d::Class* RadarWnd::GetClass() const
{
    return RT_CLASS_LOCAL(RadarWnd);
}

m3d::Object* RadarWnd::Clone()
{
    return new RadarWnd(*this);
}

m3d::Object* RadarWnd::CreateObject()
{
    return new RadarWnd;
}

m3d::Class* RadarWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

RadarWnd::RadarWnd() :
    m_cvDefaultRadarScanRadius("defaultRadarScanRadius", "500", m3d::CVar::CVAR_FLOAT, m3d::CVar::CVAR_ARCHIVE)
{
    M3D_ENGINE_CFG.m_console->RegisterCVar(&m_cvDefaultRadarScanRadius, 0);
    m_test = 0;
    m_bNavPointsAllowed = 1;
    m_bVehiclesAllowed = 0;
    m_bTurretsAllowed = 0;
    m_bDistancesAllowed = 0;
    m_bNavPointsEnabled = 1;
    m_bVehiclesEnabled = 1;
    m_bTurretsEnabled = 1;
    m_bDistancesEnabled = 1;
    m_scanRadius = m_cvDefaultRadarScanRadius.GetF();
}

RadarWnd::RadarWnd(RadarWnd const&) : RadarWnd()
{
}

RadarWnd::~RadarWnd()
{
    ClearRadarItemMap(m_vehicleItems);
    ClearRadarItemMap(m_turretItems);
    ClearNavPointItems();
    ClearRadarItemMap(m_worldsideItems);
    Reset();

    for (auto& tex : m_icoTextures)
    {
        M3D_RENDERER->ReleaseTexture(tex);
    }
    M3D_RENDERER->ReleaseTexture(m_cameraSightTex);
    M3D_RENDERER->ReleaseTexture(m_playerVehicleTex);
    M3D_RENDERER->ReleaseTexture(m_highlightTex);

    M3D_ENGINE_CFG.m_console->UnregisterCVar(&m_cvDefaultRadarScanRadius);
}

// ===========================================================================
//  Setup / teardown driven by the game data protocol
// ===========================================================================

int RadarWnd::GameDataSetup()
{
    // RVA 0x52CE40 - resolves every radar icon plus the three distance digitals
    // (built from XML pattern windows) and records one NpDistance per nav-point
    // type in m_distances.
    if ((m_gameDataFlags & 2) == 0)
    {
        for (int i = 0; i < RADARICO_NUM_RADARICOS; ++i)
        {
            m_icoTextures[i] = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_icoNames[i], 0);
            M3D_RENDERER->ReferenceTexture(m_icoTextures[i]);
        }
        m_cameraSightTex = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_cameraSightTexName, 0);
        M3D_RENDERER->ReferenceTexture(m_cameraSightTex);
        m_playerVehicleTex = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_playerVehicleTexName, 0);
        M3D_RENDERER->ReferenceTexture(m_playerVehicleTex);
        m_highlightTex = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_highlightTexName, 0);
        M3D_RENDERER->ReferenceTexture(m_highlightTex);

        int res = 1;
        for (int i = 0; i < NavPoint::NAVPOINT_TYPE_NUM_NAVPOINT_TYPES; ++i)
        {
            CStr const& wndName = m_aif.m_wndsDistancesNames[i];
            ElectronicDigitalWnd* wndDigital = nullptr;

            m3d::Object* child = GetChildByName(wndName);
            if (child && child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
            {
                wndDigital = static_cast<ElectronicDigitalWnd*>(m3d::g_Kernel->New("ElectronicDigitalWnd"));
                if (!wndDigital)
                {
                    M3D_LOG_INFO(
                        "Make control error: cannot create " + wndName +
                        " - cannot find rtti class ElectronicDigitalWnd");
                    res = 0;
                }
                else if (!wndDigital->CreateFromPattern(static_cast<m3d::ui::Wnd*>(child), true))
                {
                    M3D_LOG_INFO("Make control error: cannot create " + wndName + " from pattern class");
                    res = 0;
                }
            }
            else
            {
                M3D_LOG_INFO("Make control error: control " + wndName + " is not found or incorrect type");
                res = 0;
            }

            m_distances.push_back(NpDistance(wndDigital, static_cast<NavPoint::NavPointType>(i)));
            if (wndDigital)
            {
                wndDigital->SetDigitalSize(ElectronicDigitalWnd::DIGITAL_SIZE_SMALL);
            }
        }

        if (res)
        {
            m_gameDataFlags |= 1u;
        }
    }

    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("RadarWnd: error - fail to init because of a bad resource");
        return 0;
    }
    return 1;
}

int RadarWnd::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x52D600
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    switch (dataType)
    {
    case 28:
        OnAddNavPoint(data);
        break;
    case 29:
        OnDeleteNavPoint(data);
        break;
    case 85:
        OnStartLevel();
        break;
    case 89:
        UpdateOnNewFrame();
        break;
    }
    return 1;
}

int RadarWnd::GameDataSave(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode)
{
    // RVA 0x52F9C0
    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("RadarWnd::GameDataSave error - journal has been not properly inited");
        return 0;
    }
    if (!xmlFile || !guiNode)
    {
        M3D_LOG_INFO("RadarWnd::GameDataSave error - invalid params");
        return 0;
    }

    ref_ptr radarNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Radar");
    guiNode->AddChild(radarNode);

    radarNode->SetAttribute("ScanRadius", CStr(m_scanRadius).c_str());
    radarNode->SetAttribute("NavPointsAllowed", CStr(m_bNavPointsAllowed).c_str());
    radarNode->SetAttribute("VehiclesAllowed", CStr(m_bVehiclesAllowed).c_str());
    radarNode->SetAttribute("TurretsAllowed", CStr(m_bTurretsAllowed).c_str());
    radarNode->SetAttribute("DistancesAllowed", CStr(m_bDistancesAllowed).c_str());
    return 1;
}

int RadarWnd::GameDataLoad(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode)
{
    // RVA 0x52FC30
    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("RadarWnd::GameDataLoad error - journal has been not properly inited");
        return 0;
    }
    GameDataClear(false);
    if (!xmlFile || !guiNode)
    {
        M3D_LOG_INFO("RadarWnd::GameDataLoad error - invalid params");
        return 0;
    }

    ref_ptr radarNode = xmlFile->CreateNode();
    guiNode->GetFirstChild(radarNode, "Radar");
    if (radarNode->IsEmpty())
    {
        M3D_LOG_INFO("RadarWnd::GameDataLoad error - cannot find journal node");
        return 0;
    }

    float scanRadius = 0.0f;
    bool bNavPointsAllowed = true;
    bool bVehiclesAllowed = false;
    bool bTurretsAllowed = false;
    bool bDistancesAllowed = false;
    m3d::SafeFloatAttrib(scanRadius, radarNode, "ScanRadius");
    m3d::SafeBoolAttrib(bNavPointsAllowed, radarNode, "NavPointsAllowed");
    m3d::SafeBoolAttrib(bVehiclesAllowed, radarNode, "VehiclesAllowed");
    m3d::SafeBoolAttrib(bTurretsAllowed, radarNode, "TurretsAllowed");
    m3d::SafeBoolAttrib(bDistancesAllowed, radarNode, "DistancesAllowed");

    // NOTE: nav points go through the AllowNavPoints() sequence (assign, then
    // rebuild the list) while vehicles and turrets are plain assignments and
    // distances go through AllowDistances(); as shipped.
    m_bNavPointsAllowed = bNavPointsAllowed;
    m_scanRadius = scanRadius;
    CreateNavPoints();
    m_bVehiclesAllowed = bVehiclesAllowed;
    m_bTurretsAllowed = bTurretsAllowed;
    AllowDistances(bDistancesAllowed);
    OnStartLevel();
    return 1;
}

void RadarWnd::OnStartLevel()
{
    // RVA 0x52D6D0
    CreateWorldSides();
    if (m_bNavPointsAllowed)
    {
        CreateNavPoints();
    }
}

void RadarWnd::CreateWorldSides()
{
    // RVA 0x52D770
    ClearWorldsideItems();
    for (int i = 0; i < WORLDSIDE_NUM_WORLDSIDES; ++i)
    {
        AddWorldside(static_cast<Worldside>(i));
    }
}

void RadarWnd::CreateNavPoints()
{
    // RVA 0x52D7A0
    ClearNavPointItems();

    std::vector<int> const nps =
        M3D_APP->m_pInterfaceManager->GetNavPointManager()->GetNavPointsForLevel(help::GetCurrentLevelName());
    for (int i = 0; i < static_cast<int>(nps.size()); ++i)
    {
        AddNavPoint(nps[i]);
    }
}

// ===========================================================================
//  Per-frame update
// ===========================================================================

int RadarWnd::UpdateOnNewFrame()
{
    // RVA 0x52DA10
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    UpdateWorldsidesOnNewFrame();
    if (m_bVehiclesAllowed && m_bVehiclesEnabled)
    {
        UpdateVehiclesOnNewFrame();
    }
    if (m_bTurretsAllowed && m_bTurretsEnabled)
    {
        UpdateTurretsOnNewFrame();
    }
    if (m_bNavPointsAllowed && m_bNavPointsEnabled)
    {
        UpdateNavPointsOnNewFrame();
    }
    if (m_bDistancesAllowed && m_bDistancesEnabled)
    {
        UpdateDistancesOnNewFrame();
    }
    return 1;
}

int RadarWnd::UpdateObjectsOnNewFrame(RadarItemMap& items, m3d::Class const* cls)
{
    // RVA 0x52DAB0 - syncs `items` with every live object of class `cls`: adds or
    // refreshes the ones inside the scan radius, drops the rest, then removes the
    // entries whose object no longer exists at all.
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (!cls || !cls->IsKindOf(&ai::PhysicObj::m_classPhysicObj))
    {
        return 0;
    }

    std::set<int> const* os = M3D_APP->m_pInterfaceManager->GetObjectCollection().GetObjectsByClass(cls);
    if (!os)
    {
        return 0;
    }

    for (auto vIt = os->begin(); vIt != os->end(); ++vIt)
    {
        ai::Obj* o = ai::theObjects->GetEntityByObjId(*vIt);
        if (!o || !o->IsKindOf(cls))
        {
            continue;
        }

        bool const known = items.find(o->GetId()) != items.end();
        if (o->bIsVisible())
        {
            CVector const coord = FixCoord(static_cast<ai::PhysicObj*>(o)->GetPosition());
            if (!IsPositionOutsideScanRadius(coord) && o != help::GetPlayerVehicle())
            {
                if (known)
                {
                    UpdateObject(o->GetId(), items);
                }
                else
                {
                    AddObject(o->GetId(), items);
                }
                continue;
            }
        }
        if (known)
        {
            RemoveItem(items, o->GetId());
        }
    }

    for (auto mIt = items.begin(); mIt != items.end();)
    {
        int const objId = mIt->first;
        ++mIt;
        if (os->find(objId) == os->end())
        {
            RemoveItem(items, objId);
        }
    }
    return 1;
}

int RadarWnd::UpdateVehiclesOnNewFrame()
{
    // RVA 0x52DFC0
    return UpdateObjectsOnNewFrame(m_vehicleItems, &ai::Vehicle::m_classVehicle);
}

int RadarWnd::UpdateTurretsOnNewFrame()
{
    // RVA 0x52E010
    return UpdateObjectsOnNewFrame(m_turretItems, &ai::StaticAutoGun::m_classStaticAutoGun);
}

int RadarWnd::UpdateNavPointsOnNewFrame()
{
    // RVA 0x52E880
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    int res = 1;
    for (auto const& [npId, items] : m_navPointItems)
    {
        res &= UpdateNavPoint(npId);
    }
    return res;
}

int RadarWnd::UpdateWorldsidesOnNewFrame()
{
    // RVA 0x52EE90
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    int res = 1;
    for (auto const& [side, items] : m_worldsideItems)
    {
        res &= UpdateWorldside(static_cast<Worldside>(side));
    }
    return res;
}

int RadarWnd::UpdateDistancesOnNewFrame()
{
    // RVA 0x52ECC0
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    int res = 1;
    for (int i = NavPoint::NAVPOINT_TYPE_MAIN_QUEST; i < NavPoint::NAVPOINT_TYPE_NUM_NAVPOINT_TYPES; ++i)
    {
        res &= UpdateDistance(static_cast<NavPoint::NavPointType>(i));
    }
    return res;
}

// ===========================================================================
//  Generic object items
// ===========================================================================

int RadarWnd::AddObject(int objId, RadarItemMap& items)
{
    // RVA 0x52DC70
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    ai::Obj* o = ai::theObjects->GetEntityByObjId(objId);
    if (!o || !o->IsKindOf(&ai::PhysicObj::m_classPhysicObj))
    {
        return 0;
    }
    if (items.find(objId) != items.end())
    {
        return 0;
    }

    PointBase<float> const radarCoord =
        WorldToRadarCoords(FixCoord(static_cast<ai::PhysicObj*>(o)->GetPosition()), ITEMTYPE_OBJECT);

    m3d::rend::TexHandle tex;
    PointBase<float> icoSz{0.0f, 0.0f};
    GetIcoForObject(o, tex, icoSz);

    RadarItemVector itemsVector;
    itemsVector.push_back(new RadarItem(tex, icoSz, radarCoord, 0.0f));
    items.insert(RadarItemMap::value_type(objId, itemsVector));
    return 1;
}

int RadarWnd::UpdateObject(int objId, RadarItemMap const& items)
{
    // RVA 0x52DE60
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    ai::Obj* o = ai::theObjects->GetEntityByObjId(objId);
    if (!o || !o->IsKindOf(&ai::PhysicObj::m_classPhysicObj))
    {
        return 0;
    }

    auto const mIt = items.find(objId);
    if (mIt == items.end() || mIt->second.size() != 1 || !mIt->second[0])
    {
        return 0;
    }
    RadarItem* item = mIt->second[0];

    item->m_coords = WorldToRadarCoords(FixCoord(static_cast<ai::PhysicObj*>(o)->GetPosition()), ITEMTYPE_OBJECT);

    m3d::rend::TexHandle tex;
    PointBase<float> icoSz{0.0f, 0.0f};
    GetIcoForObject(o, tex, icoSz);
    item->SetTexture(tex);
    item->m_size = icoSz;
    return 1;
}

int RadarWnd::RemoveObject(int objId, RadarItemMap& items)
{
    // RVA 0x52DFB0
    return RemoveItem(items, objId);
}

int RadarWnd::RemoveItem(RadarItemMap& itemMap, int itemId)
{
    // RVA 0x52E060
    auto const mIt = itemMap.find(itemId);
    if (mIt == itemMap.end())
    {
        return 0;
    }

    for (auto*& item : mIt->second)
    {
        delete item;
        item = nullptr;
    }
    itemMap.erase(mIt);
    return 1;
}

int RadarWnd::AddVehicle(int vehicleId)
{
    // RVA 0x52DFE0
    return AddObject(vehicleId, m_vehicleItems);
}

int RadarWnd::UpdateVehicle(int vehicleId)
{
    // RVA 0x52DFF0
    return UpdateObject(vehicleId, m_vehicleItems);
}

int RadarWnd::RemoveVehicle(int vehicleId)
{
    // RVA 0x52E000
    return RemoveItem(m_vehicleItems, vehicleId);
}

int RadarWnd::AddTurret(int turretId)
{
    // RVA 0x52E030
    return AddObject(turretId, m_turretItems);
}

int RadarWnd::UpdateTurret(int turretId)
{
    // RVA 0x52E040
    return UpdateObject(turretId, m_turretItems);
}

int RadarWnd::RemoveTurret(int turretId)
{
    // RVA 0x52E050
    return RemoveItem(m_turretItems, turretId);
}

// ===========================================================================
//  Nav points
// ===========================================================================

int RadarWnd::AddNavPoint(int npId)
{
    // RVA 0x52EA70
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    NavPoint const* np = M3D_APP->m_pInterfaceManager->GetNavPointManager()->GetNavPointById(npId);
    if (!np)
    {
        return 0;
    }
    if (m_navPointItems.find(npId) != m_navPointItems.end())
    {
        return 0;
    }
    if (np->GetLevelName() != help::GetCurrentLevelName())
    {
        return 0;
    }

    CVector const* coordinate = np->GetCoordinate();
    if (!coordinate)
    {
        return 0;
    }

    CVector const worldCoord = FixCoord(*coordinate);
    PointBase<float> const radarCoord = WorldToRadarCoords(worldCoord, ITEMTYPE_NAVPOINT);

    m3d::rend::TexHandle tex;
    PointBase<float> icoSize{0.0f, 0.0f};
    GetIcoForNavPoint(np->GetNavPointType(), worldCoord, tex, icoSize);

    RadarItemVector items;
    items.push_back(new RadarItem(tex, icoSize, radarCoord, 0.0f));
    m_navPointItems.insert(RadarItemMap::value_type(npId, items));

    if (m_bDistancesAllowed && m_bDistancesEnabled)
    {
        return AddDistance(npId);
    }
    return 1;
}

int RadarWnd::UpdateNavPoint(int npId)
{
    // RVA 0x52E8D0
    auto const npIt = m_navPointItems.find(npId);
    if (npIt == m_navPointItems.end())
    {
        return 0;
    }

    NavPoint const* np = M3D_APP->m_pInterfaceManager->GetNavPointManager()->GetNavPointById(npId);
    if (!np)
    {
        return 0;
    }

    CVector const* coordinate = np->GetCoordinate();
    if (!coordinate)
    {
        return 0;
    }
    CVector const worldCoord = FixCoord(*coordinate);

    for (auto* item : npIt->second)
    {
        if (!item)
        {
            continue;
        }

        item->m_coords = WorldToRadarCoords(worldCoord, ITEMTYPE_NAVPOINT);
        item->m_angle = GetIcoRotationAngle(item->m_coords);

        m3d::rend::TexHandle tex;
        PointBase<float> size{0.0f, 0.0f};
        GetIcoForNavPoint(np->GetNavPointType(), worldCoord, tex, size);
        item->SetTexture(tex);
        item->m_size = size;
    }
    return 1;
}

int RadarWnd::RemoveNavPoint(int npId)
{
    // RVA 0x52EC80
    int const res = RemoveItem(m_navPointItems, npId) & 1;
    if (m_bDistancesAllowed && m_bDistancesEnabled)
    {
        return RemoveDistance(npId) & res;
    }
    return res;
}

int RadarWnd::OnAddNavPoint(void* data)
{
    // RVA 0x52F870 - the nav-point id lives at offset 52 of the message payload.
    if (!data)
    {
        return 0;
    }
    if (!m_bNavPointsAllowed)
    {
        return 1;
    }
    return AddNavPoint(static_cast<int*>(data)[13]);
}

int RadarWnd::OnDeleteNavPoint(void* data)
{
    // RVA 0x52F890 - note this repeats RemoveNavPoint()'s body rather than
    // calling it; behaviour is identical.
    if (!data)
    {
        return 0;
    }

    int const npId = static_cast<int*>(data)[13];
    int const res = RemoveItem(m_navPointItems, npId) & 1;
    if (m_bDistancesAllowed && m_bDistancesEnabled)
    {
        return RemoveDistance(npId) & res;
    }
    return res;
}

// ===========================================================================
//  Distance readouts
// ===========================================================================

int RadarWnd::AddDistance(int npId)
{
    // RVA 0x52ED70 - one digital readout per nav-point type, so adding a distance
    // means pointing that type's readout at this nav point.
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (npId == -1)
    {
        return 0;
    }

    NavPoint const* np = M3D_APP->m_pInterfaceManager->GetNavPointManager()->GetNavPointById(npId);
    if (!np)
    {
        return 0;
    }

    NavPoint::NavPointType const npType = np->GetNavPointType();
    if (npType == NavPoint::NAVPOINT_TYPE_NUM_NAVPOINT_TYPES)
    {
        return 0;
    }
    return m_distances[npType].SetNavPointId(npId);
}

int RadarWnd::RemoveDistance(int npId)
{
    // RVA 0x52EDF0
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    for (int i = 0; i < static_cast<int>(m_distances.size()); ++i)
    {
        NpDistance& distance = m_distances[i];
        if (distance.m_navPointId != npId)
        {
            continue;
        }
        if (distance.IsValid())
        {
            distance.m_navPointId = -1;
            distance.m_wndDigital->Clear();
            distance.Show(false);
        }
        return 1;
    }
    return 0;
}

int RadarWnd::UpdateDistance(NavPoint::NavPointType npType)
{
    // RVA 0x52ED00
    if ((m_gameDataFlags & 1) == 0 || npType < 0 || npType >= static_cast<int>(m_distances.size()))
    {
        return 0;
    }

    NpDistance& distance = m_distances[npType];
    if (distance.m_navPointId == -1)
    {
        return 1;
    }
    return distance.SetDistance(CalculateDistanceToNavPoint(distance.m_navPointId));
}

float RadarWnd::CalculateDistanceToNavPoint(int npId) const
{
    // RVA 0x52F600
    if (npId == -1)
    {
        return -1.0f;
    }

    NavPoint const* np = M3D_APP->m_pInterfaceManager->GetNavPointManager()->GetNavPointById(npId);
    if (!np)
    {
        return -1.0f;
    }

    CVector const* coordinate = np->GetCoordinate();
    if (!coordinate)
    {
        return -1.0f;
    }
    CVector const npPos = FixCoord(*coordinate);

    ai::Vehicle const* vehicle = help::GetPlayerVehicle();
    if (!vehicle)
    {
        return -1.0f;
    }

    CVector const playerPos = vehicle->GetPosition();
    float const dx = npPos.x - playerPos.x;
    float const dy = npPos.y - playerPos.y;
    float const dz = npPos.z - playerPos.z;
    return std::sqrt(dz * dz + dy * dy + dx * dx);
}

// ===========================================================================
//  World sides
// ===========================================================================

int RadarWnd::AddWorldside(Worldside side)
{
    // RVA 0x52EF90
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (side >= WORLDSIDE_NUM_WORLDSIDES)
    {
        return 0;
    }

    PointBase<float> const radarCoord = WorldToRadarCoords(GetWorldsideCoords(side), ITEMTYPE_WORLDSIDE);

    RadarItemVector items;
    items.push_back(new RadarItem(GetIcoForWorldside(side), m_aif.m_icoSizeWorldside, radarCoord, 0.0f));
    m_worldsideItems.insert(RadarItemMap::value_type(side, items));
    return 1;
}

int RadarWnd::UpdateWorldside(Worldside side)
{
    // RVA 0x52EEE0
    auto const wsIt = m_worldsideItems.find(side);
    if (wsIt == m_worldsideItems.end())
    {
        return 0;
    }
    if (wsIt->second.size() != 1 || !wsIt->second[0])
    {
        return 0;
    }

    RadarItem* item = wsIt->second[0];
    item->m_coords = WorldToRadarCoords(GetWorldsideCoords(side), ITEMTYPE_WORLDSIDE);
    item->m_angle = GetIcoRotationAngle(item->m_coords);
    return 1;
}

int RadarWnd::RemoveWorldside(Worldside side)
{
    // RVA 0x52F0C0
    return RemoveItem(m_worldsideItems, side);
}

CVector RadarWnd::GetWorldsideCoords(Worldside side) const
{
    // RVA 0x52F0D0 - north comes from the level info; south is north turned by
    // pi around the vertical axis. Both are reported relative to the player.
    CVector north{0.0f, 0.0f, -1.0f};

    LevelInfo const* levelInfo =
        M3D_APP->m_pInterfaceManager->GetLevelInfoManager()->GetLevelInfoByName(help::GetCurrentLevelName());
    if (levelInfo)
    {
        north = levelInfo->GetNorth();
    }

    float const s = std::sin(3.141592741012573f);
    float const c = std::cos(3.141592741012573f);

    CVector worldDirs[WORLDSIDE_NUM_WORLDSIDES];
    worldDirs[WORLDSIDE_NORD] = north;
    worldDirs[WORLDSIDE_SOUTH].x = c * north.x - north.z * s;
    worldDirs[WORLDSIDE_SOUTH].y = north.y;
    worldDirs[WORLDSIDE_SOUTH].z = north.z * c + s * north.x;

    ai::Vehicle const* vehicle = help::GetPlayerVehicle();
    if (side >= WORLDSIDE_NUM_WORLDSIDES || !vehicle)
    {
        return CVector{0.0f, 0.0f, 0.0f};
    }

    CVector const playerPos = vehicle->GetPosition();
    return CVector{
        worldDirs[side].x + playerPos.x, worldDirs[side].y + playerPos.y, worldDirs[side].z + playerPos.z};
}

// ===========================================================================
//  Painting
// ===========================================================================

void RadarWnd::DrawItems(m3d::ui::DrawInfo const& di, RadarItemMap const& items) const
{
    // RVA 0x52F2C0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    for (auto const& [itemId, itemVector] : items)
    {
        for (auto* item : itemVector)
        {
            if (item)
            {
                item->Draw(di);
            }
        }
    }
}

void RadarWnd::DrawVehicles(m3d::ui::DrawInfo const& di) const
{
    // RVA 0x52F370
    DrawItems(di, m_vehicleItems);
}

void RadarWnd::DrawTurrets(m3d::ui::DrawInfo const& di) const
{
    // RVA 0x52F380
    DrawItems(di, m_turretItems);
}

void RadarWnd::DrawNavPoints(m3d::ui::DrawInfo const& di) const
{
    // RVA 0x52F390
    DrawItems(di, m_navPointItems);
}

void RadarWnd::DrawWorldsides(m3d::ui::DrawInfo const& di) const
{
    // RVA 0x52F3A0
    DrawItems(di, m_worldsideItems);
}

void RadarWnd::DrawCameraSight(m3d::ui::DrawInfo const& di) const
{
    // RVA 0x52F3C0 - the cone showing where the camera looks, rotated so that it
    // trails the vehicle heading; hidden in fly-camera mode.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    ai::Vehicle const* vehicle = help::GetPlayerVehicle();
    if (!vehicle)
    {
        return;
    }
    if (M3D_APP->m_player.m_cameraMode == CM_FLYCAMERA)
    {
        return;
    }

    PointBase<float> const coord = m_aif.m_rotationCenter;
    CVector const vehicleDir = vehicle->GetDirection();

    float const halfW = m_aif.m_cameraSightSz.x * 0.5f;
    float const halfH = m_aif.m_cameraSightSz.y * 0.5f;
    float const xEdge = di.m_originalRect.x0;
    float const yEdge = di.m_originalRect.y0;
    float const cameraAngle = -std::atan2(vehicleDir.x, vehicleDir.z) - M3D_APP->m_curCamera.m_rotYaw;

    if (!m_cameraSightTex.IsValid())
    {
        M3D_RENDERER->SetWhiteTexture(0);
    }
    else
    {
        M3D_RENDERER->SetTexture(0, m_cameraSightTex, -1.0);
    }

    M3D_APP->PutSpriteRelRot(
        coord.x + xEdge,
        (coord.y + yEdge) - halfH,
        halfW,
        halfH,
        0xFFFFFFFF,
        cameraAngle,
        0.0,
        halfH,
        0.0,
        0.0,
        0.0);
}

void RadarWnd::OnPaintOverChildren(m3d::ui::DrawInfo const& di)
{
    // RVA 0x52F6D0
    M3D_RENDERER->SetAlphaTest(M3D_ENGINE_CFG.m_alphaTestInterface.GetI());
    M3D_RENDERER->SetStageState(0, m3d::rend::BM_COLOR, m3d::rend::TS_MODULATE);
    M3D_RENDERER->SetStageState(0, m3d::rend::BM_ALPHA, m3d::rend::TS_MODULATE);
    M3D_RENDERER->SetStageState(1, m3d::rend::BM_COLOR, m3d::rend::TS_NONE);
    // NOTE: the shipped build repeats the previous call verbatim here (the second
    // one was presumably meant to be BM_ALPHA); reproduced as-is.
    M3D_RENDERER->SetStageState(1, m3d::rend::BM_COLOR, m3d::rend::TS_NONE);
    M3D_RENDERER->PushBlend(m3d::rend::BM_ALPHA);
    M3D_RENDERER->PushZbState(m3d::rend::ZB_DISABLE);

    DrawCameraSight(di);
    DrawWorldsides(di);
    DrawPlayerVehicle(di);
    if (m_bTurretsAllowed && m_bTurretsEnabled)
    {
        DrawTurrets(di);
    }
    if (m_bVehiclesAllowed && m_bVehiclesEnabled)
    {
        DrawVehicles(di);
    }
    if (m_bNavPointsAllowed && m_bNavPointsEnabled)
    {
        DrawNavPoints(di);
    }
    DrawHighlight(di);

    M3D_RENDERER->SetAlphaTest(0);
    M3D_RENDERER->PopBlend();
    M3D_RENDERER->PopZbState();
}
