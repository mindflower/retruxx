#include "mortarshell.h"

#include <stdexcept>

#include "core/kernel.h"
#include "core/log.h"
#include "mortar.h"
#include "server/objects/blastwave.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/base/prototypemanager.h"
#include "server/objects/physicbodies/simplephysicbody.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(MortarShell)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(MortarShell);

    MortarShellPrototypeInfo::MortarShellPrototypeInfo()
    {
        m_velocity = 1.0f;
        m_acceleration = 1.0f;
        m_flyTime = 1.0f;
        m_blastWavePrototypeId = -1;
    }

    bool MortarShellPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = ShellPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            ai::SimplePhysicObjPrototypeInfo::_SetGeomType(GEOM_TYPE_BOX);
            m3d::SafeFloatAttrib(m_flyTime, xmlNode, "FlyTime");
            m3d::SafeStrAttrib(m_blastWavePrototypeName, xmlNode, "BlastWavePrototype");
        }
        return result;
    }

    void MortarShellPrototypeInfo::PostLoad()
    {
        // RVA 0x84A940
        m_blastWavePrototypeId = thePrototypeManager->GetPrototypeId(m_blastWavePrototypeName);
        if (m_blastWavePrototypeId == -1 && m_blastWavePrototypeName.c_str() && strlen(m_blastWavePrototypeName.c_str()))
        {
            M3D_LOG_ERR(CStr("Error: Unknown blast wave prototype name: '") + m_blastWavePrototypeName + CStr("' for mortar shell '") +
                        m_prototypeName + CStr("'"));
            SYS_ERROR("!\"Critical error, see log\"");
        }
    }

    Obj* MortarShellPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x84B0B0
        return new MortarShell(*this);
    }

    void MortarShell::SetInitialPosition(CVector const& initialPosition)
    {
        // RVA 0x84A360
        m_initialPosition = initialPosition;
    }

    CVector MortarShell::GetInitialVelocity() const
    {
        // RVA 0x84A520
        return m_initialVelocity;
    }

    CVector MortarShell::GetInitialPosition() const
    {
        // RVA 0x84A550
        return m_initialPosition;
    }

    void MortarShell::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x84A450
        Shell::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("LifeTime", CStr(m_lifeTime.value().get()).c_str());
        xmlNode->SetAttribute("InitialVelocity", CStr(m_initialVelocity).c_str());
        xmlNode->SetAttribute("InitialPosition", CStr(m_initialPosition).c_str());
    }

    void MortarShell::SetInitialVelocity(CVector const& initialVelocity)
    {
        // RVA 0x84A340
        m_initialVelocity = initialVelocity;
    }

    void MortarShell::CreateBlastWave() const
    {
        // RVA 0x84AF50
        // NOTE: the blast wave is not checked for null once created.
        int const blastWaveId = theObjects->CreateNewObject(GetPrototypeInfo()->m_blastWavePrototypeId, "", -1, -1);
        if (blastWaveId != -1)
        {
            BlastWave* blastWave = static_cast<BlastWave*>(theObjects->GetEntityByObjId(blastWaveId));
            blastWave->SetRocketId(GetId());
            blastWave->SetPosition(GetPosition());
            blastWave->SetEmitterId(GetGunObjId());
            blastWave->SetBelong(GetBelong());
        }
    }

    m3d::Class* MortarShell::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Shell);
    }

    MortarShellPrototypeInfo const* MortarShell::GetPrototypeInfo() const
    {
        // RVA 0x84AD20
        // NOTE: the prototype is cast without a type check.
        return static_cast<MortarShellPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    m3d::Class* MortarShell::GetClass() const
    {
        // RVA 0x84A330
        return RT_CLASS_LOCAL(MortarShell);
    }

    void MortarShell::Update(float elapsedTime, unsigned workTime)
    {
        // RVA 0x84AD70
        // Flies a fixed ballistic arc (see Mortar::GetMortarShellPosition) for the prototype's
        // fly time. The collision box is stretched back along the path moved this frame so
        // that a fast shell cannot pass through anything between two frames.
        SimplePhysicObj::Update(elapsedTime, workTime);
        m_lifeTime.regenerate(elapsedTime);
        if (m_lifeTime.value().get() == m_lifeTime.maxValue().get())
        {
            Remove();
            return;
        }

        CVector const newPos = Mortar::GetMortarShellPosition(m_initialPosition, m_initialVelocity, m_lifeTime.value().get());
        CVector const size = GetPrototypeInfo()->GetSize();
        CVector const pos = GetPosition();
        float const dx = newPos.x - pos.x;
        float const dy = newPos.y - pos.y;
        float const dz = newPos.z - pos.z;
        float stretch = static_cast<float>(sqrt(dz * dz + dy * dy + dx * dx) - size.z);
        if (stretch <= 0.0f)
        {
            stretch = 0.0f;
        }
        CollisionInfo& collisionInfo = m_collisionInfos.front();
        collisionInfo.m_relTranslation = CVector(0.0f, 0.0f, stretch * -0.5f);
        collisionInfo.m_size = CVector(size.x, size.y, size.z + stretch);
        _UpdatePhysicBodyByCollisionInfo(m_collisionInfos);
        SetPosition(newPos);
    }

    MortarShell::MortarShell(MortarShellPrototypeInfo const& prototype) :
        Shell(prototype),
        m_lifeTime(0.0f, 0.0f, prototype.m_flyTime, 1.0f)
    {
        // RVA 0x84B000 - moved by hand in Update, so the body starts off.
        PhysicObj::DisablePhysics();
        m_physicState |= 1u;
        SetCorrectEnabledCellsCounter();
        SimplePhysicObj::EnableGeometry(false);
        m_physicBody->SetNodeAction(1, true);
    }

    void MortarShell::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x84AC50
        // NOTE: the shipped code reads LifeTime into an uninitialised local, so a missing
        // attribute stores garbage. Here the local starts at 0.
        Shell::LoadRuntimeValues(xmlFile, xmlNode);
        float lifeTime = 0.0f;
        m3d::SafeFloatAttrib(lifeTime, xmlNode, "LifeTime");
        m_lifeTime.value().set(lifeTime);
        m3d::SafeVectorAttrib(m_initialVelocity, xmlNode, "InitialVelocity");
        m3d::SafeVectorAttrib(m_initialPosition, xmlNode, "InitialPosition");
    }

    // RVA 0x84AD50
    MortarShell::~MortarShell() = default;

    m3d::Object* MortarShell::Clone()
    {
        // RVA 0x84A580
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* MortarShell::CreateObject()
    {
        // RVA 0x84A740
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }
}  // namespace ai
