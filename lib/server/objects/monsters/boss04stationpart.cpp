#include "boss04stationpart.h"

#include <algorithm>
#include <stdexcept>

#include "m3dapp.h"
#include "skelmodel.h"
#include "core/kernel.h"
#include "scene/servers/dataserver.h"
#include "server/objects/base/prototypemanager.h"
#include "server/objects/monsters/boss04station.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Boss04StationPart)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Boss04StationPart);

    Boss04StationPartPrototypeInfo::Boss04StationPartPrototypeInfo()
    {
        // RVA 0x804E90
        m_bCollisionTrimeshAllowed = true;
        m_maxHealth = 0.0;
    }

    void Boss04StationPartPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x8059A0
        // The critical mesh groups are those named in CriticalMeshGroups; together they make up
        // the part's health.
        VehiclePartPrototypeInfo::RefreshFromXml(xmlFile, xmlNode);
        if (!m_criticalMeshGroupIds.empty())
        {
            return;
        }
        m_maxHealth = 0.0f;
        CStr strCriticalMeshGroups;
        m3d::SafeStrAttrib(strCriticalMeshGroups, xmlNode, "CriticalMeshGroups");
        retruxx::vector<CStr> meshGroupsArr;
        m3d::Tokenize(strCriticalMeshGroups, meshGroupsArr, "(), ;\t");
        m3d::AnimatedModel* mdl = nullptr;
        M3D_APP->GetAnimatedModelsServer().GetItemProperty(m_engineModelId, 16394, &mdl);
        if (!mdl)
        {
            return;
        }
        for (unsigned i = 0; i < mdl->GetGroupsNum(); ++i)
        {
            char const* const groupName = mdl->GetGroup(i).Name.c_str();
            if (std::find(meshGroupsArr.begin(), meshGroupsArr.end(), groupName) != meshGroupsArr.end())
            {
                m_criticalMeshGroupIds.push_back(static_cast<int>(i));
                m_maxHealth = m_maxHealth + m_groupHealthes[i];
            }
        }
    }

    Obj* Boss04StationPartPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x805820
        return new Boss04StationPart(*this);
    }

    bool Boss04StationPartPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x804340
        return VehiclePartPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    Boss04StationPart::MeshGroupInfo::MeshGroupInfo(int groupId, float health) : m_groupId(groupId), m_health(health)
    {
        // RVA 0x804300
    }

    void Boss04StationPart::RegisterProperty(char const* name, int id, eGObjPropertySaveStatus saveStatus)
    {
        // Declared in the PDB but never emitted in the shipped build (Registration is empty).
        m_propertiesMap[CStr(name)] = id;
        m_propertiesSaveStatesMap[id] = saveStatus;
    }

    void Boss04StationPart::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        // Declared in the PDB but never emitted in the shipped build; the usual property-map pattern.
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.second);
        }
        VehiclePart::GetPropertiesIDs(Props);
    }

    m3d::Class* Boss04StationPart::GetClass() const
    {
        // RVA 0x804330
        return RT_CLASS_LOCAL(Boss04StationPart);
    }

    int Boss04StationPart::GetPropertyId(char const* PropertyName) const
    {
        // RVA 0x805410
        auto it = m_propertiesMap.find(PropertyName);
        if (it != m_propertiesMap.end())
        {
            return it->second;
        }
        return VehiclePart::GetPropertyId(PropertyName);
    }

    CStr Boss04StationPart::GetPropertyName(int id) const
    {
        // RVA 0x805580
        for (auto const& property : m_propertiesMap)
        {
            if (property.second == id)
            {
                return property.first;
            }
        }
        return VehiclePart::GetPropertyName(id);
    }

    eGObjPropertySaveStatus Boss04StationPart::GetPropertySaveStatus(int id) const
    {
        // RVA 0x8053D0
        auto it = m_propertiesSaveStatesMap.find(id);
        if (it != m_propertiesSaveStatesMap.end())
        {
            return it->second;
        }
        return VehiclePart::GetPropertySaveStatus(id);
    }

    Boss04StationPartPrototypeInfo const* Boss04StationPart::GetPrototypeInfo() const
    {
        // RVA 0x804E60
        return static_cast<Boss04StationPartPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    Boss04StationPart::Boss04StationPart(Boss04StationPartPrototypeInfo const& prototype) : VehiclePart(prototype)
    {
        // RVA 0x805390
    }

    float Boss04StationPart::GetHealth() const
    {
        // RVA 0x804D80
        float health = 0.0f;
        for (auto const& info : m_meshGroupInfos)
        {
            health = info.m_health + health;
        }
        return health;
    }

    void Boss04StationPart::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
    {
        // Declared in the PDB but never emitted in the shipped build; the usual property-map pattern.
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.first);
        }
        VehiclePart::GetPropertiesNames(Props);
    }

    void Boss04StationPart::Registration()
    {
        // RVA 0x804360 - registers nothing.
    }

    void Boss04StationPart::Update(float elapsedTime, unsigned workTime)
    {
        // RVA 0x805B50 - once every critical mesh group is destroyed the station is told.
        VehiclePart::Update(elapsedTime, workTime);
        if (elapsedTime < 0.001)
        {
            return;
        }
        _UpdateMeshGroupsHealth();
        if (m_meshGroupInfos.size() != m_prevMeshGroupInfos.size())
        {
            SYS_ERROR("m_meshGroupInfos.size() == m_prevMeshGroupInfos.size()");
        }
        unsigned numDestroyed = 0;
        for (auto const& info : m_meshGroupInfos)
        {
            if (info.m_health < 0.000099999997)
            {
                ++numDestroyed;
            }
        }
        if (numDestroyed == m_prevMeshGroupInfos.size())
        {
            // NOTE: the owner is assumed to be a Boss04Station without a type check.
            static_cast<Boss04Station*>(GetOwner())->EvaluateToDead();
        }
    }

    m3d::Class* Boss04StationPart::GetBaseClass()
    {
        // RVA 0x804320
        return RT_CLASS_LOCAL(VehiclePart);
    }

    bool Boss04StationPart::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        // RVA 0x804380
        return VehiclePart::SetPropertyById(propertyId, newValue);
    }

    void Boss04StationPart::_InternalPostLoad()
    {
        // RVA 0x805D60
        // NOTE: does not chain to VehiclePart::_InternalPostLoad.
        _UpdateMeshGroupsHealth();
        m_prevMeshGroupInfos = m_meshGroupInfos;
    }

    bool Boss04StationPart::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x804390
        return VehiclePart::_GetPropertyDefaultInternal(propertyId, retVal);
    }

    Boss04StationPart::~Boss04StationPart()
    {
        // RVA 0x8055F0
    }

    bool Boss04StationPart::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x804370
        return VehiclePart::_GetPropertyInternal(propertyId, retVal);
    }

    m3d::Object* Boss04StationPart::CreateObject()
    {
        // RVA 0x8049C0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* Boss04StationPart::Clone()
    {
        // RVA 0x804800
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    void Boss04StationPart::_UpdateMeshGroupsHealth()
    {
        // RVA 0x8058B0
        Boss04StationPartPrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        m_meshGroupInfos.clear();
        m_meshGroupInfos.reserve(prototypeInfo->m_criticalMeshGroupIds.size());
        for (int const groupId : prototypeInfo->m_criticalMeshGroupIds)
        {
            m_meshGroupInfos.push_back(MeshGroupInfo(groupId, _GetModelPartHealth(groupId)));
        }
    }
}  // namespace ai
