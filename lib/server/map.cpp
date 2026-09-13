#include "map.h"

#include <cstdlib>
#include <stdexcept>

#include "core/log.h"
#include "core/scoped_ptr.h"
#include "file/fileserver.h"
#include "file/filestream.h"

namespace ai
{
    namespace
    {
        // Map::Circle and friends step around the ring in 45 degree increments.
        // The shipped build uses this slightly-off value for sin(45), not
        // 0.70710678 - keep it so the rasterised rings match cell for cell.
        constexpr float DIAG_45 = 0.70710856f;

        // Out-of-bounds cells read as 0xFF, so they block for every threshold
        // except 255 itself.
        constexpr unsigned char OUT_OF_BOUNDS_VALUE = 0xFF;
    }  // namespace

    GMAPOFFSET::GMAPOFFSET(int Px, int Py)
    {
        this->x = Px;
        this->y = Py;
    }

    void DirSet::AddItem(bool IsDiag, int x, int y)
    {
        // RVA 0x7D9E30. One call fills the four rotations of a single offset:
        // the axis-aligned set goes to m_Dir[0..3], the diagonal set to
        // m_Dir[4..7], in the order 0,2,1,3 and 4,6,5,7 respectively.
        if (IsDiag)
        {
            m_Dir[4].m_Off.emplace_back(x, y);
            m_Dir[6].m_Off.emplace_back(-x, -y);
            m_Dir[5].m_Off.emplace_back(-x, y);
            m_Dir[7].m_Off.emplace_back(x, -y);
        }
        else
        {
            m_Dir[0].m_Off.emplace_back(x, y);
            m_Dir[2].m_Off.emplace_back(-x, y);
            m_Dir[1].m_Off.emplace_back(y, x);
            m_Dir[3].m_Off.emplace_back(y, -x);
        }
    }

    MapIndex::MapIndex(int xx, int yy) : x(xx), y(yy)
    {
    }

    // ---------------------------------------------------------------- helpers

    // Every accessor below repeats this bounds test inline in the binary; the
    // row stride is always m_lastIndex.x and the index is y * stride + x.
    bool Map::InBounds(int xIndex, int yIndex) const
    {
        return xIndex >= 0 && xIndex < m_lastIndex.x && yIndex >= 0 && yIndex < m_lastIndex.y;
    }

    unsigned char* Map::CellAt(int xIndex, int yIndex)
    {
        return InBounds(xIndex, yIndex) ? &pField[yIndex * m_lastIndex.x + xIndex] : nullptr;
    }

    unsigned char Map::CellValue(int xIndex, int yIndex) const
    {
        if (!InBounds(xIndex, yIndex))
        {
            return OUT_OF_BOUNDS_VALUE;
        }
        return pField[yIndex * m_lastIndex.x + xIndex];
    }

    // ------------------------------------------------------------- allocation

    void Map::Create(float cellSizeX, float cellSizeY, float sizeX, float sizeY, Map* pParentMap)
    {
        // RVA 0x7D78E0
        this->m_size.x = sizeX;
        this->m_lastIndex.x = static_cast<int>(sizeX / cellSizeX);
        this->m_size.y = sizeY;
        this->m_cellSize.x = cellSizeX;
        this->m_cellSize.y = cellSizeY;
        this->m_pParentMap = pParentMap;
        this->m_lastIndex.y = static_cast<int>(sizeY / cellSizeY);
        if (pField)
            delete[] pField;
        this->pField = 0;
        auto size = this->m_lastIndex.y * this->m_lastIndex.x;
        this->pField = new unsigned char[size];
        memset(pField, 0, size);
    }

    void Map::Clear()
    {
        // RVA 0x7D7980
        if (pField)
        {
            memset(pField, 0, this->m_lastIndex.x * this->m_lastIndex.y);
        }
        else
        {
            M3D_LOG_ERR("Error: map field was not created");
        }
    }

    void Map::Fill(unsigned char value)
    {
        // RVA 0x7D6CD0 - a plain memset of the whole field, no bounds guard.
        memset(pField, value, m_lastIndex.x * m_lastIndex.y);
    }

    void Map::LoadFromRawFile(char const* fileName)
    {
        // RVA 0x7D7A10
        scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
        if (stream->Open(fileName, m3d::fs::IStream::OPEN_READ))
        {
            stream->ReadBytes(pField, this->m_lastIndex.x * this->m_lastIndex.y);
            stream->Close();
        }
    }

    void Map::SetGlobalMap(Map* pGlobalMap)
    {
        // RVA 0x7D74F0
        theGlobalMap = pGlobalMap;
    }

    // ---------------------------------------------------------- index helpers

    void Map::GetIndex(CVector2 const& pos, MapIndex& index)
    {
        // RVA 0x7BC7D0
        index.x = static_cast<int>(pos.x / m_cellSize.x);
        index.y = static_cast<int>(pos.y / m_cellSize.y);
    }

    void Map::GetCellCenter(MapIndex const& index, CVector2& center)
    {
        // RVA 0x7D6FB0
        center.x = (static_cast<float>(index.x) + 0.5f) * m_cellSize.x;
        center.y = (static_cast<float>(index.y) + 0.5f) * m_cellSize.y;
    }

    void Map::GetCellCenter(CVector2 const& pos, CVector2& center)
    {
        // RVA 0x7D6FF0
        center.x = (static_cast<float>(static_cast<int>(pos.x / m_cellSize.x)) + 0.5f) * m_cellSize.x;
        center.y = (static_cast<float>(static_cast<int>(pos.y / m_cellSize.y)) + 0.5f) * m_cellSize.y;
    }

    CVector2 const& Map::GetCellSize() const
    {
        // RVA 0x601150
        return m_cellSize;
    }

    MapIndex const& Map::GetLastIndex() const
    {
        // RVA 0x601140
        return m_lastIndex;
    }

    // The radius arguments of the CVector2 overloads are converted with the
    // SMALLER of the two cell dimensions, so the cell count never understates
    // the world-space radius on a non-square grid.
    int Map::RadiusToCells(float radius) const
    {
        float const cell = (m_cellSize.x <= m_cellSize.y) ? m_cellSize.x : m_cellSize.y;
        return static_cast<int>(radius / cell);
    }

    // -------------------------------------------------------------- per-cell

    void Map::SetValue(int xIndex, int yIndex, unsigned char value)
    {
        // RVA 0x7D6D10
        if (auto* cell = CellAt(xIndex, yIndex))
        {
            *cell = value;
        }
    }

    void Map::IncValue(int xIndex, int yIndex, unsigned char value)
    {
        // RVA 0x7D6D40 - saturating add.
        if (auto* cell = CellAt(xIndex, yIndex))
        {
            if (*cell > 255 - value)
                *cell = 0xFF;
            else
                *cell += value;
        }
    }

    void Map::DecValue(int xIndex, int yIndex, unsigned char value)
    {
        // RVA 0x7D6DA0 - saturating subtract.
        if (auto* cell = CellAt(xIndex, yIndex))
        {
            if (*cell < value)
                *cell = 0;
            else
                *cell -= value;
        }
    }

    void Map::SetValue(CVector2 const& point, unsigned char value)
    {
        // RVA 0x7D6DF0
        SetValue(static_cast<int>(point.x / m_cellSize.x), static_cast<int>(point.y / m_cellSize.y), value);
    }

    void Map::IncValue(CVector2 const& point, unsigned char value)
    {
        // RVA 0x7D6E40
        IncValue(static_cast<int>(point.x / m_cellSize.x), static_cast<int>(point.y / m_cellSize.y), value);
    }

    void Map::DecValue(CVector2 const& point, unsigned char value)
    {
        // RVA 0x7D6EC0
        DecValue(static_cast<int>(point.x / m_cellSize.x), static_cast<int>(point.y / m_cellSize.y), value);
    }

    unsigned char Map::GetValue(int xIndex, int yIndex)
    {
        // RVA 0x7D6F30
        return CellValue(xIndex, yIndex);
    }

    unsigned char Map::GetValue(float x, float y)
    {
        // RVA 0x7D6F60
        return CellValue(static_cast<int>(x / m_cellSize.x), static_cast<int>(y / m_cellSize.y));
    }

    // ------------------------------------------------------------------ lines

    void Map::MoveTo(CVector2 const& point)
    {
        // RVA 0x7D70D0
        m_CurPos.x = static_cast<int>(point.x / m_cellSize.x);
        m_CurPos.y = static_cast<int>(point.y / m_cellSize.y);
    }

    // Shared Bresenham walk for the LineTo / Line8 family. `plotFirst` selects
    // whether the starting cell is written: Line8 writes it, LineTo does not
    // (it is assumed to have been covered by the previous segment).
    template <typename Plot>
    void Map::Walk8(int x1, int y1, int x2, int y2, bool plotFirst, Plot plot)
    {
        int const dx = std::abs(x2 - x1);
        int const dy = std::abs(y2 - y1);
        int const sx = (x2 >= x1) ? 1 : -1;
        int const sy = (y2 >= y1) ? 1 : -1;

        int cx = x1;
        int cy = y1;

        if (dy > dx)
        {
            int d = 2 * dx - dy;
            int const dinc0 = 2 * dx;
            int const dinc1 = 2 * (dx - dy);
            if (plotFirst)
                plot(cx, cy);
            cy += sy;
            for (int k = dy; k >= 1; --k)
            {
                if (d <= 0)
                    d += dinc0;
                else
                {
                    d += dinc1;
                    cx += sx;
                }
                plot(cx, cy);
                cy += sy;
            }
        }
        else
        {
            int d = 2 * dy - dx;
            int const dinc0 = 2 * dy;
            int const dinc1 = 2 * (dy - dx);
            if (plotFirst)
                plot(cx, cy);
            cx += sx;
            for (int k = dx; k >= 1; --k)
            {
                if (d <= 0)
                    d += dinc0;
                else
                {
                    d += dinc1;
                    cy += sy;
                }
                plot(cx, cy);
                cx += sx;
            }
        }
    }

    // Shared 4-connected DDA for the Line4 family. Takes dx + dy steps, moving
    // along exactly one axis each time, and always writes the starting cell.
    template <typename Plot>
    void Map::Walk4(int x1, int y1, int x2, int y2, Plot plot)
    {
        int const dx = std::abs(x2 - x1);
        int const dy = std::abs(y2 - y1);
        int const n = dx + dy;
        int const sx = (x2 >= x1) ? 1 : -1;
        int const sy = (y2 >= y1) ? 1 : -1;

        plot(x1, y1);

        int cx = x1;
        int cy = y1;
        int accX = 0;  // grows by n on every x step
        int accY = 0;  // grows by n on every y step
        int tx = dx;   // dx * step
        int ty = dy;   // dy * step

        for (int i = 1; i <= n; ++i)
        {
            int px = cx;
            int py = cy;
            if ((accX - tx) >= (accY - ty))
            {
                accY += n;
                py = cy + sy;
                cy += sy;
            }
            else
            {
                accX += n;
                px = cx + sx;
                cx += sx;
            }
            plot(px, py);
            tx += dx;
            ty += dy;
        }
    }

    void Map::LineTo(CVector2 const& point, unsigned char value)
    {
        // RVA 0x7D7100. Does NOT write the starting cell, and leaves m_CurPos
        // at the destination so segments can be chained.
        MapIndex const end(static_cast<int>(point.x / m_cellSize.x), static_cast<int>(point.y / m_cellSize.y));
        Walk8(m_CurPos.x, m_CurPos.y, end.x, end.y, false, [this, value](int x, int y) { SetValue(x, y, value); });
        m_CurPos = end;
    }

    void Map::IncLineTo(CVector2 const& point, unsigned char value)
    {
        // RVA 0x7D7250
        MapIndex const end(static_cast<int>(point.x / m_cellSize.x), static_cast<int>(point.y / m_cellSize.y));
        Walk8(m_CurPos.x, m_CurPos.y, end.x, end.y, false, [this, value](int x, int y) { IncValue(x, y, value); });
        m_CurPos = end;
    }

    void Map::DecLineTo(CVector2 const& point, unsigned char value)
    {
        // RVA 0x7D7370. NOTE - original asymmetry, kept verbatim: the steep
        // branch decrements the starting cell before walking (0x7D73CE) while
        // the shallow branch does not, so a steep segment touches its origin
        // one extra time. LineTo and IncLineTo never touch it in either branch.
        MapIndex const end(static_cast<int>(point.x / m_cellSize.x), static_cast<int>(point.y / m_cellSize.y));
        bool const steep = std::abs(end.y - m_CurPos.y) > std::abs(end.x - m_CurPos.x);
        if (steep)
        {
            DecValue(m_CurPos.x, m_CurPos.y, value);
        }
        Walk8(m_CurPos.x, m_CurPos.y, end.x, end.y, false, [this, value](int x, int y) { DecValue(x, y, value); });
        m_CurPos = end;
    }

    void Map::Line8(CVector2 const& point1, CVector2 const& point2, unsigned char value)
    {
        // RVA 0x7D7DE0. The Line/Rectangle family scales by 1/cellSize rather
        // than dividing, which rounds marginally differently from GetIndex.
        float const invX = 1.0f / m_cellSize.x;
        float const invY = 1.0f / m_cellSize.y;
        Walk8(static_cast<int>(point1.x * invX),
              static_cast<int>(point1.y * invY),
              static_cast<int>(point2.x * invX),
              static_cast<int>(point2.y * invY),
              true,
              [this, value](int x, int y) { SetValue(x, y, value); });
    }

    void Map::IncLine8(CVector2 const& point1, CVector2 const& point2, unsigned char value)
    {
        // RVA 0x7D7F90
        float const invX = 1.0f / m_cellSize.x;
        float const invY = 1.0f / m_cellSize.y;
        Walk8(static_cast<int>(point1.x * invX),
              static_cast<int>(point1.y * invY),
              static_cast<int>(point2.x * invX),
              static_cast<int>(point2.y * invY),
              true,
              [this, value](int x, int y) { IncValue(x, y, value); });
    }

    void Map::DecLine8(CVector2 const& point1, CVector2 const& point2, unsigned char value)
    {
        // RVA 0x7D8100
        float const invX = 1.0f / m_cellSize.x;
        float const invY = 1.0f / m_cellSize.y;
        Walk8(static_cast<int>(point1.x * invX),
              static_cast<int>(point1.y * invY),
              static_cast<int>(point2.x * invX),
              static_cast<int>(point2.y * invY),
              true,
              [this, value](int x, int y) { DecValue(x, y, value); });
    }

    void Map::Line4(CVector2 const& point1, CVector2 const& point2, unsigned char value)
    {
        // RVA 0x7D8570
        float const invX = 1.0f / m_cellSize.x;
        float const invY = 1.0f / m_cellSize.y;
        Walk4(static_cast<int>(point1.x * invX),
              static_cast<int>(point1.y * invY),
              static_cast<int>(point2.x * invX),
              static_cast<int>(point2.y * invY),
              [this, value](int x, int y) { SetValue(x, y, value); });
    }

    void Map::IncLine4(CVector2 const& point1, CVector2 const& point2, unsigned char value)
    {
        // RVA 0x7D8700
        float const invX = 1.0f / m_cellSize.x;
        float const invY = 1.0f / m_cellSize.y;
        Walk4(static_cast<int>(point1.x * invX),
              static_cast<int>(point1.y * invY),
              static_cast<int>(point2.x * invX),
              static_cast<int>(point2.y * invY),
              [this, value](int x, int y) { IncValue(x, y, value); });
    }

    void Map::DecLine4(CVector2 const& point1, CVector2 const& point2, unsigned char value)
    {
        // RVA 0x7D88B0
        float const invX = 1.0f / m_cellSize.x;
        float const invY = 1.0f / m_cellSize.y;
        Walk4(static_cast<int>(point1.x * invX),
              static_cast<int>(point1.y * invY),
              static_cast<int>(point2.x * invX),
              static_cast<int>(point2.y * invY),
              [this, value](int x, int y) { DecValue(x, y, value); });
    }

    // ------------------------------------------------------------- rectangles

    // The three rectangle routines share this normalised cell range.
    void Map::RectRange(CVector2 const& point1, CVector2 const& point2, int& x0, int& y0, int& x1, int& y1) const
    {
        float const invX = 1.0f / m_cellSize.x;
        float const invY = 1.0f / m_cellSize.y;
        x0 = static_cast<int>(point1.x * invX);
        y0 = static_cast<int>(point1.y * invY);
        x1 = static_cast<int>(point2.x * invX);
        y1 = static_cast<int>(point2.y * invY);
        if (x0 > x1)
            std::swap(x0, x1);
        if (y0 > y1)
            std::swap(y0, y1);
    }

    void Map::Rectangle(CVector2 const& point1, CVector2 const& point2, unsigned char value)
    {
        // RVA 0x7D82D0
        int x0, y0, x1, y1;
        RectRange(point1, point2, x0, y0, x1, y1);
        for (int y = y0; y <= y1; ++y)
            for (int x = x0; x <= x1; ++x)
                SetValue(x, y, value);
    }

    void Map::IncRectangle(CVector2 const& point1, CVector2 const& point2, unsigned char value)
    {
        // RVA 0x7D8390
        int x0, y0, x1, y1;
        RectRange(point1, point2, x0, y0, x1, y1);
        for (int y = y0; y <= y1; ++y)
            for (int x = x0; x <= x1; ++x)
                IncValue(x, y, value);
    }

    void Map::DecRectangle(CVector2 const& point1, CVector2 const& point2, unsigned char value)
    {
        // RVA 0x7D8490
        int x0, y0, x1, y1;
        RectRange(point1, point2, x0, y0, x1, y1);
        for (int y = y0; y <= y1; ++y)
            for (int x = x0; x <= x1; ++x)
                DecValue(x, y, value);
    }

    // ---------------------------------------------------------------- circles

    // Circle / IncCircle / DecCircle all seed m_CurPos at the +X point of the
    // ring and then chain eight LineTo segments counter-clockwise through the
    // 45 degree marks. Note the seed's y comes from point.y un-offset, which is
    // what the binary does (0x7D8CD5).
    template <typename LineToFn>
    void Map::WalkRing(CVector2 const& point, float radius, LineToFn lineTo)
    {
        m_CurPos.x = static_cast<int>((point.x + radius) / m_cellSize.x);
        m_CurPos.y = static_cast<int>(point.y / m_cellSize.y);

        float const diag = radius * DIAG_45;
        CVector2 p;

        p.x = point.x + diag;  p.y = point.y + diag;   lineTo(p);
        p.x = point.x;         p.y = point.y + radius; lineTo(p);
        p.x = point.x - diag;  p.y = point.y + diag;   lineTo(p);
        p.x = point.x - radius; p.y = point.y;         lineTo(p);
        p.x = point.x - diag;  p.y = point.y - diag;   lineTo(p);
        p.x = point.x;         p.y = point.y - radius; lineTo(p);
        p.x = point.x + diag;  p.y = point.y - diag;   lineTo(p);
        p.x = point.x + radius; p.y = point.y;         lineTo(p);
    }

    void Map::Circle(CVector2 const& point, float radius, unsigned char value)
    {
        // RVA 0x7D8CC0
        WalkRing(point, radius, [this, value](CVector2 const& p) { LineTo(p, value); });
    }

    void Map::IncCircle(CVector2 const& point, float radius, unsigned char value)
    {
        // RVA 0x7D8E50
        WalkRing(point, radius, [this, value](CVector2 const& p) { IncLineTo(p, value); });
    }

    void Map::DecCircle(CVector2 const& point, float radius, unsigned char value)
    {
        // RVA 0x7D8FD0
        WalkRing(point, radius, [this, value](CVector2 const& p) { DecLineTo(p, value); });
    }

    // ---------------------------------------------------------------- queries

    bool Map::IsLine8Blocked(int xIndex1, int yIndex1, int xIndex2, int yIndex2, unsigned BV)
    {
        // RVA 0x7D7A90 - the 8-connected walk, including the starting cell.
        bool blocked = false;
        Walk8(xIndex1, yIndex1, xIndex2, yIndex2, true, [&](int x, int y) {
            if (!blocked && CellValue(x, y) > BV)
                blocked = true;
        });
        return blocked;
    }

    bool Map::IsLine4Blocked(int xIndex1, int yIndex1, int xIndex2, int yIndex2, unsigned BV)
    {
        // RVA 0x7D7C70 - the 4-connected walk, including the starting cell.
        bool blocked = false;
        Walk4(xIndex1, yIndex1, xIndex2, yIndex2, [&](int x, int y) {
            if (!blocked && CellValue(x, y) > BV)
                blocked = true;
        });
        return blocked;
    }

    bool Map::IsSolidBlocked(MapIndex const& index1, MapIndex const& index2, int radius, unsigned char blockedValue)
    {
        // RVA 0x7D8A90. Tests the centre line offset by +radius in x, then, if
        // the radius is non-zero, the -x, +y and -y offsets as well. Note the
        // first probe is offset rather than centred, so radius 0 still shifts
        // nothing and the four probes are not symmetric about the centre line.
        if (IsLine4Blocked(index1.x + radius, index1.y, index2.x + radius, index2.y, blockedValue))
            return true;
        if (!radius)
            return false;
        if (IsLine4Blocked(index1.x - radius, index1.y, index2.x - radius, index2.y, blockedValue))
            return true;
        if (IsLine4Blocked(index1.x, index1.y + radius, index2.x, index2.y + radius, blockedValue))
            return true;
        return IsLine4Blocked(index1.x, index1.y - radius, index2.x, index2.y - radius, blockedValue);
    }

    bool Map::IsSolidBlocked(CVector2 const& point1, CVector2 const& point2, float radius, unsigned char blockedValue)
    {
        // RVA 0x7D8B50
        float const invX = 1.0f / m_cellSize.x;
        float const invY = 1.0f / m_cellSize.y;
        MapIndex const index1(static_cast<int>(point1.x * invX), static_cast<int>(point1.y * invY));
        MapIndex const index2(static_cast<int>(point2.x * invX), static_cast<int>(point2.y * invY));
        return IsSolidBlocked(index1, index2, RadiusToCells(radius), blockedValue);
    }

    bool Map::IsCircleBlocked(MapIndex const& index, int radius, unsigned char BV)
    {
        // RVA 0x7D7040. Despite the name this scans the whole square bounding
        // the radius, not a disc.
        if (-radius > radius)
            return false;
        for (int ox = -radius; ox <= radius; ++ox)
        {
            for (int oy = -radius; oy <= radius; ++oy)
            {
                if (CellValue(index.x + ox, index.y + oy) > BV)
                    return true;
            }
        }
        return false;
    }

    bool Map::IsCircleBlocked(CVector2 const& point, float radius, unsigned char BV)
    {
        // RVA 0x7D8BF0
        MapIndex const index(static_cast<int>(point.x / m_cellSize.x), static_cast<int>(point.y / m_cellSize.y));
        return IsCircleBlocked(index, RadiusToCells(radius), BV);
    }

    bool Map::IsBlocked(int xIndex, int yIndex, int direction, int radius, unsigned char BV)
    {
        // RVA 0x7D9690. With no radius this is a single cell test; otherwise it
        // walks the precomputed offset fan for `direction` at ring `radius - 1`,
        // clamped to the last ring the table holds.
        if (!radius)
        {
            return CellValue(xIndex, yIndex) > BV;
        }

        unsigned ring = static_cast<unsigned>(radius - 1);
        if (ring >= m_DirSet.size())
        {
            ring = static_cast<unsigned>(m_DirSet.size()) - 1;
        }

        auto const& offsets = m_DirSet[ring].m_Dir[direction].m_Off;
        for (auto const& off : offsets)
        {
            if (CellValue(xIndex + off.x, yIndex + off.y) > BV)
                return true;
        }
        return false;
    }

    // ----------------------------------------------------------- ctor / dtor

    Map::Map() : m_lastIndex(0, 0), m_CurPos(0, 0)
    {
        // RVA 0x7DA900. Rings 0..2 are spelled out one offset at a time in the
        // shipped build (with a few offsets repeated - reproduced verbatim
        // below), while rings 3..7 are generated by the loop at the end.
        pField = 0;

        m_DirSet.resize(8);

        m_DirSet[0].AddItem(false, 0, 0);
        m_DirSet[0].AddItem(false, 1, -1);
        m_DirSet[0].AddItem(false, 1, 0);
        m_DirSet[0].AddItem(false, 1, 1);
        m_DirSet[0].AddItem(true, 0, 0);
        m_DirSet[0].AddItem(true, 0, 1);
        m_DirSet[0].AddItem(true, 1, 0);
        m_DirSet[0].AddItem(true, 1, 1);

        m_DirSet[1].AddItem(false, 0, 0);
        m_DirSet[1].AddItem(false, 2, 0);
        m_DirSet[1].AddItem(false, 2, 1);
        m_DirSet[1].AddItem(false, 2, -1);
        m_DirSet[1].AddItem(false, 1, -2);
        m_DirSet[1].AddItem(false, 1, 2);
        m_DirSet[1].AddItem(false, 0, -2);
        m_DirSet[1].AddItem(false, 0, 2);
        m_DirSet[1].AddItem(true, 0, 0);
        m_DirSet[1].AddItem(true, 2, 1);
        m_DirSet[1].AddItem(true, 1, 2);
        m_DirSet[1].AddItem(true, 2, 0);
        m_DirSet[1].AddItem(true, 0, 2);
        m_DirSet[1].AddItem(true, 2, -1);
        m_DirSet[1].AddItem(true, -1, 2);

        m_DirSet[2].AddItem(false, 0, 0);
        m_DirSet[2].AddItem(false, 3, 0);
        m_DirSet[2].AddItem(false, 3, 1);
        m_DirSet[2].AddItem(false, 3, -1);
        m_DirSet[2].AddItem(false, 2, 2);
        m_DirSet[2].AddItem(false, 2, -2);
        m_DirSet[2].AddItem(false, 1, 3);
        m_DirSet[2].AddItem(false, 1, 3);  // duplicated in the original
        m_DirSet[2].AddItem(false, 0, 3);
        m_DirSet[2].AddItem(false, 0, 3);  // duplicated in the original
        m_DirSet[2].AddItem(true, 0, 0);
        m_DirSet[2].AddItem(true, 2, 2);
        m_DirSet[2].AddItem(true, 1, 3);
        m_DirSet[2].AddItem(true, 3, 1);
        m_DirSet[2].AddItem(true, 3, 0);
        m_DirSet[2].AddItem(true, 0, 3);
        m_DirSet[2].AddItem(true, -1, 3);
        m_DirSet[2].AddItem(true, 3, -1);
        m_DirSet[2].AddItem(true, -2, 2);
        m_DirSet[2].AddItem(true, 2, -2);

        // Rings 3..7. Per (x, y) the original emits the axis-aligned rotations
        // of (x, y) and of (x, -y), then the diagonal rotations of (x, y). The
        // first of the three is inlined in the binary rather than calling
        // AddItem, but the four push_backs are identical to AddItem(false,x,y).
        for (int ring = 3; ring < 8; ++ring)
        {
            for (int x = 0; x <= ring; ++x)
            {
                for (int y = 0; y <= ring; ++y)
                {
                    m_DirSet[ring].AddItem(false, x, y);
                    m_DirSet[ring].AddItem(false, x, -y);
                    m_DirSet[ring].AddItem(true, x, y);
                }
            }
        }
    }

    Map::~Map()
    {
        // RVA 0x7DA870
        delete[] pField;
        pField = nullptr;
    }
}  // namespace ai
