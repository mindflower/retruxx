#include "boss04.h"

#include <cmath>
#include <stdexcept>

#include "m3dapp.h"
#include "skelmodel.h"
#include "core/kernel.h"
#include "core/log.h"
#include "scene/nodes/sgnode.h"
#include "scene/servers/dataserver.h"
#include "server/damageinfo.h"
#include "server/event.h"
#include "server/utils.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/base/prototypemanager.h"
#include "server/objects/monsters/boss04drone.h"
#include "server/objects/monsters/boss04part.h"
#include "server/objects/monsters/boss04station.h"
#include "server/objects/monsters/boss04stationpart.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Boss04, NextState)
{
    // RVA 0x73E9F0
    static_cast<ai::Boss04*>(context->asObject(0, "Boss04"))->NextState();
    return 1;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Boss04)
    RT_CLASS_EXPORT(Boss04, m3d::METHOD, NextState, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Boss04);

    // The boss is ringed by three stations; losing a station blows off the boss parts bound to it,
    // and losing all three kills it.
    int const Boss04::NUM_STATIONS = 3;

    namespace
    {
        CVector const INITIAL_UP_DIRECTION(0.0f, 1.0f, 0.0f);
        CVector const INITIAL_OBJECTS_DIRECTION(0.0f, 0.0f, 1.0f);

        // v rotated by q (row vector times q's rotation matrix), as expanded inline in the
        // shipped code.
        CVector RotateByQuaternion(CVector const& v, Quaternion const& q)
        {
            float const xz = q.x * q.z;
            float const xw = q.x * q.w;
            float const xx = q.x * q.x;
            float const xy = q.x * q.y;
            float const zy = q.z * q.y;
            float const yy = q.y * q.y;
            float const zz = q.z * q.z;
            float const yw = q.y * q.w;
            float const zw = q.z * q.w;
            float const m11 = 1.0f - (zz + yy) * 2.0f;
            float const m21 = (xy - zw) * 2.0f;
            float const m31 = (yw + xz) * 2.0f;
            float const m12 = (zw + xy) * 2.0f;
            float const m22 = 1.0f - (zz + xx) * 2.0f;
            float const m32 = (zy - xw) * 2.0f;
            float const m13 = (xz - yw) * 2.0f;
            float const m23 = (xw + zy) * 2.0f;
            float const m33 = 1.0f - (yy + xx) * 2.0f;
            return CVector(
                (v.x * m11 + m21 * v.y) + m31 * v.z,
                (v.x * m12 + m22 * v.y) + m32 * v.z,
                (v.x * m13 + m23 * v.y) + m33 * v.z);
        }

        // The Hamilton product a * b, as expanded inline in the shipped code.
        Quaternion QuatMul(Quaternion const& a, Quaternion const& b)
        {
            return Quaternion(
                a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
                a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
                a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
                a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z);
        }
    }  // namespace

    void Boss04PrototypeInfo::PostLoad()
    {
        // RVA 0x73D720
        ComplexPhysicObjPrototypeInfo::PostLoad();
        m_stationPrototypeId = thePrototypeManager->GetPrototypeId(m_stationPrototypeName);
        m_dronePrototypeId = thePrototypeManager->GetPrototypeId(m_dronePrototypeName);
    }

    Boss04PrototypeInfo::Boss04PrototypeInfo()
    {
        // RVA 0x740DC0
        m_stationPrototypeId = -1;
        m_dronePrototypeId = -1;
        m_timeBetweenDrones.x = 10.0f;
        m_timeBetweenDrones.y = 20.0f;
        m_maxDrones = 0;
    }

    bool Boss04PrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x740E70
        bool const result = ComplexPhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (!result)
        {
            return result;
        }
        m3d::SafeStrAttrib(m_stationPrototypeName, xmlNode, "StationPrototype");
        m3d::SafeStrAttrib(m_dronePrototypeName, xmlNode, "DronePrototype");
        m3d::SafeVector2Attrib(m_timeBetweenDrones, xmlNode, "TimeBetweenDrones");
        int maxDrones = 0;
        if (m3d::SafeIntAttrib(maxDrones, xmlNode, "MaxDrones") && maxDrones >= 0)
        {
            m_maxDrones = static_cast<unsigned>(maxDrones);
        }
        CStr strDroneSpawningLpNames;
        m3d::SafeStrAttrib(strDroneSpawningLpNames, xmlNode, "DroneSpawningLps");
        m3d::Tokenize(strDroneSpawningLpNames, m_droneSpawningLpNames, "(), ;\t");

        // One list of boss part names per station, in station order.
        m_stationToPartBindings.clear();
        ref_ptr bindingsNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        xmlNode->GetFirstChild(bindingsNode, "StationToPartBindings");
        if (!bindingsNode->IsEmpty())
        {
            ref_ptr stationNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
            bindingsNode->GetFirstChild(stationNode, "Station");
            int i = 0;
            while (!stationNode->IsEmpty())
            {
                int id = -1;
                m3d::SafeIntAttrib(id, stationNode, "id");
                if (id != i)
                {
                    SYS_ERROR("id == i");
                }
                ++i;
                CStr strPartNames;
                m3d::SafeStrAttrib(strPartNames, stationNode, "Parts");
                retruxx::vector<CStr> partNames;
                m3d::Tokenize(strPartNames, partNames, "(), ;\t");
                m_stationToPartBindings.push_back(partNames);
                stationNode->GetNextSibling(stationNode, "Station");
            }
        }
        return true;
    }

    Obj* Boss04PrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x740D60
        return new Boss04(*this);
    }

    void Boss04PrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        // RVA 0x740280 - resolves the drone spawning load points on the main part's model.
        // NOTE: does not chain to the base RefreshFromXml.
        if (!m_droneSpawningLpIds.empty())
        {
            return;
        }
        auto const it = m_partPrototypeIds.find(CStr("MAIN"));
        if (it == m_partPrototypeIds.end())
        {
            SYS_ERROR("iter != m_partPrototypeIds.end()");
        }
        // NOTE: carries on with the end iterator after the assertion.
        PrototypeInfo const* const pi = thePrototypeManager->GetPrototypeInfo(it->second);
        if (!pi->IsPrototypeOf(RT_CLASS_LOCAL(Boss04Part)))
        {
            SYS_ERROR("IS_PROTOTYPE_OF( pi, Boss04Part )");
        }
        CStr const mainPartModelName(static_cast<PhysicBodyPrototypeInfo const*>(pi)->m_engineModelName);
        m3d::DataServer* server = &M3D_APP->GetAnimatedModelsServer();
        int const itemId = server->GetItemByName(mainPartModelName.c_str(), true);
        if (itemId == -1)
        {
            return;
        }
        m3d::AnimatedModel* mdl = nullptr;
        server->GetItemProperty(itemId, 16394, &mdl);
        if (!mdl)
        {
            return;
        }
        for (auto const& lpName : m_droneSpawningLpNames)
        {
            int const lpId = mdl->GetLoadPointIdByName(lpName.c_str());
            if (lpId == -1)
            {
                M3D_LOG_ERR(CStr("Error: LoadPoint not found! Model = '") + mainPartModelName + CStr("', lp = ") + lpName);
            }
            else
            {
                m_droneSpawningLpIds.push_back(lpId);
            }
        }
    }

    Boss04::Boss04(Boss04PrototypeInfo const& prototype) : ComplexPhysicObj(prototype)
    {
        // RVA 0x740A40
        m_state = STATE_WAITING;
        ComplexPhysicObj::DisablePhysics();
    }

    Boss04::~Boss04()
    {
        // RVA 0x740CC0
    }

    m3d::Object* Boss04::Clone()
    {
        // RVA 0x73E1F0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* Boss04::CreateObject()
    {
        // RVA 0x73E3B0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Class* Boss04::GetBaseClass()
    {
        // RVA 0x73D700
        return RT_CLASS_LOCAL(ComplexPhysicObj);
    }

    m3d::Class* Boss04::GetClass() const
    {
        // RVA 0x73D710
        return RT_CLASS_LOCAL(Boss04);
    }

    Boss04PrototypeInfo const* Boss04::GetPrototypeInfo() const
    {
        // RVA 0x73F5C0
        return static_cast<Boss04PrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    void Boss04::RegisterProperty(char const* name, int id, eGObjPropertySaveStatus saveStatus)
    {
        // Declared in the PDB but never emitted in the shipped build (Registration is empty).
        m_propertiesMap[CStr(name)] = id;
        m_propertiesSaveStatesMap[id] = saveStatus;
    }

    void Boss04::Registration()
    {
        // RVA 0x73D7D0 - registers nothing.
    }

    eGObjPropertySaveStatus Boss04::GetPropertySaveStatus(int id) const
    {
        // RVA 0x740AA0
        auto it = m_propertiesSaveStatesMap.find(id);
        if (it != m_propertiesSaveStatesMap.end())
        {
            return it->second;
        }
        return PhysicObj::GetPropertySaveStatus(id);
    }

    void Boss04::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
    {
        // Declared in the PDB but never emitted in the shipped build; the usual property-map pattern.
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.first);
        }
        PhysicObj::GetPropertiesNames(Props);
    }

    void Boss04::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        // Declared in the PDB but never emitted in the shipped build; the usual property-map pattern.
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.second);
        }
        PhysicObj::GetPropertiesIDs(Props);
    }

    CStr Boss04::GetPropertyName(int id) const
    {
        // RVA 0x740C50
        for (auto const& property : m_propertiesMap)
        {
            if (property.second == id)
            {
                return property.first;
            }
        }
        return PhysicObj::GetPropertyName(id);
    }

    bool Boss04::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        // RVA 0x73D7F0
        return PhysicObj::SetPropertyById(propertyId, newValue);
    }

    int Boss04::GetPropertyId(char const* PropertyName) const
    {
        // RVA 0x740AE0
        auto it = m_propertiesMap.find(PropertyName);
        if (it != m_propertiesMap.end())
        {
            return it->second;
        }
        return PhysicObj::GetPropertyId(PropertyName);
    }

    bool Boss04::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x73D800
        return PhysicObj::_GetPropertyDefaultInternal(propertyId, retVal);
    }

    bool Boss04::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x73D7E0
        return PhysicObj::_GetPropertyInternal(propertyId, retVal);
    }

    int Boss04::OnEvent(Event const& evn)
    {
        // RVA 0x73F5F0
        int const result = Obj::OnEvent(evn);
        if (evn.m_eventId == GE_BOSS04_STATION_DESTROYED)
        {
            _OnBoss04StationDestroyed(evn);
            return 1;
        }
        return result;
    }

    bool Boss04::CanChildBeAdded(m3d::Class* pClass) const
    {
        // RVA 0x73D760
        if (Obj::CanChildBeAdded(pClass))
        {
            return true;
        }
        return pClass->IsKindOf(RT_CLASS_LOCAL(Boss04Station)) || pClass->IsKindOf(RT_CLASS_LOCAL(Boss04Drone));
    }

    void Boss04::AddChild(Obj* pObj)
    {
        // RVA 0x7409C0
        ComplexPhysicObj::AddChild(pObj);
        if (!pObj)
        {
            return;
        }
        if (pObj->IsKindOf(RT_CLASS_LOCAL(Boss04Station)))
        {
            m_stations.push_back(static_cast<Boss04Station*>(pObj));
            pObj->LinkToParent(GetId(), HIERARCHY_CHILD);
        }
        else if (pObj->IsKindOf(RT_CLASS_LOCAL(Boss04Drone)))
        {
            m_drones.push_back(static_cast<Boss04Drone*>(pObj));
            pObj->LinkToParent(GetId(), HIERARCHY_CHILD);
        }
    }

    bool Boss04::RemoveChild(Obj* pChild)
    {
        // RVA 0x73F620
        ComplexPhysicObj::RemoveChild(pChild);
        if (!pChild)
        {
            return false;
        }
        if (pChild->IsKindOf(RT_CLASS_LOCAL(Boss04Station)))
        {
            for (auto it = m_stations.begin(); it != m_stations.end(); ++it)
            {
                if (*it == pChild)
                {
                    pChild->m_parentId = -1;
                    m_stations.erase(it);
                    return true;
                }
            }
        }
        else if (pChild->IsKindOf(RT_CLASS_LOCAL(Boss04Drone)))
        {
            for (auto it = m_drones.begin(); it != m_drones.end(); ++it)
            {
                if (*it == pChild)
                {
                    pChild->m_parentId = -1;
                    m_drones.erase(it);
                    return true;
                }
            }
        }
        return false;
    }

    void Boss04::CreateChildren()
    {
        // RVA 0x740790
        ComplexPhysicObj::CreateChildren();
        _CreateStations(false);
    }

    void Boss04::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x8D0780
        ComplexPhysicObj::LoadFromXML(xmlFile, xmlNode);
        CStr strPathNamesForDrones;
        m3d::SafeStrAttrib(strPathNamesForDrones, xmlNode, "PathNamesForDrones");
        m3d::Tokenize(strPathNamesForDrones, m_pathNamesForDrones, "(), ;\t");
        if (m_state != STATE_DEAD && m_stations.empty())
        {
            _CreateStations(true);
        }
    }

    void Boss04::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x73E570
        ComplexPhysicObj::LoadRuntimeValues(xmlFile, xmlNode);
        m3d::SafeEnumAttrib(m_state, xmlNode, "Boss04State");
    }

    void Boss04::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x73DDB0
        ComplexPhysicObj::SaveToXML(xmlFile, xmlNode);
        xmlNode->SetAttribute("PathNamesForDrones", StringVectorToStr(m_pathNamesForDrones).c_str());
    }

    void Boss04::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x73DE10
        ComplexPhysicObj::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("Boss04State", CStr(static_cast<int>(m_state)).c_str());
    }

    void Boss04::SetPosition(CVector const& pos)
    {
        // RVA 0x73EA10 - the stations move with the boss.
        CVector const oldPos = GetPosition();
        SetPositionSelf(pos);
        for (Boss04Station* const station : m_stations)
        {
            CVector const stationPos = station->GetPosition();
            station->SetPosition(CVector(
                pos.x + (stationPos.x - oldPos.x),
                (stationPos.y - oldPos.y) + pos.y,
                (stationPos.z - oldPos.z) + pos.z));
        }
    }

    void Boss04::SetRotation(Quaternion const& rot)
    {
        // RVA 0x73EAC0 - the stations turn around the boss with it.
        Quaternion const rotForStations = QuatMul(rot, GetRotation().getInversed());
        SetRotationSelf(rot);
        CVector const pos = GetPosition();
        for (Boss04Station* const station : m_stations)
        {
            CVector const stationPos = station->GetPosition();
            CVector const offset =
                RotateByQuaternion(CVector(stationPos.x - pos.x, stationPos.y - pos.y, stationPos.z - pos.z), rotForStations);
            station->SetPosition(CVector(offset.x + pos.x, pos.y + offset.y, pos.z + offset.z));
            station->SetRotation(QuatMul(rotForStations, station->GetRotation()));
        }
    }

    void Boss04::Update(float elapsedTime, unsigned workTime)
    {
        // RVA 0x7407B0
        // NOTE: calls PhysicObj::Update directly, skipping ComplexPhysicObj's.
        PhysicObj::Update(elapsedTime, workTime);
        if (m_state == STATE_ACTION && TimeOutFinished())
        {
            if (m_drones.size() < GetPrototypeInfo()->m_maxDrones)
            {
                _SpawnDrone();
            }
            SetTimeOut(GetPrototypeInfo()->m_timeBetweenDrones.randomValue());
        }
    }

    void Boss04::NextState()
    {
        // RVA 0x73E5C0
        switch (m_state)
        {
        case STATE_WAITING:
            _SetState(STATE_ACTION);
            break;
        case STATE_ACTION:
            _SetState(STATE_DEAD);
            break;
        case STATE_DEAD:
            M3D_LOG_INFO(CStr("Warning: setting DEAD state for Boss04 twice"));
            break;
        default:
            SYS_ERROR("0");
            break;
        }
    }

    float Boss04::GetMaxHealth() const
    {
        // RVA 0x740830 - three stations' worth of the station's main part.
        Boss04PrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        if (!prototypeInfo)
        {
            return 0.0f;
        }
        PrototypeInfo const* const stationInfo = thePrototypeManager->GetPrototypeInfo(prototypeInfo->m_stationPrototypeId);
        if (!stationInfo || !stationInfo->IsPrototypeOf(RT_CLASS_LOCAL(Boss04Station)))
        {
            return 0.0f;
        }
        auto const& partPrototypeIds = static_cast<ComplexPhysicObjPrototypeInfo const*>(stationInfo)->m_partPrototypeIds;
        auto const stationPartIt = partPrototypeIds.find(CStr("MAIN"));
        if (stationPartIt == partPrototypeIds.end())
        {
            return 0.0f;
        }
        PrototypeInfo const* const partInfo = thePrototypeManager->GetPrototypeInfo(stationPartIt->second);
        if (!partInfo || !partInfo->IsPrototypeOf(RT_CLASS_LOCAL(Boss04StationPart)))
        {
            return 0.0f;
        }
        return static_cast<Boss04StationPartPrototypeInfo const*>(partInfo)->m_maxHealth * 3.0f;
    }

    float Boss04::GetHealth() const
    {
        // RVA 0x740160 - the stations' main parts.
        float health = 0.0f;
        for (Boss04Station const* const station : m_stations)
        {
            if (!station || station->m_vehicleParts.empty())
            {
                continue;
            }
            VehiclePart* const part = station->m_vehicleParts.begin()->second;
            if (part && part->IsKindOf(RT_CLASS_LOCAL(Boss04StationPart)))
            {
                health = static_cast<Boss04StationPart*>(part)->GetHealth() + health;
            }
        }
        return health;
    }

    void Boss04::_InternalPostLoad()
    {
        // RVA 0x73D7B0
        // NOTE: calls PhysicObj::_InternalPostLoad directly, skipping ComplexPhysicObj's.
        PhysicObj::_InternalPostLoad();
    }

    void Boss04::_InternalCreateVisualPart()
    {
        // RVA 0x73D7C0
        ComplexPhysicObj::_InternalCreateVisualPart();
    }

    void Boss04::_CreateStations(bool bWithSuspendedPostLoad)
    {
        // RVA 0x73F6E0
        // The stations stand a unit away from the boss's centre, 120 degrees apart and each
        // facing outwards.
        if (!m_stations.empty())
        {
            SYS_ERROR("m_stations.empty()");
        }
        int const stationPrototypeId = GetPrototypeInfo()->m_stationPrototypeId;
        float angle = 0.0f;
        for (int i = NUM_STATIONS; i != 0; --i)
        {
            int const stationId = bWithSuspendedPostLoad
                ? theObjects->CreateNewObjectWithSuspendedPostLoad(stationPrototypeId, "", -1, -1)
                : theObjects->CreateNewObject(stationPrototypeId, "", -1, -1);
            PhysicObj* station = nullptr;
            if (stationId >= 0)
            {
                station = static_cast<PhysicObj*>(theObjects->GetEntityByObjId(stationId));
            }
            float const halfAngle = angle * 0.5f;
            float const s = static_cast<float>(sin(halfAngle));
            Quaternion const relRot(
                s * INITIAL_UP_DIRECTION.x, s * INITIAL_UP_DIRECTION.y, s * INITIAL_UP_DIRECTION.z, static_cast<float>(cos(halfAngle)));
            CVector const offset = RotateByQuaternion(INITIAL_OBJECTS_DIRECTION, relRot);
            CVector const pos = GetPosition();
            // NOTE: the station is used without a null check.
            station->SetPosition(CVector(pos.x + offset.x, pos.y + offset.y, pos.z + offset.z));
            station->SetRotation(relRot);
            AddChild(station);
            angle = angle + 2.0943952f;
        }
    }

    void Boss04::_SetState(Boss04State newState)
    {
        // RVA 0x8D02E0
        m_state = newState;
        if (newState == STATE_ACTION)
        {
            SetTimeOut(GetPrototypeInfo()->m_timeBetweenDrones.randomValue());
        }
        else if (newState == STATE_DEAD)
        {
            // Let go of the stations and destroy the drones.
            while (!m_stations.empty())
            {
                RemoveChild(m_stations.back());
            }
            DamageInfo damageToDrones;
            damageToDrones.damage = 1000000000.0f;
            damageToDrones.damagedPartName = CStr("MAIN");
            while (!m_drones.empty())
            {
                m_drones.back()->InflictDamage(damageToDrones);
                RemoveChild(m_drones.back());
            }
            _SetDeadStatus();
        }
    }

    void Boss04::_SpawnDrone()
    {
        // RVA 0x73FBC0
        // A drone appears at a random spawning load point of the main part and flies a random
        // one of the placed boss's paths.
        Boss04PrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        int const droneId = theObjects->CreateNewObject(prototypeInfo->m_dronePrototypeId, "", -1, -1);
        Boss04Drone* drone = nullptr;
        if (droneId >= 0)
        {
            drone = static_cast<Boss04Drone*>(theObjects->GetEntityByObjId(droneId));
        }
        AddChild(drone);
        m3d::AnimatedModel* mdl = nullptr;
        // NOTE: the main part is used without a null check.
        GetPartByName(CStr("MAIN"))->m_Node->GetServerItemProperty(0x400A, &mdl);
        if (!mdl)
        {
            return;
        }
        auto const& lpIds = GetPrototypeInfo()->m_droneSpawningLpIds;
        if (lpIds.empty())
        {
            M3D_LOG_ERR(CStr("Error: no drone spawning loadpoints for boss04"));
            return;
        }
        unsigned const lpIdx = (static_cast<unsigned>(lpIds.size()) * rand()) >> 15;
        CMatrix const mat = mdl->GetBoneMatrix(lpIds[lpIdx]);
        CVector const relPos(mat._41, mat._42, mat._43);
        Quaternion relRot;
        relRot.FromMatrix(mat);
        CVector const offset = RotateByQuaternion(relPos, GetRotation());
        CVector const pos = GetPosition();
        // NOTE: the drone is used without a null check.
        drone->SetPosition(CVector(offset.x + pos.x, pos.y + offset.y, pos.z + offset.z));
        drone->SetRotation(QuatMul(GetRotation(), relRot));
        // NOTE: with no path names this indexes an empty vector.
        unsigned const pathIdx = (static_cast<unsigned>(m_pathNamesForDrones.size()) * rand()) >> 15;
        drone->SetFlyPathName(m_pathNamesForDrones[pathIdx]);
    }

    void Boss04::_OnBoss04StationDestroyed(Event const& evn)
    {
        // RVA 0x73EF30
        for (unsigned i = 0; i < m_stations.size(); ++i)
        {
            if (m_stations[i]->GetId() == evn.m_senderObjId)
            {
                for (auto const& partName : GetPrototypeInfo()->m_stationToPartBindings[i])
                {
                    if (VehiclePart* const part = GetPartByName(partName))
                    {
                        Flow(part, 0.0f);
                    }
                }
                break;
            }
        }
        bool allDestroyed = true;
        for (Boss04Station const* const station : m_stations)
        {
            allDestroyed &= station->bDestroyed();
        }
        if (allDestroyed)
        {
            _SetState(STATE_DEAD);
        }
    }
}  // namespace ai
