#include "visibilitymap.h"

#include <cmath>

#include "levelinfo.h"
#include "game/m3dgame.h"
#include "game/uimisc/guihelper.h"
#include "core/ini.h"
#include "core/log.h"
#include "server/utils.h"
#include "server/objects/player.h"
#include "server/objects/vehicle.h"

VisibilityMap::AuxInfo::AuxInfo() : m_circleTexName("VisibilityCircle")
{
}

CStr const& VisibilityMap::GetLevelName() const
{
    return m_levelName;
}

int VisibilityMap::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode)
{
    Clear();

    if (xmlFile == nullptr || xmlNode == nullptr)
    {
        M3D_LOG_INFO("VisibilityMap::LoadFromXml error - invalid params");
        return 0;
    }

    m3d::SafeStrAttrib(m_levelName, xmlNode, "LevelName");
    if (m_levelName.empty())
    {
        M3D_LOG_INFO("VisibilityMap::LoadFromXml error - empty level name");
        return 0;
    }

    CVector2 gridSize{0.0f, 0.0f};
    m3d::SafeVector2Attrib(gridSize, xmlNode, "GridSize");
    m_gridSize.x = static_cast<int>(gridSize.x);
    m_gridSize.y = static_cast<int>(gridSize.y);

    auto levelInfoManager = M3D_APP->m_pInterfaceManager->GetLevelInfoManager();
    int const levelSizeInCells = static_cast<int>(levelInfoManager->GetLevelSize(m_levelName) * 0.0078125);
    if (m_gridSize.x != levelSizeInCells || m_gridSize.y != levelSizeInCells)
    {
        M3D_LOG_INFO(
            "VisibilityMap::LoadFromXml error - grid size does not match real level size (level " + m_levelName + ")");
        return 0;
    }

    CStr strGrid;
    m3d::SafeStrAttrib(strGrid, xmlNode, "Grid");
    retruxx::vector<int> intVectorGrid;
    ai::StrToIntVector(strGrid, intVectorGrid);

    if (!IntVector2CellsVector(intVectorGrid, m_grid))
    {
        M3D_LOG_INFO("VisibilityMap::LoadFromXml error - invalid values in grid");
        return 0;
    }

    if (static_cast<int>(m_grid.size()) != m_gridSize.x * m_gridSize.y)
    {
        M3D_LOG_INFO("VisibilityMap::LoadFromXml error - grid size param does not match real grid size");
        return 0;
    }

    for (int i = 0; i < static_cast<int>(m_grid.size()); ++i)
    {
        if (m_grid[i] == CELL_MARKED)
        {
            m_newCellIds.insert(i);
        }
    }

    if (!InitStructures())
    {
        M3D_LOG_INFO("VisibilityMap::LoadFromXml error - cannot init structures");
        return 0;
    }

    return 1;
}

int VisibilityMap::ShowArea(BoundsBase<float> const& rect)
{
    if (!IsValid())
    {
        M3D_LOG_INFO("VisibilityMap::ShowArea error - visibility map is not valid");
        return 0;
    }

    float const levelSize = M3D_APP->m_pInterfaceManager->GetLevelInfoManager()->GetLevelSize(m_levelName);

    float x0;
    float y0;
    float width;
    float height;
    float right = rect.width + rect.x0;
    float bottom = rect.height + rect.y0;
    if (rect.x0 > levelSize || right < 0.0f || rect.y0 > levelSize || bottom < 0.0f)
    {
        x0 = 0.0f;
        y0 = 0.0f;
        width = 0.0f;
        height = 0.0f;
    }
    else
    {
        x0 = rect.x0 > 0.0f ? rect.x0 : 0.0f;
        if (right >= levelSize)
        {
            right = levelSize;
        }
        y0 = rect.y0 > 0.0f ? rect.y0 : 0.0f;
        if (bottom >= levelSize)
        {
            bottom = levelSize;
        }
        width = right - x0;
        height = bottom - y0;
    }

    if (width != 0.0f && height != 0.0f)
    {
        CVector corner;
        corner.x = x0;
        corner.y = 0.0f;
        corner.z = y0;
        PointBase<int> const tl = GetCellCoordByWorldPos(corner);
        corner.x = (width + x0) - 1.0f;
        corner.y = 0.0f;
        corner.z = (height + y0) - 1.0f;
        PointBase<int> const br = GetCellCoordByWorldPos(corner);

        for (int x = tl.x; x <= br.x; ++x)
        {
            for (int y = tl.y; y <= br.y; ++y)
            {
                if (x != -1 && y != -1)
                {
                    MarkCell(x + y * m_gridSize.x);
                }
            }
        }
    }

    return 1;
}

int VisibilityMap::ShowArea(CVector const& origin, float radius)
{
    if (!IsValid())
    {
        M3D_LOG_INFO("VisibilityMap::ShowArea error - visibility map is not valid");
        return 0;
    }

    if (M3D_APP->m_pInterfaceManager->GetLevelInfoManager()->GetLevelSize(m_levelName) == 0.0f)
    {
        M3D_LOG_INFO("VisibilityMap::ShowArea error - cannot find level size for level " + m_levelName);
        return 0;
    }

    float const minX = origin.x - radius;
    float const minZ = origin.z - radius;
    float const width = (origin.x + radius) - minX;
    float const height = (origin.z + radius) - minZ;

    CVector corner;
    corner.x = minX;
    corner.y = 0.0f;
    corner.z = minZ;
    PointBase<int> const tl = GetCellCoordByWorldPos(corner);
    corner.x = (width + minX) - 1.0f;
    corner.y = 0.0f;
    corner.z = (height + minZ) - 1.0f;
    PointBase<int> const br = GetCellCoordByWorldPos(corner);

    for (int i = tl.x; i <= br.x; ++i)
    {
        for (int j = tl.y; j <= br.y; ++j)
        {
            if (i == -1 || j == -1)
            {
                continue;
            }
            int const cellId = i + j * m_gridSize.x;
            CVector const cellWorldPos = GetWorldPosByCellId(cellId);
            float const dx = cellWorldPos.x - origin.x;
            float const dy = origin.y - origin.y;  // faithful to original (always 0)
            float const dz = cellWorldPos.z - origin.z;
            if (radius >= std::sqrt(dz * dz + dy * dy + dx * dx))
            {
                MarkCell(cellId);
            }
        }
    }

    return 1;
}

int VisibilityMap::SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode)
{
    if (xmlFile == nullptr || xmlNode == nullptr)
    {
        M3D_LOG_INFO("VisibilityMap::SaveToXml error - invalid params");
        return 0;
    }

    if (!IsValid())
    {
        M3D_LOG_INFO("VisibilityMap::SaveToXml error - invalid visibility map");
        return 0;
    }

    xmlNode->SetAttribute("LevelName", m_levelName.c_str());

    CVector2 gridSize;
    gridSize.x = static_cast<float>(m_gridSize.x);
    gridSize.y = static_cast<float>(m_gridSize.y);
    xmlNode->SetAttribute("GridSize", CStr(gridSize).c_str());

    retruxx::vector<int> gridInts(m_grid.begin(), m_grid.end());
    xmlNode->SetAttribute("Grid", ai::IntVectorToStr(gridInts).c_str());

    return 1;
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
    m_grid.assign(levelSize * levelSize, CELL_EMPTY);
    return (CreateEmptyBg() & 1) & CreateCircle();
}

void VisibilityMap::UpdateGrid()
{
    if (!IsValid())
    {
        return;
    }
    if (m_levelName != help::GetCurrentLevelName())
    {
        return;
    }
    if (!ai::thePlayer)
    {
        return;
    }
    auto* vehicle = ai::thePlayer->GetVehicle();
    if (!vehicle)
    {
        return;
    }
    int const cellId = GetCellIdByWorldPos(vehicle->GetPosition());
    if (cellId != -1)
    {
        MarkCell(cellId);
    }
}

VisibilityMap::VisibilityMap()
{
    InitStatic();
}

VisibilityMap::~VisibilityMap()
{
    Clear();
    M3D_RENDERER->ReleaseTexture(m_circlePatternTex);
}

m3d::rend::TexHandle VisibilityMap::GetBg() const
{
    return m_bgTex;
}

bool VisibilityMap::IsWorldPositionVisible(CVector const& worldPos) const
{
    if (m_bgBits == nullptr)
    {
        return false;
    }
    int const px = static_cast<int>(static_cast<float>(static_cast<int>(worldPos.x)) * 0.0625f);
    int const pz = static_cast<int>(static_cast<float>(static_cast<int>(worldPos.z)) * 0.0625f);
    int const idx = px + pz * m_bgSize.x;
    if (idx < 0 || idx >= m_bgSize.x * m_bgSize.y)
    {
        return false;
    }
    return m_bgBits[4 * idx + 3] <= 0xC8;
}

void VisibilityMap::UpdateBg()
{
    if (!IsValid())
    {
        return;
    }
    for (auto it = m_newCellIds.begin(); it != m_newCellIds.end();)
    {
        DrawCircle(*it);
        it = m_newCellIds.erase(it);
    }
}

BoundsBase<int> VisibilityMap::GetCellBounds(int cellId) const
{
    BoundsBase<int> bounds;
    if (cellId < 0 || cellId >= static_cast<int>(m_grid.size()))
    {
        bounds.x0 = 0;
        bounds.y0 = 0;
        bounds.width = 0;
        bounds.height = 0;
        return bounds;
    }
    int const col = cellId % m_gridSize.y;
    int const row = cellId / m_gridSize.x;
    bounds.x0 = col * 8;
    bounds.y0 = row * 8;
    bounds.width = 8 * (col + 1) - bounds.x0;
    bounds.height = 8 * (row + 1) - bounds.y0;
    return bounds;
}

int VisibilityMap::CreateEmptyBg()
{
    M3D_APP->m_renderer->ReleaseTexture(m_bgTex);
    delete[] m_bgBits;
    m_bgSize = {0, 0};
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

PointBase<int> VisibilityMap::GetCellCoordByWorldPos(CVector const& worldPos) const
{
    PointBase<int> result;
    if (m_gridSize.x == 0 && m_gridSize.y == 0)
    {
        result.x = -1;
        result.y = -1;
        return result;
    }
    int x = static_cast<int>(worldPos.x * 0.0078125f);
    int y = static_cast<int>(worldPos.z * 0.0078125f);
    if (x < 0 || x >= m_gridSize.x)
    {
        x = -1;
    }
    if (y < 0 || y >= m_gridSize.y)
    {
        y = -1;
    }
    result.x = x;
    result.y = y;
    return result;
}

CVector VisibilityMap::GetWorldPosByCellId(int cellId) const
{
    CVector result;
    if (cellId < 0 || cellId >= static_cast<int>(m_grid.size()))
    {
        result.x = 0.0f;
        result.y = 0.0f;
        result.z = 0.0f;
        return result;
    }
    int const w = m_gridSize.x;
    result.x = static_cast<float>(cellId % w) * 128.0f + 64.0f;
    result.y = 0.0f;
    result.z = static_cast<float>(cellId / w) * 128.0f + 64.0f;
    return result;
}

int VisibilityMap::GetCellIdByWorldPos(CVector const& worldPos) const
{
    if (m_gridSize.x == 0 && m_gridSize.y == 0)
    {
        return -1;
    }
    int x = static_cast<int>(worldPos.x * 0.0078125f);
    int z = static_cast<int>(worldPos.z * 0.0078125f);
    if (x < 0 || x >= m_gridSize.x)
    {
        x = -1;
    }
    if (z < 0 || z >= m_gridSize.y)
    {
        z = -1;
    }
    if (x != -1 && z != -1)
    {
        return x + z * m_gridSize.x;
    }
    return -1;
}

void VisibilityMap::ClearStatic()
{
    M3D_APP->m_renderer->ReleaseTexture(m_circlePatternTex);
}

int VisibilityMap::GameUnitToVisibilityPixel(int valInGameUnits) const
{
    return static_cast<int>(static_cast<float>(valInGameUnits) * 0.0625f);
}

int VisibilityMap::SetCellValue(int cellId, CellValue value)
{
    if (cellId < 0 || cellId >= static_cast<int>(m_grid.size()))
    {
        return 0;
    }
    m_grid[cellId] = value;
    return 1;
}

int VisibilityMap::InitStructures()
{
    return (CreateEmptyBg() & 1) & CreateCircle();
}

int VisibilityMap::IntVector2CellsVector(std::vector<int> const& intVector, std::vector<CellValue>& cellsVector) const
{
    int result = 1;
    cellsVector.resize(intVector.size(), CELL_EMPTY);
    for (size_t i = 0; i < intVector.size(); ++i)
    {
        if (intVector[i] == CELL_EMPTY || intVector[i] == CELL_MARKED)
        {
            cellsVector[i] = static_cast<CellValue>(intVector[i]);
        }
        else
        {
            result = 0;
            cellsVector[i] = CELL_INVALID;
        }
    }
    return result;
}

int VisibilityMap::MarkCell(int cellId)
{
    if (cellId < 0 || cellId >= static_cast<int>(m_grid.size()))
    {
        return 0;
    }
    CellValue const value = GetCellValue(cellId);
    if (value == CELL_INVALID)
    {
        return 0;
    }
    if (value == CELL_EMPTY)
    {
        if (!SetCellValue(cellId, CELL_MARKED))
        {
            return 0;
        }
        m_newCellIds.insert(cellId);
    }
    return 1;
}

int VisibilityMap::DrawCircle(int cellId)
{
    if (!IsValid() || cellId < 0 || cellId >= static_cast<int>(m_grid.size()))
    {
        return 0;
    }

    BoundsBase<int> const cellB = GetCellBounds(cellId);
    if (cellB.width == 0 && cellB.height == 0)
    {
        return 0;
    }

    int const circleW = m_circleSize.x;
    int const circleH = m_circleSize.y;
    int const circlePixelCount = circleH * circleW;

    int const cellCenterY = (cellB.height + 2 * cellB.y0) / 2;
    int const cellCenterX = (cellB.width + 2 * cellB.x0) / 2;
    int const top = cellCenterY - circleH / 2;
    int const bottom = cellCenterY + circleH / 2;
    int const left = cellCenterX - circleW / 2;
    int const right = cellCenterX + circleW / 2;

    int const bgByteCount = 4 * m_bgSize.y * m_bgSize.x;

    int circleBitIndex = 0;
    for (int bgX = left; bgX < right; ++bgX)
    {
        for (int bgY = top; bgY < bottom; ++bgY, ++circleBitIndex)
        {
            if (bgX < 0 || bgX >= m_bgSize.x || bgY < 0 || bgY >= m_bgSize.y)
            {
                continue;
            }
            int const alphaIdx = 4 * (bgX + bgY * m_bgSize.x) + 3;
            if (alphaIdx < 0 || alphaIdx >= bgByteCount || circleBitIndex < 0 || circleBitIndex >= circlePixelCount)
            {
                continue;
            }
            int const diff = m_bgBits[alphaIdx] - m_circleBits[circleBitIndex];
            m_bgBits[alphaIdx] = static_cast<unsigned char>(diff >= 0 ? diff : 0);
        }
    }

    return M3D_APP->m_renderer->UploadTexImage(
        m_bgTex, m_bgSize.x, m_bgSize.y, m_bgBits, m3d::rend::TM_DTF_RGBA8888, 0);
}

int VisibilityMap::ClearBg()
{
    if (!m_bgTex.IsValid() || m_bgBits == nullptr)
    {
        return 0;
    }
    for (int i = 0; i < m_bgSize.y * m_bgSize.x; ++i)
    {
        m_bgBits[4 * i] = -8355712;
    }
    return M3D_APP->m_renderer->UploadTexImage(
        m_bgTex, m_bgSize.x, m_bgSize.y, m_bgBits, m3d::rend::TM_DTF_RGBA8888, 0);
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

VisibilityMap::CellValue VisibilityMap::GetCellValue(int cellId) const
{
    if (cellId < 0 || cellId >= static_cast<int>(m_grid.size()))
    {
        return CELL_INVALID;
    }
    return m_grid[cellId];
}

void VisibilityMap::Clear()
{
    m_grid.clear();
    m_newCellIds.clear();
    M3D_APP->m_renderer->ReleaseTexture(m_bgTex);
    delete[] m_bgBits;
    delete[] m_circleBits;
    m_bgBits = nullptr;
    m_circleBits = nullptr;
    m_levelName = {};
    m_gridSize = {0, 0};
    m_bgSize = {0, 0};
    m_circleSize = {0, 0};
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
    return !m_levelName.empty() && !m_grid.empty() && m_bgTex.IsValid() && m_bgBits != nullptr &&
        m_circleBits != nullptr;
}
