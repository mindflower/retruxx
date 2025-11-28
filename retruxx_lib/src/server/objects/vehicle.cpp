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
#include "level.h"
#include "staticautogun.h"
#include "team.h"
#include "vehiclerecollection.h"
#include "core/timer.h"
#include "engine/ode/sources/joint.h"
#include "guns/compoundgun.h"
#include "guns/rocketlauncher.h"
#include "guns/rocketvolleylauncher.h"
#include "scene/nodes/sgnodesound.h"
#include "server/externalpaths.h"
#include "server/intersectionmanager.h"
#include "server/path.h"
#include "server/weaponfirer.h"
#include "server/formations/formation.h"
#include "server/roles/VehicleRole.h"
#include "server/statistic/floatstatistic.h"
#include "server/statistic/statisticmanager.h"

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
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetBasket)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetChassis)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetHealth)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetMaxHealth)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetFuel)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetMaxFuel)
{
	RETRUXX_NOT_IMPLEMENTED;
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
	RETRUXX_NOT_IMPLEMENTED;
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
	RETRUXX_NOT_IMPLEMENTED;
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
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetSteer)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, FireFromWeaponCustom)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, FireFromWeaponCustom2)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, HoldFire)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetCustomControlWeapons)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetCustomControlWeapons)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetCustomControlWeaponsTarget)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetCustomControlWeaponsTarget)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetCustomControlWeaponsTargetObj)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetCustomControlWeaponsTargetObj)
{
	RETRUXX_NOT_IMPLEMENTED;
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
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, HasAmountOfItemsInRepository)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, CanPlaceItemsToRepository)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, AddObjectToRepository)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, TakeOffAllGuns)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, AttachTrailer)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, DetachTrailer)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, TrailerExists)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetTrailer)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, getGodMode)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, setGodMode)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, getImmortalMode)
{
	RETRUXX_NOT_IMPLEMENTED;
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
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetHorn)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetVisible)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetInvisible)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetMaxTorque)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetMaxTorque)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetMaxSpeed)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetMaxSpeed)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetCruisingSpeed)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetCruisingSpeed)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, LimitMaxSpeed)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, UnlimitMaxSpeed)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetForcedMaxTorque)
{
	RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, ResetForcedMaxTorque)
{
	RETRUXX_NOT_IMPLEMENTED;
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
        class FlatLine
        {
        public:
            /* 0x0000 */ CVector normal;
            /* 0x000c */ CVector origin;
            static FlatLine* CreateOrthogonal(FlatLine*, const CVector&, const CVector&);

			FlatLine()
			{
				auto z = this->normal.z;
				this->normal.x = 1.0;
				this->normal.y = 1.0;
				auto v3 = 1.0 / sqrt(z * z + 2.0);
				auto v2 = this->normal.z;
				this->normal.x = v3;
				this->normal.y = v3;
				this->normal.z = v2 * v3;
				this->origin.y = 0.0;
				this->origin.z = 0.0;
				this->origin.x = 0.0;
			}

            bool IsPointInFront(const CVector&)
            {
                RETRUXX_NOT_IMPLEMENTED;
            }

            void RenderDebugInfo(unsigned int)
            {
                RETRUXX_NOT_IMPLEMENTED;
            }
        }; /* size: 0x0018 */


        FlatLine* FlatLine::CreateOrthogonal(FlatLine* a1,const CVector& p1, const CVector& p2)
        {
			auto result = a1;
			auto v7 = p2.x - p1.x;
			auto v4 = p2.z - p1.z;
			auto p2a = 1.0 / sqrt(v4 * v4 + v7 * v7 + 0.00000011920929);
			auto res_12 = p1.x;
			auto y = p1.y;
			auto z = p1.z;
			result->normal.x = p2a * v7;
			result->normal.y = p2a * 0.0;
			result->normal.z = p2a * v4;
			result->origin.x = res_12;
			result->origin.y = y;
			result->origin.z = z;
			return result;
        }

        struct DrivingValues
        {
            /* 0x0000 */ FlatLine checkLine;
            /* 0x0018 */ float checkCircleRadius;
            /* 0x001c */ float nextAngle;
            /* 0x0020 */ float brakingCircleRadius;
        }; /* size: 0x0024 */


        float GetAngleBetween(CVector const& vehiclePos, CVector const& point, CVector const& nextPoint)
        {
			// TODO: check and refactor this
			auto v4 = point.z - nextPoint.z;
			auto v5 = point.y - nextPoint.y;
			auto v6 = v4 * v4 + v5 * v5;
			auto v7 = point.x - nextPoint.x;
			if (sqrt(v6 + v7 * v7) < 0.0099999998)
				return 3.1415927;
			auto y = vehiclePos.y;
			auto v18 = point.z - vehiclePos.z;
			auto v16 = point.x - vehiclePos.x;
			auto nextPointb = 1.0 / sqrt(v18 * v18 + (float)(y - y) * (float)(y - y) + v16 * v16 + 0.00000011920929);
			auto v20 = (float)(y - y) * nextPointb;
			auto v10 = nextPointb * v16;
			auto v11 = v18 * nextPointb;
			auto v19 = nextPoint.z - point.z;
			auto v17 = nextPoint.x - point.x;
			auto nextPointc = 1.0 / sqrt(v19 * v19 + (float)(y - y) * (float)(y - y) + v17 * v17 + 0.00000011920929);
			auto v12 = nextPointc * v17;
			auto v13 = v19 * nextPointc;
			auto v14 = -0.99999899;
			auto nextPointa = (float)((float)((float)(v19 * nextPointc) * v11) + (float)((float)((float)(y - y) * nextPointc) * v20))
				+ (float)((float)(nextPointc * v17) * v10);
			if (nextPointa < -0.99999899 || (v14 = 0.99999899, nextPointa > 0.99999899))
				nextPointa = v14;
			auto v15 = -1;
			if ((float)((float)(v12 * v11) - (float)(v13 * v10)) >= 0.0)
				v15 = 1;
			return acos(nextPointa) * (double)v15;
        }

        void CalcDrivingValues(Vehicle const& vehicle, CVector const& point, CVector const& nextPoint, bool bPrecisely, DrivingValues& dv)
        {           
            auto pos = vehicle.GetPosition();
            
            dv.nextAngle = GetAngleBetween(pos, point, nextPoint);
            dv.checkLine = *FlatLine::CreateOrthogonal(&dv.checkLine, point, nextPoint);
            auto absAngle = fabs(dv.nextAngle);
            if (absAngle < 0.0)
            {
                absAngle = 0.0;
            }
            if (absAngle > 2.5132742)
            {
                absAngle = 2.5132742;
            }

            auto const vehicleSize = vehicle.GetSize();
            auto const v13 = dv.checkLine.normal.z * vehicleSize.x;
            auto const v14 = (dv.checkLine.normal.y * vehicleSize.x) * 0.2;
            auto const v15 = dv.checkLine.origin.x - ((dv.checkLine.normal.x * vehicleSize.x) * 0.2);
            dv.checkLine.origin.y = dv.checkLine.origin.y - v14;
            dv.checkLine.origin.z = dv.checkLine.origin.z - (v13 * 0.2);
            dv.checkLine.origin.x = v15;
            dv.checkCircleRadius = (2.2 - (absAngle * 0.7957747)) * vehicleSize.x;

            auto const nextPointb = sqrt(
                vehicleSize.z * vehicleSize.z
                + vehicleSize.y * vehicleSize.y
                + vehicleSize.x * vehicleSize.x)
                * 0.5;

            if (nextPointb > dv.checkCircleRadius)
            {
                dv.checkCircleRadius = nextPointb;
            }
            if (dv.checkCircleRadius > 1.0e30)
            {
                dv.checkCircleRadius = 1.0e30;
            }

            // bots logic fix
            //if (!bPrecisely)
			if (!bPrecisely)
            {
                dv.checkCircleRadius = dv.checkCircleRadius * 3.0;
            }
   
            auto const velocity = vehicle.GetLinearVelocity();
            auto const scalVelocity = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
            dv.brakingCircleRadius = fabs(dv.nextAngle) * (scalVelocity * log2(scalVelocity) * 0.04);
        }
    }

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
        this->m_cameraMaxDist = 2.0;
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
        const char* DestroyEffectNames[] = { "DestroyEffectPiercing", "DestroyEffectBlast", "DestroyEffectEnergy", "DestroyEffectWater" };
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
                for (wheelsNode->GetFirstChild(wheelNode, "Wheel"); !wheelNode->IsEmpty(); wheelNode->GetNextSibling(wheelNode, "Wheel"))
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

	// TODO: check this
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
		this->m_initialRot = { 0.0, 0.0, 0.0, 1.0};
		this->m_bWheelPresent = wheel != 0;
		this->m_wheel = wheel;
	}

	bool Vehicle::WheelRuntimeInfo::IsWheelPresent() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::WheelRuntimeInfo::SetWheel(Wheel*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	float Vehicle::GetCruisingSpeed() const
	{
		return this->m_cruisingSpeed;
	}

	void Vehicle::UnlimitMaxSpeed()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::IncStoppageMode()
	{
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::RecalcGadgets()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::HoldFire(int)
	{
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
	}

	Quaternion Vehicle::GetWheelInitialRotation(unsigned)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::ActivateHeadLights(bool)
	{
		// TODO: implement Vehicle::ActivateHeadLights
		// RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::Flow(Obj*, float)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::DetachTrailer()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::AddChild(Obj*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Vehicle::GetHorn() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Vehicle::IsTrailer() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	float Vehicle::GetCameraHeight() const
	{
		return this->m_cameraHeight;
	}

	int Vehicle::GetLockedObjId() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::setGodMode(bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	retruxx::map<int, Gadget*, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, Gadget*>>> const& Vehicle::GetGadgets() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::SetRandomSkin()
	{
		auto* cabin = GetCabin();
		if (cabin)
		{
			auto* mdl = cabin->GetModel();
			if (mdl)
			{
				// TODO: check this
				auto& loadedSkins = mdl->GetLoadedSkins();
				if (loadedSkins.loadAllSkins)
				{
					SetSkin(rand() % mdl->GetNumSkins());
				}
				else
				{
					int skin = 0;
					if (!loadedSkins.loadSkins.empty())
					{
						auto begin = loadedSkins.loadSkins.begin();
						std::advance(begin, rand() % loadedSkins.loadSkins.size());
						skin = *begin;
					}
					SetSkin(skin);
				}
			}
		}
	}

	float Vehicle::GetDefaultCruisingSpeed() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	float Vehicle::GetMaxEngineRpm() const
	{
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
	}

	Chassis* Vehicle::GetChassis()
	{
		RETRUXX_NOT_IMPLEMENTED;
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

	void Vehicle::FireFromWeaponCustom2(bool, int)
	{
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
	}

	unsigned Vehicle::GetNumWheels() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::SetHandBrake()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	CVector Vehicle::GetRecollectionPosition(float) const
	{
        RETRUXX_NOT_IMPLEMENTED;
        //TODO: implement recollection logic
        auto center = GetGeometricCenter();
        return center;
	}

	float Vehicle::GetSteer() const
	{
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Class* Vehicle::GetBaseClass()
	{
		return RT_CLASS_LOCAL(ComplexPhysicObj);
	}

	float Vehicle::GetMaxFuel() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Vehicle::getImmortalMode() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	float Vehicle::GetHealth() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Vehicle::AddThing(GeomRepositoryItem const&, bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::EnableSounds(bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::TransferToSpace(dxSpace*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::AIParam Vehicle::VehicleAIOnAttack(Obj*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::SetCanBeDistractedFromMoving(bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int Vehicle::GetCustomControlWeaponsTargetObj() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int Vehicle::GetToBeLockedObjId() const
	{
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::FireFromWeaponCustom(bool, CVector const&, Obj*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	float Vehicle::GetMass() const
	{
		float res = PhysicObj::GetMass();
		for (const auto& wheelInfo : m_wheels)
		{
		    if (const auto* wheel = wheelInfo.GetWheel())
		    {
				res += wheel->GetMass();
		    }
		}
		return res;
	}

	unsigned Vehicle::GetPrice(IPriceCoeffProvider const*) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Vehicle::FireFromWeaponByGunId(int, bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::SetMoveStatus(VehicleMoveStatus moveStatus)
	{
		this->m_moveStatus = moveStatus;
	}

	void Vehicle::SetPassedToAnotherMapStatus()
	{
		RETRUXX_NOT_IMPLEMENTED;
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

	void Vehicle::SetExternalDestination(CVector const&)
	{
		RETRUXX_NOT_IMPLEMENTED;
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

	eGObjPropertySaveStatus Vehicle::GetPropertySaveStatus(int) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::SetPartByName(CStr const& partName, VehiclePart* vehiclePart, bool bUnsafe)
	{
		// TODO: check this
		M3D_ASSERT(!vehiclePart || partName != CHASSIS || IS_KIND_OF(vehiclePart, Chassis));

		auto part = GetPartByName(partName);
		if (part)
		{
		    for (auto& gadget : m_gadgets)
		    {
				gadget.second->ApplyToVp(part, false);
		    }

			if (IS_KIND_OF(part, Chassis))
			{
				auto chassis = RT_DYNCAST(part, Chassis);

				float arg = 0.0;
				chassis->Health().m_BeforeRegenerate(arg);

			}

			if (IS_KIND_OF(part, CompoundVehiclePart))
			{
				auto compound = RT_DYNCAST(part, CompoundVehiclePart);
				compound->SetDurabilityRegeneration(0.0);
			}
			else
			{
				float arg = 0.0;
				part->Durability().m_BeforeRegenerate(arg);
			}
		}

		if (vehiclePart)
		{
		    if (theObjects->m_SaveType != ObjContainer::SAVE_FULL)
		    {
		        for (auto& gadget : m_gadgets)
		        {
					if (gadget.second)
					{
						gadget.second->ApplyToVp(vehiclePart, false);
					}
		        }
		    }
		}

		ComplexPhysicObj::SetPartByName(partName, vehiclePart, bUnsafe);
		m_bRocketLaunchersPresent = 0;

		for (auto part : m_vehicleParts)
		{
			auto actualPart = part.second;
			if (IS_KIND_OF(actualPart, CompoundGun))
			{
				actualPart = RT_DYNCAST(actualPart, CompoundGun)->begin()->second.vp;
			}
			m_bRocketLaunchersPresent = IS_KIND_OF(actualPart, RocketLauncher) || IS_KIND_OF(actualPart, RocketVolleyLauncher);
			if (m_bRocketLaunchersPresent)
			{
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

		if (vehiclePart)
		{
			if (m_bIsControlledByPlayer)
			{
				M3D_APP->EnqueueMessage(66558, vehiclePart->GetPrototypeId(), 0, 0, 0, {}, {});
			}

			auto protoInfo = GetPrototypeInfo();
			if (protoInfo)
			{
				if (IS_KIND_OF(vehiclePart, Chassis))
				{
					auto chassis = RT_DYNCAST(vehiclePart, Chassis);

					float arg = protoInfo->m_healthRegeneration;
					chassis->Health().m_BeforeChange(arg);
				}
				if (IS_KIND_OF(vehiclePart, CompoundVehiclePart))
				{
					auto compound = RT_DYNCAST(vehiclePart, CompoundVehiclePart);
					compound->SetDurabilityRegeneration(protoInfo->m_durabilityRegeneration);
				}
				else
				{
					float arg = protoInfo->m_durabilityRegeneration;
					vehiclePart->Durability().m_BeforeRegenerate(arg);
				}
			}
		}
	}

	float Vehicle::GetCurrentSteerAngle() const
	{
		for (auto& wheelInfo : m_wheels)
		{
		    if (const auto& wheel = wheelInfo.GetWheel())
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
		RETRUXX_NOT_IMPLEMENTED;
	}

	CVector Vehicle::GetLinearVelocity() const
	{
		if (bIsUpdatingByODE())
		{
			return PhysicObj::GetLinearVelocity();
		}

	    return m_ownUpdater->GetLinearVelocity();
	}

	void Vehicle::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::SetCustomControlWeapons(int)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::SetCustomControlWeapons(CustomWeaponControlType)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	unsigned Vehicle::GetSchwarz() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::UnsubscribeRadioManagerFromNearbyObjId(int) const
	{
        // TODO: implement Vehicle::UnsubscribeRadioManagerFromNearbyObjId
		// RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::Blow(Obj*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::SetHorn(bool)
	{
		// TODO: implement Vehicle::SetHorn
		//RETRUXX_NOT_IMPLEMENTED;
	}

	int Vehicle::SetExternalPathByName(char const* pathName)
	{
        auto path = pServer->GetExternalPaths()->GetPath(pathName);
		return SetExternalPath(path);
	}

	void Vehicle::IncInSmokeScreenMode()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	unsigned char Vehicle::GetPriority() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Vehicle::getGodMode() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Vehicle::FireFromWeaponByGunPartName(const CStr& gunPartName, bool enable)
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
		if (byODE && !bIsUpdatingByODE())
		{
			auto pos = GetPosition();
			pos.y += 0.5;
			SetPosition(pos);

			_EnableIntersections(false);

			CVector newPos;
			auto validPosition = ai::GetValidPosition(GetPosition(), GetIntersectionRadius(), GetPrototypeInfo()->m_priority, newPos, true, false, {});
			if (validPosition)
			{
				SetGamePositionOnGround(newPos, true, false);
                _EnableIntersections(true);
			}
			else
			{
				M3D_LOG_ERR("Error: couldn't find valid position for " + GetDebugDescription() + " when enabling physics");
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
		RETRUXX_NOT_IMPLEMENTED;
	}

	float Vehicle::GetMaxHealth() const
	{
		RETRUXX_NOT_IMPLEMENTED;
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

	float Vehicle::EstimateDamageAI(CVector const&, retruxx::vector<int, retruxx::allocator<int>>) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	float Vehicle::EstimateDamageAI() const
	{
        float result = 0.0;
        for (auto const&[name, part] : m_vehicleParts)
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

	void Vehicle::PickUpNearbyObjects(bool, unsigned&, retruxx::vector<int, retruxx::allocator<int>>&)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	float Vehicle::GetFullDurability() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	NumericInRangeRegenerating<float> const& Vehicle::Health() const
	{
		const auto chassis = RT_DYNCAST(GetPartByName(CHASSIS), const Chassis);
		if (chassis)
		{
			return chassis->Health();
		}

		static NumericInRangeRegenerating<float> dummy{ 0.0, 0.0, 0.0, 0.0 };
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

	void Vehicle::DisableGeometry(bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::SetSteer(float radians)
	{
		this->m_steerRadians = radians;
	}

	float Vehicle::GetControl() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Vehicle::AddItemsToRepository(char const*, int)
	{
		// TODO: implement Vehicle::AddItemsToRepository
		//RETRUXX_NOT_IMPLEMENTED;
		return true;
	}

	void Vehicle::ResetForcedMaxTorque()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::ClearSavedStatus()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::HealWheels()
	{
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Vehicle::IsHealthZero() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::FireFromWeaponAI(bool, float, Obj*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Vehicle::AddGadget(Gadget*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::SetCustomControlWeaponsTargetObj(int)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Vehicle::AddObjectToRepository(Obj*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::ResetPositionAndRotation()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::SetPositionSelf(CVector const& pos)
	{
		// TODO: generated code
		// Store old position and calculate shift
		CVector oldpos = GetPosition();

		CVector shift;
		shift.x = pos.x - oldpos.x;
		shift.y = pos.y - oldpos.y;
		shift.z = pos.z - oldpos.z;

		// Set new position for the vehicle itself
		ai::PhysicObj::SetPositionSelf(pos);

		// Update positions for all wheels
		for (auto& wheelInfo : m_wheels)
		{
			if (auto* wheel = wheelInfo.GetWheel())
			{
				CVector wheelPos = wheel->GetPosition();

				// Apply the same shift to the wheel
				wheelPos.x += shift.x;
				wheelPos.y += shift.y;
				wheelPos.z += shift.z;

				wheel->SetPosition(wheelPos);
			}
		}

		// Update position for trailer if it exists
		if (m_trailerObjId >= 0)
		{
			auto* trailerObj = dynamic_cast<PhysicObj*>(theObjects->GetEntityByObjId(m_trailerObjId));
            if (trailerObj)
			{
				CVector trailerPos = GetPosition();

				// Apply the same shift to the trailer
				trailerPos.x += shift.x;
				trailerPos.y += shift.y;
				trailerPos.z += shift.z;

				trailerObj->SetPosition(trailerPos);
			}
		}

		// Update taking sphere position and reset pickup flag
		m_bAllowPickUpMessage = true;

		if (m_takingSphere && m_takingSphere->GetGeomId())
		{
			const float* spherePos = dGeomGetPosition(m_takingSphere->GetGeomId());
			m_pastTakingSpherePosition.x = spherePos[0];
			m_pastTakingSpherePosition.y = spherePos[1];
			m_pastTakingSpherePosition.z = spherePos[2];
		}
	}

	void Vehicle::WeaponLookAtPoint(CVector const&, float)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Vehicle::CanPlaceItemsToRepository(char const*, int)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::AttachTrailer(char const*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::SetTurboThrottleValue(float)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::SetMaxPower(float)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Vehicle::GetStoppageMode() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::GetEnemiesInNeighborhood(float, retruxx::vector<int, retruxx::allocator<int>>&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	VehicleRole* Vehicle::GetRole() const
	{
        return RT_DYNCAST(theObjects->GetEntityByObjId(m_roleId), VehicleRole);
	}

	void Vehicle::SubscribeRadioManagerOnNearbyObjId(int) const
	{
		// TODO: implement Vehicle::SubscribeRadioManagerOnNearbyObjId
		// RETRUXX_NOT_IMPLEMENTED;
	}

	int Vehicle::GetValidSlotIdForGadget(Gadget const*) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	Wheel const* Vehicle::GetFirstExistingWheel() const
	{
		for (const auto&  wheelInfo : m_wheels)
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
		RETRUXX_NOT_IMPLEMENTED;
	}

	float Vehicle::GetAverageEngineRpm() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	CVector Vehicle::GetCustomControlWeaponsTarget() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	float Vehicle::GetMaxTorque() const
	{
		if (this->m_maxTorqueForced)
		{
			return this->m_maxTorqueForcedValue;
		}

		const auto cabin = RT_DYNCAST(GetPartByName(CABIN), const Cabin);
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
        m_timeOutForNextIntersectionWithWorld{ 0,0,10,-1 }
	{
        this->m_diffRatio = prototypeInfo.m_diffRatio;
        this->m_maxEngineRpm = prototypeInfo.m_maxEngineRpm;
        this->m_lowGearShiftLimit = prototypeInfo.m_lowGearShiftLimit;
        this->m_highGearShiftLimit = prototypeInfo.m_highGearShiftLimit;
        this->m_steeringSpeed = prototypeInfo.m_steeringSpeed;
        this->m_driftCoeff = prototypeInfo.m_driftCoeff;

        this->m_lookBox = ai::Box::CreateObject(nullptr, { 1.0, 1.0, 1.0 }, nullptr);
        this->m_targetBox = ai::Box::CreateObject(nullptr, { 1.0, 1.0, 1.0 }, nullptr);

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
        this->m_externalDestination = { 0.0, 0.0, 0.0 };
        this->m_numOfDrivenWheels = 0;
        this->m_bumperPoint = { 0.0, 0.0, 0.0 };
        this->m_bIsControlledByPlayer = 0;
        this->m_bIsMovingAlongExternalPath = 0;
        this->m_pathIndex = 0;
        this->m_bCanBeDistractedFromMoving = 0;
        this->m_size = { 0.0, 0.0, 0.0 };
        this->m_currentDestination = { 0.0, 0.0, 0.0 };
        this->m_pathNum = -1;
        this->m_pPath = 0;
        this->m_bCustomControl = 0;
        this->m_customControlWeapons = CUSTOM_WEAPON_CONTROL_NONE;
        this->m_customControlWeaponsTarget = { 0.0, 0.0, 0.0 };
        this->m_customControlWeaponsTargetObjId = -1;
        this->m_indexInTeam = -1;
        this->m_bRocketLaunchersPresent = 0;
        this->m_moveStatus = MOVE_IDLE;
        this->m_attackStatus = ATTACK_IDLE;
        this->m_lastDamage = DAMAGE_BLAST;
        this->m_lastDamagedPart = 0;
        this->m_deathDamage = DAMAGE_BLAST;

        this->m_takingSphere = SphereForIntersection::CreateObject(prototypeInfo.m_takingRadius, SphereForIntersection::LOOKING, 0);
        dGeomSetBody(this->m_takingSphere->GetGeomId(), GetBody()->id());

        this->m_repository = dynamic_cast<IzvratRepository*>(M3D_KERNEL->New("IzvratRepository"));
        this->m_repository->Clear(false);
        this->m_repository->SetGeomSize({15, 35});

        this->m_groundRepository = dynamic_cast<GeomRepository*>(M3D_KERNEL->New("GeomRepository"));
        this->m_groundRepository->SetGeomSize(ai::theGlobProp.m_groundRepositorySize);

        this->m_effectActions.resize(2);
        this->m_effectActions[0] = AT_STAND1;
        this->m_effectActions[1] = AT_RESERVED1;

        for (int i =0 ; i < 4; ++i)
        {
            m_destroyEffectNames[i] = prototypeInfo.m_destroyEffectNames[i];
        }

        this->m_engineHighSoundNode = 0;
        this->m_engineLowSoundNode = 0;
        this->m_hornSoundNode = 0;
        this->m_trailerJoint = 0;
        this->m_trailerObjId = -1;
        this->m_relTrailerJointPosOnMe = {0.0, 0.0, 0.0};
        this->m_relTrailerJointPosOnTrailer = { 0.0, 0.0, 0.0 };
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
        this->m_prevPosToCheckStuck = { 0.0, 0.0, 0.0 };
        this->m_timeOutToCheckStuck = 0.0;
        this->m_curSteeringForce = { 0.0, 0.0, 0.0 };
        this->m_soundRechargeChannelId = -1;
        this->m_bCurSteeringForceValid = 0;
	}

	void Vehicle::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::DecInSmokeScreenMode()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::SetTrailer()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::RenderDebugInfo() const
	{
        CVector curPoint;
        if (GetPathItem(m_pPath, m_pathNum, curPoint))
        {
            auto const nextPoint = _GetNextPathPoint();

            DrivingValues dv;
            CalcDrivingValues(*this, curPoint, nextPoint, true, dv);

            DebugCircle(curPoint, dv.brakingCircleRadius, 0xFF00FF00);
            auto curPoint1 = curPoint;

            auto pos = GetPosition();
            //pos.y += 10;
            curPoint1.y = M3D_KERNEL->GetEngineCfg().GetHeight(curPoint1.x, curPoint1.z);
            pos.x = curPoint1.x - pos.x;
            pos.y = curPoint1.y - pos.y;
            pos.z = curPoint1.z - pos.z;
            auto scal = sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);

            auto velocity = GetLinearVelocity();
            auto smth = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);

            //M3D_APP->DrawLine(pos, curPoint, 0xFF00FFFF);

            DebugCircle(curPoint, scal + 500/smth, 0xFFFF0000);
            DebugCircle(curPoint, dv.checkCircleRadius, 0xFFFFFF00);
        }
		//RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::SetVisible()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int Vehicle::GetNpcMotionControllerId() const
	{
		RETRUXX_NOT_IMPLEMENTED;
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

		static NumericInRangeRegenerating<float> dummy{ 0.0, 0.0, 0.0, 0.0 };
		return dummy;
	}

	NumericInRangeRegenerating<float> const& Vehicle::Fuel() const
	{
		const auto chassis = RT_DYNCAST(GetPartByName(CHASSIS), const Chassis);
		if (chassis)
		{
			return chassis->Fuel();
		}

		static NumericInRangeRegenerating<float> dummy{ 0.0, 0.0, 0.0, 0.0 };
		return dummy;
	}

	float Vehicle::GetFuel() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int Vehicle::GetSeenObjId() const
	{
        return m_seenObjId;
	}

	void Vehicle::GetGeoms(retruxx::vector<Geom*, retruxx::allocator<Geom*>>&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
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
			auto* chassis = dynamic_cast<const Chassis*>(part);
			if (chassis && chassis->Fuel().value().get() == chassis->Fuel().minValue().get())
			{
				return maxSpeed > theGlobProp.m_maxSpeedWithNoFuel ? theGlobProp.m_maxSpeedWithNoFuel : maxSpeed;
			}
		}
		return maxSpeed;
	}

	bool Vehicle::CanChildBeAdded(m3d::Class*) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	float Vehicle::GetTurboThrottleTime() const
	{
        return m_turboThrottleTime;
	}

	float Vehicle::GetFullDurabilityCoeffForDamageType(DamageType) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	Obj* Vehicle::CloneObj()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int Vehicle::CheckSkin(int skinNum)
	{
		if (!m_bIsControlledByPlayer)
		{
			// TODO: check this
			auto cabin = GetPartByName(CABIN);
			if (cabin && cabin->IsKindOf(RT_CLASS_LOCAL(Cabin)))
			{
				auto model = cabin->GetModel();
				if (model && !model->GetLoadedSkins().loadAllSkins)
				{
					auto it = model->GetLoadedSkins().loadSkins.find(skinNum);
					if (it == model->GetLoadedSkins().loadSkins.end())
					{
					    if (!model->GetLoadedSkins().loadSkins.empty())
					    {
							return *model->GetLoadedSkins().loadSkins.begin();
					    }
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
		RETRUXX_NOT_IMPLEMENTED;
	}

	float Vehicle::GetEngineRpm() const
	{
		RETRUXX_NOT_IMPLEMENTED;
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

        if (m_trailerObjId >=0)
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
		RETRUXX_NOT_IMPLEMENTED;
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

	void Vehicle::SetBasket(VehiclePart*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	Cabin const* Vehicle::GetCabin() const
	{
		return RT_DYNCAST(GetPartByName(CABIN), const Cabin);
	}

	Cabin* Vehicle::GetCabin()
	{
		return RT_DYNCAST(GetPartByName(CABIN), Cabin);
	}

	float Vehicle::GetTurboThrottleValue() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int Vehicle::GetMaxGadgets(CStr const&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	float Vehicle::GetMaxPower() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::DecStoppageMode()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::ShowVehicle(bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
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

	retruxx::set<ref_ptr<Obstacle>, retruxx::less<ref_ptr<Obstacle>>, retruxx::allocator<ref_ptr<Obstacle>>> const& Vehicle::
	GetNearbyObstacles() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	CStr Vehicle::GetPropertyName(int) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Vehicle::RemoveItemsFromRepository(char const*, int)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::SetCustomControlWeaponsTarget(CVector const&)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	float Vehicle::GetTimeToLockTarget() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::SetGamePositionOnGround(CVector const& pos, bool bWithCollisions, bool bWithWater)
	{
		// TODO: generated code
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

			float waterHeight = m3d::pClient->GetWorld().GetLandscape().getWaterHeight(landscapeX,
				landscapeZ
			);

			// If water is higher than ground, use water level
			if (waterHeight > groundPos.y)
			{
				groundPos.y = waterHeight;
				isOnWater = true;
			}
		}

		// Step 3: Get current vehicle rotation for reference
		Quaternion currentRotation = GetRotation();

		// Step 4: Calculate hover height based on wheel geometry
		// Find the first valid wheel to determine appropriate hover height
		float hoverHeight = 0.0f;
		bool foundValidWheel = false;

		for (const auto& wheelInfo : m_wheels)
		{
			// Check if this wheel has all required components
			if (wheelInfo.GetWheel() &&
				wheelInfo.GetWheel()->GetPhysicBody() &&
				!wheelInfo.GetWheel()->GetPhysicBody()->m_pGeoms.empty() &&
				wheelInfo.GetWheel()->GetPhysicBody()->m_pGeoms[0]->GetGeom()) {

				// Get the wheel's sphere geometry to determine radius
				ai::Sphere* wheelSphere = dynamic_cast<ai::Sphere*>(
					wheelInfo.GetWheel()->GetPhysicBody()->m_pGeoms[0]->GetGeom());

				if (wheelSphere)
				{
					float wheelRadius = wheelSphere->GetRadius();

					// Calculate hover height: wheel radius minus initial Y position plus small offset
					// This positions the vehicle so wheels touch the ground at their initial positions
					hoverHeight = wheelRadius - wheelInfo.m_initialPos.y + 0.1f;
					foundValidWheel = true;
					break;
				}
			}
		}

		// Fallback: if no valid wheels found, use vehicle size
		if (!foundValidWheel)
		{
			hoverHeight = m_size.y * 0.5f;  // Use half vehicle height as reasonable default
		}

		hoverOffset.y = hoverHeight;

		// Step 5: Get terrain surface normal (unless on water)
		if (!isOnWater)
		{
			CVector terrainNormal = ai::pServer->GetWorld()->GetLandscape().getNormal(groundPos.x, groundPos.z);
			normal = terrainNormal;
		}

		// Step 6: Calculate vehicle orientation based on ground surface
		CVector vehicleForwardDir = GetDirection();

		// Project the forward direction onto the ground plane defined by the surface normal
		CVector projectedForwardDir = ai::ProjectVectorOntoPlane(normal, vehicleForwardDir);

		// TODO: check this!!!!

		// Only update orientation if the projected direction is significant
		float projectedDirLengthSq = ((projectedForwardDir.x * projectedForwardDir.x) + (projectedForwardDir.z * projectedForwardDir.z)) + (projectedForwardDir.y * projectedForwardDir.y);

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

	void Vehicle::SetMaxTorque(float)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::LimitMaxSpeed(float)
	{
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
	}

	Vehicle::VehicleAttackStatus Vehicle::GetAttackStatus() const
	{
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::IncOnOilMode()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	CVector Vehicle::GetGeometricCenter() const
	{
        // TODO: generated code
		// Get the vehicle's rotation as a quaternion
		Quaternion rotation = GetRotation();

		// Calculate intermediate values for quaternion to matrix conversion
		float qx_qx = rotation.x * rotation.x;
		float qy_qy = rotation.y * rotation.y;
		float qz_qz = rotation.z * rotation.z;

		float qx_qy = rotation.x * rotation.y;
		float qx_qz = rotation.x * rotation.z;
		float qx_qw = rotation.x * rotation.w;

		float qy_qz = rotation.y * rotation.z;
		float qy_qw = rotation.y * rotation.w;
		float qz_qw = rotation.z * rotation.w;

		// Construct rotation matrix from quaternion
		// First row
		float m11 = 1.0f - 2.0f * (qy_qy + qz_qz);
		float m12 = 2.0f * (qx_qy + qz_qw);
		float m13 = 2.0f * (qx_qz - qy_qw);

		// Second row  
		float m21 = 2.0f * (qx_qy - qz_qw);
		float m22 = 1.0f - 2.0f * (qx_qx + qz_qz);
		float m23 = 2.0f * (qy_qz + qx_qw);

		// Third row
		float m31 = 2.0f * (qx_qz + qy_qw);
		float m32 = 2.0f * (qy_qz - qx_qw);
		float m33 = 1.0f - 2.0f * (qx_qx + qy_qy);

		// Create the full rotation matrix
		CMatrix rotationMatrix;

		// Set rotation components
		rotationMatrix._11 = m11;
		rotationMatrix._12 = m12;
		rotationMatrix._13 = m13;
		rotationMatrix._14 = 0.0f;

		rotationMatrix._21 = m21;
		rotationMatrix._22 = m22;
		rotationMatrix._23 = m23;
		rotationMatrix._24 = 0.0f;

		rotationMatrix._31 = m31;
		rotationMatrix._32 = m32;
		rotationMatrix._33 = m33;
		rotationMatrix._34 = 0.0f;

		// Set translation components to zero and homogeneous coordinate to 1
		rotationMatrix._41 = 0.0f;
		rotationMatrix._42 = 0.0f;
		rotationMatrix._43 = 0.0f;
		rotationMatrix._44 = 1.0f;

		// Create a copy of the matrix (as in the original code)
		CMatrix finalMatrix;
		finalMatrix = rotationMatrix;

		// Define the initial up direction vector (typically (0, 1, 0) or similar)
		const CVector INITIAL_UP_DIRECTION = { 0.0, 1.0, 0.0 };

		// Calculate the offset vector by rotating the initial up direction
		// and scaling by vehicle height and a factor of 0.4
		float vehicleHeight = this->m_size.y;
		float scaleFactor = 0.4f;

		// Transform the up direction vector by the rotation matrix
		// This rotates the vector from local space to world space
		float offsetX = (finalMatrix._11 * INITIAL_UP_DIRECTION.x +
						 finalMatrix._21 * INITIAL_UP_DIRECTION.y +
						 finalMatrix._31 * INITIAL_UP_DIRECTION.z) * vehicleHeight * scaleFactor;

		float offsetY = (finalMatrix._12 * INITIAL_UP_DIRECTION.x +
						 finalMatrix._22 * INITIAL_UP_DIRECTION.y +
						 finalMatrix._32 * INITIAL_UP_DIRECTION.z) * vehicleHeight * scaleFactor;

		float offsetZ = (finalMatrix._13 * INITIAL_UP_DIRECTION.x +
						 finalMatrix._23 * INITIAL_UP_DIRECTION.y +
						 finalMatrix._33 * INITIAL_UP_DIRECTION.z) * vehicleHeight * scaleFactor;

		// Get the vehicle's world position
		CVector vehiclePosition = GetPosition();

		// Calculate the geometric center by adding the offset to the vehicle position
		// The geometric center is typically above the vehicle's base position
		CVector result;
		result.x = vehiclePosition.x + offsetX;
		result.y = vehiclePosition.y + offsetY;
		result.z = vehiclePosition.z + offsetZ;

		return result;

	}

	float Vehicle::EstimateDamageFromPositionAI(CVector const&, CVector const&,
		retruxx::vector<int, retruxx::allocator<int>>) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::SetNpcMotionControllerId(int)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	VehiclePrototypeInfo const* Vehicle::GetPrototypeInfo() const
	{
		return dynamic_cast<VehiclePrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
	}

	void Vehicle::PlaceToEndOfPath()
	{
		// TODO: check this
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
		RETRUXX_NOT_IMPLEMENTED;
	}

	CVector Vehicle::GetBumperPoint() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	GeomRepository* Vehicle::GetGroundRepository() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Vehicle::ApplyModifier(Modifier const&)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::SetTurningToGroundForceAndTorque(CVector const&, CVector const&, CVector const&)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	Wheel* Vehicle::GetWheel(unsigned)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	Wheel const* Vehicle::GetWheel(unsigned) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	float Vehicle::GetThrottle() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	IzvratRepository const* Vehicle::GetRepository() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	IzvratRepository* Vehicle::GetRepository()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Vehicle::RemoveChild(Obj*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::SetBrake(float)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::AIParam Vehicle::VehicleAIOnDead(Obj*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::SetMaxSpeed(float)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Vehicle::HasAmountOfItemsInRepository(char const*, int) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::SetTurboThrottleTime(float)
	{
		RETRUXX_NOT_IMPLEMENTED;
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
	}

	void Vehicle::Update(float elapsedTime, unsigned workTime)
	{
		// TODO: check this
		if (!GetParentRepository() && (GetFlags() & 1) != 0)
		{
			ai::LocalProfiler prof(pServer->GetPathFindingProfiler());
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

			if (elapsedTime < 0.000099999997)
			{
				return;
			}

			_UpdatePhysicsUpdater();
			auto& heath = Health();
			if (heath.minValue().get() >= heath.value().get())
			{
			    if (!m_bImmortalMode)
			    {
					_EvaluateToDead();
					return;
			    }

				CauseEvent(GE_VEHICLE_WITHOUT_HEALTH, 0.0, { GetId() }, {});
			}

			const auto* prototypeInfo = GetPrototypeInfo();
			if (prototypeInfo)
			{
			    if (prototypeInfo->m_healthRegeneration != 0.0)
			    {
					heath.regenerate(elapsedTime);
			    }
				if (prototypeInfo->m_durabilityRegeneration != 0.0)
				{
				    for (auto& [name, part] : m_vehicleParts)
				    {
				        if (IS_KIND_OF(part, CompoundVehiclePart))
				        {
							auto compoundPart = RT_DYNCAST(part, CompoundVehiclePart);
							compoundPart->RegenerateDurability(elapsedTime);
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
				auto target = _GetCustomWeaponTargetPoint();
				WeaponFirer::WeaponLookAtPoint(this, target, elapsedTime);
				_CauseCustomGunPointedEvents();
			}
			if (m_bIsControlledByPlayer)
			{
				if (m_bMustGetOutOfDifficultPlace)
				{
					_GetOutOfDifficlultPlaceInternal();
					m_bMustGetOutOfDifficultPlace = false;
				}

				auto* cabin = GetCabin();
				if (cabin)
				{
					auto time = GetMass() * fabs(m_engineRpm) * cabin->GetFuelConsumption() * elapsedTime * 0.000001;
					Fuel().regenerate(time);
				}

				auto* globalStatistic = (FloatStatistic*)theStatisticManager->GetStatistic(STATISTIC_PATH_ELAPSED, "FloatStatistic");
				globalStatistic->m_bGlobalFlag = true;

				const auto linearVelocity = GetLinearVelocity();
				const auto len = linearVelocity.length() * elapsedTime;
				globalStatistic->Increase(len);

				auto* levelStatistic = (FloatStatistic*)theStatisticManager->GetStatistic(STATISTIC_PATH_ELAPSED + pServer->GetWorld()->m_level->m_levelName, "FloatStatistic");
				levelStatistic->m_bGlobalFlag = false;
				levelStatistic->Increase(len);
				_CheckForNearbyChests();
			}
			if (!m_bIsTrailer)
			{
				auto* role = GetRole();
				if (role && (!m_bIsMovingAlongExternalPath || m_bCanBeDistractedFromMoving))
				{
					role->UpdateVehicle(elapsedTime, this);
				}

				m_timeOutForNextIntersectionWithWorld.regenerate(elapsedTime);
				if (this->m_timeOutForNextIntersectionWithWorld.value().get() == this->m_timeOutForNextIntersectionWithWorld.minValue().get())
				{
					auto time = _GetTimeOutForNextIntersectionWithWorld();
					m_timeOutForNextIntersectionWithWorld.value().set(time);
					IntersectWithWorld();
				}
				if (!m_bIsControlledByPlayer || m_bIsMovingAlongExternalPath)
				{
					if (!m_bCustomControl)
					{
						SetThrottle(0.0, 1);
						m_steerRadians = 0.0;
					}
				}
				else if (!m_bCustomControl)
				{
					_UpdateSeenObjAndWeapons(elapsedTime);
					_UpdateAlarmStatus();
					_UpdateLockedObj(elapsedTime);
					_TakeWaterIntoAccount(elapsedTime);
					_ApplyStabilizingForces();
					_KeepThrottle(1);
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
				    return;
				}
				if (!m_moveStatus)
				{
					SetThrottle(0.0, 1);
					if (m_pPath)
					{
						delete m_pPath;
					}
					m_pPath = 0;
					m_pathNum = -1;
				}
				if (m_moveStatus == 2)
				{
					const auto steerForce = _CalcSteeringForce(elapsedTime);
					_DriveBySteeringForce(steerForce);
					auto* parent = RT_DYNCAST(GetParent(), Team);
					auto formation = parent->GetFormation();
					if (!formation->bIsMoving())
					{
						auto direction = formation->GetDirection();

						CVector nextPoint;
						nextPoint.x = m_externalDestination.x + direction.x;
						nextPoint.y = m_externalDestination.y + direction.y;
						nextPoint.z = m_externalDestination.z + direction.z;
						if (_bPassedPathPoint(m_externalDestination, nextPoint, true))
						{
							m_moveStatus = MOVE_IDLE;
						}
					}
				}
				else
				{
					if (m_pPath && m_pathNum >= 0)
					{
						CVector curPoint;
						ai::GetPathItem(m_pPath, m_pathNum, curPoint);

						CVector nextPoint = _GetNextPathPoint();
						
						const bool isLastPath = m_pathNum == m_pPath->GetSize() - 1;
						const auto force = _CalcSteeringForce(elapsedTime);
						_DriveBySteeringForce(force);
						if (_bPassedPathPoint(curPoint, nextPoint, elapsedTime))
						{
							++m_pathNum;
						}
						if (m_pathNum >= m_pPath->GetSize() && m_moveStatus == MOVE_MOVING_ALONG_PATH)
						{
							_SetIdleMoveStatusAndCauseTargetReached();
						}
					}
					else if (!m_bCustomControl)
					{
						SetThrottle(0.0, 1);
						m_steerRadians = 0.0;
					}
				}
				if (!m_attackStatus)
				{
					for (auto& obstacle : m_currentNearbyObstacles)
					{
						auto owner = RT_DYNCAST(obstacle->GetOwner(), Obj);
						if (owner && owner->GetClass() == &ai::Vehicle::m_classVehicle && owner->bIsEnemyWith(this))
						{
							CauseEvent(GE_NOTICE_ENEMY, 0.0, owner->GetId(), {});
						}
					}
				}

				// TODO: remove code duplication
				_TakeWaterIntoAccount(elapsedTime);
				_ApplyStabilizingForces();
				_KeepThrottle(1);
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
				return;
			}

			SetThrottle(0.0, 0);
			// TODO: remove code duplication
			_TakeWaterIntoAccount(elapsedTime);
			_ApplyStabilizingForces();
			_KeepThrottle(1);
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
			return;
		}
	}

	float Vehicle::GetCollisionRadius() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	float Vehicle::GetDriftCoeff() const
	{
		RETRUXX_NOT_IMPLEMENTED;
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
			if (!wheel) continue;

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
			newWheelPos.x = (rotMatrix._11 * wheelRelPos.x) + (rotMatrix._21 * wheelRelPos.y) + (rotMatrix._31 * wheelRelPos.z) + vehiclePos.x;
			newWheelPos.y = (rotMatrix._12 * wheelRelPos.x) + (rotMatrix._22 * wheelRelPos.y) + (rotMatrix._32 * wheelRelPos.z) + vehiclePos.y;
			newWheelPos.z = (rotMatrix._13 * wheelRelPos.x) + (rotMatrix._23 * wheelRelPos.y) + (rotMatrix._33 * wheelRelPos.z) + vehiclePos.z;

			// Set new wheel position
			wheel->SetPosition(newWheelPos);

			// Apply relative rotation to wheel
			Quaternion wheelRot = wheel->GetRotation();

			Quaternion newWheelRot;
			newWheelRot.x = (wheelRot.w * relRot.x) + (wheelRot.z * relRot.y) + (wheelRot.x * relRot.w) - (wheelRot.y * relRot.z);
			newWheelRot.y = (wheelRot.w * relRot.y) + (wheelRot.y * relRot.w) + (wheelRot.x * relRot.z) - (wheelRot.z * relRot.x);
			newWheelRot.z = (wheelRot.w * relRot.z) + (wheelRot.z * relRot.w) + (wheelRot.y * relRot.x) - (wheelRot.x * relRot.y);
			newWheelRot.w = (wheelRot.w * relRot.w) - (wheelRot.x * relRot.x) - (wheelRot.y * relRot.y) - (wheelRot.z * relRot.z);

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
					newTrailerPos.x = (rotMatrix._11 * trailerRelPos.x) + (rotMatrix._21 * trailerRelPos.y) + (rotMatrix._31 * trailerRelPos.z) + vehiclePos.x;
					newTrailerPos.y = (rotMatrix._12 * trailerRelPos.x) + (rotMatrix._22 * trailerRelPos.y) + (rotMatrix._32 * trailerRelPos.z) + vehiclePos.y;
					newTrailerPos.z = (rotMatrix._13 * trailerRelPos.x) + (rotMatrix._23 * trailerRelPos.y) + (rotMatrix._33 * trailerRelPos.z) + vehiclePos.z;

					// Set new trailer position
					trailer->SetPosition(newTrailerPos);

					// Apply relative rotation to trailer
					Quaternion trailerRot = trailer->GetRotation();

					Quaternion newTrailerRot;
					newTrailerRot.x = (trailerRot.w * relRot.x) + (trailerRot.z * relRot.y) + (trailerRot.x * relRot.w) - (trailerRot.y * relRot.z);
					newTrailerRot.y = (trailerRot.w * relRot.y) + (trailerRot.y * relRot.w) + (trailerRot.x * relRot.z) - (trailerRot.z * relRot.x);
					newTrailerRot.z = (trailerRot.w * relRot.z) + (trailerRot.z * relRot.w) + (trailerRot.y * relRot.x) - (trailerRot.x * relRot.y);
					newTrailerRot.w = (trailerRot.w * relRot.w) - (trailerRot.x * relRot.x) - (trailerRot.y * relRot.y) - (trailerRot.z * relRot.z);

					trailer->SetRotation(newTrailerRot);
				}
		}
	}

	void Vehicle::InflictDamage(DamageInfo const&)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	float Vehicle::GetMaxFullDurability() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::SetInvisible()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::SetCustomLinearVelocity(float velocityValue)
	{
		auto direction = GetDirection();
		auto value = 1.0 / sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z + 0.00000011920929);

		CVector velocity;
		velocity.x = (direction.x * value) * velocityValue;
		velocity.y = (direction.y * value) * velocityValue;
		velocity.z = (direction.z * value) * velocityValue;

		SetLinearVelocity(velocity);
		SetAngularVelocity({ 0.0, 0.0, 0.0 });
		for (auto& wheelInfo : m_wheels)
		{
			if (auto* wheel = wheelInfo.GetWheel())
			{
				wheel->SetLinearVelocity(velocity);
				wheel->SetAngularVelocity({ 0.0, 0.0, 0.0 });
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
		return RT_DYNCAST(GetPartByName(BASKET), const Basket);
	}

	void Vehicle::SetCabin(VehiclePart*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::IntersectWithWorld()
	{
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

		IntersectionManager::GetIntersectedObjects(m_currentNearbyObstacles, GetIntersectionSphere(), m_targetClasses, false, false);

		if (m_bIsControlledByPlayer)
		{
			// TODO: check this
			for (const auto& obstacle : m_currentNearbyObstacles)
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
                    RETRUXX_NOT_IMPLEMENTED;
                }
			}

			for (const auto& obstacle : m_pastNearbyObstacles)
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

	void Vehicle::SetForcedMaxTorque(float)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Vehicle::DriveToPoint(CVector const&, CVector const&, bool, float)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::EnableGeometry(bool changePhysicState)
	{
		// TODO: check changePhysicState
		ComplexPhysicObj::EnableGeometry(true);
		for (auto& wheelInfo : m_wheels)
		{
		    if (auto* wheel = wheelInfo.GetWheel())
		    {
				wheel->EnableGeometry(true);
		    }
		}
	}

	bool Vehicle::_GetPropertyInternal(int, m3d::AIParam&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_PutContour()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	namespace
	{
		bool bMustTakeScreenShot = false;
	}

	void Vehicle::_InternalCreateVisualPart()
	{
		ai::ComplexPhysicObj::_InternalCreateVisualPart();

		this->m_maxSpeedLimited = 0;
		this->m_maxTorqueForced = 0;

		int validSkin = CheckSkin(GetSkin());
		this->SetSkin(validSkin);

		// Set global screenshot flag
		ai::bMustTakeScreenShot = true;

		// Get prototype information
		const auto * prototypeInfo = this->GetPrototypeInfo();

		// Find chassis part
		Chassis* chassis = nullptr;
		VehiclePart* chassisPart = GetPartByName("CHASSIS");
		if (chassisPart && chassisPart->IsKindOf(&ai::Chassis::m_classChassis))
		{
			chassis = dynamic_cast<Chassis*>(chassisPart);
		}

		// Find cabin part
		Cabin* cabin = nullptr;
		VehiclePart* cabinPart = GetPartByName("CABIN");
		if (cabinPart && cabinPart->IsKindOf(&ai::Cabin::m_classCabin))
		{
			cabin = dynamic_cast<Cabin*>(cabinPart);
		}

		// Proceed only if chassis exists
		if (!chassis)
		{
			// Log error about missing chassis
			M3D_LOG_ERR("Error: the vehicle with prototype '" +
						prototypeInfo->m_prototypeName +
						"' haven't CHASSIS part");
			return;
		}

		if (cabin)
		{
			const auto* cabinPrototypeInfo = cabin->GetPrototypeInfo();
			if (!cabinPrototypeInfo->m_engineHighSoundName.empty())
			{
				CVector scale = { 1.0, 1.0, 1.0 };
				// TODO: check this;
				auto node = PhysicBody::CreateNode(cabinPrototypeInfo->m_engineHighSoundName, 0, scale, nullptr, false);
                m_engineHighSoundNode = (m3d::SgSoundSourceNode*)node;
				cabin->m_Node->AddChild(node);
			}
		}

		const auto oldPosition = GetPosition();
		const auto oldRotation = GetRotation();
		SetPosition({ 0.0, 0.0, 0.0 });
		SetRotation({ 0.0, 0.0, 0.0, 1.0 });

		auto* animatedModelsServer = (m3d::AnimatedModelsServer*)&M3D_APP->GetAnimatedModelsServer();

		for (int i =0; i < m_wheels.size(); ++i)
		{
		    if (auto* wheel = m_wheels[i].GetWheel())
		    {
				wheel->CreateSuspensionNode();

				if (!wheel->m_suspensionNode) continue;

				// Generate suspension load point name
				const char* side = (i % 2 == 0) ? "L" : "R";
				int wheelNumber = (i / 2) + 1;

				CStr suspensionLpName = "LP_SSP" + CStr(0) + CStr(wheelNumber) + side;

				// Add suspension node to chassis
				chassis->m_Node->AddChild(wheel->m_suspensionNode);

				// Get bone matrix for suspension point
				CMatrix boneMatrix;
				if (animatedModelsServer->GetBoneMatrixByNameFromModelName(
					chassis->m_modelname.c_str(),
					suspensionLpName,
					boneMatrix,
					false))
				{
					// Set suspension node transform from bone matrix
					CVector origin(boneMatrix.m[3][0], boneMatrix.m[3][1], boneMatrix.m[3][2]);
					Quaternion rotation;
					rotation.FromMatrix(boneMatrix);

					wheel->m_suspensionNode->SetOriginAbs(origin);
					wheel->m_suspensionNode->SetRotation(rotation);
					wheel->m_suspensionNode->UpdateXForm(false, true);
				}
				else
				{
					// Load point not found - set to zero and log error
					wheel->m_suspensionNode->SetOriginAbs({0.0, 0.0, 0.0});

					M3D_LOG_ERR("Error: LoadPoint not found: '" + suspensionLpName +
								"' for model '" + chassis->m_modelname + "'");
				}
		    }
		}

		SetPosition(oldPosition);
		SetRotation(oldRotation);
		for (int i = 0; i < m_wheels.size(); ++i)
		{
			if (auto* wheel = m_wheels[i].GetWheel())
			{
				wheel->CreateVisualPart();
				wheel->TransferPhysicParamsToSceneGraphNode();
				wheel->GetPhysicBody()->m_Node->UpdateXForm(false, true);
				wheel->m_suspensionNode->UpdateXForm(false, true);
			}
		}

		TransferPhysicParamsToSceneGraphNode();
		IntersectWithWorld();

		const auto flags = GetFlags();
		if ((flags & 8) == 0 && (flags & 2) == 0 && !GetParentRepository())
		{
			auto* basketPart = GetPartByName(BASKET);
			if (basketPart && IS_KIND_OF(basketPart, Basket))
			{
				basketPart->SetEffectActions(m_effectActions);
				basketPart->SetNodeAnimAction(m_effectActions.front(), true);
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

	bool Vehicle::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_InternalPostLoad()
	{
		// TODO: generated code
		// Call parent implementation
		ai::PhysicObj::_InternalPostLoad();

		// Set up repository
		if (m_repository)
		{
			m_repository->SetVehicle(this);
		}

		// Adjust vehicle properties
		_AdjustSizeAndBumperPoint();

		// Set default camera height if not set
		if (m_cameraHeight <= 0.0f)
		{
			m_cameraHeight = m_size.y + 1.0f;
		}

		// Set cruising speed
		m_cruisingSpeed = GetMaxSpeed();

		// Get prototype info
		const auto* prototypeInfo = GetPrototypeInfo();

		// Verify chassis part exists
		VehiclePart* part = GetPartByName("CHASSIS");
		if (!part || !part->IsKindOf(&ai::Chassis::m_classChassis))
		{
			M3D_LOG_ERR("Error: the vehicle with prototype '" + prototypeInfo->m_prototypeName + "' haven't CHASSIS part");
			return;
		}

		// Get chassis model name
		auto* chassisPart = dynamic_cast<Chassis*>(part);
		const auto chassisModelName = chassisPart->m_modelname;

		auto* serverAnimatedModels = dynamic_cast<m3d::AnimatedModelsServer*>(&M3D_APP->GetAnimatedModelsServer());

		// Store current position and rotation
		CVector oldPos = GetPosition();
		Quaternion oldRot = GetRotation();

		// Reset to origin for setup
		this->SetPosition({0.0, 0.0, 0.0});
		this->SetRotation({0.0, 0.0, 0.0, 1.0});

		// Create wheels if they don't exist
		bool wheelsJustCreated = m_wheels.empty();
		if (wheelsJustCreated)
		{
			for (size_t i = 0; i < prototypeInfo->m_wheelInfos.size(); ++i)
			{
				const auto& wheelInfo = prototypeInfo->m_wheelInfos[i];

				// Create wheel object
				Wheel* wheel = nullptr;
				int newObjectId = theObjects->CreateNewObject(
					wheelInfo.m_wheelPrototypeId,
					{}, -1, -1);

				wheel = dynamic_cast<Wheel*>(theObjects->GetEntityByObjId(newObjectId));

				// Add wheel runtime info
				WheelRuntimeInfo wheelRuntime(wheel);
				m_wheels.push_back(std::move(wheelRuntime));
			}
		}

		// Initialize wheel counters
		m_numOfDrivenWheels = 0;

		// Set up each wheel
		for (size_t i = 0; i < prototypeInfo->m_wheelInfos.size(); ++i)
		{
			auto& wheelInfo = prototypeInfo->m_wheelInfos[i];
			WheelRuntimeInfo& runtimeInfo = m_wheels[i];
			Wheel* wheel = runtimeInfo.GetWheel();

			if (!wheel)
			{
				continue;
			}

			// Configure wheel properties
			wheel->m_driven = 1;
			wheel->m_steering = wheelInfo.m_steering;

			// Determine wheel position name (LP_WHL0L, LP_WHL0R, etc.)
			CStr wheelSide = (i % 2 == 0) ? "L" : "R";
			int wheelNumber = (i / 2) + 1;
			CStr boneName = "LP_WHL" + CStr("0") + CStr(wheelNumber) + wheelSide;

			// Get wheel position from bone matrix
			CMatrix boneMatrix;
			bool boneFound = serverAnimatedModels->GetBoneMatrixByNameFromModelName(
				chassisModelName.c_str(), boneName, boneMatrix,0) != 0;

			if (!boneFound)
			{
				M3D_LOG_ERR("Error: LoadPoint not found: " + boneName +
							" for model '" + chassisModelName + "'");

				// Use current wheel position as fallback
				runtimeInfo.m_initialPos = wheel->GetPosition();
			}
			else
			{
				// Extract position and rotation from bone matrix
				runtimeInfo.m_initialPos = CVector(boneMatrix.m[3][0], boneMatrix.m[3][1], boneMatrix.m[3][2]);
				runtimeInfo.m_initialRot.FromMatrix(boneMatrix);

				if (wheelsJustCreated)
				{
					wheel->SetRotation(runtimeInfo.m_initialRot);
				}
			}

			// Adjust wheel position
			runtimeInfo.m_initialPos.y -= prototypeInfo->m_additionalWheelsHover;

			// Set wheel position relative to mass center
			if (wheelsJustCreated)
			{
				wheel->SetPosition(runtimeInfo.m_initialPos);
			}
			else
			{
				CVector wheelPos = wheel->GetPosition();
				CVector relativePos;
				relativePos.x = wheelPos.x - m_massCenter.x;
				relativePos.y = wheelPos.y - m_massCenter.y;
				relativePos.z = wheelPos.z - m_massCenter.z;
				wheel->SetPosition(relativePos);
			}

			// Store initial rotation in wheel
			wheel->SetInitialRotation(runtimeInfo.m_initialRot);

			// Store current wheel state, reset to initial, then restore
			CVector currentWheelPos = wheel->GetPosition();
			Quaternion currentWheelRot = wheel->GetRotation();

			wheel->SetPosition(runtimeInfo.m_initialPos);
			wheel->SetRotation(runtimeInfo.m_initialRot);

			// Attach wheel to vehicle
			wheel->AttachToPhysicObj(this);

			// Restore wheel position
			wheel->SetPosition(currentWheelPos);
			wheel->SetRotation(currentWheelRot);

			// Count driven wheels
			if (wheel->m_driven)
			{
				m_numOfDrivenWheels++;
			}

			// Set wheel space ID
			wheel->TransferToSpace(m_spaceId);

			// Enable or disable wheel based on vehicle flags
			if (GetFlags() & 1)
			{
				wheel->SetVisible();
			}
			else {
				wheel->SetInvisible();
			}
		}

		// Restore original position and rotation
		this->SetPosition(oldPos);
		this->SetRotation(oldRot);

		// Reset vehicle controls
		SetThrottle(0.0f, true);
		m_steerRadians = 0.0f;

		// Send messages to radio manager if this is not a player-controlled vehicle
		if (ai::thePlayer &&
			ai::thePlayer->GetRadioManagerId() != -1 &&
			!m_bIsControlledByPlayer)
		{

			// Send three different message types (45, 46, 47)
			for (int messageType : {46, 47, 45})
			{
				m3d::AIParam param(messageType);
				ai::theProcessManager->PostMessageA(2, GetId(), ai::thePlayer->GetRadioManagerId(), 0.0f, param, {}, 1);
			}
		}

		// Create vehicle updater if needed
		if (!m_ownUpdater)
		{
			m_ownUpdater = new ai::VehicleUpdater(this);
		}

		// Final setup
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
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_KeepSteer(float elapsedTime)
	{
		// TODO: generated code
		for (auto& wheelInfo : m_wheels)
		{
			ai::Wheel* wheel = wheelInfo.GetWheel();
			if (!wheel)
				continue;

			// Adjust wheel parameters
			_AdjustWheel(wheelInfo);

			// Calculate target steering angle based on wheel's steering ratio
			float targetAngle = static_cast<float>(wheel->m_steering) * m_steerRadians;
			float currentAngle = wheel->m_curAngle;

			// Determine if we need to adjust steering speed based on current position
			float angleDifference = targetAngle - currentAngle;
			float steeringSpeed = m_steeringSpeed;

			// Check if we're close to the target (using dot product-like check)
			float proximityCheck = angleDifference * currentAngle;

			// If we're not very close to target, use dynamic steering speed
			if (fabs(proximityCheck) > 0.000001f)
			{
				// Use base steering speed
				steeringSpeed = m_steeringSpeed;
			}
			else
			{
				// Use dynamic steering speed that increases with current angle
				// This helps with centering and makes steering more responsive at larger angles
				float absCurrentAngle = fabs(currentAngle);
				steeringSpeed = (2.0f * absCurrentAngle + 1.0f) * m_steeringSpeed;
			}

			// Determine steering direction (1 for positive, -1 for negative)
			int steeringDirection = (angleDifference >= 0.0f) ? 1 : -1;

			// Calculate new steering angle based on steering speed and time
			float angleChange = steeringDirection * steeringSpeed * elapsedTime;
			float newAngle = currentAngle + angleChange;

			// Check if we would overshoot the target
			float newAngleDifference = newAngle - targetAngle;
			int newDirection;
			if (newAngleDifference > 0.000001f)
			{
				newDirection = 1;
			}
			else if (newAngleDifference < -0.000001f)
			{
				newDirection = -1;
			}
			else
			{
				newDirection = 0;
			}

			// Check original direction
			float originalDifference = currentAngle - targetAngle;
			int originalDirection;
			if (originalDifference > 0.000001f)
			{
				originalDirection = 1;
			}
			else if (originalDifference < -0.000001f)
			{
				originalDirection = -1;
			}
			else
			{
				originalDirection = 0;
			}

			// If we're changing direction (overshooting), clamp to target angle
			if (newDirection * originalDirection <= 0)
			{
				newAngle = targetAngle;
			}

			// Apply the steering angle change to the wheel
			float angleDelta = newAngle - currentAngle;
			_TurnWheelByAngle(wheel, angleDelta);

			// Update wheel's current angle
			wheel->m_curAngle = newAngle;
		}
	}

	void Vehicle::_UpdateOwnPhysics(float elapsedTime)
	{
		PhysicObj::_UpdateOwnPhysics(elapsedTime);
		const auto flags = GetFlags();
		if ((flags & 8) == 0 && (flags & 2) == 0 && !GetParentRepository())
		{
			m_ownUpdater->Update(elapsedTime);
		}
	}

	AI* Vehicle::GetAIPtr()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	Vehicle::~Vehicle()
	{
		if (m_bIsControlledByPlayer)
		{
			SetHorn(false);
		}

		// TODO: check this
		delete m_pPath;
		delete m_takingSphere;
		delete m_repository;
		delete m_groundRepository;
		delete m_ownUpdater;
		delete m_trailerJoint;
	}

	void Vehicle::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
	{
		RETRUXX_NOT_IMPLEMENTED;
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
		M3D_ASSERT(IsAlive());
		
		// TODO: check this!!
		const auto linearVelocity = GetLinearVelocity();
		if (m_numWheelsTouchingGround > 0)
		{
            const auto horizVel = sqrt(linearVelocity.z * linearVelocity.z + linearVelocity.x * linearVelocity.x);
            if (horizVel > 5.0)
            {
                const auto pressingForce = GetPrototypeInfo()->m_pressingForce;
                const auto mass = GetMass();

				CVector force;
                force.x = 0.0;
                force.y = mass * pressingForce * horizVel * -0.1962;
                force.z = 0.0;
                AddForce(force);
            }
		}

		const auto velocity = sqrt(linearVelocity.y * linearVelocity.y + linearVelocity.z * linearVelocity.z + linearVelocity.x * linearVelocity.x);
		if (m_numWheelsTouchingGround > 0)
		{
                if (const auto* wheel = GetFirstExistingWheel())
                {
                    const auto dir = GetDirection();
                    auto throttle = m_throttle * 0.5;
					auto direction = -1;
                    if ((((dir.y * linearVelocity.y) + (dir.z * linearVelocity.z)) + (dir.x * linearVelocity.x)) >= 0.0)
                    {
                        direction = 1;
                    }

					const CVector INITIAL_UP_DIRECTION = {0.0, 1.0, 0.0};

					CVector relDir;
                    relDir.x = (0.0 - INITIAL_UP_DIRECTION.x) * wheel->m_curAngle;
                    relDir.y = (0.0 - INITIAL_UP_DIRECTION.y) * wheel->m_curAngle;
                    relDir.z = (0.0 - INITIAL_UP_DIRECTION.z) * wheel->m_curAngle;

                    const auto mass = GetMass();
                    relDir.x = ((relDir.x * mass) * velocity) * m_driftCoeff;
                    relDir.y = ((relDir.y * mass) * velocity) * m_driftCoeff;
                    relDir.z = ((relDir.z * mass) * velocity) * m_driftCoeff;

                    const auto cabinControlCoeff = _GetCabinControlCoeff();

                    auto v21 = fabs(throttle) + 0.5;

					CVector force;
                    force.x = ((relDir.x * cabinControlCoeff) * direction) * v21;
                    force.y = ((relDir.y * cabinControlCoeff) * direction) * v21;
                    force.z = ((relDir.z * cabinControlCoeff) * direction) * v21;
                    AddRelTorque(force);
                }
		}

        m_numWheelsTouchingGround = 0;
	}

	void Vehicle::_UpdateAlarmStatus()
	{
		// TODO: implement Vehicle::_UpdateAlarmStatus
		for (auto& obstacle : m_currentNearbyObstacles)
		{
			// RETRUXX_NOT_IMPLEMENTED;
		}
	}

	m3d::Object* Vehicle::Clone()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_AttachExistingTrailer(Vehicle*, bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_InflictDamageToRepository(float)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Vehicle::_SetIdleMoveStatus()
	{
		if (!this->m_moveStatus)
		{
			return 0;
		}
		this->m_bIsMovingAlongExternalPath = 0;
		this->m_moveStatus = MOVE_IDLE;

		SetThrottle( 0.0, 1);
		if (m_pPath)
		{
			delete m_pPath;
		}
		this->m_pPath = 0;
		this->m_pathNum = -1;
		return 1;
	}

	CVector Vehicle::_CalcRepulsionForNearbyObjects(CVector const&, CVector const&, CVector const&, CVector const&,
		bool, CVector&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
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
			RETRUXX_NOT_IMPLEMENTED;
		}
		SetThrottle(0.0, 1);
		_KeepThrottle(0);
		_KeepGearBox(elapsedTime);
		_KeepSteer(elapsedTime);
		_KeepSuspension();
	}

	void Vehicle::_OnChangeCabin()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Vehicle::_bPassedPathPoint(CVector const& point, CVector const& nextPoint, bool bPrecisely) const
	{
		const auto vehiclePos =  GetPosition();

		ai::DrivingValues dv;
		CalcDrivingValues(*this, point, nextPoint, bPrecisely, dv);
			return dv.checkCircleRadius > sqrt(
				(float)(point.z - vehiclePos.z) * (float)(point.z - vehiclePos.z)
				+ 0.0 * 0.0
				+ (float)(point.x - vehiclePos.x) * (float)(point.x - vehiclePos.x))
			&& (float)((float)(vehiclePos.z * dv.checkLine.normal.z) + (float)(vehiclePos.x * dv.checkLine.normal.x)) > (float)((float)(dv.checkLine.origin.z * dv.checkLine.normal.z) + (float)(dv.checkLine.origin.x * dv.checkLine.normal.x));
	}

	void Vehicle::_CauseCustomGunPointedEvents()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_KeepSuspension()
	{
		// TODO: implement Vehicle::_KeepSuspension
		//RETRUXX_NOT_IMPLEMENTED;
	}

	CVector Vehicle::_GetNextPathPoint() const
	{
		// TODO: generated code
		// Get current path point
		CVector curPoint;
		if (!ai::GetPathItem(m_pPath, m_pathNum, curPoint))
		{
			// No valid path point found, return zero vector
			return ZeroVector;
		}

		CVector nextPoint;
		int pathSize = m_pPath->GetSize();

		// Determine the next point based on current position in path
		if (m_pathNum < pathSize - 1)
		{
			// Normal case: get next point in path
			ai::GetPathItem(m_pPath, m_pathNum + 1, nextPoint);
		}
		else if (m_pathNum > 0)
		{
			// At end of path: extrapolate from previous point
			CVector prevPoint;
			ai::GetPathItem(m_pPath, m_pathNum - 1, prevPoint);

			// Calculate direction from previous to current point
			CVector direction;
			direction.x = curPoint.x - prevPoint.x;
			direction.y = curPoint.y - prevPoint.y;
			direction.z = curPoint.z - prevPoint.z;

			// Normalize the direction vector
			CVector normalizedDir = direction.getNormalized();

			// Extrapolate next point by continuing in the same direction
			nextPoint.x = curPoint.x + normalizedDir.x;
			nextPoint.y = curPoint.y + normalizedDir.y;
			nextPoint.z = curPoint.z + normalizedDir.z;
		}
		else
		{
			// At start of path with no previous point, use current point
			nextPoint = curPoint;
		}

		// Check if next point is too close to current point (degenerate case)
		float distanceSq = (curPoint.x - nextPoint.x) * (curPoint.x - nextPoint.x) +
			(curPoint.y - nextPoint.y) * (curPoint.y - nextPoint.y) +
			(curPoint.z - nextPoint.z) * (curPoint.z - nextPoint.z);

		float distance = sqrt(distanceSq);

		if (distance < 0.01f)
		{
			// Points are too close, create an artificial offset
			nextPoint.x = curPoint.x + 1.0f;
			nextPoint.y = curPoint.y + 1.0f;
			nextPoint.z = curPoint.z + 1.0f;
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
            auto const isWrongWay = (wheelRpm > 5.0 && (RoughSign(m_engineRpm) * RoughSign(m_throttle) <= 0)) ||
                (wheelRpm <= 5.0 && ((direction.z * velocity.z + direction.y * velocity.y + direction.x * velocity.x) < -0.1 || RoughSign(m_throttle) == 0));

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

        const auto doApplyActions = [&](const ActionType& type)
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
            else if ((velocity.z * velocity.z + velocity.y * velocity.y + velocity.x * velocity.x) > 1.0)
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
		RETRUXX_NOT_IMPLEMENTED;
	}

	CStr Vehicle::_GetTrailerName() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	CVector Vehicle::_CalcRepulsionForObstacle(Obstacle const*, CVector const&, CVector const&, CVector const&,
		CVector const&, bool, CVector&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_DriveBySteeringForce(CVector const& steeringForce)
	{
		if (!m_inSmokeScreenMode || m_bIsControlledByPlayer)
		{
			const auto vehiclePos = GetPosition();
			const auto velocity = GetLinearVelocity();
			const auto direction = GetDirection();


			auto speed = (float)((float)((float)(direction.y * velocity.y) + (float)(direction.z * velocity.z)) + (float)(direction.x * velocity.x)) > 0.0;

			CVector point;
			point.x = steeringForce.x + vehiclePos.x;
			point.y = steeringForce.y + vehiclePos.y;
			point.z = steeringForce.z + vehiclePos.z;


			const auto angleTo = _GetAngleTo(point);
			auto steer = angleTo;
			auto throttle = fabs(angleTo);
			if (throttle < 2.5132742
				&& speed
				&& sqrt(velocity.y * velocity.y + velocity.z * velocity.z + velocity.x * velocity.x) > 8.333334)
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
				    if ((float)((float)((float)(velocity.y * velocity.y) + (float)(velocity.z * velocity.z))
                               + (float)(velocity.x * velocity.x)) < 1.0
                       && 0.0 != fabs((double)(m_steerRadians < 0.1)))
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
					steeringForce.x * steeringForce.x
					+ steeringForce.y * steeringForce.y
					+ steeringForce.z * steeringForce.z)
					* (4.0 - fabs(steer) * 2.7)
					* 0.25;
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
				steeringForce.x * steeringForce.x
				+ steeringForce.y * steeringForce.y
				+ steeringForce.z * steeringForce.z);
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
		// TODO: generated code
		auto anglea = (float)(0.0 - angle) * 0.5;
		auto wheelRelativeRot_4 = sin(anglea);
		auto wheelRelativeRot_12 = cos(anglea);
		auto Rotation = ai::PhysicObj::GetRotation();
		auto v5 = wheelRelativeRot_4;
		auto v6 = Rotation.w * 0.0;
		auto v7 = Rotation.y * 0.0;
		auto v8 = Rotation.x * 0.0;
		auto wheelRelativeRot = (float)((float)((float)(Rotation.x * wheelRelativeRot_12) + v6) + v7)
			- (float)(Rotation.z * wheelRelativeRot_4);
		auto v9 = Rotation.z * 0.0;
		auto wheelRelativeRot_4a = (float)((float)((float)(Rotation.y * wheelRelativeRot_12)
											  + (float)(Rotation.w * wheelRelativeRot_4))
									  + v9)
			- v8;
		auto wheelRelativeRot_8 = (float)((float)((float)(Rotation.z * wheelRelativeRot_12) + (float)(Rotation.x * v5)) + v6)
			- v7;
		auto wheelRelativeRot_12a = (float)((float)((float)(Rotation.w * wheelRelativeRot_12) - v8) - (float)(Rotation.y * v5))
			- v9;
		auto v10 = ai::PhysicObj::GetRotation();
		auto Inversed = v10.getInversed();
		auto v12 = wheelRelativeRot_4a;
		auto v13 = wheelRelativeRot;
		auto wheelRelativeRota = (float)((float)((float)(Inversed.x * wheelRelativeRot_12a)
											+ (float)(wheelRelativeRot_4a * Inversed.z))
									+ (float)(wheelRelativeRot * Inversed.w))
			- (float)(Inversed.y * wheelRelativeRot_8);
		auto wheelRelativeRot_4b = (float)((float)((float)(Inversed.x * wheelRelativeRot_8)
											  + (float)(Inversed.y * wheelRelativeRot_12a))
									  + (float)(wheelRelativeRot_4a * Inversed.w))
			- (float)(v13 * Inversed.z);
		auto v14 = wheelRelativeRot_8 * Inversed.z;
		auto wheelRelativeRot_8a = (float)((float)((float)(Inversed.y * v13) + (float)(wheelRelativeRot_12a * Inversed.z))
									  + (float)(wheelRelativeRot_8 * Inversed.w))
			- (float)(Inversed.x * v12);
		auto wheelRelativeRot_12b = (float)((float)((float)(wheelRelativeRot_12a * Inversed.w) - (float)(Inversed.x * v13))
									   - (float)(Inversed.y * v12))
			- v14;
		auto v15 = pWheel->GetRotation();
		auto v16 = (float)((float)((float)(wheelRelativeRot_8a * v15.x) + (float)(v15.y * wheelRelativeRot_12b))
					  + (float)(v15.w * wheelRelativeRot_4b))
			- (float)(wheelRelativeRota * v15.z);
		auto v17 = (float)((float)((float)(wheelRelativeRot_12b * v15.z) + (float)(v15.w * wheelRelativeRot_8a))
					  + (float)(v15.y * wheelRelativeRota))
			- (float)(wheelRelativeRot_4b * v15.x);
		auto v18 = (float)((float)((float)(v15.w * wheelRelativeRot_12b) - (float)(wheelRelativeRota * v15.x))
					  - (float)(v15.y * wheelRelativeRot_4b))
			- (float)(wheelRelativeRot_8a * v15.z);

		Quaternion v30;
		v30.x = (float)((float)((float)(wheelRelativeRot_12b * v15.x) + (float)(wheelRelativeRot_4b * v15.z))
						 + (float)(v15.w * wheelRelativeRota))
			- (float)(v15.y * wheelRelativeRot_8a);
		v30.y = v16;
		v30.z = v17;
		v30.w = v18;
		pWheel->SetRotation(v30);
	}

	CVector Vehicle::_GetCustomWeaponTargetPoint() const
	{
		RETRUXX_NOT_IMPLEMENTED;
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

	void Vehicle::_UpdateLockedObj(float)
	{
		if (!m_bRocketLaunchersPresent)
		{
			m_toBeLockedObjId = -1;
			m_lockedObjId = -1;
			m_timeToLockTarget = 0.0;
			return;
		}
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Vehicle::_bPointIsBehind(CVector const&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_GetOutOfDifficlultPlaceInternal()
	{
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_AdjustLookBox(bool, CVector const&, CVector const&, CVector const&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	CVector Vehicle::_CalcSteeringForceToPathPoint(CVector const& point, CVector const& nextPoint) const
	{
		// TODO: check and refactor this
		const auto vehiclePos = GetPosition();

		ai::DrivingValues dv;
		auto v8 = 1.0 / sqrt(dv.checkLine.normal.z * dv.checkLine.normal.z + 2.0);
		dv.checkLine.normal.x = v8;
		dv.checkLine.normal.y = v8;
		dv.checkLine.normal.z = dv.checkLine.normal.z * v8;
		memset(&dv.checkLine.origin, 0, sizeof(dv.checkLine.origin));

		CalcDrivingValues(*this, point, nextPoint, 1, dv);

	    auto v5 = point.z - vehiclePos.z;
		float v6 = 0.0;
		if (fabs(dv.nextAngle) <= 0.1570796370506287
			|| (
				dv.brakingCircleRadius <= sqrt(
					(float)(point.z - vehiclePos.z) * (float)(point.z - vehiclePos.z)
					+ 0.0 * 0.0
					+ (float)(point.x - vehiclePos.x) * (float)(point.x - vehiclePos.x))))
		{
			v6 = 1.0;
		}
		auto nextPointa = 1.0
			/ sqrt(
				v5 * v5
				+ 0.0 * 0.0
				+ (float)(point.x - vehiclePos.x) * (float)(point.x - vehiclePos.x)
				+ 0.00000011920929);

		CVector result;
		result.x = (float)(nextPointa * (float)(point.x - vehiclePos.x)) * v6;
		result.y = (float)(0.0 * nextPointa) * v6;
		result.z = (float)(v5 * nextPointa) * v6;
		return result;
	}

	void Vehicle::_TakeWaterIntoAccount(float elapsedTime)
	{
		const auto pos = GetPosition();
		const auto waterHeight = m3d::pClient->GetWorld().GetLandscape().getWaterHeight(pos.x * 0.03125, pos.z * 0.03125);
		if (waterHeight > m_size.y + pos.y)
		{
			RETRUXX_NOT_IMPLEMENTED;
		}
	}

	void Vehicle::_KeepGearBox(float elapsedTime)
	{
		// TODO: generated code
		// Calculate engine RPMs based on wheel rotation
		_CalcRpms();

		// Find first valid wheel
		auto wheelIter = std::find_if(m_wheels.begin(), m_wheels.end(),
									  [](const WheelRuntimeInfo& wheelInfo) { return wheelInfo.GetWheel() != nullptr; });

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
		if (m_bIsControlledByPlayer || m_attackStatus == 1)
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
			m_averageEngineRpm *= 0.1f; // Divide by 10
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
		// TODO: generated code
		if (bIsUpdatingByODE())
		{
			m_averageWheelAVel = 0.0f;

			// Get vehicle rotation and its inverse
			Quaternion vehicleRot = GetRotation();
			Quaternion invVehicleRot = vehicleRot.getInversed();

			int wheelCount = 0;

			// Process each wheel to calculate average angular velocity
			for (auto& wheelInfo : m_wheels)
			{
				ai::Wheel* wheel = wheelInfo.GetWheel();
				if (!wheel)
					continue;

				wheelCount++;

				// Get wheel's angular velocity in world space
				CVector wheelAngularVelWorld = wheel->GetAngularVelocity();

				// Convert quaternion to rotation matrix for transformation
				CMatrix rotMatrix;

				// Calculate rotation matrix from inverse quaternion
				float x = invVehicleRot.x;
				float y = invVehicleRot.y;
				float z = invVehicleRot.z;
				float w = invVehicleRot.w;

				float x2 = x * x;
				float y2 = y * y;
				float z2 = z * z;
				float xy = x * y;
				float xz = x * z;
				float yz = y * z;
				float wx = w * x;
				float wy = w * y;
				float wz = w * z;

				// Build rotation matrix from quaternion
				rotMatrix._11 = 1.0f - 2.0f * (y2 + z2);
				rotMatrix._12 = 2.0f * (xy + wz);
				rotMatrix._13 = 2.0f * (xz - wy);
				rotMatrix._14 = 0.0f;

				rotMatrix._21 = 2.0f * (xy - wz);
				rotMatrix._22 = 1.0f - 2.0f * (x2 + z2);
				rotMatrix._23 = 2.0f * (yz + wx);
				rotMatrix._24 = 0.0f;

				rotMatrix._31 = 2.0f * (xz + wy);
				rotMatrix._32 = 2.0f * (yz - wx);
				rotMatrix._33 = 1.0f - 2.0f * (x2 + y2);
				rotMatrix._34 = 0.0f;

				rotMatrix._41 = 0.0f;
				rotMatrix._42 = 0.0f;
				rotMatrix._43 = 0.0f;
				rotMatrix._44 = 1.0f;

				// Transform angular velocity from world space to vehicle local space
				float localVelX = wheelAngularVelWorld.x * rotMatrix._11 +
					wheelAngularVelWorld.y * rotMatrix._21 +
					wheelAngularVelWorld.z * rotMatrix._31;

				float localVelY = wheelAngularVelWorld.x * rotMatrix._12 +
					wheelAngularVelWorld.y * rotMatrix._22 +
					wheelAngularVelWorld.z * rotMatrix._32;

				float localVelZ = wheelAngularVelWorld.x * rotMatrix._13 +
					wheelAngularVelWorld.y * rotMatrix._23 +
					wheelAngularVelWorld.z * rotMatrix._33;

				// We're interested in the angular velocity in the vehicle's forward direction
				// Assuming X is forward, Z is up in vehicle space
				float forwardAngularVel = localVelX;  // X component in vehicle space

				// Calculate magnitude and preserve sign
				float angularVelMagnitude = fabs(forwardAngularVel);
				int directionSign = (forwardAngularVel >= 0.0f) ? 1 : -1;

				// Accumulate for average calculation
				m_averageWheelAVel += angularVelMagnitude * directionSign;
			}

			// Calculate average angular velocity
			if (wheelCount > 0)
			{
				m_averageWheelAVel /= static_cast<float>(wheelCount);
			}

			// Convert wheel angular velocity to engine RPM
			// Formula: RPM = (gear_ratio * diff_ratio * 108.0 * angular_velocity) / (2 * PI)
			// The constant 0.15915494 is 1/(2*PI) for conversion from radians to revolutions
			float gearRatio = GEAR_RATIOS[m_currentGear];
			m_engineRpm = (gearRatio * m_diffRatio * 108.0f * m_averageWheelAVel) * 0.15915494f;
		}
		else
		{
			// Use external updater for RPM calculation
			int dummy = 0;
			m_ownUpdater->CalcRpmsAndGear(m_averageWheelAVel, m_engineRpm, dummy);
		}
	}

	int Vehicle::_UpdateRepositoryOnChangeBasket()
	{
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

			const auto& size = protoInfo->GetRepositorySize();
			m_repository->SetGeomSize(size);

			const auto& slotPositions = protoInfo->GetSlotPositions();
			for (const auto& slot : slotPositions)
			{
				const auto& bounds = protoInfo->GetSlotBounds(slot.first, true);
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

		// TODO: check this
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
		// TODO: generated code
		Aabb myAabb;
		myAabb.m_box[0] = 0.0;
		myAabb.m_box[1] = 0.0;
		myAabb.m_box[2] = 0.0;
		myAabb.m_box[3] = 0.0;
		myAabb.m_box[4] = 0.0;
		myAabb.m_box[5] = 0.0;
		bool hasParts = false;

		// Iterate through all vehicle parts to calculate the overall bounding box
		for (const auto& partPair : m_vehicleParts)
		{
			VehiclePart* part = partPair.second;
			if (!part) continue;

			if (part->IsKindOf(&CompoundVehiclePart::m_classCompoundVehiclePart))
			{
				// Handle compound vehicle parts (contain multiple sub-parts)
				CompoundVehiclePart* compoundPart = dynamic_cast<CompoundVehiclePart*>(part);

				for (const auto& subPartPair : *compoundPart)
				{
					VehiclePart* subPart = subPartPair.second.vp;
					if (!subPart) continue;

					// Get part position and size
					CVector partPos = subPart->GetNodeRelativePosition();
					CVector partSize = subPart->GetSize();
					CVector halfSize;
					halfSize.x = partSize.x * 0.5f;
					halfSize.y = partSize.y * 0.5f;
					halfSize.z = partSize.z * 0.5f;

					// Calculate part's AABB in local space
					Aabb partAabb;
					partAabb.m_box[0] = partPos.x - halfSize.x;
					partAabb.m_box[1] = partPos.y - halfSize.y;
					partAabb.m_box[2] = partPos.z - halfSize.z;

					partAabb.m_box[3] = partPos.x + halfSize.x;
					partAabb.m_box[4] = partPos.y + halfSize.y;
					partAabb.m_box[5] = partPos.z + halfSize.z;

					// Expand the overall AABB to include this part
					if (!hasParts)
					{
						myAabb = partAabb;
						hasParts = true;
					}
					else
					{
						myAabb.m_box[0] = std::min(myAabb.m_box[0], partAabb.m_box[0]);
						myAabb.m_box[1] = std::min(myAabb.m_box[1], partAabb.m_box[1]);
						myAabb.m_box[2] = std::min(myAabb.m_box[2], partAabb.m_box[2]);
						myAabb.m_box[3] = std::max(myAabb.m_box[3], partAabb.m_box[3]);
						myAabb.m_box[4] = std::max(myAabb.m_box[4], partAabb.m_box[4]);
						myAabb.m_box[5] = std::max(myAabb.m_box[5], partAabb.m_box[5]);
					}
				}
			}
			else
			{
				// Handle regular vehicle parts
				// Get part position and size
				CVector partPos = part->GetNodeRelativePosition();
				CVector partSize = part->GetSize();
				CVector halfSize;
				halfSize.x = partSize.x * 0.5f;
				halfSize.y = partSize.y * 0.5f;
				halfSize.z = partSize.z * 0.5f;

				// Calculate part's AABB in local space
				Aabb partAabb;
				partAabb.m_box[0] = partPos.x - halfSize.x;
				partAabb.m_box[1] = partPos.y - halfSize.y;
				partAabb.m_box[2] = partPos.z - halfSize.z;

				partAabb.m_box[3] = partPos.x + halfSize.x;
				partAabb.m_box[4] = partPos.y + halfSize.y;
				partAabb.m_box[5] = partPos.z + halfSize.z;

				// Expand the overall AABB to include this part
				if (!hasParts)
				{
					myAabb = partAabb;
					hasParts = true;
				}
				else
				{
					myAabb.m_box[0] = std::min(myAabb.m_box[0], partAabb.m_box[0]);
					myAabb.m_box[1] = std::min(myAabb.m_box[1], partAabb.m_box[1]);
					myAabb.m_box[2] = std::min(myAabb.m_box[2], partAabb.m_box[2]);
					myAabb.m_box[3] = std::max(myAabb.m_box[3], partAabb.m_box[3]);
					myAabb.m_box[4] = std::max(myAabb.m_box[4], partAabb.m_box[4]);
					myAabb.m_box[5] = std::max(myAabb.m_box[5], partAabb.m_box[5]);
				}
			}
		}

		// If no parts were found, use a default size
		if (!hasParts)
		{
			myAabb.m_box[0] = -0.5f;
			myAabb.m_box[1] = -0.5f;
			myAabb.m_box[2] = -0.5f;
			myAabb.m_box[3] = 0.5f;
			myAabb.m_box[4] = 0.5f;
			myAabb.m_box[5] = 0.5f;
		}

		// Calculate vehicle size from AABB dimensions
		m_size.x = myAabb.m_box[3] - myAabb.m_box[0];
		m_size.y = myAabb.m_box[4] - myAabb.m_box[1];
		m_size.z = myAabb.m_box[5] - myAabb.m_box[2];

		// Set bumper point (front collision detection point)
		m_bumperPoint.x = 0.0f;  // Centered on X axis
		m_bumperPoint.y = m_size.y * 0.2f;  // 20% from front in Y direction
		m_bumperPoint.z = (m_size.z * 0.5f) + 0.1f;  // Slightly above center in Z direction

		// Update look box dimensions
		CVector lookBoxSize;
		lookBoxSize.x = m_size.x * 1.5f;  // 50% wider than vehicle
		lookBoxSize.y = m_size.y * 5.0f;  // 5 times longer for forward vision
		lookBoxSize.z = ai::theGlobProp.m_defaultLookBoxLength;  // Use global property

		m_lookBox->SetSize(lookBoxSize);

		// Update target box dimensions
		CVector targetBoxSize;
		targetBoxSize.x = m_size.x * 1.5f;  // 50% wider than vehicle
		targetBoxSize.y = m_size.y * 5.0f;  // 5 times longer for target detection
		targetBoxSize.z = ai::theGlobProp.m_defaultTargetBoxLength;  // Use global property

		m_targetBox->SetSize(targetBoxSize);
	}

	void Vehicle::_OnChangeBasket()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	CVector Vehicle::_GetLastPathPoint() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	float Vehicle::_GetAngleTo(CVector const& point) const
	{
		// TODO: check this and refactor
		const auto vehiclePos = GetPosition();
		auto v3 = point.z - vehiclePos.z;
		auto v4 = point.y - vehiclePos.y;
		auto v5 = point.x - vehiclePos.x;
		if (sqrt(v3 * v3 + v4 * v4 + v5 * v5) < 0.0099999998)
			return 0.0;
		auto v21 = 1.0 / sqrt(v3 * v3 + v4 * v4 + v5 * v5 + 0.00000011920929);
		auto v16 = v21 * v5;
		auto v17 = v4 * v21;
		auto v18 = v3 * v21;
		auto Rotation = ai::PhysicObj::GetRotation();
		auto Inversed = Rotation.getInversed();
		auto v9 = Inversed.w * Inversed.z;
		auto v10 = Inversed.x * Inversed.z;
		auto v11 = Inversed.w * Inversed.x;
		auto v24 = Inversed.x * Inversed.x;
		auto v22 = Inversed.x * Inversed.y;
		auto v25 = Inversed.z * Inversed.y;
		auto v12 = Inversed.z * Inversed.z;
		auto v19 = Inversed.w * Inversed.y;
		auto v13 = Inversed.y * Inversed.y;

		CMatrix vv;
		vv._11 = 1.0 - (float)((float)(v12 + v13) * 2.0);
		vv._21 = (float)(v22 - v9) * 2.0;
		vv._31 = (float)(v19 + v10) * 2.0;
		vv._12 = (float)(v9 + v22) * 2.0;
		vv._22 = 1.0 - (float)((float)(v12 + v24) * 2.0);
		vv._33 = 1.0 - (float)((float)(v13 + v24) * 2.0);
		vv._32 = (float)(v25 - v11) * 2.0;
		vv.m[0][2] = ((float)(v10 - v19) * 2.0);
		vv.m[0][3] = 0.0;
		vv.m[1][2] = ((float)(v11 + v25) * 2.0);
		vv.m[1][3] = 0.0;
		memset(&vv.m[2][3], 0, 16);
		vv._44 = 1.0;


		auto v14 = -0.99999899;
		auto v15 = (float)((float)(vv._33 * v18) + (float)(vv._23 * v17)) + (float)(vv._13 * v16);
		auto v20 = v15;
		if (v15 < -0.99999899 || (v14 = 0.99999899, v15 > 0.99999899))
			v20 = v14;
		auto v23 = -1;
		if ((float)((float)((float)(vv._31 * v18) + (float)(vv._21 * v17)) + (float)(vv._11 * v16)) >= 0.0)
			v23 = 1;
		return acos(v20) * (double)v23;
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
        v50.x = (float)((float)(invRot.x + (float)(invRot.z * 0.0)) + (float)(invRot.w * 0.0)) - (float)(invRot.y * 0.0);
        v50.y = (float)((float)(invRot.y + (float)(invRot.x * 0.0)) + (float)(invRot.w * 0.0)) - (float)(invRot.z * 0.0);
        axis.y = v50.y;
        v50.z = (float)((float)(invRot.z + (float)(invRot.y * 0.0)) + (float)(invRot.w * 0.0)) - (float)(invRot.x * 0.0);
        v50.w = (float)((float)(invRot.w - (float)(invRot.x * 0.0)) - (float)(invRot.y * 0.0)) - (float)(invRot.z * 0.0);
        axis.x = v50.x;
        axis.z = v50.z;
        auto w = v50.w;
        auto v48 = cos(v11);
        auto Rotation = m_wheel->GetRotation();
        v50.x = (float)((float)((float)(Rotation.x * w) + (float)(axis.y * Rotation.z)) + (float)(axis.x * Rotation.w)) - (float)(axis.z * Rotation.y);
        v50.y = (float)((float)((float)(Rotation.x * axis.z) + (float)(axis.y * Rotation.w)) + (float)(w * Rotation.y)) - (float)(axis.x * Rotation.z);
        auto v16 = w * Rotation.w;
        auto v17 = axis.z * Rotation.z;
        auto v18 = axis.y * Rotation.y;
        v50.z = (float)((float)((float)(Rotation.y * axis.x) + (float)(w * Rotation.z)) + (float)(axis.z * Rotation.w)) - (float)(Rotation.x * axis.y);
        auto v19 = (float)((float)(v16 - (float)(Rotation.x * axis.x)) - v18) - v17;
        axis.y = v50.y;
        v50.w = v19;
        axis.x = v50.x;
        axis.z = v50.z;
        w = v19;
        auto Inversed = wheelInfo.m_initialRot.getInversed();
        v50.x = (float)((float)((float)(axis.y * Inversed.z) + (float)(v19 * Inversed.x)) + (float)(axis.x * Inversed.w)) - (float)(axis.z * Inversed.y);
        v50.y = (float)((float)((float)(axis.y * Inversed.w) + (float)(axis.z * Inversed.x)) + (float)(v19 * Inversed.y)) - (float)(axis.x * Inversed.z);
        auto v21 = axis.z * Inversed.z;
        auto v22 = v19 * Inversed.w;
        auto v23 = axis.y * Inversed.y;
        v50.z = (float)((float)((float)(Inversed.y * axis.x) + (float)(w * Inversed.z)) + (float)(axis.z * Inversed.w)) - (float)(axis.y * Inversed.x);
        v50.w = (float)((float)(v22 - (float)(Inversed.x * axis.x)) - v23) - v21;
        auto v24 = (float)((float)((float)(rot.y * wheelDir.z) + (float)(v48 * rot.x)) + (float)(rot.w * wheelDir.x)) - (float)(rot.z * wheelDir.y);
        auto v25 = (float)((float)((float)(rot.y * v48) + (float)(rot.w * wheelDir.y)) + (float)(rot.z * wheelDir.x)) - (float)(wheelDir.z * rot.x);
        axis.x = v50.x;
        w = v50.w;
        axis.z = v50.z;
        auto v26 = (float)((float)((float)(rot.z * v48) + (float)(rot.w * wheelDir.z)) + (float)(wheelDir.y * rot.x)) - (float)(rot.y * wheelDir.x);
        axis.y = v50.y;
        auto v27 = (float)((float)((float)(v25 * v50.z) + (float)(v50.w * v24)) +
                      (float)((float)((float)((float)((float)(rot.w * v48) - (float)(rot.x * wheelDir.x)) - (float)(rot.y * wheelDir.y)) -
                                      (float)(rot.z * wheelDir.z)) *
                              v50.x)) -
            (float)(v26 * v50.y);
        auto v28 = (float)((float)((float)(v25 * v50.w) +
                              (float)((float)((float)((float)((float)(rot.w * v48) - (float)(rot.x * wheelDir.x)) - (float)(rot.y * wheelDir.y)) -
                                              (float)(rot.z * wheelDir.z)) *
                                      v50.y)) +
                      (float)(v26 * v50.x)) -
            (float)(v50.z * v24);
        auto v29 = (float)((float)((float)((float)((float)((float)((float)(rot.w * v48) - (float)(rot.x * wheelDir.x)) - (float)(rot.y * wheelDir.y)) -
                                              (float)(rot.z * wheelDir.z)) *
                                      v50.w) -
                              (float)(v24 * v50.x)) -
                      (float)(v25 * v50.y)) -
            (float)(v26 * v50.z);
        auto v30 = (float)((float)((float)(v26 * v50.w) +
                              (float)((float)((float)((float)((float)(rot.w * v48) - (float)(rot.x * wheelDir.x)) - (float)(rot.y * wheelDir.y)) -
                                              (float)(rot.z * wheelDir.z)) *
                                      v50.z)) +
                      (float)(v50.y * v24)) -
            (float)(v25 * v50.x);
        v50.x =
            (float)((float)((float)(v29 * wheelInfo.m_initialRot.x) + (float)(wheelInfo.m_initialRot.w * v27)) + (float)(wheelInfo.m_initialRot.z * v28)) -
            (float)(v30 * wheelInfo.m_initialRot.y);
        auto z = wheelInfo.m_initialRot.z;
        v50.y =
            (float)((float)((float)(v29 * wheelInfo.m_initialRot.y) + (float)(v30 * wheelInfo.m_initialRot.x)) + (float)(wheelInfo.m_initialRot.w * v28)) -
            (float)(wheelInfo.m_initialRot.z * v27);
        auto v32 = (float)((float)(v27 * wheelInfo.m_initialRot.y) + (float)(z * v29)) + (float)(wheelInfo.m_initialRot.w * v30);
        auto v33 = v28 * wheelInfo.m_initialRot.x;
        auto v34 = v28 * wheelInfo.m_initialRot.y;
        auto v35 = v32 - v33;
        auto x = wheelInfo.m_initialRot.x;
        v50.z = v35;
        v50.w = (float)((float)((float)(wheelInfo.m_initialRot.w * v29) - (float)(x * v27)) - v34) - (float)(wheelInfo.m_initialRot.z * v30);
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
        wheelDir.y = (float)((float)(v66._32 * wheelDir.z) + (float)(v66._22 * wheelDir.y)) + (float)(v66._12 * wheelDir.x);
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
        axis.x = (float)((float)((float)(v66._21 * wheelDir.y) + (float)(v66._31 * wheelDir.z)) + (float)(v66._11 * wheelDir.x)) + pos.x;
        axis.y = pos.y + (float)((float)((float)(v66._22 * wheelDir.y) + (float)(v66._32 * wheelDir.z)) + (float)(v66._12 * wheelDir.x));
        axis.z = pos.z + (float)((float)((float)(v66._23 * wheelDir.y) + (float)(v66._33 * wheelDir.z)) + (float)(v66._13 * wheelDir.x));
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
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_CheckForNearbyChests() const
	{
		if (m_bAllowPickUpMessage)
		{
			if (m_currentNumNearbyChests)
			{
				RETRUXX_NOT_IMPLEMENTED;
			}
			else
			{
				if (!m_pastNumNearbyChests)
				{
					return;
				}
				RETRUXX_NOT_IMPLEMENTED;
			}

			//M3D_APP->EnqueueMessage()
		}
	}

	void Vehicle::_AdjustTrailer()
	{
		if (m_trailerObjId >= 0)
		{
			RETRUXX_NOT_IMPLEMENTED;
		}
	}

	void Vehicle::_UpdateSeenObjAndWeapons(float elapsedTime)
	{
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

		auto* seenObj = theObjects->GetEntityByObjId(m_seenObjId);
        if (seenObj && IS_KIND_OF(seenObj, Wheel))
        {
            auto* wheel = RT_DYNCAST(seenObj, Wheel);
            auto* vehicle = wheel->GetVehicle();
            if (vehicle)
            {
                m_seenObjId = vehicle->GetId();
            }
			else
            {
                m_seenObjId = -1;
			}
        }

        if (m_seenObjId == GetId())
        {
            m_seenObjId = -1;
            seenNode = nullptr;
        }
		// TODO: check this
        if (!seenNode)
        {
            CMatrix mat;
            mat.rotYPR(M3D_APP->m_curCamera.m_rotYaw, M3D_APP->m_curCamera.m_rotPitch, M3D_APP->m_curCamera.m_rotRoll);

			const CVector INITIAL_OBJECTS_DIRECTION_4(0.0, 0.0, 1.0);
			lookAt.x = (((mat._13 * INITIAL_OBJECTS_DIRECTION_4.z) + (mat._11 * INITIAL_OBJECTS_DIRECTION_4.x)) + (INITIAL_OBJECTS_DIRECTION_4.y * mat._12)) *
                1000000.0;
            lookAt.y = (((mat._23 * INITIAL_OBJECTS_DIRECTION_4.z) + (mat._22 * INITIAL_OBJECTS_DIRECTION_4.y)) + (mat._21 * INITIAL_OBJECTS_DIRECTION_4.x)) *
                1000000.0;
            lookAt.z = (((mat._33 * INITIAL_OBJECTS_DIRECTION_4.z) + (mat._32 * INITIAL_OBJECTS_DIRECTION_4.y)) + (mat._31 * INITIAL_OBJECTS_DIRECTION_4.x)) *
                1000000.0;
        }

		if (!seenObj || !IS_KIND_OF(seenObj, Vehicle) && !IS_KIND_OF(seenObj, StaticAutoGun))
		{
            m_seenObjId = -1;
            seenNode = nullptr;
		}

		WeaponFirer::WeaponLookAtPoint(this, lookAt, elapsedTime);
        m_curLookAt.x = lookAt.x;
        m_curLookAt.y = lookAt.y;
        m_curLookAt.z = lookAt.z;
	}

	CVector Vehicle::_CalcSteeringForce(float elapsedTime)
	{
		// TODO: generated code
		// Return cached steering force if valid
		if (m_bCurSteeringForceValid)
		{
			return m_curSteeringForce;
		}

		// Get current position and velocity
		CVector pos = GetPosition();
		CVector vel = GetLinearVelocity();

		// Determine guide direction (use current direction if moving slowly, otherwise use velocity)
		CVector guide;
		if (vel.lengthSq() <= 1.0f)
		{
			guide = GetDirection(); // Use facing direction when moving slowly
		}
		else
		{
			guide = vel; // Use velocity direction when moving fast
		}

		// Initialize steering forces
		CVector attraction = ZeroVector; // Force towards target
		CVector repulsion = ZeroVector;  // Force away from obstacles

		// Calculate path following attraction force
		CVector curPoint;
		if (ai::GetPathItem(m_pPath, m_pathNum, curPoint))
		{
			CVector nextPoint = _GetNextPathPoint();
			CVector pathForce = _CalcSteeringForceToPathPoint(curPoint, nextPoint);
			attraction += pathForce;
		}

		// Add external destination force if in specific move status
		if (m_moveStatus == 2)
		{
			curPoint = ai::GetGroundPos(m_externalDestination, false, false);
			CVector destForce = _CalcSteeringForceToPathPoint(m_externalDestination, m_externalDestination);
			attraction += destForce;
		}

		// Add team-based steering forces if attacking
		if (m_attackStatus == 1)
		{
			ai::Team* team = static_cast<ai::Team*>(GetParent());
			if (team)
			{
				auto& steeringForceMap = team->GetSteeringForceMap();
				auto it = steeringForceMap.find(GetId());
				if (it != steeringForceMap.end())
				{
					attraction += it->second;
				}
			}
		}

		// Calculate obstacle avoidance repulsion force if not on external path
		if (!m_bIsMovingAlongExternalPath)
		{
			// Predict future position
			CVector predictedPos;
			predictedPos.x = pos.x + vel.x * ai::theGlobProp.m_predictionTime;
			predictedPos.y = pos.y + vel.y * ai::theGlobProp.m_predictionTime;
			predictedPos.z = pos.z + vel.z * ai::theGlobProp.m_predictionTime;

			// Calculate repulsion forces for different look directions
			_AdjustLookBox(true, pos, curPoint, guide);
			CVector repulsion1 = _CalcRepulsionForNearbyObjects(pos, predictedPos, vel, guide, true, attraction);

			_AdjustLookBox(false, pos, curPoint, guide);
			CVector repulsion2 = _CalcRepulsionForNearbyObjects(pos, predictedPos, vel, guide, false, attraction);

			repulsion = repulsion1 + repulsion2;
		}

		// Combine all steering forces
		CVector totalForce = attraction + repulsion;

		// Normalize and clamp the total force
		float forceMagnitude = totalForce.length();
		float clampedMagnitude = std::clamp(forceMagnitude, 0.0f, 1.0f);

		if (forceMagnitude > 0.0f)
		{
			totalForce.normalizeInplace();
			totalForce *= clampedMagnitude;
		}

		// Store the previous steering force for comparison
		CVector prevSteeringForce = m_curSteeringForce;

		// Update current steering force
		m_curSteeringForce = totalForce;

		// Handle stuck detection
		m_timeOutToCheckStuck -= elapsedTime;

		if (m_bWasStuck || (float)((float)((float)(this->m_curSteeringForce.z * prevSteeringForce.z)
										   + (float)(this->m_curSteeringForce.y * prevSteeringForce.y))
								   + (float)(prevSteeringForce.x * m_curSteeringForce.x)) >= 0.0)
		{
			// Not stuck or consistent steering direction
			// Reset timeout if we were previously stuck but now have consistent steering
			if (!m_bWasStuck)
			{
				m_timeOutToCheckStuck = 1.0f;
			}
		}
		else
		{
			// Inconsistent steering direction, reset stuck timer
			m_timeOutToCheckStuck = 1.0f;
		}

		// Check if vehicle is stuck (not moving significantly)
		if (m_timeOutToCheckStuck < 0.0f)
		{
			float moveDistanceSq = (pos - m_prevPosToCheckStuck).lengthSq();

			if (moveDistanceSq >= 0.1f)
			{
				// Vehicle has moved enough, not stuck
				m_prevPosToCheckStuck = pos;
				m_timeOutToCheckStuck = 1.0f;
			}
			else
			{
				// Vehicle is stuck
				m_bWasStuck = true;
				m_timeOutToCheckStuck = 2.0f; // Longer timeout when stuck
			}
		}

		// Mark steering force as valid and return result
		m_bCurSteeringForceValid = true;
		return m_curSteeringForce;
	}

	CVector Vehicle::_GetEtalonWheelAVel() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_DropChests()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_EvaluateToDead()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

    Vehicle::VehicleMoveStatus Vehicle::GetMoveStatus() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
