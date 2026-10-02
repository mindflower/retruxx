#define NOMINMAX

#include "vehicle.h"

#include <algorithm>

#include "physicbodies/geoms/box.h"
#include <stdexcept>
#include <core/aiparam.h>
#include <server/ai/aimessage.h>
#include <server/ai/aipassagestate.h>
#include <server/obstacle.h>
#include "server/objects/basket.h"
#include "server/objects/blastwave.h"
#include "server/objects/guns/thunderbolt.h"
#include "server/objects/cabin.h"
#include "server/utils.h"
#include <server/objects/physicbodies/physichelpers.h>

#include "chassis.h"
#include "chest.h"
#include "landscape.h"
#include "player.h"
#include "vehicleupdater.h"
#include "base/globalproperties.h"
#include "base/prototypemanager.h"
#include "core/ini.h"
#include "core/log.h"
#include "server/ai/aimanager.h"
#include "include/m3dapp.h"
#include "include/core/kernel.h"
#include "include/config.h"
#include "server/izvratrepository.h"

#include "ode/collision.h"
#include "ode/objects.h"

#include "ode/odecpp.h"
#include "base/objcontainer.h"
#include "physicbodies/compoundvehiclepart.h"
#include "scene/servers/DataServer.h"
#include "scene/servers/serveranimatedmodel.h"
#include "server/processmanager.h"
#include <client.h>

#include "world.h"
#include <server/server.h>

#include "gadget.h"
#include "ware.h"
#include "level.h"
#include "staticautogun.h"
#include "team.h"
#include "vehiclerecollection.h"
#include "core/timer.h"
#include "engine/ode/sources/joint.h"
#include "guns/compoundgun.h"
#include "guns/bullet.h"
#include "guns/rocketlauncher.h"
#include "guns/rocketvolleylauncher.h"
#include "scene/nodes/sgnodesound.h"
#include "server/externalpaths.h"
#include "server/infocone.h"
#include "server/intersectionmanager.h"
#include "server/path.h"
#include "server/geomrepositoryitem.h"
#include "server/processmanager.h"
#include "server/resourcemanager.h"
#include "server/weaponfirer.h"
#include "server/formations/formation.h"
#include "server/roles/VehicleRole.h"
#include "server/statistic/floatstatistic.h"
#include "server/statistic/statisticmanager.h"
#include "server/statistic/intstatistic.h"
#include "server/dynamicquestmanager.h"
#include "server/dynamicscene.h"
#include "server/map.h"
#include "server/relationship.h"

extern "C" void __cdecl _assert(char const* message, char const* file, unsigned line);

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetRandomSkin)
{
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    vehicle->SetRandomSkin();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetGamePositionOnGround)
{
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    auto vec = context->asVector(1);
    vehicle->SetGamePositionOnGround(vec, true, true);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetSize)
{
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    auto res = vehicle->GetSize();
    context->pushVector(res);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetCabin)
{
    // RVA 0x5CFD80
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    context->pushObject(vehicle->GetCabin());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetBasket)
{
    // RVA 0x5CFDE0
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    context->pushObject(vehicle->GetBasket());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetChassis)
{
    // RVA 0x5CFE40
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    context->pushObject(vehicle->GetChassis());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetHealth)
{
    // RVA 0x5D5020
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    // NOTE: a vehicle without a chassis is dereferenced as null.
    context->pushFloat(vehicle->GetChassis()->Health().value().get());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetMaxHealth)
{
    // RVA 0x5D5070
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    // NOTE: a vehicle without a chassis is dereferenced as null.
    context->pushFloat(vehicle->GetChassis()->Health().maxValue().get());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetFuel)
{
    // RVA 0x5D50C0
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    // NOTE: a vehicle without a chassis is dereferenced as null.
    context->pushFloat(vehicle->GetChassis()->Fuel().value().get());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetMaxFuel)
{
    // RVA 0x5D5110
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    // NOTE: a vehicle without a chassis is dereferenced as null.
    context->pushFloat(vehicle->GetChassis()->Fuel().maxValue().get());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetExternalPathByName)
{
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    auto name = context->asString(1);
    vehicle->SetExternalPathByName(name);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetCanBeDistractedFromMoving)
{
    // RVA 0x5CFEA0
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    vehicle->SetCanBeDistractedFromMoving(context->asBool(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, PlaceToEndOfPath)
{
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    vehicle->PlaceToEndOfPath();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetThrottle)
{
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    auto throttle = context->asFloat(1);
    auto autoBreak = context->asBool(2);
    vehicle->SetThrottle(throttle, autoBreak);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetThrottle)
{
    // RVA 0x5CFED0
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    context->pushFloat(vehicle->GetThrottle());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetCustomControlEnabled)
{
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    auto enable = context->asBool(1);
    vehicle->SetCustomControlEnabled(enable);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetSteer)
{
    // RVA 0x5CFF30
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    vehicle->SetSteer(context->asFloat(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetSteer)
{
    // RVA 0x5CFF60
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    context->pushFloat(vehicle->GetSteer());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, FireFromWeaponCustom)
{
    // RVA 0x5CFF90
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    CVector const& targetPoint = context->asVector(2);
    bool const enable = context->asBool(1);
    ai::WeaponFirer::FireFromWeaponsIfPossible(vehicle, enable, targetPoint, nullptr);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, FireFromWeaponCustom2)
{
    // RVA 0x5DF570
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    int const targetId = context->asInt(2);
    vehicle->FireFromWeaponCustom2(context->asBool(1), targetId);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, HoldFire)
{
    // RVA 0x5CFFD0
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    vehicle->HoldFire(context->asInt(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetCustomControlWeapons)
{
    // RVA 0x5D51C0
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    vehicle->SetCustomControlWeapons(context->asInt(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetCustomControlWeapons)
{
    // RVA 0x5D0020
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    context->pushInt(vehicle->GetCustomControlWeapons());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetCustomControlWeaponsTarget)
{
    // RVA 0x5D0050
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    vehicle->SetCustomControlWeaponsTarget(context->asVector(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetCustomControlWeaponsTarget)
{
    // RVA 0x5D0090
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    context->pushVector(vehicle->GetCustomControlWeaponsTarget());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetCustomControlWeaponsTargetObj)
{
    // RVA 0x5D00F0
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    vehicle->SetCustomControlWeaponsTargetObj(context->asInt(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetCustomControlWeaponsTargetObj)
{
    // RVA 0x5D0120
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    context->pushInt(vehicle->GetCustomControlWeaponsTargetObj());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetCustomLinearVelocity)
{
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    auto velocity = context->asFloat(1);
    vehicle->SetCustomLinearVelocity(velocity);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, AddItemsToRepository)
{
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    auto protoName = context->asString(1);
    auto amount = context->asInt(2);

    auto res = vehicle->AddItemsToRepository(protoName, amount);

    context->pushBool(res);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, RemoveItemsFromRepository)
{
    // RVA 0x5D0190
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    int const amount = context->asInt(2);
    char const* const prototypeName = context->asString(1);
    auto* const repository = vehicle->GetRepository();
    context->pushBool(repository ? repository->RemoveItems(prototypeName, amount) : false);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, HasAmountOfItemsInRepository)
{
    // RVA 0x5D01F0
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    int const amount = context->asInt(2);
    char const* const prototypeName = context->asString(1);
    auto const* const repository = vehicle->GetRepository();
    context->pushBool(repository ? repository->HasAmountOfItems(prototypeName, amount) : false);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, CanPlaceItemsToRepository)
{
    // RVA 0x5D0250
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    int const amount = context->asInt(2);
    char const* const prototypeName = context->asString(1);
    auto* const repository = vehicle->GetRepository();
    context->pushBool(repository ? repository->CanPlaceItems(prototypeName, amount) : false);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, AddObjectToRepository)
{
    // RVA 0x5D51F0
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    auto* const obj = dynamic_cast<ai::Obj*>(context->asObject(1, "Obj"));
    context->pushBool(vehicle->AddObjectToRepository(obj));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, TakeOffAllGuns)
{
    // RVA 0x5E9B50
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    context->pushAIParam(vehicle->TakeOffAllGuns());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, AttachTrailer)
{
    // RVA 0x5DF5B0
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    vehicle->AttachTrailer(context->asString(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, DetachTrailer)
{
    // RVA 0x5E2970
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    vehicle->DetachTrailer();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, TrailerExists)
{
    // RVA 0x5E2990
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    context->pushBool(vehicle->GetTrailer() != nullptr);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetTrailer)
{
    // RVA 0x5DF5E0
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    context->pushObject(vehicle->GetTrailer());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, getGodMode)
{
    // RVA 0x5D02B0
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    context->pushBool(vehicle->getGodMode());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, setGodMode)
{
    // RVA 0x5D02E0
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    vehicle->setGodMode(context->asBool(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, getImmortalMode)
{
    // RVA 0x5D0310
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    context->pushBool(vehicle->getImmortalMode());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, setImmortalMode)
{
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    auto enable = context->asBool(1);
    vehicle->setImmortalMode(enable);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetHorn)
{
    // RVA 0x5D0370
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    context->pushBool(vehicle->GetHorn());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetHorn)
{
    // RVA 0x5E9B90
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    vehicle->SetHorn(context->asBool(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetVisible)
{
    // RVA 0x5CB790
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    vehicle->SetVisible();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetInvisible)
{
    // RVA 0x5CB7B0
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    vehicle->SetInvisible();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetMaxTorque)
{
    // RVA 0x5D03A0
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    context->pushFloat(vehicle->GetMaxTorque());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetMaxTorque)
{
    // RVA 0x5D0410
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    vehicle->SetMaxTorque(context->asFloat(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetMaxSpeed)
{
    // RVA 0x5D5230
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    context->pushFloat(vehicle->GetMaxSpeed());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetMaxSpeed)
{
    // RVA 0x5D0480
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    vehicle->SetMaxSpeed(context->asFloat(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetCruisingSpeed)
{
    // RVA 0x5D04F0
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    context->pushFloat(vehicle->GetCruisingSpeed());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetCruisingSpeed)
{
    // RVA 0x5D5260
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    vehicle->SetCruisingSpeed(context->asFloat(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, LimitMaxSpeed)
{
    // RVA 0x5D0520
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    vehicle->LimitMaxSpeed(context->asFloat(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, UnlimitMaxSpeed)
{
    // RVA 0x5D0550
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    vehicle->UnlimitMaxSpeed();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetForcedMaxTorque)
{
    // RVA 0x5D0570
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    vehicle->SetForcedMaxTorque(context->asFloat(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, ResetForcedMaxTorque)
{
    // RVA 0x5D05A0
    auto* vehicle = dynamic_cast<ai::Vehicle*>(context->asObject(0, "Vehicle"));
    vehicle->ResetForcedMaxTorque();
    return 1;
}

CStr CABIN = "CABIN";
CStr CHASSIS = "CHASSIS";
CStr BASKET = "BASKET";
CStr ENGINE = "ENGINE";

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Vehicle)
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, SetRandomSkin, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, SetGamePositionOnGround, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, GetSize, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, GetCabin, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, GetBasket, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, GetChassis, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, GetHealth, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, GetMaxHealth, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, GetFuel, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, GetMaxFuel, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, SetExternalPathByName, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, SetCanBeDistractedFromMoving, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, PlaceToEndOfPath, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, SetThrottle, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, GetThrottle, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, SetCustomControlEnabled, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, SetSteer, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, GetSteer, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, FireFromWeaponCustom, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, FireFromWeaponCustom2, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, HoldFire, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, SetCustomControlWeapons, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, GetCustomControlWeapons, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, SetCustomControlWeaponsTarget, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, GetCustomControlWeaponsTarget, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, SetCustomControlWeaponsTargetObj, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, GetCustomControlWeaponsTargetObj, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, SetCustomLinearVelocity, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, AddItemsToRepository, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, RemoveItemsFromRepository, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, HasAmountOfItemsInRepository, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, CanPlaceItemsToRepository, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, AddObjectToRepository, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, TakeOffAllGuns, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, AttachTrailer, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, DetachTrailer, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, TrailerExists, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, GetTrailer, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, getGodMode, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, setGodMode, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, getImmortalMode, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, setImmortalMode, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, GetHorn, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, SetHorn, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, SetVisible, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, SetInvisible, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, GetMaxTorque, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, SetMaxTorque, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, GetMaxSpeed, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, SetMaxSpeed, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, GetCruisingSpeed, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, SetCruisingSpeed, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, LimitMaxSpeed, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, UnlimitMaxSpeed, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, SetForcedMaxTorque, "", "", "")
    RT_CLASS_EXPORT(Vehicle, m3d::METHOD, ResetForcedMaxTorque, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Vehicle);

    namespace
    {
        // A line in the ground plane, given by its (normalised) normal and a point on it.
        class FlatLine
        {
        public:
            /* 0x0000 */ CVector normal;
            /* 0x000c */ CVector origin;

            FlatLine()
            {
                // RVA 0x5D0630
                // NOTE: the original normalises (1, 1, normal.z) with normal.z left
                // uninitialised. Every line is overwritten by CreateOrthogonal before use.
                normal.x = 1.0f;
                normal.y = 1.0f;
                float const invLen = 1.0 / sqrt(normal.z * normal.z + 2.0);
                normal.x = invLen;
                normal.y = invLen;
                normal.z = normal.z * invLen;
                origin.y = 0.0f;
                origin.z = 0.0f;
                origin.x = 0.0f;
            }

            // The line through p1 whose normal is the ground-plane direction from p1 to p2.
            static FlatLine CreateOrthogonal(CVector const& p1, CVector const& p2)
            {
                // RVA 0x5D5680
                float const dx = p2.x - p1.x;
                float const dz = p2.z - p1.z;
                float const invLen = 1.0 / sqrt(dz * dz + dx * dx + 0.00000011920929);

                FlatLine line;
                line.normal.x = invLen * dx;
                line.normal.y = invLen * 0.0f;
                line.normal.z = invLen * dz;
                line.origin = p1;
                return line;
            }

            bool IsPointInFront(CVector const& point)
            {
                // RVA 0x5CB7F0 - in the ground plane: the point lies on the normal's side of the line.
                return point.z * normal.z + point.x * normal.x > origin.z * normal.z + origin.x * normal.x;
            }

            void RenderDebugInfo(unsigned int color)
            {
                // RVA 0x5D06C0 - a 40 m segment of the line, centred on its origin.
                CVector const halfSegment((0.0f - normal.z) * 20.0f, normal.y * 20.0f, normal.x * 20.0f);
                CVector p1 = origin - halfSegment;
                CVector p2 = origin + halfSegment;
                ai::DebugLineOnGround(p1, p2, 0.5, color);
            }
        }; /* size: 0x0018 */

        struct DrivingValues
        {
            /* 0x0000 */ FlatLine checkLine;
            /* 0x0018 */ float checkCircleRadius;
            /* 0x001c */ float nextAngle;
            /* 0x0020 */ float brakingCircleRadius;
        }; /* size: 0x0024 */

        // Signed ground-plane angle between the direction from the vehicle to point and the
        // segment from point to nextPoint.
        float GetAngleBetween(CVector const& vehiclePos, CVector const& point, CVector const& nextPoint)
        {
            // RVA 0x5D07A0
            if ((point - nextPoint).length() < 0.0099999998)
            {
                return 3.1415927f;
            }

            // Both directions are flattened: their Y is vehiclePos.y - vehiclePos.y, i.e. zero.
            float const flatY = vehiclePos.y - vehiclePos.y;

            CVector const toPoint(point.x - vehiclePos.x, flatY, point.z - vehiclePos.z);
            float const invLenToPoint =
                1.0 / sqrt(toPoint.z * toPoint.z + toPoint.y * toPoint.y + toPoint.x * toPoint.x + 0.00000011920929);
            CVector const dirToPoint(invLenToPoint * toPoint.x, toPoint.y * invLenToPoint, toPoint.z * invLenToPoint);

            CVector const segment(nextPoint.x - point.x, flatY, nextPoint.z - point.z);
            float const invLenSegment =
                1.0 / sqrt(segment.z * segment.z + segment.y * segment.y + segment.x * segment.x + 0.00000011920929);
            CVector const dirSegment(invLenSegment * segment.x, segment.y * invLenSegment, segment.z * invLenSegment);

            float cosAngle = dirSegment.z * dirToPoint.z + dirSegment.y * dirToPoint.y + dirSegment.x * dirToPoint.x;
            if (cosAngle < -0.99999899f)
            {
                cosAngle = -0.99999899f;
            }
            else if (cosAngle > 0.99999899f)
            {
                cosAngle = 0.99999899f;
            }

            float const cross = dirSegment.x * dirToPoint.z - dirSegment.z * dirToPoint.x;
            int const sign = cross < 0.0f ? -1 : 1;
            return acos(cosAngle) * sign;
        }

        void CalcDrivingValues(
            Vehicle const& vehicle,
            CVector const& point,
            CVector const& nextPoint,
            bool bPrecisely,
            DrivingValues& dv)
        {
            // RVA 0x5D57A0
            dv.nextAngle = GetAngleBetween(vehicle.GetPosition(), point, nextPoint);

            float absAngle = fabs(dv.nextAngle);
            if (absAngle < 0.0f)
            {
                absAngle = 0.0f;
            }
            else if (absAngle > 2.5132742f)
            {
                absAngle = 2.5132742f;
            }

            // The check line crosses the path at point, pulled back by a fifth of the vehicle length.
            auto const vehicleSize = vehicle.GetSize();
            dv.checkLine = FlatLine::CreateOrthogonal(point, nextPoint);
            dv.checkLine.origin -= dv.checkLine.normal * vehicleSize.x * 0.2f;

            // The sharper the turn at point, the smaller the circle within which it counts as
            // reached, but never smaller than half the vehicle's diagonal.
            dv.checkCircleRadius = (2.2f - absAngle * 0.7957747f) * vehicleSize.x;
            float const halfDiagonal = vehicleSize.length() * 0.5f;
            if (halfDiagonal > dv.checkCircleRadius)
            {
                dv.checkCircleRadius = halfDiagonal;
            }
            if (dv.checkCircleRadius > 1.0e30f)
            {
                dv.checkCircleRadius = 1.0e30f;
            }
            if (!bPrecisely)
            {
                dv.checkCircleRadius = dv.checkCircleRadius * 3.0f;
            }

            float const speed = vehicle.GetLinearVelocity().length();
            dv.brakingCircleRadius = fabs(dv.nextAngle) * (speed * speed * 0.025484199f);
        }
    }  // namespace

    extern AIManager* theAIManager;

    VehiclePrototypeInfo::WheelInfo::WheelInfo(CStr wheelPrototypeName, Wheel::WheelSteering steering)
    {
        this->m_wheelPrototypeId = -1;
        this->m_steering = steering;
        this->m_wheelPrototypeName = wheelPrototypeName;
    }

    void VehiclePrototypeInfo::WheelInfo::PostLoad()
    {
        m_wheelPrototypeId = thePrototypeManager->GetPrototypeId(m_wheelPrototypeName);
    }

    VehiclePrototypeInfo::VehiclePrototypeInfo()
    {
        this->m_selfBrakingCoeff = 0.0060000001;
        this->m_diffRatio = 1.0;
        this->m_maxEngineRpm = 1.0;
        this->m_lowGearShiftLimit = 1.0;
        this->m_highGearShiftLimit = 1.0;
        this->m_steeringSpeed = 1.0;
        this->m_takingRadius = 1.0;
        this->m_priority = -56;
        this->m_decisionMatrixNum = -1;
        this->m_cameraHeight = -1.0;
        this->m_cameraMaxDist = 25.0;
        this->m_blastWavePrototypeId = -1;
        this->m_additionalWheelsHover = 0.0;
        this->m_driftCoeff = 1.0;
        this->m_pressingForce = 1.0;
        this->m_healthRegeneration = 0.0;
        this->m_durabilityRegeneration = 0.0;
        for (auto& name : this->m_destroyEffectNames)
        {
            name = "ET_PS_VEH_EXP";
        }
        this->m_bVisibleInEncyclopedia = 0;
    }

    namespace
    {
        char const* DestroyEffectNames[] = {
            "DestroyEffectPiercing",
            "DestroyEffectBlast",
            "DestroyEffectEnergy",
            "DestroyEffectWater"};
    }

    bool VehiclePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ComplexPhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_diffRatio, xmlNode, "DiffRatio");
            m3d::SafeFloatAttrib(m_maxEngineRpm, xmlNode, "MaxEngineRpm");
            m3d::SafeFloatAttrib(m_lowGearShiftLimit, xmlNode, "LowGearShiftLimit");
            m3d::SafeFloatAttrib(m_highGearShiftLimit, xmlNode, "HighGearShiftLimit");
            m3d::SafeFloatAttrib(m_selfBrakingCoeff, xmlNode, "SelfBrakingCoeff");
            m3d::SafeFloatAttrib(m_steeringSpeed, xmlNode, "SteeringSpeed");

            CStr decisionMatrixName;
            m3d::SafeStrAttrib(decisionMatrixName, xmlNode, "DecisionMatrix");
            if (!decisionMatrixName.empty())
            {
                theAIManager->LoadMatrix(decisionMatrixName.c_str());
                m_decisionMatrixNum = theAIManager->GetMatrixNum(decisionMatrixName);
            }

            m3d::SafeFloatAttrib(m_takingRadius, xmlNode, "TakingRadius");
            m3d::SafeUintAttrib((unsigned&)m_priority, xmlNode, "Priority");
            m3d::SafeStrAttrib(m_hornSoundName, xmlNode, "HornSound");
            m3d::SafeFloatAttrib(m_cameraHeight, xmlNode, "CameraHeight");
            m3d::SafeFloatAttrib(m_cameraMaxDist, xmlNode, "CameraMaxDist");

            for (int i = 0; i < 4; ++i)
            {
                m3d::SafeStrAttrib(m_destroyEffectNames[i], xmlNode, DestroyEffectNames[i]);
            }

            ref_ptr wheelsNode = xmlFile->CreateNode();
            xmlNode->GetFirstChild(wheelsNode, "Wheels");
            if (!wheelsNode->IsEmpty())
            {
                if (!m_parentPrototypeName.empty())
                {
                    M3D_LOG_ERR("Error: wheels info is present for inherited vehicle '" + m_prototypeName + "'");
                    M3D_CRITICAL_ERROR("");
                }

                m_wheelInfos.clear();
                ref_ptr wheelNode = xmlFile->CreateNode();
                for (wheelsNode->GetFirstChild(wheelNode, "Wheel"); !wheelNode->IsEmpty();
                     wheelNode->GetNextSibling(wheelNode, "Wheel"))
                {
                    CStr wheelPrototypeName;
                    m3d::SafeStrAttrib(wheelPrototypeName, wheelNode, "Prototype");

                    CStr steeringStr;
                    m3d::SafeStrAttrib(steeringStr, wheelNode, "steering");

                    Wheel::WheelSteering steering = Wheel::WheelSteering::STEERING_NO;
                    if (steeringStr == "correct")
                    {
                        steering = Wheel::WheelSteering::STEERING_CORRECT;
                    }
                    else if (steeringStr == "inverse")
                    {
                        steering = Wheel::WheelSteering::STEERING_INVERSE;
                    }

                    WheelInfo wheelInfo(wheelPrototypeName, steering);
                    m_wheelInfos.push_back(std::move(wheelInfo));
                }
            }

            m3d::SafeStrAttrib(m_blastWavePrototypeName, xmlNode, "BlastWave");
            m3d::SafeFloatAttrib(m_additionalWheelsHover, xmlNode, "AdditionalWheelsHover");
            m3d::SafeFloatAttrib(m_driftCoeff, xmlNode, "DriftCoeff");
            m3d::SafeFloatAttrib(m_pressingForce, xmlNode, "PressingForce");
            m3d::SafeFloatAttrib(m_healthRegeneration, xmlNode, "HealthRegeneration");
            m3d::SafeFloatAttrib(m_durabilityRegeneration, xmlNode, "DurabilityRegeneration");
        }
        return result;
    }

    void VehiclePrototypeInfo::PostLoad()
    {
        ComplexPhysicObjPrototypeInfo::PostLoad();
        for (auto& wheelInfo : this->m_wheelInfos)
        {
            wheelInfo.PostLoad();
        }
        m_blastWavePrototypeId = thePrototypeManager->GetPrototypeId(m_blastWavePrototypeName);
    }

    // RVA 0x5E51F0 - only destroys the members.
    VehiclePrototypeInfo::~VehiclePrototypeInfo() = default;

    ai::Obj* VehiclePrototypeInfo::CreateTargetObject() const
    {
        return new Vehicle(*this);
    }

    void VehiclePrototypeInfo::_InternalCopyFrom(PrototypeInfo const& rhs)
    {
        *this = static_cast<VehiclePrototypeInfo const&>(rhs);
    }

    Wheel const* Vehicle::WheelRuntimeInfo::GetWheel() const
    {
        return m_wheel;
    }

    Wheel* Vehicle::WheelRuntimeInfo::GetWheel()
    {
        return m_wheel;
    }

    Vehicle::WheelRuntimeInfo::WheelRuntimeInfo(Wheel* wheel)
    {
        this->m_initialPos = {0.0, 0.0, 0.0};
        this->m_initialRot = {0.0, 0.0, 0.0, 1.0};
        this->m_bWheelPresent = wheel != 0;
        this->m_wheel = wheel;
    }

    bool Vehicle::WheelRuntimeInfo::IsWheelPresent() const
    {
        // RVA 0x5CE880
        return m_bWheelPresent;
    }

    void Vehicle::WheelRuntimeInfo::SetWheel(Wheel* wheel)
    {
        // RVA 0x5CE8F0
        m_wheel = wheel;
        m_bWheelPresent = wheel != nullptr;
    }

    float Vehicle::GetCruisingSpeed() const
    {
        return this->m_cruisingSpeed;
    }

    void Vehicle::UnlimitMaxSpeed()
    {
        // RVA 0x5CC420
        m_maxSpeedLimited = false;
    }

    void Vehicle::IncStoppageMode()
    {
        // RVA 0x5CCD10
        ++m_stoppageMode;
    }

    void Vehicle::EnablePhysics()
    {
        ComplexPhysicObj::EnablePhysics();
        for (auto& wheelInfo : m_wheels)
        {
            if (auto* wheel = wheelInfo.GetWheel())
            {
                wheel->EnablePhysics();
            }
        }
    }

    int Vehicle::GetInfoObjId() const
    {
        auto* player = RT_DYNCAST(GetParent(), Player);
        return player->GetInfoObjId();
    }

    bool Vehicle::bRocketLaunchersPresent() const
    {
        // RVA 0x5CCF20
        return m_bRocketLaunchersPresent;
    }

    void Vehicle::RecalcGadgets()
    {
        // RVA 0x5DCAF0
        // Detach any gadget whose slot is no longer valid and stow it in the repository.
        for (auto it = m_gadgets.begin(); it != m_gadgets.end();)
        {
            auto* gadget = it->second;
            if (it->first == GetValidSlotIdForGadget(gadget))
            {
                ++it;
                continue;
            }
            ++it;
            RemoveChild(gadget);
            if (m_repository)
            {
                m_repository->AddThing(GeomRepositoryItem(gadget->GetId()), 0);
            }
        }
    }

    void Vehicle::HoldFire(int msc)
    {
        // RVA 0x5CBF40
        m_bIsShooting = false;
        m_shootTypeChangeTime = m3d::g_Kernel->GetTimer().GetCurTime();
        m_shootTimeToWait = static_cast<unsigned>(msc);
    }

    VehicleRecollection* Vehicle::GetRecollection() const
    {
        return RT_DYNCAST(theObjects->GetEntityByObjId(m_recollectionId), VehicleRecollection);
    }

    void Vehicle::SetLinearVelocity(CVector const& linearVel)
    {
        PhysicObj::SetLinearVelocity(linearVel);

        for (auto& wheelInfo : m_wheels)
        {
            if (auto* wheel = wheelInfo.GetWheel())
            {
                wheel->SetLinearVelocity(linearVel);
            }
        }

        if (auto* trailer = theObjects->GetEntityByObjId(m_trailerObjId))
        {
            auto* trailerVehicle = RT_DYNCAST(trailer, Vehicle);
            trailerVehicle->SetLinearVelocity(linearVel);
        }
    }

    Vehicle::CustomWeaponControlType Vehicle::GetCustomControlWeapons() const
    {
        // RVA 0x5CBBC0
        return m_customControlWeapons;
    }

    Quaternion Vehicle::GetWheelInitialRotation(unsigned num)
    {
        // RVA 0x5DA0D0
        return m_wheels[num].m_initialRot;
    }

    void Vehicle::ActivateHeadLights(bool bActivate)
    {
        // RVA 0x5DA130: LIGHT_ACTION toggles between AT_RESERVED1 (off) and
        // AT_RESERVED2 (on); re-push the action list to the basket and cabin.
        ActionType const wanted = bActivate ? AT_RESERVED2 : AT_RESERVED1;
        if (m_effectActions[LIGHT_ACTION] == wanted)
        {
            return;
        }
        m_effectActions[LIGHT_ACTION] = wanted;
        if (auto* basket = GetBasket())
        {
            basket->SetEffectActions(m_effectActions);
        }
        if (auto* cabin = GetCabin())
        {
            cabin->SetEffectActions(m_effectActions);
        }
    }

    void Vehicle::Flow(Obj* partToFlow, float averageSpeed)
    {
        // RVA 0x5DA290 - besides tearing off parts, a vehicle can shed a wheel, which flies off and later vanishes.
        if (!partToFlow)
        {
            return;
        }
        ComplexPhysicObj::Flow(partToFlow, averageSpeed);
        if ((GetFlags() & 2) != 0)
        {
            return;
        }

        auto wheelInfo = m_wheels.begin();
        while (wheelInfo != m_wheels.end() && wheelInfo->GetWheel() != partToFlow)
        {
            ++wheelInfo;
        }
        if (wheelInfo == m_wheels.end())
        {
            return;
        }

        Wheel* const wheel = wheelInfo->GetWheel();
        wheel->DetachFromPhysicObj();
        wheel->TransferToNewSpace();

        // Away from the vehicle's centre raised by its height, roughly, at 0.5 to 1.5 times the average speed.
        float const height = m_size.y;
        CVector const vehiclePos = GetPosition();
        CVector const wheelPos = wheel->GetPosition();
        CVector const away(wheelPos.x - vehiclePos.x, wheelPos.y - vehiclePos.y + height, wheelPos.z - vehiclePos.z);
        float const invLen =
            static_cast<float>(1.0 / sqrt(static_cast<double>(away.z) * away.z + static_cast<double>(away.y) * away.y +
                                          static_cast<double>(away.x) * away.x + 0.00000011920929f));
        CVector const flyDir = GetRandomDeviatedVector(CVector(invLen * away.x, away.y * invLen, away.z * invLen), 1.0f);

        float const lowSpeed = averageSpeed * 0.5f;
        float const highSpeed = averageSpeed * 1.5f;
        float const minSpeed = std::min(lowSpeed, highSpeed);
        float const maxSpeed = std::max(lowSpeed, highSpeed);
        float const speed = static_cast<float>(rand()) * (maxSpeed - minSpeed) * 0.000030518509f + minSpeed;
        wheel->SetLinearVelocity(CVector(flyDir.x * speed, flyDir.y * speed, flyDir.z * speed));
        wheel->SetAutoDisabling(true, 0.1f, 0.1f, 5);
        wheel->SetDeadTimer(60000, true);
        wheel->GetPhysicBody()->SetNodeAction(2 * rand() / 0x8000 == 1 ? 8 : 9, true);

        wheelInfo->SetWheel(nullptr);
    }

    void Vehicle::DetachTrailer()
    {
        // RVA 0x5E0CF0
        if (auto* trailer = theObjects->GetEntityByObjId(m_trailerObjId))
        {
            dJointDestroy(m_trailerJoint);
            m_trailerJoint = nullptr;
            trailer->Remove();
            m_trailerObjId = -1;
        }
    }

    void Vehicle::AddChild(Obj* pObj)
    {
        // RVA 0x5E5E00
        ComplexPhysicObj::AddChild(pObj);
        if (!pObj)
        {
            return;
        }
        // Bullets just get parented; gadgets additionally take a slot in m_gadgets.
        if (!pObj->IsKindOf(RT_CLASS_LOCAL(Bullet)))
        {
            auto* gadget = RT_DYNCAST(pObj, Gadget);
            if (!gadget)
            {
                return;
            }
            m_gadgets.insert({gadget->GetSlotNum(), gadget});
            M3D_APP->EnqueueMessage(66551, GetId(), gadget->GetSlotNum(), 0, 0, {}, {});
        }
        pObj->LinkToParent(GetId(), HIERARCHY_CHILD);
    }

    void Vehicle::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x5D9350
        ComplexPhysicObj::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("TimeAfterDeath", CStr(m_timeAfterDeath).c_str());
        xmlNode->SetAttribute("NumBlownParts", CStr(m_numBlownParts).c_str());
        xmlNode->SetAttribute("TimeAfterLastBlow", CStr(m_timeAfterLastBlow).c_str());
        if (_GetDeadStatus())
        {
            xmlNode->SetAttribute("DeathDamage", CStr(static_cast<int>(m_deathDamage)).c_str());
        }
        xmlNode->SetAttribute("CurrentGear", CStr(m_currentGear).c_str());
        xmlNode->SetAttribute("Throttle", CStr(m_throttle).c_str());
        xmlNode->SetAttribute("RealThrottle", CStr(m_realThrottle).c_str());
        xmlNode->SetAttribute("AutoBrake", CStr(static_cast<int>(m_bAutoBrake)).c_str());
        xmlNode->SetAttribute("HandBrake", CStr(static_cast<int>(m_bHandBrake)).c_str());
        xmlNode->SetAttribute("EngineRPM", CStr(m_engineRpm).c_str());
        if (m_npcMotionControllerId != -1)
        {
            xmlNode->SetAttribute("NpcMotionControllerId", CStr(m_npcMotionControllerId).c_str());
        }
        xmlNode->SetAttribute("CurrentDestination", CStr(m_currentDestination).c_str());
        xmlNode->SetAttribute("PathNum", CStr(m_pathNum).c_str());
        xmlNode->SetAttribute("Priority", CStr(static_cast<int>(m_priority)).c_str());
        xmlNode->SetAttribute("PathIndex", CStr(m_pathIndex).c_str());
        xmlNode->SetAttribute("IsMovingAlongExternalPath", CStr(static_cast<int>(m_bIsMovingAlongExternalPath)).c_str());
        xmlNode->SetAttribute("CanBeDistractedFromMoving", CStr(static_cast<int>(m_bCanBeDistractedFromMoving)).c_str());
        xmlNode->SetAttribute("StoppageMode", CStr(m_stoppageMode).c_str());
        xmlNode->SetAttribute("OnOilMode", CStr(m_onOilMode).c_str());
        xmlNode->SetAttribute("InSmokeScreenMode", CStr(m_inSmokeScreenMode).c_str());
        xmlNode->SetAttribute("TurboThrottleTime", CStr(m_turboThrottleTime).c_str());
        xmlNode->SetAttribute("TurboThrottleValue", CStr(m_turboThrottleValue).c_str());
        xmlNode->SetAttribute("ImmortalMode", CStr(static_cast<int>(m_bImmortalMode)).c_str());
        xmlNode->SetAttribute("Hidden", CStr(static_cast<int>(m_bHidden)).c_str());
        xmlNode->SetAttribute("MoveStatus", CStr(static_cast<int>(m_moveStatus)).c_str());
        xmlNode->SetAttribute("CruisingSpeed", CStr(m_cruisingSpeed).c_str());
        xmlNode->SetAttribute("Role", CStr(m_roleId).c_str());
        xmlNode->SetAttribute("RecollectionId", CStr(m_recollectionId).c_str());
        xmlNode->SetAttribute("WasStuck", CStr(static_cast<int>(m_bWasStuck)).c_str());
        xmlNode->SetAttribute("PrevPosToCheckStuck", CStr(m_prevPosToCheckStuck).c_str());
        xmlNode->SetAttribute("TimeOutToCheckStuck", CStr(m_timeOutToCheckStuck).c_str());
        if (m_bIsControlledByPlayer)
        {
            xmlNode->SetAttribute("PastTakingPos", CStr(m_pastTakingSpherePosition).c_str());
            // m3d::XmlNodeSetAttribute<bool>, inlined.
            xmlNode->SetAttribute("AllowInventoryMessage", CStr(static_cast<int>(m_bAllowPickUpMessage)).c_str());
        }

        if (m_pPath)
        {
            ref_ptr pathNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Path");
            xmlNode->AddChild(pathNode);
            m_pPath->SaveToXML(xmlFile, pathNode);
        }

        if (!m_gunsPointed.empty())
        {
            ref_ptr gunsNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "GunPointed");
            xmlNode->AddChild(gunsNode);
            for (auto const& [gunId, pointed] : m_gunsPointed)
            {
                ref_ptr gunNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Gun");
                gunsNode->AddChild(gunNode);
                gunNode->SetAttribute("Id", CStr(gunId).c_str());
                gunNode->SetAttribute("Pointed", CStr(static_cast<int>(pointed)).c_str());
            }
        }

        // The second effect action is the headlights: 13 when they are on, 12 when off.
        if (m_effectActions[1] == 13)
        {
            xmlNode->SetAttribute("Headlights", CStr(1).c_str());
        }

        ref_ptr wheelsNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Wheels");
        xmlNode->AddChild(wheelsNode);
        for (auto it = m_wheels.begin(); it != m_wheels.end(); ++it)
        {
            ref_ptr wheelInfoNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "WheelInfo");
            wheelsNode->AddChild(wheelInfoNode);
            wheelInfoNode->SetAttribute("Id", CStr(static_cast<int>(it - m_wheels.begin())).c_str());
            wheelInfoNode->SetAttribute("present", CStr(static_cast<int>(it->IsWheelPresent())).c_str());
            if (it->IsWheelPresent())
            {
                ref_ptr wheelNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Wheel");
                wheelInfoNode->AddChild(wheelNode);
                it->GetWheel()->SaveToXML(xmlFile, wheelNode);
            }
        }
    }

    bool Vehicle::GetHorn() const
    {
        // RVA 0x5CCCB0
        return m_bHorn;
    }

    bool Vehicle::IsTrailer() const
    {
        return m_bIsTrailer;
    }

    float Vehicle::GetCameraHeight() const
    {
        return m_cameraHeight;
    }

    int Vehicle::GetLockedObjId() const
    {
        // RVA 0x5CCEF0
        return m_lockedObjId;
    }

    void Vehicle::setGodMode(bool bGod)
    {
        // RVA 0x5CCCD0
        m_bGodMode = bGod;
    }

    retruxx::map<int, Gadget*, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, Gadget*>>> const&
        Vehicle::GetGadgets() const
    {
        // RVA 0x5CBDF0
        return m_gadgets;
    }

    void Vehicle::SetRandomSkin()
    {
        // RVA 0x5DD0B0
        // NOTE: the shipped code only randomises when there are more than two skins, and scales rand() by
        // (count - 1), so the last skin is never picked; with one or two skins it always takes the first.
        auto* cabin = GetPartByName(CABIN);
        if (!cabin || !cabin->IsKindOf(RT_CLASS_LOCAL(Cabin)))
        {
            return;
        }

        auto* mdl = cabin->GetModel();
        if (!mdl)
        {
            return;
        }

        auto const& loadedSkins = mdl->GetLoadedSkins();
        if (loadedSkins.loadAllSkins)
        {
            unsigned const numSkins = mdl->GetNumSkins();
            SetSkin(numSkins > 2 ? ((numSkins - 1) * static_cast<unsigned>(rand())) >> 15 : 0);
        }
        else
        {
            // NOTE: with an empty set the original passes the tree head's uninitialised value; we pass 0.
            if (loadedSkins.loadSkins.empty())
            {
                SetSkin(0);
                return;
            }

            int const numSkins = static_cast<int>(loadedSkins.loadSkins.size());
            auto it = loadedSkins.loadSkins.begin();
            if (numSkins > 2)
            {
                std::advance(it, (numSkins - 1) * rand() / 0x8000);
            }
            SetSkin(*it);
        }
    }

    float Vehicle::GetDefaultCruisingSpeed() const
    {
        // RVA 0x5D14C0
        return GetMaxSpeed() * 0.60000002f;
    }

    float Vehicle::GetMaxEngineRpm() const
    {
        // RVA 0x5CBC60
        return m_maxEngineRpm;
    }

    int Vehicle::SetExternalPath(retruxx::vector<CVector2, retruxx::allocator<CVector2>> const& path)
    {
        delete m_pPath;
        m_pPath = new ai::Path(path);
        ++m_pathIndex;
        m_bIsMovingAlongExternalPath = true;
        m_moveStatus = VehicleMoveStatus::MOVE_MOVING_ALONG_PATH;
        m_pathNum = 0;
        return m_pathIndex;
    }

    Chassis const* Vehicle::GetChassis() const
    {
        // RVA 0x5CBA30
        return dynamic_cast<Chassis const*>(GetPartByName(CHASSIS));
    }

    Chassis* Vehicle::GetChassis()
    {
        // RVA 0x5CB9A0
        return RT_DYNCAST(GetPartByName(CHASSIS), Chassis);
    }

    void Vehicle::Remove()
    {
        for (auto& wheel : m_wheels)
        {
            if (auto* wheelPtr = wheel.GetWheel())
            {
                wheelPtr->DetachFromPhysicObj();
                wheelPtr->Remove();
            }
        }

        for (auto& [id, gadget] : m_gadgets)
        {
            if (gadget)
            {
                gadget->Remove();
            }
        }

        auto trailer = theObjects->GetEntityByObjId(m_trailerObjId);
        if (trailer)
        {
            dJointDestroy(m_trailerJoint);
            m_trailerJoint = nullptr;
            trailer->Remove();
            m_trailerObjId = -1;
        }

        auto recollection = theObjects->GetEntityByObjId(m_recollectionId);
        if (recollection)
        {
            recollection->Remove();
        }

        auto role = theObjects->GetEntityByObjId(m_roleId);
        if (role)
        {
            role->Remove();
        }
        m_roleId = -1;

        ComplexPhysicObj::Remove();
    }

    void Vehicle::FireFromWeaponCustom2(bool enable, int targetId)
    {
        // RVA 0x5DCBF0
        if (auto* target = theObjects->GetEntityByObjId(targetId))
        {
            WeaponFirer::FireFromWeaponsIfPossible(
                this, enable, getPhysicObjOrPhysicBodyGeometricCenter(target), target);
        }
    }

    float Vehicle::GetCameraMaxDist() const
    {
        return this->m_cameraMaxDist;
    }

    bool Vehicle::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        if (propertyId == 12)
        {
            this->m_driftCoeff = newValue.GetAsFloat();
            return 1;
        }
        else if (propertyId == 13)
        {
            this->m_antiMissileGadgetSavingRadius = newValue.GetAsFloat();
            return 1;
        }
        else
        {
            return ai::PhysicObj::SetPropertyById(propertyId, newValue);
        }
    }

    void Vehicle::DecOnOilMode()
    {
        // RVA 0x5CCD50
        --m_onOilMode;
    }

    unsigned Vehicle::GetNumWheels() const
    {
        return m_wheels.size();
    }

    void Vehicle::SetHandBrake()
    {
        // RVA 0x5CC2B0
        m_bHandBrake = true;
    }

    CVector Vehicle::GetRecollectionPosition(float recollectionRange) const
    {
        auto* recollection = RT_DYNCAST(theObjects->GetEntityByObjId(m_recollectionId), VehicleRecollection);
        if (recollection)
        {
            auto range = (double)rand() * 0.000030518509 * recollectionRange;
            auto time = theObjects->GetGameTimeDiff() - ai::theGlobProp.m_gameTimeMult * range;
            return recollection->GetRecollectionPosition(time);
        }
        return GetGeometricCenter();
    }

    float Vehicle::GetSteer() const
    {
        // RVA 0x5CC2C0
        return m_steerRadians;
    }

    void Vehicle::IncNumWheelsTouchingGround()
    {
        ++m_numWheelsTouchingGround;
    }

    void Vehicle::CreateChildren()
    {
        ai::ComplexPhysicObj::CreateChildren();
        ai::Vehicle::_UpdateRepositoryOnChangeBasket();
    }

    void Vehicle::SetCustomControlEnabled(bool value)
    {
        this->m_bCustomControl = value;
    }

    float Vehicle::GetMaxFiringRangeAI() const
    {
        // RVA 0x5CC020 (thunk)
        return WeaponFirer::GetMaxFiringRange(this);
    }

    m3d::Class* Vehicle::GetBaseClass()
    {
        return RT_CLASS_LOCAL(ComplexPhysicObj);
    }

    float Vehicle::GetMaxFuel() const
    {
        // RVA 0x5D0C80: unchecked, as shipped.
        return GetChassis()->Fuel().maxValue().get();
    }

    bool Vehicle::getImmortalMode() const
    {
        // RVA 0x5CCCE0
        return m_bImmortalMode;
    }

    float Vehicle::GetHealth() const
    {
        // RVA 0x5D0BC0: the shipped code dereferences GetChassis() unchecked.
        return GetChassis()->Health().value().get();
    }

    bool Vehicle::AddThing(GeomRepositoryItem const& item, bool bFlushInReferenceChests)
    {
        // RVA 0x5CCB50: try the vehicle repository first, then the ground one.
        if (m_repository && m_repository->AddThing(item, 0))
        {
            return true;
        }
        if (!m_groundRepository)
        {
            return false;
        }
        bool const added = m_groundRepository->AddThing(item, 0);
        if (bFlushInReferenceChests)
        {
            m_groundRepository->FlushInReferenceChests(GetPosition());
        }
        return added;
    }

    void Vehicle::EnableSounds(bool bEnable)
    {
        if (m_engineHighSoundNode)
            m_engineHighSoundNode->SetProperty(9733u, &bEnable);
        if (m_engineLowSoundNode)
            m_engineLowSoundNode->SetProperty(9733u, &bEnable);
    }

    void Vehicle::TransferToSpace(dxSpace* newSpace)
    {
        // RVA 0x5D9F00
        ComplexPhysicObj::TransferToSpace(newSpace);
        for (auto& wheelInfo : m_wheels)
        {
            if (auto* wheel = wheelInfo.GetWheel())
            {
                wheel->TransferToSpace(newSpace);
            }
        }
    }

    m3d::AIParam Vehicle::VehicleAIOnAttack(Obj*)
    {
        // RVA 0x5E4B60: registered AI hook, returns an empty AIParam in the shipped build.
        return {};
    }

    void Vehicle::SetCanBeDistractedFromMoving(bool bCanBeDistracted)
    {
        // RVA 0x5CCAB0
        m_bCanBeDistractedFromMoving = bCanBeDistracted;
    }

    int Vehicle::GetCustomControlWeaponsTargetObj() const
    {
        // RVA 0x5CBC30
        return m_customControlWeaponsTargetObjId;
    }

    int Vehicle::GetToBeLockedObjId() const
    {
        // RVA 0x5CCF00
        return m_toBeLockedObjId;
    }

    void Vehicle::SetSkin(int skin)
    {
        ComplexPhysicObj::SetSkin(skin);
        for (auto& wheelInfo : m_wheels)
        {
            if (auto* wheel = wheelInfo.GetWheel())
            {
                wheel->SetSkin(skin);
            }
        }
    }

    void Vehicle::DisablePhysics()
    {
        // RVA 0x5DA820
        ComplexPhysicObj::DisablePhysics();
        for (auto& wheelInfo : m_wheels)
        {
            if (auto* wheel = wheelInfo.GetWheel())
            {
                wheel->DisablePhysics();
            }
        }
    }

    void Vehicle::FireFromWeaponCustom(bool enable, CVector const& targetPoint, Obj* target)
    {
        // RVA 0x5CBF20
        WeaponFirer::FireFromWeaponsIfPossible(this, enable, targetPoint, target);
    }

    float Vehicle::GetMass() const
    {
        float res = PhysicObj::GetMass();
        for (auto const& wheelInfo : m_wheels)
        {
            if (auto const* wheel = wheelInfo.GetWheel())
            {
                res += wheel->GetMass();
            }
        }
        return res;
    }

    unsigned Vehicle::GetPrice(IPriceCoeffProvider const* priceCoeffProvider) const
    {
        // RVA 0x5DA620: the vehicle plus everything it carries.
        unsigned price = ComplexPhysicObj::GetPrice(priceCoeffProvider);
        if (m_repository)
        {
            unsigned const numItems = m_repository->GetNumItems();
            for (unsigned i = 0; i < numItems; ++i)
            {
                if (auto const* obj = m_repository->GetItem(i).GetObj())
                {
                    price += obj->GetPrice(priceCoeffProvider);
                }
            }
        }
        for (auto const& entry : m_gadgets)
        {
            if (entry.second)
            {
                price += entry.second->GetPrice(priceCoeffProvider);
            }
        }
        return price;
    }

    bool Vehicle::FireFromWeaponByGunId(int gunId, bool enable)
    {
        // RVA 0x5DCD10
        if (gunId == -1)
        {
            return false;
        }
        Obj* const gun = theObjects->GetEntityByObjId(gunId);
        // NOTE: the gun has to be both a Gun and a CompoundGun, which no object is, so this never fires.
        if (gun && gun->IsKindOf(RT_CLASS_LOCAL(Gun)) && gun->IsKindOf(RT_CLASS_LOCAL(CompoundGun)))
        {
            return FireFromWeaponByGunPartName(static_cast<VehiclePart*>(gun)->GetPartName(), enable);
        }
        return false;
    }

    void Vehicle::SetMoveStatus(VehicleMoveStatus moveStatus)
    {
        this->m_moveStatus = moveStatus;
    }

    void Vehicle::SetPassedToAnotherMapStatus()
    {
        // RVA 0x5EA560 - everything the vehicle carries or tows goes along with it.
        _SetIdleMoveStatus();
        if (m_bIsControlledByPlayer)
        {
            SetHorn(false);
        }
        m_engineHighSoundNode = nullptr;
        m_engineLowSoundNode = nullptr;
        ComplexPhysicObj::SetPassedToAnotherMapStatus();

        if (m_repository)
        {
            for (unsigned int slot = 0; slot < m_repository->GetNumItems(); ++slot)
            {
                if (Obj* const item = theObjects->GetEntityByObjId(m_repository->GetItem(slot).GetObjId()))
                {
                    item->SetPassedToAnotherMapStatus();
                }
            }
        }
        for (auto& wheelInfo : m_wheels)
        {
            if (Wheel* const wheel = wheelInfo.GetWheel())
            {
                wheel->SetPassedToAnotherMapStatus();
            }
        }
        for (auto const& [slot, gadget] : m_gadgets)
        {
            if (gadget)
            {
                gadget->SetPassedToAnotherMapStatus();
            }
        }
        if (Obj* const trailer = theObjects->GetEntityByObjId(m_trailerObjId))
        {
            trailer->SetPassedToAnotherMapStatus();
        }
        if (Obj* const recollection = theObjects->GetEntityByObjId(m_recollectionId))
        {
            recollection->SetPassedToAnotherMapStatus();
        }

        m_roleId = -1;
        m_currentNearbyObstacles.clear();
        m_pastNearbyObstacles.clear();
        m_pastTakingSpherePosition = ZeroVector;
        m_pastNumNearbyChests = 0;
        m_currentNumNearbyChests = 0;
    }

    void Vehicle::SetCruisingSpeed(float cruisingSpeed)
    {
        auto maxSpeed = GetMaxSpeed();
        if (cruisingSpeed < 0.0)
            cruisingSpeed = 0.0;
        if (cruisingSpeed > maxSpeed)
            cruisingSpeed = maxSpeed;
        this->m_cruisingSpeed = cruisingSpeed;
    }

    void Vehicle::SetExternalDestination(CVector const& destination)
    {
        this->m_externalDestination = destination;
        this->m_moveStatus = Vehicle::MOVE_MOVING_BY_STEERING_FORCE;
    }

    void Vehicle::Registration()
    {
        theAIManager->RegisterFunc("VehicleAIOnDefend", &Vehicle::VehicleAIOnDefend);
        theAIManager->RegisterFunc("VehicleAIOnAttack", &Vehicle::VehicleAIOnAttack);
        theAIManager->RegisterFunc("VehicleAIOnMove", &Vehicle::VehicleAIOnMove);
        theAIManager->RegisterFunc("VehicleAIOnDead", &Vehicle::VehicleAIOnDead);
        m_propertiesMap["DriftCoeff"] = 12;
        m_propertiesMap["GadgetAntiMissileRadius"] = 13;
    }

    eGObjPropertySaveStatus Vehicle::GetPropertySaveStatus(int id) const
    {
        // RVA 0x5ECFC0
        auto const it = m_propertiesSaveStatesMap.find(id);
        return it == m_propertiesSaveStatesMap.end() ? PhysicObj::GetPropertySaveStatus(id) : it->second;
    }

    void Vehicle::SetPartByName(CStr const& partName, VehiclePart* vehiclePart, bool bUnsafe)
    {
        // RVA 0x5E6020
        if (vehiclePart)
        {
            M3D_ASSERT(!vehiclePart || partName != CHASSIS || IS_KIND_OF(vehiclePart, Chassis));
            M3D_ASSERT(!vehiclePart || partName != CABIN || IS_KIND_OF(vehiclePart, Cabin));
            M3D_ASSERT(!vehiclePart || partName != BASKET || IS_KIND_OF(vehiclePart, Basket));
        }

        // The outgoing part loses the gadgets' effects and the vehicle's regeneration.
        if (auto* oldPart = GetPartByName(partName))
        {
            for (auto& gadget : m_gadgets)
            {
                gadget.second->ApplyToVp(oldPart, false);
            }

            if (IS_KIND_OF(oldPart, Chassis))
            {
                static_cast<Chassis*>(oldPart)->Health().regeneration().set(0.0f);
            }
            if (IS_KIND_OF(oldPart, CompoundVehiclePart))
            {
                static_cast<CompoundVehiclePart*>(oldPart)->SetDurabilityRegeneration(0.0f);
            }
            else
            {
                oldPart->Durability().regeneration().set(0.0f);
            }
        }

        // A full load restores the new part with the gadgets' effects already applied.
        if (vehiclePart && theObjects->m_SaveType != ObjContainer::SAVE_FULL)
        {
            for (auto& gadget : m_gadgets)
            {
                gadget.second->ApplyToVp(vehiclePart, true);
            }
        }

        ComplexPhysicObj::SetPartByName(partName, vehiclePart, bUnsafe);

        m_bRocketLaunchersPresent = false;
        for (auto& [name, part] : m_vehicleParts)
        {
            VehiclePart* actualPart = part;
            if (IS_KIND_OF(actualPart, CompoundGun))
            {
                actualPart = static_cast<CompoundGun*>(actualPart)->begin()->second.vp;
            }
            // NOTE: a volley launcher is a RocketLauncher too, but it does not count.
            if (IS_KIND_OF(actualPart, RocketLauncher) && !IS_KIND_OF(actualPart, RocketVolleyLauncher))
            {
                m_bRocketLaunchersPresent = true;
                break;
            }
        }

        if (!bUnsafe)
        {
            if (partName == CABIN)
            {
                _OnChangeCabin();
            }
            else if (partName == BASKET)
            {
                _ValidateVehicleParts();
                _UpdateRepositoryOnChangeBasket();
            }
        }

        if (!vehiclePart)
        {
            return;
        }

        if (m_bIsControlledByPlayer)
        {
            M3D_APP->EnqueueMessage(66558, vehiclePart->GetPrototypeId(), 0, 0, 0, {}, {});
        }

        // The incoming part regenerates at the vehicle's rates.
        if (auto const* protoInfo = GetPrototypeInfo())
        {
            if (IS_KIND_OF(vehiclePart, Chassis))
            {
                static_cast<Chassis*>(vehiclePart)->Health().regeneration().set(protoInfo->m_healthRegeneration);
            }
            if (IS_KIND_OF(vehiclePart, CompoundVehiclePart))
            {
                static_cast<CompoundVehiclePart*>(vehiclePart)->SetDurabilityRegeneration(protoInfo->m_durabilityRegeneration);
            }
            else
            {
                vehiclePart->Durability().regeneration().set(protoInfo->m_durabilityRegeneration);
            }
        }
    }

    float Vehicle::GetCurrentSteerAngle() const
    {
        for (auto& wheelInfo : m_wheels)
        {
            if (auto const& wheel = wheelInfo.GetWheel())
            {
                if (wheel->m_steering)
                {
                    return wheel->m_steering * wheel->m_curAngle;
                }
            }
        }
        return 0.0;
    }

    int Vehicle::GetCurrentGear() const
    {
        // RVA 0x5CBC40
        return m_currentGear;
    }

    CVector Vehicle::GetLinearVelocity() const
    {
        if (bIsUpdatingByODE())
        {
            return PhysicObj::GetLinearVelocity();
        }

        return m_ownUpdater->GetLinearVelocity();
    }

    void Vehicle::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x5DF8C0: the base state plus the repository contents and any trailer.
        ComplexPhysicObj::SaveToXML(xmlFile, xmlNode);
        if (m_repository)
        {
            ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Repository");
            m_repository->SaveToXML(xmlFile, node);
            xmlNode->AddChild(node);
        }
        if (auto* trailer = theObjects->GetEntityByObjId(m_trailerObjId))
        {
            ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Trailer");
            trailer->SaveToXML(xmlFile, node);
            xmlNode->AddChild(node);
        }
    }

    void Vehicle::SetCustomControlWeapons(int custom)
    {
        // RVA 0x5D0CE0
        m_customControlWeapons = static_cast<CustomWeaponControlType>(custom);
    }

    void Vehicle::SetCustomControlWeapons(CustomWeaponControlType custom)
    {
        // RVA 0x5CBBB0
        m_customControlWeapons = custom;
    }

    unsigned Vehicle::GetSchwarz() const
    {
        // RVA 0x5E0DB0
        float const price = static_cast<float>(ComplexPhysicObj::GetPrice(nullptr));
        NumericInRange<float> const durability(GetFullDurability(), 0.0f, GetMaxFullDurability());
        return static_cast<unsigned>(GetDurabilityPriceCoeff(durability) * price);
    }

    void Vehicle::UnsubscribeRadioManagerFromNearbyObjId(int objId) const
    {
        // RVA 0x5E7010: drop the radio manager's subscriptions on events 9 and 44.
        if (!thePlayer || thePlayer->GetRadioManagerId() == -1)
        {
            return;
        }
        int const radioManagerId = thePlayer->GetRadioManagerId();
        theProcessManager->PostMessageA(3, GetId(), radioManagerId, 0.0f, m3d::AIParam(9), {}, 1);
        theProcessManager->PostMessageA(3, GetId(), radioManagerId, 0.0f, m3d::AIParam(44), {}, 1);
    }

    void Vehicle::Blow(Obj* partToBlow)
    {
        // RVA 0x5DA510 - besides bursting parts, a vehicle can lose a wheel in an explosion.
        if (!partToBlow)
        {
            return;
        }
        ComplexPhysicObj::Blow(partToBlow);
        if ((GetFlags() & 2) != 0)
        {
            return;
        }

        auto wheelInfo = m_wheels.begin();
        while (wheelInfo != m_wheels.end() && wheelInfo->GetWheel() != partToBlow)
        {
            ++wheelInfo;
        }
        if (wheelInfo == m_wheels.end())
        {
            return;
        }

        Wheel* const wheel = wheelInfo->GetWheel();
        Quaternion const rotation = GetRotation();
        CVector const position = wheel->GetPosition();
        PhysicBody::CreateEffectNode(wheel->GetPrototypeInfo()->m_blowEffectName, position, rotation, true, 1.0f);
        wheel->DetachFromPhysicObj();
        wheel->Remove();
        wheel->m_parentId = -1;
        wheelInfo->SetWheel(nullptr);
    }

    void Vehicle::SetHorn(bool bHorn)
    {
        // RVA 0x5E7510
        if (!m_bHorn && bHorn)
        {
            CStr const& hornSoundName = GetPrototypeInfo()->m_hornSoundName;
            if (hornSoundName.c_str() && strlen(hornSoundName.c_str()) != 0)
            {
                m3d::SgNode* node = PhysicBody::CreateNode(hornSoundName, 0, CVector(1.0f, 1.0f, 1.0f), nullptr, false);
                if (node->IsKindOf(&m3d::SgSoundSourceNode::m_classSgSoundSourceNode))
                {
                    m_hornSoundNode = node;
                    int looped = 1;
                    node->SetProperty(9728, &looped);
                    // NOTE: the chassis is not checked for null.
                    GetChassis()->m_Node->AddChild(m_hornSoundNode);
                }
                else if (node)
                {
                    node->GetGraph()->RemoveNode(node);
                }
            }
        }

        if (m_bHorn && !bHorn)
        {
            thePlayer->CauseEvent(GE_PLAYER_VEHICLE_HORN, 0.0f, m3d::AIParam(), m3d::AIParam());
            if (m_hornSoundNode)
            {
                m_hornSoundNode->GetGraph()->RemoveNode(m_hornSoundNode);
                m_hornSoundNode = nullptr;
            }
        }
        m_bHorn = bHorn;
    }

    int Vehicle::SetExternalPathByName(char const* pathName)
    {
        auto path = pServer->GetExternalPaths()->GetPath(pathName);
        return SetExternalPath(path);
    }

    void Vehicle::IncInSmokeScreenMode()
    {
        // RVA 0x5CCD70
        ++m_inSmokeScreenMode;
    }

    unsigned char Vehicle::GetPriority() const
    {
        // RVA 0x5CC2F0
        return m_priority;
    }

    bool Vehicle::getGodMode() const
    {
        // RVA 0x5CCCC0
        return m_bGodMode;
    }

    bool Vehicle::FireFromWeaponByGunPartName(CStr const& gunPartName, bool enable)
    {
        if (gunPartName.empty())
        {
            return false;
        }

        auto* obj = GetPartByName(gunPartName);
        if (!obj)
        {
            return false;
        }

        if (IS_KIND_OF(obj, CompoundGun))
        {
            auto* gun = RT_DYNCAST(obj, CompoundGun);
            gun->SetProperTargetId(m_seenObjId, m_lockedObjId);
            gun->Fire(enable);
            return true;
        }

        if (IS_KIND_OF(obj, RocketLauncher))
        {
            auto* gun = RT_DYNCAST(obj, RocketLauncher);
            gun->SetTargetId(m_lockedObjId);
            gun->Fire(enable);
            return true;
        }

        if (IS_KIND_OF(obj, Gun))
        {
            auto* gun = RT_DYNCAST(obj, Gun);
            gun->SetTargetId(m_seenObjId);
            if (!enable || gun->isLookAtPoint(m_curLookAt, 0.050000001))
            {
                gun->Fire(enable);
            }
        }
        return true;
    }

    void Vehicle::SetUpdatingByODE(bool byODE)
    {
        // RVA 0x5E3B80
        if (byODE && !bIsUpdatingByODE())
        {
            auto pos = GetPosition();
            pos.y += 0.5;
            SetPosition(pos);

            _EnableIntersections(false);

            CVector newPos;
            auto validPosition = ai::GetValidPosition(
                GetPosition(), GetIntersectionRadius(), GetPrototypeInfo()->m_priority, newPos, true, false, {});
            if (validPosition)
            {
                SetGamePositionOnGround(newPos, true, false);
            }
            else
            {
                M3D_LOG_ERR(
                    "Error: couldn't find valid position for " + GetDebugDescription() + " when enabling physics");
            }
            _EnableIntersections(true);
        }

        // The wheels go in and out of the simulation with the vehicle; when its state changes they are put
        // back on their mount points.
        for (auto& wheelInfo : m_wheels)
        {
            Wheel* wheel = wheelInfo.GetWheel();
            if (!wheel)
            {
                continue;
            }
            wheel->SetUpdatingByODE(byODE);
            if (bIsUpdatingByODE() != byODE)
            {
                wheel->SetPosition(GetPositionAtRelPoint(wheelInfo.m_initialPos));
            }
        }

        PhysicObj::SetUpdatingByODE(byODE);
        auto trailer = dynamic_cast<PhysicObj*>(theObjects->GetEntityByObjId(m_trailerObjId));
        if (trailer)
        {
            trailer->SetUpdatingByODE(byODE);
        }
    }

    void Vehicle::GetOutOfDifficultPlace()
    {
        // RVA 0x5CBB00
        m_bMustGetOutOfDifficultPlace = true;
    }

    float Vehicle::GetMaxHealth() const
    {
        // RVA 0x5D0C00: unchecked, as shipped.
        return GetChassis()->Health().maxValue().get();
    }

    int Vehicle::GetPropertyId(char const* name) const
    {
        auto it = m_propertiesMap.find(name);
        if (it != m_propertiesMap.end())
        {
            return it->second;
        }
        return PhysicObj::GetPropertyId(name);
    }

    float Vehicle::EstimateDamageAI(CVector const& point, retruxx::vector<int, retruxx::allocator<int>> exceptions) const
    {
        // RVA 0x5DFD50 - the damage all guns would deal at the point, ignoring this vehicle.
        exceptions.push_back(GetId());
        float result = 0.0f;
        for (auto const& [name, part] : m_vehicleParts)
        {
            if (part->IsKindOf(RT_CLASS_LOCAL(CompoundGun)))
            {
                result = static_cast<CompoundGun*>(part)->EstimateDamage(point, exceptions) + result;
            }
            else if (part->IsKindOf(RT_CLASS_LOCAL(Gun)))
            {
                result = static_cast<Gun*>(part)->EstimateDamage(point, exceptions) + result;
            }
        }
        return result;
    }

    float Vehicle::EstimateDamageAI() const
    {
        float result = 0.0;
        for (auto const& [name, part] : m_vehicleParts)
        {
            if (IS_KIND_OF(part, CompoundGun))
            {
                auto* compoundGun = RT_DYNCAST(part, CompoundGun);
                result += compoundGun->EstimateDamage();
            }
            else if (IS_KIND_OF(part, Gun))
            {
                auto* gun = RT_DYNCAST(part, Gun);
                result += gun->EstimateDamage();
            }
        }
        return result;
    }

    void Vehicle::PickUpNearbyObjects(
        bool bNeedCollectFromGround,
        unsigned& originalNumItems,
        retruxx::vector<int, retruxx::allocator<int>>& addedObjIds)
    {
        // RVA 0x5EA900 - parts are fitted into empty slots, gadgets into gadget slots, and the rest goes to the cargo.
        originalNumItems = 0;
        addedObjIds = retruxx::vector<int>();
        if (!m_groundRepository)
        {
            return;
        }
        VehiclePrototypeInfo const* const vehicleInfo = GetPrototypeInfo();
        if (!vehicleInfo)
        {
            return;
        }
        if (bNeedCollectFromGround)
        {
            CollectNearbyObjectsToGroundRepository();
        }

        unsigned const numGroundItems = m_groundRepository->GetNumItems();
        originalNumItems = numGroundItems;
        for (unsigned slot = 0; slot < numGroundItems; ++slot)
        {
            GeomRepositoryItem item = m_groundRepository->GetItem(slot);
            Obj* const obj = item.GetObj();
            if (!obj)
            {
                continue;
            }

            if (obj->IsKindOf(RT_CLASS_LOCAL(VehiclePart)))
            {
                if (PrototypeInfo const* const partInfo = obj->GetPrototypeInfo())
                {
                    int const vpResourceId = partInfo->m_resourceId;
                    bool attached = false;
                    for (unsigned i = 0; i < vehicleInfo->GetAllPartNames().size(); ++i)
                    {
                        CStr const& partName = vehicleInfo->GetAllPartNames()[i];
                        if (GetPartByName(partName) || !CanPartBeAttached(partName))
                        {
                            continue;
                        }
                        auto const* const description = vehicleInfo->GetPartDescriptionByName(partName);
                        if (description &&
                            theResourceManager->bResourceIsKindOf(vpResourceId, description->GetPartResourceId()))
                        {
                            m_groundRepository->GiveUpThingFromSlotUnsafe(slot, 1);
                            SetPartByName(partName, static_cast<VehiclePart*>(obj), false);
                            attached = true;
                            break;
                        }
                    }
                    if (attached)
                    {
                        addedObjIds.push_back(obj->GetId());
                        continue;
                    }
                }
            }
            else if (obj->IsKindOf(RT_CLASS_LOCAL(Gadget)) && GetValidSlotIdForGadget(static_cast<Gadget*>(obj)) != -1)
            {
                m_groundRepository->GiveUpThingFromSlotUnsafe(slot, 1);
                if (AddGadget(static_cast<Gadget*>(obj)))
                {
                    addedObjIds.push_back(obj->GetId());
                    continue;
                }
                m_groundRepository->AddThing(item, 0);
            }

            if (!m_repository || !m_repository->CanAddThing(item))
            {
                continue;
            }
            m_groundRepository->GiveUpThingFromSlotUnsafe(slot, 1);
            if (m_repository->AddThing(item, 0))
            {
                addedObjIds.push_back(obj->GetId());
            }
            else
            {
                m_groundRepository->AddThing(item, 0);
            }
        }

        m_groundRepository->SetChanged();
        if (bNeedCollectFromGround)
        {
            m_groundRepository->FlushInReferenceChests(GetPosition());
        }
    }

    float Vehicle::GetFullDurability() const
    {
        float res = 0.0;

        auto* cabin = GetCabin();
        if (cabin)
        {
            res += cabin->Durability().value().get();
        }

        auto* basket = GetBasket();
        if (basket)
        {
            res += basket->Durability().value().get();
        }

        return res;
    }

    NumericInRangeRegenerating<float> const& Vehicle::Health() const
    {
        auto const chassis = RT_DYNCAST(GetPartByName(CHASSIS), Chassis const);
        if (chassis)
        {
            return chassis->Health();
        }

        static NumericInRangeRegenerating<float> dummy{0.0, 0.0, 0.0, 0.0};
        return dummy;
    }

    NumericInRangeRegenerating<float>& Vehicle::Health()
    {
        auto chassis = RT_DYNCAST(GetPartByName(CHASSIS), Chassis);
        if (chassis)
        {
            return chassis->Health();
        }

        static NumericInRangeRegenerating<float> dummy{0.0, 0.0, 0.0, 0.0};
        return dummy;
    }

    bool Vehicle::GetOnOilMode() const
    {
        return m_onOilMode != 0;
    }

    bool Vehicle::bIsMovingAlongExternalPath() const
    {
        return m_bIsMovingAlongExternalPath;
    }

    void Vehicle::DisableGeometry(bool changePhysicState)
    {
        // RVA 0x5DA8A0
        // NOTE: the shipped code passes a hard-coded true, ignoring changePhysicState.
        ComplexPhysicObj::DisableGeometry(true);
        for (auto& wheelInfo : m_wheels)
        {
            if (auto* wheel = wheelInfo.GetWheel())
            {
                wheel->DisableGeometry(true);
            }
        }
    }

    void Vehicle::SetSteer(float radians)
    {
        this->m_steerRadians = radians;
    }

    float Vehicle::GetControl() const
    {
        // RVA 0x5CD5D0
        auto const* cabin = GetCabin();
        return cabin ? cabin->GetControl() : 50.0f;
    }

    bool Vehicle::AddItemsToRepository(char const* prototypeName, int amount)
    {
        // RVA 0x5CCBC0
        if (m_repository && m_repository->AddItems(prototypeName, amount))
        {
            return true;
        }
        if (!m_groundRepository || !m_groundRepository->AddItems(prototypeName, amount))
        {
            return false;
        }
        m_groundRepository->FlushInReferenceChests(GetPosition());
        return true;
    }

    void Vehicle::ResetForcedMaxTorque()
    {
        // RVA 0x5CC450
        m_maxTorqueForced = false;
    }

    void Vehicle::ClearSavedStatus()
    {
        // RVA 0x5CCE00
        ComplexPhysicObj::ClearSavedStatus();
        if (!m_repository)
        {
            return;
        }
        unsigned const numItems = m_repository->GetNumItems();
        for (unsigned i = 0; i < numItems; ++i)
        {
            if (auto* obj = m_repository->GetItem(i).GetObj())
            {
                obj->ClearSavedStatus();
            }
        }
    }

    void Vehicle::HealWheels()
    {
        // RVA 0x5DA700
        for (auto& wheelInfo : m_wheels)
        {
            if (auto* wheel = wheelInfo.GetWheel())
            {
                wheel->HealModel();
            }
        }
    }

    void Vehicle::setImmortalMode(bool bImmortal)
    {
        this->m_bImmortalMode = bImmortal;
    }

    bool Vehicle::bIsBraking() const
    {
        return m_brake > ((GetPrototypeInfo())->m_selfBrakingCoeff + 0.000099999997);
    }

    m3d::AIParam Vehicle::VehicleAIOnMove(Obj*)
    {
        // RVA 0x5E4B30: registered AI hook, returns an empty AIParam in the shipped build.
        return {};
    }

    bool Vehicle::IsHealthZero() const
    {
        // RVA 0x5D3690
        auto const* chassis = GetChassis();
        return !chassis || chassis->Health().value().get() <= 0.001f;
    }

    void Vehicle::FireFromWeaponAI(bool enable, float elapsedTime, Obj* target)
    {
        if (!enable)
        {
            ai::WeaponFirer::AimAndFireFromWeapons(this, 0, elapsedTime, target);
            return;
        }

        auto const curTime = M3D_KERNEL->GetTimer().GetCurTime();
        if (curTime - m_shootTypeChangeTime > m_shootTimeToWait)
        {
            bool const isShooting = m_bIsShooting;
            m_shootTypeChangeTime = curTime;
            m_bIsShooting = !isShooting;
            float timeBetweenBursts = 0.0;
            if (!isShooting)
                timeBetweenBursts = ai::theGlobProp.m_minBurstTime +
                    rand() % (ai::theGlobProp.m_maxBurstTime - ai::theGlobProp.m_minBurstTime);
            else
                timeBetweenBursts = ai::theGlobProp.m_timeBetweenBursts;
            m_shootTimeToWait = timeBetweenBursts;
        }
        if (m_bIsShooting)
            ai::WeaponFirer::AimAndFireFromWeapons(this, 1, elapsedTime, target);
        else
            ai::WeaponFirer::AimAndFireFromWeapons(this, 0, elapsedTime, target);
    }

    bool Vehicle::AddGadget(Gadget* g)
    {
        // RVA 0x5DCAB0
        if (!g)
        {
            return false;
        }
        int const slotId = GetValidSlotIdForGadget(g);
        if (slotId == -1)
        {
            return false;
        }
        g->SetSlotNum(slotId);
        AddChild(g);
        g->ApplyToVehicle(this, true);
        return true;
    }

    void Vehicle::SetCustomControlWeaponsTargetObj(int lookAt)
    {
        // RVA 0x5CBC20
        m_customControlWeaponsTargetObjId = lookAt;
    }

    bool Vehicle::AddObjectToRepository(Obj* pObj)
    {
        // RVA 0x5D1530
        if (!pObj)
        {
            return false;
        }
        GeomRepositoryItem const item(pObj->GetId());
        if (m_repository && m_repository->AddThing(item, 0))
        {
            return true;
        }
        if (!m_groundRepository)
        {
            return false;
        }
        bool const added = m_groundRepository->AddThing(item, 0);
        m_groundRepository->FlushInReferenceChests(GetPosition());
        return added;
    }

    void Vehicle::ResetPositionAndRotation()
    {
        // RVA 0x5DA1F0
        SetPosition(ZeroVector);
        SetRotation(IdentityQuaternion);
        SetLinearVelocity(ZeroVector);
        SetAngularVelocity(ZeroVector);
        for (auto& wheelInfo : m_wheels)
        {
            if (auto* wheel = wheelInfo.GetWheel())
            {
                wheel->SetPosition(wheelInfo.m_initialPos);
                wheel->SetRotation(wheelInfo.m_initialRot);
                wheel->SetLinearVelocity(ZeroVector);
                wheel->SetAngularVelocity(ZeroVector);
            }
        }
    }

    void Vehicle::SetPositionSelf(CVector const& pos)
    {
        // RVA 0x5DFFB0 - moves the vehicle and carries its wheels and trailer along by the same offset.
        CVector const shift = pos - GetPosition();
        ai::PhysicObj::SetPositionSelf(pos);

        for (auto& wheelInfo : m_wheels)
        {
            if (auto* wheel = wheelInfo.GetWheel())
            {
                wheel->SetPosition(wheel->GetPosition() + shift);
            }
        }

        if (m_trailerObjId >= 0)
        {
            if (auto* trailer = static_cast<PhysicObj*>(theObjects->GetEntityByObjId(m_trailerObjId)))
            {
                trailer->SetPosition(trailer->GetPosition() + shift);
            }
        }

        m_bAllowPickUpMessage = true;
        float const* spherePos = dGeomGetPosition(m_takingSphere->GetGeomId());
        m_pastTakingSpherePosition = CVector(spherePos[0], spherePos[1], spherePos[2]);
    }

    void Vehicle::WeaponLookAtPoint(CVector const& lookAt, float elapsedTime)
    {
        // RVA 0x5CBF00
        WeaponFirer::WeaponLookAtPoint(this, lookAt, elapsedTime);
    }

    bool Vehicle::CanPlaceItemsToRepository(char const* prototypeName, int amount)
    {
        // RVA 0x5CCC80
        return m_repository && m_repository->CanPlaceItems(prototypeName, amount);
    }

    void Vehicle::AttachTrailer(char const* trailerPrototypeName)
    {
        // RVA 0x5DCDA0
        CStr const prototypeName(trailerPrototypeName);
        int const trailerId = theObjects->CreateNewObject(
            thePrototypeManager->GetPrototypeId(prototypeName), _GetTrailerName().c_str(), -1, -1);
        auto* const trailer = static_cast<Vehicle*>(theObjects->GetEntityByObjId(trailerId));
        // NOTE: an existing object is attached without a type check, and a missing one is type checked through a null
        // pointer.
        if (trailer || static_cast<m3d::Object*>(trailer)->IsKindOf(RT_CLASS_LOCAL(Vehicle)))
        {
            _AttachExistingTrailer(trailer, true);
            return;
        }
        M3D_LOG_ERR("Error: attaching invalid trailer to " + GetDebugDescription());
    }

    void Vehicle::SetTurboThrottleValue(float value)
    {
        // RVA 0x5CCDE0
        m_turboThrottleValue = value;
    }

    void Vehicle::SetMaxPower(float newMaxPower)
    {
        // RVA 0x5CBCD0
        if (auto* cabin = GetCabin())
        {
            cabin->SetMaxPower(newMaxPower);
        }
    }

    bool Vehicle::GetStoppageMode() const
    {
        // RVA 0x5CCD00
        return m_stoppageMode != 0;
    }

    void Vehicle::GetEnemiesInNeighborhood(float radius, retruxx::vector<int, retruxx::allocator<int>>& enemiesIds) const
    {
        // RVA 0x5DFAE0 - living hostile vehicles and static guns within the radius.
        enemiesIds.resize(0);
        auto const& allObjects = theObjects->m_allObjects;
        for (int id = allObjects.m_firstNodeId; id != -1; id = allObjects.m_records[id].m_nextId)
        {
            Obj* const obj = allObjects.m_records[id].m_value;
            if (!obj->IsKindOf(RT_CLASS_LOCAL(Vehicle)) && !obj->IsKindOf(RT_CLASS_LOCAL(StaticAutoGun)))
            {
                continue;
            }
            auto* const physicObj = static_cast<PhysicObj*>(obj);
            unsigned const flags = physicObj->GetFlags();
            if ((flags & 8) != 0 || (flags & 2) != 0 || physicObj->GetParentRepository() ||
                theRelationship->CheckTolerance(GetBelong(), physicObj->GetBelong()) > RS_ENEMY)
            {
                continue;
            }
            CVector const enemyPos = physicObj->GetPosition();
            CVector const myPos = GetPosition();
            double const dz = myPos.z - enemyPos.z;
            double const dy = myPos.y - enemyPos.y;
            double const dx = myPos.x - enemyPos.x;
            if (radius > sqrt(dz * dz + dy * dy + dx * dx))
            {
                enemiesIds.push_back(physicObj->GetId());
            }
        }
    }

    VehicleRole* Vehicle::GetRole() const
    {
        return RT_DYNCAST(theObjects->GetEntityByObjId(m_roleId), VehicleRole);
    }

    void Vehicle::SubscribeRadioManagerOnNearbyObjId(int objId) const
    {
        // RVA 0x5E6E50
        if (!thePlayer || thePlayer->GetRadioManagerId() == -1)
        {
            return;
        }
        CauseEvent(GE_NOTICE_SOMEONE, 0.0f, m3d::AIParam(objId), {});
        int const radioManagerId = thePlayer->GetRadioManagerId();
        theProcessManager->PostMessageA(2, GetId(), radioManagerId, 0.0f, m3d::AIParam(9), {}, 1);
        theProcessManager->PostMessageA(2, GetId(), radioManagerId, 0.0f, m3d::AIParam(44), {}, 1);
    }

    int Vehicle::GetValidSlotIdForGadget(Gadget const* gadget) const
    {
        // RVA 0x5DC950
        if (!gadget)
        {
            return -1;
        }
        auto const* cabin = GetCabin();
        if (!cabin)
        {
            return -1;
        }
        auto const* cabinProto = cabin->GetPrototypeInfo();
        auto const* gadgetProto = gadget->GetPrototypeInfo();
        if (!cabinProto || !gadgetProto)
        {
            return -1;
        }
        // Find the slot group whose resource kind this gadget matches.
        for (auto const& slot : cabinProto->m_gadgetSlots)
        {
            int const slotResourceId = theResourceManager->GetResourceId(slot.first);
            if (!theResourceManager->bResourceIsKindOf(gadgetProto->m_resourceId, slotResourceId))
            {
                continue;
            }
            int const firstSlot = slot.second.x;
            int const lastSlot = slot.second.y;
            if (gadget->GetSlotNum() != -1)
            {
                // Keep the requested slot if it is free and inside this group.
                int const wanted = gadget->GetSlotNum();
                bool const free = m_gadgets.find(wanted) == m_gadgets.end();
                return (free && wanted >= firstSlot && wanted <= lastSlot) ? wanted : -1;
            }
            for (int candidate = firstSlot; candidate <= lastSlot; ++candidate)
            {
                if (m_gadgets.find(candidate) == m_gadgets.end())
                {
                    return candidate;
                }
            }
            return -1;
        }
        return -1;
    }

    Wheel const* Vehicle::GetFirstExistingWheel() const
    {
        for (auto const& wheelInfo : m_wheels)
        {
            if (auto* wheel = wheelInfo.GetWheel())
            {
                return wheel;
            }
        }
        return nullptr;
    }

    bool Vehicle::TrailerExists() const
    {
        // RVA 0x5E0D60
        return theObjects->GetEntityByObjId(m_trailerObjId) != nullptr;
    }

    float Vehicle::GetAverageEngineRpm() const
    {
        // RVA 0x5CBC70
        return m_averageEngineRpm;
    }

    CVector Vehicle::GetCustomControlWeaponsTarget() const
    {
        // RVA 0x5CBBF0
        return m_customControlWeaponsTarget;
    }

    float Vehicle::GetMaxTorque() const
    {
        if (this->m_maxTorqueForced)
        {
            return this->m_maxTorqueForcedValue;
        }

        auto const cabin = RT_DYNCAST(GetPartByName(CABIN), Cabin const);
        if (cabin)
        {
            return cabin->GetMaxTorque();
        }
        return 0.0;
    }

    void Vehicle::SetAttackStatus(VehicleAttackStatus attackStatus)
    {
        m_attackStatus = attackStatus;
        if (!attackStatus)
        {
            WeaponFirer::FireFromWeaponsIfPossible(this, 0, {0.0, 0.0, 0.0}, 0);
        }
    }

    Vehicle::Vehicle(VehiclePrototypeInfo const& prototypeInfo) :
        ComplexPhysicObj(prototypeInfo),
        m_timeOutForNextIntersectionWithWorld{0, 0, 10, -1}
    {
        this->m_diffRatio = prototypeInfo.m_diffRatio;
        this->m_maxEngineRpm = prototypeInfo.m_maxEngineRpm;
        this->m_lowGearShiftLimit = prototypeInfo.m_lowGearShiftLimit;
        this->m_highGearShiftLimit = prototypeInfo.m_highGearShiftLimit;
        this->m_steeringSpeed = prototypeInfo.m_steeringSpeed;
        this->m_driftCoeff = prototypeInfo.m_driftCoeff;

        this->m_lookBox = ai::Box::CreateObject(nullptr, {1.0, 1.0, 1.0}, nullptr);
        this->m_targetBox = ai::Box::CreateObject(nullptr, {1.0, 1.0, 1.0}, nullptr);

        this->m_priority = prototypeInfo.m_priority;
        this->m_cameraHeight = prototypeInfo.m_cameraHeight;
        this->m_cameraMaxDist = prototypeInfo.m_cameraMaxDist;

        this->m_AI.SetDecisionMatrix(prototypeInfo.m_decisionMatrixNum);
        this->m_bHorn = 0;
        this->m_bGodMode = 0;
        this->m_bImmortalMode = 0;
        this->m_stoppageMode = 0;
        this->m_onOilMode = 0;
        this->m_inSmokeScreenMode = 0;
        this->m_turboThrottleTime = 0.0;
        this->m_turboThrottleValue = 1.0;
        this->m_timeAfterDeath = 0.0;
        this->m_numBlownParts = 0;
        this->m_timeAfterLastBlow = 0.0;
        this->m_shootTypeChangeTime = 0;
        this->m_shootTimeToWait = 0;
        this->m_bIsShooting = 0;
        this->m_antiMissileGadgetSavingRadius = 0.0;
        this->m_bIsTrailer = 0;
        this->m_cruisingSpeed = 0.0;
        this->m_maxSpeedLimited = 0;
        this->m_maxSpeedLimit = 0.0;
        this->m_maxTorqueForced = 0;
        this->m_maxTorqueForcedValue = 0.0;
        this->m_currentGear = 0;
        this->m_throttle = 0.0;
        this->m_brake = 0.0;
        this->m_realThrottle = 0.0;
        this->m_engineRpm = 0.0;
        this->m_averageEngineRpm = 0.0;
        this->m_averageWheelAVel = 0.0;
        this->m_bAutoBrake = 1;
        this->m_bHandBrake = 0;
        this->m_steerRadians = 0.0;
        this->m_turningBackStatus = TURN_BACK_NONE;
        this->m_seenObjId = -1;
        this->m_curLookAt = {0.0, 0.0, 0.0};
        this->m_npcMotionControllerId = -1;

        dGeomDisable(this->m_lookBox->GetGeomId());
        dGeomDisable(this->m_targetBox->GetGeomId());

        this->m_pastTakingSpherePosition = {0.0, 0.0, 0.0};
        this->m_bAllowPickUpMessage = 1;
        this->m_pastNumNearbyChests = 0;
        this->m_currentNumNearbyChests = 0;
        this->m_externalDestination = {0.0, 0.0, 0.0};
        this->m_numOfDrivenWheels = 0;
        this->m_bumperPoint = {0.0, 0.0, 0.0};
        this->m_bIsControlledByPlayer = 0;
        this->m_bIsMovingAlongExternalPath = 0;
        this->m_pathIndex = 0;
        this->m_bCanBeDistractedFromMoving = 0;
        this->m_size = {0.0, 0.0, 0.0};
        this->m_currentDestination = {0.0, 0.0, 0.0};
        this->m_pathNum = -1;
        this->m_pPath = 0;
        this->m_bCustomControl = 0;
        this->m_customControlWeapons = CUSTOM_WEAPON_CONTROL_NONE;
        this->m_customControlWeaponsTarget = {0.0, 0.0, 0.0};
        this->m_customControlWeaponsTargetObjId = -1;
        this->m_indexInTeam = -1;
        this->m_bRocketLaunchersPresent = 0;
        this->m_moveStatus = MOVE_IDLE;
        this->m_attackStatus = ATTACK_IDLE;
        this->m_lastDamage = DAMAGE_BLAST;
        this->m_lastDamagedPart = 0;
        this->m_deathDamage = DAMAGE_BLAST;

        this->m_takingSphere =
            SphereForIntersection::CreateObject(prototypeInfo.m_takingRadius, SphereForIntersection::LOOKING, 0);
        dGeomSetBody(this->m_takingSphere->GetGeomId(), GetBody()->id());

        this->m_repository = dynamic_cast<IzvratRepository*>(M3D_KERNEL->New("IzvratRepository"));
        this->m_repository->Clear(false);
        this->m_repository->SetGeomSize({15, 35});

        this->m_groundRepository = dynamic_cast<GeomRepository*>(M3D_KERNEL->New("GeomRepository"));
        this->m_groundRepository->SetGeomSize(ai::theGlobProp.m_groundRepositorySize);

        this->m_effectActions.resize(2);
        this->m_effectActions[0] = AT_STAND1;
        this->m_effectActions[1] = AT_RESERVED1;

        for (int i = 0; i < 4; ++i)
        {
            m_destroyEffectNames[i] = prototypeInfo.m_destroyEffectNames[i];
        }

        this->m_engineHighSoundNode = 0;
        this->m_engineLowSoundNode = 0;
        this->m_hornSoundNode = 0;
        this->m_trailerJoint = 0;
        this->m_trailerObjId = -1;
        this->m_relTrailerJointPosOnMe = {0.0, 0.0, 0.0};
        this->m_relTrailerJointPosOnTrailer = {0.0, 0.0, 0.0};
        this->m_recollectionId = -1;
        this->m_ownUpdater = 0;
        this->m_roleId = -1;
        this->m_numWheelsTouchingGround = 0;
        this->m_bHidden = 0;
        this->m_lockedObjId = -1;
        this->m_toBeLockedObjId = -1;
        this->m_bMustGetOutOfDifficultPlace = 0;
        this->m_bWasStuck = 0;
        this->m_timeToLockTarget = 0.0;
        this->m_prevPosToCheckStuck = {0.0, 0.0, 0.0};
        this->m_timeOutToCheckStuck = 0.0;
        this->m_curSteeringForce = {0.0, 0.0, 0.0};
        this->m_soundRechargeChannelId = -1;
        this->m_bCurSteeringForceValid = 0;
    }

    void Vehicle::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x5E5470
        ComplexPhysicObj::LoadRuntimeValues(xmlFile, xmlNode);
        m3d::SafeFloatAttrib(m_timeAfterDeath, xmlNode, "TimeAfterDeath");
        int numBlownParts = -1;
        if (m3d::SafeIntAttrib(numBlownParts, xmlNode, "NumBlownParts") && numBlownParts >= 0)
        {
            m_numBlownParts = numBlownParts;
        }
        m3d::SafeFloatAttrib(m_timeAfterLastBlow, xmlNode, "TimeAfterLastBlow");
        int deathDamage = m_deathDamage;
        m3d::SafeIntAttrib(deathDamage, xmlNode, "DeathDamage");
        m_deathDamage = static_cast<DamageType>(deathDamage);
        m3d::SafeIntAttrib(m_currentGear, xmlNode, "CurrentGear");
        m3d::SafeFloatAttrib(m_throttle, xmlNode, "Throttle");
        m3d::SafeFloatAttrib(m_realThrottle, xmlNode, "RealThrottle");
        m3d::SafeBoolAttrib(m_bAutoBrake, xmlNode, "AutoBrake");
        m3d::SafeBoolAttrib(m_bHandBrake, xmlNode, "HandBrake");
        m3d::SafeFloatAttrib(m_engineRpm, xmlNode, "EngineRPM");
        m3d::SafeIntAttrib(m_npcMotionControllerId, xmlNode, "NpcMotionControllerId");
        m3d::SafeVectorAttrib(m_currentDestination, xmlNode, "CurrentDestination");
        m3d::SafeIntAttrib(m_pathNum, xmlNode, "PathNum");
        int priority = m_priority;
        m3d::SafeIntAttrib(priority, xmlNode, "Priority");
        m_priority = static_cast<unsigned char>(priority);
        m3d::SafeIntAttrib(m_pathIndex, xmlNode, "PathIndex");
        m3d::SafeBoolAttrib(m_bIsMovingAlongExternalPath, xmlNode, "IsMovingAlongExternalPath");
        m3d::SafeBoolAttrib(m_bCanBeDistractedFromMoving, xmlNode, "CanBeDistractedFromMoving");
        m3d::SafeIntAttrib(m_stoppageMode, xmlNode, "StoppageMode");
        m3d::SafeIntAttrib(m_onOilMode, xmlNode, "OnOilMode");
        m3d::SafeIntAttrib(m_inSmokeScreenMode, xmlNode, "InSmokeScreenMode");
        m3d::SafeFloatAttrib(m_turboThrottleTime, xmlNode, "TurboThrottleTime");
        m3d::SafeFloatAttrib(m_turboThrottleValue, xmlNode, "TurboThrottleValue");
        m3d::SafeBoolAttrib(m_bImmortalMode, xmlNode, "ImmortalMode");
        m3d::SafeBoolAttrib(m_bHidden, xmlNode, "Hidden");
        int moveStatus = m_moveStatus;
        m3d::SafeIntAttrib(moveStatus, xmlNode, "MoveStatus");
        m_moveStatus = static_cast<VehicleMoveStatus>(moveStatus);
        m3d::SafeFloatAttrib(m_cruisingSpeed, xmlNode, "CruisingSpeed");
        m3d::SafeVectorAttrib(m_pastTakingSpherePosition, xmlNode, "PastTakingPos");
        m3d::SafeBoolAttrib(m_bAllowPickUpMessage, xmlNode, "AllowInventoryMessage");
        m3d::SafeIntAttrib(m_roleId, xmlNode, "Role");
        m3d::SafeIntAttrib(m_recollectionId, xmlNode, "RecollectionId");
        m3d::SafeBoolAttrib(m_bWasStuck, xmlNode, "WasStuck");
        m3d::SafeVectorAttrib(m_prevPosToCheckStuck, xmlNode, "PrevPosToCheckStuck");
        m3d::SafeFloatAttrib(m_timeOutToCheckStuck, xmlNode, "TimeOutToCheckStuck");

        ref_ptr pathNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        xmlNode->GetFirstChild(pathNode, "Path");
        if (!pathNode->IsEmpty())
        {
            delete m_pPath;
            m_pPath = nullptr;
            m_pPath = new Path();
            m_pPath->LoadFromXML(xmlFile, pathNode, Map::theGlobalMap);
        }

        ref_ptr gunsNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        xmlNode->GetFirstChild(gunsNode, "GunPointed");
        // NOTE: the saved guns are only read when the map already has entries, not when the node exists.
        if (!m_gunsPointed.empty())
        {
            ref_ptr gunNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
            for (gunsNode->GetFirstChild(gunNode, "Gun"); !gunNode->IsEmpty(); gunNode->GetNextSibling(gunNode, "Gun"))
            {
                int gunId = -1;
                m3d::SafeIntAttrib(gunId, gunNode, "Id");
                bool pointed = false;
                m3d::SafeBoolAttrib(pointed, gunNode, "Pointed");
                if (gunId != -1)
                {
                    m_gunsPointed.insert({gunId, pointed});
                }
            }
        }

        bool headlights = false;
        m3d::SafeBoolAttrib(headlights, xmlNode, "Headlights");
        m_effectActions[1] = static_cast<ActionType>(headlights + 12);

        m_wheels.clear();
        ref_ptr wheelsNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        xmlNode->GetFirstChild(wheelsNode, "Wheels");
        if (!wheelsNode->IsEmpty())
        {
            m_wheels.resize(GetPrototypeInfo()->m_wheelInfos.size(), WheelRuntimeInfo(nullptr));
            // NOTE: a WheelInfo without an Id reuses the previous one, and the index is not range checked.
            int wheelId = 0;
            ref_ptr wheelInfoNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
            for (wheelsNode->GetFirstChild(wheelInfoNode, "WheelInfo"); !wheelInfoNode->IsEmpty();
                 wheelInfoNode->GetNextSibling(wheelInfoNode, "WheelInfo"))
            {
                m3d::SafeIntAttrib(wheelId, wheelInfoNode, "Id");
                bool present = true;
                m3d::SafeBoolAttrib(present, wheelInfoNode, "present");
                if (present)
                {
                    ref_ptr wheelNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
                    wheelInfoNode->GetFirstChild(wheelNode, "Wheel");
                    int const wheelObjId = gDynamicScene->ReadNewObjectFromXml(xmlFile, wheelNode, {});
                    m_wheels[wheelId].SetWheel(static_cast<Wheel*>(theObjects->GetEntityByObjId(wheelObjId)));
                }
            }
        }
    }

    void Vehicle::DecInSmokeScreenMode()
    {
        // RVA 0x5CCD80
        if (--m_inSmokeScreenMode < 0)
        {
            m_inSmokeScreenMode = 0;
        }
    }

    void Vehicle::SetTrailer()
    {
        // RVA 0x5CCB30
        m_bIsTrailer = true;
    }

    void Vehicle::RenderDebugInfo() const
    {
        ComplexPhysicObj::RenderDebugInfo();
        if (!IsAlive())
        {
            return;
        }

        ai::pServer->GetWorld()->GetLandscape().DrawGeom(m_lookBox->GetGeomId());
        ai::pServer->GetWorld()->GetLandscape().DrawGeom(m_targetBox->GetGeomId());

        // Get current position
        CVector pos = GetPosition();
        pos.y += 10.0f;  // Offset slightly above ground

        // Draw path if it exists and has points
        if (m_pPath && m_pPath->GetSize() > 0)
        {
            CVector previousPoint = pos;
            CVector currentPoint;

            // Draw all path points
            unsigned int pathSize = m_pPath->GetSize();
            unsigned int pathColor = 0xFFFF0000;  // Start with red

            for (unsigned int i = 0; i < pathSize; ++i)
            {
                ai::GetPathItem(m_pPath, i, currentPoint);

                // Adjust point height slightly above terrain
                currentPoint.y = M3D_ENGINE_CFG.GetHeight(currentPoint.x, currentPoint.z) + 10.0f;

                // Draw cross at path point
                M3D_APP->DrawCross(currentPoint, 2.0f, pathColor);

                // Draw line from previous point (except for points before current position)
                if (i >= m_pathNum)
                {
                    M3D_APP->DrawLine(previousPoint, currentPoint, 0xFF00FFFF);
                    previousPoint = currentPoint;
                }

                // Cycle through colors
                pathColor += 0x505;
            }

            // Draw navigation debug info if we have a valid current path point
            if (m_pathNum >= 0 && m_pathNum < static_cast<int>(pathSize))
            {
                CVector pathPoint;
                ai::GetPathItem(m_pPath, m_pathNum, pathPoint);

                CVector nextPathPoint = _GetNextPathPoint();

                // Calculate driving values
                ai::DrivingValues dv;
                ai::CalcDrivingValues(
                    *this, pathPoint, nextPathPoint, (m_pathNum == static_cast<int>(pathSize) - 1), dv);

                // Draw check line and circle
                dv.checkLine.RenderDebugInfo(0xFF00FF00);
                ai::DebugCircle(pathPoint, dv.checkCircleRadius, 0xFF00FF00);

                // Draw braking circle
                ai::DebugCircle(pathPoint, dv.brakingCircleRadius, 0xFF00FFFF);
            }
        }
        else if (m_moveStatus == 2)
        {
            // Get formation-based destination
            ai::Team* team = reinterpret_cast<ai::Team*>(GetParent());
            ai::Formation* formation = team->GetFormation();

            ai::DrivingValues dv;
            CVector formationDirection = formation->GetDirection();

            // Calculate target point
            CVector targetPoint = m_externalDestination + formationDirection;

            // Calculate driving values
            ai::CalcDrivingValues(*this, m_externalDestination, targetPoint, true, dv);

            // Draw check line and circle
            dv.checkLine.RenderDebugInfo(0xFF00FF00);
            ai::DebugCircle(m_externalDestination, dv.checkCircleRadius, 0xFF00FF00);

            // Draw braking circle
            ai::DebugCircle(m_externalDestination, dv.brakingCircleRadius, 0xFF00FFFF);
        }

        // Draw steering force if not player-controlled or on external path
        if (!m_bIsControlledByPlayer || m_bIsMovingAlongExternalPath)
        {
            // Calculate intersection with world
            IntersectWithWorld();

            // Calculate and draw steering force
            CVector steeringForce = _CalcSteeringForce(0.0f);

            CVector forceEndPoint;
            forceEndPoint.x = pos.x + (steeringForce.x * 100.0f);
            forceEndPoint.y = pos.y + (steeringForce.y * 100.0f);
            forceEndPoint.z = pos.z + (steeringForce.z * 100.0f);

            M3D_APP->DrawLine(pos, forceEndPoint, 0xFFFF0000);
        }

        // Draw player info cone if player-controlled
        if (m_bIsControlledByPlayer)
        {
            ai::Player* player = reinterpret_cast<ai::Player*>(GetParent());
            ai::InfoCone const& infoCone = player->GetInfoCone();
            infoCone.RenderDebugInfo();
        }

        // Draw vehicle name
        CStr name(GetName());
        if (name.empty())
        {
            name = "No name";
        }

        // Calculate text position (above vehicle)
        CVector textPos = GetPosition();
        textPos.y += m_size.y;

        // Draw debug text
        ai::DebugText(textPos, 11.0f, 0.0f, reinterpret_cast<uint32_t>(this) | 0xFF000000, name);
    }

    void Vehicle::SetVisible()
    {
        // RVA 0x5DD1A0
        ComplexPhysicObj::SetVisible();
        for (auto& wheelInfo : m_wheels)
        {
            if (auto* wheel = wheelInfo.GetWheel())
            {
                wheel->SetVisible();
            }
        }
        EnableGeometry(true);
        if (auto* trailer = theObjects->GetEntityByObjId(m_trailerObjId))
        {
            trailer->SetVisible();
        }
        bool enabled = true;
        if (m_engineHighSoundNode)
        {
            m_engineHighSoundNode->SetProperty(9733u, &enabled);
        }
        if (m_engineLowSoundNode)
        {
            m_engineLowSoundNode->SetProperty(9733u, &enabled);
        }
    }

    int Vehicle::GetNpcMotionControllerId() const
    {
        // RVA 0x5CCEC0
        return m_npcMotionControllerId;
    }

    void Vehicle::SetIndexInTeam(int indexInTeam)
    {
        m_indexInTeam = indexInTeam;
    }

    NumericInRangeRegenerating<float>& Vehicle::Fuel()
    {
        auto chassis = RT_DYNCAST(GetPartByName(CHASSIS), Chassis);
        if (chassis)
        {
            return chassis->Fuel();
        }

        static NumericInRangeRegenerating<float> dummy{0.0, 0.0, 0.0, 0.0};
        return dummy;
    }

    NumericInRangeRegenerating<float> const& Vehicle::Fuel() const
    {
        auto const chassis = RT_DYNCAST(GetPartByName(CHASSIS), Chassis const);
        if (chassis)
        {
            return chassis->Fuel();
        }

        static NumericInRangeRegenerating<float> dummy{0.0, 0.0, 0.0, 0.0};
        return dummy;
    }

    float Vehicle::GetFuel() const
    {
        // RVA 0x5D0C40: unchecked, as shipped.
        return GetChassis()->Fuel().value().get();
    }

    int Vehicle::GetSeenObjId() const
    {
        return m_seenObjId;
    }

    void Vehicle::GetGeoms(retruxx::vector<Geom*, retruxx::allocator<Geom*>>& geoms) const
    {
        // RVA 0x5D5C90
        ComplexPhysicObj::GetGeoms(geoms);
        for (auto const& wheelInfo : m_wheels)
        {
            if (Wheel const* const wheel = wheelInfo.GetWheel())
            {
                wheel->GetPhysicBody()->GetGeoms(geoms);
            }
        }
    }

    void Vehicle::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        ComplexPhysicObj::LoadFromXML(xmlFile, xmlNode);
        _UpdateRepositoryOnChangeBasket();

        ref_ptr node = xmlFile->CreateNode();

        xmlNode->GetFirstChild(node, "Repository");
        if (!node->IsEmpty())
        {
            if (m_repository)
            {
                m_repository->LoadFromXML(xmlFile, node);
                RefreshMass();
            }
        }

        xmlNode->GetFirstChild(node, "Trailer");
        if (!node->IsEmpty())
        {
            auto objId = gDynamicScene->ReadNewObjectFromXml(xmlFile, node, {});
            auto obj = theObjects->GetEntityByObjId(objId);
            if (IS_KIND_OF(obj, Vehicle))
            {
                _AttachExistingTrailer(RT_DYNCAST(obj, Vehicle), false);
            }
            else
            {
                M3D_LOG_ERR("Error: loading trailer which is not a vehicle: " + obj->GetDebugDescription());
            }
        }
    }

    bool Vehicle::bIsControlledByPlayer() const
    {
        return m_bIsControlledByPlayer;
    }

    m3d::AIParam Vehicle::TakeOffAllGuns()
    {
        // RVA 0x5E7390
        // Collect first, then detach - SetPartByName mutates m_vehicleParts.
        retruxx::vector<int> takenOff;
        retruxx::vector<CStr> partNames;
        for (auto const& entry : m_vehicleParts)
        {
            if (entry.second->IsKindOf(RT_CLASS_LOCAL(Gun)) || entry.second->IsKindOf(RT_CLASS_LOCAL(CompoundGun)))
            {
                takenOff.push_back(entry.second->GetId());
                partNames.push_back(entry.first);
            }
        }
        for (auto const& partName : partNames)
        {
            SetPartByName(partName, nullptr, false);
        }
        return m3d::AIParam(takenOff);
    }

    void Vehicle::SetThrottle(float throttle, bool autoBrake)
    {
        if (fabs(throttle) <= 1.1)
        {
            m_throttle = throttle;
            m_brake = 0.0;
            m_bAutoBrake = autoBrake;
            if (fabs(throttle) > 0.001)
            {
                m_bHandBrake = false;
            }
        }
        else
        {
            M3D_LOG_ERR("Error: throttle too large for " + GetDebugDescription());
        }
    }

    float Vehicle::GetBrake() const
    {
        // RVA 0x5CC280
        return m_brake;
    }

    float Vehicle::GetMaxSpeed() const
    {
        auto cabin = GetCabin();
        float maxSpeed = cabin ? cabin->GetMaxSpeed() : 0.0;

        if (m_maxSpeedLimited)
        {
            if (m_maxSpeedLimit > maxSpeed)
            {
                return maxSpeed;
            }
            return m_maxSpeedLimit;
        }

        if (m_bIsControlledByPlayer)
        {
            auto* part = GetPartByName(CHASSIS);
            auto* chassis = dynamic_cast<Chassis const*>(part);
            if (chassis && chassis->Fuel().value().get() == chassis->Fuel().minValue().get())
            {
                return maxSpeed > theGlobProp.m_maxSpeedWithNoFuel ? theGlobProp.m_maxSpeedWithNoFuel : maxSpeed;
            }
        }
        return maxSpeed;
    }

    bool Vehicle::CanChildBeAdded(m3d::Class* pClass) const
    {
        // RVA 0x5CB8F0
        if (Obj::CanChildBeAdded(pClass))
        {
            return true;
        }
        return pClass->IsKindOf(RT_CLASS_LOCAL(Bullet)) || pClass->IsKindOf(RT_CLASS_LOCAL(Gadget));
    }

    float Vehicle::GetTurboThrottleTime() const
    {
        return m_turboThrottleTime;
    }

    float Vehicle::GetFullDurabilityCoeffForDamageType(DamageType damageType) const
    {
        float res = 0.0;

        auto* cabin = GetCabin();
        if (cabin)
        {
            res += cabin->GetDurabilityCoeffForDamageType(damageType);
        }

        auto* basket = GetBasket();
        if (basket)
        {
            res += basket->GetDurabilityCoeffForDamageType(damageType);
        }

        return res;
    }

    Obj* Vehicle::CloneObj()
    {
        // RVA 0x5DA730: re-create the visual parts the base clone did not.
        auto* clone = RT_DYNCAST(ComplexPhysicObj::CloneObj(), Vehicle);
        if (!clone)
        {
            return nullptr;
        }
        if (clone->m_repository)
        {
            unsigned const numItems = clone->m_repository->GetNumItems();
            for (unsigned i = 0; i < numItems; ++i)
            {
                if (auto* obj = clone->m_repository->GetItem(i).GetObj())
                {
                    obj->PostLoad();
                    obj->CreateVisualPart();
                }
            }
        }
        for (auto& wheelInfo : clone->m_wheels)
        {
            if (auto* wheel = wheelInfo.GetWheel())
            {
                wheel->PostLoad();
                wheel->CreateVisualPart();
            }
        }
        return clone;
    }

    int Vehicle::CheckSkin(int skinNum)
    {
        // RVA 0x5E1870 - an NPC falls back to its first loaded skin when skinNum isn't loaded; the player's
        // vehicle loads skinNum into every part's model instead.
        if (!m_bIsControlledByPlayer)
        {
            auto* cabin = GetPartByName(CABIN);
            if (cabin && cabin->IsKindOf(RT_CLASS_LOCAL(Cabin)))
            {
                auto* model = cabin->GetModel();
                if (model && !model->GetLoadedSkins().loadAllSkins)
                {
                    auto const& loadSkins = model->GetLoadedSkins().loadSkins;
                    if (loadSkins.find(skinNum) == loadSkins.end() && !loadSkins.empty())
                    {
                        return *loadSkins.begin();
                    }
                }
            }
            return skinNum;
        }

        for (auto& part : m_vehicleParts)
        {
            if (part.second->m_Node)
            {
                auto server = part.second->m_Node->GetServer();

                m3d::AnimatedModel* model = 0;
                server->GetItemProperty(part.second->m_Node->GetServerHandle(), 16394, &model);
                if (model)
                {
                    model->LoadSkin(skinNum);
                }
            }
        }

        return skinNum;
    }

    m3d::AIParam Vehicle::VehicleAIOnDefend(Obj*)
    {
        // RVA 0x5E4B00: registered AI hook, returns an empty AIParam in the shipped build.
        return {};
    }

    float Vehicle::GetEngineRpm() const
    {
        // RVA 0x5CBC50
        return m_engineRpm;
    }

    void Vehicle::SetBelong(int newBelong)
    {
        ComplexPhysicObj::SetBelong(newBelong);
        for (auto& wheelInfo : m_wheels)
        {
            if (auto* wheel = wheelInfo.GetWheel())
            {
                wheel->SetBelong(newBelong);
            }
        }

        if (m_trailerObjId >= 0)
        {
            auto* obj = theObjects->GetEntityByObjId(m_trailerObjId);
            if (obj)
            {
                obj->SetBelong(newBelong);
            }
        }
    }

    Team* Vehicle::GetTeam() const
    {
        // RVA 0x5CC230: the shipped code casts the parent without a type check.
        return reinterpret_cast<Team*>(GetParent());
    }

    void Vehicle::UnsubscribeRadioManagerFromAllNearbyObjIds() const
    {
        for (auto& obstacle : m_currentNearbyObstacles)
        {
            auto* obj = obstacle->GetOwnerPhysicObj();
            if (obj && obj->IsKindOf(&ai::Vehicle::m_classVehicle))
            {
                dynamic_cast<Vehicle*>(obj)->UnsubscribeRadioManagerFromNearbyObjId(GetId());
            }
        }
    }

    void Vehicle::SetBasket(VehiclePart* newBasket)
    {
        // RVA 0x5CBAE0
        SetPartByName(BASKET, newBasket, false);
    }

    Cabin const* Vehicle::GetCabin() const
    {
        return RT_DYNCAST(GetPartByName(CABIN), Cabin const);
    }

    Cabin* Vehicle::GetCabin()
    {
        return RT_DYNCAST(GetPartByName(CABIN), Cabin);
    }

    float Vehicle::GetTurboThrottleValue() const
    {
        // RVA 0x5CCDD0
        return m_turboThrottleValue;
    }

    int Vehicle::GetMaxGadgets(CStr const& gadgetResourceName) const
    {
        // RVA 0x5CBDB0
        auto const* cabin = GetCabin();
        if (!cabin)
        {
            return 0;
        }
        auto const* protoInfo = cabin->GetPrototypeInfo();
        return protoInfo ? protoInfo->GetMaxGadgets(gadgetResourceName) : 0;
    }

    float Vehicle::GetMaxPower() const
    {
        // RVA 0x5CBC80
        auto const* cabin = GetCabin();
        return cabin ? cabin->GetMaxPower() : 0.0f;
    }

    void Vehicle::DecStoppageMode()
    {
        // RVA 0x5CCD20
        --m_stoppageMode;
    }

    void Vehicle::ShowVehicle(bool bShow)
    {
        // RVA 0x5CCE70
        if (bShow != m_bHidden)
        {
            return;
        }
        if (m_bHidden)
        {
            SetVisible();
        }
        else
        {
            SetInvisible();
        }
        m_bHidden = !bShow;
    }

    void Vehicle::SetRole(VehicleRole* role)
    {
        if (auto* currRole = RT_DYNCAST(theObjects->GetEntityByObjId(m_roleId), VehicleRole))
        {
            currRole->Remove();
        }
        if (role)
        {
            m_roleId = role->GetId();
        }
        else
        {
            m_roleId = -1;
        }
    }

    retruxx::set<ref_ptr<Obstacle>, retruxx::less<ref_ptr<Obstacle>>, retruxx::allocator<ref_ptr<Obstacle>>> const&
        Vehicle::GetNearbyObstacles() const
    {
        // RVA 0x5CBB70
        return m_currentNearbyObstacles;
    }

    CStr Vehicle::GetPropertyName(int id) const
    {
        // RVA 0x5ED460
        for (auto const& entry : m_propertiesMap)
        {
            if (entry.second == id)
            {
                return entry.first;
            }
        }
        return PhysicObj::GetPropertyName(id);
    }

    bool Vehicle::RemoveItemsFromRepository(char const* prototypeName, int amount)
    {
        // RVA 0x5CCC40
        return m_repository && m_repository->RemoveItems(prototypeName, amount);
    }

    void Vehicle::SetCustomControlWeaponsTarget(CVector const& lookAt)
    {
        // RVA 0x5CBBD0
        m_customControlWeaponsTarget = lookAt;
    }

    float Vehicle::GetTimeToLockTarget() const
    {
        // RVA 0x5CCF10
        return m_timeToLockTarget;
    }

    void Vehicle::SetGamePositionOnGround(CVector const& pos, bool bWithCollisions, bool bWithWater)
    {
        // RVA 0x5E09D0
        CVector normal = {0.0, 1.0, 0.0};  // Default up vector
        CVector hoverOffset = {0.0, 0.0, 0.0};

        // Step 1: Get the ground position at the target location
        // This considers terrain height and optionally collisions with other objects
        CVector groundPos = ai::GetGroundPos(pos, bWithCollisions, true);

        bool isOnWater = false;

        // Step 2: Handle water collisions if enabled
        if (bWithWater)
        {
            // Convert world coordinates to landscape coordinates (scale factor 0.03125 = 1/32)
            float landscapeX = pos.x * 0.03125f;
            float landscapeZ = pos.z * 0.03125f;

            float waterHeight = m3d::pClient->GetWorld().GetLandscape().getWaterHeight(landscapeX, landscapeZ);

            // If water is higher than ground, use water level
            if (waterHeight > groundPos.y)
            {
                groundPos.y = waterHeight;
                isOnWater = true;
            }
        }

        // Step 3: Get current vehicle rotation for reference
        Quaternion currentRotation = GetRotation();

        // Step 4: the hover height comes from the first wheel slot that holds a wheel: its radius
        // below the wheel's mount point, plus 0.1. Without wheels, half the vehicle's height.
        float hoverHeight;
        auto wheelInfo = m_wheels.begin();
        while (wheelInfo != m_wheels.end() && !wheelInfo->GetWheel())
        {
            ++wheelInfo;
        }
        if (wheelInfo != m_wheels.end())
        {
            // NOTE: the geometry is assumed to be a sphere without a type check.
            auto* wheelSphere = static_cast<ai::Sphere*>(wheelInfo->GetWheel()->GetPhysicBody()->m_pGeoms[0]->GetGeom());
            hoverHeight = static_cast<float>(double(wheelSphere->GetRadius()) - wheelInfo->m_initialPos.y + double(0.1f));
        }
        else
        {
            hoverHeight = m_size.y * 0.5f;
        }

        hoverOffset.y = hoverHeight;

        // Step 5: Get terrain surface normal (unless on water)
        if (!isOnWater)
        {
            // The landscape reports its normal with "up" in z; swap it into the world's y-up frame.
            // NOTE: getNormal's out-of-range fallback (0, 1, 0) is already y-up, so after the swap
            // it lies flat, (0, 0, 1), as in the shipped code.
            CVector const terrainNormal = ai::pServer->GetWorld()->GetLandscape().getNormal(groundPos.x, groundPos.z);
            normal = CVector(terrainNormal.x, terrainNormal.z, terrainNormal.y);
        }

        // Step 6: Calculate vehicle orientation based on ground surface
        CVector vehicleForwardDir = GetDirection();

        // Project the forward direction onto the ground plane defined by the surface normal
        CVector projectedForwardDir = ai::ProjectVectorOntoPlane(normal, vehicleForwardDir);

        // Only update orientation if the projected direction is significant
        float projectedDirLengthSq =
            ((projectedForwardDir.x * projectedForwardDir.x) + (projectedForwardDir.z * projectedForwardDir.z)) +
            (projectedForwardDir.y * projectedForwardDir.y);

        if (projectedDirLengthSq > 0.001f)
        {
            // Normalize the projected direction vector
            float invLength = 1.0f / sqrt(projectedDirLengthSq + 1.1920929e-7f);

            CVector normalizedForwardDir;
            normalizedForwardDir.x = projectedForwardDir.x * invLength;
            normalizedForwardDir.y = projectedForwardDir.y * invLength;
            normalizedForwardDir.z = projectedForwardDir.z * invLength;

            // Set the vehicle's orientation to align with the ground surface
            this->SetDirections(normalizedForwardDir, normal);
        }

        // Step 7: Calculate final position and set it
        CVector finalPosition;
        finalPosition.x = groundPos.x + hoverOffset.x;
        finalPosition.y = groundPos.y + hoverOffset.y;
        finalPosition.z = groundPos.z + hoverOffset.z;
        this->SetPosition(finalPosition);
    }

    void Vehicle::SetMaxTorque(float newMaxTorque)
    {
        // RVA 0x5CBD70
        if (auto* cabin = GetCabin())
        {
            cabin->SetMaxTorque(newMaxTorque);
        }
    }

    void Vehicle::LimitMaxSpeed(float maxSpeedLimit)
    {
        // RVA 0x5CC400
        m_maxSpeedLimited = true;
        m_maxSpeedLimit = maxSpeedLimit;
    }

    m3d::Class* Vehicle::GetClass() const
    {
        return RT_CLASS_LOCAL(Vehicle);
    }

    CVector Vehicle::GetSize() const
    {
        return m_size;
    }

    void Vehicle::CollectNearbyObjectsToGroundRepository()
    {
        // RVA 0x5EA7A0: rebuild the ground repository from every chest inside the taking sphere.
        if (!m_groundRepository)
        {
            return;
        }
        m_groundRepository->Clear(false);
        retruxx::set<m3d::Class*> affectedClasses;
        affectedClasses.insert(RT_CLASS_LOCAL(Chest));
        retruxx::set<ref_ptr<Obstacle>> found;
        IntersectionManager::GetIntersectedObjects(found, m_takingSphere, affectedClasses, false, false);
        for (auto const& obstacle : found)
        {
            m_groundRepository->AppendChest(static_cast<Chest*>(obstacle->GetOwner()));
        }
    }

    Vehicle::VehicleAttackStatus Vehicle::GetAttackStatus() const
    {
        return m_attackStatus;
    }

    void Vehicle::ReleaseAllPedals()
    {
        SetThrottle(0.0, 0);
        m_brake = GetPrototypeInfo()->m_selfBrakingCoeff;
    }

    bool Vehicle::GetInSmokeScreenMode() const
    {
        return m_inSmokeScreenMode != 0;
    }

    void Vehicle::PlaySoundOnRechargeWeapon()
    {
        if (m_bIsControlledByPlayer)
        {
            if (M3D_ENGINE_CFG.m_snd_Enable.GetB() &&
                (m_soundRechargeChannelId == -1 || !M3D_APP->m_sound->IsChannelPlaying(m_soundRechargeChannelId)))
            {
                m_soundRechargeChannelId = -1;
                auto& serverSound = M3D_APP->GetSoundServer();

                auto itemByName = serverSound.GetItemByName("S_RECHARGE_WEAPON", 1);
                if (itemByName != -1)
                {
                    int soundId = -1;
                    serverSound.GetItemProperty(itemByName, m3d::PROP_SRV_SND_ID, &soundId);
                    if (soundId != -1)
                    {
                        m_soundRechargeChannelId = M3D_APP->m_sound->PlaySound2D(soundId, 0);
                    }
                }
            }
        }
    }

    Vehicle* Vehicle::GetTrailer() const
    {
        // RVA 0x5DCF50
        // NOTE: the shipped code casts without a type check.
        return static_cast<Vehicle*>(theObjects->GetEntityByObjId(m_trailerObjId));
    }

    void Vehicle::IncOnOilMode()
    {
        // RVA 0x5CCD40
        ++m_onOilMode;
    }

    CVector Vehicle::GetGeometricCenter() const
    {
        // RVA 0x5D0F10 - 40% of the vehicle's height up from its position, along its own up axis.
        CMatrix rotation;
        rotation.rotTranslate(GetRotation(), ZeroVector);

        // The up axis (0, 1, 0) in world space: the matrix's second row (row-vector convention).
        CVector const up(rotation._21, rotation._22, rotation._23);
        return GetPosition() + up * m_size.y * 0.40000001f;
    }

    float Vehicle::EstimateDamageFromPositionAI(
        CVector const& position,
        CVector const& point,
        retruxx::vector<int, retruxx::allocator<int>> exceptions) const
    {
        // RVA 0x5DFE80 - as EstimateDamageAI, but as if the vehicle stood at position.
        exceptions.push_back(GetId());
        float result = 0.0f;
        for (auto const& [name, part] : m_vehicleParts)
        {
            if (part->IsKindOf(RT_CLASS_LOCAL(CompoundGun)))
            {
                result = static_cast<CompoundGun*>(part)->EstimateDamageFromPosition(position, point, exceptions) + result;
            }
            else if (part->IsKindOf(RT_CLASS_LOCAL(Gun)))
            {
                result = static_cast<Gun*>(part)->EstimateDamageFromPosition(position, point, exceptions) + result;
            }
        }
        return result;
    }

    void Vehicle::SetNpcMotionControllerId(int npcMotionControllerId)
    {
        // RVA 0x5CCED0
        m_npcMotionControllerId = npcMotionControllerId;
    }

    VehiclePrototypeInfo const* Vehicle::GetPrototypeInfo() const
    {
        return dynamic_cast<VehiclePrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    void Vehicle::PlaceToEndOfPath()
    {
        // RVA 0x5E7220 - puts the vehicle on the ground at the last path point, facing along the last segment.
        SetLinearVelocity(ZeroVector);
        if (m_pPath)
        {
            auto pathSize = m_pPath->GetSize();
            m_pathNum = pathSize;
            CVector pos;
            if (ai::GetPathItem(m_pPath, pathSize - 1, pos))
            {
                pos.y = M3D_ENGINE_CFG.GetHeight(pos.x, pos.z);

                CVector prevPos;
                if (ai::GetPathItem(m_pPath, pathSize - 2, prevPos))
                {
                    prevPos.y = M3D_ENGINE_CFG.GetHeight(prevPos.x, prevPos.z);
                    auto vec = pos - prevPos;
                    auto len = vec.length();
                    if (len > 0.01)
                    {
                        auto res = vec.getNormalized();
                        SetDirection(res);
                    }
                }
                SetGamePositionOnGround(pos, true, false);
                _SetIdleMoveStatusAndCauseTargetReached();
            }
        }
    }

    int Vehicle::GetIndexInTeam() const
    {
        // RVA 0x5CC210
        return m_indexInTeam;
    }

    CVector Vehicle::GetBumperPoint() const
    {
        // RVA 0x5CC560
        return m_bumperPoint;
    }

    GeomRepository* Vehicle::GetGroundRepository() const
    {
        // RVA 0x5CBB60
        return m_groundRepository;
    }

    bool Vehicle::ApplyModifier(Modifier const& modifier)
    {
        // RVA 0x5D9F50
        if (Obj::ApplyModifier(modifier))
        {
            return true;
        }
        // hp / fuel modifiers are clamped against the corresponding maximum.
        if (modifier.m_PropertyName == "hp")
        {
            Health().value().ApplyModifier(modifier, Health().maxValue().get());
            return true;
        }
        if (modifier.m_PropertyName == "maxhp")
        {
            Health().maxValue().ApplyModifier(modifier, Health().maxValue().get());
            return true;
        }
        if (modifier.m_PropertyName == "fuel")
        {
            Fuel().value().ApplyModifier(modifier, Fuel().maxValue().get());
            return true;
        }
        if (modifier.m_PropertyName == "maxfuel")
        {
            Fuel().maxValue().ApplyModifier(modifier, Fuel().maxValue().get());
            return true;
        }
        return false;
    }

    void Vehicle::SetTurningToGroundForceAndTorque(CVector const& pos, CVector const& force, CVector const& torque)
    {
        // RVA 0x5CC460: NOTE: pos is unused by the shipped code.
        // The inputs are in units of g per unit mass; scale them into real N / N*m.
        float const mass = GetMass();
        CVector const realTorque = torque * (mass * 9.8100004f * 4.0f);
        CVector const realForce = force * (mass * 9.8100004f * 0.83333331f);
        AddForce(realForce);
        AddRelTorque(realTorque);
    }

    Wheel* Vehicle::GetWheel(unsigned num)
    {
        return m_wheels[num].GetWheel();
    }

    Wheel const* Vehicle::GetWheel(unsigned num) const
    {
        return m_wheels[num].GetWheel();
    }

    float Vehicle::GetThrottle() const
    {
        // RVA 0x5CC240
        return m_throttle;
    }

    IzvratRepository const* Vehicle::GetRepository() const
    {
        // RVA 0x5CBB50
        return m_repository;
    }

    IzvratRepository* Vehicle::GetRepository()
    {
        // RVA 0x5CBB40
        return m_repository;
    }

    bool Vehicle::RemoveChild(Obj* pObj)
    {
        // RVA 0x5E5F10
        ComplexPhysicObj::RemoveChild(pObj);
        if (!pObj || pObj->GetParentId() != GetId())
        {
            return false;
        }
        if (pObj->IsKindOf(RT_CLASS_LOCAL(Bullet)))
        {
            pObj->SetParentInvalid();
            return true;
        }
        if (auto* gadget = RT_DYNCAST(pObj, Gadget))
        {
            m_gadgets.erase(gadget->GetSlotNum());
            M3D_APP->ImmediateMessage(66551, GetId(), gadget->GetSlotNum(), 0, 0, {}, {});
            gadget->ApplyToVehicle(this, false);
            gadget->SetParentInvalid();
            return true;
        }
        return false;
    }

    void Vehicle::SetBrake(float brake)
    {
        // RVA 0x5CC290
        m_brake = brake;
    }

    m3d::AIParam Vehicle::VehicleAIOnDead(Obj*)
    {
        // RVA 0x5E4B90: registered AI hook, returns an empty AIParam in the shipped build.
        return {};
    }

    void Vehicle::SetMaxSpeed(float speed)
    {
        // RVA 0x5CC320
        if (auto* cabin = GetCabin())
        {
            cabin->SetMaxSpeed(speed);
        }
    }

    bool Vehicle::HasAmountOfItemsInRepository(char const* prototypeName, int amount) const
    {
        // RVA 0x5CCC60
        return m_repository && m_repository->HasAmountOfItems(prototypeName, amount);
    }

    void Vehicle::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
    {
        // RVA 0x5ED260
        for (auto const& prop : m_propertiesMap)
        {
            Props.insert(prop.first);
        }
        // NOTE: ComplexPhysicObj is skipped.
        PhysicObj::GetPropertiesNames(Props);
    }

    void Vehicle::SetTurboThrottleTime(float time)
    {
        // RVA 0x5CCDB0
        m_turboThrottleTime = time;
    }

    namespace
    {
        class LocalProfiler
        {
        public:
            LocalProfiler(m3d::Profiler* profiler) : m_profiler(profiler)
            {
                m_profiler->StartCountdown();
            }

            ~LocalProfiler()
            {
                m_profiler->EndCountdown();
            }

        private:
            /* 0x0000 */ m3d::Profiler* m_profiler;
        }; /* size: 0x0004 */
    }  // namespace

    void Vehicle::Update(float elapsedTime, unsigned workTime)
    {
        // RVA 0x5EC0D0 - one AI step of a vehicle on the map (not one lying in a repository, and
        // only while it is active): health and durability upkeep, the player's fuel and mileage,
        // then the role and the driving - by the player, along a path, or in formation - and
        // finally the physical upkeep: water, stabilisation, engine, gears, steering, suspension.
        if (GetParentRepository() || (GetFlags() & 1) == 0)
        {
            return;
        }
        LocalProfiler prof(pServer->GetPathFindingProfiler());
        PhysicObj::Update(elapsedTime, workTime);
        if (GetPassedToAnotherMapStatus())
        {
            return;
        }

        m_bCurSteeringForceValid = false;
        _EnsureRecollection();
        if (_GetDeadStatus())
        {
            _DeadActions(elapsedTime);
            return;
        }
        if (elapsedTime < 0.000099999997f)
        {
            return;
        }

        _UpdatePhysicsUpdater();
        if (Health().minValue().get() >= Health().value().get())
        {
            if (!m_bImmortalMode)
            {
                _EvaluateToDead();
                return;
            }
            CauseEvent(GE_VEHICLE_WITHOUT_HEALTH, 0.0f, m3d::AIParam(GetId()), m3d::AIParam());
        }

        if (VehiclePrototypeInfo const* prototypeInfo = GetPrototypeInfo())
        {
            if (prototypeInfo->m_healthRegeneration != 0.0f)
            {
                Health().regenerate(elapsedTime);
            }
            if (prototypeInfo->m_durabilityRegeneration != 0.0f)
            {
                for (auto& [name, part] : m_vehicleParts)
                {
                    if (!part)
                    {
                        continue;
                    }
                    if (part->IsKindOf(RT_CLASS_LOCAL(CompoundVehiclePart)))
                    {
                        static_cast<CompoundVehiclePart*>(part)->RegenerateDurability(elapsedTime);
                    }
                    else
                    {
                        part->Durability().regenerate(elapsedTime);
                    }
                }
            }
        }

        if (m_bCustomControl && m_customControlWeapons)
        {
            WeaponFirer::WeaponLookAtPoint(this, _GetCustomWeaponTargetPoint(), elapsedTime);
            _CauseCustomGunPointedEvents();
        }

        if (m_bIsControlledByPlayer)
        {
            if (m_bMustGetOutOfDifficultPlace)
            {
                _GetOutOfDifficlultPlaceInternal();
                m_bMustGetOutOfDifficultPlace = false;
            }
            // The engine burns fuel in proportion to the vehicle's mass and its revs.
            if (Cabin* cabin = GetCabin())
            {
                float const fuelConsumption = cabin->GetFuelConsumption();
                double const rpm = fabs(m_engineRpm);
                Fuel().regenerate(static_cast<float>(GetMass() * rpm * fuelConsumption * elapsedTime * 0.000001));
            }
            // The distance driven, over the whole game and on this map.
            auto* totalPath = static_cast<FloatStatistic*>(
                theStatisticManager->GetStatistic(STATISTIC_PATH_ELAPSED, CStr("FloatStatistic")));
            totalPath->m_bGlobalFlag = true;
            totalPath->Increase(static_cast<float>(GetLinearVelocity().length() * elapsedTime));
            auto* mapPath = static_cast<FloatStatistic*>(theStatisticManager->GetStatistic(
                STATISTIC_PATH_ELAPSED + pServer->GetWorld()->m_level->m_levelName, CStr("FloatStatistic")));
            mapPath->m_bGlobalFlag = false;
            mapPath->Increase(static_cast<float>(GetLinearVelocity().length() * elapsedTime));
            _CheckForNearbyChests();
        }

        if (m_bIsTrailer)
        {
            // A trailer is only towed.
            SetThrottle(0.0f, false);
        }
        else
        {
            if (VehicleRole* role = GetRole())
            {
                if (!m_bIsMovingAlongExternalPath || m_bCanBeDistractedFromMoving)
                {
                    role->UpdateVehicle(elapsedTime, this);
                }
            }
            m_timeOutForNextIntersectionWithWorld.regenerate(elapsedTime);
            if (m_timeOutForNextIntersectionWithWorld.value().get() ==
                m_timeOutForNextIntersectionWithWorld.minValue().get())
            {
                m_timeOutForNextIntersectionWithWorld.value().set(_GetTimeOutForNextIntersectionWithWorld());
                IntersectWithWorld();
            }

            bool const playerDrives = m_bIsControlledByPlayer && !m_bIsMovingAlongExternalPath;
            if (playerDrives && !m_bCustomControl)
            {
                // The player drives; the vehicle only looks around and aims.
                _UpdateSeenObjAndWeapons(elapsedTime);
                _UpdateAlarmStatus();
                _UpdateLockedObj(elapsedTime);
            }
            else
            {
                // NOTE: a player's vehicle under custom control also comes this way, but keeps its
                // throttle and steering.
                if (!playerDrives && !m_bCustomControl)
                {
                    SetThrottle(0.0f, true);
                    m_steerRadians = 0.0f;
                }
                if (m_moveStatus == MOVE_IDLE)
                {
                    SetThrottle(0.0f, true);
                    delete m_pPath;
                    m_pPath = nullptr;
                    m_pathNum = -1;
                }

                if (m_moveStatus == MOVE_MOVING_BY_STEERING_FORCE)
                {
                    // Driving in formation: towards the external destination until the formation
                    // stops and the vehicle has reached its place in it.
                    _DriveBySteeringForce(_CalcSteeringForce(elapsedTime));
                    // NOTE: the parent is taken to be a team without a type check.
                    Formation* formation = static_cast<Team*>(GetParent())->GetFormation();
                    if (!formation->bIsMoving())
                    {
                        CVector const direction = formation->GetDirection();
                        CVector const nextPoint(
                            m_externalDestination.x + direction.x,
                            m_externalDestination.y + direction.y,
                            m_externalDestination.z + direction.z);
                        if (_bPassedPathPoint(m_externalDestination, nextPoint, true))
                        {
                            m_moveStatus = MOVE_IDLE;
                        }
                    }
                }
                else if (m_pPath && m_pathNum >= 0)
                {
                    // Following a path, one point after another; the last one must be reached
                    // precisely.
                    CVector curPoint;
                    GetPathItem(m_pPath, m_pathNum, curPoint);
                    CVector const nextPoint = _GetNextPathPoint();
                    bool const isLastPoint = m_pathNum == static_cast<int>(m_pPath->GetSize()) - 1;
                    CVector const steeringForce = _CalcSteeringForce(elapsedTime);
                    _DriveBySteeringForce(steeringForce);
                    if (_bPassedPathPoint(curPoint, nextPoint, isLastPoint))
                    {
                        ++m_pathNum;
                    }
                    if (m_pathNum >= static_cast<int>(m_pPath->GetSize()) && m_moveStatus == MOVE_MOVING_ALONG_PATH)
                    {
                        _SetIdleMoveStatusAndCauseTargetReached();
                    }
                }
                else if (!m_bCustomControl)
                {
                    SetThrottle(0.0f, true);
                    m_steerRadians = 0.0f;
                }

                // Not yet attacking: any enemy vehicle among the nearby obstacles is noticed.
                if (m_attackStatus == ATTACK_IDLE)
                {
                    for (auto const& obstacle : m_currentNearbyObstacles)
                    {
                        m3d::Object* owner = obstacle->GetOwner();
                        if (owner && owner->GetClass() == RT_CLASS_LOCAL(Vehicle) &&
                            static_cast<Obj*>(owner)->bIsEnemyWith(this))
                        {
                            CauseEvent(
                                GE_NOTICE_ENEMY, 0.0f, m3d::AIParam(static_cast<Obj*>(owner)->GetId()), m3d::AIParam());
                        }
                    }
                }
            }
        }

        _TakeWaterIntoAccount(elapsedTime);
        _ApplyStabilizingForces();
        _KeepThrottle(true);
        _KeepGearBox(elapsedTime);
        _KeepSteer(elapsedTime);
        _KeepSuspension();
        _AdjustTrailer();
        if (!m_bIsControlledByPlayer)
        {
            ActivateHeadLights(m3d::pClient->GetWorld().GetWeatherManager().GetCurrentDayTime() == m3d::GTP_NIGHT_TIME);
        }
        if (m_stoppageMode)
        {
            SetLinearVelocity(ZeroVector);
            SetAngularVelocity(ZeroVector);
        }
    }

    float Vehicle::GetCollisionRadius() const
    {
        // RVA 0x5D15B0
        // NOTE: the shipped code raises an "obsolete" SysError here before returning.
        float const hx = m_size.x * 0.5f * 0.69999999f;
        float const hy = m_size.y * 0.5f * 0.69999999f;
        float const hz = m_size.z * 0.5f * 0.69999999f;
        return std::sqrt(hz * hz + hy * hy + hx * hx);
    }

    float Vehicle::GetDriftCoeff() const
    {
        // RVA 0x5CCE60
        return m_driftCoeff;
    }

    void Vehicle::SetRotationSelf(Quaternion const& rot)
    {
        // Store old rotation and calculate relative rotation
        Quaternion oldRot = GetRotation();

        Quaternion invOldRot = oldRot.getInversed();

        // Calculate relative rotation: rot * invOldRot
        Quaternion relRot;
        relRot.x = (rot.w * invOldRot.x) + (rot.y * invOldRot.z) + (invOldRot.w * rot.x) - (invOldRot.y * rot.z);
        relRot.y = (invOldRot.w * rot.y) + (invOldRot.x * rot.z) + (rot.w * invOldRot.y) - (rot.x * invOldRot.z);
        relRot.z = (invOldRot.w * rot.z) + (invOldRot.y * rot.x) + (rot.w * invOldRot.z) - (rot.y * invOldRot.x);
        relRot.w = (invOldRot.w * rot.w) - (rot.x * invOldRot.x) - (rot.y * invOldRot.y) - (invOldRot.z * rot.z);

        // Apply new rotation to vehicle
        ai::PhysicObj::SetRotationSelf(rot);

        // Get new vehicle position
        CVector vehiclePos = GetPosition();

        // Update wheel positions and rotations
        for (auto wheelInfo : m_wheels)
        {
            ai::PhysicObj* wheel = wheelInfo.GetWheel();
            if (!wheel)
                continue;

            // Get wheel position relative to vehicle
            CVector wheelWorldPos = wheel->GetPosition();

            CVector wheelRelPos;
            wheelRelPos.x = wheelWorldPos.x - vehiclePos.x;
            wheelRelPos.y = wheelWorldPos.y - vehiclePos.y;
            wheelRelPos.z = wheelWorldPos.z - vehiclePos.z;

            // Create rotation matrix from relative rotation quaternion
            CMatrix rotMatrix;
            memset(&rotMatrix, 0, sizeof(rotMatrix));

            // Convert quaternion to rotation matrix
            float qx = relRot.x, qy = relRot.y, qz = relRot.z, qw = relRot.w;
            float xx = qx * qx, yy = qy * qy, zz = qz * qz;
            float xy = qx * qy, xz = qx * qz, yz = qy * qz;
            float wx = qw * qx, wy = qw * qy, wz = qw * qz;

            rotMatrix._11 = 1.0f - 2.0f * (yy + zz);
            rotMatrix._12 = 2.0f * (xy + wz);
            rotMatrix._13 = 2.0f * (xz - wy);

            rotMatrix._21 = 2.0f * (xy - wz);
            rotMatrix._22 = 1.0f - 2.0f * (xx + zz);
            rotMatrix._23 = 2.0f * (yz + wx);

            rotMatrix._31 = 2.0f * (xz + wy);
            rotMatrix._32 = 2.0f * (yz - wx);
            rotMatrix._33 = 1.0f - 2.0f * (xx + yy);

            // Transform wheel position by relative rotation
            CVector newWheelPos;
            newWheelPos.x = (rotMatrix._11 * wheelRelPos.x) + (rotMatrix._21 * wheelRelPos.y) +
                (rotMatrix._31 * wheelRelPos.z) + vehiclePos.x;
            newWheelPos.y = (rotMatrix._12 * wheelRelPos.x) + (rotMatrix._22 * wheelRelPos.y) +
                (rotMatrix._32 * wheelRelPos.z) + vehiclePos.y;
            newWheelPos.z = (rotMatrix._13 * wheelRelPos.x) + (rotMatrix._23 * wheelRelPos.y) +
                (rotMatrix._33 * wheelRelPos.z) + vehiclePos.z;

            // Set new wheel position
            wheel->SetPosition(newWheelPos);

            // Apply relative rotation to wheel
            Quaternion wheelRot = wheel->GetRotation();

            Quaternion newWheelRot;
            newWheelRot.x =
                (wheelRot.w * relRot.x) + (wheelRot.z * relRot.y) + (wheelRot.x * relRot.w) - (wheelRot.y * relRot.z);
            newWheelRot.y =
                (wheelRot.w * relRot.y) + (wheelRot.y * relRot.w) + (wheelRot.x * relRot.z) - (wheelRot.z * relRot.x);
            newWheelRot.z =
                (wheelRot.w * relRot.z) + (wheelRot.z * relRot.w) + (wheelRot.y * relRot.x) - (wheelRot.x * relRot.y);
            newWheelRot.w =
                (wheelRot.w * relRot.w) - (wheelRot.x * relRot.x) - (wheelRot.y * relRot.y) - (wheelRot.z * relRot.z);

            wheel->SetRotation(newWheelRot);
        }

        // Update trailer if exists
        int trailerObjId = this->m_trailerObjId;
        if (trailerObjId >= 0)
        {
            ai::PhysicObj* trailer = dynamic_cast<ai::PhysicObj*>(theObjects->GetEntityByObjId(trailerObjId));

            // Check if trailer object is valid
            if (trailer)
            {
                // Get trailer position relative to vehicle
                CVector trailerWorldPos = trailer->GetPosition();

                CVector trailerRelPos;
                trailerRelPos.x = trailerWorldPos.x - vehiclePos.x;
                trailerRelPos.y = trailerWorldPos.y - vehiclePos.y;
                trailerRelPos.z = trailerWorldPos.z - vehiclePos.z;

                // Create rotation matrix from relative rotation quaternion (same as above)
                CMatrix rotMatrix;
                rotMatrix.zero();

                float qx = relRot.x, qy = relRot.y, qz = relRot.z, qw = relRot.w;
                float xx = qx * qx, yy = qy * qy, zz = qz * qz;
                float xy = qx * qy, xz = qx * qz, yz = qy * qz;
                float wx = qw * qx, wy = qw * qy, wz = qw * qz;

                rotMatrix._11 = 1.0f - 2.0f * (yy + zz);
                rotMatrix._12 = 2.0f * (xy + wz);
                rotMatrix._13 = 2.0f * (xz - wy);

                rotMatrix._21 = 2.0f * (xy - wz);
                rotMatrix._22 = 1.0f - 2.0f * (xx + zz);
                rotMatrix._23 = 2.0f * (yz + wx);

                rotMatrix._31 = 2.0f * (xz + wy);
                rotMatrix._32 = 2.0f * (yz - wx);
                rotMatrix._33 = 1.0f - 2.0f * (xx + yy);

                // Transform trailer position by relative rotation
                CVector newTrailerPos;
                newTrailerPos.x = (rotMatrix._11 * trailerRelPos.x) + (rotMatrix._21 * trailerRelPos.y) +
                    (rotMatrix._31 * trailerRelPos.z) + vehiclePos.x;
                newTrailerPos.y = (rotMatrix._12 * trailerRelPos.x) + (rotMatrix._22 * trailerRelPos.y) +
                    (rotMatrix._32 * trailerRelPos.z) + vehiclePos.y;
                newTrailerPos.z = (rotMatrix._13 * trailerRelPos.x) + (rotMatrix._23 * trailerRelPos.y) +
                    (rotMatrix._33 * trailerRelPos.z) + vehiclePos.z;

                // Set new trailer position
                trailer->SetPosition(newTrailerPos);

                // Apply relative rotation to trailer
                Quaternion trailerRot = trailer->GetRotation();

                Quaternion newTrailerRot;
                newTrailerRot.x = (trailerRot.w * relRot.x) + (trailerRot.z * relRot.y) + (trailerRot.x * relRot.w) -
                    (trailerRot.y * relRot.z);
                newTrailerRot.y = (trailerRot.w * relRot.y) + (trailerRot.y * relRot.w) + (trailerRot.x * relRot.z) -
                    (trailerRot.z * relRot.x);
                newTrailerRot.z = (trailerRot.w * relRot.z) + (trailerRot.z * relRot.w) + (trailerRot.y * relRot.x) -
                    (trailerRot.x * relRot.y);
                newTrailerRot.w = (trailerRot.w * relRot.w) - (trailerRot.x * relRot.x) - (trailerRot.y * relRot.y) -
                    (trailerRot.z * relRot.z);

                trailer->SetRotation(newTrailerRot);
            }
        }
    }

    void Vehicle::InflictDamage(DamageInfo const& damageInfo)
    {
        // RVA 0x5E68A0
        if (m_bGodMode || damageInfo.damage < 0.0099999998f)
        {
            return;
        }

        auto* attacker = theObjects->GetEntityByObjId(damageInfo.attackerId);

        // Friendly fire is ignored unless the hit explicitly asks for it.
        if (!damageInfo.bDamageFriends && attacker &&
            theRelationship->GetTolerance(attacker->GetBelong(), GetBelong()) >= 3.0f)
        {
            return;
        }

        // A vehicle that is already battered soaks up proportionally less: the
        // coefficient grows with how much durability is left to lose.
        float const fullDurability = GetFullDurability();
        float const maxDurability = GetMaxFullDurability();
        float const durabilitySpecialCoeff = GetFullDurabilityCoeffForDamageType(damageInfo.damageType);

        float durabilityCoeff = 0.0f;
        if (fullDurability > 0.0001f)
        {
            durabilityCoeff =
                ((maxDurability + fullDurability) * 0.050000001f + durabilitySpecialCoeff) * 0.0099999998f;
        }

        float damageCoeff = 1.0f;
        if (auto* agent = theObjects->GetEntityByObjId(damageInfo.attackingAgentId))
        {
            if (m_bIsControlledByPlayer)
            {
                // What the player takes from enemy fire is scaled by difficulty.
                if (agent->IsKindOf(&Shell::m_classShell) ||
                    agent->IsKindOf(&Thunderbolt::m_classThunderbolt) ||
                    agent->IsKindOf(&BlastWave::m_classBlastWave))
                {
                    damageCoeff = theGlobProp.GetCoeffsForCurrentDifficultyLevel().m_damageCoeffForPlayerFromEnemies;
                }
            }
            else if (agent->IsKindOf(&Vehicle::m_classVehicle) &&
                     static_cast<Vehicle*>(agent)->m_bIsControlledByPlayer)
            {
                // ... and what the player deals by ramming has its own coefficient.
                damageCoeff = M3D_ENGINE_CFG.m_ai_enemies_ramming_damage_coeff.GetF();
            }
        }

        float const damage = ((1.0f - durabilityCoeff) * damageCoeff) * damageInfo.damage;
        if (damage < 0.0099999998f)
        {
            return;
        }

        SetLastDamageSource(damageInfo.attackerId);

        if (damageInfo.damagedPartName.empty())
        {
            M3D_LOG_ERR("Error: vehicle part with empty name damaged");
            return;
        }

        auto* part = GetPartByName(damageInfo.damagedPartName);
        if (!part)
        {
            M3D_LOG_ERR("Error: unknown vehicle part damaged: '" + damageInfo.damagedPartName + "'");
            return;
        }

        m_lastDamage = damageInfo.damageType;
        m_lastDamagedPart = part;

        // Durability is worn down on the part that was hit; health comes off the
        // vehicle as a whole.
        Modifier modToDurability;
        float const durabilityDamage = ((100.0f - durabilitySpecialCoeff) * damage) * 0.00050000002f;
        modToDurability.Create("dur", MO_SUB, m3d::AIParam(durabilityDamage));
        part->AddModifier(modToDurability);

        Modifier modToHealth;
        modToHealth.Create("hp", MO_SUB, m3d::AIParam(damage));
        modToHealth.m_SenderID = damageInfo.bDamageFriends ? -1 : damageInfo.attackerId;
        AddModifier(modToHealth);

        if (part->IsKindOf(&Basket::m_classBasket))
        {
            _InflictDamageToRepository(durabilityDamage);
        }

        // Visual damage only while the vehicle is actually simulated, and never
        // from water.
        if (bIsUpdatingByODE() && damageInfo.damageType != DAMAGE_WATER)
        {
            VehiclePart::BreakData breakData;
            breakData.point = damageInfo.hitPos;
            breakData.dir = damageInfo.hitDir;
            breakData.normal = damageInfo.normal;
            breakData.damage = modToDurability.m_Value.GetAsFloat();
            breakData.decalId = damageInfo.decalId;
            part->BreakModel(breakData);
        }

        if (m_bIsControlledByPlayer)
        {
            M3D_APP->EnqueueMessage(
                66561,
                damageInfo.attackerId,
                damageInfo.gunPrototypeId,
                damageInfo.damageType,
                static_cast<int>(damage),
                {},
                {});
        }
    }

    float Vehicle::GetMaxFullDurability() const
    {
        float res = 0.0;

        auto* cabin = GetCabin();
        if (cabin)
        {
            res += cabin->Durability().maxValue().get();
        }

        auto* basket = GetBasket();
        if (basket)
        {
            res += basket->Durability().maxValue().get();
        }

        return res;
    }

    void Vehicle::SetInvisible()
    {
        // RVA 0x5DD290
        ComplexPhysicObj::SetInvisible();
        for (auto& wheelInfo : m_wheels)
        {
            if (auto* wheel = wheelInfo.GetWheel())
            {
                wheel->SetInvisible();
            }
        }
        DisableGeometry(true);
        if (auto* trailer = theObjects->GetEntityByObjId(m_trailerObjId))
        {
            trailer->SetInvisible();
        }
        bool enabled = false;
        if (m_engineHighSoundNode)
        {
            m_engineHighSoundNode->SetProperty(9733u, &enabled);
        }
        if (m_engineLowSoundNode)
        {
            m_engineLowSoundNode->SetProperty(9733u, &enabled);
        }
    }

    void Vehicle::SetCustomLinearVelocity(float velocityValue)
    {
        auto direction = GetDirection();
        auto value = 1.0 /
            sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z + 0.00000011920929);

        CVector velocity;
        velocity.x = (direction.x * value) * velocityValue;
        velocity.y = (direction.y * value) * velocityValue;
        velocity.z = (direction.z * value) * velocityValue;

        SetLinearVelocity(velocity);
        SetAngularVelocity({0.0, 0.0, 0.0});
        for (auto& wheelInfo : m_wheels)
        {
            if (auto* wheel = wheelInfo.GetWheel())
            {
                wheel->SetLinearVelocity(velocity);
                wheel->SetAngularVelocity({0.0, 0.0, 0.0});
            }
        }

        if (auto* trailer = theObjects->GetEntityByObjId(m_trailerObjId))
        {
            auto* trailerVehicle = RT_DYNCAST(trailer, Vehicle);
            trailerVehicle->SetCustomLinearVelocity(velocityValue);
        }
    }

    Basket* Vehicle::GetBasket()
    {
        return RT_DYNCAST(GetPartByName(BASKET), Basket);
    }

    Basket const* Vehicle::GetBasket() const
    {
        return RT_DYNCAST(GetPartByName(BASKET), Basket const);
    }

    void Vehicle::SetCabin(VehiclePart* newCabin)
    {
        // RVA 0x5CBAC0
        SetPartByName(CABIN, newCabin, false);
    }

    void Vehicle::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        // RVA 0x5ED360
        for (auto const& prop : m_propertiesMap)
        {
            Props.insert(prop.second);
        }
        // NOTE: ComplexPhysicObj is skipped.
        PhysicObj::GetPropertiesIDs(Props);
    }

    void Vehicle::IntersectWithWorld() const
    {
        // RVA 0x5EABC0 - refreshes the obstacles around the vehicle. For the player it also counts the chests
        // within reach and (un)subscribes vehicles entering or leaving the look sphere to its radio.
        if (m_bIsControlledByPlayer)
        {
            m_pastNearbyObstacles = m_currentNearbyObstacles;

            auto* pos = dGeomGetPosition(m_takingSphere->GetGeomId());
            m_pastTakingSpherePosition.x = pos[0];
            m_pastTakingSpherePosition.y = pos[1];
            m_pastTakingSpherePosition.z = pos[2];
            m_pastNumNearbyChests = m_currentNumNearbyChests;
            m_currentNumNearbyChests = 0;
            m_bAllowPickUpMessage = 1;
        }

        IntersectionManager::GetIntersectedObjects(
            m_currentNearbyObstacles, GetIntersectionSphere(), m_targetClasses, false, false);

        if (m_bIsControlledByPlayer)
        {
            for (auto const& obstacle : m_currentNearbyObstacles)
            {
                auto* ownerObj = obstacle->GetOwnerPhysicObj();
                if (!ownerObj)
                {
                    continue;
                }

                if (IS_KIND_OF(ownerObj, Vehicle))
                {
                    if (m_pastNearbyObstacles.find(obstacle) == m_pastNearbyObstacles.end())
                    {
                        auto* vehicle = RT_DYNCAST(ownerObj, Vehicle);
                        vehicle->SubscribeRadioManagerOnNearbyObjId(GetId());
                    }
                }
                if (IS_KIND_OF(ownerObj, Chest))
                {
                    // Chests actually touching the pick-up sphere are counted.
                    dReal const* const takePos = dGeomGetPosition(m_takingSphere->GetGeomId());
                    CVector const takeCenter(takePos[0], takePos[1], takePos[2]);
                    float const takeRadius = m_takingSphere->GetRadius();
                    float const chestRadius = ownerObj->m_intersectionObstacle
                        ? ownerObj->m_intersectionObstacle->GetSphere()->GetRadius()
                        : 0.0f;
                    if (IntersectionManager::SpheresIntersect(
                            ownerObj->GetMassCenterPosition(), chestRadius, takeCenter, takeRadius))
                    {
                        ++m_currentNumNearbyChests;
                    }
                }
            }

            for (auto const& obstacle : m_pastNearbyObstacles)
            {
                auto* ownerObj = obstacle->GetOwnerPhysicObj();
                if (!ownerObj)
                {
                    continue;
                }

                if (IS_KIND_OF(ownerObj, Vehicle))
                {
                    if (m_currentNearbyObstacles.find(obstacle) == m_currentNearbyObstacles.end())
                    {
                        auto* vehicle = RT_DYNCAST(ownerObj, Vehicle);
                        vehicle->UnsubscribeRadioManagerFromNearbyObjId(GetId());
                    }
                }
            }
        }
    }

    void Vehicle::SetForcedMaxTorque(float forcedMaxTorque)
    {
        // RVA 0x5CC430
        m_maxTorqueForced = true;
        m_maxTorqueForcedValue = forcedMaxTorque;
    }

    bool Vehicle::DriveToPoint(CVector const& point, CVector const& nextPoint, bool bPrecisely, float)
    {
        // RVA 0x5D59C0 - steers and throttles towards point; true once it has been passed.
        CVector const vehiclePos = GetPosition();
        DrivingValues dv;
        CalcDrivingValues(*this, point, nextPoint, bPrecisely, dv);

        float const dx = point.x - vehiclePos.x;
        float const dz = point.z - vehiclePos.z;
        float const distToPoint =
            static_cast<float>(sqrt(static_cast<double>(dz) * dz + 0.0 * 0.0 + static_cast<double>(dx) * dx));
        if ((!bPrecisely || dv.checkCircleRadius > distToPoint) &&
            vehiclePos.z * dv.checkLine.normal.z + vehiclePos.x * dv.checkLine.normal.x >
                dv.checkLine.origin.z * dv.checkLine.normal.z + dv.checkLine.origin.x * dv.checkLine.normal.x)
        {
            return true;
        }

        // The steering is limited to 30 degrees either way.
        float const angle = _GetAngleTo(point);
        float clampedAngle = angle;
        if (fabs(angle) > 0.52359879f)
        {
            clampedAngle = static_cast<float>(angle < 0.0f ? -1 : 1) * 0.52359879f;
        }
        float const steerCoeff = clampedAngle * 1.9098593f;
        m_steerRadians = steerCoeff * -0.78539819f;

        float throttle;
        if (fabs(dv.nextAngle) > 0.1570796370506287 && dv.brakingCircleRadius > distToPoint)
        {
            throttle = 0.0f;
        }
        else
        {
            throttle = static_cast<float>((4.0 - fabs(steerCoeff) * 2.7f) * 0.25f);
        }
        SetThrottle(throttle, true);
        return false;
    }

    void Vehicle::EnableGeometry(bool changePhysicState)
    {
        // RVA 0x5DA8F0
        // NOTE: the shipped code ignores changePhysicState and always passes true, to the base and to every wheel.
        ComplexPhysicObj::EnableGeometry(true);
        for (auto& wheelInfo : m_wheels)
        {
            if (auto* wheel = wheelInfo.GetWheel())
            {
                wheel->EnableGeometry(true);
            }
        }
    }

    bool Vehicle::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x5E5310
        if (propertyId == 12)
        {
            retVal = m3d::AIParam(m_driftCoeff);
            return true;
        }
        if (propertyId == 13)
        {
            retVal = m3d::AIParam(m_antiMissileGadgetSavingRadius);
            return true;
        }
        return PhysicObj::_GetPropertyInternal(propertyId, retVal);
    }

    void Vehicle::_PutContour()
    {
        // RVA 0x5DB6D0: the base contour plus every wheel body and suspension node.
        ComplexPhysicObj::_PutContour();
        auto& graph = m3d::pClient->GetWorld().GetGraph();
        for (auto& wheelInfo : m_wheels)
        {
            auto* wheel = wheelInfo.GetWheel();
            if (!wheel)
            {
                continue;
            }
            graph.InsertInContourList(wheel->GetPhysicBody()->m_Node, m_contourColor, m_contourWidth);
            graph.InsertInContourList(wheel->m_suspensionNode, m_contourColor, m_contourWidth);
        }
    }

    namespace
    {
        bool bMustTakeScreenShot = false;
    }

    void Vehicle::_InternalCreateVisualPart()
    {
        // RVA 0x5EB380
        ai::ComplexPhysicObj::_InternalCreateVisualPart();

        m_maxSpeedLimited = false;
        m_maxTorqueForced = false;
        SetSkin(CheckSkin(GetSkin()));
        ai::bMustTakeScreenShot = true;

        auto* chassisPart = GetPartByName(CHASSIS);
        auto* chassis = chassisPart && IS_KIND_OF(chassisPart, Chassis) ? chassisPart : nullptr;
        auto* cabinPart = GetPartByName(CABIN);
        auto* cabin = cabinPart && IS_KIND_OF(cabinPart, Cabin) ? static_cast<Cabin*>(cabinPart) : nullptr;

        if (!chassis)
        {
            M3D_LOG_ERR(
                "Error: the vehicle with prototype '" + GetPrototypeInfo()->m_prototypeName + "' haven't CHASSIS part");
            return;
        }

        if (cabin)
        {
            auto const& engineHighSoundName = cabin->GetPrototypeInfo()->m_engineHighSoundName;
            if (!engineHighSoundName.empty())
            {
                // NOTE: the cabin's engine sound is attached to the chassis node.
                auto* node = PhysicBody::CreateNode(engineHighSoundName, 0, CVector(1.0f, 1.0f, 1.0f), nullptr, false);
                m_engineHighSoundNode = static_cast<m3d::SgSoundSourceNode*>(node);
                chassis->m_Node->AddChild(node);
            }
        }

        // The suspension load points are read with the vehicle at the origin, unrotated.
        auto& animatedModelsServer = static_cast<m3d::AnimatedModelsServer&>(M3D_APP->GetAnimatedModelsServer());
        CStr const chassisModelName = chassis->m_modelname;
        CVector const oldPosition = GetPosition();
        Quaternion const oldRotation = GetRotation();
        SetPosition(ZeroVector);
        SetRotation(Quaternion(0.0f, 0.0f, 0.0f, 1.0f));

        for (unsigned i = 0; i < m_wheels.size(); ++i)
        {
            auto* wheel = m_wheels[i].GetWheel();
            if (!wheel)
            {
                continue;
            }

            wheel->CreateSuspensionNode();
            // LP_SSP0<axle><side>: wheels come in left/right pairs per axle, counted from 1.
            CStr const suspensionLpName =
                CStr("LP_SSP") + CStr("0") + CStr(static_cast<int>(i / 2 + 1)) + CStr(i & 1 ? "R" : "L");
            if (!wheel->m_suspensionNode)
            {
                continue;
            }

            chassis->m_Node->AddChild(wheel->m_suspensionNode);
            CMatrix boneMatrix;
            if (animatedModelsServer.GetBoneMatrixByNameFromModelName(
                    chassisModelName.c_str(), suspensionLpName, boneMatrix, false))
            {
                Quaternion rotation;
                rotation.FromMatrix(boneMatrix);
                wheel->m_suspensionNode->SetOriginAbs(CVector(boneMatrix._41, boneMatrix._42, boneMatrix._43));
                wheel->m_suspensionNode->SetRotation(rotation);
                wheel->m_suspensionNode->UpdateXForm(false, true);
            }
            else
            {
                wheel->m_suspensionNode->SetOriginAbs(ZeroVector);
                M3D_LOG_ERR(
                    "Error: LoadPoint not found: " + suspensionLpName + " for model '" + chassisModelName + "'");
            }
        }

        SetPosition(oldPosition);
        SetRotation(oldRotation);
        for (auto& wheelInfo : m_wheels)
        {
            if (auto* wheel = wheelInfo.GetWheel())
            {
                wheel->CreateVisualPart();
                wheel->TransferPhysicParamsToSceneGraphNode();
                wheel->GetPhysicBody()->m_Node->UpdateXForm(false, true);
                wheel->m_suspensionNode->UpdateXForm(false, true);
            }
        }

        TransferPhysicParamsToSceneGraphNode();
        IntersectWithWorld();

        auto const flags = GetFlags();
        if ((flags & 8) == 0 && (flags & 2) == 0 && !GetParentRepository())
        {
            auto* basket = GetPartByName(BASKET);
            if (basket && IS_KIND_OF(basket, Basket))
            {
                basket->SetEffectActions(m_effectActions);
                basket->SetNodeAnimAction(m_effectActions.front(), true);
            }
            if (cabin)
            {
                cabin->SetEffectActions(m_effectActions);
                cabin->SetNodeAnimAction(m_effectActions.front(), true);
            }
        }

        if (bIsContoured())
        {
            PutContour();
        }
    }

    bool Vehicle::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x5E53C0
        if (propertyId == 12)
        {
            retVal = m3d::AIParam(GetPrototypeInfo()->m_driftCoeff);
            return true;
        }
        if (propertyId == 13)
        {
            retVal = m3d::AIParam(0.0f);
            return true;
        }
        return PhysicObj::_GetPropertyDefaultInternal(propertyId, retVal);
    }

    void Vehicle::_InternalPostLoad()
    {
        // RVA 0x5E76D0 - creates the wheels on first load and attaches every wheel at its chassis load point.
        ai::PhysicObj::_InternalPostLoad();
        if (m_repository)
        {
            m_repository->SetVehicle(this);
        }
        _AdjustSizeAndBumperPoint();
        if (m_cameraHeight <= 0.0f)
        {
            m_cameraHeight = m_size.y + 1.0f;
        }
        m_cruisingSpeed = GetMaxSpeed();

        auto const* prototypeInfo = GetPrototypeInfo();
        auto* chassis = GetPartByName(CHASSIS);
        if (!chassis || !IS_KIND_OF(chassis, Chassis))
        {
            M3D_LOG_ERR(
                "Error: the vehicle with prototype '" + prototypeInfo->m_prototypeName + "' haven't CHASSIS part");
            return;
        }

        CStr const chassisModelName = chassis->m_modelname;
        auto& animatedModelsServer = static_cast<m3d::AnimatedModelsServer&>(M3D_APP->GetAnimatedModelsServer());

        // The load points are read with the vehicle at the origin, unrotated.
        CVector const oldPos = GetPosition();
        Quaternion const oldRot = GetRotation();
        SetPosition(ZeroVector);
        SetRotation(Quaternion(0.0f, 0.0f, 0.0f, 1.0f));

        bool const wheelsJustCreated = m_wheels.empty();
        if (wheelsJustCreated)
        {
            for (auto const& wheelInfo : prototypeInfo->m_wheelInfos)
            {
                int const wheelId = theObjects->CreateNewObject(wheelInfo.m_wheelPrototypeId, {}, -1, -1);
                m_wheels.push_back(WheelRuntimeInfo(static_cast<Wheel*>(theObjects->GetEntityByObjId(wheelId))));
            }
        }

        m_numOfDrivenWheels = 0;
        for (unsigned i = 0; i < prototypeInfo->m_wheelInfos.size(); ++i)
        {
            WheelRuntimeInfo& runtimeInfo = m_wheels[i];
            Wheel* wheel = runtimeInfo.GetWheel();
            if (!wheel)
            {
                continue;
            }

            wheel->m_driven = 1;
            wheel->m_steering = prototypeInfo->m_wheelInfos[i].m_steering;

            // LP_WHL0<axle><side>: wheels come in left/right pairs per axle, counted from 1.
            CStr const boneName =
                CStr("LP_WHL") + CStr("0") + CStr(static_cast<int>(i / 2 + 1)) + CStr(i & 1 ? "R" : "L");
            CMatrix boneMatrix;
            CVector initPos;
            if (animatedModelsServer.GetBoneMatrixByNameFromModelName(
                    chassisModelName.c_str(), boneName, boneMatrix, false))
            {
                initPos = CVector(boneMatrix._41, boneMatrix._42, boneMatrix._43);
                runtimeInfo.m_initialRot.FromMatrix(boneMatrix);
                if (wheelsJustCreated)
                {
                    wheel->SetRotation(runtimeInfo.m_initialRot);
                }
            }
            else
            {
                M3D_LOG_ERR("Error: LoadPoint not found: " + boneName + " for model '" + chassisModelName + "'");
                initPos = wheel->GetPosition();
            }
            initPos.y -= prototypeInfo->m_additionalWheelsHover;
            runtimeInfo.m_initialPos = initPos;

            // A loaded wheel keeps its saved position, made relative to the mass center.
            wheel->SetPosition(wheelsJustCreated ? initPos : wheel->GetPosition() - m_massCenter);
            wheel->SetInitialRotation(runtimeInfo.m_initialRot);

            // The joint is attached in the initial pose, then the wheel is put back where it was.
            CVector const wheelPos = wheel->GetPosition();
            Quaternion const wheelRot = wheel->GetRotation();
            wheel->SetPosition(runtimeInfo.m_initialPos);
            wheel->SetRotation(runtimeInfo.m_initialRot);
            wheel->AttachToPhysicObj(this);
            wheel->SetPosition(wheelPos);
            wheel->SetRotation(wheelRot);

            if (wheel->m_driven)
            {
                ++m_numOfDrivenWheels;
            }
            wheel->TransferToSpace(m_spaceId);
            if (GetFlags() & 1)
            {
                wheel->SetVisible();
            }
            else
            {
                wheel->SetInvisible();
            }
        }

        SetPosition(oldPos);
        SetRotation(oldRot);
        SetThrottle(0.0f, true);
        m_steerRadians = 0.0f;

        if (ai::thePlayer && ai::thePlayer->GetRadioManagerId() != -1 && !m_bIsControlledByPlayer)
        {
            for (int const messageId : {46, 47, 45})
            {
                ai::theProcessManager->PostMessageA(
                    2, GetId(), ai::thePlayer->GetRadioManagerId(), 0.0f, m3d::AIParam(messageId), {}, 1);
            }
        }

        if (!m_ownUpdater)
        {
            m_ownUpdater = new ai::VehicleUpdater(this);
        }
        _EnsureRecollection();
    }

    float Vehicle::_CalcMassForBody() const
    {
        auto res = ai::ComplexPhysicObj::_CalcMassForBody();
        if (m_repository)
        {
            return m_repository->GetMass() + res;
        }
        return res;
    }

    void Vehicle::_RemoveContour()
    {
        // RVA 0x5DB790
        ComplexPhysicObj::_RemoveContour();
        auto& graph = m3d::pClient->GetWorld().GetGraph();
        for (auto& wheelInfo : m_wheels)
        {
            auto* wheel = wheelInfo.GetWheel();
            if (!wheel)
            {
                continue;
            }
            graph.DeleteFromContourList(wheel->GetPhysicBody()->m_Node);
            graph.DeleteFromContourList(wheel->m_suspensionNode);
        }
    }

    void Vehicle::_KeepSteer(float elapsedTime)
    {
        // RVA 0x5DA940 - turns each wheel towards m_steerRadians (times its steering factor) at
        // m_steeringSpeed; a wheel swinging back towards the centre turns faster, the more so the
        // further it is out. A step that would overshoot the target stops at it.
        for (auto& wheelInfo : m_wheels)
        {
            Wheel* wheel = wheelInfo.GetWheel();
            if (!wheel)
            {
                continue;
            }
            _AdjustWheel(wheelInfo);

            float const target = static_cast<float>(wheel->m_steering) * m_steerRadians;
            float const current = wheel->m_curAngle;
            float const towardsCentre = (target - current) * current;
            float speed;
            if (towardsCentre > 0.000001f || towardsCentre >= -0.000001f)
            {
                speed = m_steeringSpeed;
            }
            else
            {
                double const absAngle = fabs(current);
                speed = static_cast<float>((absAngle + absAngle + 1.0) * m_steeringSpeed);
            }
            int const dir = target - current >= 0.0f ? 1 : -1;
            float newAngle = static_cast<float>(dir) * speed * elapsedTime + current;

            float const after = newAngle - target;
            int const signAfter = after > 0.000001f ? 1 : (after >= -0.000001f ? 0 : -1);
            double const before = current - static_cast<float>(wheel->m_steering) * m_steerRadians;
            int const signBefore = before > 0.000001 ? 1 : (before >= -0.000001 ? 0 : -1);
            if (signAfter * signBefore <= 0)
            {
                newAngle = static_cast<float>(wheel->m_steering) * m_steerRadians;
            }
            _TurnWheelByAngle(wheel, newAngle - current);
            wheel->m_curAngle = newAngle;
        }
    }

    void Vehicle::_UpdateOwnPhysics(float elapsedTime)
    {
        PhysicObj::_UpdateOwnPhysics(elapsedTime);
        auto const flags = GetFlags();
        if ((flags & 8) == 0 && (flags & 2) == 0 && !GetParentRepository())
        {
            m_ownUpdater->Update(elapsedTime);
        }
    }

    AI* Vehicle::GetAIPtr()
    {
        // RVA 0x5CCF30
        return &m_AI;
    }

    Vehicle::~Vehicle()
    {
        // RVA 0x5ECCC0
        if (m_bIsControlledByPlayer)
        {
            SetHorn(false);
        }

        delete m_pPath;
        m_pPath = nullptr;
        delete m_takingSphere;
        m_takingSphere = nullptr;
        delete m_repository;
        m_repository = nullptr;
        delete m_groundRepository;
        m_groundRepository = nullptr;
        delete m_ownUpdater;
        m_ownUpdater = nullptr;
        if (m_trailerJoint)
        {
            dJointDestroy(m_trailerJoint);
            m_trailerJoint = nullptr;
        }
    }

    void Vehicle::RegisterProperty(char const* Name, int id, eGObjPropertySaveStatus saveStatus)
    {
        // RVA 0x5E9AD0
        m_propertiesMap[CStr(Name)] = id;
        if (saveStatus)
        {
            m_propertiesSaveStatesMap[id] = saveStatus;
        }
    }

    float Vehicle::_GetTimeOutForNextIntersectionWithWorld() const
    {
        if (ai::PhysicObj::bIsUpdatingByODE())
            return (float)rand() * 0.000030518509 * 0.1;
        else
            return (float)rand() * 0.000030518509 * 0.30000001 + 0.2;
    }

    void Vehicle::_ApplyStabilizingForces()
    {
        // RVA 0x5D5E60 - while any wheel is on the ground: a downforce growing with the horizontal
        // speed, and a yaw torque turning the vehicle with its steered wheels (reversed when
        // driving backwards). Resets the wheels-on-ground count for the next step.
        M3D_ASSERT(IsAlive());

        CVector const vel = GetLinearVelocity();
        if (m_numWheelsTouchingGround > 0)
        {
            float const horizVel = static_cast<float>(sqrt(double(vel.z) * vel.z + double(vel.x) * vel.x));
            if (horizVel > 5.0f)
            {
                float const pressingForce = GetPrototypeInfo()->m_pressingForce;
                CVector force;
                force.x = 0.0f;
                force.y = static_cast<float>(double(GetMass()) * pressingForce * horizVel * -0.19620000f);
                force.z = 0.0f;
                AddForce(force);
            }
        }

        float const speed =
            static_cast<float>(sqrt(double(vel.y) * vel.y + double(vel.z) * vel.z + double(vel.x) * vel.x));
        if (m_numWheelsTouchingGround > 0)
        {
            if (Wheel const* wheel = GetFirstExistingWheel())
            {
                static CVector const INITIAL_UP_DIRECTION(0.0f, 1.0f, 0.0f);
                float const curAngle = wheel->m_curAngle;
                CVector const dir = GetDirection();
                float const halfThrottle = m_throttle * 0.5f;
                // NOTE: a NaN velocity counts as driving forwards.
                int const sign = 0.0f > (dir.y * vel.y + dir.z * vel.z) + dir.x * vel.x ? -1 : 1;

                float const mass = GetMass();
                CVector torque(
                    (0.0f - INITIAL_UP_DIRECTION.x) * curAngle * mass * speed * m_driftCoeff,
                    (0.0f - INITIAL_UP_DIRECTION.y) * curAngle * mass * speed * m_driftCoeff,
                    (0.0f - INITIAL_UP_DIRECTION.z) * curAngle * mass * speed * m_driftCoeff);
                float const cabinControl = _GetCabinControlCoeff();
                float const throttleCoeff = static_cast<float>(fabs(halfThrottle) + 0.5);
                float const fsign = static_cast<float>(sign);
                torque.x = torque.x * cabinControl * fsign * throttleCoeff;
                torque.y = torque.y * cabinControl * fsign * throttleCoeff;
                torque.z = torque.z * cabinControl * fsign * throttleCoeff;
                AddRelTorque(torque);
            }
        }

        m_numWheelsTouchingGround = 0;
    }

    void Vehicle::_UpdateAlarmStatus()
    {
        // RVA 0x5E4540: raise GE_NOTICE_ENEMY once for every hostile vehicle or
        // turret standing nearby.
        for (auto const& obstacle : m_currentNearbyObstacles)
        {
            auto* owner = obstacle->GetOwnerPhysicObj();
            if (!owner || owner == this)
            {
                continue;
            }
            auto* ownerClass = owner->GetClass();
            if (ownerClass != RT_CLASS_LOCAL(Vehicle) && ownerClass != &StaticAutoGun::m_classStaticAutoGun)
            {
                continue;
            }
            unsigned const flags = owner->GetFlags();
            if ((flags & 8) != 0 || (flags & 2) != 0 || owner->GetParentRepository())
            {
                continue;
            }
            if (theRelationship->CheckTolerance(GetBelong(), owner->GetBelong()) <= RS_ENEMY)
            {
                CauseEvent(GE_NOTICE_ENEMY, 0.0f, {}, {});
            }
        }
    }

    m3d::Object* Vehicle::Clone()
    {
        // RVA 0x5D52D0
        // The shipped code raises a SysError ("Object cannot be cloned") and returns
        // null; CloneObj() is the supported path.
        return nullptr;
    }

    void Vehicle::_AttachExistingTrailer(Vehicle* trailer, bool bTrailerIsNew)
    {
        // RVA 0x5D72F0 - hooks the trailer to the basket's LP_TRAIL01 load point with a universal joint.
        if (!trailer)
        {
            return;
        }
        if (m_trailerObjId != -1)
        {
            M3D_LOG_INFO("Warning: attaching trailer twice to " + GetDebugDescription());
            return;
        }

        m_trailerObjId = trailer->GetId();
        // The joint is built with the vehicle at the origin, then everything is put back.
        CVector const oldPos = GetPosition();
        Quaternion const oldRot = GetRotation();
        CVector const oldTrailerPos = trailer->GetPosition();
        Quaternion const oldTrailerRot = trailer->GetRotation();
        SetPosition(ZeroVector);
        SetRotation(IdentityQuaternion);
        trailer->m_bIsTrailer = true;
        trailer->LinkToParent(GetId(), HIERARCHY_CHILD);

        // NOTE: neither the trailer's chassis nor this vehicle's basket is checked for null.
        CStr const trailerChassisModelName(trailer->GetChassis()->m_modelname);
        auto* const modelsServer = static_cast<m3d::AnimatedModelsServer*>(&M3D_APP->GetAnimatedModelsServer());
        Basket* const basket = GetBasket();
        CVector const basketRelPos = basket->GetNodeRelativePosition();
        Quaternion const basketRelRot = basket->GetNodeRelativeRotation();

        CMatrix res;
        res.zero();
        if (!modelsServer->GetBoneMatrixByNameFromModelName(
                basket->m_modelname.c_str(), CStr("LP_TRAIL") + CStr("0") + CStr(1), res, false))
        {
            M3D_LOG_ERR(
                CStr("Error: LoadPoint not found: ") + (CStr("LP_TRAIL") + CStr("0") + CStr(1)) + CStr(" on basket ") +
                basket->m_modelname);
        }
        CMatrix const basketRot = basketRelRot.ToMatrix();
        float const rotX = basketRot._31 * res._43 + basketRot._21 * res._42 + res._41 * basketRot._11;
        float const rotY = basketRot._32 * res._43 + basketRot._22 * res._42 + basketRot._12 * res._41;
        float const rotZ = basketRot._33 * res._43 + basketRot._23 * res._42 + basketRot._13 * res._41;
        m_relTrailerJointPosOnMe.x = m_massCenter.x + (rotX + basketRelPos.x);
        m_relTrailerJointPosOnMe.y = m_massCenter.y + (basketRelPos.y + rotY);
        m_relTrailerJointPosOnMe.z = m_massCenter.z + (basketRelPos.z + rotZ);

        // NOTE: the bone matrix of the basket is reused as is if the trailer has no load point.
        if (!modelsServer->GetBoneMatrixByNameFromModelName(
                trailerChassisModelName.c_str(), CStr("LP_TRAIL") + CStr("0") + CStr(1), res, false))
        {
            M3D_LOG_ERR(
                CStr("Error: LoadPoint not found: ") + (CStr("LP_TRAIL") + CStr("0") + CStr(1)) + CStr(" on trailer ") +
                trailerChassisModelName);
            m_relTrailerJointPosOnTrailer = ZeroVector;
        }
        else
        {
            m_relTrailerJointPosOnTrailer = CVector(res._41, res._42, res._43);
        }

        trailer->SetPosition(CVector(
            m_relTrailerJointPosOnMe.x - m_relTrailerJointPosOnTrailer.x,
            m_relTrailerJointPosOnMe.y - m_relTrailerJointPosOnTrailer.y,
            m_relTrailerJointPosOnMe.z - m_relTrailerJointPosOnTrailer.z));
        trailer->SetRotation(IdentityQuaternion);

        m_trailerJoint = dJointCreateUniversal(gGlobalWorld, nullptr);
        dJointAttach(m_trailerJoint, m_body->id(), trailer->m_body->id());
        dJointSetUniversalAnchor(
            m_trailerJoint, m_relTrailerJointPosOnMe.x, m_relTrailerJointPosOnMe.y, m_relTrailerJointPosOnMe.z);
        dJointSetUniversalAxis1(m_trailerJoint, 0.0f, 1.0f, 0.0f);
        dJointSetUniversalAxis2(m_trailerJoint, 1.0f, 0.0f, 0.0f);
        dJointSetUniversalParam(m_trailerJoint, dParamLoStop, -0.78539819f);
        dJointSetUniversalParam(m_trailerJoint, dParamHiStop, 0.78539819f);
        dJointSetUniversalParam(m_trailerJoint, dParamLoStop2, -0.78539819f);
        dJointSetUniversalParam(m_trailerJoint, dParamHiStop2, 0.78539819f);
        // Heavy vehicles get a stiffer joint.
        float const cfm = GetMass() <= 100.0f ? 0.001f : 0.0000099999997f;
        dJointSetUniversalParam(m_trailerJoint, dParamCFM, cfm);
        dJointSetUniversalParam(m_trailerJoint, dParamBounce, 0.0f);
        dJointSetUniversalParam(m_trailerJoint, dParamStopCFM, cfm);
        dJointSetUniversalParam(m_trailerJoint, dParamStopERP, 0.89999998f);
        dJointSetUniversalParam(m_trailerJoint, dParamFudgeFactor, 0.001f);
        dJointSetUniversalParam(m_trailerJoint, dParamCFM2, cfm);
        dJointSetUniversalParam(m_trailerJoint, dParamBounce2, 0.0f);
        dJointSetUniversalParam(m_trailerJoint, dParamStopCFM2, cfm);
        dJointSetUniversalParam(m_trailerJoint, dParamStopERP2, 0.89999998f);
        dJointSetUniversalParam(m_trailerJoint, dParamFudgeFactor2, 0.001f);

        SetPosition(oldPos);
        SetRotation(oldRot);
        if (!bTrailerIsNew)
        {
            trailer->SetPosition(oldTrailerPos);
            trailer->SetRotation(oldTrailerRot);
        }
    }

    void Vehicle::_InflictDamageToRepository(float damage)
    {
        // RVA 0x5CD500
        if (m_repository)
        {
            m_repository->ApplyDamageToAllItems(damage / m_repository->GetNumItems());
        }
    }

    bool Vehicle::_SetIdleMoveStatus()
    {
        if (!this->m_moveStatus)
        {
            return 0;
        }
        this->m_bIsMovingAlongExternalPath = 0;
        this->m_moveStatus = MOVE_IDLE;

        SetThrottle(0.0, 1);
        if (m_pPath)
        {
            delete m_pPath;
        }
        this->m_pPath = 0;
        this->m_pathNum = -1;
        return 1;
    }

    CVector Vehicle::_CalcRepulsionForNearbyObjects(
        CVector const& myPos,
        CVector const& myPredictedPos,
        CVector const& myVel,
        CVector const& guide,
        bool bIsLookObstacle,
        CVector& attraction) const
    {
        CVector repulsion = ZeroVector;
        for (auto const& obstacle : m_currentNearbyObstacles)
        {
            auto const* owner = obstacle->GetOwnerPhysicObj();
            if (owner != this && (!owner || (owner->GetFlags() & 1) != 0))
            {
                repulsion += _CalcRepulsionForObstacle(
                    obstacle, myPos, myPredictedPos, myVel, guide, bIsLookObstacle, attraction);
            }
        }
        return repulsion;
    }

    void Vehicle::_DeadActions(float elapsedTime)
    {
        m_timeAfterDeath = elapsedTime + m_timeAfterDeath;
        m_timeAfterLastBlow = elapsedTime + m_timeAfterLastBlow;

        if (m_timeAfterDeath > 60.0)
        {
            if (!m_vehicleParts.empty())
            {
                auto* part = m_vehicleParts.begin()->second;
                if (part->m_Node->m_frameVisible != M3D_KERNEL->GetTimer().GetCurFrame() - 1)
                {
                    Remove();
                }
            }
            else
            {
                Remove();
            }
        }
        if (m_deathDamage == DAMAGE_ENERGY)
        {
            // An energy kill makes the wreck shed parts and wheels one by one until 60% of them are gone.
            unsigned const numParts = m_vehicleParts.size();
            unsigned const numWheels = m_wheels.size();
            if (theGlobProp.m_energyBlowDeltaTime * 2.0f <= m_timeAfterDeath &&
                theGlobProp.m_energyBlowDeltaTime <= m_timeAfterLastBlow &&
                static_cast<double>(m_numBlownParts) <= static_cast<double>(static_cast<int>(numWheels + numParts)) * 0.60000002f)
            {
                for (auto it = m_vehicleParts.begin(); it != m_vehicleParts.end();)
                {
                    VehiclePart* const part = (it++)->second;
                    int const chance = theGlobProp.m_energyVpBlowProbability * static_cast<int>(numParts);
                    if (chance * rand() / 0x8000 == 0 && !part->IsKindOf(RT_CLASS_LOCAL(Chassis)))
                    {
                        Blow(part);
                        ++m_numBlownParts;
                        m_timeAfterLastBlow = 0.0f;
                    }
                }
                for (auto& wheelInfo : m_wheels)
                {
                    if (wheelInfo.GetWheel())
                    {
                        unsigned const chance = theGlobProp.m_energyWheelBlowProbability * m_wheels.size();
                        if ((chance * rand() & 0xFFFF8000u) == 0)
                        {
                            Blow(wheelInfo.GetWheel());
                            ++m_numBlownParts;
                            m_timeAfterLastBlow = 0.0f;
                        }
                    }
                }
                FlowUnattachableParts(theGlobProp.m_flowWheelVelocity);
                _Construct(false);
            }
        }
        SetThrottle(0.0, 1);
        _KeepThrottle(0);
        _KeepGearBox(elapsedTime);
        _KeepSteer(elapsedTime);
        _KeepSuspension();
    }

    void Vehicle::_OnChangeCabin()
    {
        // RVA 0x5E2EF0 - the engine sound comes from the cabin, so it is recreated.
        Chassis* const chassis = GetChassis();
        Cabin* const cabin = GetCabin();
        if (m_engineHighSoundNode)
        {
            m3d::SgNode* toRemove = m_engineHighSoundNode;
            m_engineHighSoundNode->GetGraph()->RemoveNode(toRemove);
            m_engineHighSoundNode = nullptr;
        }
        if (cabin)
        {
            CStr const& soundName = cabin->GetPrototypeInfo()->m_engineHighSoundName;
            if (soundName.c_str() && strlen(soundName.c_str()) != 0)
            {
                m_engineHighSoundNode = static_cast<m3d::SgSoundSourceNode*>(PhysicBody::CreateNode(
                    cabin->GetPrototypeInfo()->m_engineHighSoundName, 0, CVector(1.0f, 1.0f, 1.0f), nullptr, false));
                // NOTE: the chassis is not checked for null.
                chassis->m_Node->AddChild(m_engineHighSoundNode);
            }
        }
        _ValidateVehicleParts();
        RecalcGadgets();
    }

    bool Vehicle::_bPassedPathPoint(CVector const& point, CVector const& nextPoint, bool bPrecisely) const
    {
        auto const vehiclePos = GetPosition();

        ai::DrivingValues dv;
        CalcDrivingValues(*this, point, nextPoint, bPrecisely, dv);
        return dv.checkCircleRadius >
            sqrt((float)(point.z - vehiclePos.z) * (float)(point.z - vehiclePos.z) + 0.0 * 0.0 +
                 (float)(point.x - vehiclePos.x) * (float)(point.x - vehiclePos.x)) &&
            (float)((float)(vehiclePos.z * dv.checkLine.normal.z) + (float)(vehiclePos.x * dv.checkLine.normal.x)) >
            (float)((float)(dv.checkLine.origin.z * dv.checkLine.normal.z) +
                    (float)(dv.checkLine.origin.x * dv.checkLine.normal.x));
    }

    void Vehicle::_CauseCustomGunPointedEvents()
    {
        // RVA 0x5E9550 - fires an event whenever a gun starts or stops bearing
        // on the custom control target, so scripts can react to the moment the
        // aim lands rather than polling.
        float const AIM_EPS = 0.02f;

        CVector const customTarget = _GetCustomWeaponTargetPoint();
        for (auto& [name, part] : m_vehicleParts)
        {
            bool newState;
            int gunId;

            if (part->IsKindOf(&Gun::m_classGun))
            {
                auto* gun = static_cast<Gun*>(part);
                gunId = gun->GetId();
                newState = gun->isLookAtPoint(customTarget, AIM_EPS);
            }
            else if (part->IsKindOf(&CompoundGun::m_classCompoundGun))
            {
                auto* gun = static_cast<CompoundGun*>(part);
                gunId = gun->GetId();
                newState = gun->isLookAtPoint(customTarget, AIM_EPS);
            }
            else
            {
                continue;
            }

            bool oldState = false;
            auto const it = m_gunsPointed.find(gunId);
            if (it != m_gunsPointed.end())
            {
                oldState = it->second;
            }

            if (newState != oldState)
            {
                CauseEvent(
                    newState ? GE_CUSTOM_GUN_POINTED : GE_CUSTOM_GUN_DISPOINTED,
                    0.0f,
                    GetId(),
                    gunId);
            }
            m_gunsPointed[gunId] = newState;
        }
    }

    void Vehicle::_KeepSuspension()
    {
        // RVA 0x5DAF20 - drives each wheel's suspension animation from how far
        // the wheel has travelled vertically, measured in the vehicle's own
        // frame so that body roll does not count as suspension travel.
        for (auto& info : m_wheels)
        {
            auto* wheel = info.GetWheel();
            if (!wheel)
            {
                continue;
            }
            auto* node = wheel->m_suspensionNode;
            if (!node)
            {
                continue;
            }

            CVector const vehiclePos = GetPosition();
            CVector const wheelPos = wheel->GetPosition();
            CVector delta;
            delta.x = wheelPos.x - vehiclePos.x;
            delta.y = wheelPos.y - vehiclePos.y;
            delta.z = wheelPos.z - vehiclePos.z;

            CMatrix unrot;
            unrot.rotTranslate(GetRotation().getInversed(), CVector(0.0f, 0.0f, 0.0f));
            float const localY = unrot.vecRot(delta).y;

            float const range = wheel->GetPrototypeInfo()->m_suspensionRange;
            float suspensionDelta = localY - info.m_initialPos.y;
            if (-range > suspensionDelta)
            {
                suspensionDelta = -range;
            }
            if (suspensionDelta > range)
            {
                suspensionDelta = range;
            }

            // The animation runs across the whole travel, so the middle of its
            // range is the wheel at rest.
            if (auto* anim = GetNodeAnimInfo(node))
            {
                anim->SetCurFrame((suspensionDelta + range) / (range * 2.0f));
            }
        }
    }

    CVector Vehicle::_GetNextPathPoint() const
    {
        // RVA 0x5CCF40
        CVector curPoint;
        if (!ai::GetPathItem(m_pPath, m_pathNum, curPoint))
        {
            return ZeroVector;
        }

        CVector nextPoint;
        if (m_pathNum < static_cast<int>(m_pPath->GetSize()) - 1)
        {
            ai::GetPathItem(m_pPath, m_pathNum + 1, nextPoint);
        }
        else if (m_pathNum > 0)
        {
            // Past the last point: extrapolate one unit along the last segment.
            CVector prevPoint;
            ai::GetPathItem(m_pPath, m_pathNum - 1, prevPoint);
            // NOTE: the original subtracts the segment direction (subss at 0x5CD030), so the
            // extrapolated point lies one unit *behind* the last point, not beyond it.
            nextPoint = curPoint - (curPoint - prevPoint).getNormalized();
        }
        else
        {
            nextPoint = curPoint;
        }

        // Degenerate case: keep the next point from coinciding with the current one.
        if ((curPoint - nextPoint).length() < 0.01)
        {
            nextPoint = curPoint + CVector(1.0f, 1.0f, 1.0f);
        }

        return nextPoint;
    }

    void Vehicle::_KeepThrottle(bool applyActions)
    {
        auto const wheelRpm = fabs(m_averageWheelAVel) * 9.5492964;
        auto const velocity = GetLinearVelocity();
        if (m_bAutoBrake)
        {
            auto const direction = GetDirection();
            auto const throttleSign = RoughSign(m_throttle);

            // Above walking pace the vehicle is fighting itself when the engine
            // and the throttle disagree. Near standstill the test is whether it
            // is drifting against the commanded direction, which is why the dot
            // product is signed by the throttle - rolling backwards under
            // reverse throttle is not the wrong way.
            auto const alongThrottle =
                (direction.z * velocity.z + direction.y * velocity.y + direction.x * velocity.x) *
                static_cast<float>(throttleSign);

            auto const isWrongWay = (wheelRpm > 5.0 && (RoughSign(m_engineRpm) * throttleSign <= 0)) ||
                (wheelRpm <= 5.0 && (throttleSign == 0 || alongThrottle < -0.1));

            if (isWrongWay)
            {
                m_throttle = 0.0;
                m_brake = 1.0;
            }
        }

        if (RoughSign(m_throttle) == 0 &&
            sqrt(velocity.z * velocity.z + velocity.y * velocity.y + velocity.x * velocity.x) < 0.5)
        {
            m_bHandBrake = true;
        }

        if (m_bHandBrake)
        {
            m_throttle = 0.0;
            m_brake = 1.0;
        }

        m_realThrottle = m_throttle - ((RoughSign(m_engineRpm) * m_brake) * 10.0);

        auto const doApplyActions = [&](ActionType const& type)
        {
            auto const flags = GetFlags();
            if ((flags & 8) == 0 && (flags & 2) == 0 && !GetParentRepository())
            {
                auto& effect = m_effectActions.front();
                if (effect != type)
                {
                    effect = type;

                    auto* basket = GetBasket();
                    if (basket)
                    {
                        basket->SetEffectActions(m_effectActions);
                        basket->SetNodeAnimAction(type, true);
                    }

                    auto* cabin = GetCabin();
                    if (cabin)
                    {
                        cabin->SetEffectActions(m_effectActions);
                        cabin->SetNodeAnimAction(type, true);
                    }
                }
            }
        };

        if (applyActions)
        {
            if (m_brake <= (GetPrototypeInfo()->m_selfBrakingCoeff + 0.000099999997))
            {
                doApplyActions(AT_MOVE1);
            }
            else if ((velocity.z * velocity.z + velocity.y * velocity.y + velocity.x * velocity.x) >= 0.1)
            {
                doApplyActions(AT_MOVE2);
            }
            else
            {
                doApplyActions(AT_STAND1);
            }
        }
    }

    void Vehicle::_CreateBlastWave()
    {
        // RVA 0x5DE120 - spawns the vehicle's death blast wave, named "<vehicle name>BlastWave", at its position.
        VehiclePrototypeInfo const* const protoInfo = GetPrototypeInfo();
        CStr const name = CStr(GetName()) + CStr("BlastWave");
        int const blastWaveId = theObjects->CreateNewObject(protoInfo->m_blastWavePrototypeId, name.c_str(), -1, -1);
        if (blastWaveId != -1)
        {
            // NOTE: the shipped code does not check the looked-up object, so a stale id dereferences null.
            auto* const blastWave = static_cast<PhysicObj*>(theObjects->GetEntityByObjId(blastWaveId));
            blastWave->SetPosition(GetPosition());
        }
    }

    CStr Vehicle::_GetTrailerName() const
    {
        // RVA 0x5D3190
        // The shipped code asserts the vehicle has a name before deriving the trailer's.
        return CStr(GetName()) + "_Trailer";
    }

    CVector Vehicle::_CalcRepulsionForObstacle(
        Obstacle const* ob,
        CVector const& myPos,
        CVector const& myPredictedPos,
        CVector const& myVel,
        CVector const& guide,
        bool bIsLookObstacle,
        CVector& attraction) const
    {
        // RVA 0x5D6470
        // The look box steers around obstacles ahead (along guide); the target box (along the
        // attraction) pushes away from obstacles in the way of where the vehicle wants to go.
        if (!bIsLookObstacle &&
            attraction.x * attraction.x + attraction.y * attraction.y + attraction.z * attraction.z < 0.001f)
        {
            return ZeroVector;
        }

        CVector const obPos = ob->GetPosition();
        CVector const obVel = ob->GetLinearVelocity();
        CVector const obPredictedPos(
            obPos.x + obVel.x * ai::theGlobProp.m_predictionTime,
            obPos.y + obVel.y * ai::theGlobProp.m_predictionTime,
            obPos.z + obVel.z * ai::theGlobProp.m_predictionTime);

        // Gaps between the intersection spheres, now and at the predicted positions.
        CVector const delta = myPos - obPos;
        float const flatDistSq = delta.z * delta.z + delta.x * delta.x;
        float const dist = sqrt(delta.y * delta.y + flatDistSq) - (ob->GetIntersectionRadius() + GetIntersectionRadius());

        CVector predictedDelta = myPredictedPos - obPredictedPos;
        float const predictedDistSq =
            predictedDelta.x * predictedDelta.x + predictedDelta.z * predictedDelta.z + predictedDelta.y * predictedDelta.y;
        float predictedDist = sqrt(predictedDistSq);
        predictedDist = predictedDist - (ob->GetIntersectionRadius() + GetIntersectionRadius());

        CVector repulsion = ZeroVector;

        Geom const* obGeom = ob->GetBox();
        if (!obGeom)
        {
            obGeom = ob->GetSphere();
        }
        scoped_ptr<ai::Box> const& myBox = bIsLookObstacle ? m_lookBox : m_targetBox;

        // Only X and Z of the steering direction are used below.
        CVector const steerDir = bIsLookObstacle ? guide : attraction;
        float const steerX = steerDir.x;
        float const steerZ = steerDir.z;

        if (!(predictedDist >= -2.0f && dist >= -1.0f) && !ob->GetBox())
        {
            // Already deep inside the obstacle's sphere: give up on the attraction and push
            // straight out, with a slight sideways bias.
            attraction.x = 0.0f;
            attraction.y = 0.0f;
            attraction.z = 0.0f;

            float const invPredictedLen = 1.0 / sqrt(predictedDistSq + 0.00000011920929);
            CVector const push(
                invPredictedLen * predictedDelta.x * ai::theGlobProp.m_repulsiveCoeff,
                predictedDelta.y * invPredictedLen * ai::theGlobProp.m_repulsiveCoeff,
                predictedDelta.z * invPredictedLen * ai::theGlobProp.m_repulsiveCoeff);

            // up x (steerX, 0, steerZ)
            CVector const up(0.0f, 1.0f, 0.0f); // INITIAL_UP_DIRECTION
            CVector const side(
                steerZ * up.y - up.z * 0.0f,
                steerX * up.z - steerZ * up.x,
                up.x * 0.0f - steerX * up.y);
            float const invSideLen = 1.0 / sqrt(side.z * side.z + side.y * side.y + side.x * side.x + 0.00000011920929);
            float const pushLen = sqrt(push.x * push.x + push.z * push.z + push.y * push.y);

            repulsion.x = invSideLen * side.x * pushLen * 0.1f + push.x;
            repulsion.y = invSideLen * side.y * pushLen * 0.1f + push.y;
            repulsion.z = invSideLen * side.z * pushLen * 0.1f + push.z;
            return repulsion;
        }

        dContact contact;
        if (dCollide(obGeom->GetGeomId(), myBox->GetGeomId(), 1, &contact.geom, sizeof(dContact)) <= 0)
        {
            return repulsion;
        }

        // Ground-plane direction from the obstacle to the vehicle, crossed with the steering direction.
        float const invFlatDist = 1.0 / sqrt(flatDistSq + 0.00000011920929);
        CVector const flatDir(invFlatDist * delta.x, invFlatDist * 0.0f, delta.z * invFlatDist);
        CVector const cross(
            steerZ * flatDir.y - flatDir.z * 0.0f,
            flatDir.z * steerX - steerZ * flatDir.x,
            flatDir.x * 0.0f - flatDir.y * steerX);

        if (cross.z * cross.z + cross.y * cross.y + cross.x * cross.x < 0.001f)
        {
            // Heading straight at the obstacle. Push away from the nearer of the predicted
            // position and the contact point.
            CVector const fromContact(
                myPos.x - contact.geom.pos[0], myPos.y - contact.geom.pos[1], myPos.z - contact.geom.pos[2]);
            float const contactDistSq =
                fromContact.z * fromContact.z + fromContact.y * fromContact.y + fromContact.x * fromContact.x;
            if (predictedDistSq > contactDistSq)
            {
                predictedDelta = fromContact;
                predictedDist = sqrt(contactDistSq) - GetIntersectionRadius();
            }

            CVector relVel = myVel - obVel;
            if (predictedDelta.x * predictedDelta.x + predictedDelta.z * predictedDelta.z +
                    predictedDelta.y * predictedDelta.y >
                0.0099999998f)
            {
                float const relSpeed = sqrt(relVel.z * relVel.z + relVel.y * relVel.y + relVel.x * relVel.x);
                relVel = predictedDelta.getNormalized() * relSpeed;
            }

            // Too close to brake in time: drop the attraction and swerve harder.
            float const relSpeed = sqrt(relVel.z * relVel.z + relVel.y * relVel.y + relVel.x * relVel.x);
            bool const cannotBrake = relSpeed * relSpeed * 0.050968397 > predictedDist;
            if (cannotBrake)
            {
                attraction.z = 0.0f;
                attraction.y = 0.0f;
                attraction.x = 0.0f;
            }

            float const distSq = predictedDist * predictedDist;
            float const falloff = 1.0f / (distSq < 1.0f ? 1.0f : distSq);
            CVector const pushDir = predictedDelta.getNormalized();
            CVector const push(
                falloff * (pushDir.x * ai::theGlobProp.m_repulsiveCoeff),
                pushDir.y * ai::theGlobProp.m_repulsiveCoeff * falloff,
                pushDir.z * ai::theGlobProp.m_repulsiveCoeff * falloff);

            float const sideScale = cannotBrake ? 0.5f : 0.1f;
            // up x (steerX, 0, steerZ)
            CVector const up(0.0f, 1.0f, 0.0f); // INITIAL_UP_DIRECTION
            CVector const side(
                up.y * steerZ - up.z * 0.0f,
                up.z * steerX - steerZ * up.x,
                up.x * 0.0f - up.y * steerX);
            float const pushLen = sqrt(push.x * push.x + push.z * push.z + push.y * push.y);
            CVector const sideDir = side.getNormalized();

            repulsion.x = sideDir.x * pushLen * sideScale + push.x;
            repulsion.y = sideDir.y * pushLen * sideScale + push.y;
            repulsion.z = sideDir.z * pushLen * sideScale + push.z;
            return repulsion;
        }

        // Sideways (perpendicular to the steering direction in the ground plane): cross x steer.
        CVector const sideDir = CVector(
                                    steerZ * cross.y - cross.z * 0.0f,
                                    cross.z * steerX - steerZ * cross.x,
                                    cross.x * 0.0f - cross.y * steerX)
                                    .getNormalized();

        // The look box swerves to whichever side the attraction already leans to; otherwise
        // (and always for the target box) to the side the vehicle is already on.
        CVector const attractionDir = attraction.getNormalized();
        float const attractionSide =
            attractionDir.y * sideDir.y + attractionDir.z * sideDir.z + attractionDir.x * sideDir.x;
        int sign;
        if (attraction.x * attraction.x + attraction.y * attraction.y + attraction.z * attraction.z > 0.000099999997f &&
            bIsLookObstacle && fabs(attractionSide) > 0.1)
        {
            sign = attractionSide < 0.0f ? -1 : 1;
        }
        else
        {
            sign = sideDir.x * delta.x + sideDir.z * delta.z + sideDir.y * 0.0f < 0.0f ? -1 : 1;
        }

        CVector const swerve(sign * sideDir.x, sideDir.y * sign, sideDir.z * sign);
        if (bIsLookObstacle)
        {
            float const falloff = 1.0f / (predictedDist < 1.0f ? 1.0f : predictedDist);
            repulsion.x = falloff * (swerve.x * ai::theGlobProp.m_repulsiveCoeff);
            repulsion.y = swerve.y * ai::theGlobProp.m_repulsiveCoeff * falloff;
            repulsion.z = swerve.z * ai::theGlobProp.m_repulsiveCoeff * falloff;
        }
        else
        {
            repulsion.x = swerve.x * ai::theGlobProp.m_repulsiveCoeff;
            repulsion.y = swerve.y * ai::theGlobProp.m_repulsiveCoeff;
            repulsion.z = swerve.z * ai::theGlobProp.m_repulsiveCoeff;
        }
        return repulsion;
    }

    void Vehicle::_DriveBySteeringForce(CVector const& steeringForce)
    {
        if (!m_inSmokeScreenMode || m_bIsControlledByPlayer)
        {
            auto const vehiclePos = GetPosition();
            auto const velocity = GetLinearVelocity();
            auto const direction = GetDirection();

            auto speed = (float)((float)((float)(direction.y * velocity.y) + (float)(direction.z * velocity.z)) +
                                 (float)(direction.x * velocity.x)) > 0.0;

            CVector point;
            point.x = steeringForce.x + vehiclePos.x;
            point.y = steeringForce.y + vehiclePos.y;
            point.z = steeringForce.z + vehiclePos.z;

            auto const angleTo = _GetAngleTo(point);
            auto steer = angleTo;
            auto throttle = fabs(angleTo);
            if (throttle < 2.5132742 && speed &&
                sqrt(velocity.y * velocity.y + velocity.z * velocity.z + velocity.x * velocity.x) > 8.333334)
            {
                m_turningBackStatus = TURN_BACK_DISABLED;
            }
            else
            {
                switch (m_turningBackStatus)
                {
                case TURN_BACK_NONE:
                {
                    m_turningBackStatus = TURN_BACK_DISABLED;
                    if (throttle >= 1.5707964)
                    {
                        m_turningBackStatus = TURN_BACK_ENABLED_ACCELERATING;
                    }
                    m_turningBackStatus = m_turningBackStatus;
                    break;
                }
                case TURN_BACK_ENABLED_ACCELERATING:
                {
                    if (throttle < 0.94247788)
                    {
                        m_turningBackStatus = TURN_BACK_ENABLED_BRAKING;
                    }
                    break;
                }
                case TURN_BACK_ENABLED_BRAKING:
                {
                    if ((float)((float)((float)(velocity.y * velocity.y) + (float)(velocity.z * velocity.z)) +
                                (float)(velocity.x * velocity.x)) < 1.0 &&
                        0.0 != fabs((double)(m_steerRadians < 0.1)))
                    {
                        m_turningBackStatus = TURN_BACK_DISABLED;
                    }
                    break;
                }
                case TURN_BACK_DISABLED:
                {
                    if (throttle > 1.8849558)
                    {
                        this->m_turningBackStatus = TURN_BACK_ENABLED_ACCELERATING;
                    }
                    break;
                }
                default:
                    break;
                }
            }

            if (m_turningBackStatus == TURN_BACK_DISABLED)
            {
                if (throttle > 0.52359879)
                {
                    int dir = 0;
                    if (steer >= 0.0)
                        dir = 1;
                    else
                        dir = -1;
                    steer = (float)dir * 0.52359879;
                }
                steer = steer * 1.9098593;
                throttle = sqrt(
                               steeringForce.x * steeringForce.x + steeringForce.y * steeringForce.y +
                               steeringForce.z * steeringForce.z) *
                    (4.0 - fabs(steer) * 2.7) * 0.25;
            }
            else
            {
                throttle = 0.0;
                if (m_turningBackStatus == TURN_BACK_ENABLED_ACCELERATING)
                {
                    int dir = 0;
                    if (steer >= 0.0)
                        dir = 1;
                    else
                        dir = -1;
                    steer = 0.0 - (float)dir;
                    throttle = -1.0;
                }
                else
                {
                    steer = 0.0;
                }
            }
            if (fabs(steer) >= 1.000001)
            {
                M3D_LOG_INFO("Error: steer of " + GetDebugDescription() + "is invalid: " + CStr(steer));
                M3D_ASSERT(0);
            }

            auto v20 = -1.0;
            if (!m_bWasStuck)
                v20 = 1.0;
            auto v21 = 0.0;
            m_steerRadians = (float)(0.0 - (float)(0.78539819 * steer)) * v20;
            auto v22 = sqrt(
                steeringForce.x * steeringForce.x + steeringForce.y * steeringForce.y +
                steeringForce.z * steeringForce.z);
            if (v22 >= 0.0)
            {
                v21 = v22;
                auto absAngle = v22;
                if (absAngle > 1.0)
                    v21 = 1.0;
            }
            SetThrottle((float)(v21 * v20) * throttle, 1);
        }
    }

    void Vehicle::_TurnWheelByAngle(Wheel* pWheel, float angle)
    {
        // RVA 0x5CD1D0 - turns the wheel by -angle about the vehicle's up axis:
        // wheelRot' = (vehicleRot * turn * vehicleRot^-1) * wheelRot.
        // NOTE: the shipped build inlines these products with its own summation order, so the last bit of the
        // result can differ from Quaternion's operator*.
        Quaternion turn;
        turn.FromAxisAngle(CVector(0.0f, 1.0f, 0.0f), -angle);
        Quaternion const vehicleRot = GetRotation();
        pWheel->SetRotation(vehicleRot * turn * vehicleRot.getInversed() * pWheel->GetRotation());
    }

    CVector Vehicle::_GetCustomWeaponTargetPoint() const
    {
        // RVA 0x5DD560
        if (m_customControlWeapons == CUSTOM_WEAPON_CONTROL_POINT)
        {
            return m_customControlWeaponsTarget;
        }
        if (m_customControlWeapons == CUSTOM_WEAPON_CONTROL_OBJECT)
        {
            if (auto const* target = theObjects->GetEntityByObjId(m_customControlWeaponsTargetObjId))
            {
                return getPhysicObjOrPhysicBodyGeometricCenter(target);
            }
        }
        return ZeroVector;
    }

    void Vehicle::_EnsureRecollection()
    {
        if (m_bIsControlledByPlayer)
        {
            if (m_recollectionId == -1)
            {
                auto protoId = thePrototypeManager->GetPrototypeId("someRecollection");
                auto objId = theObjects->CreateNewObject(protoId, {}, -1, GetBelong());
                m_recollectionId = objId;

                auto obj = RT_DYNCAST(theObjects->GetEntityByObjId(m_recollectionId), VehicleRecollection);
                if (obj)
                {
                    obj->SetVehicle(this);
                }
            }
        }
        else if (m_recollectionId != -1)
        {
            auto recollection = GetRecollection();
            recollection->Remove();
            m_recollectionId = -1;
        }
    }

    void Vehicle::_UpdateLockedObj(float elapsedTime)
    {
        // RVA 0x5DDBB0: rocket launchers lock onto whatever the player is looking
        // at, after theGlobProp.m_lockTimeout seconds of steady aim.
        if (!m_bRocketLaunchersPresent)
        {
            m_toBeLockedObjId = -1;
            m_lockedObjId = -1;
            m_timeToLockTarget = 0.0;
            return;
        }

        float newTimeToLock = 0.0f;
        if (m_seenObjId != m_lockedObjId && m_seenObjId != -1)
        {
            if (m_toBeLockedObjId != m_seenObjId)
            {
                m_toBeLockedObjId = m_seenObjId;
                m_timeToLockTarget = 0.0f;
            }
            if (theGlobProp.m_lockTimeout <= m_timeToLockTarget)
            {
                m_lockedObjId = m_seenObjId;
                m_toBeLockedObjId = -1;
            }
            else
            {
                newTimeToLock = elapsedTime + m_timeToLockTarget;
            }
        }
        else
        {
            m_toBeLockedObjId = -1;
        }
        m_timeToLockTarget = newTimeToLock;

        if (m_lockedObjId == -1)
        {
            return;
        }

        // Drop the lock once the target drifts too far from the cursor.
        auto* locked = RT_DYNCAST(theObjects->GetEntityByObjId(m_lockedObjId), PhysicObj);
        if (!locked || (locked->GetFlags() & 8) != 0 || (locked->GetFlags() & 2) != 0 || locked->GetParentRepository())
        {
            m_lockedObjId = -1;
            return;
        }

        CVector const camOrg = M3D_RENDERER->MatGetOrgInv();
        CVector screenPos = M3D_RENDERER->Project(locked->GetPosition() - camOrg);
        float mouseX = static_cast<float>(M3D_APP->GetMouseX());
        float mouseY = static_cast<float>(M3D_APP->GetMouseY());
        M3D_RENDERER->AbsToRel(screenPos.x, screenPos.y);
        M3D_RENDERER->AbsToRel(mouseX, mouseY);
        if (std::fabs(screenPos.x - mouseX) > theGlobProp.m_unlockRegion.x ||
            std::fabs(screenPos.y - mouseY) > theGlobProp.m_unlockRegion.y)
        {
            m_lockedObjId = -1;
        }
    }

    bool Vehicle::_bPointIsBehind(CVector const& point) const
    {
        // RVA 0x5D3160: more than 30 degrees off the nose.
        return std::fabs(_GetAngleTo(point)) > 0.52359879f;
    }

    void Vehicle::_GetOutOfDifficlultPlaceInternal()
    {
        // RVA 0x5E4740 - moves a stuck or overturned vehicle to the nearest free spot on the ground.
        _EnableIntersections(false);
        CVector myPos = GetPosition();
        CVector pos = ZeroVector;
        float const radius =
            m_intersectionObstacle ? m_intersectionObstacle->GetSphere()->GetRadius() : 0.0f;
        bool const bValid = GetValidPosition(myPos, radius, m_priority, pos, false, true, retruxx::set<m3d::Class*>());

        CVector const INITIAL_UP_DIRECTION(0.0f, 1.0f, 0.0f);
        CMatrix const rot = GetRotation().ToMatrix();
        CVector const up(
            rot._31 * INITIAL_UP_DIRECTION.z + rot._21 * INITIAL_UP_DIRECTION.y + rot._11 * INITIAL_UP_DIRECTION.x,
            rot._32 * INITIAL_UP_DIRECTION.z + rot._22 * INITIAL_UP_DIRECTION.y + rot._12 * INITIAL_UP_DIRECTION.x,
            rot._33 * INITIAL_UP_DIRECTION.z + rot._23 * INITIAL_UP_DIRECTION.y + rot._13 * INITIAL_UP_DIRECTION.x);
        bool const bOverturned =
            up.z * INITIAL_UP_DIRECTION.z + up.y * INITIAL_UP_DIRECTION.y + up.x * INITIAL_UP_DIRECTION.x < -0.2f;

        if (!bValid)
        {
            pos = myPos;
        }
        if (bValid || bOverturned)
        {
            myPos.y = 0.0f;
            pos.y = 0.0f;
            double const dx = myPos.x - pos.x;
            double const dz = myPos.z - pos.z;
            if (sqrt(dz * dz + dx * dx) > 1.0 || bOverturned)
            {
                SetGamePositionOnGround(pos, true, false);
            }
        }
        _EnableIntersections(true);
    }

    float Vehicle::_GetCabinControlCoeff() const
    {
        auto* cabin = GetCabin();

        float coeff = 50.0;
        if (cabin)
        {
            coeff = cabin->GetControl();
        }

        coeff = std::clamp(coeff, 0.0f, 100.0f);

        return 1.5 - coeff * 0.0099999998;
    }

    void Vehicle::_ValidateVehicleParts()
    {
        // RVA 0x5E2E00
        // Drop any attached part the vehicle can no longer carry. The player keeps them
        // in the repository; the AI just deletes them.
        auto const attachedParts = GetAttachedPartNames();
        for (auto const& partName : attachedParts)
        {
            if (CanPartBeAttached(partName))
            {
                continue;
            }
            auto* part = GetPartByName(partName);
            if (!part)
            {
                continue;
            }
            SetPartByName(partName, nullptr, false);
            if (m_bIsControlledByPlayer)
            {
                AddThing(GeomRepositoryItem(part->GetId()), true);
            }
            else
            {
                part->Remove();
            }
        }
    }

    void Vehicle::_AdjustLookBox(bool bForLooking, CVector const& myPos, CVector const& pathPoint, CVector const& guide)
        const
    {
        // RVA 0x5D6FB0 - lays the look box (along guide) or the target box (towards pathPoint) out
        // in front of the vehicle: 1.5 times its width, twice its height, and as long as the
        // default length or the distance to pathPoint, whichever is shorter.
        // NOTE: the look box is also cut short by the distance to pathPoint, although it does
        // not point at it.
        float const dx = pathPoint.x - myPos.x;
        float const dy = pathPoint.y - myPos.y;
        float const dz = pathPoint.z - myPos.z;
        float const distToPathPoint = static_cast<float>(sqrt(double(dz) * dz + double(dy) * dy + double(dx) * dx));

        float defaultLength;
        CVector dir;
        if (bForLooking)
        {
            defaultLength = theGlobProp.m_defaultLookBoxLength;
            float const inv = static_cast<float>(
                1.0 / sqrt(double(guide.x) * guide.x + double(guide.y) * guide.y + double(guide.z) * guide.z + 0.00000011920929));
            dir = CVector(guide.x * inv, guide.y * inv, guide.z * inv);
        }
        else
        {
            defaultLength = theGlobProp.m_defaultTargetBoxLength;
            // NOTE: a path point at the vehicle's own position gives a zero direction.
            float const inv = static_cast<float>(
                1.0 / sqrt(double(dz) * dz + double(dy) * dy + double(dx) * dx + 0.00000011920929));
            dir = CVector(inv * dx, dy * inv, dz * inv);
        }
        float const length = distToPathPoint > defaultLength ? defaultLength : distToPathPoint;

        Box& box = bForLooking ? *m_lookBox : *m_targetBox;
        box.SetSize(CVector(m_size.x * 1.5f, m_size.y * 2.0f, length));
        SetDirectionToObject<Geom>(box, dir);
        dGeomSetPosition(
            box.GetGeomId(),
            myPos.x + dir.x * length * 0.5f,
            dir.y * length * 0.5f + myPos.y,
            myPos.z + dir.z * length * 0.5f);
    }

    CVector Vehicle::_CalcSteeringForceToPathPoint(CVector const& point, CVector const& nextPoint) const
    {
        // RVA 0x5D62E0
        // A unit ground-plane force towards point, or zero while the vehicle is still
        // inside the braking circle of a sharp (more than 9 degrees) turn at point.
        auto const vehiclePos = GetPosition();

        ai::DrivingValues dv;
        CalcDrivingValues(*this, point, nextPoint, true, dv);

        CVector const toPoint(point.x - vehiclePos.x, 0.0f, point.z - vehiclePos.z);
        float scale = 0.0f;
        if (fabs(dv.nextAngle) <= 0.1570796370506287 ||
            dv.brakingCircleRadius <= sqrt(toPoint.z * toPoint.z + toPoint.y * toPoint.y + toPoint.x * toPoint.x))
        {
            scale = 1.0f;
        }

        float const invLen = 1.0 / sqrt(toPoint.z * toPoint.z + toPoint.y * toPoint.y + toPoint.x * toPoint.x + 0.00000011920929);
        return CVector(invLen * toPoint.x * scale, toPoint.y * invLen * scale, toPoint.z * invLen * scale);
    }

    void Vehicle::_TakeWaterIntoAccount(float elapsedTime)
    {
        auto const pos = GetPosition();
        auto const waterHeight =
            m3d::pClient->GetWorld().GetLandscape().getWaterHeight(
                static_cast<int>(pos.x * 0.03125f), static_cast<int>(pos.z * 0.03125f));
        if (waterHeight > m_size.y + pos.y)
        {
            // Sunk: a splash unless the vehicle cannot be hurt, and the chassis drowns at a fifth of its health a second.
            if (!m_bGodMode && !m_bImmortalMode)
            {
                PhysicBody::CreateEffectNode(
                    CStr("ET_PS_VEH_WATERDEATH"), CVector(pos.x, waterHeight, pos.z), IdentityQuaternion, true, 1.0f);
            }
            DamageInfo damageInfo;
            // NOTE: the chassis is not checked for null.
            damageInfo.damage = GetChassis()->Health().maxValue().get() * elapsedTime * 0.2f;
            damageInfo.damageType = DAMAGE_WATER;
            damageInfo.damagedPartName = CHASSIS;
            InflictDamage(damageInfo);
        }
    }

    void Vehicle::_KeepGearBox(float elapsedTime)
    {
        // RVA 0x5E0E40 - automatic gearbox: shifts gears by engine RPM and drives the wheel
        // joints' motors towards the target RPM with the available torque.
        _CalcRpms();

        // Find first valid wheel
        auto wheelIter = std::find_if(
            m_wheels.begin(),
            m_wheels.end(),
            [](WheelRuntimeInfo const& wheelInfo)
            {
                return wheelInfo.GetWheel() != nullptr;
            });

        if (wheelIter == m_wheels.end())
        {
            return;
        }

        ai::Wheel* referenceWheel = wheelIter->GetWheel();

        // Get wheel radius for calculations
        float etalonRadius = referenceWheel->GetRadius();
        float currentSpeed = fabs(m_averageWheelAVel) * etalonRadius;

        // Calculate torque based on throttle
        float throttleMagnitude = fabs(m_realThrottle);
        float maxTorque = GetMaxTorque();
        float torque = maxTorque * throttleMagnitude;

        // Update turbo throttle timer
        m_turboThrottleTime -= elapsedTime;
        if (m_turboThrottleTime < 0.0f)
        {
            m_turboThrottleTime = 0.0f;
        }

        // Apply turbo boost if active
        if (m_turboThrottleTime > 0.0f)
        {
            torque *= m_turboThrottleValue;
        }

        // Determine target speed (cruising speed or max speed)
        float targetSpeed;
        if (m_bIsControlledByPlayer || m_attackStatus == ATTACK_ATTACKING)
        {
            targetSpeed = GetMaxSpeed();
        }
        else
        {
            targetSpeed = m_cruisingSpeed;
        }

        // Apply turbo to target speed
        if (m_turboThrottleTime > 0.0f)
        {
            targetSpeed *= m_turboThrottleValue;
        }

        // Cut torque if we're going too fast and not braking
        float speedDifference = currentSpeed - targetSpeed;
        float brakeThreshold = GetPrototypeInfo()->m_selfBrakingCoeff + 0.0001f;

        if (speedDifference > 0.1f && m_brake <= brakeThreshold)
        {
            torque = 0.0f;
        }

        // Maintain recent RPMs history (sliding window of 10 values)
        if (m_recentEngineRpms.size() >= 10)
        {
            if (!m_recentEngineRpms.empty())
            {
                m_recentEngineRpms.pop_front();
            }
        }
        m_recentEngineRpms.push_back(m_engineRpm);

        // Calculate average RPM over the window
        m_averageEngineRpm = 0.0f;
        if (m_recentEngineRpms.size() == 10)
        {
            for (float rpm : m_recentEngineRpms)
            {
                m_averageEngineRpm += rpm;
            }
            m_averageEngineRpm *= 0.1f;  // Divide by 10
        }

        // Update engine sound based on RPM
        if (m_engineHighSoundNode)
        {
            float rpmAbs = fabs(m_averageEngineRpm);
            float soundPitch;

            if (rpmAbs <= 500.0f)
            {
                soundPitch = 0.5f;
            }
            else
            {
                soundPitch = ((rpmAbs - 500.0f) * 0.0002f) + 0.5f;
            }

            m_engineHighSoundNode->SetProperty(m3d::PROP_SND_PLAYBACK_COEFF, &soundPitch);
        }

        // Automatic gear shifting based on RPM limits
        if (m_engineRpm < m_lowGearShiftLimit)
        {
            --m_currentGear;
        }

        if (m_engineRpm > m_highGearShiftLimit)
        {
            ++m_currentGear;
        }

        // Clamp gear to valid range [0, 4]
        m_currentGear = std::clamp(m_currentGear, 0, 4);

        // Determine target RPM based on throttle input
        float targetRpm;
        if (m_throttle > 0.000001f)
        {
            targetRpm = m_maxEngineRpm;
        }
        else if (m_throttle < -0.000001f)
        {
            targetRpm = -4000.0f;
        }
        else
        {
            targetRpm = 0.0f;
        }

        // Apply turbo to target RPM
        if (m_turboThrottleTime > 0.0f)
        {
            targetRpm *= m_turboThrottleValue;
        }

        // Apply torque to all driven wheels
        for (auto& wheelInfo : m_wheels)
        {
            ai::Wheel* wheel = wheelInfo.GetWheel();
            if (!wheel || !wheel->m_driven)
            {
                continue;
            }

            dxJointHinge2* joint = static_cast<dxJointHinge2*>(wheel->m_jointID);
            if (!joint)
                continue;

            // Calculate gear ratio for this wheel
            float wheelRadius = wheel->GetRadius();
            float gearRatio = GEAR_RATIOS[m_currentGear] * m_diffRatio * wheelRadius;

            // Calculate wheel torque distribution
            float wheelTorque = (gearRatio * torque * 1.8f) / (m_numOfDrivenWheels * etalonRadius);

            // Convert target RPM to angular velocity for the joint
            float angularVelocity = (targetRpm * etalonRadius * 6.2831855f) / (gearRatio * 108.0f);

            // Apply parameters to physics joint
            dJointSetHinge2Param(joint, dParamVel2, angularVelocity);
            dJointSetHinge2Param(joint, dParamFMax2, wheelTorque);
        }
    }

    void Vehicle::_SetIdleMoveStatusAndCauseTargetReached()
    {
        if (_SetIdleMoveStatus())
        {
            CauseEvent(GE_TARGET_REACHED, 0.0, m_pathIndex, {});
        }
    }

    void Vehicle::_CalcRpms()
    {
        // RVA 0x5DB3B0
        if (!bIsUpdatingByODE())
        {
            int gear = 0;
            m_ownUpdater->CalcRpmsAndGear(m_averageWheelAVel, m_engineRpm, gear);
            return;
        }

        // Average the wheels' spin in the vehicle's frame: the length of the angular velocity
        // in the local XZ plane, signed by its X (axle) component.
        m_averageWheelAVel = 0.0f;
        Quaternion const q = GetRotation().getInversed();
        float const m11 = 1.0f - (q.z * q.z + q.y * q.y) * 2.0f;
        float const m21 = (q.y * q.x - q.w * q.z) * 2.0f;
        float const m31 = (q.w * q.y + q.z * q.x) * 2.0f;
        float const m13 = (q.z * q.x - q.w * q.y) * 2.0f;
        float const m23 = (q.w * q.x + q.z * q.y) * 2.0f;
        float const m33 = 1.0f - (q.y * q.y + q.x * q.x) * 2.0f;

        int wheelCount = 0;
        for (auto& wheelInfo : m_wheels)
        {
            ai::Wheel* wheel = wheelInfo.GetWheel();
            if (!wheel)
            {
                continue;
            }
            ++wheelCount;

            CVector const aVel = wheel->GetAngularVelocity();
            float const localX = aVel.y * m21 + aVel.z * m31 + m11 * aVel.x;
            float const localZ = aVel.y * m23 + aVel.z * m33 + m13 * aVel.x;
            int const sign = localX < 0.0f ? -1 : 1;
            m_averageWheelAVel = sqrt(localZ * localZ + localX * localX) * sign + m_averageWheelAVel;
        }
        if (wheelCount > 0)
        {
            m_averageWheelAVel = m_averageWheelAVel / static_cast<float>(wheelCount);
        }

        // rad/s at the wheels to engine RPM through the gear and differential.
        m_engineRpm = GEAR_RATIOS[m_currentGear] * m_diffRatio * 108.0f * m_averageWheelAVel * 0.15915494f;
    }

    int Vehicle::_UpdateRepositoryOnChangeBasket()
    {
        // RVA 0x5DD430 - a basket gets a repository shaped by its slots; without one the repository's contents
        // go to the ground repository and it is destroyed.
        auto part = GetPartByName(BASKET);
        if (part && IS_KIND_OF(part, Basket))
        {
            auto* basket = RT_DYNCAST(part, Basket);
            auto* protoInfo = basket->GetPrototypeInfo();
            if (!protoInfo)
            {
                return 0;
            }

            if (!m_repository)
            {
                m_repository = RT_DYNCAST(M3D_KERNEL->New("IzvratRepository"), IzvratRepository);
            }
            if (!m_repository)
            {
                return 0;
            }

            auto const& size = protoInfo->GetRepositorySize();
            m_repository->SetGeomSize(size);

            auto const& slotPositions = protoInfo->GetSlotPositions();
            for (auto const& slot : slotPositions)
            {
                auto const& bounds = protoInfo->GetSlotBounds(slot.first, true);
                if (bounds.width || bounds.height)
                {
                    m_repository->SnapPiece(bounds);
                }
            }

            return 1;
        }

        if (!m_repository)
        {
            return 1;
        }

        m_repository->TransferToRepository(m_groundRepository);
        delete m_repository;
        m_repository = nullptr;
        return 1;
    }

    void Vehicle::_UpdatePhysicsUpdater()
    {
        auto* playerVehicle = gDynamicScene->GetVehicleControlledByPlayer();
        if (playerVehicle && playerVehicle != this)
        {
            auto* trailer = theObjects->GetEntityByObjId(playerVehicle->m_trailerObjId);
            if (trailer != this)
            {
                auto vehiclePos = playerVehicle->GetPosition();
                auto thisPos = GetPosition();
                CVector dist = thisPos - vehiclePos;
                auto distValue = dist.length();
                if (theGlobProp.m_distToTurnOnPhysics <= distValue)
                {
                    if (distValue > ai::theGlobProp.m_distToTurnOffPhysics)
                        this->SetUpdatingByODE(false);
                }
                else
                {
                    this->SetUpdatingByODE(true);
                }
            }
        }
    }

    void Vehicle::_AdjustSizeAndBumperPoint()
    {
        // RVA 0x5DD610 - the vehicle's size is the box around all its parts' boxes (in node space), and the
        // bumper point and the look/target boxes are derived from it.
        // NOTE: the box starts as the zero box rather than an empty one, so it always contains the origin.
        Aabb box;
        for (int i = 0; i < 6; ++i)
        {
            box.m_box[i] = 0.0f;
        }

        auto const embracePart = [&box](VehiclePart* part) {
            CVector const pos = part->GetNodeRelativePosition();
            CVector const halfSize = part->GetSize() * 0.5f;
            Aabb partBox;
            partBox.m_box[0] = pos.x - halfSize.x;
            partBox.m_box[1] = pos.y - halfSize.y;
            partBox.m_box[2] = pos.z - halfSize.z;
            partBox.m_box[3] = pos.x + halfSize.x;
            partBox.m_box[4] = pos.y + halfSize.y;
            partBox.m_box[5] = pos.z + halfSize.z;
            box.EmbraceBox(partBox);
        };

        for (auto const& part : m_vehicleParts)
        {
            if (IS_KIND_OF(part.second, CompoundVehiclePart))
            {
                for (auto const& subPart : *static_cast<CompoundVehiclePart*>(part.second))
                {
                    embracePart(subPart.second.vp);
                }
            }
            else
            {
                embracePart(part.second);
            }
        }

        m_size = CVector(box.m_box[3] - box.m_box[0], box.m_box[4] - box.m_box[1], box.m_box[5] - box.m_box[2]);
        m_bumperPoint = CVector(0.0f, m_size.y * 0.2f, m_size.z * 0.5f + 0.1f);
        m_lookBox->SetSize(CVector(m_size.x * 1.5f, m_size.y * 5.0f, ai::theGlobProp.m_defaultLookBoxLength));
        m_targetBox->SetSize(CVector(m_size.x * 1.5f, m_size.y * 5.0f, ai::theGlobProp.m_defaultTargetBoxLength));
    }

    void Vehicle::_OnChangeBasket()
    {
        // RVA 0x5E3010
        _ValidateVehicleParts();
        _UpdateRepositoryOnChangeBasket();
    }

    CVector Vehicle::_GetLastPathPoint() const
    {
        // RVA 0x5CD150
        CVector lastPoint = ZeroVector;
        if (m_pPath)
        {
            GetPathItem(m_pPath, m_pPath->GetSize() - 1, lastPoint);
        }
        return lastPoint;
    }

    float Vehicle::_GetAngleTo(CVector const& point) const
    {
        // RVA 0x5D1830
        // Signed angle between the vehicle's forward axis and the direction to the point:
        // positive when the point is to the right (local +X), negative to the left.
        auto const toPoint = point - GetPosition();
        if (sqrt(toPoint.z * toPoint.z + toPoint.y * toPoint.y + toPoint.x * toPoint.x) < 0.0099999998)
        {
            return 0.0f;
        }
        float const invLen = 1.0 / sqrt(toPoint.z * toPoint.z + toPoint.y * toPoint.y + toPoint.x * toPoint.x + 0.00000011920929);
        CVector const dir(invLen * toPoint.x, toPoint.y * invLen, toPoint.z * invLen);

        // Bring the direction into the vehicle's local frame with the rotation matrix of the
        // inverse orientation (the original inlines the quaternion-to-matrix conversion).
        auto const q = GetRotation().getInversed();
        float const m11 = 1.0f - (q.z * q.z + q.y * q.y) * 2.0f;
        float const m21 = (q.x * q.y - q.w * q.z) * 2.0f;
        float const m31 = (q.w * q.y + q.x * q.z) * 2.0f;
        float const m13 = (q.x * q.z - q.w * q.y) * 2.0f;
        float const m23 = (q.w * q.x + q.z * q.y) * 2.0f;
        float const m33 = 1.0f - (q.y * q.y + q.x * q.x) * 2.0f;

        float const side = m31 * dir.z + m21 * dir.y + m11 * dir.x;
        float cosAngle = m33 * dir.z + m23 * dir.y + m13 * dir.x;
        if (cosAngle < -0.99999899f)
        {
            cosAngle = -0.99999899f;
        }
        else if (cosAngle > 0.99999899f)
        {
            cosAngle = 0.99999899f;
        }

        int const sign = side < 0.0f ? -1 : 1;
        return acos(cosAngle) * sign;
    }

    void Vehicle::_AdjustWheel(WheelRuntimeInfo& wheelInfo)
    {
        auto m_wheel = wheelInfo.GetWheel();

        auto pos = GetPosition();
        auto rot = GetRotation();
        auto invRot = rot.getInversed();

        auto v5 = m_wheel->GetDirection();
        auto xy = invRot.y * invRot.x;
        auto xy_2 = invRot.y * invRot.x;
        auto yz = invRot.z * invRot.y;
        auto yz_2 = invRot.z * invRot.y;
        auto xx = invRot.x * invRot.x;
        auto xx_2 = invRot.x * invRot.x;
        auto wy = invRot.w * invRot.y;

        CMatrix vv;
        vv._11 = 1.0 - (float)((float)((float)(invRot.z * invRot.z) + (float)(invRot.y * invRot.y)) * 2.0);
        vv._21 = (float)((float)(invRot.y * invRot.x) - (float)(invRot.w * invRot.z)) * 2.0;
        auto wz = invRot.w * invRot.z;
        auto zz = invRot.z * invRot.z;
        auto& v6 = v5;
        auto yy = invRot.y * invRot.y;
        auto xz = invRot.z * invRot.x;
        auto wx = invRot.w * invRot.x;
        vv._31 = (float)((float)(invRot.w * invRot.y) + (float)(invRot.z * invRot.x)) * 2.0;
        vv._12 = (float)((float)(invRot.w * invRot.z) + (float)(invRot.y * invRot.x)) * 2.0;
        vv._22 = 1.0 - (float)((float)((float)(invRot.z * invRot.z) + (float)(invRot.x * invRot.x)) * 2.0);
        vv._33 = 1.0 - (float)((float)((float)(invRot.y * invRot.y) + (float)(invRot.x * invRot.x)) * 2.0);
        vv._32 = (float)((float)(invRot.z * invRot.y) - (float)(invRot.w * invRot.x)) * 2.0;
        vv._13 = (float)((float)(invRot.z * invRot.x) - (float)(invRot.w * invRot.y)) * 2.0;
        vv._23 = (float)((float)(invRot.w * invRot.x) + (float)(invRot.z * invRot.y)) * 2.0;
        vv._14 = 0.0;
        vv._24 = 0.0;
        memset(&vv.m[2][3], 0, 16);
        vv._44 = 1.0;

        auto v66 = vv;
        auto v7 = v66._22 * v6.y + v66._32 * v6.z + v66._12 * v6.x;
        auto v8 = (float)(v6.y * v66._23) + (float)(v6.z * v66._33);
        auto v9 = v66._13 * v6.x;

        CVector axis;
        axis.x = (float)((float)(v6.y * v66._21) + (float)(v6.z * v66._31)) + (float)(v6.x * v66._11);
        auto v10 = v8 + v9;
        yz_2 = 1.0 / sqrt((float)(0.0 - axis.x) * (float)(0.0 - axis.x) + (float)(v10 * v10) + 0.00000011920929);
        auto v11 = atan2(v7, sqrt(axis.x * axis.x + (float)(v10 * v10))) * 0.5;
        auto v12 = (float)(0.0 - axis.x) * yz_2;
        auto v13 = yz_2 * v10;
        auto v14 = yz_2 * 0.0;
        yz_2 = sin(v11);

        CVector wheelDir;
        wheelDir.z = v12 * yz_2;
        wheelDir.y = v14 * yz_2;
        wheelDir.x = yz_2 * v13;

        Quaternion v50;
        v50.x =
            (float)((float)(invRot.x + (float)(invRot.z * 0.0)) + (float)(invRot.w * 0.0)) - (float)(invRot.y * 0.0);
        v50.y =
            (float)((float)(invRot.y + (float)(invRot.x * 0.0)) + (float)(invRot.w * 0.0)) - (float)(invRot.z * 0.0);
        axis.y = v50.y;
        v50.z =
            (float)((float)(invRot.z + (float)(invRot.y * 0.0)) + (float)(invRot.w * 0.0)) - (float)(invRot.x * 0.0);
        v50.w =
            (float)((float)(invRot.w - (float)(invRot.x * 0.0)) - (float)(invRot.y * 0.0)) - (float)(invRot.z * 0.0);
        axis.x = v50.x;
        axis.z = v50.z;
        auto w = v50.w;
        auto v48 = cos(v11);
        auto Rotation = m_wheel->GetRotation();
        v50.x =
            (float)((float)((float)(Rotation.x * w) + (float)(axis.y * Rotation.z)) + (float)(axis.x * Rotation.w)) -
            (float)(axis.z * Rotation.y);
        v50.y =
            (float)((float)((float)(Rotation.x * axis.z) + (float)(axis.y * Rotation.w)) + (float)(w * Rotation.y)) -
            (float)(axis.x * Rotation.z);
        auto v16 = w * Rotation.w;
        auto v17 = axis.z * Rotation.z;
        auto v18 = axis.y * Rotation.y;
        v50.z =
            (float)((float)((float)(Rotation.y * axis.x) + (float)(w * Rotation.z)) + (float)(axis.z * Rotation.w)) -
            (float)(Rotation.x * axis.y);
        auto v19 = (float)((float)(v16 - (float)(Rotation.x * axis.x)) - v18) - v17;
        axis.y = v50.y;
        v50.w = v19;
        axis.x = v50.x;
        axis.z = v50.z;
        w = v19;
        auto Inversed = wheelInfo.m_initialRot.getInversed();
        v50.x =
            (float)((float)((float)(axis.y * Inversed.z) + (float)(v19 * Inversed.x)) + (float)(axis.x * Inversed.w)) -
            (float)(axis.z * Inversed.y);
        v50.y =
            (float)((float)((float)(axis.y * Inversed.w) + (float)(axis.z * Inversed.x)) + (float)(v19 * Inversed.y)) -
            (float)(axis.x * Inversed.z);
        auto v21 = axis.z * Inversed.z;
        auto v22 = v19 * Inversed.w;
        auto v23 = axis.y * Inversed.y;
        v50.z =
            (float)((float)((float)(Inversed.y * axis.x) + (float)(w * Inversed.z)) + (float)(axis.z * Inversed.w)) -
            (float)(axis.y * Inversed.x);
        v50.w = (float)((float)(v22 - (float)(Inversed.x * axis.x)) - v23) - v21;
        auto v24 = (float)((float)((float)(rot.y * wheelDir.z) + (float)(v48 * rot.x)) + (float)(rot.w * wheelDir.x)) -
            (float)(rot.z * wheelDir.y);
        auto v25 = (float)((float)((float)(rot.y * v48) + (float)(rot.w * wheelDir.y)) + (float)(rot.z * wheelDir.x)) -
            (float)(wheelDir.z * rot.x);
        axis.x = v50.x;
        w = v50.w;
        axis.z = v50.z;
        auto v26 = (float)((float)((float)(rot.z * v48) + (float)(rot.w * wheelDir.z)) + (float)(wheelDir.y * rot.x)) -
            (float)(rot.y * wheelDir.x);
        axis.y = v50.y;
        auto v27 = (float)((float)((float)(v25 * v50.z) + (float)(v50.w * v24)) +
                           (float)((float)((float)((float)((float)(rot.w * v48) - (float)(rot.x * wheelDir.x)) -
                                                   (float)(rot.y * wheelDir.y)) -
                                           (float)(rot.z * wheelDir.z)) *
                                   v50.x)) -
            (float)(v26 * v50.y);
        auto v28 = (float)((float)((float)(v25 * v50.w) +
                                   (float)((float)((float)((float)((float)(rot.w * v48) - (float)(rot.x * wheelDir.x)) -
                                                           (float)(rot.y * wheelDir.y)) -
                                                   (float)(rot.z * wheelDir.z)) *
                                           v50.y)) +
                           (float)(v26 * v50.x)) -
            (float)(v50.z * v24);
        auto v29 = (float)((float)((float)((float)((float)((float)((float)(rot.w * v48) - (float)(rot.x * wheelDir.x)) -
                                                           (float)(rot.y * wheelDir.y)) -
                                                   (float)(rot.z * wheelDir.z)) *
                                           v50.w) -
                                   (float)(v24 * v50.x)) -
                           (float)(v25 * v50.y)) -
            (float)(v26 * v50.z);
        auto v30 = (float)((float)((float)(v26 * v50.w) +
                                   (float)((float)((float)((float)((float)(rot.w * v48) - (float)(rot.x * wheelDir.x)) -
                                                           (float)(rot.y * wheelDir.y)) -
                                                   (float)(rot.z * wheelDir.z)) *
                                           v50.z)) +
                           (float)(v50.y * v24)) -
            (float)(v25 * v50.x);
        v50.x = (float)((float)((float)(v29 * wheelInfo.m_initialRot.x) + (float)(wheelInfo.m_initialRot.w * v27)) +
                        (float)(wheelInfo.m_initialRot.z * v28)) -
            (float)(v30 * wheelInfo.m_initialRot.y);
        auto z = wheelInfo.m_initialRot.z;
        v50.y = (float)((float)((float)(v29 * wheelInfo.m_initialRot.y) + (float)(v30 * wheelInfo.m_initialRot.x)) +
                        (float)(wheelInfo.m_initialRot.w * v28)) -
            (float)(wheelInfo.m_initialRot.z * v27);
        auto v32 = (float)((float)(v27 * wheelInfo.m_initialRot.y) + (float)(z * v29)) +
            (float)(wheelInfo.m_initialRot.w * v30);
        auto v33 = v28 * wheelInfo.m_initialRot.x;
        auto v34 = v28 * wheelInfo.m_initialRot.y;
        auto v35 = v32 - v33;
        auto x = wheelInfo.m_initialRot.x;
        v50.z = v35;
        v50.w = (float)((float)((float)(wheelInfo.m_initialRot.w * v29) - (float)(x * v27)) - v34) -
            (float)(wheelInfo.m_initialRot.z * v30);
        m_wheel->SetRotation(v50);

        auto Position = m_wheel->GetPosition();
        wheelDir.x = Position.x - pos.x;
        wheelDir.y = Position.y - pos.y;
        wheelDir.z = Position.z - pos.z;
        vv._11 = 1.0 - (float)((float)(zz + yy) * 2.0);
        vv._21 = (float)(xy_2 - wz) * 2.0;
        vv._31 = (float)(wy + xz) * 2.0;
        vv._12 = (float)(wz + xy_2) * 2.0;
        vv._22 = 1.0 - (float)((float)(zz + xx_2) * 2.0);
        vv._32 = (float)(yz - wx) * 2.0;
        vv._33 = 1.0 - (float)((float)(yy + xx_2) * 2.0);
        vv._13 = (float)(xz - wy) * 2.0;
        vv._23 = (float)(wx + yz) * 2.0;
        vv._14 = 0.0;
        vv._24 = 0.0;
        memset(&vv.m[2][3], 0, 16);
        vv._44 = 1.0;

        v66 = vv;
        wheelDir.y =
            (float)((float)(v66._32 * wheelDir.z) + (float)(v66._22 * wheelDir.y)) + (float)(v66._12 * wheelDir.x);
        wheelDir.x = wheelInfo.m_initialPos.x;
        wheelDir.z = wheelInfo.m_initialPos.z;
        xx = rot.x * rot.x;
        auto y = rot.y;
        yz_2 = rot.y * rot.x;
        xy = rot.z * rot.y;
        vv._11 = 1.0 - (float)((float)((float)(rot.z * rot.z) + (float)(y * y)) * 2.0);
        vv._21 = (float)((float)(rot.y * rot.x) - (float)(rot.z * rot.w)) * 2.0;
        vv._31 = (float)((float)(rot.y * rot.w) + (float)(rot.z * rot.x)) * 2.0;
        vv._12 = (float)((float)(rot.z * rot.w) + (float)(rot.y * rot.x)) * 2.0;
        vv._22 = 1.0 - (float)((float)((float)(rot.z * rot.z) + (float)(rot.x * rot.x)) * 2.0);
        vv._33 = 1.0 - (float)((float)((float)(y * y) + (float)(rot.x * rot.x)) * 2.0);
        vv._32 = (float)((float)(rot.z * rot.y) - (float)(rot.w * rot.x)) * 2.0;
        vv._13 = (float)((float)(rot.z * rot.x) - (float)(rot.y * rot.w)) * 2.0;
        vv._23 = (float)((float)(rot.w * rot.x) + (float)(rot.z * rot.y)) * 2.0;
        vv._14 = 0.0;
        vv._24 = 0.0;
        memset(&vv.m[2][3], 0, 16);
        vv._44 = 1.0;

        v66 = vv;
        axis.x = (float)((float)((float)(v66._21 * wheelDir.y) + (float)(v66._31 * wheelDir.z)) +
                         (float)(v66._11 * wheelDir.x)) +
            pos.x;
        axis.y = pos.y +
            (float)((float)((float)(v66._22 * wheelDir.y) + (float)(v66._32 * wheelDir.z)) +
                    (float)(v66._12 * wheelDir.x));
        axis.z = pos.z +
            (float)((float)((float)(v66._23 * wheelDir.y) + (float)(v66._33 * wheelDir.z)) +
                    (float)(v66._13 * wheelDir.x));
        m_wheel->SetPosition(axis);

        auto v40 = m_wheel->GetDirection();
        vv._11 = 1.0 - (float)((float)(zz + yy) * 2.0);
        vv._21 = (float)(xy_2 - wz) * 2.0;
        vv._12 = (float)(wz + xy_2) * 2.0;
        auto& v41 = v40;
        vv._31 = (float)(wy + xz) * 2.0;
        vv._22 = 1.0 - (float)((float)(zz + xx_2) * 2.0);
        vv._32 = (float)(yz - wx) * 2.0;
        vv._33 = 1.0 - (float)((float)(yy + xx_2) * 2.0);
        vv._13 = (float)(xz - wy) * 2.0;
        vv._23 = (float)(wx + yz) * 2.0;
        vv._14 = 0.0;
        vv._24 = 0.0;
        memset(&vv.m[2][3], 0, 16);
        vv._44 = 1.0;

        v66 = vv;
        auto v42 = (float)((float)(v41[0] * v66._11) + (float)(v66._21 * v41[1])) + (float)(v66._31 * v41[2]);
        auto v43 = (float)((float)(v66._12 * v41[0]) + (float)(v66._22 * v41[1])) + (float)(v66._32 * v41[2]);
        wheelDir.z = (float)((float)(v66._13 * v41[0]) + (float)(v66._23 * v41[1])) + (float)(v66._33 * v41[2]);
        axis.z = wheelDir.z;
        wheelDir.x = v42;
        axis.x = v42;
        wheelDir.y = v43;
        axis.y = v43;
        auto angle = m_wheel->m_curAngle - atan2(-wheelDir.z, -v42);
        _TurnWheelByAngle(m_wheel, angle);
    }

    m3d::Object* Vehicle::CreateObject()
    {
        // RVA 0x5D5490
        // The shipped code raises a SysError ("Object cannot be created directly")
        // and returns null - a Vehicle only comes from its prototype.
        return nullptr;
    }

    void Vehicle::_CheckForNearbyChests() const
    {
        // RVA 0x5E93E0: raise the pick-up hint the first frame chests come into
        // range, and clear it the first frame they all leave.
        if (!m_bAllowPickUpMessage)
        {
            return;
        }
        if (m_currentNumNearbyChests)
        {
            if (m_pastNumNearbyChests)
            {
                return;
            }
            M3D_APP->EnqueueMessage(66567, 1, 0, 0, 0, {}, {});
            M3D_APP->EnqueueMessage(66563, 9, 0, 0, 0, {}, {});
        }
        else
        {
            if (!m_pastNumNearbyChests)
            {
                return;
            }
            M3D_APP->EnqueueMessage(66567, 0, 0, 0, 0, {}, {});
        }
        m_bAllowPickUpMessage = false;
    }

    void Vehicle::_AdjustTrailer()
    {
        // RVA 0x5DDB50: point the trailer's steering back at the towing vehicle.
        if (auto* trailer = static_cast<Vehicle*>(theObjects->GetEntityByObjId(m_trailerObjId)))
        {
            trailer->m_steerRadians = -trailer->_GetAngleTo(GetPosition());
        }
    }

    void Vehicle::_UpdateSeenObjAndWeapons(float elapsedTime)
    {
        // RVA 0x5DDDE0 - the player's aiming: finds the vehicle or static gun under the mouse
        // cursor and turns the weapons towards the hit point (or far along the camera's view).
        if (!M3D_APP->bIsMousePointing())
        {
            return;
        }

        m_seenObjId = -1;

        CVector lookAt;
        m3d::SgNode* seenNode = nullptr;
        if (M3D_APP->GetMouseHitPoint(lookAt, seenNode))
        {
            PhysicBody* body = nullptr;
            if (seenNode)
            {
                seenNode->GetProperty(m3d::PROP_NODE_PHYSICBODY, &body);
                if (body)
                {
                    m_seenObjId = body->GetOwnerId();
                }
            }
        }

        // A wheel counts as the vehicle it belongs to.
        Obj* seenObj = theObjects->GetEntityByObjId(m_seenObjId);
        if (seenObj && IS_KIND_OF(seenObj, Wheel))
        {
            Vehicle* vehicle = static_cast<Wheel*>(seenObj)->GetVehicle();
            seenObj = vehicle;
            m_seenObjId = vehicle ? vehicle->GetId() : -1;
        }

        if (m_seenObjId == GetId())
        {
            m_seenObjId = -1;
            seenNode = nullptr;
        }

        if (!seenNode)
        {
            // Nothing (else) under the cursor: aim far along the camera's forward axis.
            CMatrix mat;
            mat.rotYPR(M3D_APP->m_curCamera.m_rotYaw, M3D_APP->m_curCamera.m_rotPitch, M3D_APP->m_curCamera.m_rotRoll);

            CVector const forward(0.0f, 0.0f, 1.0f); // INITIAL_OBJECTS_DIRECTION
            lookAt.x = (mat._13 * forward.z + mat._11 * forward.x + forward.y * mat._12) * 1000000.0f;
            lookAt.y = (mat._23 * forward.z + mat._22 * forward.y + mat._21 * forward.x) * 1000000.0f;
            lookAt.z = (mat._33 * forward.z + mat._32 * forward.y + mat._31 * forward.x) * 1000000.0f;
        }

        if (!seenObj || !IS_KIND_OF(seenObj, Vehicle) && !IS_KIND_OF(seenObj, StaticAutoGun))
        {
            m_seenObjId = -1;
            seenNode = nullptr;
        }

        WeaponFirer::WeaponLookAtPoint(this, lookAt, elapsedTime);
        m_curLookAt = lookAt;
    }

    CVector Vehicle::_CalcSteeringForce(float elapsedTime) const
    {
        // RVA 0x5E12C0 - the direction (of length at most 1) the vehicle wants to drive in: pulled
        // along its path, towards an external destination and by the team's attack formation,
        // pushed away from nearby objects. Computed once per frame, then cached. Also runs the
        // stuck check: if the vehicle has moved less than sqrt(0.1) over a second of steady
        // steering, m_bWasStuck is set for 2 seconds.
        if (m_bCurSteeringForceValid)
        {
            return m_curSteeringForce;
        }

        CVector const pos = GetPosition();
        CVector const vel = GetLinearVelocity();
        CVector guide;
        if (vel.z * vel.z + vel.y * vel.y + vel.x * vel.x <= 1.0f)
        {
            guide = GetDirection();
        }
        else
        {
            guide = vel;
        }

        CVector attraction = ZeroVector;
        CVector curPoint = ZeroVector;
        if (GetPathItem(m_pPath, m_pathNum, curPoint))
        {
            CVector const nextPoint = _GetNextPathPoint();
            CVector const force = _CalcSteeringForceToPathPoint(curPoint, nextPoint);
            attraction.x = force.x + attraction.x;
            attraction.y = force.y + attraction.y;
            attraction.z = force.z + attraction.z;
        }
        if (m_moveStatus == MOVE_MOVING_BY_STEERING_FORCE)
        {
            curPoint = GetGroundPos(m_externalDestination, false, false);
            CVector const force = _CalcSteeringForceToPathPoint(m_externalDestination, m_externalDestination);
            attraction.x = force.x + attraction.x;
            attraction.y = force.y + attraction.y;
            attraction.z = force.z + attraction.z;
        }
        if (m_attackStatus == ATTACK_ATTACKING)
        {
            // NOTE: the parent is taken to be a team without a type check.
            if (Team* team = static_cast<Team*>(GetParent()))
            {
                auto const& steeringForces = team->GetSteeringForceMap();
                auto const it = steeringForces.find(GetId());
                if (it != steeringForces.end())
                {
                    attraction.x = attraction.x + it->second.x;
                    attraction.y = it->second.y + attraction.y;
                    attraction.z = it->second.z + attraction.z;
                }
            }
        }

        CVector const predictedPos(
            pos.x + vel.x * theGlobProp.m_predictionTime,
            pos.y + vel.y * theGlobProp.m_predictionTime,
            pos.z + vel.z * theGlobProp.m_predictionTime);
        float repulsionX = 0.0f;
        float repulsionY = 0.0f;
        float repulsionZ = 0.0f;
        if (!m_bIsMovingAlongExternalPath)
        {
            _AdjustLookBox(true, pos, curPoint, guide);
            CVector const repulsion1 = _CalcRepulsionForNearbyObjects(pos, predictedPos, vel, guide, true, attraction);
            _AdjustLookBox(false, pos, curPoint, guide);
            CVector const repulsion2 = _CalcRepulsionForNearbyObjects(pos, predictedPos, vel, guide, false, attraction);
            repulsionX = repulsion2.x + repulsion1.x;
            repulsionY = repulsion2.y + repulsion1.y;
            repulsionZ = repulsion2.z + repulsion1.z;
        }

        // Normalize the sum, keeping its length when it is shorter than 1.
        float const forceX = repulsionX + attraction.x;
        float const forceY = repulsionY + attraction.y;
        float const forceZ = repulsionZ + attraction.z;
        float const lenSq = forceZ * forceZ + forceY * forceY + forceX * forceX;
        float const len = static_cast<float>(sqrt(lenSq));
        float scale;
        if (0.0f > len)
        {
            scale = 0.0f;
        }
        else
        {
            scale = len;
            if (len > 1.0f)
            {
                scale = 1.0f;
            }
        }
        float const invLen = static_cast<float>(1.0 / sqrt(lenSq + 0.00000011920929f));
        CVector const prevForce = m_curSteeringForce;
        m_curSteeringForce.x = invLen * forceX * scale;
        m_curSteeringForce.y = forceY * invLen * scale;
        m_curSteeringForce.z = forceZ * invLen * scale;

        // The stuck check. Its timer restarts whenever the steering turns by more than 90 degrees.
        m_timeOutToCheckStuck = m_timeOutToCheckStuck - elapsedTime;
        bool const wasStuck = m_bWasStuck;
        if (!wasStuck &&
            0.0f > m_curSteeringForce.z * prevForce.z + m_curSteeringForce.y * prevForce.y + prevForce.x * m_curSteeringForce.x)
        {
            m_timeOutToCheckStuck = 1.0f;
        }
        if (0.0f > m_timeOutToCheckStuck)
        {
            if (wasStuck)
            {
                m_bWasStuck = false;
                m_timeOutToCheckStuck = 1.0f;
            }
            else
            {
                float const dz = pos.z - m_prevPosToCheckStuck.z;
                float const dy = pos.y - m_prevPosToCheckStuck.y;
                float const dx = pos.x - m_prevPosToCheckStuck.x;
                if (0.1f > dz * dz + dy * dy + dx * dx)
                {
                    m_bWasStuck = true;
                    m_timeOutToCheckStuck = 2.0f;
                }
                else
                {
                    m_prevPosToCheckStuck = pos;
                    m_timeOutToCheckStuck = 1.0f;
                }
            }
        }

        m_bCurSteeringForceValid = true;
        return m_curSteeringForce;
    }

    CVector Vehicle::_GetEtalonWheelAVel() const
    {
        // RVA 0x5D7260 - the shipped build only asserts.
        _assert("0", "e:\\Builders\\ExMachina\\tmpBuildDir5084\\truxx\\Server\\Objects\\Vehicle.cpp", 7182);
        return ZeroVector;
    }

    void Vehicle::_DropChests()
    {
        // RVA 0x5E3EB0 - a destroyed vehicle may leave a chest with some of its cargo and guns.
        if (!theGlobProp.m_vehiclesDropChests)
        {
            return;
        }

        int const chestPrototypeId = thePrototypeManager->GetPrototypeId(CStr("vanishingChest"));
        int const chestId = theObjects->CreateNewObject(chestPrototypeId, "", -1, -1);
        // NOTE: the chest is used without a null check.
        auto* const chest = static_cast<SimplePhysicObj*>(theObjects->GetEntityByObjId(chestId));

        // Behind the wreck, lifted by the chest's own height, then tossed upwards.
        float const backOffset = m_size.z * 0.60000002f + 1.0f;
        CVector const dir = GetDirection();
        CVector const offset(backOffset * dir.x, dir.y * backOffset, dir.z * backOffset);
        CVector const position = GetPosition();
        CVector posForChest(position.x - offset.x, position.y - offset.y, position.z - offset.z);
        Aabb const chestAabb = static_cast<Geom*>(chest->GetPhysicBody()->m_pGeoms[0])->GetAabb();
        posForChest.y = chestAabb.m_box[4] - chestAabb.m_box[1] + posForChest.y;
        chest->SetPosition(posForChest);

        float const throwSpeed = static_cast<float>(rand()) * 0.00015259255f + 2.0f;
        CVector const throwDir = GetRandomDeviatedVector(CVector(0.0f, 1.0f, 0.0f), 1.5707964f);
        chest->SetLinearVelocity(CVector(throwDir.x * throwSpeed, throwDir.y * throwSpeed, throwDir.z * throwSpeed));
        chest->SetAutoDisabling(true, 0.1f, 0.1f, 5);

        bool added = false;
        if (m_repository)
        {
            for (unsigned int slot = 0; slot < m_repository->GetNumItems(); ++slot)
            {
                auto* const killsStatistic = theStatisticManager->GetStatistic(
                    STATISTIC_VEHICLE_KILLED + pServer->GetWorld()->m_level->m_levelName, CStr("IntStatistic"));
                killsStatistic->m_bGlobalFlag = false;
                int kills = 0;
                {
                    CStr const value = killsStatistic->GetValue();
                    if (value.c_str() && strlen(value.c_str()) != 0)
                    {
                        sscanf(value.c_str(), "%d", &kills);
                    }
                }

                // NOTE: the ratio sqrt(level size) / kills is only tested for being positive; the chance is then
                // clamped from the kill count itself, so it is 0.2 before the first kill on the level and 1 after.
                float const killsF = static_cast<float>(kills);
                float dropProbability = 1.0f;
                if (sqrt(pServer->GetLevelSize()) / killsF > 0.0)
                {
                    dropProbability = killsF;
                    if (killsF < 0.2f)
                    {
                        dropProbability = 0.2f;
                    }
                    else if (killsF > 1.0f)
                    {
                        dropProbability = 1.0f;
                    }
                }

                if (theGlobProp.m_probabilityToDropArticlesFromDeadVehicles * dropProbability >
                    static_cast<float>(rand()) * 0.000030518509f * 0.99000001f)
                {
                    int const objId = m_repository->GetItem(slot).GetObjId();
                    if (objId != -1)
                    {
                        m_repository->GiveUpThingFromSlotUnsafe(slot, 1);
                        chest->AddChild(theObjects->GetEntityByObjId(objId));
                        added = true;
                    }
                }
            }
            m_repository->Purge();
        }

        // Each gun shown in the encyclopedia may go into the chest; the chance is rolled for every part.
        retruxx::vector<CStr> partsToPutInChest;
        for (auto const& [name, part] : m_vehicleParts)
        {
            if (theGlobProp.m_probabilityToDropGunsFromDeadVehicles > static_cast<float>(rand()) * 0.000030518509f * 0.99000001f &&
                (part->IsKindOf(RT_CLASS_LOCAL(Gun)) || part->IsKindOf(RT_CLASS_LOCAL(CompoundGun))) &&
                part->GetPrototypeInfo()->m_bVisibleInEncyclopedia)
            {
                partsToPutInChest.push_back(name);
                added = true;
            }
        }

        // A dropped gun is worn down to 20-50% of its durability.
        for (CStr const& partName : partsToPutInChest)
        {
            VehiclePart* const part = GetPartByName(partName);
            chest->AddChild(static_cast<Obj*>(part));
            SetPartByName(partName, nullptr, false);
            float const maxDurability = part->m_durability.maxValue().get();
            part->m_durability.value().set((static_cast<float>(rand()) * 0.000030518509f * 0.30000001f + 0.2f) * maxDurability);
        }

        if (!added)
        {
            chest->Remove();
        }
    }

    void Vehicle::_EvaluateToDead()
    {
        // RVA 0x5E88E0
        CStr const& levelName = pServer->GetWorld()->m_level->m_levelName;
        if (m_bIsControlledByPlayer)
        {
            auto* deaths = static_cast<IntStatistic*>(theStatisticManager->GetStatistic(STATISTIC_DEATH_COUNTER, "IntStatistic"));
            deaths->SetGlobalFlag(true);
            deaths->Increase(1);
            auto* levelDeaths =
                static_cast<IntStatistic*>(theStatisticManager->GetStatistic(STATISTIC_DEATH_COUNTER + levelName, "IntStatistic"));
            levelDeaths->SetGlobalFlag(false);
            levelDeaths->Increase(1);
        }

        if (Vehicle* const playerVehicle = thePlayer->GetVehicle())
        {
            if (GetLastDamageSource() == playerVehicle->GetId())
            {
                DynamicQuestManager::ConsiderPlayerKill(GetBelong());
                auto* kills = static_cast<IntStatistic*>(theStatisticManager->GetStatistic(STATISTIC_VEHICLE_KILLED, "IntStatistic"));
                kills->SetGlobalFlag(true);
                kills->Increase(1);
                auto* levelKills =
                    static_cast<IntStatistic*>(theStatisticManager->GetStatistic(STATISTIC_VEHICLE_KILLED + levelName, "IntStatistic"));
                levelKills->SetGlobalFlag(false);
                levelKills->Increase(1);
            }
        }

        // Nothing regenerates on a wreck.
        VehiclePart* const chassisPart = GetPartByName(CHASSIS);
        if (chassisPart && chassisPart->IsKindOf(RT_CLASS_LOCAL(Chassis)))
        {
            static_cast<Chassis*>(chassisPart)->Health().regeneration().set(0.0f);
        }
        for (auto const& [name, part] : m_vehicleParts)
        {
            if (!part)
            {
                continue;
            }
            if (part->IsKindOf(RT_CLASS_LOCAL(CompoundVehiclePart)))
            {
                static_cast<CompoundVehiclePart*>(part)->SetDurabilityRegeneration(0.0f);
            }
            else
            {
                part->m_durability.regeneration().set(0.0f);
            }
        }

        // One of up to three variants of the explosion for the killing damage type: "name", "name1" or "name2".
        CStr effectName(m_destroyEffectNames[m_lastDamage]);
        int const variant = 3 * rand() / 0x8000;
        if (variant > 0)
        {
            effectName += CStr(variant);
        }
        PhysicBody::CreateEffectNode(effectName, GetPosition(), GetRotation(), true, 1.0f);
        _CreateBlastWave();
        WeaponFirer::FireFromWeaponsIfPossible(this, false, ZeroVector, nullptr);
        if (bIsContoured())
        {
            RemoveContour();
        }
        _DropChests();
        SetSkin(8);
        m_deathDamage = m_lastDamage;
        if (m_engineHighSoundNode)
        {
            m3d::SgNode* node = m_engineHighSoundNode;
            m_engineHighSoundNode->GetGraph()->RemoveNode(node);
            m_engineHighSoundNode = nullptr;
        }

        // One of the two death animations (8 or 9), picked at random.
        auto const randomDeathAction = []() { return 2 * rand() / 0x8000 == 1 ? 8 : 9; };

        if (!bIsUpdatingByODE())
        {
            Remove();
        }
        else if (m_lastDamage == DAMAGE_PIERCING)
        {
            // Thrown up; the part that took the last hit flies off or bursts, everything else plays its death animation.
            AddForce(CVector(0.0f, GetMass() * theGlobProp.m_throwCoeff * 9.8100004f, 0.0f));
            for (auto it = m_vehicleParts.begin(); it != m_vehicleParts.end();)
            {
                VehiclePart* const part = (it++)->second;
                if (part == m_lastDamagedPart && !part->IsKindOf(RT_CLASS_LOCAL(Chassis)))
                {
                    if (2 * rand() / 0x8000 == 1)
                    {
                        Flow(part, theGlobProp.m_flowVpVelocity);
                    }
                    else
                    {
                        Blow(part);
                    }
                }
                else
                {
                    part->SetNodeEffectAction(randomDeathAction());
                    part->SetNodeAnimAction(0, true);
                    part->SetNodeAnimAction(8, true);
                }
            }
            for (auto& wheelInfo : m_wheels)
            {
                Wheel* const wheel = wheelInfo.GetWheel();
                if (!wheel)
                {
                    continue;
                }
                int action = randomDeathAction();
                wheel->m_suspensionNode->SetProperty(8704, &action);
                if (2 * rand() / 0x8000 == 1)
                {
                    Flow(wheel, theGlobProp.m_flowWheelVelocity);
                }
                else
                {
                    wheel->GetPhysicBody()->SetNodeAction(randomDeathAction(), true);
                }
            }
            FlowUnattachableParts(theGlobProp.m_flowWheelVelocity);
            _Construct(false);
        }
        else if (m_lastDamage == DAMAGE_BLAST)
        {
            // Blown apart: the part that took the last hit bursts, the rest flies off, and the hulk is removed.
            for (auto it = m_vehicleParts.begin(); it != m_vehicleParts.end();)
            {
                VehiclePart* const part = (it++)->second;
                if (part == m_lastDamagedPart)
                {
                    Blow(part);
                }
                else
                {
                    Flow(part, theGlobProp.m_flowVpVelocity);
                }
            }
            for (auto& wheelInfo : m_wheels)
            {
                if (wheelInfo.GetWheel())
                {
                    Flow(wheelInfo.GetWheel(), theGlobProp.m_flowWheelVelocity);
                }
            }
            Remove();
        }
        else if (m_lastDamage == DAMAGE_ENERGY)
        {
            // Thrown up and burnt out in one piece.
            AddForce(CVector(0.0f, GetMass() * theGlobProp.m_throwCoeff * 9.8100004f, 0.0f));
            for (auto it = m_vehicleParts.begin(); it != m_vehicleParts.end();)
            {
                VehiclePart* const part = (it++)->second;
                part->SetNodeEffectAction(randomDeathAction());
                part->SetNodeAnimAction(0, true);
                part->SetNodeAnimAction(8, true);
            }
            for (auto& wheelInfo : m_wheels)
            {
                Wheel* const wheel = wheelInfo.GetWheel();
                if (!wheel)
                {
                    continue;
                }
                int action = randomDeathAction();
                wheel->m_suspensionNode->SetProperty(8704, &action);
                wheel->GetPhysicBody()->SetNodeAction(randomDeathAction(), true);
            }
        }

        _SetDeadStatus();

        if (m_bIsControlledByPlayer)
        {
            M3D_APP->KillPostEffect("DamageStatic");
            M3D_APP->KillPostEffect("DamageBlast");
            M3D_APP->KillPostEffect("DamageIntegrated");
            M3D_APP->KillPostEffect("DamageEnergy");
            M3D_APP->AddPostEffect("Dead", 0.0f);

            m3d::sArgStack stack;
            stack.newIn()->SetV(GetPosition());
            if (auto const error = M3D_KERNEL->GetScriptServer().callScriptFunc("PlayerDead", stack, 3))
            {
                M3D_LOG_ERR(M3D_KERNEL->GetScriptServer().getFormatedScriptErrorDesc(error));
            }
            M3D_APP->ImmediateMessage(0x103F0, 1, 0, 0, 0, CStr(), m3d::AIParam());
        }
    }

    Vehicle::VehicleMoveStatus Vehicle::GetMoveStatus() const
    {
        // RVA 0x5CBB10
        return m_moveStatus;
    }
}  // namespace ai
