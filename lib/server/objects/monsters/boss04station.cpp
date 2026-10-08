#include "boss04station.h"

#include <stdexcept>

#include "core/kernel.h"
#include "core/log.h"
#include "server/damageinfo.h"
#include "server/objects/base/prototypemanager.h"
#include "server/objects/physicbodies/vehiclepart.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Boss04Station)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Boss04Station);

    bool Boss04StationPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x805DC0
        return ComplexPhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    Boss04StationPrototypeInfo::Boss04StationPrototypeInfo() = default;

    void Boss04StationPrototypeInfo::PostLoad()
    {
        // RVA 0x805DE0
        ComplexPhysicObjPrototypeInfo::PostLoad();
    }

    Obj* Boss04StationPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x805EE0
        return new Boss04Station(*this);
    }

    void Boss04Station::EvaluateToDead()
    {
        // RVA 0x8067F0 - reported once, when the last critical part of the station is gone.
        if (!m_bDestroyed)
        {
            CauseEvent(GE_BOSS04_STATION_DESTROYED, 0.0f, m3d::AIParam(), m3d::AIParam());
            m_bDestroyed = true;
        }
    }

    Boss04Station::Boss04Station(Boss04StationPrototypeInfo const& prototype) : ComplexPhysicObj(prototype)
    {
        // RVA 0x805DF0
        m_bDestroyed = false;
        ComplexPhysicObj::DisablePhysics();
    }

    void Boss04Station::Update(float elapsedTime, unsigned workTime)
    {
        // RVA 0x805E70
        // NOTE: calls PhysicObj::Update directly, skipping ComplexPhysicObj's.
        PhysicObj::Update(elapsedTime, workTime);
    }

    void Boss04Station::InflictDamage(DamageInfo const& damageInfo)
    {
        // RVA 0x806370
        // Hits only break the model; the station's parts track their health from their mesh
        // groups.
        if (damageInfo.damage < 0.0099999998)
        {
            return;
        }
        SetLastDamageSource(damageInfo.attackerId);
        if (damageInfo.damagedPartName.empty())
        {
            M3D_LOG_ERR(CStr("Error: vehicle part with empty name damaged"));
            return;
        }
        VehiclePart* const part = GetPartByName(damageInfo.damagedPartName);
        if (!part)
        {
            M3D_LOG_ERR(CStr("Error: unknown vehicle part damaged: '") + damageInfo.damagedPartName + CStr("'"));
            return;
        }
        if (damageInfo.damageType != DAMAGE_WATER)
        {
            VehiclePart::BreakData breakData;
            breakData.point = damageInfo.hitPos;
            breakData.dir = damageInfo.hitDir;
            breakData.normal = damageInfo.normal;
            breakData.damage = damageInfo.damage;
            breakData.decalId = damageInfo.decalId;
            part->BreakModel(breakData);
        }
    }

    m3d::Class* Boss04Station::GetClass() const
    {
        // RVA 0x805D90
        return RT_CLASS_LOCAL(Boss04Station);
    }

    m3d::Class* Boss04Station::GetBaseClass()
    {
        // RVA 0x805D80
        return RT_CLASS_LOCAL(ComplexPhysicObj);
    }

    Boss04StationPrototypeInfo const* Boss04Station::GetPrototypeInfo() const
    {
        // RVA 0x8065A0
        return static_cast<Boss04StationPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    void Boss04Station::RegisterProperty(char const* name, int id, eGObjPropertySaveStatus saveStatus)
    {
        // Declared in the PDB but never emitted in the shipped build (Registration is empty).
        m_propertiesMap[CStr(name)] = id;
        m_propertiesSaveStatesMap[id] = saveStatus;
    }

    void Boss04Station::Registration()
    {
        // RVA 0x805E80 - registers nothing.
    }

    eGObjPropertySaveStatus Boss04Station::GetPropertySaveStatus(int id) const
    {
        // RVA 0x8065D0
        auto it = m_propertiesSaveStatesMap.find(id);
        if (it != m_propertiesSaveStatesMap.end())
        {
            return it->second;
        }
        return PhysicObj::GetPropertySaveStatus(id);
    }

    void Boss04Station::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
    {
        // Declared in the PDB but never emitted in the shipped build; the usual property-map pattern.
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.first);
        }
        PhysicObj::GetPropertiesNames(Props);
    }

    void Boss04Station::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        // Declared in the PDB but never emitted in the shipped build; the usual property-map pattern.
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.second);
        }
        PhysicObj::GetPropertiesIDs(Props);
    }

    CStr Boss04Station::GetPropertyName(int id) const
    {
        // RVA 0x806780
        for (auto const& property : m_propertiesMap)
        {
            if (property.second == id)
            {
                return property.first;
            }
        }
        return PhysicObj::GetPropertyName(id);
    }

    bool Boss04Station::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        // RVA 0x805E40
        return PhysicObj::SetPropertyById(propertyId, newValue);
    }

    int Boss04Station::GetPropertyId(char const* PropertyName) const
    {
        // RVA 0x806610
        auto it = m_propertiesMap.find(PropertyName);
        if (it != m_propertiesMap.end())
        {
            return it->second;
        }
        return PhysicObj::GetPropertyId(PropertyName);
    }

    bool Boss04Station::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x805E50
        return PhysicObj::_GetPropertyDefaultInternal(propertyId, retVal);
    }

    bool Boss04Station::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x805E30
        return PhysicObj::_GetPropertyInternal(propertyId, retVal);
    }

    void Boss04Station::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x8D00F0
        ComplexPhysicObj::LoadFromXML(xmlFile, xmlNode);
    }

    void Boss04Station::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x805F60
        ComplexPhysicObj::LoadRuntimeValues(xmlFile, xmlNode);
        m3d::SafeBoolAttrib(m_bDestroyed, xmlNode, "IsDestroyed");
    }

    void Boss04Station::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x805E60
        ComplexPhysicObj::SaveToXML(xmlFile, xmlNode);
    }

    void Boss04Station::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x806310
        ComplexPhysicObj::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("IsDestroyed", CStr(static_cast<int>(m_bDestroyed)).c_str());
    }

    bool Boss04Station::bDestroyed() const
    {
        // RVA 0x73D6F0
        return m_bDestroyed;
    }

    void Boss04Station::_InternalPostLoad()
    {
        // RVA 0x805E90
        // NOTE: calls PhysicObj::_InternalPostLoad directly, skipping ComplexPhysicObj's.
        PhysicObj::_InternalPostLoad();
    }

    void Boss04Station::_InternalCreateVisualPart()
    {
        // RVA 0x805EA0
        ComplexPhysicObj::_InternalCreateVisualPart();
        _SetStatic();
    }

    Boss04Station::~Boss04Station()
    {
        // RVA 0x805E20
    }

    m3d::Object* Boss04Station::CreateObject()
    {
        // RVA 0x806150
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* Boss04Station::Clone()
    {
        // RVA 0x805F90
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai
