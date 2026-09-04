#include "izvratrepository.h"

#include <stdexcept>

#include "geomrepositoryitem.h"
#include "objects/base/globalproperties.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(IzvratRepository)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(IzvratRepository);

    void IzvratRepository::Purge()
    {
        // TODO: generated code
        // First pass: remove invalid or empty items
        auto it = m_slots.begin();
        while (it != m_slots.end())
        {
            if (it->IsValid() && (it->m_repositoryItemType || it->m_amount))
            {
                // Item is valid, keep it
                ++it;
            }
            else
            {
                // Item is invalid or empty - remove it
                // First, mark the cell area as available (-1)
                BoundsBase<int> bounds = it->GetBounds();

                // Adjust bounds to account for centering within max geometry
                bounds.x0 += (m_maxGeomSize.x - m_geomSize.x) / 2;
                bounds.y0 += (m_maxGeomSize.y - m_geomSize.y) / 2;

                MarkCellPieceByValue(-1, bounds);

                // Remove the item from the slots
                it = m_slots.erase(it);
            }
        }

        // Second pass: reassign cell indices to remaining items
        int index = 0;
        for (auto& item : m_slots)
        {
            BoundsBase<int> bounds = item.GetBounds();

            // Adjust bounds to account for centering within max geometry
            bounds.x0 += (m_maxGeomSize.x - m_geomSize.x) / 2;
            bounds.y0 += (m_maxGeomSize.y - m_geomSize.y) / 2;

            // Mark the cell area with the item's new index
            MarkCellPieceByValue(index, bounds);
            ++index;
        }
    }

    std::vector<int, std::allocator<int>> const& IzvratRepository::GetCells() const
    {
        // RVA 0x6E8E90
        return m_cells;
    }

    IzvratRepository::~IzvratRepository()
    {
    }

    int IzvratRepository::CanAddThingToPlace(
        GeomRepositoryItem const& item,
        PointBase<int> const& origin,
        int* existingSlot) const
    {
        // RVA 0x6E9510
        if (existingSlot)
        {
            *existingSlot = -1;
        }

        BoundsBase<int> const itemBounds = item.GetBounds();

        // If the item already lives here, find which slot it currently occupies so
        // GetUnifyCellValueFromPiece can treat those cells as free.
        int slotByPlace = -1;
        if (item.m_parentRepository == this)
        {
            PointBase<int> const currentPlace(itemBounds.x0, itemBounds.y0);
            slotByPlace = GetSlotByPlace(currentPlace);
        }

        BoundsBase<int> piece;
        piece.x0 = origin.x + (m_maxGeomSize.x - m_geomSize.x) / 2;
        piece.y0 = origin.y + (m_maxGeomSize.y - m_geomSize.y) / 2;
        piece.width = itemBounds.width;
        piece.height = itemBounds.height;

        int const unified = GetUnifyCellValueFromPiece(piece, slotByPlace);
        if (unified == -1)
        {
            // All-free footprint: the whole item fits.
            if (!item.IsValid())
            {
                return 0;
            }
            return item.m_repositoryItemType ? 1 : static_cast<int>(item.m_amount);
        }
        if (unified < 0)
        {
            return 0;
        }
        if (unified >= static_cast<int>(m_slots.size()))
        {
            return 0;
        }
        // The footprint sits exactly on an existing slot - ask it how much it takes.
        int const result = static_cast<int>(m_slots[unified].GetAcceptedNum(item));
        if (result > 0 && existingSlot)
        {
            *existingSlot = unified;
        }
        return result;
    }

    m3d::Object* IzvratRepository::Clone()
    {
        // RVA 0x6E9A60
        return new IzvratRepository(*this);
    }

    m3d::Class* IzvratRepository::GetBaseClass()
    {
        return RT_CLASS_LOCAL(GeomRepository);
    }

    bool IzvratRepository::AddThingToPlace(GeomRepositoryItem& item, PointBase<int> const& origin)
    {
        // RVA 0x6E9A90
        if (!item.IsValid() || (item.m_repositoryItemType == GeomRepositoryItem::ITEMTYPE_RESOURCE && !item.m_amount))
        {
            return false;
        }

        int existingSlot = -1;
        if (CanAddThingToPlace(item, origin, &existingSlot) <= 0)
        {
            return false;
        }

        if (existingSlot >= 0 && existingSlot < static_cast<int>(m_slots.size()))
        {
            m_slots[existingSlot].AcceptItem(item);
            SetChanged();
            return true;
        }

        GeomRepositoryItem newThing = item;
        newThing.AddToRepository(this, origin);
        m_slots.push_back(newThing);
        MarkCellPieceByValue(static_cast<int>(m_slots.size()) - 1, ToMaxSzRelative(newThing.GetBounds()));
        item.Invalidate();
        SetChanged();
        return true;
    }

    m3d::Object* IzvratRepository::CreateObject()
    {
        return new IzvratRepository;
    }

    bool IzvratRepository::SetGeomSize(PointBase<int> const& geomSize)
    {
        // TODO: generated code
        // Determine the actual size to use (clamped by maximum size)
        int actualX = (geomSize.x > m_maxGeomSize.x) ? m_maxGeomSize.x : geomSize.x;
        int actualY = (geomSize.y > m_maxGeomSize.y) ? m_maxGeomSize.y : geomSize.y;

        PointBase<int> actualSize(actualX, actualY);

        // Calculate total number of cells needed
        int totalCells = m_maxGeomSize.x * m_maxGeomSize.y;

        // Clear and resize the cells vector, initializing with -2
        m_cells.clear();
        m_cells.resize(totalCells, -2);

        // Calculate the piece bounds (centered within the maximum geometry)
        BoundsBase<int> piece;
        piece.x0 = (m_maxGeomSize.x - actualSize.x) / 2;
        piece.y0 = (m_maxGeomSize.y - actualSize.y) / 2;
        piece.width = actualSize.x;
        piece.height = actualSize.y;

        // Mark the active piece area with -1
        MarkCellPieceByValue(-1, piece);

        // Call base class implementation
        return GeomRepository::SetGeomSize(actualSize);
    }

    BoundsBase<int> IzvratRepository::ToMaxSzRelative(BoundsBase<int> geomSzRelative) const
    {
        // RVA 0x6E8F40
        BoundsBase<int> result(geomSzRelative);
        result.x0 += (m_maxGeomSize.x - m_geomSize.x) / 2;
        result.y0 += (m_maxGeomSize.y - m_geomSize.y) / 2;
        return result;
    }

    PointBase<int> IzvratRepository::ToMaxSzRelative(PointBase<int> geomSzRelative) const
    {
        // RVA 0x6E8F90
        // Shift a point from active-area space into the fixed max-size grid.
        PointBase<int> result(geomSzRelative);
        result.x += (m_maxGeomSize.x - m_geomSize.x) / 2;
        result.y += (m_maxGeomSize.y - m_geomSize.y) / 2;
        return result;
    }

    int IzvratRepository::SnapPiece(BoundsBase<int> const& piece)
    {
        // TODO: check and refactor this

        auto y = this->m_maxGeomSize.y;
        BoundsBase<int> pc;
        pc.x0 = 0;
        pc.y0 = 0;
        pc.width = this->m_maxGeomSize.x;
        pc.height = y;
        auto v4 = pc.Intersect(piece);
        auto x0 = v4.x0;
        auto width = v4.width;
        auto y0 = v4.y0;
        auto height = v4.height;
        auto v9 = x0 + width;
        pc.y0 = y0;
        auto v10 = x0;
        auto piecea = v9;
        if (x0 < v9)
        {
            auto v11 = height + y0;
            while (1)
            {
                if (pc.y0 < v11)
                {
                    do
                    {
                        auto v12 = v10 + y0 * this->m_maxGeomSize.x;
                        if (v12 >= 0)
                        {
                            auto v14 = this->m_cells.size();
                            if (v12 < v14)
                                this->m_cells[v12] = -2;
                        }
                        ++y0;
                    } while (y0 < v11);
                    v9 = piecea;
                }
                if (++v10 >= v9)
                    break;
                y0 = pc.y0;
            }
        }
        _RepackItems(this->m_sortStyle);
        return 1;
    }

    void IzvratRepository::SetMaxGeomSize(PointBase<int> const& maxGeomSize)
    {
        auto p_m_maxGeomSize = &this->m_maxGeomSize;
        this->m_maxGeomSize = maxGeomSize;
        auto v3 = this->GetGeomSize().x < this->m_maxGeomSize.x;
        auto y = this->GetGeomSize().y;

        PointBase<int> geomSize;
        geomSize.x = this->GetGeomSize().x;
        geomSize.y = y;
        if (v3)
            p_m_maxGeomSize = &geomSize;
        v3 = y < this->m_maxGeomSize.y;
        geomSize.x = p_m_maxGeomSize->x;
        auto p_y = &this->m_maxGeomSize.y;
        if (v3)
            p_y = &geomSize.y;
        auto v6 = *p_y;
        geomSize.y = v6;
        SetGeomSize(geomSize);
    }

    m3d::Class* IzvratRepository::GetClass() const
    {
        // RVA 0x6E8E70
        return RT_CLASS_LOCAL(IzvratRepository);
    }

    void IzvratRepository::Clear(bool bUnsafe)
    {
        GeomRepository::Clear(bUnsafe);

        BoundsBase<int> piece;
        piece.x0 = 0;
        piece.y0 = 0;
        piece.width = m_maxGeomSize.x;
        piece.height = m_maxGeomSize.y;
        MarkCellPieceByValueExcluding(-1, piece, -2);
    }

    IzvratRepository& IzvratRepository::operator=(IzvratRepository const& rhs)
    {
        // RVA 0x6E99B0
        // RVA 0x6E99B0. NOTE: the shipped code copies only these five members - the
        // base m_referenceChests / m_vehicleId / m_sortStyle are deliberately left as is.
        m_Changed = true;
        m_geomSize = rhs.m_geomSize;
        m_slots = rhs.m_slots;
        m_maxGeomSize = rhs.m_maxGeomSize;
        m_cells = rhs.m_cells;
        return *this;
    }

    PointBase<int> const& IzvratRepository::GetMaxGeomSize() const
    {
        // RVA 0x6E8E80
        return m_maxGeomSize;
    }

    PointBase<int> IzvratRepository::ToGeomSzRelative(PointBase<int> maxSzRelative) const
    {
        // RVA 0x6E8FD0
        // The inverse of ToMaxSzRelative.
        PointBase<int> result(maxSzRelative);
        result.x -= (m_maxGeomSize.x - m_geomSize.x) / 2;
        result.y -= (m_maxGeomSize.y - m_geomSize.y) / 2;
        return result;
    }

    BoundsBase<int> IzvratRepository::ToGeomSzRelative(BoundsBase<int> maxSzRelative) const
    {
        // RVA 0x6E9030
        BoundsBase<int> result(maxSzRelative);
        result.x0 -= (m_maxGeomSize.x - m_geomSize.x) / 2;
        result.y0 -= (m_maxGeomSize.y - m_geomSize.y) / 2;
        return result;
    }

    IzvratRepository::IzvratRepository(IzvratRepository const& rhs)
    {
        // RVA 0x6E9A00
        // RVA 0x6E9A00: default-construct the base, then copy the same five members
        // operator= handles.
        m_Changed = true;
        m_geomSize = rhs.m_geomSize;
        m_slots = rhs.m_slots;
        m_maxGeomSize = rhs.m_maxGeomSize;
        m_cells = rhs.m_cells;
    }

    IzvratRepository::IzvratRepository()
    {
        SetMaxGeomSize(ai::theGlobProp.m_izvratRepositoryMaxSize);
    }

    int IzvratRepository::GetUnifyCellValueFromPiece(BoundsBase<int> const& piece, int itemId) const
    {
        // RVA 0x6E9180
        // Fold every cell the (grid-clipped) piece covers into one value: -3 when the
        // region is empty or the cells disagree, otherwise the common value. A piece
        // that only straddles free cells and its own slot's cells (itemId) folds to -1.
        BoundsBase<int> const grid(0, 0, m_maxGeomSize.x, m_maxGeomSize.y);
        BoundsBase<int> const region = grid.Intersect(piece);

        int unified = -3;
        bool haveBaseline = false;
        int const stride = m_maxGeomSize.x;
        for (int col = region.x0; col < region.x0 + region.width; ++col)
        {
            for (int row = region.y0; row < region.y0 + region.height; ++row)
            {
                int const cellIndex = row * stride + col;
                if (cellIndex < 0 || cellIndex >= static_cast<int>(m_cells.size()))
                {
                    continue;
                }
                int const cell = m_cells[cellIndex];
                if (!haveBaseline)
                {
                    unified = cell;
                    haveBaseline = true;
                    continue;
                }
                if (cell == unified)
                {
                    continue;
                }
                if (itemId != -1 && ((cell == itemId && unified == -1) || (cell == -1 && unified == itemId)))
                {
                    unified = -1;
                    continue;
                }
                return -3;
            }
        }
        return unified;
    }

    void IzvratRepository::MarkCellPieceByValue(int value, BoundsBase<int> const& piece)
    {
        // TODO: generated code
        // Create bounds for the entire grid
        BoundsBase<int> gridBounds(0, 0, m_maxGeomSize.x, m_maxGeomSize.y);

        // Calculate the intersection between the grid and the requested piece
        BoundsBase<int> intersection = gridBounds.Intersect(piece);

        // If no intersection, nothing to do
        if (intersection.width <= 0 || intersection.height <= 0)
            return;

        // Iterate through the intersected area
        for (int x = intersection.x0; x < intersection.x0 + intersection.width; ++x)
        {
            for (int y = intersection.y0; y < intersection.y0 + intersection.height; ++y)
            {
                // Calculate cell index
                int cellIndex = x + y * m_maxGeomSize.x;

                // Bounds check and set value
                if (cellIndex >= 0 && cellIndex < static_cast<int>(m_cells.size()))
                {
                    m_cells[cellIndex] = value;
                }
            }
        }
    }

    void IzvratRepository::MarkCellPieceByValueExcluding(int value, BoundsBase<int> const& piece, int excludingValue)
    {
        // TODO: generated code
        // Create bounds for the entire grid
        BoundsBase<int> gridBounds(0, 0, m_maxGeomSize.x, m_maxGeomSize.y);

        // Calculate the intersection between the grid and the requested piece
        BoundsBase<int> intersection = gridBounds.Intersect(piece);

        // If no intersection, nothing to do
        if (intersection.width <= 0 || intersection.height <= 0)
            return;

        // Iterate through the intersected area
        for (int x = intersection.x0; x < intersection.x0 + intersection.width; ++x)
        {
            for (int y = intersection.y0; y < intersection.y0 + intersection.height; ++y)
            {
                // Calculate cell index
                int cellIndex = x + y * m_maxGeomSize.x;

                // Bounds check
                if (cellIndex >= 0 && cellIndex < static_cast<int>(m_cells.size()))
                {
                    // Set value only if current value is not the excluding value
                    if (m_cells[cellIndex] != excludingValue)
                    {
                        m_cells[cellIndex] = value;
                    }
                }
            }
        }
    }

    bool IzvratRepository::IsValueAnItemId(int cellValue) const
    {
        // RVA 0x6E8F00
        // Non-negative cell values below the slot count are slot indices; -1 (free),
        // -2 (outside the active area) and -3 (mixed) are not.
        if (cellValue < 0)
        {
            return false;
        }
        return cellValue < static_cast<int>(m_slots.size());
    }
}  // namespace ai
