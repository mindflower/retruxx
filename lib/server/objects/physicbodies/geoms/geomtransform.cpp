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
        // RVA 0x5CE5E0
        return m_innerGeom;
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
