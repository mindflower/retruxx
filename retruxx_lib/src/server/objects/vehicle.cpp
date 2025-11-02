#define NOMINMAX

#include "vehicle.h"
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
#include "vehiclerecollection.h"
#include "engine/ode/sources/joint.h"
#include "guns/compoundgun.h"
#include "guns/rocketlauncher.h"
#include "guns/rocketvolleylauncher.h"
#include "server/externalpaths.h"
#include "server/intersectionmanager.h"
#include "server/path.h"
#include "server/weaponfirer.h"
#include "server/roles/VehicleRole.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetRandomSkin)
{
	RETRUXX_NOT_IMPLEMENTED;
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
	RETRUXX_NOT_IMPLEMENTED;
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
	RETRUXX_NOT_IMPLEMENTED;
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

            bool IsPointInFront(const CVector&)
            {
                RETRUXX_NOT_IMPLEMENTED;
            }

            void RenderDebugInfo(unsigned int)
            {
                RETRUXX_NOT_IMPLEMENTED;
            }
        }; /* size: 0x0018 */


        RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION(0x005D5680, FlatLine::CreateOrthogonal)
        FlatLine* FlatLine::CreateOrthogonal(FlatLine* ,const CVector&, const CVector&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        struct DrivingValues
        {
            /* 0x0000 */ FlatLine checkLine;
            /* 0x0018 */ float checkCircleRadius;
            /* 0x001c */ float nextAngle;
            /* 0x0020 */ float brakingCircleRadius;
        }; /* size: 0x0024 */


        float GetAngleBetween(CVector const&, CVector const&, CVector const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }
        RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION(0x005D07A0, GetAngleBetween)

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
            {
                dv.checkCircleRadius = dv.checkCircleRadius * 3.0;
            }
   
            auto const velocity = vehicle.GetLinearVelocity();
            auto const scalVelocity = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
            dv.brakingCircleRadius = fabs(dv.nextAngle) * (scalVelocity * log2(scalVelocity) * 0.04);
        }
        RETRUXX_DLL_INJECT_FUNCTION(0x005D57A0, CalcDrivingValues);
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

	VehiclePrototypeInfo::~VehiclePrototypeInfo()
	{
        RETRUXX_NOT_IMPLEMENTED;
	}

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
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
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

	bool Vehicle::SetPropertyById(int, m3d::AIParam const&)
	{
		RETRUXX_NOT_IMPLEMENTED;
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

    RETRUXX_DLL_INJECT_FUNCTION(0x005DCFD0, Vehicle::GetRecollectionPosition)
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
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
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

	bool Vehicle::FireFromWeaponByGunPartName(CStr const&, bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
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

	int Vehicle::GetPropertyId(char const*) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	float Vehicle::EstimateDamageAI(CVector const&, retruxx::vector<int, retruxx::allocator<int>>) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	float Vehicle::EstimateDamageAI() const
	{
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
	}

	NumericInRangeRegenerating<float>& Vehicle::Health()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Vehicle::GetOnOilMode() const
	{
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::SetAttackStatus(VehicleAttackStatus attackStatus)
	{
		this->m_attackStatus = attackStatus;
		if (!attackStatus)
			ai::WeaponFirer::FireFromWeaponsIfPossible(this, 0, {0.0, 0.0, 0.0}, 0);
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

    RETRUXX_DLL_INJECT_VIRTUAL_FUNCITON_NAMESPACED(0x005EAEE0, ai, Vehicle, RenderDebugInfo)
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
		RETRUXX_NOT_IMPLEMENTED;
	}

	NumericInRangeRegenerating<float> const& Vehicle::Fuel() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	float Vehicle::GetFuel() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int Vehicle::GetSeenObjId() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::GetGeoms(retruxx::vector<Geom*, retruxx::allocator<Geom*>>&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Vehicle::bIsControlledByPlayer() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::AIParam Vehicle::TakeOffAllGuns()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

    RETRUXX_DLL_INJECT_FUNCTION(0x005D1210, Vehicle::SetThrottle)
	void Vehicle::SetThrottle(float throttle, bool autoBrake)
	{
        if (fabs(throttle) <= 1.1)
        {
            m_throttle = throttle;
            m_brake = 0.0;
            m_bAutoBrake = false;
            if (fabs(throttle) > 0.001)
            {
                m_bHandBrake = 0;
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
		auto cabin = GetPartByName(CABIN);
		float maxSpeed = (cabin && cabin->IsKindOf(RT_CLASS_LOCAL(Cabin))) ? dynamic_cast<const Cabin*>(cabin)->GetMaxSpeed() : 0.0;

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
		RETRUXX_NOT_IMPLEMENTED;
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

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION_TYPED(0x005CBA60, Vehicle::GetCabin, Cabin const* (Vehicle::*)()const)
	Cabin const* Vehicle::GetCabin() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION_TYPED(0x005CB9D0, Vehicle::GetCabin, Cabin* (Vehicle::*)())
	Cabin* Vehicle::GetCabin()
	{
		RETRUXX_NOT_IMPLEMENTED;
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

	void Vehicle::SetRole(VehicleRole*)
	{
		RETRUXX_NOT_IMPLEMENTED;
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

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION(0x005CC1C0, Vehicle::GetSize)
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
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::PlaySoundOnRechargeWeapon()
	{
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
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

	void Vehicle::Update(float, unsigned)
	{
		// TODO: implement Vehicle::Update
		//RETRUXX_NOT_IMPLEMENTED;
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

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION_TYPED(0x005CBA00, Vehicle::GetBasket, Basket* (Vehicle::*)())
	Basket* Vehicle::GetBasket()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION_TYPED(0x005CBA90, Vehicle::GetBasket, Basket const*(Vehicle::*)()const)
	Basket const* Vehicle::GetBasket() const
	{
		RETRUXX_NOT_IMPLEMENTED;
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
			for (auto& obstacle : m_currentNearbyObstacles)
			{
				RETRUXX_NOT_IMPLEMENTED;
			}

			for (auto& obstacle : m_pastNearbyObstacles)
			{
				RETRUXX_NOT_IMPLEMENTED;
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
			CStr boneName = "LP_WHL" + CStr(wheelNumber) + wheelSide;

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

	void Vehicle::_KeepSteer(float)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_UpdateOwnPhysics(float)
	{
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_ApplyStabilizingForces()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_UpdateAlarmStatus()
	{
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
	}

	CVector Vehicle::_CalcRepulsionForNearbyObjects(CVector const&, CVector const&, CVector const&, CVector const&,
		bool, CVector&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_DeadActions(float)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_OnChangeCabin()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Vehicle::_bPassedPathPoint(CVector const&, CVector const&, bool) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_CauseCustomGunPointedEvents()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_KeepSuspension()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_CLASS_METHOD(0x005CCF40, Vehicle, _GetNextPathPoint)
	CVector Vehicle::_GetNextPathPoint() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

    RETRUXX_DLL_INJECT_CLASS_METHOD(0x005DAAE0, Vehicle, _KeepThrottle)
	void Vehicle::_KeepThrottle(bool applyActions)
	{
        auto const wheelRpm = fabs(m_averageWheelAVel) * 9.5492964;
        auto const velocity = GetLinearVelocity();
        if (m_bAutoBrake) 
        {
            CVector curPoint;
            auto const direction = GetDirection();
            auto const directionState = RoughSign(m_engineRpm);
            auto const isWrongWay =
                (direction.z * velocity.z + direction.y * velocity.y + direction.x * velocity.x) < -0.1 &&
                RoughSign(m_throttle) == 0;

            if (isWrongWay)
            {
                m_brake = 1.0;
            }
            else if (GetPathItem(m_pPath, m_pathNum, curPoint))
            {
                auto tempPoint = curPoint;
                tempPoint.y = M3D_KERNEL->GetEngineCfg().GetHeight(tempPoint.x, tempPoint.z);

                auto pos = GetPosition();
                pos.x = tempPoint.x - pos.x;
                pos.y = tempPoint.y - pos.y;
                pos.z = tempPoint.z - pos.z;

                auto const nextPoint = _GetNextPathPoint();
                auto const distanceToPoint = sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);
                auto const scalVelocity = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
                auto const offsetBase = 500;

                DrivingValues dv;
                CalcDrivingValues(*this, curPoint, nextPoint, true, dv);
                if (offsetBase / scalVelocity + distanceToPoint < dv.brakingCircleRadius)
                {
                    auto const steeringForce = _CalcSteeringForceToPathPoint(curPoint, nextPoint);
                    auto const scalSteeringForce = sqrt(steeringForce.x * steeringForce.x + steeringForce.y * steeringForce.y + steeringForce.z * steeringForce.z);
                    m_brake = 1 - pow(((scalSteeringForce * 0.5) + 0.5), 2);
                }
            }
        }

        if (RoughSign(m_throttle) == 0 &&
            sqrt(velocity.z * velocity.z + velocity.y * velocity.y + velocity.x * velocity.x) < 0.5)
        {
            m_bHandBrake = 1;
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

	void Vehicle::_DriveBySteeringForce(CVector const&)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_TurnWheelByAngle(Wheel*, float)
	{
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_ValidateVehicleParts()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_AdjustLookBox(bool, CVector const&, CVector const&, CVector const&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_CLASS_METHOD(0x005D62E0, Vehicle, _CalcSteeringForceToPathPoint)
	CVector Vehicle::_CalcSteeringForceToPathPoint(CVector const&, CVector const&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_TakeWaterIntoAccount(float)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_KeepGearBox(float)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_SetIdleMoveStatusAndCauseTargetReached()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_CalcRpms()
	{
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
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

	float Vehicle::_GetAngleTo(CVector const&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_AdjustWheel(WheelRuntimeInfo&)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Object* Vehicle::CreateObject()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_CheckForNearbyChests() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_AdjustTrailer()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Vehicle::_UpdateSeenObjAndWeapons(float)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	CVector Vehicle::_CalcSteeringForce(float) const
	{
		RETRUXX_NOT_IMPLEMENTED;
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
