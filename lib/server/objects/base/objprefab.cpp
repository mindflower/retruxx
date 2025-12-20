#include "objprefab.h"

#include <stdexcept>
#include <server/utils.h>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(ObjPrefab)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(ObjPrefab);

    ObjPrefabPrototypeInfo::ObjInfo::ObjInfo()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjPrefabPrototypeInfo::ObjInfo::PostLoad()
    {
        // TODO: implement ::PostLoad
    }

    ObjPrefabPrototypeInfo::ObjPrefabPrototypeInfo()
    {
        m_bIsUpdating = false;
    }

    void ObjPrefabPrototypeInfo::PostLoad()
    {
        // TODO: implement ::PostLoad
    }

    ai::Obj* ObjPrefabPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool ObjPrefabPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = ai::SimplePhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            _SetGeomType(GEOM_TYPE_BOX);
            // TODO: implement ObjPrefabPrototypeInfo::LoadFromXML
        }
        return result;
    }

    ai::ObjPrefabPrototypeInfo const* ObjPrefab::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* ObjPrefab::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool ObjPrefab::RemoveChild(ai::Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjPrefab::SetPosition(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ObjPrefab::ObjPrefab(ai::ObjPrefabPrototypeInfo const& prototype) : SimplePhysicObj(prototype)
    {
        m_team = 0;
        DisablePhysics();
        DisableGeometry(1);
    }

    void ObjPrefab::Remove()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjPrefab::CreateChildren()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool ObjPrefab::CanChildBeAdded(m3d::Class*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjPrefab::SetPositionSelf(CVector const& pos)
    {
        PhysicObj::SetPositionSelf(ai::GetGroundPos(pos, 0, 0));
    }

    void ObjPrefab::SetRotation(Quaternion const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjPrefab::AddChild(ai::Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* ObjPrefab::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    void ObjPrefab::_InternalPostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ObjPrefab::~ObjPrefab()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* ObjPrefab::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjPrefab::AddTeam()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjPrefab::AddVehicleChild(Vehicle*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* ObjPrefab::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
