#include "settlement.h"

#include <stdexcept>

#include "core/ini.h"
#include "base/prototypemanager.h"
#include <core/log.h>
#include <server/utils.h>
#include "staticautogun.h"
#include "team.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Settlement)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Settlement);

    SettlementPrototypeInfo::auxZoneInfo::auxZoneInfo(auxZoneInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Settlement::Registration()
    {
    }

    void Settlement::Remove()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Settlement::Settlement(SettlementPrototypeInfo const& prototype) : SimplePhysicObj(prototype), m_population(5, 0, 5)
    {
        DisablePhysics();
        _SetStatic();
    }

    void Settlement::CreateChildren()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector Settlement::GetDeployPos() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Settlement::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector Settlement::GetDefendPos() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Location* Settlement::GetLocation(Location::LocationType) const
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Settlement::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    int Settlement::OnEvent(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Settlement::SetPosition(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Team* Settlement::GetTeamByType(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Settlement::SetTeamByType(CStr const&, Team*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Settlement::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        Obj::LoadFromXML(xmlFile, xmlNode);
        // TODO: implement Settlement::LoadFromXML
    }

    void Settlement::SetPositionSelf(CVector const& pos)
    {
        // TODO: check this
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Settlement::RemoveChild(Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Settlement::SetRotation(Quaternion const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector Settlement::GetAttackPos() const
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    Vehicle* Settlement::_SpawnVehicle()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Settlement::_GetNumSpawnedVehicles()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Settlement::_GetOverallPopulation()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Settlement::CreateChildLocation(Location::LocationType, CStr const&, CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Settlement::_GetMaxOverallPopulaition()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Settlement::_InternalPostLoad()
    {
        SimplePhysicObj::_InternalPostLoad();
    }

    int Settlement::_GetMaxSpawnedVehicles()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Settlement::~Settlement()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Settlement::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Settlement::FillingFeedBackParam()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Settlement::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
