#include "location.h"

#include <stdexcept>

#include "vehicle.h"
#include "monsters/boss02.h"
#include "server/utils.h"
#include "player.h"
#include <server/processmanager.h>
#include "base/prototypemanager.h"

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

    CStr Location::GetPropertyName(int) const
    {
        throw std::logic_error("Not implemented");
    }

    CStr Location::GetLocationNameFromPassageAddress(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Location::AddChild(Obj*)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    bool Location::CanChildBeAdded(m3d::Class*) const
    {
        throw std::logic_error("Not implemented");
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

    void Location::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Location::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    void Location::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void Location::Remove()
    {
        throw std::logic_error("Not implemented");
    }

    CStr Location::GetLevelNameFromPassageAddress(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool Location::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        switch (propertyId)
        {
        case 49:
            SetRadius(newValue.GetAsFloat());
            return true;

        case 52:
            throw std::logic_error("Not implemented");
            break;

        case 53:
            throw std::logic_error("Not implemented");
            break;

        case 54:
            throw std::logic_error("Not implemented");
            break;

        case 55:
            throw std::logic_error("Not implemented");
            break;

        case 56:
            throw std::logic_error("Not implemented");
            break;

        default:
            return ai::SimplePhysicObj::SetPropertyById(propertyId, newValue);
        }
        return false;
    }

    Location* Location::GetCorrespondingPassageLocation() const
    {
        throw std::logic_error("Not implemented");
    }

    void Location::SetPositionSelf(CVector const& pos)
    {
        auto groundPos = ai::GetGroundPos(pos, 0, 0);
        ai::PhysicObj::SetPositionSelf(groundPos);
    }

    eGObjPropertySaveStatus Location::GetPropertySaveStatus(int) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Location::IsObjectInside(int) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Location::GetClass() const
    {
        return RT_CLASS_LOCAL(Location);
    }

    void Location::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void Location::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Location::SetActive(bool)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    bool Location::IsPassageActive() const
    {
        throw std::logic_error("Not implemented");
    }

    void Location::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    Location::LocationType Location::GetLocationType() const
    {
        throw std::logic_error("Not implemented");
    }

    float Location::GetRadius() const
    {
        throw std::logic_error("Not implemented");
    }

    void Location::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Location::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Location::OnObjectIn(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void Location::OnObjectOut(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void Location::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw std::logic_error("Not implemented");
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
                            SetLocationType(m_passageAddress.empty() ? LOCATION_PASSAGE : LOCATION_GENERIC);
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
            throw std::logic_error("Not implemented");
        }

        if (thePlayer)
        {
            // TODO: check this
            m3d::AIParam param(1);
            theProcessManager->PostMessageA(2, thePlayer->GetId(), GetId(), 0.0, param, {}, 1);
        }
    }

    bool Location::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    Location::~Location()
    {
        throw std::logic_error("Not implemented");
    }

    void Location::_OnGameStart(Event const& evn)
    {
        // TODO: check this
        m_timeForNextCheck.setToMin();
    }

    m3d::Object* Location::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Location::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    bool Location::_MustCheckObject(Obj const*) const
    {
        throw std::logic_error("Not implemented");
    }

    void Location::_CheckIncomingOutgoingObjects(
        std::set<ref_ptr<Obstacle>, std::less<ref_ptr<Obstacle>>, std::allocator<ref_ptr<Obstacle>>>&)
    {
        throw std::logic_error("Not implemented");
    }
}
