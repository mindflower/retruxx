#include "staticautogun.h"
#include "vehicle.h"
#include <cassert>
#include <cstdlib>
#include <core/ini.h>
#include <core/kernel.h>
#include <m3dapp.h>
#include <server/utils.h>
#include "base/prototypemanager.h"
#include "base/globalproperties.h"
#include <ode/objects.h>
#include <ode/odecpp.h>
#include "physicbodies/vehiclepart.h"
#include "guns/gun.h"
#include "guns/compoundgun.h"
#include "guns/bullet.h"
#include "server/objects/base/objcontainer.h"
#include "../intersectionmanager.h"
#include "../obstacle.h"
#include "../relationship.h"

namespace ai
{
    namespace
    {
        CStr const STR_CANNON = "CANNON";
        CStr const STR_DOT = "DOT";
    }  // namespace

    RT_CLASS_EXPORTS_BEGIN(StaticAutoGun)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(StaticAutoGun);

    StaticAutoGunPrototypeInfo::StaticAutoGunPrototypeInfo()
    {
        m_maxHealth = 1.0f;
    }

    Obj* StaticAutoGunPrototypeInfo::CreateTargetObject() const
    {
        return new StaticAutoGun(*this);
    }

    bool StaticAutoGunPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = ComplexPhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            if (m_parentPrototypeName.empty())
            {
                m3d::SafeFloatAttrib(m_maxHealth, xmlNode, "MaxHealth");
                m3d::SafeStrAttrib(m_destroyedModelName, xmlNode, "DestroyedModel");
            }
        }
        return result;
    }

    void StaticAutoGunPrototypeInfo::_InternalCopyFrom(PrototypeInfo const& rhs)
    {
        // RVA 0x7436D0 - a plain assignment from another prototype of the same class.
        *this = static_cast<StaticAutoGunPrototypeInfo const&>(rhs);
    }

    void StaticAutoGun::WeaponLookAtPoint(CVector const& lookAt, float elapsedTime)
    {
        // RVA 0x742080 - NOTE: the "CANNON" part is used as a Gun without any type check.
        auto* cannon = static_cast<Gun*>(GetPartByName(STR_CANNON));
        if (cannon)
        {
            cannon->LookAtPoint(lookAt, elapsedTime);
        }
    }

    void StaticAutoGun::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        // RVA 0x743010
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.second);
        }
        PhysicObj::GetPropertiesIDs(Props);
    }

    bool StaticAutoGun::RemoveChild(Obj* pObj)
    {
        // RVA 0x742270 - the turret parents the bullets it fires (see AddChild) and lets go of them
        // here. NOTE: whatever ComplexPhysicObj::RemoveChild returns is ignored.
        ComplexPhysicObj::RemoveChild(pObj);
        if (!pObj || !pObj->IsKindOf(&Bullet::m_classBullet) || pObj->m_parentId != GetId())
        {
            return false;
        }
        pObj->m_parentId = -1;
        return true;
    }

    m3d::Class* StaticAutoGun::GetClass() const
    {
        return RT_CLASS_LOCAL(StaticAutoGun);
    }

    NumericInRange<float> const& StaticAutoGun::Health() const
    {
        return m_health;
    }

    NumericInRange<float>& StaticAutoGun::Health()
    {
        return m_health;
    }

    eGObjPropertySaveStatus StaticAutoGun::GetPropertySaveStatus(int id) const
    {
        // RVA 0x742EE0
        auto it = m_propertiesSaveStatesMap.find(id);
        if (it != m_propertiesSaveStatesMap.end())
        {
            return it->second;
        }
        return PhysicObj::GetPropertySaveStatus(id);
    }

    CStr StaticAutoGun::GetPropertyName(int id) const
    {
        // RVA 0x743090 - a linear search, since the map is keyed by name.
        for (auto const& property : m_propertiesMap)
        {
            if (property.second == id)
            {
                return property.first;
            }
        }
        return PhysicObj::GetPropertyName(id);
    }

    StaticAutoGunPrototypeInfo const* StaticAutoGun::GetPrototypeInfo() const
    {
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), StaticAutoGunPrototypeInfo const);
    }

    int StaticAutoGun::GetPropertyId(char const* propName) const
    {
        auto it = StaticAutoGun::m_propertiesMap.find(propName);
        if (it != StaticAutoGun::m_propertiesMap.end())
        {
            return it->second;
        }

        return ComplexPhysicObj::GetPropertyId(propName);
    }

    void StaticAutoGun::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x7421D0 - only the current values are persisted; the maximum health comes back
        // from the prototype on load.
        ComplexPhysicObj::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("Health", CStr(m_health.value().get()).c_str());
        xmlNode->SetAttribute("TimeForNextCheck", CStr(m_timeForNextCheck.value().get()).c_str());
    }

    void StaticAutoGun::RenderDebugInfo() const
    {
        // RVA 0x7420D0
        ComplexPhysicObj::RenderDebugInfo();
    }

    void StaticAutoGun::Registration()
    {
        // RVA 0x743190
        m_propertiesMap["Health"] = 26;
    }

    bool StaticAutoGun::PointIsReachableForGun(
        CVector const& pos,
        retruxx::vector<int, retruxx::allocator<int>> const& exceptions)
    {
        // RVA 0x742CA0 - NOTE: the "CANNON" part is used as a Gun without any type check.
        auto* cannon = static_cast<Gun*>(GetPartByName(STR_CANNON));
        if (!cannon)
        {
            return false;
        }
        return cannon->PointIsReachable(pos, exceptions);
    }

    Gun* StaticAutoGun::getGun()
    {
        // RVA 0x742050 - NOTE: the "CANNON" part is returned as a Gun without any type check.
        return static_cast<Gun*>(GetPartByName(STR_CANNON));
    }

    void StaticAutoGun::GetPropertiesNames(
        retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
    {
        // RVA 0x742F90
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.first);
        }
        PhysicObj::GetPropertiesNames(Props);
    }

    void StaticAutoGun::InflictDamage(DamageInfo const& damageInfo)
    {
        // RVA 0x7431D0 - a turret has no parts to wear down: the whole hit comes off its health.
        if (damageInfo.damage < 0.0099999998f)
        {
            return;
        }

        Obj* attacker = theObjects->GetEntityByObjId(damageInfo.attackerId);

        // Friendly fire is ignored unless the hit explicitly asks for it.
        if (!damageInfo.bDamageFriends && attacker &&
            theRelationship->GetTolerance(attacker->GetBelong(), GetBelong()) >= 3.0f)
        {
            return;
        }

        Modifier modToHealth;
        modToHealth.Create("hp", MO_SUB, m3d::AIParam(damageInfo.damage));
        modToHealth.m_SenderID = damageInfo.attackerId;
        AddModifier(modToHealth);
    }

    void StaticAutoGun::FireFromWeapon(bool enable)
    {
        // RVA 0x742060 - NOTE: the "CANNON" part is used as a Gun without any type check.
        auto* cannon = static_cast<Gun*>(GetPartByName(STR_CANNON));
        if (cannon)
        {
            cannon->Fire(enable);
        }
    }

    void StaticAutoGun::SetPositionSelf(CVector const& pos)
    {
        ai::PhysicObj::SetPositionSelf(ai::GetGroundPos(pos, 1, 0));
    }

    bool StaticAutoGun::ApplyModifier(Modifier const& modifier)
    {
        // RVA 0x742AC0 - NOTE: this chains straight to Obj::ApplyModifier, skipping PhysicObj and
        // ComplexPhysicObj. Only the first two characters of the property name are compared, so any
        // name that starts with "hp" changes the health, and the shipped substring code asserts on a
        // name shorter than two characters.
        if (Obj::ApplyModifier(modifier))
        {
            return true;
        }

        assert(modifier.m_PropertyName.length() >= 2);
        if (modifier.m_PropertyName.substr(0, 2) == "hp")
        {
            m_health.value().ApplyModifier(modifier, m_health.maxValue().get());
            return true;
        }
        return false;
    }

    void StaticAutoGun::Update(float elapsedTime, unsigned workTime)
    {
        // RVA 0x743740 - every one to one and a half seconds the turret picks the first hostile
        // vehicle inside its look sphere. In between, it keeps aiming at that vehicle and fires
        // whenever the gun has a clear line to it.
        PhysicObj::Update(elapsedTime, workTime);
        if (!IsAlive())
        {
            return;
        }

        m_timeForNextCheck.regenerate(elapsedTime);
        if (m_timeForNextCheck.value().get() == m_timeForNextCheck.minValue().get())
        {
            m_timeForNextCheck.value().set(static_cast<float>(rand()) * 0.000030518509f * 0.5f + 1.0f);

            m_currentEnemyId = -1;
            retruxx::set<ref_ptr<Obstacle>> seenObstacles;
            IntersectionManager::GetIntersectedObjects(seenObstacles, _GetLookSphere(), m_targetClasses, false, false);
            for (auto const& obstacle : seenObstacles)
            {
                PhysicObj* obj = obstacle->GetOwnerPhysicObj();
                if (obj && obj->IsKindOf(&Vehicle::m_classVehicle) && obj->IsAlive() &&
                    theRelationship->CheckTolerance(obj->GetBelong(), GetBelong()) <= RS_ENEMY)
                {
                    m_currentEnemyId = obj->GetId();
                    break;
                }
            }
        }

        bool fire = false;
        auto* enemy = static_cast<Vehicle*>(theObjects->GetEntityByObjId(m_currentEnemyId));
        if (enemy)
        {
            // The gun aims where the enemy was a moment ago, and how long ago depends on the difficulty.
            CVector const enemyPos =
                enemy->GetRecollectionPosition(theGlobProp.GetCoeffsForCurrentDifficultyLevel().m_enemiesShootingDelay);

            retruxx::vector<int> exceptions;
            exceptions.push_back(enemy->GetId());
            if (PointIsReachableForGun(enemyPos, exceptions))
            {
                WeaponLookAtPoint(enemyPos, elapsedTime);
                fire = true;
            }
        }
        FireFromWeapon(fire);
    }

    StaticAutoGun::StaticAutoGun(StaticAutoGunPrototypeInfo const& prototype) :
        ComplexPhysicObj(prototype),
        m_health(prototype.m_maxHealth, 0.0, prototype.m_maxHealth),
        m_timeForNextCheck(0.1, 0.0, 10.0, -1.0)
    {
        m_destroyedModelName = prototype.m_destroyedModelName;

        m_health.m_BeforeValueApplyModifier =
            new ai::MemberFunctionTwoArgsRef<ai::StaticAutoGun, ai::Modifier, float, bool>(
                *this, &StaticAutoGun::_OnHealthValueBeforeApplyModifier);
        m_health.m_AfterValueChange = new ai::MemberFunctionOneArg<ai::StaticAutoGun, float, void>(
            *this, &StaticAutoGun::_OnHealthValueAfterChange);

        m_targetClasses.insert(RT_CLASS_LOCAL(Vehicle));
        m_currentEnemyId = -1;
        DisablePhysics();
    }

    void StaticAutoGun::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x7426A0 - each attribute defaults to whatever the value already holds, so a save
        // without them leaves the prototype's values in place.
        ComplexPhysicObj::LoadRuntimeValues(xmlFile, xmlNode);

        float health = m_health.value().get();
        m3d::SafeFloatAttrib(health, xmlNode, "Health");
        m_health.value().set(health);

        float timeForNextCheck = m_timeForNextCheck.value().get();
        m3d::SafeFloatAttrib(timeForNextCheck, xmlNode, "TimeForNextCheck");
        m_timeForNextCheck.value().set(timeForNextCheck);
    }

    m3d::Class* StaticAutoGun::GetBaseClass()
    {
        return RT_CLASS_LOCAL(ComplexPhysicObj);
    }

    float StaticAutoGun::GetMaxFiringRangeAI() const
    {
        // RVA 0x742BD0 - the longest range among the guns mounted on the turret.
        float maxRange = 0.0f;
        for (auto const& part : m_vehicleParts)
        {
            float range;
            if (part.second->IsKindOf(&CompoundGun::m_classCompoundGun))
            {
                range = static_cast<CompoundGun const*>(part.second)->GetFiringRange();
            }
            else if (part.second->IsKindOf(&Gun::m_classGun))
            {
                range = static_cast<Gun const*>(part.second)->GetFiringRange();
            }
            else
            {
                continue;
            }

            maxRange = maxRange > range ? maxRange : range;
        }
        return maxRange;
    }

    void StaticAutoGun::AddChild(Obj* pObj)
    {
        // RVA 0x742850 - bullets fired by the turret are parented to it; RemoveChild undoes this.
        ComplexPhysicObj::AddChild(pObj);
        if (pObj && pObj->IsKindOf(&Bullet::m_classBullet))
        {
            pObj->LinkToParent(GetId(), Obj::HIERARCHY_CHILD);
        }
    }

    bool StaticAutoGun::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        if (propertyId != 26)
            return ai::PhysicObj::SetPropertyById(propertyId, newValue);
        m_health.value().SetUnsafe(newValue.GetAsFloat());
        return 1;
    }

    void StaticAutoGun::_InternalPostLoad()
    {
        ai::PhysicObj::_InternalPostLoad();
        if (!IsAlive())
        {
            if (!m_destroyedModelName.empty())
            {
                auto* partByName = GetPartByName(STR_DOT);
                if (partByName)
                    partByName->SetModelNameUnsafe(m_destroyedModelName);
            }
        }
    }

    void StaticAutoGun::RegisterProperty(char const* Name, int id, eGObjPropertySaveStatus saveStatus)
    {
        // RVA 0x743100 - the default save status is not stored.
        m_propertiesMap[Name] = id;
        if (saveStatus)
        {
            m_propertiesSaveStatesMap[id] = saveStatus;
        }
    }

    void StaticAutoGun::_Construct(bool bForAnimation)
    {
        ComplexPhysicObj::_Construct(bForAnimation);
        for (auto* i = dBodyGetFirstGeom(m_body->id()); i; i = dGeomGetBodyNext(i))
        {
            dGeomSetCategoryBits(i, 1u);
            dGeomSetCollideBits(i, 0xFFFFFFFE);
        }

        auto* partByName = GetPartByName(STR_DOT);
        if (partByName)
        {
            theObjects->AddObjToNotUpdate(partByName);
        }
    }

    bool StaticAutoGun::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x743370 - a fresh turret is at full health.
        StaticAutoGunPrototypeInfo const* prototype = GetPrototypeInfo();
        if (propertyId != 26)
        {
            return PhysicObj::_GetPropertyDefaultInternal(propertyId, retVal);
        }
        retVal = prototype->m_maxHealth;
        return true;
    }

    StaticAutoGun::~StaticAutoGun() = default;

    bool StaticAutoGun::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x743310
        if (propertyId != 26)
        {
            return PhysicObj::_GetPropertyInternal(propertyId, retVal);
        }
        retVal = m_health.value().get();
        return true;
    }

    m3d::Object* StaticAutoGun::CreateObject()
    {
        // RVA 0x7424A0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    bool StaticAutoGun::_OnHealthValueBeforeApplyModifier(Modifier const& modifier, float& newHealth)
    {
        // RVA 0x7433E0 - never vetoes the change. It reports the hit to the turret's AI and costs the
        // player reputation when the player is the one shooting.
        if (modifier.m_SenderID != -1)
        {
            // NOTE: unlike Chassis, the event is raised without checking how the turret's side
            // regards the attacker.
            CauseEvent(GE_UNDER_ATTACK, 0.0f, m3d::AIParam(modifier.m_SenderID), m3d::AIParam());

            if (M3D_APP->GetCurGameMode() != 1)
            {
                Obj* sender = theObjects->GetEntityByObjId(modifier.m_SenderID);
                if (sender && sender->IsKindOf(&Vehicle::m_classVehicle))
                {
                    auto* senderVehicle = static_cast<Vehicle*>(sender);
                    if (senderVehicle->bIsControlledByPlayer() && GetBelong() != senderVehicle->GetBelong())
                    {
                        // Only actual damage costs reputation, in proportion to the fraction of the
                        // turret destroyed.
                        float const damage = m_health.value().get() - newHealth;
                        if (damage > 0.0f)
                        {
                            DecToleranceWhenDamageFromPlayerInflicted(this, damage / m_health.maxValue().get());
                        }
                    }
                }
            }
        }
        return false;
    }

    void StaticAutoGun::_OnHealthValueAfterChange(float)
    {
        // RVA 0x742890 - once the health hits the bottom, the cannon is torn off, the base switches
        // to its wrecked model and the turret blows up.
        if (!IsAlive() || m_health.value().get() != m_health.minValue().get())
        {
            return;
        }

        VehiclePart* cannon = GetPartByName(STR_CANNON);
        if (cannon)
        {
            SetPartByName(STR_CANNON, nullptr, false);
            cannon->Remove();
        }

        if (!m_destroyedModelName.empty())
        {
            VehiclePart* dot = GetPartByName(STR_DOT);
            if (dot)
            {
                dot->SetModelName(m_destroyedModelName);
            }
        }

        // One of three explosions: ET_PS_AUTOGUN_EXP, ET_PS_AUTOGUN_EXP1 or ET_PS_AUTOGUN_EXP2.
        CStr effectName("ET_PS_AUTOGUN_EXP");
        int const variant = 3 * rand() / 0x8000;
        if (variant > 0)
        {
            effectName += CStr(variant);
        }
        PhysicBody::CreateEffectNode(effectName, GetPosition(), GetRotation(), true, 1.0f);
        _SetDeadStatus();
    }

    m3d::Object* StaticAutoGun::Clone()
    {
        // RVA 0x7422E0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai
