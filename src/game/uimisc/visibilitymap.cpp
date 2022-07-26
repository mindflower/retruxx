#include "visibilitymap.h"

CStr const& VisibilityMap::GetLevelName() const
{
	throw std::logic_error("Not implemented");
}

int VisibilityMap::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
	throw std::logic_error("Not implemented");
}

int VisibilityMap::ShowArea(BoundsBase<float> const&)
{
	throw std::logic_error("Not implemented");
}

int VisibilityMap::ShowArea(CVector const&, float)
{
	throw std::logic_error("Not implemented");
}

int VisibilityMap::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
	throw std::logic_error("Not implemented");
}

int VisibilityMap::SetUpForLevel(CStr const&)
{
	throw std::logic_error("Not implemented");
}

void VisibilityMap::UpdateGrid()
{
	throw std::logic_error("Not implemented");
}

VisibilityMap::VisibilityMap()
{
	throw std::logic_error("Not implemented");
}

VisibilityMap::~VisibilityMap()
{
	throw std::logic_error("Not implemented");
}

m3d::rend::TexHandle VisibilityMap::GetBg() const
{
	throw std::logic_error("Not implemented");
}

bool VisibilityMap::IsWorldPositionVisible(CVector const&) const
{
	throw std::logic_error("Not implemented");
}

void VisibilityMap::UpdateBg()
{
	throw std::logic_error("Not implemented");
}

BoundsBase<int> VisibilityMap::GetCellBounds(int) const
{
	throw std::logic_error("Not implemented");
}

int VisibilityMap::CreateEmptyBg()
{
	throw std::logic_error("Not implemented");
}

PointBase<int> VisibilityMap::GetCellCoordByWorldPos(CVector const&) const
{
	throw std::logic_error("Not implemented");
}

CVector VisibilityMap::GetWorldPosByCellId(int) const
{
	throw std::logic_error("Not implemented");
}

int VisibilityMap::GetCellIdByWorldPos(CVector const&) const
{
	throw std::logic_error("Not implemented");
}

void VisibilityMap::ClearStatic()
{
	throw std::logic_error("Not implemented");
}

int VisibilityMap::GameUnitToVisibilityPixel(int) const
{
	throw std::logic_error("Not implemented");
}

int VisibilityMap::SetCellValue(int, CellValue)
{
	throw std::logic_error("Not implemented");
}

int VisibilityMap::InitStructures()
{
	throw std::logic_error("Not implemented");
}

int VisibilityMap::IntVector2CellsVector(std::vector<int> const&, std::vector<CellValue>&) const
{
	throw std::logic_error("Not implemented");
}

int VisibilityMap::MarkCell(int)
{
	throw std::logic_error("Not implemented");
}

int VisibilityMap::DrawCircle(int)
{
	throw std::logic_error("Not implemented");
}

int VisibilityMap::ClearBg()
{
	throw std::logic_error("Not implemented");
}

int VisibilityMap::InitStatic()
{
	throw std::logic_error("Not implemented");
}

VisibilityMap::CellValue VisibilityMap::GetCellValue(int) const
{
	throw std::logic_error("Not implemented");
}

void VisibilityMap::Clear()
{
	throw std::logic_error("Not implemented");
}

int VisibilityMap::CreateCircle()
{
	throw std::logic_error("Not implemented");
}

bool VisibilityMap::IsValid() const
{
	throw std::logic_error("Not implemented");
}
