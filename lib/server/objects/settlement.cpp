#include "settlement.h"

#include <stdexcept>

#include "core/ini.h"
#include "base/prototypemanager.h"
#include <core/log.h>
#include <server/utils.h>
#include "staticautogun.h"
#include "team.h"
#include "base/objcontainer.h"
#include "physicbodies/physichelpers.h"
#include "vehicle.h"
#include <config.h>
#include <core/kernel.h>
#include <server/dynamicscene.h>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Settlement)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Settlement);

    SettlementPrototypeInfo::auxZoneInfo::auxZoneInfo(auxZoneInfo const& other)
        : m_action(other.m_action), m_offset(other.m_offset), m_radius(other.m_radius)
    {
        // RVA 0x835290
    }

    SettlementPrototypeInfo::auxZoneInfo::auxZoneInfo()
    {
        m_offset.x = 0.0;
        m_offset.y = 0.0;
        m_offset.z = 0.0;
        m_radius = 10.0;
    }

    bool SettlementPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::SimplePhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            ref_ptr node = xmlFile->CreateNode();
            for (xmlNode->GetFirstChild(node, "zone"); !node->IsEmpty(); node->GetNextSibling(node, "zone"))
            {
                ai::SettlementPrototypeInfo::auxZoneInfo ZI;
                m3d::SafeStrAttrib(ZI.m_action, node, "action");
                m3d::SafeVectorAttrib(ZI.m_offset, node, "offset");
                m3d::SafeFloatAttrib(ZI.m_radius, node, "radius");
                m_zoneInfos.push_back(std::move(ZI));
            }
            m3d::SafeStrAttrib(m_vehiclesPrototypeName, xmlNode, "Vehicles");
        }
        return result;
    }

    void SettlementPrototypeInfo::PostLoad()
    {
        if (!m_vehiclesPrototypeName.empty())
        {
            m_vehiclesPrototypeId = thePrototypeManager->GetPrototypeId(m_vehiclesPrototypeName);
            if (m_vehiclesPrototypeId == -1)
            {
                M3D_LOG_ERR(
                    "Error: invalid vehicles prototype '" + m_vehiclesPrototypeName + "' for settlement prototype '" + m_prototypeName +
                    "'");
            }
        }
    }

    SettlementPrototypeInfo::SettlementPrototypeInfo()
    {
    }

    m3d::Class* Settlement::GetClass() const
    {
        // RVA 0x833C90
        return RT_CLASS_LOCAL(Settlement);
    }

    void Settlement::Registration()
    {
    }

    void Settlement::Remove()
    {
        // RVA 0x838430 - everything the settlement owns goes with it.
        for (Location* const location : m_locations)
        {
            location->Remove();
        }
        for (StaticAutoGun* const gun : m_staticAutoGuns)
        {
            gun->Remove();
        }
        for (auto const& [type, team] : m_teams)
        {
            if (team)
            {
                team->Remove();
            }
        }
        for (Team* const caravan : m_caravans)
        {
            caravan->Remove();
        }
        SimplePhysicObj::Remove();
    }

    Settlement::Settlement(SettlementPrototypeInfo const& prototype) : SimplePhysicObj(prototype), m_population(5, 0, 5)
    {
        DisablePhysics();
        _SetStatic();
    }

    void Settlement::CreateChildren()
    {
        // RVA 0x833D20
        Obj::CreateChildren();
    }

    CVector Settlement::GetDeployPos() const
    {
        // RVA 0x836710
        Location const* const location = GetLocation(Location::LOCATION_DEPLOY);
        return location ? location->GetPosition() : GetPosition();
    }

    void Settlement::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x83AB90 - a team is written out only when it differs from what loading would create by default.
        Obj::SaveToXML(xmlFile, xmlNode);
        ref_ptr partsNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Parts");
        xmlNode->AddChild(partsNode);

        retruxx::map<CStr, int> partPrototypeIds;
        partPrototypeIds[CStr("AttackTeam")] = thePrototypeManager->GetPrototypeId(CStr("settlementTeam"));
        partPrototypeIds[CStr("DefendTeam")] = thePrototypeManager->GetPrototypeId(CStr("settlementTeam"));
        for (auto const& [partName, prototypeId] : partPrototypeIds)
        {
            auto const team = m_teams.find(partName);
            bool const present = team != m_teams.end();
            if (theObjects->m_SaveType == ObjContainer::SAVE_FULL || !present ||
                team->second->GetPrototypeInfo()->m_prototypeId != prototypeId || !team->second->bIsEqualToPrototype())
            {
                ref_ptr partNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, partName.c_str());
                partNode->SetAttribute("present", present ? "yes" : "no");
                if (present)
                {
                    team->second->SaveToXML(xmlFile, partNode);
                }
                partsNode->AddChild(partNode);
            }
        }
    }

    CVector Settlement::GetDefendPos() const
    {
        // RVA 0x836800 - without a location, 50 units ahead.
        if (Location const* const location = GetLocation(Location::LOCATION_DEFEND))
        {
            return location->GetPosition();
        }
        CVector const dir = GetDirection();
        CVector const offset(dir.x * 50.0f, dir.y * 50.0f, dir.z * 50.0f);
        CVector const pos = GetPosition();
        return CVector(pos.x + offset.x, pos.y + offset.y, pos.z + offset.z);
    }

    Location* Settlement::GetLocation(Location::LocationType locationType) const
    {
        // RVA 0x835B90
        for (Location* const location : m_locations)
        {
            if (location->GetLocationType() == locationType)
            {
                return location;
            }
        }
        return nullptr;
    }

    bool Settlement::CanChildBeAdded(m3d::Class* pClass) const
    {
        if (Obj::CanChildBeAdded(pClass))
            return 1;
        return pClass->IsKindOf(&ai::StaticAutoGun::m_classStaticAutoGun) || pClass->IsKindOf(&ai::Location::m_classLocation) ||
            pClass->IsKindOf(&ai::Team::m_classTeam);
    }

    void Settlement::SetRotationSelf(Quaternion const& rot)
    {
        PhysicObj::SetRotationSelf(rot);
    }

    CVector Settlement::GetCaravanArrivePos() const
    {
        // RVA 0x8368B0 - without a location, 70 units ahead.
        if (Location const* const location = GetLocation(Location::LOCATION_CARAVAN_ARRIVE))
        {
            return location->GetPosition();
        }
        CVector const dir = GetDirection();
        CVector const offset(dir.x * 70.0f, dir.y * 70.0f, dir.z * 70.0f);
        CVector const pos = GetPosition();
        return CVector(pos.x + offset.x, pos.y + offset.y, pos.z + offset.z);
    }

    m3d::Class* Settlement::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    int Settlement::OnEvent(Event const& evn)
    {
        return Obj::OnEvent(evn);
    }

    void Settlement::SetPosition(CVector const& pos)
    {
        // RVA 0x836E00 - the locations move along, keeping their offsets.
        CVector const oldPos = GetPosition();
        SetPositionSelf(pos);
        for (Location* const location : m_locations)
        {
            CVector const locationPos = location->GetPosition();
            location->SetPosition(CVector(
                pos.x + (locationPos.x - oldPos.x), locationPos.y - oldPos.y + pos.y, locationPos.z - oldPos.z + pos.z));
        }
    }

    Team* Settlement::GetTeamByType(CStr const& partType)
    {
        // RVA 0x8386C0
        auto const it = m_teams.find(partType);
        return it != m_teams.end() ? it->second : nullptr;
    }

    void Settlement::SetTeamByType(CStr const& partType, Team* team)
    {
        // RVA 0x839E30 - a team already of this type is detached first.
        auto const it = m_teams.find(partType);
        if (it != m_teams.end())
        {
            it->second->m_parentId = -1;
            m_teams.erase(it);
        }
        if (team)
        {
            m_teams[partType] = team;
            team->LinkToParent(GetId(), HIERARCHY_COMPONENT);
        }
    }

    void Settlement::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x83A5A0 - the attack and defend teams come from the Parts node, or are created from their prototype.
        Obj::LoadFromXML(xmlFile, xmlNode);

        retruxx::map<CStr, int> partPrototypeIds;
        partPrototypeIds[CStr("AttackTeam")] = thePrototypeManager->GetPrototypeId(CStr("settlementTeam"));
        partPrototypeIds[CStr("DefendTeam")] = thePrototypeManager->GetPrototypeId(CStr("settlementTeam"));

        ref_ptr partsNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        xmlNode->GetFirstChild(partsNode, "Parts");
        for (auto const& [partName, prototypeId] : partPrototypeIds)
        {
            ref_ptr partNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
            bool present = true;
            if (!partsNode->IsEmpty())
            {
                partsNode->GetFirstChild(partNode, partName.c_str());
                if (!partNode->IsEmpty())
                {
                    m3d::SafeBoolAttrib(present, partNode, "present");
                    if (!present)
                    {
                        continue;
                    }
                }
            }

            int objId;
            if (partNode->IsEmpty())
            {
                objId = theObjects->CreateNewObjectWithSuspendedPostLoad(prototypeId, "", -1, -1);
                if (objId == -1)
                {
                    continue;
                }
            }
            else
            {
                objId = gDynamicScene->ReadNewObjectFromXml(xmlFile, partNode, {});
                if (objId == -1)
                {
                    M3D_LOG_ERR("Error: could not read object part from XML, part name = '" + partName + "'");
                    continue;
                }
            }

            // NOTE: a stale id leaves a null object, which is type checked through a null pointer.
            Obj* const obj = theObjects->GetEntityByObjId(objId);
            if (static_cast<m3d::Object*>(obj)->IsKindOf(RT_CLASS_LOCAL(Team)))
            {
                SetTeamByType(partName, static_cast<Team*>(obj));
            }
            else
            {
                M3D_LOG_ERR("Error: the part '" + partName + "' isn't a Team");
            }
        }
    }

    void Settlement::SetPositionSelf(CVector const& pos)
    {
        // RVA 0x834BC0
        int const physicState = GetPhysicState();
        bool const enabled = (physicState & 2) != 0;
        if (enabled)
        {
            _SetGeomEnabledBit(false);
        }
        PhysicObj::SetPositionSelf(ai::GetGroundPos(pos, 1, 0));
        if (enabled)
        {
            _SetGeomEnabledBit(true);
        }
    }

    SettlementPrototypeInfo const* Settlement::GetPrototypeInfo() const
    {
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), SettlementPrototypeInfo const);
    }

    bool Settlement::RemoveChild(Obj* pChild)
    {
        // RVA 0x838590
        Obj::RemoveChild(pChild);
        if (!pChild)
        {
            return false;
        }
        if (pChild->IsKindOf(RT_CLASS_LOCAL(Location)))
        {
            m_locations.erase(static_cast<Location*>(pChild));
            pChild->m_parentId = -1;
            return true;
        }
        if (pChild->IsKindOf(RT_CLASS_LOCAL(StaticAutoGun)))
        {
            m_staticAutoGuns.erase(static_cast<StaticAutoGun*>(pChild));
            pChild->m_parentId = -1;
            return true;
        }
        if (!pChild->IsKindOf(RT_CLASS_LOCAL(Team)))
        {
            return false;
        }
        auto const it = std::find(m_caravans.begin(), m_caravans.end(), pChild);
        if (it == m_caravans.end())
        {
            return false;
        }
        m_caravans.erase(it);
        pChild->m_parentId = -1;
        return true;
    }

    void Settlement::SetRotation(Quaternion const& rot)
    {
        // RVA 0x836960 - the locations turn around the settlement with it.
        CMatrix const oldRotInversed = GetRotation().getInversed().ToMatrix();
        SetRotationSelf(rot);
        for (Location* const location : m_locations)
        {
            CVector const pos = GetPosition();
            CVector const locationPos = location->GetPosition();
            float const dz = locationPos.z - pos.z;
            float const dx = locationPos.x - pos.x;
            float const dy = locationPos.y - pos.y;
            // Back into the settlement's frame...
            float const lx = dz * oldRotInversed._31 + oldRotInversed._11 * dx + oldRotInversed._21 * dy;
            float const ly = dz * oldRotInversed._32 + oldRotInversed._12 * dx + oldRotInversed._22 * dy;
            float const lz = dz * oldRotInversed._33 + oldRotInversed._13 * dx + oldRotInversed._23 * dy;
            // ...and out with the new rotation.
            CMatrix const newRot = rot.ToMatrix();
            float const rx = newRot._31 * lz + newRot._21 * ly + newRot._11 * lx;
            float const ry = newRot._32 * lz + newRot._22 * ly + newRot._12 * lx;
            float const rz = newRot._33 * lz + newRot._23 * ly + newRot._13 * lx;
            CVector const center = GetPosition();
            location->SetPosition(CVector(rx + center.x, center.y + ry, center.z + rz));
            location->SetRotation(rot);
        }
    }

    CVector Settlement::GetAttackPos() const
    {
        // RVA 0x836750 - without a location, 100 units ahead.
        if (Location const* const location = GetLocation(Location::LOCATION_ATTACK))
        {
            return location->GetPosition();
        }
        CVector const dir = GetDirection();
        CVector const offset(dir.x * 100.0f, dir.y * 100.0f, dir.z * 100.0f);
        CVector const pos = GetPosition();
        return CVector(pos.x + offset.x, pos.y + offset.y, pos.z + offset.z);
    }

    void Settlement::AddChild(Obj* pObj)
    {
        Obj::AddChild(pObj);
        if (pObj)
        {
            if (pObj->IsKindOf(&ai::StaticAutoGun::m_classStaticAutoGun))
            {
                m_staticAutoGuns.insert((StaticAutoGun*)pObj);
            }
            else if (pObj->IsKindOf(&ai::Location::m_classLocation))
            {
                m_locations.insert((Location*)pObj);
            }
            else
            {
                if (!pObj->IsKindOf(&ai::Team::m_classTeam))
                    return;

                CStr name = pObj->GetName();
                if (name.findsubstr("_Caravan", 0) == -1)
                {
                    M3D_ASSERT(0);
                }
                else
                {
                    m_caravans.push_back((Team*)pObj);
                }
            }
            pObj->LinkToParent(GetId(), HIERARCHY_CHILD);
        }
    }

    CVector Settlement::GetEnterPos() const
    {
        // RVA 0x8366D0
        Location const* const location = GetLocation(Location::LOCATION_ENTER);
        return location ? location->GetPosition() : GetPosition();
    }

    Vehicle* Settlement::_SpawnVehicle()
    {
        // RVA 0x839A10 - a vehicle of the settlement's side on free ground near the deploy point, not yet registered.
        int const vehiclesPrototypeId = GetPrototypeInfo()->m_vehiclesPrototypeId;
        if (vehiclesPrototypeId == -1)
        {
            M3D_LOG_ERR("Error: settlement '" + CStr(GetName()) + "' could not spawn vehicle: wrong prototype");
            return nullptr;
        }
        // NOTE: the prototype is used without a null check.
        auto const* const vehicleInfo =
            static_cast<VehiclePrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(vehiclesPrototypeId));
        CVector pos;
        if (!GetValidPosition(GetDeployPos(), 5.0f, vehicleInfo->m_priority, pos, true, false, retruxx::set<m3d::Class*>()))
        {
            return nullptr;
        }
        pos.y = M3D_ENGINE_CFG.GetHeight(pos.x, pos.z) + 2.0f;
        Obj* const vehicle = vehicleInfo->CreateTargetObject();
        vehicle->SetBelong(GetBelong());
        static_cast<PhysicObj*>(vehicle)->SetPosition(pos);
        return static_cast<Vehicle*>(vehicle);
    }

    int Settlement::_GetNumSpawnedVehicles()
    {
        // RVA 0x836EF0
        int result = 0;
        for (auto const& [type, team] : m_teams)
        {
            result += static_cast<int>(team->GetVehicles().size());
        }
        return result;
    }

    int Settlement::_GetOverallPopulation()
    {
        // RVA 0x834C10
        return m_population.value().get() + _GetNumSpawnedVehicles();
    }

    void Settlement::CreateChildLocation(Location::LocationType locationType, CStr const& nameSuffix, CVector const& pos)
    {
        // RVA 0x83A2A0 - adds a generic 50 unit location of the given type, unless there is one already.
        if (GetLocation(locationType))
        {
            return;
        }
        int const prototypeId = thePrototypeManager->GetPrototypeId(CStr("genericLocation"));
        int const locationId = theObjects->CreateNewObject(prototypeId, (CStr(GetName()) + nameSuffix).c_str(), -1, -1);
        if (locationId == -1)
        {
            return;
        }
        // NOTE: the location is used without a null check.
        auto* const location = static_cast<Location*>(theObjects->GetEntityByObjId(locationId));
        AddChild(location);
        location->SetLocationType(locationType);
        location->SetPosition(pos);
        location->SetRadius(50.0f);
        if (locationType != Location::LOCATION_GENERIC && locationType != Location::LOCATION_PASSAGE &&
            locationType != Location::LOCATION_DEPLOY && locationType != Location::LOCATION_ENTER &&
            locationType != Location::LOCATION_CARAVAN_ARRIVE)
        {
            location->SetPropertyById(53, m3d::AIParam(static_cast<int>(Location::LOCATION_GENERIC)));
        }
    }

    int Settlement::_GetMaxOverallPopulaition()
    {
        // RVA 0x834C30
        return m_population.maxValue().get() + _GetMaxSpawnedVehicles();
    }

    void Settlement::_InternalPostLoad()
    {
        SimplePhysicObj::_InternalPostLoad();
    }

    int Settlement::_GetMaxSpawnedVehicles()
    {
        // RVA 0x833D50
        return 0;
    }

    Settlement::~Settlement()
    {
        // RVA 0x83A1B0 - only the members are destroyed.
    }

    m3d::Object* Settlement::CreateObject()
    {
        // RVA 0x8357F0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    void Settlement::FillingFeedBackParam()
    {
        // RVA 0x833D10
    }

    m3d::Object* Settlement::Clone()
    {
        // RVA 0x835630
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai
