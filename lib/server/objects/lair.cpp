#include "lair.h"
#include <server/event.h>
#include "team.h"
#include "vehicle.h"
#include "base/objcontainer.h"
#include "base/prototypemanager.h"
#include "core/kernel.h"
#include "core/log.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Lair)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Lair);

    namespace
    {
        // Appended to the lair's name to name the two teams it spawns.
        CStr const STR_ATTACKING_TEAM_SUFFIX = "_AttackTeam";
        CStr const STR_DEFENDING_TEAM_SUFFIX = "_DefendTeam";

        // Seconds between two spawn attempts.
        float const SPAWN_TIMEOUT = 2.0f;
    }  // namespace

    LairPrototypeInfo::LairPrototypeInfo()
    {
        m_maxAttackers = 1;
        m_maxDefenders = 1;
    }

    Obj* LairPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x833680
        return new Lair(*this);
    }

    bool LairPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = SettlementPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            _SetGeomType(GEOM_TYPE_BOX);

            m3d::SafeIntAttrib(m_maxAttackers, xmlNode, "MaxAttackers");
            M3D_ASSERT(m_maxAttackers <= MAX_VEHICLES_IN_TEAM);

            m3d::SafeIntAttrib(m_maxDefenders, xmlNode, "MaxDefenders");
            M3D_ASSERT(m_maxDefenders <= MAX_VEHICLES_IN_TEAM);
        }
        return result;
    }

    Lair::Lair(LairPrototypeInfo const& prototype) :
        Settlement(prototype),
        // Counts down from 30 to 0 at one unit per second: the time to breed a new vehicle.
        m_reproductTime(30.0f, 0.0f, 30.0f, -1.0f)
    {
        // RVA 0x833460
        m_maxAttackers = prototype.m_maxAttackers;
        m_maxDefenders = prototype.m_maxDefenders;
        m_state = LS_REPRODUCE_WAIT;
        SetTimeOut(SPAWN_TIMEOUT);
    }

    int Lair::GetPropertyId(char const* PropertyName) const
    {
        // RVA 0x8333C0
        auto it = m_propertiesMap.find(PropertyName);
        if (it != m_propertiesMap.end())
        {
            return it->second;
        }
        return SimplePhysicObj::GetPropertyId(PropertyName);
    }

    LairPrototypeInfo const* Lair::GetPrototypeInfo() const
    {
        // RVA 0x832ED0 - NOTE: the prototype is cast without a type check.
        return static_cast<LairPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    CStr Lair::GetPropertyName(int id) const
    {
        // RVA 0x833610 - a linear search, since the map is keyed by name.
        for (auto const& property : m_propertiesMap)
        {
            if (property.second == id)
            {
                return property.first;
            }
        }
        return SimplePhysicObj::GetPropertyName(id);
    }

    void Lair::CreateChildren()
    {
        // RVA 0x832F00 - adds an attacking and a defending team (unless the level file already
        // gave the lair some) and the three locations the teams gather at.
        Settlement::CreateChildren();

        if (!GetTeamByType("AttackTeam"))
        {
            CStr const teamName = CStr(GetName()) + STR_ATTACKING_TEAM_SUFFIX;
            int const prototypeId = thePrototypeManager->GetPrototypeId(CStr("settlementTeam"));
            int const teamId = theObjects->CreateNewObject(prototypeId, teamName.c_str(), -1, -1);
            SetTeamByType("AttackTeam", static_cast<Team*>(theObjects->GetEntityByObjId(teamId)));
        }

        if (!GetTeamByType("DefendTeam"))
        {
            CStr const teamName = CStr(GetName()) + STR_DEFENDING_TEAM_SUFFIX;
            int const prototypeId = thePrototypeManager->GetPrototypeId(CStr("settlementTeam"));
            int const teamId = theObjects->CreateNewObject(prototypeId, teamName.c_str(), -1, -1);
            SetTeamByType("DefendTeam", static_cast<Team*>(theObjects->GetEntityByObjId(teamId)));
        }

        CreateChildLocation(Location::LOCATION_DEFEND, "_defend", GetDefendPos());
        CreateChildLocation(Location::LOCATION_DEPLOY, "_deploy", GetDeployPos());
        CreateChildLocation(Location::LOCATION_ATTACK, "_attack", GetAttackPos());
    }

    m3d::Class* Lair::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Settlement);
    }

    eGObjPropertySaveStatus Lair::GetPropertySaveStatus(int id) const
    {
        // RVA 0x833380
        auto it = m_propertiesSaveStatesMap.find(id);
        if (it != m_propertiesSaveStatesMap.end())
        {
            return it->second;
        }
        return SimplePhysicObj::GetPropertySaveStatus(id);
    }

    bool Lair::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        // RVA 0x8326A0
        if (propertyId == 64)
        {
            m_maxDefenders = newValue.GetAsID();
            return true;
        }
        if (propertyId == 66)
        {
            m_maxAttackers = newValue.GetAsID();
            return true;
        }
        return SimplePhysicObj::SetPropertyById(propertyId, newValue);
    }

    int Lair::OnEvent(Event const& evn)
    {
        // RVA 0x833430
        int const result = Settlement::OnEvent(evn);
        if (evn.m_eventId == GE_OBJECT_ENTERS_LOCATION)
        {
            _OnObjectEntersLocation(evn);
            return 1;
        }
        return result;
    }

    void Lair::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        // RVA 0x833590
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.second);
        }
        SimplePhysicObj::GetPropertiesIDs(Props);
    }

    m3d::Class* Lair::GetClass() const
    {
        // RVA 0x832680
        return RT_CLASS_LOCAL(Lair);
    }

    void Lair::Registration()
    {
        // RVA 0x833710
        m_propertiesMap["MaxAttackers"] = 66;
        m_propertiesMap["MaxDefenders"] = 64;
    }

    void Lair::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
    {
        // RVA 0x833510
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.first);
        }
        SimplePhysicObj::GetPropertiesNames(Props);
    }

    void Lair::Update(float elapsedTime, unsigned workTime)
    {
        // RVA 0x833790
        SimplePhysicObj::Update(elapsedTime, workTime);

        // Every SPAWN_TIMEOUT seconds one vehicle of the population is released, filling the
        // defending team first and then the attacking one.
        if (TimeOutFinished())
        {
            if (m_population.value().get() > 0)
            {
                int const maxSpawnedVehicles = _GetMaxSpawnedVehicles();
                if (_GetNumSpawnedVehicles() < maxSpawnedVehicles)
                {
                    Vehicle* vehicle = _SpawnVehicle();
                    if (vehicle)
                    {
                        m_population.value().sub(1);
                        Team* defendingTeam = _GetDefendingTeam();
                        Team* attackingTeam = _GetAttackingTeam();
                        if (defendingTeam && static_cast<int>(defendingTeam->GetNumVehicles()) < m_maxDefenders)
                        {
                            defendingTeam->AddChild(vehicle);
                            if (defendingTeam->GetNumVehicles() == 1)
                            {
                                defendingTeam->SetDestination(GetDefendPos());
                            }
                        }
                        else if (attackingTeam && static_cast<int>(attackingTeam->GetNumVehicles()) < m_maxAttackers)
                        {
                            attackingTeam->AddChild(vehicle);
                            if (attackingTeam->GetNumVehicles() == 1)
                            {
                                attackingTeam->SetDestination(GetAttackPos());
                            }
                        }
                        else
                        {
                            // NOTE: the spawned vehicle is left without a team, and the population
                            // has already been decremented for it.
                            M3D_LOG_ERR(CStr("Error: vehicles counting mismatch"));
                        }
                    }
                    else
                    {
                        M3D_LOG_ERR(CStr("Error: couldn't create vehicle for ") + GetDebugDescription() + CStr(": no room"));
                    }
                }
            }
            SetTimeOut(SPAWN_TIMEOUT);
        }

        switch (m_state)
        {
        case LS_REPRODUCE_WAIT:
            // The breeding timer only runs while the settlement is below its population cap.
            if (_GetOverallPopulation() < _GetMaxOverallPopulaition())
            {
                m_reproductTime.regenerate(elapsedTime);
            }
            if (m_reproductTime.minValue().get() >= m_reproductTime.value().get())
            {
                m_state = LS_REPRODUCE;
            }
            {
                // The lair dies once its defenders are gone and nothing is left to spawn. This is
                // checked after the switch to LS_REPRODUCE, so it wins in the same frame.
                Team* defendingTeam = _GetDefendingTeam();
                if ((!defendingTeam || defendingTeam->GetNumVehicles() == 0) && m_population.value().get() <= 0)
                {
                    m_state = LS_DYING;
                }
            }
            break;

        case LS_REPRODUCE:
            m_population.value().add(1);
            m_reproductTime.setToMax();
            m_state = LS_REPRODUCE_WAIT;
            break;

        case LS_DYING:
            Remove();
            CauseEvent(GE_OBJECT_DIE, 0.0f, m3d::AIParam(), m3d::AIParam());
            m_state = LS_DEAD;
            break;

        default:
            break;
        }
    }

    bool Lair::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x833B00
        if (propertyId == 64)
        {
            retVal = m_maxDefenders;
            return true;
        }
        if (propertyId == 66)
        {
            retVal = m_maxAttackers;
            return true;
        }
        return SimplePhysicObj::_GetPropertyInternal(propertyId, retVal);
    }

    int Lair::_GetMaxSpawnedVehicles()
    {
        // RVA 0x832E30 - the settlement's own limit plus a full attacking and defending team.
        int maxSpawnedVehicles = Settlement::_GetMaxSpawnedVehicles();
        if (GetTeamByType("AttackTeam"))
        {
            maxSpawnedVehicles += m_maxAttackers;
        }
        if (GetTeamByType("DefendTeam"))
        {
            maxSpawnedVehicles += m_maxDefenders;
        }
        return maxSpawnedVehicles;
    }

    // RVA 0x833300
    Lair::~Lair() = default;

    int Lair::_GetNumSpawnedVehicles()
    {
        // RVA 0x832690
        return Settlement::_GetNumSpawnedVehicles();
    }

    void Lair::RegisterProperty(char const* Name, int id, eGObjPropertySaveStatus saveStatus)
    {
        // RVA 0x8336B0 - the default save status is not stored.
        m_propertiesMap[Name] = id;
        if (saveStatus)
        {
            m_propertiesSaveStatesMap[id] = saveStatus;
        }
    }

    bool Lair::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x833BB0 - NOTE: both properties default to 0, not to the prototype's values.
        if (propertyId != 64 && propertyId != 66)
        {
            return SimplePhysicObj::_GetPropertyDefaultInternal(propertyId, retVal);
        }
        retVal = 0;
        return true;
    }

    m3d::Object* Lair::Clone()
    {
        // RVA 0x8327A0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    void Lair::_OnObjectEntersLocation(Event const& evn)
    {
        // RVA 0x833320 - NOTE: an empty handler in the shipped build. It resolves the entering
        // object and asks for its class, but does nothing with either.
        if (evn.m_param1.GetType() == m3d::AIPARAM_ID && evn.m_param2.GetType() == m3d::AIPARAM_ID)
        {
            evn.m_param2.GetAsID();
            Obj* obj = theObjects->GetEntityByObjId(evn.m_param1.GetAsID());
            if (obj)
            {
                obj->GetClass();
            }
        }
    }

    m3d::Object* Lair::CreateObject()
    {
        // RVA 0x832960
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    Team* Lair::_GetDefendingTeam()
    {
        // RVA 0x832750
        return GetTeamByType("DefendTeam");
    }

    Team* Lair::_GetAttackingTeam()
    {
        // RVA 0x832700
        return GetTeamByType("AttackTeam");
    }
}  // namespace ai
