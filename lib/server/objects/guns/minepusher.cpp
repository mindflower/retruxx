#include "minepusher.h"

#include <core/kernel.h>
#include <math/matrix.h>
#include <math/vector.h>
#include <stdexcept>

#include "server/objects/base/objcontainer.h"
#include "server/objects/base/prototypemanager.h"
#include "server/objects/guns/rocket.h"
#include "server/objects/vehicle.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(MinePusher)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(MinePusher);

    MinePusherPrototypeInfo::MinePusherPrototypeInfo()
    {
        m_damageType = DAMAGE_BLAST;
    }

    Obj* MinePusherPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x8475D0
        return new MinePusher(*this);
    }

    MinePusher::MinePusher(MinePusherPrototypeInfo const& prototype) : Gun(prototype)
    {
        // RVA 0x847590 - a mine pusher adds no state of its own to a gun.
    }

    m3d::Class* MinePusher::GetClass() const
    {
        // RVA 0x847580
        return RT_CLASS_LOCAL(MinePusher);
    }

    MinePusherPrototypeInfo const* MinePusher::GetPrototypeInfo() const
    {
        // RVA 0x8479C0
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), MinePusherPrototypeInfo const);
    }

    m3d::Class* MinePusher::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Gun);
    }

    MinePusher::~MinePusher()
    {
        // RVA 0x8475B0 - nothing of its own to release.
    }

    void MinePusher::_LaunchShells()
    {
        // RVA 0x8479F0 - a mine is not fired but dropped: it is created at rest and left
        // behind the truck, where the pursuer will drive over it.
        Gun::_LaunchShells();

        int const objId = theObjects->CreateNewObject(GetShellPrototypeId(), "", -1, -1);
        // NOTE: the result is used unchecked. A failed creation, or a shell prototype that is
        // not a Rocket, is dereferenced and cast blindly, exactly as the shipped build does.
        Rocket* const mine = static_cast<Rocket*>(theObjects->GetEntityByObjId(objId));
        mine->SetGunObjId(GetId());

        PhysicObj* const owner = GetOwner();
        Vehicle* vehicle = nullptr;
        if (owner)
        {
            mine->SetBelong(owner->GetBelong());
            if (IS_KIND_OF(owner, Vehicle))
            {
                vehicle = static_cast<Vehicle*>(owner);
            }
        }

        if (vehicle)
        {
            // Half the truck's length plus a metre puts the mine clear of its own tail.
            float const behind = vehicle->GetSize().z * 0.5f + 1.0f;
            mine->SetPosition(vehicle->GetPosition() - vehicle->GetDirection() * behind);
        }
        else
        {
            mine->SetPosition(_CalcPosForNextShot());
        }

        mine->SetDirection(_CalcDirForNextShot());
        mine->m_initialDirection = mine->GetDirection();
        mine->SetInitialVelocity(ZeroVector);
    }

    bool MinePusher::isLookAtPoint(CVector const&, float) const
    {
        // RVA 0x8475C0 - a dropped mine needs no aiming, so the pusher is always on target.
        return true;
    }

    m3d::Object* MinePusher::Clone()
    {
        // RVA 0x847640
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* MinePusher::CreateObject()
    {
        // RVA 0x847800
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }
}  // namespace ai
