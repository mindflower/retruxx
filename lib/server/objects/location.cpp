#include "location.h"

#include <cstdlib>
#include <stdexcept>

#include "vehicle.h"
#include "monsters/boss02.h"
#include "server/utils.h"
#include "player.h"
#include <m3dapp.h>
#include <server/processmanager.h>
#include "base/prototypemanager.h"
#include "core/kernel.h"
#include "core/log.h"
#include "core/ref_ptr.h"
#include "server/intersectionmanager.h"
#include "server/statistic/statisticmanager.h"
#include <server/server.h>

#include "level.h"
#include "npc.h"
#include "world.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Location)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Location);

    Obj* LocationPrototypeInfo::CreateTargetObject() const
    {
        return new Location(*this);
    }

    LocationPrototypeInfo::LocationPrototypeInfo()
    {
    }

    bool LocationPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::SimplePhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            _SetGeomType(GEOM_TYPE_BOX);
            return 1;
        }
        return result;
    }

    LocationPrototypeInfo const* Location::GetPrototypeInfo() const
    {
        return dynamic_cast<LocationPrototypeInfo const*>(ai::thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    CStr Location::GetPropertyName(int id) const
    {
        for (auto const& [name, propId] : m_propertiesMap)
        {
            if (propId == id)
            {
                return name;
            }
        }
        return ai::SimplePhysicObj::GetPropertyName(id);
    }

    CStr Location::GetLocationNameFromPassageAddress(CStr const& passageAddress)
    {
        retruxx::vector<CStr> tokens;
        m3d::Tokenize(passageAddress, tokens, "(), ;\t");
        if (tokens.size() != 2)
        {
            return {};
        }
        return tokens[1];
    }

    void Location::AddChild(Obj* pObj)
    {
        Obj::AddChild(pObj);
        if (pObj)
        {
            if (pObj->IsKindOf(&ai::Npc::m_classNpc))
            {
                pObj->LinkToParent(GetId(), HIERARCHY_CHILD);
                m_npcs.push_back((Npc*)pObj);
            }
        }
    }

    Location::Location(LocationPrototypeInfo const& prototype) :
        SimplePhysicObj(prototype),
        m_timeForNextCheck(0.0, 0.0, 10.0, -1.0)
    {
        this->m_locationType = LOCATION_GENERIC;
        this->m_toleranceSet.insert(RS_OWN);
        this->m_bIsActive = true;
        this->m_lookingTimeOut = 1.0;
        this->m_bPassageActive = true;

        this->m_targetClasses.insert(&Vehicle::m_classVehicle);
        this->m_targetClasses.insert(&Boss02::m_classBoss02);

        this->m_numFramesPassed = 0;

        this->_GetLookSphere()->SetTargetClasses(m_targetClasses);

        this->DisablePhysics();
        this->DisableGeometry(1);
    }

    std::vector<Npc*, std::allocator<Npc*>> const& Location::GetNpcs() const
    {
        return m_npcs;
    }

    bool Location::CanChildBeAdded(m3d::Class* pClass) const
    {
        return ai::Obj::CanChildBeAdded(pClass) || pClass->IsKindOf(&ai::Npc::m_classNpc);
    }

    int Location::GetPropertyId(char const* propName) const
    {
        auto it = Location::m_propertiesMap.find(propName);
        if (it != Location::m_propertiesMap.end())
        {
            return it->second;
        }

        return ai::SimplePhysicObj::GetPropertyId(propName);
    }

    void Location::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        ai::SimplePhysicObj::LoadRuntimeValues(xmlFile, xmlNode);
        if (!xmlNode->IsEmpty())
        {
            if (char const* framesPassed = xmlNode->GetAttribute("FramesPassed"))
            {
                m_numFramesPassed = atoi(framesPassed);
            }
        }

        ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        for (xmlNode->GetFirstChild(node, "InsideId"); !node->IsEmpty(); node->GetNextSibling(node, "InsideId"))
        {
            int id = 0;
            if (char const* idStr = node->GetAttribute("Id"))
            {
                id = atoi(idStr);
            }
            m_idsWasInside.insert(id);
        }
    }

    m3d::Class* Location::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    void Location::RenderDebugInfo() const
    {
        CVector const pos = GetPosition();
        CVector textPos;
        textPos.x = pos.x;
        textPos.y = ai::GetGroundPos(pos, true, false).y + 2.0f;
        textPos.z = pos.z;
        ai::DebugCircle(pos, _GetLookSphere()->GetRadius(), 0xFFFFFFFF);
        ai::DebugText(textPos, 12.0f, 0.0f, 0xFFFFFFFF, CStr(GetName()));
    }

    void Location::Remove()
    {
        SimplePhysicObj::Remove();
        for (auto& npc : m_npcs)
        {
            npc->Remove();
        }

        for (auto id : m_idsWasInside)
        {
            auto obj = theObjects->GetEntityByObjId(id);
            if (obj)
            {
                OnObjectOut(obj);
            }
        }
    }

    CStr Location::GetLevelNameFromPassageAddress(CStr const& passageAddress)
    {
        retruxx::vector<CStr> tokens;
        m3d::Tokenize(passageAddress, tokens, "(), ;\t");
        // TODO: check this
        if (tokens.size() != 2)
        {
            return {};
        }

        return tokens[0];
    }

    bool Location::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        switch (propertyId)
        {
        case 49:
            SetRadius(newValue.GetAsFloat());
            return true;

        case 52:
            m_lookingTimeOut = newValue.GetAsFloat();
            return true;

        case 53:
        {
            bool const wasActive = m_bIsActive;
            m_bIsActive = (newValue.GetAsID() != 0);
            if (m_locationType == LOCATION_PASSAGE && wasActive != m_bIsActive)
            {
                // 66556 = SM location-state-changed app message
                M3D_APP->EnqueueMessage(66556, GetId(), 0, 0, 0, {}, {});
            }
            return true;
        }

        case 54:
        {
            m_passageAddress = newValue.GetAsStr();
            if (m_passageAddress.empty())
            {
                return true;
            }
            std::vector<CStr> tokens;
            m3d::Tokenize(m_passageAddress, tokens, "(), ;\t");
            if (tokens.size() != 2)
            {
                M3D_LOG_ERR(
                    "Error: Invalid passage address '" + m_passageAddress + "' for location '" + CStr(GetName()));
                m_passageAddress = {};
            }
            return true;
        }

        case 55:
            m_correspondingPassageLocationName = newValue.GetAsStr();
            return true;

        case 56:
            m_bPassageActive = (newValue.GetAsID() != 0);
            return true;

        default:
            return ai::SimplePhysicObj::SetPropertyById(propertyId, newValue);
        }
        return false;
    }

    Location* Location::GetCorrespondingPassageLocation() const
    {
        if (m_correspondingPassageLocationName.empty())
        {
            return nullptr;
        }
        Obj* obj = theObjects->GetEntityByObjName(m_correspondingPassageLocationName);
        if (!obj)
        {
            M3D_LOG_ERR(
                "Error: PassageLocation '" + m_correspondingPassageLocationName + "' does not exist for " +
                GetDebugDescription());
            return nullptr;
        }
        if (!IS_KIND_OF(obj, Location))
        {
            SYS_ERROR("pObj->IsKindOf( RT_CLASS_LOCAL( Location ) )");
        }
        return static_cast<Location*>(obj);
    }

    void Location::SetPositionSelf(CVector const& pos)
    {
        auto groundPos = ai::GetGroundPos(pos, 0, 0);
        ai::PhysicObj::SetPositionSelf(groundPos);
    }

    eGObjPropertySaveStatus Location::GetPropertySaveStatus(int id) const
    {
        auto it = m_propertiesSaveStatesMap.find(id);
        if (it != m_propertiesSaveStatesMap.end())
        {
            return it->second;
        }
        return ai::SimplePhysicObj::GetPropertySaveStatus(id);
    }

    bool Location::IsObjectInside(int objId) const
    {
        return m_idsWasInside.find(objId) != m_idsWasInside.end();
    }

    m3d::Class* Location::GetClass() const
    {
        return RT_CLASS_LOCAL(Location);
    }

    void Location::Update(float elapsedTime, unsigned workTime)
    {
        SimplePhysicObj::Update(elapsedTime, workTime);
        if (elapsedTime >= 0.001 && this->m_bIsActive)
        {
            ++this->m_numFramesPassed;
            m_timeForNextCheck.regenerate(elapsedTime);
            retruxx::set<ref_ptr<ai::Obstacle>> seenObstacles;
            if (this->m_timeForNextCheck.value().get() == this->m_timeForNextCheck.minValue().get() ||
                this->m_numFramesPassed <= 2)
            {
                auto newValue = ((rand() * 0.000030518509) * 0.39999998) + 0.30000001;
                m_timeForNextCheck.value().set(newValue);
                _CheckIncomingOutgoingObjects(seenObstacles);
            }
            if (TimeOutFinished())
            {
                for (auto& id : m_idsWasInside)
                {
                    auto obj = ai::theObjects->GetEntityByObjId(id);
                    if (obj && _MustCheckObject(obj))
                    {
                        CauseEvent(GE_OBJECT_IN_LOCATION, 0.0, {obj->GetId()}, {});
                    }
                }

                SetTimeOut(m_lookingTimeOut);
            }
        }
    }

    void Location::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>& Props) const
    {
        ai::SimplePhysicObj::GetPropertiesNames(Props);
        for (auto const& [name, id] : m_propertiesMap)
        {
            Props.insert(name);
        }
    }

    void Location::SetActive(bool activate)
    {
        m_bIsActive = activate;
    }

    void Location::SetRadius(float radius)
    {
        _GetLookSphere()->SetRadius(radius);
    }

    void Location::Registration()
    {
        m_propertiesMap["Radius"] = 49;
        m_propertiesMap["Active"] = 53;
        m_propertiesMap["LookingTimeOut"] = 52;
        m_propertiesMap["PassageAddress"] = 54;
        m_propertiesMap["ExitLocation"] = 55;
        m_propertiesMap["PassageActive"] = 56;
    }

    void Location::SetLocationType(LocationType locationType)
    {
        m_locationType = locationType;
        if (locationType != LOCATION_GENERIC && locationType != LOCATION_PASSAGE)
        {
            m_toleranceSet.clear();
            m_toleranceSet.insert(RS_ENEMY);
            m_toleranceSet.insert(RS_NEUTRAL);
            m_toleranceSet.insert(RS_ALLY);
            m_toleranceSet.insert(RS_OWN);
        }
    }

    int Location::OnEvent(Event const& evn)
    {
        auto result = ai::Obj::OnEvent(evn);
        if (evn.m_eventId == GE_GAME_START)
        {
            ai::Location::_OnGameStart(evn);
            return 1;
        }
        return result;
    }

    CStr const& Location::GetPassageAddress() const
    {
        return m_passageAddress;
    }

    bool Location::IsPassageActive() const
    {
        return m_bPassageActive;
    }

    void Location::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        ai::SimplePhysicObj::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("FramesPassed", CStr(m_numFramesPassed).c_str());
        for (int id : m_idsWasInside)
        {
            ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "InsideId");
            xmlNode->AddChild(node);
            node->SetAttribute("Id", CStr(id).c_str());
        }
    }

    Location::LocationType Location::GetLocationType() const
    {
        return m_locationType;
    }

    float Location::GetRadius() const
    {
        return _GetLookSphere()->GetRadius();
    }

    void Location::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>& Props) const
    {
        ai::SimplePhysicObj::GetPropertiesIDs(Props);
        for (auto const& [name, id] : m_propertiesMap)
        {
            Props.insert(id);
        }
    }

    bool Location::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        switch (propertyId)
        {
        case 49:
            retVal = _GetLookSphere()->GetRadius();
            return true;
        case 52:
            retVal = m_lookingTimeOut;
            return true;
        case 53:
            retVal = m_bIsActive ? 1 : 0;
            return true;
        case 54:
            retVal = m_passageAddress;
            return true;
        case 55:
            retVal = m_correspondingPassageLocationName;
            return true;
        case 56:
            retVal = m_bPassageActive ? 1 : 0;
            return true;
        default:
            return ai::SimplePhysicObj::_GetPropertyInternal(propertyId, retVal);
        }
    }

    void Location::OnObjectIn(Obj*)
    {
    }

    void Location::OnObjectOut(Obj*)
    {
    }

    void Location::RegisterProperty(char const* Name, int id, eGObjPropertySaveStatus saveStatus)
    {
        m_propertiesMap[Name] = id;
        if (saveStatus)
        {
            m_propertiesSaveStatesMap[id] = saveStatus;
        }
    }

    void Location::_InternalPostLoad()
    {
        SimplePhysicObj::_InternalPostLoad();
        auto v2 = strstr(m_name.c_str(), "_enter");
        if (!v2 || v2 - m_name.c_str() == -1)
        {
            auto v3 = strstr(m_name.c_str(), "_defend");
            if (!v3 || v3 - m_name.c_str() == -1)
            {
                auto v4 = strstr(m_name.c_str(), "_attack");
                if (!v4 || v4 - m_name.c_str() == -1)
                {
                    auto v5 = strstr(m_name.c_str(), "_deploy");
                    if (!v5 || v5 - m_name.c_str() == -1)
                    {
                        if (m_name.findsubstr("_caravan", 0) == -1)
                        {
                            SetLocationType(m_passageAddress.empty() ? LOCATION_GENERIC : LOCATION_PASSAGE);
                        }
                        else
                        {
                            ai::Location::SetLocationType(LOCATION_CARAVAN_ARRIVE);
                        }
                    }
                    else
                    {
                        ai::Location::SetLocationType(LOCATION_DEPLOY);
                    }
                }
                else
                {
                    ai::Location::SetLocationType(LOCATION_ATTACK);
                }
            }
            else
            {
                ai::Location::SetLocationType(LOCATION_DEFEND);
            }
        }
        else
        {
            ai::Location::SetLocationType(LOCATION_ENTER);
        }

        if (!m_correspondingPassageLocationName.empty())
        {
            auto obj = theObjects->GetEntityByObjName(m_correspondingPassageLocationName);
            if (!obj || !IS_KIND_OF(obj, Location))
            {
                M3D_LOG_ERR(
                    "Error: PassageLocation '" + m_correspondingPassageLocationName + "' does not exist for " +
                    GetDebugDescription());
            }
        }

        if (thePlayer)
        {
            // TODO: check this
            m3d::AIParam param(1);
            theProcessManager->PostMessageA(2, thePlayer->GetId(), GetId(), 0.0, param, {}, 1);
        }
    }

    bool Location::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        switch (propertyId)
        {
        case 49:
            retVal = GetPrototypeInfo() ? GetPrototypeInfo()->GetRadius() : 0.0f;
            return true;
        case 52:
            retVal = 1.0f;
            return true;
        case 53:
        case 56:
            retVal = 1;
            return true;
        case 54:
        case 55:
            retVal = CStr();
            return true;
        default:
            return ai::SimplePhysicObj::_GetPropertyDefaultInternal(propertyId, retVal);
        }
    }

    Location::~Location() = default;

    void Location::_OnGameStart(Event const& evn)
    {
        // TODO: check this
        m_timeForNextCheck.setToMin();
    }

    m3d::Object* Location::Clone()
    {
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* Location::CreateObject()
    {
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    bool Location::_MustCheckObject(Obj const* pObj) const
    {
        if (pObj)
        {
            auto cls = pObj->GetClass();
            auto it = m_targetClasses.find(cls);
            if (it != m_targetClasses.end())
            {
                auto belong = GetBelong();
                if (belong == -1 || theRelationship->AmongTolerance(pObj->GetBelong(), belong, m_toleranceSet))
                {
                    return true;
                }
            }
        }
        return false;
    }

    void Location::_CheckIncomingOutgoingObjects(
        std::set<ref_ptr<Obstacle>, std::less<ref_ptr<Obstacle>>, std::allocator<ref_ptr<Obstacle>>>& seenObstacles)
    {
        IntersectionManager::GetIntersectedObjects(seenObstacles, _GetLookSphere(), m_targetClasses, false, false);
        for (auto& obstacle : seenObstacles)
        {
            auto obj = obstacle->GetOwnerPhysicObj();
            if (!obj)
            {
                continue;
            }

            auto it = m_idsWasInside.find(obj->GetId());
            if (it == m_idsWasInside.end())
            {
                if (_MustCheckObject(obj))
                {
                    auto pos = obj->GetPosition();
                    auto scale = ((pos.x * pos.x) + (pos.y * pos.y)) + (pos.z * pos.z);
                    if (std::isnan(sqrt(scale)))
                    {
                        M3D_LOG_ERR(
                            "Error: " + obj->GetDebugDescription() + " is in hyperspace when intersecting with " +
                            GetDebugDescription());
                        continue;
                    }

                    CauseEvent(GE_OBJECT_ENTERS_LOCATION, 0.0, {obj->GetId()}, {});
                    OnObjectIn(obj);

                    if (!m_npcs.empty() && obj == thePlayer->GetVehicle())
                    {
                        // 66554 = "player vehicle entered a location that has NPCs" app message
                        M3D_APP->EnqueueMessage(66554, GetId(), m_npcs.front()->GetId(), 0, 0, {}, {});
                    }

                    if (this->m_locationType != LOCATION_PASSAGE || obj != thePlayer->GetVehicle() ||
                        !this->m_bPassageActive)
                    {
                        continue;
                    }

                    auto levelName = GetLevelNameFromPassageAddress(m_passageAddress);
                    auto locationName = GetLocationNameFromPassageAddress(m_passageAddress);
                    if (levelName.empty() || locationName.empty())
                    {
                        M3D_LOG_ERR(
                            "Error: invalid pasageAddress: '" + m_passageAddress + "' for location '" + m_name + "'");
                    }
                    else
                    {
                        if (!IS_KIND_OF(obj, Vehicle))
                        {
                            M3D_LOG_ERR("Error: seen obj is not a vehicle. Name = '" + CStr(obj->GetName()) + "'");
                        }
                        else
                        {
                            M3D_LOG_INFO(
                                obj->GetDebugDescription() + " pos = " + CStr(obj->GetPosition()) + " in location '" +
                                m_name + "' pos = " + CStr(GetPosition()) + " caused passage to map '" + levelName +
                                "' to location '" + locationName + "'");
                            theStatisticManager->ZeroStatisticsForLevel(pServer->GetWorld()->m_level->m_levelName);
                            theObjects->PassToMap(levelName, locationName, -1, false);
                        }
                    }
                }
            }
            else
            {
                m_idsWasInside.erase(it);
            }
        }

        for (auto& id : m_idsWasInside)
        {
            auto obj = theObjects->GetEntityByObjId(id);
            if (obj)
            {
                if (_MustCheckObject(obj))
                {
                    CauseEvent(GE_OBJECT_LEAVES_LOCATION, 0.0, {obj->GetId()}, {});
                    OnObjectOut(obj);
                }
            }
        }

        m_idsWasInside.clear();

        for (auto& obstacle : seenObstacles)
        {
            auto obj = obstacle->GetOwnerPhysicObj();
            if (obj)
            {
                m_idsWasInside.insert(obj->GetId());
            }
        }
    }
}  // namespace ai
