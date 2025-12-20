#include "objprefab.h"

#include <stdexcept>
#include <server/utils.h>
#include "server/objects/vehicle.h"
#include "server/objects/team.h"

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
        SimplePhysicObj::Remove();

        for (auto& obj : m_physicObjs)
        {
            obj->Remove();
        }

        for (auto& obj : m_otherChildren)
        {
            obj->Remove();
        }

        if (m_team)
            m_team->Remove();
    }

    void ObjPrefab::CreateChildren()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool ObjPrefab::CanChildBeAdded(m3d::Class*) const
    {
        return true;
    }

    void ObjPrefab::SetPositionSelf(CVector const& pos)
    {
        PhysicObj::SetPositionSelf(ai::GetGroundPos(pos, 0, 0));
    }

    void ObjPrefab::SetRotation(Quaternion const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ObjPrefab::AddChild(ai::Obj* pObj)
    {
        // TODO: check and refactor
        Obj::AddChild(pObj);
        if (pObj)
        {
            if (pObj->IsKindOf(&ai::Vehicle::m_classVehicle))
            {
                if (m_team)
                {
                    m_team->AddChild(pObj);
                }
                else
                {
                    m_VehiclesForAdd.push_back(pObj->GetId());
                }
                getAllChildren().erase(pObj->GetId());
            }
            else
            {
                if (pObj->IsKindOf(&ai::Team::m_classTeam))
                {
                    m_team = (Team*)pObj;
                }
                else if (pObj->IsKindOf(&ai::PhysicObj::m_classPhysicObj))
                {
                    m_physicObjs.insert((PhysicObj*)pObj);
                }
                else
                {
                    m_otherChildren.insert(pObj);
                }
                pObj->LinkToParent(GetId(), HIERARCHY_CHILD);
            }
        }
    }

    m3d::Class* ObjPrefab::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    void ObjPrefab::_InternalPostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ObjPrefab::~ObjPrefab() = default;

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
