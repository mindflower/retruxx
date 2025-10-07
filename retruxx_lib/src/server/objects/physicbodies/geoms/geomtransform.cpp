#include "geomtransform.h"

#include <stdexcept>

namespace ai
{
    ai::GeomTransform* GeomTransform::CreateObject(dxSpace* space, void(* movedCalback)(dxGeom*))
    {
        throw std::logic_error("Not implemented");
    }

    GeomTransform::~GeomTransform()
    {
        throw std::logic_error("Not implemented");
    }

    const ai::Geom* GeomTransform::GetGeom() const
    {
        throw std::logic_error("Not implemented");
    }

    ai::Geom* GeomTransform::GetGeom()
    {
        throw std::logic_error("Not implemented");
    }

    GeomTransform::GeomTransform(dxGeom* const geomId, void(*movedCalback)(dxGeom*)) : Geom(geomId, movedCalback)
    {
        throw std::logic_error("Not implemented");
    }

    void GeomTransform::SetGeom(ai::Geom* pGeom)
    {
        throw std::logic_error("Not implemented");
    }
}
