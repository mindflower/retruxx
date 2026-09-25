#include "bossmetalarmload.h"

#include <cassert>
#include <stdexcept>

#include <ode/contact.h>

#include "core/kernel.h"
#include "server/colliders/physicunitcolliders.h"
#include "server/modifier.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/base/prototypemanager.h"
#include "server/objects/physicbodies/physicbody.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(BossMetalArmLoad)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(BossMetalArmLoad);

    BossMetalArmLoadPrototypeInfo::BossMetalArmLoadPrototypeInfo()
    {
        // RVA 0x808CC0
        m_blastWavePrototypeId = -1;
        m_maxHealth = 1.0f;
        m_bIsUpdating = true;
    }

    bool BossMetalArmLoadPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x808D20
        bool const result = DummyObjectPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeStrAttrib(m_blastWavePrototypeName, xmlNode, "BlastWavePrototype");
            m3d::SafeStrAttrib(m_explosionEffectName, xmlNode, "ExplosionEffect");
            m3d::SafeFloatAttrib(m_maxHealth, xmlNode, "MaxHealth");
        }
        return result;
    }

    Obj* BossMetalArmLoadPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x809850
        return new BossMetalArmLoad(*this);
    }

    void BossMetalArmLoadPrototypeInfo::PostLoad()
    {
        // RVA 0x808DA0
        // NOTE: does not chain to the base PostLoad.
        m_blastWavePrototypeId = thePrototypeManager->GetPrototypeId(CStr(m_blastWavePrototypeName.c_str()));
    }

    bool BossMetalArmLoad::ApplyModifier(Modifier const& modifier)
    {
        // RVA 0x8094B0 - NOTE: chains straight to Obj::ApplyModifier, skipping the physic object
        // classes. Only the first two characters of the property name are compared, so any name
        // that starts with "hp" changes the health, and the shipped substring code asserts on a
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

    m3d::Class* BossMetalArmLoad::GetBaseClass()
    {
        // RVA 0x808CA0
        return RT_CLASS_LOCAL(DummyObject);
    }

    void BossMetalArmLoad::SetCollisionMode(CollisionMode collisionMode)
    {
        // RVA 0x731280
        m_collisionMode = collisionMode;
    }

    BossMetalArmLoad::BossMetalArmLoad(BossMetalArmLoadPrototypeInfo const& prototypeInfo) :
        DummyObject(prototypeInfo),
        m_health(prototypeInfo.m_maxHealth, 0.0, prototypeInfo.m_maxHealth)
    {
        // RVA 0x8097A0
        m_health.m_AfterValueChange =
            new MemberFunctionOneArg<BossMetalArmLoad, float, void>(*this, &BossMetalArmLoad::_OnAfterHealthValueChange);
        m_collisionMode = COLLIDE_NONE;
    }

    BossMetalArmLoadPrototypeInfo const* BossMetalArmLoad::GetPrototypeInfo() const
    {
        // RVA 0x809440
        return static_cast<BossMetalArmLoadPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    int BossMetalArmLoad::CollideBossMetalArmLoadWithObject(m3d::Object* obj1, m3d::Object*, dContact* contacts, unsigned&, bool)
    {
        // RVA 0x809670
        // Carried loads pass through everything; a released one bounces off with sparks, and a
        // thrown one blows up on the first thing it touches.
        auto* const load = static_cast<BossMetalArmLoad*>(obj1);
        if (load->m_collisionMode == COLLIDE_NORMAL)
        {
            float const deltaSpeed = CalcContactDeltaSpeed(*contacts);
            if (load->CanCreateCollisionEffect())
            {
                CVector const contactPos(contacts->geom.pos[0], contacts->geom.pos[1], contacts->geom.pos[2]);
                if (deltaSpeed > 1.0)
                {
                    load->SetCollisionEffectCreated();
                }
                if (deltaSpeed > 10.0)
                {
                    PhysicBody::CreateEffectNode(CStr("ET_PS_STATICSSPARKLE"), contactPos, IdentityQuaternion, true, 1.0f);
                    return 1;
                }
                if (deltaSpeed > 1.0)
                {
                    PhysicBody::CreateEffectNode(CStr("ET_PS_STATICSSPARKLE_SLOW"), contactPos, IdentityQuaternion, true, 1.0f);
                }
            }
            return 1;
        }
        if (load->m_collisionMode == COLLIDE_EXPLODE)
        {
            load->Explode();
        }
        return 0;
    }

    void BossMetalArmLoad::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x809240
        // NOTE: calls SimplePhysicObj::LoadRuntimeValues directly, skipping DummyObject's.
        SimplePhysicObj::LoadRuntimeValues(xmlFile, xmlNode);
        m3d::SafeEnumAttrib(m_collisionMode, xmlNode, "CollisionMode");
        float health = m_health.value().get();
        m3d::SafeFloatAttrib(health, xmlNode, "Health");
        m_health.value().set(health);
    }

    m3d::Class* BossMetalArmLoad::GetClass() const
    {
        // RVA 0x808CB0
        return RT_CLASS_LOCAL(BossMetalArmLoad);
    }

    void BossMetalArmLoad::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x809320
        // NOTE: calls SimplePhysicObj::SaveRuntimeValues directly, skipping DummyObject's.
        SimplePhysicObj::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("CollisionMode", CStr(static_cast<int>(m_collisionMode)).c_str());
        xmlNode->SetAttribute("Health", CStr(m_health.value().get()).c_str());
    }

    void BossMetalArmLoad::Explode()
    {
        // RVA 0x809610
        if (GetDeletedStatus())
        {
            return;
        }
        PhysicBody::CreateEffectNode(GetPrototypeInfo()->m_explosionEffectName, GetPosition(), IdentityQuaternion, true, 1.0f);
        _CreateBlastWave();
        Remove();
    }

    BossMetalArmLoad::~BossMetalArmLoad()
    {
        // RVA 0x809420
    }

    m3d::Object* BossMetalArmLoad::CreateObject()
    {
        // RVA 0x809040
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    void BossMetalArmLoad::_OnAfterHealthValueChange(float)
    {
        // RVA 0x809780
        if (m_health.value().get() == m_health.minValue().get())
        {
            Explode();
        }
    }

    m3d::Object* BossMetalArmLoad::Clone()
    {
        // RVA 0x808E80
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    void BossMetalArmLoad::_CreateBlastWave()
    {
        // RVA 0x809590
        int const blastWaveId = theObjects->CreateNewObject(GetPrototypeInfo()->m_blastWavePrototypeId, "", -1, -1);
        if (blastWaveId == -1)
        {
            return;
        }
        // NOTE: the new object is used without a null or type check.
        static_cast<PhysicObj*>(theObjects->GetEntityByObjId(blastWaveId))->SetPosition(GetPosition());
    }
}  // namespace ai
