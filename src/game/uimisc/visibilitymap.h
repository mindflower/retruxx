#pragma once
#include "core/stringm3d.h"
#include "math/point2d.h"
#include "renderer/i_renderer.h"
#include "math/bounds2d.h"
#include <vector>
#include <set>

namespace m3d
{
	namespace cmn
	{
        class XmlFile;
        class XmlNode;
	}
}

class VisibilityMap
{
public:
    enum CellValue
    {
        CELL_EMPTY = 0xFFFFFFFF,
        CELL_SNAPPED = 0xFFFFFFFE,
        CELL_INVALID = 0xFFFFFFFD,
    };

public:
    CStr const& GetLevelName() const;
    int LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*);
    int ShowArea(BoundsBase<float> const&);
    int ShowArea(CVector const&, float);
    int SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*);
    int SetUpForLevel(CStr const&);
    void UpdateGrid();
    VisibilityMap();
    ~VisibilityMap();
    m3d::rend::TexHandle GetBg() const;
    bool IsWorldPositionVisible(CVector const&) const;
    void UpdateBg();


private:
    BoundsBase<int> GetCellBounds(int) const;
    int CreateEmptyBg();
    PointBase<int> GetCellCoordByWorldPos(CVector const&) const;
    CVector GetWorldPosByCellId(int) const;
    int GetCellIdByWorldPos(CVector const&) const;
    static void __fastcall ClearStatic();
    int GameUnitToVisibilityPixel(int) const;
    int SetCellValue(int, CellValue);
    int InitStructures();
    int IntVector2CellsVector(std::vector<int> const&, class std::vector<CellValue>&) const;
    int MarkCell(int);
    int DrawCircle(int);
    int ClearBg();
    static int __fastcall InitStatic();
    CellValue GetCellValue(int) const;
    void Clear();
    int CreateCircle();
    bool IsValid() const;

private:
    std::vector<CellValue> m_grid;
    std::set<int> m_newCellIds;
    m3d::rend::TexHandle m_bgTex;
    unsigned __int8* m_bgBits;
    unsigned __int8* m_circleBits;
    CStr m_levelName;
    PointBase<int> m_bgSize;
    PointBase<int> m_circleSize;
    PointBase<int> m_gridSize;
};
