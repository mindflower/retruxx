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
#include "guns/compoundgun.h"
#include "guns/rocketlauncher.h"
#include "guns/rocketvolleylauncher.h"
#include "server/externalpaths.h"
#include "server/path.h"
#include "server/weaponfirer.h"
#include "server/roles/VehicleRole.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetRandomSkin)
{
	throw std::logic_error("Not implemented");
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
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetBasket)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetChassis)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetHealth)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetMaxHealth)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetFuel)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetMaxFuel)
{
	throw std::logic_error("Not implemented");
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
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, PlaceToEndOfPath)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetThrottle)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetThrottle)
{
	throw std::logic_error("Not implemented");
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
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetSteer)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, FireFromWeaponCustom)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, FireFromWeaponCustom2)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, HoldFire)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetCustomControlWeapons)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetCustomControlWeapons)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetCustomControlWeaponsTarget)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetCustomControlWeaponsTarget)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetCustomControlWeaponsTargetObj)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetCustomControlWeaponsTargetObj)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetCustomLinearVelocity)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, AddItemsToRepository)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, RemoveItemsFromRepository)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, HasAmountOfItemsInRepository)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, CanPlaceItemsToRepository)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, AddObjectToRepository)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, TakeOffAllGuns)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, AttachTrailer)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, DetachTrailer)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, TrailerExists)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetTrailer)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, getGodMode)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, setGodMode)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, getImmortalMode)
{
	throw std::logic_error("Not implemented");
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
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetHorn)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetVisible)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetInvisible)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetMaxTorque)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetMaxTorque)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetMaxSpeed)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetMaxSpeed)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetCruisingSpeed)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetCruisingSpeed)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, LimitMaxSpeed)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, UnlimitMaxSpeed)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetForcedMaxTorque)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, ResetForcedMaxTorque)
{
	throw std::logic_error("Not implemented");
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
                throw std::logic_error("Not implemented");
            }

            void RenderDebugInfo(unsigned int)
            {
                throw std::logic_error("Not implemented");
            }
        }; /* size: 0x0018 */


        RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION(0x005D5680, FlatLine::CreateOrthogonal)
        FlatLine* FlatLine::CreateOrthogonal(FlatLine* ,const CVector&, const CVector&)
        {
            throw std::logic_error("Not implemented");
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
            throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
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
            if (!wheelsNode->IsEmpty())
            {
                if (!m_parentPrototypeName.empty())
                {
                    M3D_LOG_ERR("Error: wheels info is present for inherited vehicle '" + m_prototypeName + "'");
                    M3D_CRITICAL_ERROR("");
                }

                m_wheelInfos.clear();
                ref_ptr wheelNode = xmlFile->CreateNode();
                for (wheelsNode->GetFirstChild(wheelNode, "Wheel"); wheelNode->IsEmpty(); wheelNode->GetNextSibling(wheelNode, "Wheel"))
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
        throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	Wheel* Vehicle::WheelRuntimeInfo::GetWheel()
	{
		throw std::logic_error("Not implemented");
	}

	Vehicle::WheelRuntimeInfo::WheelRuntimeInfo(Wheel*)
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::WheelRuntimeInfo::IsWheelPresent() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::WheelRuntimeInfo::SetWheel(Wheel*)
	{
		throw std::logic_error("Not implemented");
	}

	float Vehicle::GetCruisingSpeed() const
	{
		return this->m_cruisingSpeed;
	}

	void Vehicle::UnlimitMaxSpeed()
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::IncStoppageMode()
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::bRocketLaunchersPresent() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::RecalcGadgets()
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::HoldFire(int)
	{
		throw std::logic_error("Not implemented");
	}

	VehicleRecollection* Vehicle::GetRecollection() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetLinearVelocity(CVector const&)
	{
		throw std::logic_error("Not implemented");
	}

	Vehicle::CustomWeaponControlType Vehicle::GetCustomControlWeapons() const
	{
		throw std::logic_error("Not implemented");
	}

	Quaternion Vehicle::GetWheelInitialRotation(unsigned)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::ActivateHeadLights(bool)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::Flow(Obj*, float)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::DetachTrailer()
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::AddChild(Obj*)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::GetHorn() const
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::IsTrailer() const
	{
		throw std::logic_error("Not implemented");
	}

	float Vehicle::GetCameraHeight() const
	{
		return this->m_cameraHeight;
	}

	int Vehicle::GetLockedObjId() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::setGodMode(bool)
	{
		throw std::logic_error("Not implemented");
	}

	retruxx::map<int, Gadget*, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, Gadget*>>> const& Vehicle::GetGadgets() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetRandomSkin()
	{
		throw std::logic_error("Not implemented");
	}

	float Vehicle::GetDefaultCruisingSpeed() const
	{
		throw std::logic_error("Not implemented");
	}

	float Vehicle::GetMaxEngineRpm() const
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	Chassis* Vehicle::GetChassis()
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	float Vehicle::GetCameraMaxDist() const
	{
		return this->m_cameraMaxDist;
	}

	bool Vehicle::SetPropertyById(int, m3d::AIParam const&)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::DecOnOilMode()
	{
		throw std::logic_error("Not implemented");
	}

	unsigned Vehicle::GetNumWheels() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetHandBrake()
	{
		throw std::logic_error("Not implemented");
	}

    RETRUXX_DLL_INJECT_FUNCTION(0x005DCFD0, Vehicle::GetRecollectionPosition)
	CVector Vehicle::GetRecollectionPosition(float) const
	{
        throw std::logic_error("Not implemented");
        //TODO: implement recollection logic
        auto center = GetGeometricCenter();
        return center;
	}

	float Vehicle::GetSteer() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::IncNumWheelsTouchingGround()
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	m3d::Class* Vehicle::GetBaseClass()
	{
		return RT_CLASS_LOCAL(ComplexPhysicObj);
	}

	float Vehicle::GetMaxFuel() const
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::getImmortalMode() const
	{
		throw std::logic_error("Not implemented");
	}

	float Vehicle::GetHealth() const
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::AddThing(GeomRepositoryItem const&, bool)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::EnableSounds(bool)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::TransferToSpace(dxSpace*)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::AIParam Vehicle::VehicleAIOnAttack(Obj*)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetCanBeDistractedFromMoving(bool)
	{
		throw std::logic_error("Not implemented");
	}

	int Vehicle::GetCustomControlWeaponsTargetObj() const
	{
		throw std::logic_error("Not implemented");
	}

	int Vehicle::GetToBeLockedObjId() const
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	void Vehicle::FireFromWeaponCustom(bool, CVector const&, Obj*)
	{
		throw std::logic_error("Not implemented");
	}

	float Vehicle::GetMass() const
	{
		throw std::logic_error("Not implemented");
	}

	unsigned Vehicle::GetPrice(IPriceCoeffProvider const*) const
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::FireFromWeaponByGunId(int, bool)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetMoveStatus(VehicleMoveStatus moveStatus)
	{
		this->m_moveStatus = moveStatus;
	}

	void Vehicle::SetPassedToAnotherMapStatus()
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	int Vehicle::GetCurrentGear() const
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetCustomControlWeapons(int)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetCustomControlWeapons(CustomWeaponControlType)
	{
		throw std::logic_error("Not implemented");
	}

	unsigned Vehicle::GetSchwarz() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::UnsubscribeRadioManagerFromNearbyObjId(int) const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::Blow(Obj*)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetHorn(bool)
	{
		// TODO: implement Vehicle::SetHorn
		//throw std::logic_error("Not implemented");
	}

	int Vehicle::SetExternalPathByName(char const* pathName)
	{
        auto path = pServer->GetExternalPaths()->GetPath(pathName);
		return SetExternalPath(path);
	}

	void Vehicle::IncInSmokeScreenMode()
	{
		throw std::logic_error("Not implemented");
	}

	unsigned char Vehicle::GetPriority() const
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::getGodMode() const
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::FireFromWeaponByGunPartName(CStr const&, bool)
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	float Vehicle::GetMaxHealth() const
	{
		throw std::logic_error("Not implemented");
	}

	int Vehicle::GetPropertyId(char const*) const
	{
		throw std::logic_error("Not implemented");
	}

	float Vehicle::EstimateDamageAI(CVector const&, retruxx::vector<int, retruxx::allocator<int>>) const
	{
		throw std::logic_error("Not implemented");
	}

	float Vehicle::EstimateDamageAI() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::PickUpNearbyObjects(bool, unsigned&, retruxx::vector<int, retruxx::allocator<int>>&)
	{
		throw std::logic_error("Not implemented");
	}

	float Vehicle::GetFullDurability() const
	{
		throw std::logic_error("Not implemented");
	}

	NumericInRangeRegenerating<float> const& Vehicle::Health() const
	{
		throw std::logic_error("Not implemented");
	}

	NumericInRangeRegenerating<float>& Vehicle::Health()
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::GetOnOilMode() const
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::bIsMovingAlongExternalPath() const
	{
		return m_bIsMovingAlongExternalPath;
	}

	void Vehicle::DisableGeometry(bool)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetSteer(float radians)
	{
		this->m_steerRadians = radians;
	}

	float Vehicle::GetControl() const
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::AddItemsToRepository(char const*, int)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::ResetForcedMaxTorque()
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::ClearSavedStatus()
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::HealWheels()
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::setImmortalMode(bool bImmortal)
	{
		this->m_bImmortalMode = bImmortal;
	}

	bool Vehicle::bIsBraking() const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::AIParam Vehicle::VehicleAIOnMove(Obj*)
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::IsHealthZero() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::FireFromWeaponAI(bool, float, Obj*)
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::AddGadget(Gadget*)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetCustomControlWeaponsTargetObj(int)
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::AddObjectToRepository(Obj*)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::ResetPositionAndRotation()
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::CanPlaceItemsToRepository(char const*, int)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::AttachTrailer(char const*)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetTurboThrottleValue(float)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetMaxPower(float)
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::GetStoppageMode() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::GetEnemiesInNeighborhood(float, retruxx::vector<int, retruxx::allocator<int>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	VehicleRole* Vehicle::GetRole() const
	{
        return RT_DYNCAST(theObjects->GetEntityByObjId(m_roleId), VehicleRole);
	}

	void Vehicle::SubscribeRadioManagerOnNearbyObjId(int) const
	{
		throw std::logic_error("Not implemented");
	}

	int Vehicle::GetValidSlotIdForGadget(Gadget const*) const
	{
		throw std::logic_error("Not implemented");
	}

	Wheel const* Vehicle::GetFirstExistingWheel() const
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::TrailerExists() const
	{
		throw std::logic_error("Not implemented");
	}

	float Vehicle::GetAverageEngineRpm() const
	{
		throw std::logic_error("Not implemented");
	}

	CVector Vehicle::GetCustomControlWeaponsTarget() const
	{
		throw std::logic_error("Not implemented");
	}

	float Vehicle::GetMaxTorque() const
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	void Vehicle::DecInSmokeScreenMode()
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetTrailer()
	{
		throw std::logic_error("Not implemented");
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
		//throw std::logic_error("Not implemented");
	}

	void Vehicle::SetVisible()
	{
		throw std::logic_error("Not implemented");
	}

	int Vehicle::GetNpcMotionControllerId() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetIndexInTeam(int indexInTeam)
	{
		m_indexInTeam = indexInTeam;
	}

	NumericInRangeRegenerating<float>& Vehicle::Fuel()
	{
		throw std::logic_error("Not implemented");
	}

	NumericInRangeRegenerating<float> const& Vehicle::Fuel() const
	{
		throw std::logic_error("Not implemented");
	}

	float Vehicle::GetFuel() const
	{
		throw std::logic_error("Not implemented");
	}

	int Vehicle::GetSeenObjId() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::GetGeoms(retruxx::vector<Geom*, retruxx::allocator<Geom*>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::bIsControlledByPlayer() const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::AIParam Vehicle::TakeOffAllGuns()
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	float Vehicle::GetTurboThrottleTime() const
	{
		throw std::logic_error("Not implemented");
	}

	float Vehicle::GetFullDurabilityCoeffForDamageType(DamageType) const
	{
		throw std::logic_error("Not implemented");
	}

	Obj* Vehicle::CloneObj()
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	float Vehicle::GetEngineRpm() const
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION_TYPED(0x005CBA60, Vehicle::GetCabin, Cabin const* (Vehicle::*)()const)
	Cabin const* Vehicle::GetCabin() const
	{
		throw std::logic_error("Not implemented");
	}

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION_TYPED(0x005CB9D0, Vehicle::GetCabin, Cabin* (Vehicle::*)())
	Cabin* Vehicle::GetCabin()
	{
		throw std::logic_error("Not implemented");
	}

	float Vehicle::GetTurboThrottleValue() const
	{
		throw std::logic_error("Not implemented");
	}

	int Vehicle::GetMaxGadgets(CStr const&) const
	{
		throw std::logic_error("Not implemented");
	}

	float Vehicle::GetMaxPower() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::DecStoppageMode()
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::ShowVehicle(bool)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetRole(VehicleRole*)
	{
		throw std::logic_error("Not implemented");
	}

	retruxx::set<ref_ptr<Obstacle>, retruxx::less<ref_ptr<Obstacle>>, retruxx::allocator<ref_ptr<Obstacle>>> const& Vehicle::
	GetNearbyObstacles() const
	{
		throw std::logic_error("Not implemented");
	}

	CStr Vehicle::GetPropertyName(int) const
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::RemoveItemsFromRepository(char const*, int)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetCustomControlWeaponsTarget(CVector const&)
	{
		throw std::logic_error("Not implemented");
	}

	float Vehicle::GetTimeToLockTarget() const
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	void Vehicle::LimitMaxSpeed(float)
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	Vehicle::VehicleAttackStatus Vehicle::GetAttackStatus() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::ReleaseAllPedals()
	{
		SetThrottle(0.0, 0);
		m_brake = GetPrototypeInfo()->m_selfBrakingCoeff;
	}

	bool Vehicle::GetInSmokeScreenMode() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::PlaySoundOnRechargeWeapon()
	{
		throw std::logic_error("Not implemented");
	}

	Vehicle* Vehicle::GetTrailer() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::IncOnOilMode()
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetNpcMotionControllerId(int)
	{
		throw std::logic_error("Not implemented");
	}

	VehiclePrototypeInfo const* Vehicle::GetPrototypeInfo() const
	{
		return dynamic_cast<VehiclePrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
	}

	void Vehicle::PlaceToEndOfPath()
	{
		throw std::logic_error("Not implemented");
	}

	int Vehicle::GetIndexInTeam() const
	{
		throw std::logic_error("Not implemented");
	}

	CVector Vehicle::GetBumperPoint() const
	{
		throw std::logic_error("Not implemented");
	}

	GeomRepository* Vehicle::GetGroundRepository() const
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::ApplyModifier(Modifier const&)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetTurningToGroundForceAndTorque(CVector const&, CVector const&, CVector const&)
	{
		throw std::logic_error("Not implemented");
	}

	Wheel* Vehicle::GetWheel(unsigned)
	{
		throw std::logic_error("Not implemented");
	}

	Wheel const* Vehicle::GetWheel(unsigned) const
	{
		throw std::logic_error("Not implemented");
	}

	float Vehicle::GetThrottle() const
	{
		throw std::logic_error("Not implemented");
	}

	IzvratRepository const* Vehicle::GetRepository() const
	{
		throw std::logic_error("Not implemented");
	}

	IzvratRepository* Vehicle::GetRepository()
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::RemoveChild(Obj*)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetBrake(float)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::AIParam Vehicle::VehicleAIOnDead(Obj*)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetMaxSpeed(float)
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::HasAmountOfItemsInRepository(char const*, int) const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetTurboThrottleTime(float)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::Update(float, unsigned)
	{
		// TODO: implement Vehicle::Update
		//throw std::logic_error("Not implemented");
	}

	float Vehicle::GetCollisionRadius() const
	{
		throw std::logic_error("Not implemented");
	}

	float Vehicle::GetDriftCoeff() const
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	float Vehicle::GetMaxFullDurability() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetInvisible()
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetCustomLinearVelocity(float)
	{
		throw std::logic_error("Not implemented");
	}

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION_TYPED(0x005CBA00, Vehicle::GetBasket, Basket* (Vehicle::*)())
	Basket* Vehicle::GetBasket()
	{
		throw std::logic_error("Not implemented");
	}

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION_TYPED(0x005CBA90, Vehicle::GetBasket, Basket const*(Vehicle::*)()const)
	Basket const* Vehicle::GetBasket() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetCabin(VehiclePart*)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::IntersectWithWorld() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetForcedMaxTorque(float)
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::DriveToPoint(CVector const&, CVector const&, bool, float)
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_PutContour()
	{
		throw std::logic_error("Not implemented");
	}

	namespace
	{
		bool bMustTakeScreenShot = false;
	}

	void Vehicle::_InternalCreateVisualPart()
	{
		// TODO: generated code
		// Call parent implementation
		ai::ComplexPhysicObj::_InternalCreateVisualPart();

		// Initialize vehicle properties
		this->m_maxSpeedLimited = 0;
		this->m_maxTorqueForced = 0;

		// Validate and set skin
		int validSkin = ai::Vehicle::CheckSkin(GetSkin());
		this->SetSkin(validSkin);

		// Set global screenshot flag
		ai::bMustTakeScreenShot = 1;

		// Get prototype information
		const ai::PrototypeInfo* prototypeInfo = this->GetPrototypeInfo();

		// Find chassis part
		Chassis* chassis = nullptr;
		VehiclePart* chassisPart = ai::ComplexPhysicObj::GetPartByName("CHASSIS");
		if (chassisPart && chassisPart->IsKindOf(&ai::Chassis::m_classChassis))
		{
			chassis = dynamic_cast<Chassis*>(chassisPart);
		}

		// Find cabin part
		Cabin* cabin = nullptr;
		VehiclePart* cabinPart = ai::ComplexPhysicObj::GetPartByName("CABIN");
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


		throw std::logic_error("Not implemented");

		/*
		// Set up engine sound if cabin exists and has engine sound
		if (cabin) {
			const char* engineSoundName = cabin->GetPrototypeInfo()->m_engineHighSoundName.c_str();
			if (engineSoundName && strlen(engineSoundName) > 0) {
				// Create engine sound node
				m3d::SgSoundSourceNode* soundNode = ai::PhysicBody::CreateNode(
					cabin->GetPrototypeInfo()->m_engineHighSoundName, 0, 0, 0, false);
				this->m_engineHighSoundNode = soundNode;

				// Attach sound node to chassis
				chassis->m_Node->AddChild(chassis->m_Node, soundNode);
			}
		}

		// Get animated models server and chassis model name
		m3d::AnimatedModelsServer* animatedModelsServer = m3d::Application::g_pApp->m_serverAnimatedModels;
		std::string chassisModelName = chassis->m_modelname;

		// Store current position and rotation
		CVector oldPos;
		Quaternion oldRot;
		ai::PhysicObj::GetPosition(this, &oldPos);
		ai::PhysicObj::GetRotation(this, &oldRot);

		// Reset to origin for setup
		this->SetPosition(&CVector::Zero);
		this->SetRotation(&Quaternion::Identity);

		// Set up suspension nodes for all wheels
		for (size_t i = 0; i < m_wheels.size(); ++i) {
			WheelRuntimeInfo& wheelInfo = m_wheels[i];
			Wheel* wheel = wheelInfo.m_wheel;

			if (!wheel) continue;

			// Create suspension node for the wheel
			ai::Wheel::CreateSuspensionNode(wheel);

			if (wheel->m_suspensionNode) {
				// Determine wheel side (L/R) and number
				std::string wheelSide = (i % 2 == 0) ? "L" : "R";
				int wheelNumber = (i / 2) + 1;

				// Build suspension load point name (e.g., "LP_SSP1L")
				std::string suspensionLpName = "LP_SSP" + std::to_string(wheelNumber) + wheelSide;

				// Find chassis again for attachment
				Chassis* currentChassis = nullptr;
				VehiclePart* currentChassisPart = ai::ComplexPhysicObj::GetPartByName(this, "CHASSIS");
				if (currentChassisPart && m3d::Object::IsKindOf(currentChassisPart, &ai::Chassis::m_classChassis)) {
					currentChassis = static_cast<Chassis*>(currentChassisPart);
				}

				if (currentChassis) {
					// Attach suspension node to chassis
					currentChassis->m_Node->AddChild(currentChassis->m_Node, wheel->m_suspensionNode);

					// Get suspension position from bone matrix
					CMatrix boneMatrix;
					if (m3d::AnimatedModelsServer::GetBoneMatrixByNameFromModelName(
						animatedModelsServer,
						chassisModelName.c_str(),
						suspensionLpName.c_str(),
						&boneMatrix,
						0)) {

						// Extract position and rotation from bone matrix
						CVector suspensionOrigin(
							boneMatrix.m[3][0],
							boneMatrix.m[3][1],
							boneMatrix.m[3][2]
						);

						Quaternion suspensionRot;
						Quaternion::FromMatrix(&suspensionRot, &boneMatrix);

						// Set suspension node transform
						m3d::SgNode::SetOriginAbs(wheel->m_suspensionNode, &suspensionOrigin);
						m3d::SgNode::SetRotation(wheel->m_suspensionNode, &suspensionRot);
						wheel->m_suspensionNode->UpdateXForm(wheel->m_suspensionNode, 0, 1);
					}
					else {
						// Fallback: set to zero position and log error
						m3d::SgNode::SetOriginAbs(wheel->m_suspensionNode, &CVector::Zero);

						std::string errorMsg = "Error: LoadPoint not found: " + suspensionLpName +
							" for model '" + chassisModelName + "'";
						m3d::Log::logTex(m3d::g_Kernel->m_Log, errorMsg.c_str(), LOG_ERR);
					}
				}
			}
		}

		// Restore original position and rotation
		this->SetPosition(&oldPos);
		this->SetRotation(&oldRot);

		// Create visual parts for all wheels and update their transforms
		for (size_t i = 0; i < m_wheels.size(); ++i) {
			Wheel* wheel = m_wheels[i].m_wheel;
			if (!wheel) continue;

			// Create visual representation for wheel
			ai::Obj::CreateVisualPart(wheel);

			// Transfer physics parameters to scene graph
			wheel->TransferPhysicParamsToSceneGraphNode(wheel);

			// Update transforms
			wheel->m_physicBody->m_Node->UpdateXForm(wheel->m_physicBody->m_Node, 0, 1);
			if (wheel->m_suspensionNode) {
				wheel->m_suspensionNode->UpdateXForm(wheel->m_suspensionNode, 0, 1);
			}
		}

		// Transfer vehicle physics parameters to scene graph
		this->TransferPhysicParamsToSceneGraphNode(this);

		// Perform world intersection test
		ai::Vehicle::IntersectWithWorld(this);

		// Set up effect actions for basket and cabin if conditions are met
		unsigned int flags = this->m_flags;
		if ((flags & 8) == 0 && (flags & 2) == 0 && !ai::Obj::GetParentRepository(this)) {
			// Set up basket effect actions
			VehiclePart* basketPart = ai::ComplexPhysicObj::GetPartByName(this, "BASKET");
			if (basketPart && m3d::Object::IsKindOf(basketPart, &ai::Basket::m_classBasket)) {
				Basket* basket = static_cast<Basket*>(basketPart);
				ai::PhysicBody::SetEffectActions(basket, &this->m_effectActions);
				if (!this->m_effectActions.empty()) {
					basket->SetNodeAnimAction(basket, this->m_effectActions[0], 1);
				}
			}

			// Set up cabin effect actions
			if (cabin) {
				ai::PhysicBody::SetEffectActions(cabin, &this->m_effectActions);
				if (!this->m_effectActions.empty()) {
					cabin->SetNodeAnimAction(cabin, this->m_effectActions[0], 1);
				}
			}
		}

		// Handle contouring if enabled
		if (ai::ComplexPhysicObj::bIsContoured()) {
			ai::ComplexPhysicObj::PutContour();
		}
		*/
	}

	bool Vehicle::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_KeepSteer(float)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_UpdateOwnPhysics(float)
	{
		throw std::logic_error("Not implemented");
	}

	AI* Vehicle::GetAIPtr()
	{
		throw std::logic_error("Not implemented");
	}

	Vehicle::~Vehicle()
	{
	}

	void Vehicle::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
	{
		throw std::logic_error("Not implemented");
	}

	float Vehicle::_GetTimeOutForNextIntersectionWithWorld() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_ApplyStabilizingForces()
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_UpdateAlarmStatus()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* Vehicle::Clone()
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_AttachExistingTrailer(Vehicle*, bool)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_InflictDamageToRepository(float)
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::_SetIdleMoveStatus()
	{
		throw std::logic_error("Not implemented");
	}

	CVector Vehicle::_CalcRepulsionForNearbyObjects(CVector const&, CVector const&, CVector const&, CVector const&,
		bool, CVector&) const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_DeadActions(float)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_OnChangeCabin()
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::_bPassedPathPoint(CVector const&, CVector const&, bool) const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_CauseCustomGunPointedEvents()
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_KeepSuspension()
	{
		throw std::logic_error("Not implemented");
	}

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_CLASS_METHOD(0x005CCF40, Vehicle, _GetNextPathPoint)
	CVector Vehicle::_GetNextPathPoint() const
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	CStr Vehicle::_GetTrailerName() const
	{
		throw std::logic_error("Not implemented");
	}

	CVector Vehicle::_CalcRepulsionForObstacle(Obstacle const*, CVector const&, CVector const&, CVector const&,
		CVector const&, bool, CVector&) const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_DriveBySteeringForce(CVector const&)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_TurnWheelByAngle(Wheel*, float)
	{
		throw std::logic_error("Not implemented");
	}

	CVector Vehicle::_GetCustomWeaponTargetPoint() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_EnsureRecollection()
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_UpdateLockedObj(float)
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::_bPointIsBehind(CVector const&) const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_GetOutOfDifficlultPlaceInternal()
	{
		throw std::logic_error("Not implemented");
	}

	float Vehicle::_GetCabinControlCoeff() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_ValidateVehicleParts()
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_AdjustLookBox(bool, CVector const&, CVector const&, CVector const&) const
	{
		throw std::logic_error("Not implemented");
	}

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_CLASS_METHOD(0x005D62E0, Vehicle, _CalcSteeringForceToPathPoint)
	CVector Vehicle::_CalcSteeringForceToPathPoint(CVector const&, CVector const&) const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_TakeWaterIntoAccount(float)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_KeepGearBox(float)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_SetIdleMoveStatusAndCauseTargetReached()
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_CalcRpms()
	{
		throw std::logic_error("Not implemented");
	}

	int Vehicle::_UpdateRepositoryOnChangeBasket()
	{
		auto basket = GetPartByName(BASKET);
		if (basket && basket->IsKindOf(&ai::Basket::m_classBasket))
		{
			throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	CVector Vehicle::_GetLastPathPoint() const
	{
		throw std::logic_error("Not implemented");
	}

	float Vehicle::_GetAngleTo(CVector const&) const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_AdjustWheel(WheelRuntimeInfo&)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* Vehicle::CreateObject()
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_CheckForNearbyChests() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_AdjustTrailer()
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_UpdateSeenObjAndWeapons(float)
	{
		throw std::logic_error("Not implemented");
	}

	CVector Vehicle::_CalcSteeringForce(float) const
	{
		throw std::logic_error("Not implemented");
	}

	CVector Vehicle::_GetEtalonWheelAVel() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_DropChests()
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_EvaluateToDead()
	{
		throw std::logic_error("Not implemented");
	}

    Vehicle::VehicleMoveStatus Vehicle::GetMoveStatus() const
    {
        throw std::logic_error("Not implemented");
    }
}
