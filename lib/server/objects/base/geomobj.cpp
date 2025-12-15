#include "geomobj.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(GeomObj)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(GeomObj);

    Obj* GeomObjPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GeomObjPrototypeInfo::GeomObjPrototypeInfo() = default;

    GeomObjPrototypeInfo::~GeomObjPrototypeInfo()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool GeomObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return PhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    void GeomObj::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Geom* GeomObj::GetGeom()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomObj::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomObj::RenderDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomObj::UnlinkGeomsFromCollisionCells()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Geom::CellAabb GeomObj::GetCollisionCellAabb() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomObj::EnableGeometry(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomObj::SetGeom(CVector, float, float, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomObj::RelinkGeomsToCollisionCells()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GeomObjPrototypeInfo const* GeomObj::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GeomObj::GeomObj(GeomObjPrototypeInfo const& prototype) : PhysicObj(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* GeomObj::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomObj::TransferToSpace(dxSpace*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* GeomObj::GetBaseClass()
    {
        return RT_CLASS_LOCAL(PhysicObj);
    }

    void GeomObj::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomObj::DisableGeometry(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomObj::LinkGeomsToCollisionCells()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomObj::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomObj::_UnlinkBodyFromGeoms()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomObj::_LinkBodyToGeoms()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomObj::_SetRotationToGeoms(Quaternion const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomObj::_Construct()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomObj::_SetPositionToGeoms(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GeomObj::~GeomObj()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* GeomObj::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* GeomObj::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
