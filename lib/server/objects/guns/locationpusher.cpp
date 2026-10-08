#include "locationpusher.h"

#include <math/matrix.h>
#include <stdexcept>

#include "core/kernel.h"
#include "server/objects/temporarylocation.h"
#include "server/objects/vehicle.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/base/prototypemanager.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(LocationPusher)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(LocationPusher);

    LocationPusherPrototypeInfo::LocationPusherPrototypeInfo() = default;

    Obj* LocationPusherPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x846F70
        return new LocationPusher(*this);
    }

    m3d::Class* LocationPusher::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Gun);
    }

    LocationPusher::LocationPusher(LocationPusherPrototypeInfo const& prototype) : Gun(prototype)
    {
        // RVA 0x846F30
    }

    LocationPusherPrototypeInfo const* LocationPusher::GetPrototypeInfo() const
    {
        // RVA 0x847360 - NOTE: the prototype is cast without a type check.
        return static_cast<LocationPusherPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    m3d::Class* LocationPusher::GetClass() const
    {
        // RVA 0x846F20
        return RT_CLASS_LOCAL(LocationPusher);
    }

    // RVA 0x846F50
    LocationPusher::~LocationPusher() = default;

    void LocationPusher::_LaunchShells()
    {
        // RVA 0x847390 - the location is dropped just behind a vehicle owner, clear of its rear
        // bumper by 4 units, and turned the same way; otherwise it appears at the gun itself.
        Gun::_LaunchShells();
        // NOTE: the prototype is fetched and not used.
        GetPrototypeInfo();

        int const locationId = theObjects->CreateNewObject(GetShellPrototypeId(), "", -1, -1);
        // NOTE: the new location is not checked for null.
        TemporaryLocation* location = static_cast<TemporaryLocation*>(theObjects->GetEntityByObjId(locationId));

        PhysicObj* owner = GetOwner();
        if (owner && owner->IsKindOf(RT_CLASS_LOCAL(Vehicle)))
        {
            Vehicle* vehicle = static_cast<Vehicle*>(owner);
            float const halfLength = vehicle->GetSize().z * 0.5f;
            float const dist = location->GetRadius() + halfLength + 4.0f;
            CVector const dir = vehicle->GetDirection();
            CVector const offset(dir.x * dist, dir.y * dist, dir.z * dist);
            CVector const vehiclePos = vehicle->GetPosition();
            location->SetPosition(CVector(vehiclePos.x - offset.x, vehiclePos.y - offset.y, vehiclePos.z - offset.z));
            location->SetRotation(vehicle->GetRotation());
            location->CreateEffectNode();
        }
        else
        {
            location->SetPosition(GetPosition());
            location->CreateEffectNode();
        }
    }

    m3d::Object* LocationPusher::CreateObject()
    {
        // RVA 0x8471A0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* LocationPusher::Clone()
    {
        // RVA 0x846FE0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    bool LocationPusher::isLookAtPoint(CVector const&, float) const
    {
        // RVA 0x846F60 - it drops the location wherever it is, so it is always on target.
        return true;
    }
}  // namespace ai
