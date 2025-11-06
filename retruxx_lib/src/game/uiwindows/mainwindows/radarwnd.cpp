#include "radarwnd.h"

#include "config.h"
#include "core/kernel.h"
#include "core/console/console.h"

RT_CLASS_EXPORT_METHOD_DEFINE(RadarWnd, AllowNavPoints)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(RadarWnd, AllowVehicles)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(RadarWnd, AllowTurrets)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(RadarWnd, AllowDistances)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(RadarWnd, SetScanRadius)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(RadarWnd, SetTest)
{
    RETRUXX_NOT_IMPLEMENTED;
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

RadarWnd::NpDistance::~NpDistance()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::NpDistance::GetNavPointId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

RadarWnd::NpDistance::NpDistance(ElectronicDigitalWnd*, NavPoint::NavPointType)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::NpDistance::SetNavPointId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::NpDistance::SetDistance(float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

NavPoint::NavPointType RadarWnd::NpDistance::GetNavPointType() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::NpDistance::Show(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool RadarWnd::NpDistance::IsValid() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

PointBase<float> const& RadarWnd::RadarItem::GetCoords() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

RadarWnd::RadarItem::RadarItem(m3d::rend::TexHandle, PointBase<float> const&, PointBase<float> const&, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

PointBase<float> const& RadarWnd::RadarItem::GetSize() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::RadarItem::Draw(m3d::ui::DrawInfo const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::RadarItem::SetCoords(PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::RadarItem::SetSize(PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::RadarItem::SetAngle(float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::RadarItem::SetTexture(m3d::rend::TexHandle)
{
    RETRUXX_NOT_IMPLEMENTED;
}

float RadarWnd::RadarItem::GetAngle() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

RadarWnd::RadarItem::~RadarItem()
{
    RETRUXX_NOT_IMPLEMENTED;
}

BoundsBase<float> RadarWnd::RadarItem::GetBounds() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::rend::TexHandle RadarWnd::RadarItem::GetTexture() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

RadarWnd::AuxInfo::AuxInfo()
{
    // TODO: generated code
    // Initialize radar icon names
    const char* iconNames[] = {
        "RadarVehicleOwnSmall", "RadarVehicleOwnLarge",
        "RadarVehicleEnemySmall", "RadarVehicleEnemyLarge",
        "RadarVehicleNeutralSmall", "RadarVehicleNeutralLarge",
        "RadarVehicleAllySmall", "RadarVehicleAllyLarge",
        "RadarTurretOwnSmall", "RadarTurretOwnLarge",
        "RadarTurretEnemySmall", "RadarTurretEnemyLarge",
        "RadarTurretNeutralSmall", "RadarTurretNeutralLarge",
        "RadarTurretAllySmall", "RadarTurretAllyLarge",
        "RadarQuestMainFar", "RadarQuestMainNear",
        "RadarQuestUserFar", "RadarQuestUserNear",
        "RadarNavPointFar", "RadarNavPointNear",
        "RadarWorldsideNord", "RadarWorldsideSouth"
    };

    for (int i = 0; i < 24; ++i) {
        m_icoNames[i] = iconNames[i];
    }

    // Initialize icon sizes
    m_icoSizeVehicleSmall = { 12.0f, 12.0f };
    m_icoSizeVehicleLarge = { 16.0f, 16.0f };
    m_icoSizeTurretSmall = { 14.0f, 14.0f };
    m_icoSizeTurretLarge = { 14.0f, 14.0f };
    m_icoSizeMainQuestFar = { 16.0f, 16.0f };
    m_icoSizeMainQuestNear = { 12.0f, 12.0f };
    m_icoSizeUserQuestFar = { 16.0f, 16.0f };
    m_icoSizeUserQuestNear = { 12.0f, 12.0f };
    m_icoSizeUserLocationFar = { 16.0f, 16.0f };
    m_icoSizeUserLocationNear = { 12.0f, 12.0f };
    m_icoSizeWorldside = { 8.0f, 16.0f };

    // Initialize radar geometry
    m_rotationCenter = { 65.0f, 65.0f };
    m_innerRadius = 49.0f;
    m_outerRadius = 49.0f;

    // Initialize texture names
    m_cameraSightTexName =("CameraSight");
    m_cameraSightSz = { 128.0f, 64.0f };

    m_playerVehicleTexName = ("RadarPlayerVehicle");
    m_playerVehicleSz = { 16.0f, 16.0f };

    m_highlightTexName = ("RadarHighlight");
    m_highlightB = { 5.0f, 5.0f, 123.0f, 123.0f };

    // Initialize window distance names
    const char* distanceNames[] = {
        "wndDistance_MainQuest",
        "wndDistance_UserQuest",
        "wndDistance_UserLocation"
    };

    for (int i = 0; i < 3; ++i) {
        m_wndsDistancesNames[i] = (distanceNames[i]);
    }
}

int RadarWnd::UpdateWorldsidesOnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::ClearTurretItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

float RadarWnd::GetIcoRotationAngle(PointBase<float> const&) const
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

void RadarWnd::DrawBg(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::ClearWorldsideItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::DrawNavPoints(m3d::ui::DrawInfo const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::DrawPlayerVehicle(m3d::ui::DrawInfo const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::AllowVehicles(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::CreateWorldSides()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::SetTest(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::RemoveNavPoint(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CVector RadarWnd::FixCoord(CVector const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::DrawWorldsides(m3d::ui::DrawInfo const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::UpdateDistance(NavPoint::NavPointType)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::ClearNavPointItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

PointBase<float> RadarWnd::WorldToRadarCoords(CVector const&, ItemType) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::GetIcoForVehicle(ai::Vehicle const*, m3d::rend::TexHandle&, PointBase<float>&) const
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

void RadarWnd::AllowDistances(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::GetIcoForNavPoint(NavPoint::NavPointType, CVector const&, m3d::rend::TexHandle&, PointBase<float>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::AddTurret(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* RadarWnd::GetClass() const
{
    return RT_CLASS_LOCAL(RadarWnd);
}

int RadarWnd::AddVehicle(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::AddNavPoint(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::AllowTurrets(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::CreateNavPoints()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::RemoveDistance(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::AllowNavPoints(bool)
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

m3d::rend::TexHandle RadarWnd::GetIcoForWorldside(Worldside) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* RadarWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::GameDataSave(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::Reset()
{
    RETRUXX_NOT_IMPLEMENTED;
}

RadarWnd::~RadarWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::AddDistance(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

float RadarWnd::GetDefaultScanRadius() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool RadarWnd::IsPositionOutsideScanRadius(CVector const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::OnPaintOverChildren(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
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

void RadarWnd::GetIcoForObject(ai::Obj const*, m3d::rend::TexHandle&, PointBase<float>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::GetIcoForTurret(ai::StaticAutoGun const*, m3d::rend::TexHandle&, PointBase<float>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::UpdateOnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* RadarWnd::CreateObject()
{
    return new RadarWnd;
}

int RadarWnd::UpdateTurretsOnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::ClearVehicleItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::SetScanRadius(float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RadarWnd::GameDataSetup()
{
    // TODO: implement RadarWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
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

void RadarWnd::DrawHighlight(m3d::ui::DrawInfo const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RadarWnd::DrawTurrets(m3d::ui::DrawInfo const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

float RadarWnd::GetScanRadius() const
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

m3d::Class* RadarWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

RadarWnd::RadarWnd()
    : m_cvDefaultRadarScanRadius("defaultRadarScanRadius", "500", m3d::CVar::CVAR_FLOAT, m3d::CVar::CVAR_ARCHIVE)
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

RadarWnd::RadarWnd(RadarWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
