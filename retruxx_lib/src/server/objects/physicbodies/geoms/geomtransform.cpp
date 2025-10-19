#include "geomtransform.h"

#include <stdexcept>

namespace ai
{
    ai::GeomTransform* GeomTransform::CreateObject(dxSpace* space, void(* movedCalback)(dxGeom*))
    {
        auto geom = dCreateGeomTransform(space);
        return new GeomTransform(geom, movedCalback);
    }

    GeomTransform::~GeomTransform()
    {
        delete m_innerGeom;
    }

    const ai::Geom* GeomTransform::GetGeom() const
    {
        throw std::logic_error("Not implemented");
    }

    ai::Geom* GeomTransform::GetGeom()
    {
        return m_innerGeom;
    }

    GeomTransform::GeomTransform(dxGeom* const geomId, void(*movedCalback)(dxGeom*)) : Geom(geomId, movedCalback)
    {
        this->m_innerGeom = 0;
        dGeomTransformSetCleanup(m_geomId, 0);;
    }

    void GeomTransform::SetGeom(ai::Geom* pGeom)
    {
        delete m_innerGeom;
        m_innerGeom = nullptr;
        dGeomTransformSetGeom(m_geomId, pGeom->GetGeomId());
        m_innerGeom = pGeom;
    }
}
