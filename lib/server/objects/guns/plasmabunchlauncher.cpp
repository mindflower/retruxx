#include "plasmabunchlauncher.h"

#include <math/matrix.h>
#include <stdexcept>

#include "core/kernel.h"
#include "plasmabunch.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/base/prototypemanager.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(PlasmaBunchLauncher)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(PlasmaBunchLauncher);

    PlasmaBunchLauncherPrototypeInfo::PlasmaBunchLauncherPrototypeInfo()
    {
        m_damageType = DAMAGE_ENERGY;
        m_WithShellsPoolLimit = true;
    }

    Obj* PlasmaBunchLauncherPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x84B160
        return new PlasmaBunchLauncher(*this);
    }

    bool PlasmaBunchLauncherPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return GunPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    void PlasmaBunchLauncherPrototypeInfo::PostLoad()
    {
        // RVA 0x84B120 - NOTE: m_bunchPrototypeName is never loaded or used; the bunch comes from
        // the gun's own shell prototype.
        GunPrototypeInfo::PostLoad();
    }

    m3d::Class* PlasmaBunchLauncher::GetClass() const
    {
        // RVA 0x84B0F0
        return RT_CLASS_LOCAL(PlasmaBunchLauncher);
    }

    PlasmaBunchLauncherPrototypeInfo const* PlasmaBunchLauncher::GetPrototypeInfo() const
    {
        // RVA 0x84B5C0 - NOTE: the prototype is cast without a type check.
        return static_cast<PlasmaBunchLauncherPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    PlasmaBunchLauncher::PlasmaBunchLauncher(PlasmaBunchLauncherPrototypeInfo const& prototype) : Gun(prototype)
    {
        // RVA 0x84B130
    }

    m3d::Class* PlasmaBunchLauncher::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Gun);
    }

    void PlasmaBunchLauncher::_LaunchShells()
    {
        // RVA 0x84B5F0 - a plasma bunch starts at the speed of the vehicle that fires it and then
        // accelerates on its own (see PlasmaBunch::Update).
        Gun::_LaunchShells();

        int const bunchId = theObjects->CreateNewObject(GetShellPrototypeId(), "", -1, -1);
        PlasmaBunch* bunch = static_cast<PlasmaBunch*>(theObjects->GetEntityByObjId(bunchId));
        // NOTE: neither the bunch nor the owner is checked for null before use.
        bunch->SetGunObjId(GetId());
        PhysicObj* owner = GetOwner();
        if (owner)
        {
            bunch->SetBelong(owner->GetBelong());
        }

        CVector const ownerVel = owner->GetLinearVelocity();
        bunch->SetVelocity(static_cast<float>(
            std::sqrt(double(ownerVel.x) * ownerVel.x + double(ownerVel.y) * ownerVel.y + double(ownerVel.z) * ownerVel.z)));
        bunch->SetPosition(_CalcPosForNextShot());
        bunch->SetDirection(_CalcDirForNextShot());
        bunch->SetDeadTimer(100000, false);
        bunch->RelinkToSpace(owner->GetSpaceId());
    }

    // RVA 0x84B150
    PlasmaBunchLauncher::~PlasmaBunchLauncher() = default;

    m3d::Object* PlasmaBunchLauncher::CreateObject()
    {
        // RVA 0x84B400
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* PlasmaBunchLauncher::Clone()
    {
        // RVA 0x84B240
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai
