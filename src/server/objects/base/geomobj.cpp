#include "geomobj.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(GeomObj)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(GeomObj);

    Obj* GeomObjPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    GeomObjPrototypeInfo::GeomObjPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    GeomObjPrototypeInfo::~GeomObjPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    bool GeomObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void GeomObj::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    Geom* GeomObj::GetGeom()
    {
        throw std::logic_error("Not implemented");
    }

    void GeomObj::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void GeomObj::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void GeomObj::UnlinkGeomsFromCollisionCells()
    {
        throw std::logic_error("Not implemented");
    }

    Geom::CellAabb GeomObj::GetCollisionCellAabb() const
    {
        throw std::logic_error("Not implemented");
    }

    void GeomObj::EnableGeometry(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void GeomObj::SetGeom(CVector, float, float, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void GeomObj::RelinkGeomsToCollisionCells()
    {
        throw std::logic_error("Not implemented");
    }

    GeomObjPrototypeInfo const* GeomObj::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    GeomObj::GeomObj(GeomObjPrototypeInfo const& prototype) : PhysicObj(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* GeomObj::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void GeomObj::TransferToSpace(dxSpace*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* GeomObj::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    void GeomObj::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void GeomObj::DisableGeometry(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void GeomObj::LinkGeomsToCollisionCells()
    {
        throw std::logic_error("Not implemented");
    }

    void GeomObj::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void GeomObj::_UnlinkBodyFromGeoms()
    {
        throw std::logic_error("Not implemented");
    }

    void GeomObj::_LinkBodyToGeoms()
    {
        throw std::logic_error("Not implemented");
    }

    void GeomObj::_SetRotationToGeoms(Quaternion const&)
    {
        throw std::logic_error("Not implemented");
    }

    void GeomObj::_Construct()
    {
        throw std::logic_error("Not implemented");
    }

    void GeomObj::_SetPositionToGeoms(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    GeomObj::~GeomObj()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* GeomObj::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* GeomObj::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}
