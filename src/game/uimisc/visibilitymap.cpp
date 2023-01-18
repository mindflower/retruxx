#include "visibilitymap.h"

#include "levelinfo.h"
#include "game/m3dgame.h"

VisibilityMap::AuxInfo::AuxInfo() :
    m_circleTexName("VisibilityCircle")
{
}

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

int VisibilityMap::SetUpForLevel(CStr const& levelName)
{
	Clear();
	if (levelName.empty())
	{
		return 0;
	}
	m_levelName = levelName;
	auto levelInfoManager = M3D_APP->m_pInterfaceManager->GetLevelInfoManager();
	//TODO: check this!
	int const levelSize = levelInfoManager->GetLevelSize(m_levelName) * 0.0078125;
	if (levelSize == 0.0)
	{
		return 0;
	}
	m_gridSize.x = levelSize;
	m_gridSize.y = levelSize;
	m_grid.assign(levelSize* levelSize, CELL_EMPTY);
	return (CreateEmptyBg() & 1) & CreateCircle();
}

void VisibilityMap::UpdateGrid()
{
	throw std::logic_error("Not implemented");
}

VisibilityMap::VisibilityMap()
{
	InitStatic();
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
	M3D_APP->m_renderer->ReleaseTexture(m_bgTex);
	delete[] m_bgBits;
	m_bgSize = { 0, 0 };
	if (m_levelName.empty())
	{
		return 0;
	}
	auto levelInfoManager = M3D_APP->m_pInterfaceManager->GetLevelInfoManager();
	int const size = levelInfoManager->GetLevelSize(m_levelName) * 0.0625;
	if (size == 0.0)
	{
		return 0;
	}
	m_bgBits = new unsigned char[4 * size * size];
	m_bgSize.x = size;
	m_bgSize.y = size;
	m_bgTex = M3D_APP->m_renderer->AddDynamicTexture("$VisMapTex", size, size, 1);
	if (!m_bgTex.IsValid())
	{
		return 0;
	}
	M3D_APP->m_renderer->SetTextureParameter(m_bgTex, m3d::rend::TM_WRAP_S, 3);
	M3D_APP->m_renderer->SetTextureParameter(m_bgTex, m3d::rend::TM_WRAP_T, 3);
	return ClearBg();
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
	if (!m_bgTex.IsValid() || m_bgBits == nullptr)
	{
		return 0;
	}
	for (int i = 0; i < m_bgSize.y * m_bgSize.x; ++i)
	{
	    m_bgBits[4*i] = -8355712;
	}
	return M3D_APP->m_renderer->UploadTexImage(m_bgTex, m_bgSize.x, m_bgSize.y, m_bgBits, m3d::rend::TM_DTF_RGBA8888, 0);
}

int VisibilityMap::InitStatic()
{
	if (!m_circlePatternTex.IsValid())
	{
		m_circlePatternTex = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_circleTexName, 0);
	}
	M3D_APP->m_renderer->ReferenceTexture(m_circlePatternTex);
	return m_circlePatternTex.IsValid();
}

VisibilityMap::CellValue VisibilityMap::GetCellValue(int) const
{
	throw std::logic_error("Not implemented");
}

void VisibilityMap::Clear()
{
	m_grid.clear();
	m_newCellIds.clear();
	M3D_APP->m_renderer->ReleaseTexture(m_bgTex);
	delete[] m_bgBits;
	delete[] m_circleBits;
	m_levelName = {};
	m_gridSize = { 0, 0 };
	m_bgSize = { 0, 0 };
	m_circleSize = { 0, 0 };
}

int VisibilityMap::CreateCircle()
{
	delete[] m_circleBits;
	int const size = 2 * M3D_APP->m_pInterfaceManager->GetLevelInfoManager()->GetVisibilityRadius() * 0.0625;
	m_circleSize = {size, size};
	m_circleBits = new unsigned char[size * size];
	auto data = new unsigned[4 * size * size];
	//TODO: check this
	if (M3D_APP->m_renderer->DownloadTexImageRgba8888(data, m_circlePatternTex))
	{
	    for (int i = 0; i < size * size; ++i)
	    {
			m_circleBits[i] = data[i];
	    }
		delete[] data;
		return 1;
	}
	delete[] data;
	return 0;
}

bool VisibilityMap::IsValid() const
{
	throw std::logic_error("Not implemented");
}
