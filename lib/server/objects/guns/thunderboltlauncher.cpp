#include "thunderboltlauncher.h"

#include <math/matrix.h>
#include <math/plane.h>
#include <stdexcept>

#include "core/kernel.h"
#include "thunderbolt.h"
#include "server/objects/vehicle.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/base/prototypemanager.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(ThunderboltLauncher)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(ThunderboltLauncher);

    Obj* ThunderboltLauncherPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x848A50
        return new ThunderboltLauncher(*this);
    }

    ThunderboltLauncherPrototypeInfo::ThunderboltLauncherPrototypeInfo()
    {
        m_damageType = DAMAGE_ENERGY;
        m_WithShellsPoolLimit = true;
    }

    bool ThunderboltLauncherPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = GunPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_ActionDist, xmlNode, "ActionDist");
        }
        return result;
    }

    void ThunderboltLauncherPrototypeInfo::PostLoad()
    {
        // RVA 0x8484C0
        GunPrototypeInfo::PostLoad();
    }

    m3d::Class* ThunderboltLauncher::GetClass() const
    {
        // RVA 0x8484B0
        return RT_CLASS_LOCAL(ThunderboltLauncher);
    }

    ThunderboltLauncherPrototypeInfo const* ThunderboltLauncher::GetPrototypeInfo() const
    {
        // RVA 0x8488F0 - NOTE: the prototype is cast without a type check.
        return static_cast<ThunderboltLauncherPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    m3d::Class* ThunderboltLauncher::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Gun);
    }

    bool ThunderboltLauncher::CanFire() const
    {
        // RVA 0x848AB0 - also picks the targets: the enemies in range that are on the same side
        // of the vehicle (left or right) as the gun's muzzle.
        bool const canFire = Gun::CanFire();
        if (!canFire)
        {
            return canFire;
        }

        ThunderboltLauncherPrototypeInfo const* prototype = GetPrototypeInfo();
        retruxx::vector<int> enemies;
        // NOTE: the owner is assumed to be a vehicle and is not checked for null.
        Vehicle* owner = static_cast<Vehicle*>(GetOwner());
        owner->GetEnemiesInNeighborhood(prototype->m_ActionDist, enemies);
        if (enemies.empty())
        {
            return false;
        }

        // The vertical plane through the vehicle along its heading: normal = forward x up.
        CVector const dir = owner->GetDirection();
        float const ny0 = dir.y * 0.0f;
        float const nz0 = dir.x - ny0;
        float const ny1 = dir.z * 0.0f - dir.x * 0.0f;
        float const nx0 = ny0 - dir.z;
        float const invLen = static_cast<float>(1.0 / std::sqrt(
            double(nz0) * nz0 + double(ny1) * ny1 + double(nx0) * nx0 + double(1.1920929e-7f)));
        CPlane dividerPlane;
        dividerPlane.m_normal = CVector(invLen * nx0, invLen * ny1, invLen * nz0);
        CVector const ownerPos = owner->GetPosition();
        dividerPlane.m_dist = ownerPos.z * dividerPlane.m_normal.z + ownerPos.y * dividerPlane.m_normal.y +
            ownerPos.x * dividerPlane.m_normal.x;
        // NOTE: the shipped code never sets m_type, so the distance test reads an uninitialized
        // value; the general (non-axis-aligned) test is the one that fits this plane.
        dividerPlane.m_type = 3;

        CMatrix const shot = GetMatrixForShot(0);
        float const muzzleSide = dividerPlane.dist(CVector(shot._41, shot._42, shot._43));

        // NOTE: returns true even when no enemy is on the muzzle's side and m_enemies ends up empty.
        const_cast<ThunderboltLauncher*>(this)->m_enemies.resize(0);
        for (int const enemyId : enemies)
        {
            Obj* enemy = theObjects->GetEntityByObjId(enemyId);
            if (enemy && enemy->IsKindOf(RT_CLASS_LOCAL(PhysicObj)))
            {
                if (dividerPlane.dist(static_cast<PhysicObj*>(enemy)->GetPosition()) * muzzleSide > 0.0f)
                {
                    const_cast<ThunderboltLauncher*>(this)->m_enemies.push_back(enemyId);
                }
            }
        }
        return true;
    }

    ThunderboltLauncher::ThunderboltLauncher(ThunderboltLauncherPrototypeInfo const& prototype) : Gun(prototype)
    {
        // RVA 0x848920
    }

    void ThunderboltLauncher::_LaunchShells()
    {
        // RVA 0x8489B0 - one bolt from the gun to all the enemies CanFire picked.
        Gun::_LaunchShells();
        if (m_enemies.empty())
        {
            return;
        }
        int const boltId = theObjects->CreateNewObject(GetShellPrototypeId(), "", -1, -1);
        // NOTE: the new bolt is not checked for null.
        Thunderbolt* bolt = static_cast<Thunderbolt*>(theObjects->GetEntityByObjId(boltId));
        bolt->setSourceId(GetId(), true);
        bolt->SetBelong(GetBelong());
        bolt->setTargets(m_enemies);
    }

    // RVA 0x8488A0
    ThunderboltLauncher::~ThunderboltLauncher() = default;

    m3d::Object* ThunderboltLauncher::CreateObject()
    {
        // RVA 0x8486E0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* ThunderboltLauncher::Clone()
    {
        // RVA 0x848520
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai
