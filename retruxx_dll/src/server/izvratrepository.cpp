#include "izvratrepository.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(IzvratRepository)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(IzvratRepository);

    void IzvratRepository::Purge()
    {
        throw std::logic_error("Not implemented");
    }

    std::vector<int, std::allocator<int>> const& IzvratRepository::GetCells() const
    {
        throw std::logic_error("Not implemented");
    }

    IzvratRepository::~IzvratRepository()
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    bool IzvratRepository::SetGeomSize(PointBase<int> const&)
    {
        throw std::logic_error("Not implemented");
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

    void IzvratRepository::SetMaxGeomSize(PointBase<int> const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* IzvratRepository::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void IzvratRepository::Clear(bool)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    int IzvratRepository::GetUnifyCellValueFromPiece(BoundsBase<int> const&, int) const
    {
        throw std::logic_error("Not implemented");
    }

    void IzvratRepository::MarkCellPieceByValue(int, BoundsBase<int> const&)
    {
        throw std::logic_error("Not implemented");
    }

    void IzvratRepository::MarkCellPieceByValueExcluding(int, BoundsBase<int> const&, int)
    {
        throw std::logic_error("Not implemented");
    }

    bool IzvratRepository::IsValueAnItemId(int) const
    {
        throw std::logic_error("Not implemented");
    }
}
