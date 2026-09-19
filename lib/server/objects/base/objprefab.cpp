#include "objprefab.h"

#include <stdexcept>
#include <server/utils.h>
#include "server/objects/vehicle.h"
#include "server/objects/team.h"
#include "prototypemanager.h"
#include "objcontainer.h"
#include "server/objects/dummyobject.h"

#include <core/ini.h>
#include <core/kernel.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <math/matrix.h>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(ObjPrefab)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(ObjPrefab);

    ObjPrefabPrototypeInfo::ObjInfo::ObjInfo()
    {
        // RVA 0x8428B0
        m_prototypeId = -1;
        m_relPos = ZeroVector;
        m_relRot = IdentityQuaternion;
        m_scale = 1.0f;
    }

    ObjPrefabPrototypeInfo::ObjInfo::ObjInfo(ObjPrefabPrototypeInfo::ObjInfo const& that)
    {
        // RVA 0x8413B0 - plain memberwise copy.
        m_prototypeId = that.m_prototypeId;
        m_relPos = that.m_relPos;
        m_relRot = that.m_relRot;
        m_scale = that.m_scale;
        m_modelName = that.m_modelName;
        m_prototypeName = that.m_prototypeName;
    }

    void ObjPrefabPrototypeInfo::ObjInfo::PostLoad()
    {
        // RVA 0x842410 - prototypes are resolved by name only once everything is loaded.
        m_prototypeId = thePrototypeManager->GetPrototypeId(m_prototypeName);
    }

    ObjPrefabPrototypeInfo::ObjPrefabPrototypeInfo()
    {
        m_bIsUpdating = false;
    }

    void ObjPrefabPrototypeInfo::PostLoad()
    {
        // RVA 0x8433D0
        // NOTE: resolves each entry inline rather than calling ObjInfo::PostLoad, which does
        // exactly the same thing and is left without a caller.
        for (auto& objInfo : m_objInfos)
        {
            objInfo.m_prototypeId = thePrototypeManager->GetPrototypeId(objInfo.m_prototypeName);
        }
    }

    ai::Obj* ObjPrefabPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x845F20
        return new ObjPrefab(*this);
    }

    bool ObjPrefabPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = ai::SimplePhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            // RVA 0x846010
            _SetGeomType(GEOM_TYPE_BOX);
            m_objInfos.clear();

            ref_ptr objInfosNode = xmlFile->CreateNode();
            xmlNode->GetFirstChild(objInfosNode, "ObjInfos");
            if (!xmlNode->IsEmpty())
            {
                ref_ptr objInfoNode = xmlFile->CreateNode();
                for (objInfosNode->GetFirstChild(objInfoNode, "ObjInfo"); !objInfoNode->IsEmpty();
                     objInfoNode->GetNextSibling(objInfoNode, "ObjInfo"))
                {
                    ObjInfo objInfo;
                    m3d::SafeStrAttrib(objInfo.m_prototypeName, objInfoNode, "Prototype");
                    // An entry without a prototype names nothing to create, so it is dropped.
                    if (objInfo.m_prototypeName.empty())
                        continue;

                    m3d::SafeVectorAttrib(objInfo.m_relPos, objInfoNode, "RelPos");
                    m3d::SafeQuaternionAttrib(objInfo.m_relRot, objInfoNode, "RelRot");
                    m3d::SafeStrAttrib(objInfo.m_modelName, objInfoNode, "ModelName");
                    m3d::SafeFloatAttrib(objInfo.m_scale, objInfoNode, "Scale");

                    m_objInfos.push_back(objInfo);
                }
            }
        }
        return result;
    }

    ai::ObjPrefabPrototypeInfo const* ObjPrefab::GetPrototypeInfo() const
    {
        // RVA 0x843A90
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), ObjPrefabPrototypeInfo const);
    }

    m3d::Class* ObjPrefab::GetClass() const
    {
        // RVA 0x842400
        return RT_CLASS_LOCAL(ObjPrefab);
    }

    bool ObjPrefab::RemoveChild(ai::Obj* pChild)
    {
        // RVA 0x844F70 - the prefab keeps its children in three places depending on what they
        // are, so the removal has to find the right one.
        Obj::RemoveChild(pChild);
        if (!pChild)
            return false;

        if (IS_KIND_OF(pChild, PhysicObj))
        {
            if (!m_physicObjs.erase(static_cast<PhysicObj*>(pChild)))
                return false;
        }
        else if (IS_KIND_OF(pChild, Team))
        {
            if (m_team != pChild)
                return false;

            m_team = nullptr;
            pChild->m_parentId = -1;
            return true;
        }
        else if (!m_otherChildren.erase(pChild))
        {
            return false;
        }

        pChild->m_parentId = -1;
        return true;
    }

    void ObjPrefab::SetPosition(CVector const& pos)
    {
        // RVA 0x843BB0 - the prefab drags its children along by whatever it actually moved,
        // which is not the requested position: SetPositionSelf drops it onto the ground first.
        CVector const oldPos = GetPosition();
        PhysicObj::SetPosition(pos);
        CVector const shift = GetPosition() - oldPos;

        for (auto* physicObj : m_physicObjs)
        {
            physicObj->SetPosition(physicObj->GetPosition() + shift);
        }

        if (m_team)
        {
            for (auto* vehicle : m_team->GetVehicles())
            {
                vehicle->SetPosition(vehicle->GetPosition() + shift);
                vehicle->SetGamePositionOnGround(vehicle->GetPosition(), true, false);
            }
        }
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
        // RVA 0x845020 - spawns the prototype's fixed set of objects and places each one at
        // its recorded offset from the prefab.
        Obj::CreateChildren();

        if (!m_team)
        {
            AddTeam();
            if (!m_team)
            {
                M3D_LOG_ERR("Error: couldn't create team for " + GetDebugDescription());
            }
        }

        auto const& objInfos = GetPrototypeInfo()->m_objInfos;
        CVector const pos = GetPosition();
        Quaternion const rot = GetRotation();

        int childNum = 1;
        for (auto const& objInfo : objInfos)
        {
            int const objId = theObjects->CreateNewObject(
                objInfo.m_prototypeId, (m_name + CStr("_Child") + CStr(childNum)).c_str(), -1, -1);
            ++childNum;

            auto* obj = theObjects->GetEntityByObjId(objId);
            if (!obj)
                continue;

            // Vehicles are owned by the team; everything else hangs off the prefab itself.
            if (IS_KIND_OF(obj, Vehicle))
            {
                if (!m_team)
                {
                    m_VehiclesForAdd.push_back(obj->GetId());
                }
                else
                {
                    m_team->AddChild(obj);
                }
            }
            else
            {
                AddChild(obj);
            }

            if (IS_KIND_OF(obj, PhysicObj))
            {
                auto* physicObj = static_cast<PhysicObj*>(obj);

                Quaternion childRot = rot;
                childRot *= objInfo.m_relRot;
                physicObj->SetRotation(childRot);

                CVector const childPos = pos + rot.vecRot(objInfo.m_relPos);
                if (IS_KIND_OF(obj, Vehicle))
                {
                    static_cast<Vehicle*>(obj)->SetGamePositionOnGround(childPos, true, false);
                }
                else
                {
                    physicObj->SetPosition(childPos);
                }
            }

            if (IS_KIND_OF(obj, DummyObject) && !objInfo.m_modelName.empty())
            {
                static_cast<DummyObject*>(obj)->SetModelName(objInfo.m_modelName.c_str());
            }

            // NOTE: the scale is applied only when a model name was given - the guard tests
            // m_modelName, not m_scale, so a scaled entry without a model keeps scale 1.
            if (IS_KIND_OF(obj, SimplePhysicObj) && !objInfo.m_modelName.empty())
            {
                static_cast<SimplePhysicObj*>(obj)->SetScale(objInfo.m_scale, true);
            }
        }
    }

    bool ObjPrefab::CanChildBeAdded(m3d::Class*) const
    {
        return true;
    }

    void ObjPrefab::SetPositionSelf(CVector const& pos)
    {
        PhysicObj::SetPositionSelf(ai::GetGroundPos(pos, 0, 0));
    }

    void ObjPrefab::SetRotation(Quaternion const& rot)
    {
        // RVA 0x843DA0 - turns the whole prefab about its own origin: each child is swung
        // round by however far the prefab itself turned.
        Quaternion rotForChildren = rot;
        rotForChildren *= GetRotation().getInversed();

        SetRotationSelf(rot);

        for (auto* physicObj : m_physicObjs)
        {
            CVector const relPos = physicObj->GetPosition() - GetPosition();
            physicObj->SetPosition(GetPosition() + rotForChildren.vecRot(relPos));

            Quaternion childRot = rotForChildren;
            childRot *= physicObj->GetRotation();
            physicObj->SetRotation(childRot);
        }

        if (m_team)
        {
            for (auto* vehicle : m_team->GetVehicles())
            {
                CVector const relPos = vehicle->GetPosition() - GetPosition();
                vehicle->SetPosition(GetPosition() + rotForChildren.vecRot(relPos));

                Quaternion childRot = rotForChildren;
                childRot *= vehicle->GetRotation();
                vehicle->SetRotation(childRot);

                vehicle->SetGamePositionOnGround(vehicle->GetPosition(), true, false);
            }
        }
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
        // RVA 0x844AA0
        SimplePhysicObj::_InternalPostLoad();

        if (!m_team)
        {
            AddTeam();
            if (!m_team)
            {
                M3D_LOG_ERR("Error: couldn't create team for " + GetDebugDescription());
            }
        }

        // Vehicles that arrived before the team existed were parked here; hand them over now.
        if (m_team)
        {
            for (auto const vehicleId : m_VehiclesForAdd)
            {
                m_team->AddChild(theObjects->GetEntityByObjId(vehicleId));
            }
            m_VehiclesForAdd.clear();
        }
    }

    ObjPrefab::~ObjPrefab() = default;

    m3d::Object* ObjPrefab::CreateObject()
    {
        // RVA 0x842F20
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    void ObjPrefab::AddTeam()
    {
        // RVA 0x844650 - every prefab owns a team, so that the vehicles it spawns fight as one.
        int const prototypeId = thePrototypeManager->GetPrototypeId(CStr("settlementTeam"));
        int const teamId = theObjects->CreateNewObject(prototypeId, (m_name + CStr("_Team")).c_str(), -1, GetBelong());
        AddChild(theObjects->GetEntityByObjId(teamId));
    }

    void ObjPrefab::AddVehicleChild(Vehicle* obj)
    {
        // RVA 0x844C30 - a vehicle belongs to the team, not the prefab; if the team is not
        // built yet it waits until _InternalPostLoad.
        if (m_team)
        {
            m_team->AddChild(obj);
        }
        else
        {
            m_VehiclesForAdd.push_back(obj->GetId());
        }
    }

    m3d::Object* ObjPrefab::Clone()
    {
        // RVA 0x842D60
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai
