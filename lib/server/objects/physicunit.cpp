#include "physicunit.h"
#include "scene/servers/dataserver.h"
#include "game/m3dgame.h"
#include "core/log.h"
#include "base/prototypemanager.h"
#include "base/globalproperties.h"
#include "server/dynamicscene.h"

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

    void PhysicUnit::Update(float elapsedTime, unsigned workTime)
    {
        // RVA 0x80BD60 - a pedestrian walking its paths: it stands for a while, turns towards
        // the next waypoint, walks to it, and at the end of a path picks another one. Units far
        // from the player's vehicle stop (with hysteresis between the two distances).
        SimplePhysicObj::Update(elapsedTime, workTime);

        Vehicle* playerVehicle = gDynamicScene->GetVehicleControlledByPlayer();
        if (!playerVehicle)
        {
            m_bMustWalk = true;
        }
        else
        {
            CVector const vehiclePos = playerVehicle->GetPosition();
            CVector const pos = GetPosition();
            float const dz = pos.z - vehiclePos.z;
            float const dy = pos.y - vehiclePos.y;
            float const dx = pos.x - vehiclePos.x;
            float const dist = static_cast<float>(sqrt(double(dz * dz + dy * dy) + dx * dx));
            if (theGlobProp.m_distToTurnOnPhysics > dist)
            {
                m_bMustWalk = true;
            }
            else if (dist > theGlobProp.m_distToTurnOffPhysics)
            {
                m_bMustWalk = false;
            }
        }
        if (!m_bMustWalk)
        {
            return;
        }

        float distToNext = 0.0f;
        if (m_walkState == STAND)
        {
            if (m_pathsMap.empty())
            {
                return;
            }
            m_standTtl.regenerate(elapsedTime);
            if (m_standTtl.value().get() != m_standTtl.minValue().get())
            {
                return;
            }
            m_walkTtl.setToMin();
            m_prevWayPoint = GetPosition();
            if (m_curWayPointNum == 0)
            {
                if (!m_curPath)
                {
                    SYS_ERROR("m_curPath");
                }
                // Already standing on the first waypoint: head for the second one.
                CVector const& first = (*m_curPath)[m_curWayPointNum];
                CVector const offset(m_prevWayPoint.x - first.x, 0.0f - first.y, m_prevWayPoint.z - first.z);
                if (offset.length() < 0.001f)
                {
                    m_curWayPointNum = 1;
                }
                // After a path is finished another one (not the same, if there is a choice) is
                // picked at random.
                if (m_bMustChangePath && m_pathsMap.size() > 1)
                {
                    std::vector<CStr> names;
                    for (auto const& path : m_pathsMap)
                    {
                        names.push_back(path.first);
                    }
                    unsigned idx;
                    while (true)
                    {
                        idx = (static_cast<unsigned>(names.size()) * static_cast<unsigned>(rand())) >> 15;
                        if (!(names[idx] == m_curPathName) || names.empty() || names.size() <= 1)
                        {
                            break;
                        }
                    }
                    m_curPathName = names[idx];
                    m_curPath = &m_pathsMap[m_curPathName];
                    m_curWayPointNum = 0;
                    m_bMustChangePath = false;
                }
            }
            if (m_curWayPointNum >= m_curPath->size())
            {
                m_curWayPointNum = 0;
            }
            CVector const& next = (*m_curPath)[m_curWayPointNum];
            distToNext = CVector(next.x - m_prevWayPoint.x, 0.0f, next.z - m_prevWayPoint.z).length();
        }
        else if (m_walkState == WALK)
        {
            if (!m_curPath || m_curPath->empty())
            {
                SYS_ERROR("m_curPath && !m_curPath->empty()");
            }
            // Walk the straight line from the previous waypoint, m_walkTtl being the progress.
            m_walkTtl.regenerate(elapsedTime);
            float const t = m_walkTtl.value().get();
            CVector const& next = (*m_curPath)[m_curWayPointNum];
            SetPosition(CVector(
                m_prevWayPoint.x + (next.x - m_prevWayPoint.x) * t,
                m_prevWayPoint.y + (next.y - m_prevWayPoint.y) * t,
                m_prevWayPoint.z + (next.z - m_prevWayPoint.z) * t));
            if (m_walkTtl.value().get() != m_walkTtl.maxValue().get())
            {
                return;
            }

            unsigned const reached = m_curWayPointNum;
            if (reached == 0)
            {
                // Back at the start of the path: stand, and change paths if there is only one
                // point to walk to.
                if (m_bMustChangePath || (m_bMustChangePath = m_curPath->size() < 2))
                {
                    _SetWalkState(STAND);
                    return;
                }
            }
            m_curWayPointNum = reached + 1;
            if (m_curWayPointNum >= m_curPath->size())
            {
                m_curWayPointNum = 0;
                m_bMustChangePath = true;
                CauseEvent(GE_TARGET_REACHED, 0.0f, m3d::AIParam(GetId()), m3d::AIParam(m_curPathName));
            }
            m_walkTtl.setToMin();
            m_prevWayPoint = GetPosition();
            CVector const& following = (*m_curPath)[m_curWayPointNum];
            double const fx = following.x - m_prevWayPoint.x;
            double const fz = following.z - m_prevWayPoint.z;
            distToNext = static_cast<float>(sqrt(fx * fx + fz * fz));
        }
        else if (m_walkState == TURN)
        {
            if (!m_curPath || m_curPath->empty())
            {
                SYS_ERROR("m_curPath && !m_curPath->empty()");
            }
            // Turn about the vertical (or whatever axis takes it there) towards the next
            // waypoint, at m_turnSpeed; once facing it, walk.
            CVector const& next = (*m_curPath)[m_curWayPointNum];
            CVector needDir = CVector(next.x - m_prevWayPoint.x, next.y - m_prevWayPoint.y, next.z - m_prevWayPoint.z).getNormalized();
            CVector const dir = GetDirection();
            needDir.y = dir.y;
            needDir.normalizeInplace();
            CVector axis = CVector(
                needDir.z * dir.y - dir.z * needDir.y,
                dir.z * needDir.x - needDir.z * dir.x,
                dir.x * needDir.y - needDir.x * dir.y).getNormalized();
            if ((axis.x * axis.x + axis.z * axis.z) + axis.y * axis.y < 0.99900001f)
            {
                axis = CVector(0.0f, 1.0f, 0.0f);
            }
            float cosAngle = (needDir.y * dir.y + dir.x * needDir.x) + dir.z * needDir.z;
            if (cosAngle < -0.99999899f)
            {
                cosAngle = -0.99999899f;
            }
            else if (cosAngle > 0.99999899f)
            {
                cosAngle = 0.99999899f;
            }
            float angle = m_turnSpeed * elapsedTime;
            float const desiredAngle = static_cast<float>(acos(cosAngle));
            if (angle >= desiredAngle)
            {
                angle = desiredAngle;
                m_walkState = WALK;
            }
            double const halfAngle = angle * 0.5;
            float const s = static_cast<float>(sin(halfAngle));
            float const w = static_cast<float>(cos(halfAngle));
            float const qx = s * axis.x;
            float const qy = axis.y * s;
            float const qz = axis.z * s;
            float const yx = qy * qx;
            float const zy = qz * qy;
            float const m11 = 1.0f - (qz * qz + qy * qy) * 2.0f;
            float const m21 = (yx - w * qz) * 2.0f;
            float const m31 = (w * qy + qz * qx) * 2.0f;
            float const m12 = (w * qz + yx) * 2.0f;
            float const m22 = 1.0f - (qz * qz + qx * qx) * 2.0f;
            float const m32 = (zy - w * qx) * 2.0f;
            float const m13 = (qz * qx - w * qy) * 2.0f;
            float const m23 = (w * qx + zy) * 2.0f;
            float const m33 = 1.0f - (qy * qy + qx * qx) * 2.0f;
            SetDirection(CVector(
                (m11 * dir.x + m31 * dir.z) + m21 * dir.y,
                (m12 * dir.x + m32 * dir.z) + m22 * dir.y,
                (m13 * dir.x + m33 * dir.z) + m23 * dir.y));
            return;
        }
        else
        {
            return;
        }

        // Start the next leg: m_walkTtl runs from 0 to 1 over it.
        m_walkTtl.regeneration().set(distToNext <= 0.001f ? 1.0f : m_walkSpeed / distToNext);
        _SetWalkState(TURN);
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
