#include "blastwave.h"
#include "base/objcontainer.h"
#include "base/globalproperties.h"
#include "base/prototypemanager.h"
#include "base/shell.h"
#include "breakableobject.h"
#include "dummyobject.h"
#include "base/geomobj.h"
#include "guns/gun.h"
#include "physicunit.h"
#include "vehicle.h"
#include "server/damageinfo.h"
#include "server/dynamicscene.h"

#include <cameracontroller.h>
#include <config.h>
#include <cmath>
#include <core/kernel.h>
#include <core/log.h>
#include <core/timer.h>
#include <m3dapp.h>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(BlastWave)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(BlastWave);

    bool BlastWavePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = SimplePhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            _SetGeomType(GEOM_TYPE_SPHERE);
            m3d::SafeFloatAttrib(m_WaveForceIntensity, xmlNode, "WaveForceIntensity");
            m3d::SafeFloatAttrib(m_WaveDamageIntensity, xmlNode, "WaveDamageIntensity");
            m3d::SafeStrAttrib(m_EffectName, xmlNode, "Effect");
        }
        return result;
    }

    BlastWavePrototypeInfo::BlastWavePrototypeInfo()
    {
        m_WaveForceIntensity = 0.0;
        m_WaveDamageIntensity = 0.0;
    }

    Obj* BlastWavePrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x7DF790
        return new BlastWave(*this);
    }

    void BlastWavePrototypeInfo::_InternalCopyFrom(ai::PrototypeInfo const& rhs)
    {
        // RVA 0x7E04C0 - a plain assignment from another prototype of the same class.
        *this = static_cast<BlastWavePrototypeInfo const&>(rhs);
    }

    void BlastWave::Update(float, unsigned)
    {
        // RVA 0x7DF7C0 - on its first update the wave shows its effect, shakes the camera and joins the
        // world's collision space; one frame later it is gone.
        CStr const& effectName = GetPrototypeInfo()->m_EffectName;
        if (!effectName.empty() && !m_EffectNode)
        {
            m_EffectNode = PhysicBody::CreateEffectNode(effectName, GetPosition(), GetRotation(), true, 1.0f);

            float const radius = ai::theGlobProp.m_blastWaveCameraShakeRadiusCoeff * GetBlastRadius();
            M3D_APP->m_cameraController->DoTouch(GetPosition(), radius, m_WaveDamageIntensity);
        }

        int const framesAlive = M3D_KERNEL->GetTimer().GetCurFrame() - m_frameWhenCreated;
        if (framesAlive == 0)
        {
            return;
        }
        if (framesAlive == 1)
        {
            m_physicBody->RelinkToSpace(ai::gGlobalSpace);
            return;
        }
        Remove();
    }

    void BlastWave::GetPropertiesNames(retruxx::set<CStr>& Props) const
    {
        // RVA 0x7E0130
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.first);
        }
        SimplePhysicObj::GetPropertiesNames(Props);
    }

    m3d::Class* BlastWave::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    bool BlastWave::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        // RVA 0x7DF070
        if (propertyId == 68)
        {
            m_WaveForceIntensity = newValue.GetAsFloat();
            return true;
        }
        if (propertyId == 69)
        {
            m_WaveDamageIntensity = newValue.GetAsFloat();
            return true;
        }
        return SimplePhysicObj::SetPropertyById(propertyId, newValue);
    }

    float BlastWave::GetBlastRadius() const
    {
        // RVA 0x7DF2F0 - the wave's own collision sphere is how far it reaches.
        return GetPrototypeInfo()->GetRadius();
    }

    m3d::Class* BlastWave::GetClass() const
    {
        return RT_CLASS_LOCAL(BlastWave);
    }

    int BlastWave::GetEmitterId() const
    {
        // RVA 0x7DF100
        return m_emitterId;
    }

    short BlastWave::GetRocketExplosionType() const
    {
        // RVA 0x7DF0E0
        return m_rocketExplosionType;
    }

    int BlastWave::GetPropertyId(char const* PropertyName) const
    {
        // RVA 0x7E00C0
        auto const it = m_propertiesMap.find(PropertyName);
        if (it != m_propertiesMap.end())
        {
            return it->second;
        }
        return SimplePhysicObj::GetPropertyId(PropertyName);
    }

    BlastWavePrototypeInfo const* BlastWave::GetPrototypeInfo() const
    {
        // RVA 0x7DF8D0
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), BlastWavePrototypeInfo const);
    }

    CStr BlastWave::GetPropertyName(int id) const
    {
        // RVA 0x7E0230
        for (auto const& property : m_propertiesMap)
        {
            if (property.second == id)
            {
                return property.first;
            }
        }
        return SimplePhysicObj::GetPropertyName(id);
    }

    eGObjPropertySaveStatus BlastWave::GetPropertySaveStatus(int id) const
    {
        // RVA 0x7E0080
        auto const it = m_propertiesSaveStatesMap.find(id);
        if (it != m_propertiesSaveStatesMap.end())
        {
            return it->second;
        }
        return SimplePhysicObj::GetPropertySaveStatus(id);
    }

    float BlastWave::GetDampedDamageIntensity(float dist) const
    {
        // RVA 0x7DF330
        return exp(dist / GetBlastRadius() * -2.3) * m_WaveDamageIntensity;
    }

    float BlastWave::GetWaveDamageIntensity() const
    {
        // RVA 0x7DF0D0
        return m_WaveDamageIntensity;
    }

    void BlastWave::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        // RVA 0x7E01B0
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.second);
        }
        SimplePhysicObj::GetPropertiesIDs(Props);
    }

    float BlastWave::GetWaveForceIntensity() const
    {
        // RVA 0x7DF0C0
        return m_WaveForceIntensity;
    }

    float BlastWave::GetDampedForceIntensity(float dist) const
    {
        // RVA 0x7DF370 - the push falls off the same way, but shifted so that it is already halved at the
        // point the blast starts.
        float const blastRadius = GetBlastRadius();
        return exp((dist * -2.3 - blastRadius * 0.69) / blastRadius) * m_WaveForceIntensity;
    }

    void BlastWave::SetEmitterId(int emitterId)
    {
        // RVA 0x7DF110
        m_emitterId = emitterId;
    }

    void BlastWave::SetRocketExplosionType(short explosionType)
    {
        // RVA 0x7DF0F0
        m_rocketExplosionType = explosionType;
    }

    void BlastWave::SetRocketId(int rocketId)
    {
        // RVA 0x7E0020 - the wave shows whichever explosion the shell that caused it is meant to make.
        if (rocketId == -1)
        {
            return;
        }
        // NOTE: the object is cast to a shell without a type check.
        auto* const shell = (Shell*)theObjects->GetEntityByObjId(rocketId);
        GunPrototypeInfo const* const gunPrototypeInfo = shell->GetGunPrototypeInfo();
        if (gunPrototypeInfo)
        {
            m_rocketExplosionType = gunPrototypeInfo->m_explosionType;
        }
    }

    float BlastWave::GetDampingCoefficeient(float dist) const
    {
        // RVA 0x7DF300 - the blast falls off exponentially, down to about a tenth at the blast radius.
        return exp(dist / GetBlastRadius() * -2.3);
    }

    void BlastWave::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        // RVA 0x7DF060 - a blast wave is never part of a level; one read back from XML removes itself.
        Remove();
    }

    int BlastWave::CollideBlastWaveAndPhysicObj(
        BlastWave* blastWave,
        Obj* obj,
        dContact* contact,
        unsigned& numContacts,
        bool reverse)
    {
        // RVA 0x7DF900 - what an explosion does to whatever it touches. Nothing here ever produces a contact:
        // the wave only pushes and damages, and always returns 0 so that no physical collision is generated.
        // The blast direction is the line from the wave to the object, tilted slightly upwards so that debris
        // lifts off the ground.
        CVector const wavePos = blastWave->GetPosition();

        if (obj->IsKindOf(&ai::PhysicBody::m_classPhysicBody))
        {
            auto* const body = (PhysicBody*)obj;

            CVector const delta = body->GetPosition() - wavePos;
            float const dist = sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);

            float const invDist = 1.0 / sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z + 0.00000011920929);
            CVector const INITIAL_UP_DIRECTION(0.0, 1.0, 0.0);
            CVector blastDir = delta * invDist + INITIAL_UP_DIRECTION * 0.1;
            float const invLen =
                1.0 / sqrt(blastDir.x * blastDir.x + blastDir.y * blastDir.y + blastDir.z * blastDir.z + 0.00000011920929);
            blastDir = blastDir * invLen;

            if (!body->IsKindOf(&ai::VehiclePart::m_classVehiclePart))
            {
                blastWave->m_bCollided = true;
                return 0;
            }
            PhysicObj* const owner = body->GetOwner();
            if (!owner)
            {
                blastWave->m_bCollided = true;
                return 0;
            }

            // A gun's shots are credited to whoever is carrying the gun, not to the gun itself.
            int attackerId = blastWave->GetId();
            Obj* const emitter = theObjects->GetEntityByObjId(blastWave->m_emitterId);
            if (emitter && emitter->IsKindOf(&ai::Gun::m_classGun))
            {
                PhysicObj* const emitterOwner = ((Gun*)emitter)->GetOwner();
                if (emitterOwner)
                {
                    attackerId = emitterOwner->GetId();
                }
            }

            // The blast is shared out over all the parts of whatever was hit.
            unsigned const numParts =
                static_cast<unsigned>(((ComplexPhysicObj*)owner)->m_vehicleParts.size());

            DamageInfo damageInfo;
            damageInfo.attackerId = attackerId;
            damageInfo.attackingAgentId = blastWave->GetId();
            damageInfo.bDamageFriends = true;
            damageInfo.damagedPartName = ((VehiclePart*)body)->GetPartName();
            damageInfo.damageType = DAMAGE_BLAST;
            damageInfo.damage = blastWave->GetDampedDamageIntensity(dist) / numParts;
            damageInfo.hitPos = CVector(contact->geom.pos[0], contact->geom.pos[1], contact->geom.pos[2]);
            damageInfo.hitDir = CVector(contact->geom.normal[0], contact->geom.normal[1], contact->geom.normal[2]);
            damageInfo.normal = damageInfo.hitDir;
            owner->InflictDamage(damageInfo);

            if (owner->IsKindOf(&ai::Vehicle::m_classVehicle))
            {
                float const force = blastWave->GetDampedForceIntensity(dist);
                CVector const impulse = blastDir * force * (1.0 / static_cast<double>(numParts));
                owner->AddImpulse(impulse);
            }

            blastWave->m_bCollided = true;
            return 0;
        }

        if (!obj->IsKindOf(&ai::PhysicObj::m_classPhysicObj))
        {
            blastWave->m_bCollided = true;
            return 0;
        }
        auto* const physicObj = (PhysicObj*)obj;

        CVector const delta = physicObj->GetPosition() - wavePos;
        float const dist = sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);

        float const invDist = 1.0 / sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z + 0.00000011920929);
        CVector const INITIAL_UP_DIRECTION(0.0, 1.0, 0.0);
            CVector blastDir = delta * invDist + INITIAL_UP_DIRECTION * 0.1;
        float const invLen =
            1.0 / sqrt(blastDir.x * blastDir.x + blastDir.y * blastDir.y + blastDir.z * blastDir.z + 0.00000011920929);
        blastDir = blastDir * invLen;

        if (physicObj->IsKindOf(&ai::BreakableObject::m_classBreakableObject))
        {
            auto* const breakable = (BreakableObject*)physicObj;
            float const force = blastWave->GetDampedForceIntensity(dist);

            // Only a blast strong enough for the object's own toughness breaks it.
            if (force <= M3D_KERNEL->GetEngineCfg().m_ai_bo_activate_coeff.GetF() * breakable->GetCriticalHitEnergy())
            {
                blastWave->m_bCollided = true;
                return 0;
            }

            if (breakable->IsDestroyable())
            {
                breakable->SetInitVelocities(true);
                breakable->SetCausePos(wavePos);
                breakable->SetCauseForce(force);
            }
            else
            {
                breakable->SetRemovingEffectName(gDynamicScene->GetBoShellEffectName(
                    breakable->GetEffectType(), blastWave->m_rocketExplosionType));
            }
            breakable->SetState(BreakableObject::REMOVED);

            blastWave->m_bCollided = true;
            return 0;
        }

        if (physicObj->IsKindOf(&ai::PhysicUnit::m_classPhysicUnit))
        {
            auto* const unit = (PhysicUnit*)physicObj;
            float const force = blastWave->GetDampedForceIntensity(dist);
            if (force <= M3D_KERNEL->GetEngineCfg().m_ai_ragdoll_activate_force.GetF())
            {
                blastWave->m_bCollided = true;
                return 0;
            }

            if (unit->GetPhysicBody() && unit->GetPhysicBody()->m_Node)
            {
                ActionType action = AT_DEATH1;
                unit->GetPhysicBody()->m_Node->SetProperty(8708, &action);
            }
            unit->SetInitVelocities(true);
            unit->SetCausePos(wavePos);
            unit->SetCauseForce(force);
            unit->SetState(PhysicUnit::DEAD);

            blastWave->m_bCollided = true;
            return 0;
        }

        if (physicObj->IsKindOf(&ai::GeomObj::m_classGeomObj) ||
            physicObj->IsKindOf(&ai::DummyObject::m_classDummyObject))
        {
            physicObj->AddImpulse(blastDir * blastWave->GetDampedForceIntensity(dist));
        }

        blastWave->m_bCollided = true;
        return 0;
    }

    BlastWave::BlastWave(BlastWavePrototypeInfo const& prototypeInfo) : SimplePhysicObj(prototypeInfo)
    {
        // RVA 0x7DF220 - a blast wave lives for two frames. It starts in the temporary collision space so
        // that it does not disturb anything until Update moves it into the world.
        m_WaveForceIntensity = prototypeInfo.m_WaveForceIntensity;
        m_WaveDamageIntensity = prototypeInfo.m_WaveDamageIntensity;
        m_rocketExplosionType = 0;
        m_EffectNode = nullptr;
        m_frameWhenCreated = M3D_KERNEL->GetTimer().GetCurFrame();
        m_Frame = 0;
        m_emitterId = -1;
        m_bCollided = false;
        m_physicBody->RelinkToSpace(ai::gTempSpace);
    }

    bool BlastWave::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x7E02A0
        BlastWavePrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        if (propertyId == 68)
        {
            retVal = prototypeInfo->m_WaveForceIntensity;
            return true;
        }
        if (propertyId == 69)
        {
            retVal = prototypeInfo->m_WaveDamageIntensity;
            return true;
        }
        return SimplePhysicObj::_GetPropertyDefaultInternal(propertyId, retVal);
    }

    bool BlastWave::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x7E0360
        if (propertyId == 68)
        {
            retVal = m_WaveForceIntensity;
            return true;
        }
        if (propertyId == 69)
        {
            retVal = m_WaveDamageIntensity;
            return true;
        }
        return SimplePhysicObj::_GetPropertyInternal(propertyId, retVal);
    }

    BlastWave::~BlastWave()
    {
        // RVA 0x7DF040 - the effect node belongs to the scene graph, not to the wave.
        m_EffectNode = nullptr;
    }

    m3d::Object* BlastWave::CreateObject()
    {
        // RVA 0x7DF590
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* BlastWave::Clone()
    {
        // RVA 0x7DF3D0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai
