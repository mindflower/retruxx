#include "boss04drone.h"

#include <cmath>
#include <stdexcept>

#include "cinematic.h"
#include "m3dapp.h"
#include "core/kernel.h"
#include "core/log.h"
#include "server/damageinfo.h"
#include "server/dynamicscene.h"
#include "server/modifier.h"
#include "server/weaponfirer.h"
#include "server/objects/vehicle.h"
#include "server/objects/base/prototypemanager.h"
#include "server/objects/physicbodies/vehiclepart.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Boss04Drone, SetCustomControl)
{
    // RVA 0x743AE0
    auto* drone = static_cast<ai::Boss04Drone*>(context->asObject(0, "Boss04Drone"));
    drone->SetCustomControl(context->asBool(1));
    return 1;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Boss04Drone)
    RT_CLASS_EXPORT(Boss04Drone, m3d::METHOD, SetCustomControl, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Boss04Drone);

    Obj* Boss04DronePrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x744150
        return new Boss04Drone(*this);
    }

    Boss04DronePrototypeInfo::Boss04DronePrototypeInfo()
    {
        // RVA 0x743A70
        m_maxLinearVelocity = 0.0f;
    }

    bool Boss04DronePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x743B40
        bool const result = ComplexPhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_maxLinearVelocity, xmlNode, "MaxLinearVelocity");
        }
        return result;
    }

    Boss04Drone::Boss04Drone(Boss04DronePrototypeInfo const& prototype) : ComplexPhysicObj(prototype)
    {
        // RVA 0x743B90
        m_currentFlyPath = nullptr;
        m_currentFlyTime = 0.0f;
        m_bCustomControl = false;
    }

    Boss04Drone::~Boss04Drone()
    {
        // RVA 0x744650
        delete m_currentFlyPath;
        m_currentFlyPath = nullptr;
    }

    m3d::Object* Boss04Drone::Clone()
    {
        // RVA 0x743DD0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* Boss04Drone::CreateObject()
    {
        // RVA 0x743F90
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Class* Boss04Drone::GetBaseClass()
    {
        // RVA 0x743A50
        return RT_CLASS_LOCAL(ComplexPhysicObj);
    }

    m3d::Class* Boss04Drone::GetClass() const
    {
        // RVA 0x743A60
        return RT_CLASS_LOCAL(Boss04Drone);
    }

    Boss04DronePrototypeInfo const* Boss04Drone::GetPrototypeInfo() const
    {
        // RVA 0x7442E0
        return static_cast<Boss04DronePrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    void Boss04Drone::RegisterProperty(char const* name, int id, eGObjPropertySaveStatus saveStatus)
    {
        // Declared in the PDB but never emitted in the shipped build (Registration is empty).
        m_propertiesMap[CStr(name)] = id;
        m_propertiesSaveStatesMap[id] = saveStatus;
    }

    void Boss04Drone::Registration()
    {
        // RVA 0x743A90 - registers nothing.
    }

    eGObjPropertySaveStatus Boss04Drone::GetPropertySaveStatus(int id) const
    {
        // RVA 0x744310
        auto it = m_propertiesSaveStatesMap.find(id);
        if (it != m_propertiesSaveStatesMap.end())
        {
            return it->second;
        }
        return PhysicObj::GetPropertySaveStatus(id);
    }

    void Boss04Drone::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
    {
        // Declared in the PDB but never emitted in the shipped build; the usual property-map pattern.
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.first);
        }
        PhysicObj::GetPropertiesNames(Props);
    }

    void Boss04Drone::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        // Declared in the PDB but never emitted in the shipped build; the usual property-map pattern.
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.second);
        }
        PhysicObj::GetPropertiesIDs(Props);
    }

    CStr Boss04Drone::GetPropertyName(int id) const
    {
        // RVA 0x7444C0
        for (auto const& property : m_propertiesMap)
        {
            if (property.second == id)
            {
                return property.first;
            }
        }
        return PhysicObj::GetPropertyName(id);
    }

    bool Boss04Drone::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        // RVA 0x743AB0
        return PhysicObj::SetPropertyById(propertyId, newValue);
    }

    int Boss04Drone::GetPropertyId(char const* PropertyName) const
    {
        // RVA 0x744350
        auto it = m_propertiesMap.find(PropertyName);
        if (it != m_propertiesMap.end())
        {
            return it->second;
        }
        return PhysicObj::GetPropertyId(PropertyName);
    }

    bool Boss04Drone::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x743AC0
        return PhysicObj::_GetPropertyDefaultInternal(propertyId, retVal);
    }

    bool Boss04Drone::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x743AA0
        return PhysicObj::_GetPropertyInternal(propertyId, retVal);
    }

    bool Boss04Drone::CanChildBeAdded(m3d::Class*) const
    {
        // RVA 0x743BE0
        return false;
    }

    void Boss04Drone::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x744530
        ComplexPhysicObj::LoadRuntimeValues(xmlFile, xmlNode);
        m3d::SafeStrAttrib(m_flyPathName, xmlNode, "FlyPathName");
        m3d::SafeFloatAttrib(m_currentFlyTime, xmlNode, "CurrentFlyTime");
        m3d::SafeBoolAttrib(m_bCustomControl, xmlNode, "CustomControl");
        ref_ptr flyPathNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        xmlNode->GetFirstChild(flyPathNode, "CurrentFlyPath");
        if (!flyPathNode->IsEmpty())
        {
            // NOTE: a path the drone already has is leaked.
            m_currentFlyPath = new m3d::CameraPath();
            m_currentFlyPath->LoadFromXmlRuntime(xmlFile, flyPathNode);
        }
    }

    void Boss04Drone::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x7441C0
        ComplexPhysicObj::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("FlyPathName", m_flyPathName.c_str());
        xmlNode->SetAttribute("CurrentFlyTime", CStr(m_currentFlyTime).c_str());
        xmlNode->SetAttribute("CustomControl", CStr(static_cast<int>(m_bCustomControl)).c_str());
        if (m_currentFlyPath)
        {
            ref_ptr flyPathNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "CurrentFlyPath");
            m_currentFlyPath->SaveToXmlRuntime(xmlFile, flyPathNode);
            xmlNode->AddChild(flyPathNode);
        }
    }

    void Boss04Drone::Update(float elapsedTime, unsigned)
    {
        // RVA 0x744B70
        // Flies its path over and over, facing and shooting at the player, unless a script has
        // taken control. When the main part is destroyed the drone falls apart.
        // NOTE: no base Update is called.
        if (!m_currentFlyPath)
        {
            SYS_ERROR("m_currentFlyPath");
        }
        if (!IsAlive())
        {
            return;
        }
        VehiclePart* const mainPart = GetPartByName(CStr("MAIN"));
        if (!mainPart)
        {
            // NOTE: the assertion names a gun, left over from copied code; the part is used anyway.
            SYS_ERROR("theGun");
        }
        if (mainPart->Durability().value().get() == mainPart->Durability().minValue().get())
        {
            SetSkin(8);
            Flow(mainPart, 0.0f);
            Remove();
        }
        else if (!m_bCustomControl)
        {
            CVector pos = GetPosition();
            Quaternion qdummy;
            float fdummy;
            m_currentFlyPath->GetCameraForTime(m_currentFlyTime, pos, qdummy, fdummy);
            SetPosition(pos);
            m_currentFlyTime += elapsedTime;
            if (m_currentFlyTime >= m_currentFlyPath->GetFullTime())
            {
                _RecalcFlyPath();
            }
            _UpdateLookAtPlayer();
            WeaponFirer::AimAndFireFromWeapons(this, true, elapsedTime, gDynamicScene->GetVehicleControlledByPlayer());
        }
    }

    void Boss04Drone::SetFlyPathName(CStr const& flyPathName)
    {
        // RVA 0x744FB0
        m_flyPathName = flyPathName;
        _RecalcFlyPath();
    }

    void Boss04Drone::InflictDamage(DamageInfo const& damageInfo)
    {
        // RVA 0x744700 - only the hit part wears down; the drone has no health of its own.
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
        float const damage = damageInfo.damage;
        Modifier modToDurability;
        modToDurability.Create("dur", MO_SUB, m3d::AIParam(damage));
        part->AddModifier(modToDurability);
        if (bIsUpdatingByODE() && damageInfo.damageType != DAMAGE_WATER)
        {
            VehiclePart::BreakData breakData;
            breakData.point = damageInfo.hitPos;
            breakData.dir = damageInfo.hitDir;
            breakData.normal = damageInfo.normal;
            breakData.damage = damage;
            breakData.decalId = damageInfo.decalId;
            part->BreakModel(breakData);
        }
    }

    float Boss04Drone::GetMaxHealth() const
    {
        // RVA 0x743CD0
        if (VehiclePart const* const part = GetPartByName(CStr("MAIN")))
        {
            return part->Durability().maxValue().get();
        }
        return 0.0f;
    }

    float Boss04Drone::GetHealth() const
    {
        // RVA 0x743D50
        if (VehiclePart const* const part = GetPartByName(CStr("MAIN")))
        {
            return part->Durability().value().get();
        }
        return 0.0f;
    }

    void Boss04Drone::SetCustomControl(bool bCustomControl)
    {
        // RVA 0x743AD0
        m_bCustomControl = bCustomControl;
    }

    void Boss04Drone::_RecalcFlyPath()
    {
        // RVA 0x7449F0
        // A fresh copy of the named path starting from where the drone is now, timed for its
        // maximum speed.
        // NOTE: the camera paths file is reloaded every time.
        m3d::Cinematic* const cinematic = M3D_APP->m_cinematic;
        cinematic->Load("camera_paths.xml");
        delete m_currentFlyPath;
        m_currentFlyPath = nullptr;
        m_currentFlyPath = new m3d::CameraPath(cinematic->GetPathByName(m_flyPathName));
        m3d::CameraPathState state;
        state.m_rotation = GetRotation();
        state.m_point = GetPosition();
        state.m_zoom = 1.0f;
        state.m_speed = 1.0f;
        m_currentFlyPath->insert(0, state);
        m_currentFlyPath->CalcFullLength(1);
        m_currentFlyPath->SetFullTime(m_currentFlyPath->GetFullLength() / GetPrototypeInfo()->m_maxLinearVelocity);
        m_currentFlyPath->CalcFlyTimes(1, false);
        m_currentFlyTime = 0.0f;
    }

    void Boss04Drone::_UpdateLookAtPlayer()
    {
        // RVA 0x743BF0
        Vehicle* const playerVehicle = gDynamicScene->GetVehicleControlledByPlayer();
        if (!playerVehicle)
        {
            return;
        }
        CVector const pos = GetPosition();
        CVector const playerPos = playerVehicle->GetPosition();
        float const dx = playerPos.x - pos.x;
        float const dy = playerPos.y - pos.y;
        float const dz = playerPos.z - pos.z;
        float const invLen = static_cast<float>(1.0 / sqrt(dz * dz + dx * dx + dy * dy + 1.1920929e-7));
        SetDirection(CVector(invLen * dx, invLen * dy, dz * invLen));
    }
}  // namespace ai
