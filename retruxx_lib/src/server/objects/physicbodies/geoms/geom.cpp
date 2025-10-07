#include "geom.h"
#include <stdexcept>
#include <math/aabb.h>
#include <math/quaternion.h>
#include <math/vector.h>
#include <ode/collision.h>

namespace ai
{
    Geom::CellAabb::CellAabb()
    {
        this->x0 = 0;
        this->z0 = 0;
        this->x1 = -1;
        this->z1 = -1;
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
        return m_geomId;
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

    void Geom::RelinkToCollisionCells(int physicObjId)
    {
        auto newAabb = ai::Geom::CountCellAabb();
        if (newAabb.x0 != this->m_curAabb.x0
            || newAabb.z0 != this->m_curAabb.z0
            || newAabb.x1 != this->m_curAabb.x1
            || newAabb.z1 != this->m_curAabb.z1)
        {
            ai::Geom::UnlinkFromCollisionCells(physicObjId);
            ai::Geom::LinkToCollisionCells(physicObjId, &newAabb);
        }
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

    Geom::Geom(dxGeom* const geomId, void(* movedCallback)(dxGeom*))
    {
        m_geomId = geomId;
        m_curAabb.x0 = 0;
        m_curAabb.z0 = 0;
        m_curAabb.x1 = -1;
        m_curAabb.z1 = -1;
        dGeomSetMovedCallback(geomId, movedCallback);
        dGeomSetCategoryBits(geomId, 0xFFFFu);
        dGeomSetCollideBits(geomId, 0xFFFFu);
    }
}
