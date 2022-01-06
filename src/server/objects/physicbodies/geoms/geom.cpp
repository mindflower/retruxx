#include "geom.h"
#include <stdexcept>
#include <math/aabb.h>
#include <math/quaternion.h>
#include <math/vector.h>

namespace ai
{
    Geom::CellAabb::CellAabb()
    {
        throw std::logic_error("Not implemented");
    }

    Quaternion Geom::GetRotation() const
    {
        throw std::logic_error("Not implemented");
    }

    void Geom::SetBody(dxBody* const)
    {
        throw std::logic_error("Not implemented");
    }

    dxSpace* Geom::GetSpace() const
    {
        throw std::logic_error("Not implemented");
    }

    void Geom::UnlinkFromCollisionCells(int)
    {
        throw std::logic_error("Not implemented");
    }

    void Geom::RelinkToSpace(dxSpace*)
    {
        throw std::logic_error("Not implemented");
    }

    void Geom::SetRotation(Quaternion const&)
    {
        throw std::logic_error("Not implemented");
    }

    Geom::~Geom()
    {
        throw std::logic_error("Not implemented");
    }

    dxGeom* Geom::GetGeomId() const
    {
        throw std::logic_error("Not implemented");
    }

    void Geom::Disable()
    {
        throw std::logic_error("Not implemented");
    }

    void Geom::LinkToCollisionCells(int, CellAabb*)
    {
        throw std::logic_error("Not implemented");
    }

    CVector Geom::GetPosition() const
    {
        throw std::logic_error("Not implemented");
    }

    Geom::CellAabb Geom::GetCollisionCellAabb() const
    {
        throw std::logic_error("Not implemented");
    }

    void* Geom::GetData() const
    {
        throw std::logic_error("Not implemented");
    }

    int Geom::GetGeomClass() const
    {
        throw std::logic_error("Not implemented");
    }

    Geom::CellAabb Geom::CountCellAabb() const
    {
        throw std::logic_error("Not implemented");
    }

    Aabb Geom::GetAabb() const
    {
        throw std::logic_error("Not implemented");
    }

    void Geom::UnlinkFromBody()
    {
        throw std::logic_error("Not implemented");
    }

    void Geom::SetData(void*)
    {
        throw std::logic_error("Not implemented");
    }

    void Geom::SetPosition(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Geom::Enable()
    {
        throw std::logic_error("Not implemented");
    }

    void Geom::RelinkToCollisionCells(int)
    {
        throw std::logic_error("Not implemented");
    }

    bool Geom::IsEnabled() const
    {
        throw std::logic_error("Not implemented");
    }

    void Geom::CheckCollisionCells()
    {
        throw std::logic_error("Not implemented");
    }

    void Geom::SetDirection(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Geom::DumpPhysicInfo(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    Geom::Geom(dxGeom* const geomId, void(__fastcall * movedCallback)(dxGeom*))
    {
        throw std::logic_error("Not implemented");
    }
}
