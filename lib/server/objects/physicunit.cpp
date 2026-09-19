#include "physicunit.h"
#include "scene/servers/dataserver.h"
#include "game/m3dgame.h"
#include "core/log.h"
#include "base/prototypemanager.h"

#include <ode/objects.h>
#include "ode/odecpp.h"
#include "physicbodies/geoms/box.h"
#include <server/utils.h>
#include <server/server.h>
#include <server/externalpaths.h>
#include <server/objects/base/jointedobj.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/player.h>
#include <server/statistic/intstatistic.h>
#include <server/statistic/statisticmanager.h>
#include <server/objects/physicbodies/simplephysicbody.h>

#include <core/ini.h>
#include <core/kernel.h>
#include <core/timer.h>
#include <scene/nodes/sgnode.h>
#include <server/objects/vehicle.h>
#include <level.h>
#include <world.h>

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicUnit, AddWalkPathByName)
{
    auto* physicUnit = (ai::PhysicUnit*)context->asObject(0, "PhysicUnit");
    // RVA 0x80D520
    context->pushBool(physicUnit->AddWalkPathByName(context->asString(1)));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicUnit, SetWalkPathByName)
{
    auto* physicUnit = (ai::PhysicUnit*)context->asObject(0, "PhysicUnit");
    // RVA 0x80D560
    context->pushBool(physicUnit->SetWalkPathByName(context->asString(1)));
    return 1;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(PhysicUnit)
    RT_CLASS_EXPORT(PhysicUnit, m3d::METHOD, AddWalkPathByName, "", "", "")
    RT_CLASS_EXPORT(PhysicUnit, m3d::METHOD, SetWalkPathByName, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(PhysicUnit);

    bool PhysicUnitPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = SimplePhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            _SetGeomType(GEOM_TYPE_FROM_MODEL);
            m3d::SafeFloatAttrib(m_walkSpeed, xmlNode, "WalkSpeed");
            m3d::SafeFloatAttrib(m_maxStandTime, xmlNode, "MaxStandTime");
            m3d::SafeFloatAttrib(m_turnSpeed, xmlNode, "TurnSpeed");
            m_turnSpeed *= 0.017453292;
        }
        return result;
    }

    Obj* PhysicUnitPrototypeInfo::CreateTargetObject() const
    {
        return new PhysicUnit(*this);
    }

    PhysicUnitPrototypeInfo::PhysicUnitPrototypeInfo()
    {
        m_walkSpeed = 1.0f;
        m_turnSpeed = 1.0f;
        m_maxStandTime = 1.0f;
    }

    void PhysicUnit::SetCauseForce(float force)
    {
        // RVA 0x7DEFF0
        m_causeForce = force;
    }

    void PhysicUnit::OnCollideWithStandingVehicle()
    {
        // RVA 0x80AAD0 - a walking unit that bumps into a stopped vehicle stops and stands for a random time.
        if (m_walkState != STAND)
        {
            m_walkState = STAND;
            m_standTtl.value().set(CVector2(0.0f, m_standTtl.maxValue().get()).randomValue());
            m_physicBody->SetNodeAction(0, true);
        }
    }

    eGObjPropertySaveStatus PhysicUnit::GetPropertySaveStatus(int id) const
    {
        // RVA 0x80D820
        auto const it = m_propertiesSaveStatesMap.find(id);
        return it != m_propertiesSaveStatesMap.end() ? it->second : SimplePhysicObj::GetPropertySaveStatus(id);
    }

    PhysicUnit::PhysicUnit(PhysicUnitPrototypeInfo const& prototype) :
        SimplePhysicObj(prototype),
        m_standTtl(0.0, 0.0, m_maxStandTime, -1.0),
        m_walkTtl(0.0, 0.0, 1.0, 0.0)
    {
        m_walkSpeed = prototype.m_walkSpeed;
        m_turnSpeed = prototype.m_turnSpeed;
        m_maxStandTime = prototype.m_maxStandTime;
        m_State = LIVE;
        m_causePos = ZeroVector;
        m_causeForce = 0.0f;
        m_initVelocities = false;
        m_curWayPointNum = 0u;
        m_prevWayPoint = ZeroVector;
        m_walkState = STAND;
        m_curPath = nullptr;
        m_bMustChangePath = false;
        m_bMustWalk = true;

        auto& serverAnimatedModels = M3D_APP->GetAnimatedModelsServer();
        int const itemByName = serverAnimatedModels.GetItemByName(prototype.GetEngineModelName().c_str(), true);

        m3d::AnimatedModel* mdl = nullptr;
        serverAnimatedModels.GetItemProperty(itemByName, m3d::PROP_INTERNAL_GETMODEL, &mdl);
        if (mdl)
        {
            // TODO: check this
            auto* geom = mdl->GetGeom(0);
            if (geom && geom->Type == m3d::DRAFT_GeomType::BOX)
            {
                m_physicBody->_ClearGeoms();

                CVector const size(geom->Sizes.BoxSizes.x, geom->Sizes.BoxSizes.y, geom->Sizes.BoxSizes.z);
                auto* box = ai::Box::CreateObject(0, size, 0);
                dGeomSetPosition(box->GetGeomId(), geom->Translation.x, geom->Translation.y, geom->Translation.z);

                float quat[4] = {};
                quat[0] = geom->Rotation.w;
                quat[1] = geom->Rotation.x;
                quat[2] = geom->Rotation.y;
                quat[3] = geom->Rotation.z;
                dGeomSetQuaternion(box->GetGeomId(), quat);

                auto* object = ai::GeomTransform::CreateObject(m_spaceId, CommonGeomMovedCallback);
                dGeomSetData(object->GetGeomId(), m_physicBody);
                object->SetGeom(box);
                m_physicBody->m_pGeoms.push_back(object);
                dGeomSetBody(m_physicBody->m_pGeoms.front()->GetGeomId(), m_body->id());
                dMassSetBoxTotal(&m_physicBody->m_mass, prototype.GetMassValue(), size.x, size.y, size.z);
            }
        }
        else
        {
            M3D_LOG_ERR("Error: no model for PhysicUnit: '" + prototype.GetEngineModelName() + "' is missing");
        }
        DisablePhysics();
    }

    CStr PhysicUnit::GetPropertyName(int id) const
    {
        // RVA 0x80D9D0
        for (auto const& [name, propId] : m_propertiesMap)
        {
            if (propId == id)
            {
                return name;
            }
        }
        return SimplePhysicObj::GetPropertyName(id);
    }

    void PhysicUnit::SetState(States newState)
    {
        // RVA 0x80AD00 - dying swaps the animated unit for a RagDoll object that inherits its
        // model, pose and mass, and the unit itself is removed.
        if (newState == m_State)
            return;

        m_State = newState;
        if (newState != DEAD || !m_physicBody->m_Node)
            return;

        // Only kills the player is credited with are counted.
        auto* playerVehicle = thePlayer ? thePlayer->GetVehicle() : nullptr;
        if (playerVehicle && GetLastDamageSource() == playerVehicle->GetId())
        {
            auto* globalStatistic =
                static_cast<IntStatistic*>(theStatisticManager->GetStatistic(STATISTIC_PEOPLES_KILLED, "IntStatistic"));
            globalStatistic->m_bGlobalFlag = true;
            globalStatistic->Increase(1);

            auto* levelStatistic = static_cast<IntStatistic*>(theStatisticManager->GetStatistic(
                STATISTIC_PEOPLES_KILLED + pServer->GetWorld()->m_level->m_levelName, CStr("IntStatistic")));
            levelStatistic->m_bGlobalFlag = false;
            levelStatistic->Increase(1);
        }

        SetPostDisablePhysics();

        int const ragDollPrototypeId = theObjects->GetPrototypeId("RagDoll");
        int const ragDollObjId = theObjects->CreateNewObject(ragDollPrototypeId, "", -1, -1);
        auto* ragDoll = static_cast<JointedObj*>(theObjects->GetEntityByObjId(ragDollObjId));

        auto* node = m_physicBody->m_Node;
        CStr const modelName = node->GetServer()->GetNameByItem(node->GetServerHandle());
        CVector const pos = node->GetOriginWorldAbs();
        Quaternion const rot = node->GetRotation();
        ragDoll->Init(modelName, pos, rot, GetMass(), m_physicBody->m_Node, 1.0f);
        theObjects->AddObjToPostCollideList(ragDoll);

        // The node now belongs to the ragdoll, so the unit drops its reference before removing
        // itself - otherwise the model would be destroyed along with it.
        m_physicBody->m_Node = nullptr;

        if (m_initVelocities)
        {
            ragDoll->InitImpulses(m_causePos, m_causeForce);
        }
        ragDoll->SetDeadTimer(60000, true);
        Remove();
    }

    void PhysicUnit::Registration()
    {
        m_propertiesMap["PathsNames"] = 77;
    }

    void PhysicUnit::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        // RVA 0x80D950
        for (auto const& prop : m_propertiesMap)
        {
            Props.insert(prop.second);
        }
        SimplePhysicObj::GetPropertiesIDs(Props);
    }

    void PhysicUnit::RenderDebugInfo() const
    {
        // RVA 0x80A850 - a unit with no visual node at all still draws; one that has a node
        // only draws on the frames that node was rendered in.
        if (m_physicBody->m_Node && m_physicBody->m_Node->m_frameVisible != M3D_KERNEL->GetTimer().GetCurFrame())
            return;

        SimplePhysicObj::RenderDebugInfo();

        // Lifted clear of the model so the lines are not buried inside it.
        CVector pos = GetPosition();
        pos.y += 5.0f;

        if (m_walkState == TURN)
        {
            CVector const& wayPoint = (*m_curPath)[m_curWayPointNum];
            CVector const leg = wayPoint - m_prevWayPoint;
            float const invLegLen =
                1.0f / sqrt(leg.z * leg.z + leg.y * leg.y + leg.x * leg.x + 0.00000011920929f);

            CVector const dir = GetDirection();

            // NOTE: the desired heading keeps only x and z from the leg and borrows y from the
            // direction the unit is currently facing, so the red line is not the leg direction.
            float const needX = invLegLen * leg.x;
            float const needZ = leg.z * invLegLen;
            float const invNeedLen = 1.0f / sqrt(needZ * needZ + needX * needX + dir.y * dir.y + 0.00000011920929f);

            CVector const to(pos.x + needX * invNeedLen * 10.0f, pos.y + invNeedLen * dir.y * 10.0f,
                pos.z + needZ * invNeedLen * 10.0f);
            M3D_APP->DrawLine(pos, to, 0xFFFF0000);

            M3D_APP->DrawLine(pos, pos + dir * 10.0f, 0xFF00FF00);
        }
        else if (m_walkState == WALK)
        {
            CVector wp = (*m_curPath)[m_curWayPointNum];
            wp.y = pos.y;
            M3D_APP->DrawLine(pos, wp, 0xFF0000FF);
        }
    }

    m3d::Class* PhysicUnit::GetClass() const
    {
        return RT_CLASS_LOCAL(PhysicUnit);
    }

    void PhysicUnit::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x8098F0
        // NOTE: chains straight to Obj, skipping SimplePhysicObj, so none of the physics setup
        // is written out - a unit is rebuilt from its prototype on load. LoadFromXML above
        // skips the same way.
        Obj::SaveToXML(xmlFile, xmlNode);
    }

    void PhysicUnit::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x80A450
        SimplePhysicObj::SaveRuntimeValues(xmlFile, xmlNode);

        xmlNode->SetAttribute("State", CStr(static_cast<int>(m_State)).c_str());
        xmlNode->SetAttribute("CurWayPointNum", CStr(m_curWayPointNum).c_str());
        xmlNode->SetAttribute("CurPathName", m_curPathName.c_str());
        xmlNode->SetAttribute("WalkState", CStr(static_cast<int>(m_walkState)).c_str());
        xmlNode->SetAttribute("StandTtl", CStr(m_standTtl.value().get()).c_str());
        xmlNode->SetAttribute("WalkTtl", CStr(m_walkTtl.value().get()).c_str());
        xmlNode->SetAttribute("MustWalk", CStr(static_cast<int>(m_bMustWalk)).c_str());
        xmlNode->SetAttribute("MustChangePath", CStr(static_cast<int>(m_bMustChangePath)).c_str());
        xmlNode->SetAttribute("PrevWayPoint", CStr(m_prevWayPoint).c_str());
    }

    int PhysicUnit::GetPropertyId(char const* propName) const
    {
        auto it = PhysicUnit::m_propertiesMap.find(propName);
        if (it != PhysicUnit::m_propertiesMap.end())
        {
            return it->second;
        }

        return SimplePhysicObj::GetPropertyId(propName);
    }

    void PhysicUnit::SetCausePos(CVector const& pos)
    {
        // RVA 0x7DEFD0
        m_causePos = pos;
    }

    bool PhysicUnit::SetWalkPathByName(char const* pathName)
    {
        // RVA 0x80D140
        if (!pathName || !*pathName)
        {
            M3D_LOG_ERR("Error: path name is empty for " + GetDebugDescription());
            m_curPathName = CStr("");
            m_curPath = nullptr;
            return false;
        }

        auto it = m_pathsMap.find(CStr(pathName));
        if (it == m_pathsMap.end())
        {
            // Not registered yet, so pull it out of the level's external paths now.
            if (!AddWalkPathByName(pathName))
            {
                M3D_CRITICAL_ERROR("path name '" + CStr(pathName) + "' is not found for " + GetDebugDescription());
            }
            it = m_pathsMap.find(CStr(pathName));
        }

        m_curPathName = it->first;
        m_curPath = &it->second;
        m_curWayPointNum = 0;
        return true;
    }

    void PhysicUnit::SetPositionSelf(CVector const& pos)
    {
        PhysicObj::SetPositionSelf(GetGroundPos(pos, 1, 0));
    }

    void PhysicUnit::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        Obj::LoadFromXML(xmlFile, xmlNode);
    }

    bool PhysicUnit::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        if (propertyId != 77)
        {
            return SimplePhysicObj::SetPropertyById(propertyId, newValue);
        }

        m_pathsMap.clear();

        CStr const pathsNames = newValue.GetAsStr();
        retruxx::vector<CStr> names;
        m3d::Tokenize(pathsNames, names, "(), ;\t");

        bool res = true;
        if (!names.empty())
        {
            for (CStr const& name : names)
            {
                res = res && AddWalkPathByName(name.c_str());
            }
            m_curPathName = names[(names.size() * rand()) >> 15];
            res = res && SetWalkPathByName(m_curPathName.c_str());
        }

        return res;
    }

    void PhysicUnit::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x80DA40
        SimplePhysicObj::LoadRuntimeValues(xmlFile, xmlNode);

        m3d::SafeStrAttrib(m_curPathName, xmlNode, "CurPathName");
        // Everything below describes a position along that path, so without it there is
        // nothing left worth restoring.
        if (!SetWalkPathByName(m_curPathName.c_str()))
            return;

        m3d::SafeEnumAttrib(m_State, xmlNode, "State");

        int curWayPointNum = 0;
        if (m3d::SafeIntAttrib(curWayPointNum, xmlNode, "CurWayPointNum") && curWayPointNum >= 0)
        {
            m_curWayPointNum = curWayPointNum;
        }

        m3d::SafeEnumAttrib(m_walkState, xmlNode, "WalkState");
        m3d::SafeVectorAttrib(m_prevWayPoint, xmlNode, "PrevWayPoint");
        m3d::SafeBoolAttrib(m_bMustChangePath, xmlNode, "MustChangePath");
        m3d::SafeBoolAttrib(m_bMustWalk, xmlNode, "MustWalk");

        float standTtl = 0.0f;
        m3d::SafeFloatAttrib(standTtl, xmlNode, "StandTtl");
        m_standTtl.value().set(standTtl);

        // The walk timer regenerates at the rate that carries the unit over the leg it is
        // currently on, so it has to be recomputed from the restored waypoint.
        m_curPath = &m_pathsMap[m_curPathName];
        CVector const& wayPoint = (*m_curPath)[m_curWayPointNum];
        float const dx = wayPoint.x - m_prevWayPoint.x;
        float const dz = wayPoint.z - m_prevWayPoint.z;
        float const legLength = sqrt(dz * dz + dx * dx);
        m_walkTtl.regeneration().set(legLength > 0.001 ? m_walkSpeed / legLength : 1.0f);

        float walkTtl = 0.0f;
        m3d::SafeFloatAttrib(walkTtl, xmlNode, "WalkTtl");
        m_walkTtl.value().set(walkTtl);
    }

    PhysicUnitPrototypeInfo const* PhysicUnit::GetPrototypeInfo() const
    {
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), PhysicUnitPrototypeInfo const);
    }

    bool PhysicUnit::AddWalkPathByName(char const* name)
    {
        // TODO: check this
        CStr const nameStr = name;
        auto it = m_pathsMap.find(nameStr);
        if (it != m_pathsMap.end())
        {
            M3D_LOG_ERR("Error: duplicate path name '" + nameStr + "' is not added in PhysicUnit " + m_name);
            return false;
        }

        auto const& path = ai::pServer->GetExternalPaths()->GetPath(name);
        if (path.empty())
        {
            M3D_LOG_ERR("Error: empty path '" + nameStr + "' is not added in  " + GetDebugDescription());
            return false;
        }

        auto& resultPath = m_pathsMap[name];
        resultPath.reserve(path.size());
        for (CVector2 const& pathElem : path)
        {
            resultPath.push_back(CVector(pathElem.x, 0.0, pathElem.y));
        }
        return true;
    }

    m3d::Class* PhysicUnit::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    void PhysicUnit::SetInitVelocities(bool initVelocities)
    {
        // RVA 0x7DF010
        m_initVelocities = initVelocities;
    }

    void PhysicUnit::Update(float, unsigned)
    {
        // TODO: implement PhysicUnit::Update
        // RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicUnit::SetDirection(CVector const& direction)
    {
        // RVA 0x809D70 - a unit stays upright, so only the horizontal part of the direction is
        // kept and renormalised; the epsilon keeps a zero vector from dividing by zero.
        float const invLen = 1.0f / sqrt(direction.z * direction.z + direction.x * direction.x + 0.00000011920929f);
        PhysicObj::SetDirection(CVector(invLen * direction.x, 0.0f, direction.z * invLen));
    }

    void PhysicUnit::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
    {
        // RVA 0x80D8D0
        for (auto const& prop : m_propertiesMap)
        {
            Props.insert(prop.first);
        }
        SimplePhysicObj::GetPropertiesNames(Props);
    }

    void PhysicUnit::RegisterProperty(char const* Name, int id, eGObjPropertySaveStatus saveStatus)
    {
        // RVA 0x80B130
        m_propertiesMap[CStr(Name)] = id;
        if (saveStatus)
        {
            m_propertiesSaveStatesMap[id] = saveStatus;
        }
    }

    bool PhysicUnit::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x80B6A0 - "PathsNames" reads back as the space separated list of walk paths.
        if (propertyId != 77)
        {
            return SimplePhysicObj::_GetPropertyInternal(propertyId, retVal);
        }

        CStr value("");
        for (auto const& path : m_pathsMap)
        {
            if (!path.first.empty())
            {
                value += path.first + CStr(" ");
            }
        }

        // Drop the separator the last name left behind.
        if (!value.empty())
        {
            value.del(value.length() - 1, 1);
        }

        retVal = value;
        return true;
    }

    PhysicUnit::~PhysicUnit() = default;

    bool PhysicUnit::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x80B850 - a unit starts out with no walk paths at all.
        if (propertyId != 77)
        {
            return SimplePhysicObj::_GetPropertyDefaultInternal(propertyId, retVal);
        }

        retVal = CStr("");
        return true;
    }

    void PhysicUnit::_SetWalkState(WalkState newWalkState)
    {
        // RVA 0x80A650
        if (m_walkState == newWalkState)
            return;

        m_walkState = newWalkState;
        if (newWalkState == STAND)
        {
            // Stand for a random slice of the maximum, so a crowd does not move in lockstep.
            m_standTtl.value().set(CVector2(0.0f, m_standTtl.maxValue().get()).randomValue());
            m_physicBody->SetNodeAction(0, true);
        }
        else
        {
            // NOTE: TURN and WALK share one animation - there is no separate turn action.
            m_physicBody->SetNodeAction(2, true);
        }
    }

    m3d::Object* PhysicUnit::Clone()
    {
        // RVA 0x80A0D0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* PhysicUnit::CreateObject()
    {
        // RVA 0x80A290
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }
}  // namespace ai
