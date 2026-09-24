#include "plasmabunch.h"

#include <math/matrix.h>
#include <stdexcept>

#include "core/kernel.h"
#include "server/objects/base/prototypemanager.h"
#include "server/objects/physicbodies/simplephysicbody.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(PlasmaBunch)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(PlasmaBunch);

    PlasmaBunchPrototypeInfo::PlasmaBunchPrototypeInfo()
    {
        m_velocity = 1.0f;
        m_acceleration = 1.0f;
        m_flyTime = 1.0f;
    }

    void PlasmaBunchPrototypeInfo::PostLoad()
    {
        // RVA 0x84B930 - NOTE: ShellPrototypeInfo::PostLoad is not called, and the blast wave
        // prototype resolved here is not used by PlasmaBunch itself.
        m_BlastWavePrototypeId = thePrototypeManager->GetPrototypeId(m_BlastWavePrototypeName);
    }

    bool PlasmaBunchPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = ai::ShellPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            _SetGeomType(GEOM_TYPE_BOX);
            m3d::SafeFloatAttrib(m_velocity, xmlNode, "Velocity");
            m3d::SafeFloatAttrib(m_acceleration, xmlNode, "Acceleration");
            m3d::SafeFloatAttrib(m_flyTime, xmlNode, "FlyTime");
            m3d::SafeStrAttrib(m_BlastWavePrototypeName, xmlNode, "BlastWavePrototype");
        }
        return result;
    }

    Obj* PlasmaBunchPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x84C3A0
        return new PlasmaBunch(*this);
    }

    void PlasmaBunch::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x84B950
        Shell::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("Velocity", CStr(m_velocity.value().get()).c_str());
        xmlNode->SetAttribute("LifeTime", CStr(m_lifeTime.value().get()).c_str());
    }

    PlasmaBunch::PlasmaBunch(PlasmaBunchPrototypeInfo const& prototype) :
        Shell(prototype),
        // The speed grows by m_acceleration per second up to m_velocity; the age counts up to
        // m_flyTime, when the bunch is removed.
        m_velocity(0.0f, 0.0f, prototype.m_velocity, prototype.m_acceleration),
        m_lifeTime(0.0f, 0.0f, prototype.m_flyTime, 1.0f)
    {
        // RVA 0x84C2C0 - the bunch is moved by hand in Update, not by ODE, so its body is switched
        // off while the object still counts as enabled.
        PhysicObj::DisablePhysics();
        m_physicState |= 1u;
        SetCorrectEnabledCellsCounter();
        SimplePhysicObj::EnableGeometry(false);
        m_physicBody->SetNodeAction(1, true);
    }

    void PlasmaBunch::Remove()
    {
        // RVA 0x84B7F0
        SimplePhysicObj::Remove();
    }

    m3d::Class* PlasmaBunch::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Shell);
    }

    void PlasmaBunch::SetVelocity(float value)
    {
        // RVA 0x84B1D0
        m_velocity.value().set(value);
    }

    void PlasmaBunch::RenderDebugInfo() const
    {
        // RVA 0x84B810
        SimplePhysicObj::RenderDebugInfo();
    }

    void PlasmaBunch::TransferPhysicParamsToSceneGraphNode()
    {
        // RVA 0x84B800
        SimplePhysicObj::TransferPhysicParamsToSceneGraphNode();
    }

    void PlasmaBunch::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x84BDB0 - NOTE: the shipped code reads both attributes into one uninitialized
        // local, so a missing Velocity stores garbage and a missing LifeTime repeats the
        // velocity. Here the local starts at 0 instead of garbage.
        Shell::LoadRuntimeValues(xmlFile, xmlNode);
        float value = 0.0f;
        m3d::SafeFloatAttrib(value, xmlNode, "Velocity");
        m_velocity.value().set(value);
        m3d::SafeFloatAttrib(value, xmlNode, "LifeTime");
        m_lifeTime.value().set(value);
    }

    m3d::Class* PlasmaBunch::GetClass() const
    {
        // RVA 0x84B7E0
        return RT_CLASS_LOCAL(PlasmaBunch);
    }

    PlasmaBunchPrototypeInfo const* PlasmaBunch::GetPrototypeInfo() const
    {
        // RVA 0x84BED0 - NOTE: the prototype is cast without a type check.
        return static_cast<PlasmaBunchPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    void PlasmaBunch::Update(float elapsedTime, unsigned workTime)
    {
        // RVA 0x84BF30
        SimplePhysicObj::Update(elapsedTime, workTime);

        m_velocity.regenerate(elapsedTime);
        m_lifeTime.regenerate(elapsedTime);
        if (m_lifeTime.value().get() == m_lifeTime.maxValue().get())
        {
            Remove();
            return;
        }

        float speed = m_velocity.value().get();
        CVector const dir = GetDirection();
        CVector const step(dir.x * elapsedTime * speed, dir.y * elapsedTime * speed, dir.z * elapsedTime * speed);
        CVector const pos = GetPosition();
        CVector const newPos(step.x + pos.x, pos.y + step.y, pos.z + step.z);

        // Stretch the collision box backwards over the distance covered this frame beyond its own
        // length, so a fast bunch cannot tunnel through thin geometry between two frames.
        CVector const size = GetPrototypeInfo()->GetSize();
        CVector const curPos = GetPosition();
        CVector const delta(newPos.x - curPos.x, newPos.y - curPos.y, newPos.z - curPos.z);
        float const overshoot = static_cast<float>(
            std::sqrt(double(delta.z) * delta.z + double(delta.y) * delta.y + double(delta.x) * delta.x) - size.z);
        float const stretch = overshoot <= 0.0f ? 0.0f : overshoot;

        CollisionInfo& collisionInfo = m_collisionInfos.front();
        collisionInfo.m_relTranslation = CVector(0.0f, 0.0f, stretch * -0.5f);
        collisionInfo.m_size = CVector(size.x, size.y, size.z + stretch);
        _UpdatePhysicBodyByCollisionInfo(m_collisionInfos);

        speed = m_velocity.value().get();
        CVector const vel = GetLinearVelocity();
        float const invLen = static_cast<float>(
            1.0 / std::sqrt(double(vel.x) * vel.x + double(vel.y) * vel.y + double(vel.z) * vel.z + double(1.1920929e-7f)));
        SetLinearVelocity(CVector(vel.x * invLen * speed, vel.y * invLen * speed, vel.z * invLen * speed));
        SetPosition(newPos);
    }

    PlasmaBunch::~PlasmaBunch()
    {
        // RVA 0x84BF00
    }

    m3d::Object* PlasmaBunch::Clone()
    {
        // RVA 0x84B9F0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* PlasmaBunch::CreateObject()
    {
        // RVA 0x84BBB0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }
}  // namespace ai
