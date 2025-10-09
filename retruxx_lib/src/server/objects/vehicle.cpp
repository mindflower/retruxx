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

#include "base/prototypemanager.h"
#include "core/ini.h"
#include "core/log.h"
#include "server/ai/aimanager.h"
#include "include/m3dapp.h"
#include "include/core/kernel.h"
#include "include/config.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetRandomSkin)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, SetGamePositionOnGround)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Vehicle, GetSize)
{
	throw std::logic_error("Not implemented");
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
	throw std::logic_error("Not implemented");
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
	throw std::logic_error("Not implemented");
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
	throw std::logic_error("Not implemented");
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

                    CStr steeringStr = 0;
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
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
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

	int Vehicle::SetExternalPath(retruxx::vector<CVector2, retruxx::allocator<CVector2>> const&)
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	void Vehicle::FireFromWeaponCustom2(bool, int)
	{
		throw std::logic_error("Not implemented");
	}

	float Vehicle::GetCameraMaxDist() const
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetCustomControlEnabled(bool)
	{
		throw std::logic_error("Not implemented");
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

	void Vehicle::SetSkin(int)
	{
		throw std::logic_error("Not implemented");
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

	void Vehicle::SetMoveStatus(VehicleMoveStatus)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetPassedToAnotherMapStatus()
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetCruisingSpeed(float)
	{
		throw std::logic_error("Not implemented");
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

	void Vehicle::SetPartByName(CStr const&, VehiclePart*, bool)
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	int Vehicle::SetExternalPathByName(char const*)
	{
		throw std::logic_error("Not implemented");
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

	void Vehicle::SetUpdatingByODE(bool)
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	void Vehicle::DisableGeometry(bool)
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetSteer(float)
	{
		throw std::logic_error("Not implemented");
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

	void Vehicle::setImmortalMode(bool)
	{
		throw std::logic_error("Not implemented");
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

	void Vehicle::SetPositionSelf(CVector const&)
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
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

	void Vehicle::SetAttackStatus(VehicleAttackStatus)
	{
		throw std::logic_error("Not implemented");
	}

	Vehicle::Vehicle(VehiclePrototypeInfo const& prototypeInfo) :
		ComplexPhysicObj(prototypeInfo)
	{
		throw std::logic_error("Not implemented");
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

	void Vehicle::SetIndexInTeam(int)
	{
		throw std::logic_error("Not implemented");
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
		//throw std::logic_error("Not implemented");
	}

	float Vehicle::GetBrake() const
	{
		throw std::logic_error("Not implemented");
	}

	float Vehicle::GetMaxSpeed() const
	{
		throw std::logic_error("Not implemented");
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

	int Vehicle::CheckSkin(int)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::AIParam Vehicle::VehicleAIOnDefend(Obj*)
	{
		throw std::logic_error("Not implemented");
	}

	float Vehicle::GetEngineRpm() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetBelong(int)
	{
		throw std::logic_error("Not implemented");
	}

	Team* Vehicle::GetTeam() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::UnsubscribeRadioManagerFromAllNearbyObjIds() const
	{
		throw std::logic_error("Not implemented");
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

	void Vehicle::SetGamePositionOnGround(CVector const&, bool, bool)
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION(0x005CC1C0, Vehicle::GetSize)
	CVector Vehicle::GetSize() const
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	float Vehicle::GetCollisionRadius() const
	{
		throw std::logic_error("Not implemented");
	}

	float Vehicle::GetDriftCoeff() const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::SetRotationSelf(Quaternion const&)
	{
		throw std::logic_error("Not implemented");
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

	void Vehicle::EnableGeometry(bool)
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::_GetPropertyInternal(int, m3d::AIParam&) const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_PutContour()
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_InternalCreateVisualPart()
	{
		throw std::logic_error("Not implemented");
	}

	bool Vehicle::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_InternalPostLoad()
	{
		throw std::logic_error("Not implemented");
	}

	float Vehicle::_CalcMassForBody() const
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_UpdatePhysicsUpdater()
	{
		throw std::logic_error("Not implemented");
	}

	void Vehicle::_AdjustSizeAndBumperPoint()
	{
		throw std::logic_error("Not implemented");
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
