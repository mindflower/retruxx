#include "radarwnd.h"

RT_CLASS_EXPORT_METHOD_DEFINE(RadarWnd, AllowNavPoints)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(RadarWnd, AllowVehicles)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(RadarWnd, AllowTurrets)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(RadarWnd, AllowDistances)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(RadarWnd, SetScanRadius)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(RadarWnd, SetTest)
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
}

int RadarWnd::NpDistance::GetNavPointId() const
{
    throw std::logic_error("Not implemented");
}

RadarWnd::NpDistance::NpDistance(ElectronicDigitalWnd*, NavPoint::NavPointType)
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::NpDistance::SetNavPointId(int)
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::NpDistance::SetDistance(float)
{
    throw std::logic_error("Not implemented");
}

NavPoint::NavPointType RadarWnd::NpDistance::GetNavPointType() const
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::NpDistance::Show(bool)
{
    throw std::logic_error("Not implemented");
}

bool RadarWnd::NpDistance::IsValid() const
{
    throw std::logic_error("Not implemented");
}

PointBase<float> const& RadarWnd::RadarItem::GetCoords() const
{
    throw std::logic_error("Not implemented");
}

RadarWnd::RadarItem::RadarItem(m3d::rend::TexHandle, PointBase<float> const&, PointBase<float> const&, float)
{
    throw std::logic_error("Not implemented");
}

PointBase<float> const& RadarWnd::RadarItem::GetSize() const
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::RadarItem::Draw(m3d::ui::DrawInfo const&) const
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::RadarItem::SetCoords(PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::RadarItem::SetSize(PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::RadarItem::SetAngle(float)
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::RadarItem::SetTexture(m3d::rend::TexHandle)
{
    throw std::logic_error("Not implemented");
}

float RadarWnd::RadarItem::GetAngle() const
{
    throw std::logic_error("Not implemented");
}

RadarWnd::RadarItem::~RadarItem()
{
    throw std::logic_error("Not implemented");
}

BoundsBase<float> RadarWnd::RadarItem::GetBounds() const
{
    throw std::logic_error("Not implemented");
}

m3d::rend::TexHandle RadarWnd::RadarItem::GetTexture() const
{
    throw std::logic_error("Not implemented");
}

RadarWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::UpdateWorldsidesOnNewFrame()
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::ClearTurretItems()
{
    throw std::logic_error("Not implemented");
}

float RadarWnd::GetIcoRotationAngle(PointBase<float> const&) const
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::UpdateNavPointsOnNewFrame()
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::UpdateWorldside(Worldside)
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::DrawCameraSight(m3d::ui::DrawInfo const&) const
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::AddWorldside(Worldside)
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::DrawBg(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::ClearWorldsideItems()
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::DrawNavPoints(m3d::ui::DrawInfo const&) const
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::DrawPlayerVehicle(m3d::ui::DrawInfo const&) const
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::AllowVehicles(bool)
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::CreateWorldSides()
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::SetTest(int)
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::RemoveNavPoint(int)
{
    throw std::logic_error("Not implemented");
}

CVector RadarWnd::FixCoord(CVector const&) const
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::DrawWorldsides(m3d::ui::DrawInfo const&) const
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::UpdateDistance(NavPoint::NavPointType)
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::ClearNavPointItems()
{
    throw std::logic_error("Not implemented");
}

PointBase<float> RadarWnd::WorldToRadarCoords(CVector const&, ItemType) const
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::GetIcoForVehicle(ai::Vehicle const*, m3d::rend::TexHandle&, PointBase<float>&) const
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::OnStartLevel()
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::OnDeleteNavPoint(void*)
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::GameDataLoad(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::RemoveTurret(int)
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::RemoveVehicle(int)
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::AllowDistances(bool)
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::GetIcoForNavPoint(NavPoint::NavPointType, CVector const&, m3d::rend::TexHandle&, PointBase<float>&) const
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::AddTurret(int)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* RadarWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::AddVehicle(int)
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::AddNavPoint(int)
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::AllowTurrets(bool)
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::CreateNavPoints()
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::RemoveDistance(int)
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::AllowNavPoints(bool)
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::UpdateNavPoint(int)
{
    throw std::logic_error("Not implemented");
}

float RadarWnd::CalculateDistanceToNavPoint(int) const
{
    throw std::logic_error("Not implemented");
}

m3d::rend::TexHandle RadarWnd::GetIcoForWorldside(Worldside) const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* RadarWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::GameDataSave(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::Reset()
{
    throw std::logic_error("Not implemented");
}

RadarWnd::~RadarWnd()
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::AddDistance(int)
{
    throw std::logic_error("Not implemented");
}

float RadarWnd::GetDefaultScanRadius() const
{
    throw std::logic_error("Not implemented");
}

bool RadarWnd::IsPositionOutsideScanRadius(CVector const&) const
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::OnPaintOverChildren(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::RemoveWorldside(Worldside)
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::OnAddNavPoint(void*)
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::UpdateDistancesOnNewFrame()
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::GetIcoForObject(ai::Obj const*, m3d::rend::TexHandle&, PointBase<float>&) const
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::GetIcoForTurret(ai::StaticAutoGun const*, m3d::rend::TexHandle&, PointBase<float>&) const
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::UpdateOnNewFrame()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* RadarWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::UpdateTurretsOnNewFrame()
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::ClearVehicleItems()
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::SetScanRadius(float)
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

CVector RadarWnd::GetWorldsideCoords(Worldside) const
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::UpdateTurret(int)
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::UpdateVehicle(int)
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::DrawHighlight(m3d::ui::DrawInfo const&) const
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::DrawTurrets(m3d::ui::DrawInfo const&) const
{
    throw std::logic_error("Not implemented");
}

float RadarWnd::GetScanRadius() const
{
    throw std::logic_error("Not implemented");
}

void RadarWnd::DrawVehicles(m3d::ui::DrawInfo const&) const
{
    throw std::logic_error("Not implemented");
}

int RadarWnd::UpdateVehiclesOnNewFrame()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* RadarWnd::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

RadarWnd::RadarWnd()
{
    throw std::logic_error("Not implemented");
}

RadarWnd::RadarWnd(RadarWnd const&)
{
    throw std::logic_error("Not implemented");
}
