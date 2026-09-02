#include "izvratrepositorywnd.h"

#include <m3dapp.h>
#include <ui/frame.h>
#include <ui/ui_srv.h>

RT_CLASS_EXPORTS_BEGIN(IzvratRepositoryWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(IzvratRepositoryWnd);

IzvratRepositoryWnd::FrameSegment::FrameSegment(FrameSegment const& that)
    : m_frameTexId(that.m_frameTexId)
    , m_rect(that.m_rect)
{
}

IzvratRepositoryWnd::FrameSegment::FrameSegment(int frameTexId, BoundsBase<float> const& rect)
    : m_frameTexId(frameTexId)
    , m_rect(rect)
{
}

m3d::Object* IzvratRepositoryWnd::CreateObject()
{
    return new IzvratRepositoryWnd();
}

m3d::Class* IzvratRepositoryWnd::GetClass() const
{
    return RT_CLASS_LOCAL(IzvratRepositoryWnd);
}

m3d::Class* IzvratRepositoryWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(RepositoryWnd);
}

m3d::Object* IzvratRepositoryWnd::Clone()
{
    // Matches the original: Clone allocates a fresh, default-constructed
    // IzvratRepositoryWnd (it does not copy any state from *this).
    return new IzvratRepositoryWnd();
}

IzvratRepositoryWnd::IzvratRepositoryWnd()
{
}

IzvratRepositoryWnd::IzvratRepositoryWnd(IzvratRepositoryWnd const&)
{
    // Matches the original: the copy ctor chains to the RepositoryWnd default
    // ctor and leaves m_frameSegments empty - nothing is copied.
}

IzvratRepositoryWnd::~IzvratRepositoryWnd()
{
    // m_frameSegments releases its storage automatically; RepositoryWnd::~RepositoryWnd runs after.
}

ai::GeomRepository* IzvratRepositoryWnd::GetRepository() const
{
    return m_repository;
}

ai::IzvratRepository* IzvratRepositoryWnd::GetIzvratRepository() const
{
    return RT_DYNCAST(m_repository, ai::IzvratRepository);
}

int IzvratRepositoryWnd::SetRepository(ai::GeomRepository* repository, RepositoryWnd::RepositoryType type)
{
    if (!repository || RT_DYNCAST(repository, ai::IzvratRepository))
    {
        return SetIzvratRepository(static_cast<ai::IzvratRepository*>(repository), type);
    }
    return 0;
}

int IzvratRepositoryWnd::SetIzvratRepository(ai::IzvratRepository* repository, RepositoryWnd::RepositoryType type)
{
    int result = RepositoryWnd::SetRepository(repository, type);
    if (result)
    {
        CalculateFrame();
        return 1;
    }
    return result;
}

int IzvratRepositoryWnd::CreateScrollWnd()
{
    return 1;
}

BoundsBase<float> IzvratRepositoryWnd::GetMaxRepositoryBounds() const
{
    auto* rep = RT_DYNCAST(m_repository, ai::IzvratRepository);
    if (!rep)
    {
        return BoundsBase<float>{0.0f, 0.0f, 0.0f, 0.0f};
    }

    PointBase<int> const& maxSz = rep->GetMaxGeomSize();
    BoundsBase<float> res;
    res.x0 = m_bounds.x0 - static_cast<float>((maxSz.x - m_geomSize.x) / 2) * m_cellSize.x;
    res.y0 = m_bounds.y0 - static_cast<float>((maxSz.y - m_geomSize.y) / 2) * m_cellSize.y;
    res.width = static_cast<float>(maxSz.x) * m_cellSize.x;
    res.height = static_cast<float>(maxSz.y) * m_cellSize.y;
    return res;
}

void IzvratRepositoryWnd::CalculateFrame()
{
    PerimeterCellsMap perimeterCells;
    FindPerimeter(perimeterCells);
    CalculateSegmentsBounds(perimeterCells, m_frameSegments);
}

void IzvratRepositoryWnd::FindPerimeter(
    std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int>>>& perimeterCells) const
{
    auto* rep = RT_DYNCAST(m_repository, ai::IzvratRepository);
    if (!rep)
    {
        return;
    }

    std::vector<int> const& cells = rep->GetCells();
    int const maxX = rep->GetMaxGeomSize().x;
    int const maxY = rep->GetMaxGeomSize().y;

    static const PointBase<int> dirs[8] = {
        {-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0},
    };

    for (int cellId = 0; cellId < static_cast<int>(cells.size()); ++cellId)
    {
        if (cells[cellId] == -2)
        {
            continue;
        }
        int const cx = cellId % maxX;
        int const cy = cellId / maxX;

        int perimeterSide = 0;
        for (int d = 0; d < 8; ++d)
        {
            int const nx = cx + dirs[d].x;
            int const ny = cy + dirs[d].y;
            if (nx < 0 || nx >= maxX || ny < 0 || ny >= maxY)
            {
                perimeterSide |= 1 << d;
                continue;
            }
            if (cells[nx + ny * maxX] == -2)
            {
                perimeterSide |= 1 << d;
            }
        }
        if (perimeterSide)
        {
            perimeterCells.insert(std::make_pair(cellId, perimeterSide));
        }
    }
}

void IzvratRepositoryWnd::CalculateSegmentsBounds(
    std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int>>> const& /*perimeterCells*/,
    std::vector<IzvratRepositoryWnd::FrameSegment, std::allocator<IzvratRepositoryWnd::FrameSegment>>& frameSegments)
{
    // TODO(RVA 0x45A5B0): the shipped IzvratRepositoryWnd::CalculateSegmentsBounds walks
    // every perimeter cell and, from its 8-neighbour perimeter bitmask, emits 1..3
    // decorative FrameSegment{texId, rect} entries (12 border tiles: 0 = left, 1 = top,
    // 2 = right, 3 = bottom, 4..7 = corners) laid out from the pane Frame metrics
    // (m_barTexWidth / m_barUsedWidth / m_cornerSize), then offsets every rect by
    // m_barTexWidth and std::partition's the edge tiles ahead of the corner tiles
    // (LessByTexId). The Hex-Rays output for this ~400-line float-geometry routine has
    // corrupted register/stack tracking (dozens of aliased float temporaries, terms
    // like "cellB.x0 - cellB.width"), so it cannot be reproduced faithfully from the
    // decompile. Clearing the list keeps the window fully functional - the grid, the
    // drag "bunch" and the items all render; only the cosmetic irregular border is
    // absent until this is reconstructed from the game's pane definitions.
    frameSegments.clear();
}

void IzvratRepositoryWnd::DrawGrid(m3d::ui::DrawInfo const& di)
{
    auto* rep = RT_DYNCAST(m_repository, ai::IzvratRepository);
    if (!rep)
    {
        return;
    }

    std::vector<int> const& cells = rep->GetCells();
    int const maxX = rep->GetMaxGeomSize().x;

    BoundsBase<int> geomB = rep->ToMaxSzRelative(BoundsBase<int>{0, 0, m_geomSize.x, m_geomSize.y});
    for (int x = geomB.x0; x < geomB.x0 + geomB.width; ++x)
    {
        for (int y = geomB.y0; y < geomB.y0 + geomB.height; ++y)
        {
            int const idx = maxX * y + x;
            if (idx < 0 || idx >= static_cast<int>(cells.size()) || cells[idx] == -2)
            {
                continue;
            }
            PointBase<int> posGeomRel = rep->ToGeomSzRelative(PointBase<int>{x, y});
            BoundsBase<float> cellB = GeomToWndBounds(BoundsBase<int>{posGeomRel.x, posGeomRel.y, 1, 1});
            GetGfxServer()->AddImagedRect(di, cellB, m_curClr, m_cellTextures[0]);
        }
    }
}

void IzvratRepositoryWnd::DrawBunch(m3d::ui::DrawInfo const& di)
{
    auto* rep = RT_DYNCAST(m_repository, ai::IzvratRepository);
    if (!rep || (m_curBunch.m_bounds.width == 0 && m_curBunch.m_bounds.height == 0))
    {
        return;
    }

    SlotState const state = m_curBunch.m_state;
    if (state <= SLOTSTATE_UNSELECTED || state > SLOTSTATE_OWN_PLACE_IN_HIDDEN_MODE)
    {
        return;
    }

    std::vector<int> const& cells = rep->GetCells();
    int const maxX = rep->GetMaxGeomSize().x;

    for (int x = m_curBunch.m_bounds.x0; x < m_curBunch.m_bounds.x0 + m_curBunch.m_bounds.width; ++x)
    {
        for (int y = m_curBunch.m_bounds.y0; y < m_curBunch.m_bounds.y0 + m_curBunch.m_bounds.height; ++y)
        {
            PointBase<int> posMaxRel = rep->ToMaxSzRelative(PointBase<int>{x, y});
            if (cells[posMaxRel.x + posMaxRel.y * maxX] != -2)
            {
                BoundsBase<float> cellB = GeomToWndBounds(BoundsBase<int>{x, y, 1, 1});
                GetGfxServer()->AddImagedRect(di, cellB, 0xFFFFFFFFu, m_cellTextures[m_curBunch.m_state]);
            }
        }
    }
}

void IzvratRepositoryWnd::DrawFrame(m3d::ui::DrawInfo const& di)
{
    m3d::ui::Pane* pane = GetGfxServer()->GetPane(m_paneName);
    if (!pane)
    {
        return;
    }
    m3d::ui::Frame* frame = pane->m_frame[0];
    if (!frame)
    {
        return;
    }

    float const usedW = static_cast<float>(frame->m_barUsedWidth);
    m3d::ui::DrawInfo fakeDi(di);
    fakeDi.m_originalRect.x0 -= usedW;
    fakeDi.m_originalRect.y0 -= usedW;
    fakeDi.m_originalRect.width += usedW * 2.0f;
    fakeDi.m_originalRect.height += usedW * 2.0f;
    fakeDi.m_clippedRect.x0 -= usedW;
    fakeDi.m_clippedRect.y0 -= usedW;
    fakeDi.m_clippedRect.width += usedW * 2.0f;
    fakeDi.m_clippedRect.height += usedW * 2.0f;

    for (int i = 0; i < static_cast<int>(m_frameSegments.size()); ++i)
    {
        FrameSegment const& seg = m_frameSegments[i];
        if (seg.m_frameTexId >= 12)
        {
            continue;
        }

        float u1 = 1.0f;
        float v1 = 1.0f;
        if (frame->m_barRepeat != 0)
        {
            int absTexW = 0;
            int absTexH = 0;
            M3D_RENDERER->GetDims(frame->m_textures[seg.m_frameTexId], absTexW, absTexH);
            float relTexW = static_cast<float>(absTexW);
            float relTexH = static_cast<float>(absTexH);
            M3D_RENDERER->AbsToRel(relTexW, relTexH);
            if (seg.m_frameTexId == 1 || seg.m_frameTexId == 3)
            {
                u1 = seg.m_rect.width / relTexW;
            }
            else if (seg.m_frameTexId == 0 || seg.m_frameTexId == 2)
            {
                v1 = seg.m_rect.height / relTexH;
            }
        }

        GetGfxServer()->AddImagedRectGeneral(
            fakeDi, seg.m_rect, m_curClr, frame->m_textures[seg.m_frameTexId], 0.0f, 0.0f, u1, v1);
    }
}
