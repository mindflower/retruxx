#include "radarwnd.h"

#include <cmath>

#include "config.h"
#include "electronicdigitalwnd.h"
#include "core/kernel.h"
#include "core/console/console.h"
#include <game/m3dgame.h>
#include <math/vector.h>
#include "server/server.h"
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
    // TODO: generated code
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
    m_bNavPointsAllowed = bAllow;
    // TODO: the shipped binary rebuilds the nav-point item list here (CreateNavPoints()).
}

void RadarWnd::AllowDistances(bool bAllow)
{
    m_bDistancesAllowed = bAllow;
    if (bAllow)
    {
        // TODO: the shipped binary walks m_navPointItems calling AddDistance() for each here.
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

// ---------------------------------------------------------------------------
//  Not yet ported: nav-point / scene-graph, per-frame updates and the item
//  add/remove/update plumbing (RadarWnd::AddObject / RemoveItem / UpdateObject
//  / DrawItems helpers do not exist yet, and these depend on NavPointManager,
//  LevelInfoManager and help::GetCurrentLevelName).
// ---------------------------------------------------------------------------

int RadarWnd::UpdateWorldsidesOnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::UpdateNavPointsOnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::UpdateWorldside(Worldside)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::DrawCameraSight(m3d::ui::DrawInfo const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::AddWorldside(Worldside)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::DrawNavPoints(m3d::ui::DrawInfo const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::CreateWorldSides()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::CreateNavPoints()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::RemoveNavPoint(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::DrawWorldsides(m3d::ui::DrawInfo const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::GameDataUpdate(void*, int)
{
    // TODO: dispatch on dataType 28/29/85/89 once OnAddNavPoint/OnDeleteNavPoint/
    // OnStartLevel/UpdateOnNewFrame are ported.
    return 0;
}

int RadarWnd::UpdateDistance(NavPoint::NavPointType)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::OnStartLevel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::OnDeleteNavPoint(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::GameDataLoad(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::RemoveTurret(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::RemoveVehicle(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::AddTurret(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::AddVehicle(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::AddNavPoint(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::RemoveDistance(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::UpdateNavPoint(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

float RadarWnd::CalculateDistanceToNavPoint(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::GameDataSave(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::AddDistance(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::OnPaintOverChildren(m3d::ui::DrawInfo const&)
{
    // TODO: full radar composite draw (camera sight, worldsides, player vehicle,
    // turrets, vehicles, nav points, highlight).
}

int RadarWnd::RemoveWorldside(Worldside)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::OnAddNavPoint(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::UpdateDistancesOnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::UpdateOnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::UpdateTurretsOnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::GameDataSetup()
{
    // TODO: implement RadarWnd::GameDataSetup
    return 1;
}

CVector RadarWnd::GetWorldsideCoords(Worldside) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::UpdateTurret(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::UpdateVehicle(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::DrawTurrets(m3d::ui::DrawInfo const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::DrawVehicles(m3d::ui::DrawInfo const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::UpdateVehiclesOnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}
