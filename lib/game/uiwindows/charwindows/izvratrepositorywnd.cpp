#include "izvratrepositorywnd.h"

#include <algorithm>

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

namespace
{
    // RVA 0x45A590. The name is the shipped one, but this is a unary predicate
    // rather than a comparator: it answers "is this one of the four straight
    // bars?", which is all std::partition needs to bring the bars in front of
    // the corners.
    bool LessByTexId(IzvratRepositoryWnd::FrameSegment const& segment)
    {
        return segment.m_frameTexId == 0 || segment.m_frameTexId == 1 || segment.m_frameTexId == 2 ||
               segment.m_frameTexId == 3;
    }
}  // namespace

void IzvratRepositoryWnd::CalculateSegmentsBounds(
    std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int>>> const& perimeterCells,
    std::vector<IzvratRepositoryWnd::FrameSegment, std::allocator<IzvratRepositoryWnd::FrameSegment>>& frameSegments)
{
    // RVA 0x45A5B0. Turns the perimeter bitmask FindPerimeter recorded for each
    // cell into the decorative border tiles DrawFrame later draws. The bit
    // numbering is FindPerimeter's neighbour order:
    //
    //     0 = NW   1 = N   2 = NE
    //     7 = W            3 = E
    //     6 = SW   5 = S   4 = SE
    //
    // and a set bit means that neighbour is missing, i.e. the frame has to run
    // along that side. Tile ids index Frame::m_textures: 0 = left bar, 1 = top
    // bar, 2 = right bar, 3 = bottom bar, 4 = top-left corner, 5 = top-right,
    // 6 = bottom-left, 7 = bottom-right.
    frameSegments.clear();

    auto* rep = RT_DYNCAST(m_repository, ai::IzvratRepository);
    if (!rep)
    {
        return;
    }
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

    float const frameW = static_cast<float>(frame->m_barTexWidth);
    float const frameUsedW = static_cast<float>(frame->m_barUsedWidth);
    float const cornerSz = static_cast<float>(frame->m_cornerSize);

    for (PerimeterCellsMap::const_iterator pIt = perimeterCells.begin(); pIt != perimeterCells.end(); ++pIt)
    {
        int const maxX = rep->GetMaxGeomSize().x;
        int const cx = pIt->first % maxX;
        int const cy = pIt->first / maxX;
        BoundsBase<float> const cellB = GeomToWndBounds(rep->ToGeomSzRelative(BoundsBase<int>(cx, cy, cx + 1, cy + 1)));

        float const cellRight = cellB.x0 + cellB.width;
        float const cellBottom = cellB.y0 + cellB.height;
        int const perimeterSide = pIt->second;

        // The four straight edges: three neighbours in a row are missing.
        if (perimeterSide == 7)  // NW | N | NE
        {
            float const y0 = cellB.y0 - frameUsedW;
            BoundsBase<float> const topB(cellB.x0, y0, cellRight, y0 + frameW);
            frameSegments.push_back(FrameSegment(1, topB));
        }
        else if (perimeterSide == 0x1C)  // NE | E | SE
        {
            float const x0 = cellRight + frameUsedW - frameW;
            BoundsBase<float> const rightB(x0, cellB.y0, x0 + frameW, cellBottom);
            frameSegments.push_back(FrameSegment(2, rightB));
        }
        else if (perimeterSide == 0x70)  // SE | S | SW
        {
            float const y0 = cellBottom + frameUsedW - frameW;
            BoundsBase<float> const bottomB(cellB.x0, y0, cellRight, y0 + frameW);
            frameSegments.push_back(FrameSegment(3, bottomB));
        }
        else if (perimeterSide == 0xC1)  // NW | W | SW
        {
            float const x0 = cellB.x0 - frameUsedW;
            BoundsBase<float> const leftB(x0, cellB.y0, x0 + frameW, cellBottom);
            frameSegments.push_back(FrameSegment(0, leftB));
        }
        // Concave corners: exactly one diagonal neighbour is missing, so a
        // corner tile plus the two short bars that reach into the neighbouring
        // cells are emitted.
        else if (perimeterSide == 0x10)  // SE
        {
            float const cornerX0 = cellRight;
            float const cornerY0 = cellBottom;
            BoundsBase<float> const cornerB(cornerX0, cornerY0, cornerX0 + cornerSz, cornerY0 + cornerSz);
            frameSegments.push_back(FrameSegment(4, cornerB));
            frameSegments.push_back(FrameSegment(
                3,
                BoundsBase<float>(cornerX0 + cornerB.width, cellBottom, cornerX0 + cellB.width, cellBottom + frameW)));
            frameSegments.push_back(FrameSegment(
                2,
                BoundsBase<float>(cellRight, cornerY0 + cornerB.height, cellRight + frameW, cornerY0 + cellB.height)));
        }
        else if (perimeterSide == 0x40)  // SW
        {
            float const cornerX0 = cellB.x0 - cornerSz;
            float const cornerY0 = cellBottom;
            BoundsBase<float> const cornerB(cornerX0, cornerY0, cornerX0 + cornerSz, cornerY0 + cornerSz);
            frameSegments.push_back(FrameSegment(5, cornerB));
            frameSegments.push_back(FrameSegment(
                3, BoundsBase<float>(cellB.x0 - cellB.width, cellBottom, cornerX0, cellBottom + frameW)));
            // NOTE: the three mirrored cases extend this bar by the cell's
            // height; the shipped code (RVA 0x45AD32) adds cellB.width here
            // instead, so on a grid whose cells are not square the bottom-left
            // notch comes out the wrong length. Kept as shipped.
            float const x0 = cellB.x0 - frameUsedW;
            frameSegments.push_back(FrameSegment(
                0, BoundsBase<float>(x0, cornerY0 + cornerB.height, x0 + frameW, cornerY0 + cellB.width)));
        }
        else if (perimeterSide == 1)  // NW
        {
            float const cornerX0 = cellB.x0 - cornerSz;
            float const cornerY0 = cellB.y0 - cornerSz;
            BoundsBase<float> const cornerB(cornerX0, cornerY0, cornerX0 + cornerSz, cornerY0 + cornerSz);
            frameSegments.push_back(FrameSegment(7, cornerB));
            // NOTE: alone among the twelve cases these two bars are inset by
            // frameW rather than frameUsedW (RVA 0x45AE78 and 0x45AF3E).
            frameSegments.push_back(FrameSegment(
                0,
                BoundsBase<float>(cellB.x0 - frameW, cellB.y0 - cellB.height, cellB.x0 - frameW + frameW, cornerY0)));
            frameSegments.push_back(FrameSegment(
                1,
                BoundsBase<float>(cellB.x0 - cellB.width, cellB.y0 - frameW, cornerX0, cellB.y0 - frameW + frameW)));
        }
        else if (perimeterSide == 4)  // NE
        {
            float const cornerX0 = cellRight;
            float const cornerY0 = cellB.y0 - cornerSz;
            BoundsBase<float> const cornerB(cornerX0, cornerY0, cornerX0 + cornerSz, cornerY0 + cornerSz);
            frameSegments.push_back(FrameSegment(6, cornerB));
            frameSegments.push_back(FrameSegment(
                1,
                BoundsBase<float>(
                    cornerX0 + cornerB.width,
                    cellB.y0 - frameW,
                    cellB.x0 + cellB.width * 2.0f,
                    cellB.y0 - frameW + frameW)));
            frameSegments.push_back(FrameSegment(
                2, BoundsBase<float>(cellRight, cellB.y0 - cellB.height, cellRight + frameW, cornerY0)));
        }
        // Convex corners: a whole quadrant is missing. These are tested with
        // masks rather than for equality, so the order they run in matters.
        else if ((perimeterSide & 0x83) == 0x83)  // NW | N | W
        {
            float const cornerX0 = cellB.x0 - frameUsedW;
            float const cornerY0 = cellB.y0 - frameUsedW;
            BoundsBase<float> const cornerB(cornerX0, cornerY0, cornerX0 + cornerSz, cornerY0 + cornerSz);
            frameSegments.push_back(FrameSegment(4, cornerB));
            float const leftY0 = cornerB.y0 + cornerB.height;
            frameSegments.push_back(FrameSegment(
                0, BoundsBase<float>(cornerB.x0, leftY0, cornerB.x0 + frameW, leftY0 + cellB.height)));
            float const topX0 = cornerB.x0 + cornerB.width;
            frameSegments.push_back(FrameSegment(
                1, BoundsBase<float>(topX0, cornerB.y0, topX0 + cellB.width, cornerB.y0 + frameW)));
        }
        else if ((perimeterSide & 0x0E) == 0x0E)  // N | NE | E
        {
            float const cornerX0 = cellRight + frameUsedW - cornerSz;
            float const cornerY0 = cellB.y0 - frameUsedW;
            BoundsBase<float> const cornerB(cornerX0, cornerY0, cornerX0 + cornerSz, cornerY0 + cornerSz);
            frameSegments.push_back(FrameSegment(5, cornerB));
            frameSegments.push_back(
                FrameSegment(1, BoundsBase<float>(cellB.x0, cornerY0, cornerB.x0, cornerY0 + frameW)));
            float const rightX0 = cellRight + frameUsedW - frameW;
            frameSegments.push_back(FrameSegment(
                2, BoundsBase<float>(rightX0, cornerB.y0 + cornerB.height, rightX0 + frameW, cellBottom)));
        }
        else if ((perimeterSide & 0x38) == 0x38)  // E | SE | S
        {
            float const cornerX0 = cellRight + frameUsedW - cornerSz;
            float const cornerY0 = cellBottom + frameUsedW - cornerSz;
            BoundsBase<float> const cornerB(cornerX0, cornerY0, cornerX0 + cornerSz, cornerY0 + cornerSz);
            frameSegments.push_back(FrameSegment(7, cornerB));
            float const rightX0 = cellRight + frameUsedW - frameW;
            frameSegments.push_back(
                FrameSegment(2, BoundsBase<float>(rightX0, cellB.y0, rightX0 + frameW, cornerB.y0)));
            float const bottomY0 = cellBottom + frameUsedW - frameW;
            frameSegments.push_back(
                FrameSegment(3, BoundsBase<float>(cellB.x0, bottomY0, cornerB.x0, bottomY0 + frameW)));
        }
        else if ((perimeterSide & 0xE0) == 0xE0)  // S | SW | W
        {
            float const cornerX0 = cellB.x0 - frameUsedW;
            float const cornerY0 = cellBottom + frameUsedW - cornerSz;
            BoundsBase<float> const cornerB(cornerX0, cornerY0, cornerX0 + cornerSz, cornerY0 + cornerSz);
            frameSegments.push_back(FrameSegment(6, cornerB));
            float const bottomY0 = cellBottom + frameUsedW - frameW;
            frameSegments.push_back(FrameSegment(
                3, BoundsBase<float>(cornerB.x0 + cornerB.width, bottomY0, cellRight, bottomY0 + frameW)));
            frameSegments.push_back(
                FrameSegment(0, BoundsBase<float>(cornerX0, cellB.y0, cornerX0 + frameW, cornerB.y0)));
        }
    }

    // Every tile was laid out in unshifted window space; DrawFrame draws them
    // through a DrawInfo whose rect has been grown by the bar width, so they all
    // get shifted to match.
    for (int i = 0; i < static_cast<int>(frameSegments.size()); ++i)
    {
        frameSegments[i].m_rect.x0 += frameW;
        frameSegments[i].m_rect.y0 += frameW;
    }

    // Bars first, corners last, so the corner art always draws over the ends of
    // the bars running into it.
    std::partition(frameSegments.begin(), frameSegments.end(), LessByTexId);
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
            BoundsBase<float> cellB =
                GeomToWndBounds(BoundsBase<int>(posGeomRel.x, posGeomRel.y, posGeomRel.x + 1, posGeomRel.y + 1));
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
                BoundsBase<float> cellB = GeomToWndBounds(BoundsBase<int>(x, y, x + 1, y + 1));
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
