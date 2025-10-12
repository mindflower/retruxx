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
        throw std::logic_error("Not implemented");
    }

    IzvratRepository::~IzvratRepository()
    {
    }

    int IzvratRepository::CanAddThingToPlace(GeomRepositoryItem const&, PointBase<int> const&, int*) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* IzvratRepository::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* IzvratRepository::GetBaseClass()
    {
        return RT_CLASS_LOCAL(GeomRepository);
    }

    bool IzvratRepository::AddThingToPlace(GeomRepositoryItem&, PointBase<int> const&)
    {
        throw std::logic_error("Not implemented");
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

    BoundsBase<int> IzvratRepository::ToMaxSzRelative(BoundsBase<int>) const
    {
        throw std::logic_error("Not implemented");
    }

    PointBase<int> IzvratRepository::ToMaxSzRelative(PointBase<int>) const
    {
        throw std::logic_error("Not implemented");
    }

    int IzvratRepository::SnapPiece(BoundsBase<int> const&)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
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

    IzvratRepository& IzvratRepository::operator=(IzvratRepository const&)
    {
        throw std::logic_error("Not implemented");
    }

    PointBase<int> const& IzvratRepository::GetMaxGeomSize() const
    {
        throw std::logic_error("Not implemented");
    }

    PointBase<int> IzvratRepository::ToGeomSzRelative(PointBase<int>) const
    {
        throw std::logic_error("Not implemented");
    }

    BoundsBase<int> IzvratRepository::ToGeomSzRelative(BoundsBase<int>) const
    {
        throw std::logic_error("Not implemented");
    }

    IzvratRepository::IzvratRepository(IzvratRepository const&)
    {
        throw std::logic_error("Not implemented");
    }

    IzvratRepository::IzvratRepository()
    {
        SetMaxGeomSize(ai::theGlobProp.m_izvratRepositoryMaxSize);
    }

    int IzvratRepository::GetUnifyCellValueFromPiece(BoundsBase<int> const&, int) const
    {
        throw std::logic_error("Not implemented");
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

    bool IzvratRepository::IsValueAnItemId(int) const
    {
        throw std::logic_error("Not implemented");
    }
}
