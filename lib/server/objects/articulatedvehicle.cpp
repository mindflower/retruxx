#include "articulatedvehicle.h"

#include <cmath>

#include <ode/objects.h>
#include <ode/odecpp.h>

#include "m3dapp.h"
#include "core/kernel.h"
#include "core/log.h"
#include "math/coremath.h"
#include "scene/servers/serveranimatedmodel.h"
#include "server/dynamicscene.h"
#include "server/objects/wheel.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/base/prototypemanager.h"
#include "server/objects/chassis.h"

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(ArticulatedVehicle)
		RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(ArticulatedVehicle);

	retruxx::map<CStr, int, Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int>>> ArticulatedVehicle::m_propertiesMap;
	retruxx::map<int, eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, eGObjPropertySaveStatus>>>
		ArticulatedVehicle::m_propertiesSaveStatesMap;

	namespace
	{
		CVector const INITIAL_OBJECTS_DIRECTION(0.0f, 0.0f, 1.0f);

		// The most the trailer is swung towards its hinge direction per update, in radians.
		float const MAX_ANGLE_TO_CORRECT = 0.05f;

		// v rotated by q (row vector times q's rotation matrix), as expanded inline in the
		// shipped code.
		CVector RotateByQuaternion(CVector const& v, Quaternion const& q)
		{
			float const xz = q.x * q.z;
			float const xw = q.x * q.w;
			float const xx = q.x * q.x;
			float const xy = q.x * q.y;
			float const zy = q.z * q.y;
			float const yy = q.y * q.y;
			float const zz = q.z * q.z;
			float const yw = q.y * q.w;
			float const zw = q.z * q.w;
			float const m11 = 1.0f - (zz + yy) * 2.0f;
			float const m21 = (xy - zw) * 2.0f;
			float const m31 = (yw + xz) * 2.0f;
			float const m12 = (zw + xy) * 2.0f;
			float const m22 = 1.0f - (zz + xx) * 2.0f;
			float const m32 = (zy - xw) * 2.0f;
			float const m13 = (xz - yw) * 2.0f;
			float const m23 = (xw + zy) * 2.0f;
			float const m33 = 1.0f - (yy + xx) * 2.0f;
			return CVector(
				(v.x * m11 + m21 * v.y) + m31 * v.z,
				(v.x * m12 + m22 * v.y) + m32 * v.z,
				(v.x * m13 + m23 * v.y) + m33 * v.z);
		}

		// The Hamilton product a * b, as expanded inline in the shipped code.
		Quaternion QuatMul(Quaternion const& a, Quaternion const& b)
		{
			return Quaternion(
				a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
				a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
				a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
				a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z);
		}

		// RVA 0x84E3E0 - the hitch load point LP_TRAIL01 of a chassis model, relative to the
		// vehicle's mass centre.
		CVector GetRelJointPos(CStr const& chassisModelName, CVector massCenter)
		{
			auto* const server = static_cast<m3d::AnimatedModelsServer*>(&M3D_APP->GetAnimatedModelsServer());
			CStr const boneName = CStr("LP_TRAIL") + CStr("0") + CStr(1);
			CMatrix mat;
			if (!server->GetBoneMatrixByNameFromModelName(chassisModelName.c_str(), boneName, mat, false))
			{
				M3D_LOG_ERR(
					CStr("Error: LoadPoint not found: ") + (CStr("LP_TRAIL") + CStr("0") + CStr(1)) + CStr(" for model '") +
					chassisModelName + CStr("'"));
				return CVector(0.0f, 0.0f, 0.0f);
			}
			return CVector(mat._41 - massCenter.x, mat._42 - massCenter.y, mat._43 - massCenter.z);
		}
	}  // namespace

	void ArticulatedVehiclePrototypeInfo::PostLoad()
	{
		// RVA 0x84DD90
		VehiclePrototypeInfo::PostLoad();
		m_trailerPrototypeId = thePrototypeManager->GetPrototypeId(m_trailerPrototypeName);
	}

	bool ArticulatedVehiclePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
	{
		// RVA 0x84DED0
		bool const result = VehiclePrototypeInfo::LoadFromXML(xmlFile, xmlNode);
		if (result)
		{
			m3d::SafeStrAttrib(m_trailerPrototypeName, xmlNode, "TrailerPrototype");
		}
		return result;
	}

	ArticulatedVehiclePrototypeInfo::ArticulatedVehiclePrototypeInfo()
	{
		// RVA 0x84DEA0 - NOTE: m_trailerPrototypeId is left uninitialised until PostLoad.
	}

	Obj* ArticulatedVehiclePrototypeInfo::CreateTargetObject() const
	{
		// RVA 0x84EA30
		return new ArticulatedVehicle(*this);
	}

	void ArticulatedVehiclePrototypeInfo::_InternalCopyFrom(PrototypeInfo const& rhs)
	{
		// RVA 0x8509E0 - a plain assignment from another prototype of the same class.
		*this = static_cast<ArticulatedVehiclePrototypeInfo const&>(rhs);
	}

	CStr ArticulatedVehicle::GetPropertyName(int id) const
	{
		// RVA 0x850930
		for (auto const& property : m_propertiesMap)
		{
			if (property.second == id)
			{
				return property.first;
			}
		}
		return Vehicle::GetPropertyName(id);
	}

	eGObjPropertySaveStatus ArticulatedVehicle::GetPropertySaveStatus(int id) const
	{
		// RVA 0x84FC70
		auto it = m_propertiesSaveStatesMap.find(id);
		if (it != m_propertiesSaveStatesMap.end())
		{
			return it->second;
		}
		return Vehicle::GetPropertySaveStatus(id);
	}

	ArticulatedVehicle::ArticulatedVehicle(ArticulatedVehiclePrototypeInfo const& prototypeInfo) : Vehicle(prototypeInfo)
	{
		// RVA 0x84DF10
		m_trailerObjId = -1;
		m_trailerJoint = nullptr;
		m_relJointPosOnMe = ZeroVector;
		m_relJointPosOnTrailer = ZeroVector;
	}

	void ArticulatedVehicle::CreateChildren()
	{
		// RVA 0x84DDE0
		Vehicle::CreateChildren();
	}

	void ArticulatedVehicle::Remove()
	{
		// RVA 0x84DDD0
		Vehicle::Remove();
	}

	void ArticulatedVehicle::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
	{
		// RVA 0x84DE00
		Vehicle::SaveToXML(xmlFile, xmlNode);
	}

	m3d::Class* ArticulatedVehicle::GetClass() const
	{
		// RVA 0x84DD80
		return RT_CLASS_LOCAL(ArticulatedVehicle);
	}

	void ArticulatedVehicle::RegisterProperty(char const* name, int id, eGObjPropertySaveStatus saveStatus)
	{
		// Declared in the PDB but never emitted in the shipped build.
		m_propertiesMap[CStr(name)] = id;
		m_propertiesSaveStatesMap[id] = saveStatus;
	}

	void ArticulatedVehicle::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
	{
		// Declared in the PDB but never emitted in the shipped build; the usual property-map pattern.
		for (auto const& property : m_propertiesMap)
		{
			Props.insert(property.second);
		}
		Vehicle::GetPropertiesIDs(Props);
	}

	void ArticulatedVehicle::SetPassedToAnotherMapStatus()
	{
		// RVA 0x84DE30
		Vehicle::SetPassedToAnotherMapStatus();
	}

	ArticulatedVehiclePrototypeInfo const* ArticulatedVehicle::GetPrototypeInfo() const
	{
		// RVA 0x84ED00
		return static_cast<ArticulatedVehiclePrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
	}

	bool ArticulatedVehicle::RemoveChild(Obj* pObj)
	{
		// RVA 0x84DE20
		return Vehicle::RemoveChild(pObj);
	}

	void ArticulatedVehicle::AddChild(Obj* pObj)
	{
		// RVA 0x84DE10
		Vehicle::AddChild(pObj);
	}

	bool ArticulatedVehicle::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
	{
		// RVA 0x84DE50
		return Vehicle::SetPropertyById(propertyId, newValue);
	}

	void ArticulatedVehicle::SetPositionSelf(CVector const& pos)
	{
		// RVA 0x84FD40 - the trailer is moved along by the same shift.
		CVector const oldpos = GetPosition();
		CVector const shift(pos.x - oldpos.x, pos.y - oldpos.y, pos.z - oldpos.z);
		Vehicle::SetPositionSelf(pos);
		if (PhysicObj* const trailer = _GetTrailer())
		{
			CVector const oldTrailerPos = trailer->GetPosition();
			trailer->SetPosition(CVector(oldTrailerPos.x + shift.x, oldTrailerPos.y + shift.y, oldTrailerPos.z + shift.z));
		}
	}

	void ArticulatedVehicle::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
	{
		// RVA 0x84DDF0
		Vehicle::LoadFromXML(xmlFile, xmlNode);
	}

	int ArticulatedVehicle::GetPropertyId(char const* PropertyName) const
	{
		// RVA 0x84FCB0
		auto it = m_propertiesMap.find(PropertyName);
		if (it != m_propertiesMap.end())
		{
			return it->second;
		}
		return Vehicle::GetPropertyId(PropertyName);
	}

	void ArticulatedVehicle::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
	{
		// Declared in the PDB but never emitted in the shipped build; the usual property-map pattern.
		for (auto const& property : m_propertiesMap)
		{
			Props.insert(property.first);
		}
		Vehicle::GetPropertiesNames(Props);
	}

	void ArticulatedVehicle::SetRotationSelf(Quaternion const& rot)
	{
		// RVA 0x84FE20 - the trailer is turned around the tractor by the same rotation.
		Quaternion const rotForTrailer = QuatMul(rot, GetRotation().getInversed());
		Vehicle::SetRotationSelf(rot);
		if (PhysicObj* const trailer = _GetTrailer())
		{
			CVector const pos = GetPosition();
			CVector const trailerPos = trailer->GetPosition();
			CVector const newRelPos =
				RotateByQuaternion(CVector(trailerPos.x - pos.x, trailerPos.y - pos.y, trailerPos.z - pos.z), rotForTrailer);
			CVector const curPos = GetPosition();
			trailer->SetPosition(CVector(curPos.x + newRelPos.x, curPos.y + newRelPos.y, curPos.z + newRelPos.z));
			trailer->SetRotation(QuatMul(rotForTrailer, trailer->GetRotation()));
		}
	}

	void ArticulatedVehicle::Update(float elapsedTime, unsigned workTime)
	{
		// RVA 0x84FD20
		Vehicle::Update(elapsedTime, workTime);
		_AdjustTrailerPosition();
	}

	m3d::Class* ArticulatedVehicle::GetBaseClass()
	{
		// RVA 0x84DD70
		return RT_CLASS_LOCAL(Vehicle);
	}

	void ArticulatedVehicle::_InternalPostLoad()
	{
		// RVA 0x850360
		// The trailer is created with the tractor and hitched with a hinge at the LP_TRAIL01 load
		// points, built with both vehicles at the origin. Neither vehicle steers with its wheels;
		// the tractor steers through the hinge instead (see _KeepSteer).
		Vehicle::_InternalPostLoad();
		m_trailerObjId = theObjects->CreateNewObject(GetPrototypeInfo()->m_trailerPrototypeId, _GetTrailerName().c_str(), -1, -1);
		// NOTE: the trailer is used without a null check.
		Vehicle* const trailer = _GetTrailer();
		trailer->SetTrailer();
		trailer->TransferToSpace(m_spaceId);
		trailer->LinkToParent(GetId(), HIERARCHY_CHILD);
		CStr const chassisModelName(GetChassis()->m_modelname);
		CStr const trailerChassisModelName(trailer->GetChassis()->m_modelname);

		CVector const oldPos = GetPosition();
		Quaternion const oldRot = GetRotation();
		SetPosition(CVector(0.0f, 0.0f, 0.0f));
		SetRotation(Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
		m_relJointPosOnMe = GetRelJointPos(chassisModelName, m_massCenter);
		m_relJointPosOnTrailer = GetRelJointPos(trailerChassisModelName, trailer->m_massCenter);
		trailer->SetPosition(CVector(
			m_relJointPosOnMe.x - m_relJointPosOnTrailer.x,
			m_relJointPosOnMe.y - m_relJointPosOnTrailer.y,
			m_relJointPosOnMe.z - m_relJointPosOnTrailer.z));

		// The hitch: a vertical hinge limited to 45 degrees either way.
		m_trailerJoint = dJointCreateHinge(gGlobalWorld, nullptr);
		dJointAttach(m_trailerJoint, m_body->id(), trailer->m_body->id());
		dJointSetHingeAnchor(m_trailerJoint, m_relJointPosOnMe.x, m_relJointPosOnMe.y, m_relJointPosOnMe.z);
		dJointSetHingeAxis(m_trailerJoint, 0.0f, 1.0f, 0.0f);
		dJointSetHingeParam(m_trailerJoint, dParamLoStop, -0.78539819f);
		dJointSetHingeParam(m_trailerJoint, dParamHiStop, 0.78539819f);
		// A second hinge 5 m above on the same axis keeps the trailer from rolling off the hitch.
		// NOTE: it is never stored or destroyed.
		dJointID const upperJoint = dJointCreateHinge(gGlobalWorld, nullptr);
		dJointAttach(upperJoint, m_body->id(), trailer->m_body->id());
		dJointSetHingeAnchor(upperJoint, m_relJointPosOnMe.x, m_relJointPosOnMe.y + 5.0f, m_relJointPosOnMe.z);
		dJointSetHingeAxis(upperJoint, 0.0f, 1.0f, 0.0f);
		dJointID lastJoint;
		if (GetMass() <= 100.0f)
		{
			dJointSetHingeParam(m_trailerJoint, dParamCFM, 0.1f);
			dJointSetHingeParam(m_trailerJoint, dParamBounce, 0.0099999998f);
			dJointSetHingeParam(m_trailerJoint, dParamStopCFM, 0.1f);
			dJointSetHingeParam(m_trailerJoint, dParamStopERP, 0.2f);
			lastJoint = m_trailerJoint;
		}
		else
		{
			dJointSetHingeParam(m_trailerJoint, dParamCFM, 0.0000099999997f);
			dJointSetHingeParam(m_trailerJoint, dParamBounce, 0.0f);
			dJointSetHingeParam(m_trailerJoint, dParamStopCFM, 0.0000099999997f);
			dJointSetHingeParam(m_trailerJoint, dParamStopERP, 0.89999998f);
			dJointSetHingeParam(m_trailerJoint, dParamFudgeFactor, 0.001f);
			dJointSetHingeParam(upperJoint, dParamCFM, 0.0000099999997f);
			dJointSetHingeParam(upperJoint, dParamBounce, 0.0f);
			dJointSetHingeParam(upperJoint, dParamStopCFM, 0.0000099999997f);
			dJointSetHingeParam(upperJoint, dParamStopERP, 0.89999998f);
			lastJoint = upperJoint;
		}
		dJointSetHingeParam(lastJoint, dParamFudgeFactor, 0.001f);

		SetPosition(oldPos);
		SetRotation(oldRot);
		for (auto& info : m_wheels)
		{
			if (Wheel* const wheel = info.GetWheel())
			{
				wheel->m_steering = Wheel::STEERING_NO;
			}
		}
		for (auto& info : trailer->m_wheels)
		{
			if (Wheel* const wheel = info.GetWheel())
			{
				wheel->m_steering = Wheel::STEERING_NO;
			}
		}
	}

	bool ArticulatedVehicle::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
	{
		// RVA 0x84DE40
		return Vehicle::_GetPropertyInternal(propertyId, retVal);
	}

	ArticulatedVehicle::~ArticulatedVehicle()
	{
		// RVA 0x84DDC0
	}

	void ArticulatedVehicle::_KeepSteer(float elapsedTime)
	{
		// RVA 0x84DFB0 - drives the hitch hinge's motor towards the steering angle.
		Vehicle::_KeepSteer(elapsedTime);
		float delta = -GetSteer() - dJointGetHingeAngle(m_trailerJoint);
		if (delta > 0.1)
		{
			delta = 0.1f;
		}
		else if (delta < -0.1)
		{
			delta = -0.1f;
		}
		dJointSetHingeParam(m_trailerJoint, dParamVel, delta * 5.0f);
		dJointSetHingeParam(m_trailerJoint, dParamFMax, static_cast<float>(fabs(delta) * 100000.0));
	}

	bool ArticulatedVehicle::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
	{
		// RVA 0x84DE60
		return Vehicle::_GetPropertyDefaultInternal(propertyId, retVal);
	}

	CStr ArticulatedVehicle::_GetTrailerName() const
	{
		// RVA 0x84EA60
		if (CStr(GetName()).empty())
		{
			SYS_ERROR("!CStr( GetName() ).empty()");
		}
		return CStr(GetName()) + CStr("_Trailer");
	}

	m3d::Object* ArticulatedVehicle::Clone()
	{
		// RVA 0x84E060
		SYS_ERROR("!\"Object cannot be cloned\"");
		return nullptr;
	}

	void ArticulatedVehicle::_AdjustTrailerPosition()
	{
		// RVA 0x84ED70
		// Keeps the trailer level with the tractor and on the hitch: its heading, flattened in the
		// tractor's frame, is turned towards by at most MAX_ANGLE_TO_CORRECT, and its hitch point
		// is pulled towards the tractor's by at most 1 m per update.
		// NOTE: the trailer is used without a null check.
		PhysicObj* const trailer = _GetTrailer();
		Quaternion const rot = GetRotation();
		CVector const localDir = RotateByQuaternion(trailer->GetDirection(), rot.getInversed());
		float const invLen = static_cast<float>(1.0 / sqrt(localDir.z * localDir.z + localDir.x * localDir.x + 1.1920929e-7));
		CVector const trailerDirRegular(localDir.x * invLen, invLen * 0.0f, localDir.z * invLen);
		if (trailerDirRegular.z * trailerDirRegular.z + trailerDirRegular.x * trailerDirRegular.x + trailerDirRegular.y * trailerDirRegular.y < 0.001)
		{
			return;
		}
		Quaternion const oldRot = trailer->GetRotation();
		trailer->SetDirection(trailerDirRegular);
		Quaternion const neededRot = QuatMul(rot, trailer->GetRotation());
		CVector const oldDir = RotateByQuaternion(INITIAL_OBJECTS_DIRECTION, oldRot);
		CVector const neededDir = RotateByQuaternion(INITIAL_OBJECTS_DIRECTION, neededRot);
		float coeff = neededDir.z * oldDir.z + neededDir.y * oldDir.y + neededDir.x * oldDir.x;
		if (coeff < -0.99999899f)
		{
			coeff = -0.99999899f;
		}
		else if (coeff > 0.99999899f)
		{
			coeff = 0.99999899f;
		}
		float const angle = static_cast<float>(acos(coeff));
		float const step = angle >= 0.050000001 ? MAX_ANGLE_TO_CORRECT : angle;
		float const t = angle >= 0.001 ? step / angle : 0.0f;
		trailer->SetRotation(SLerp(oldRot, neededRot, t));

		CVector const jointOnTrailer = RotateByQuaternion(m_relJointPosOnTrailer, trailer->GetRotation());
		CVector const jointOnMe = RotateByQuaternion(m_relJointPosOnMe, GetRotation());
		CVector const trailerPos = trailer->GetPosition();
		CVector const pos = GetPosition();
		CVector const delta(
			((pos.x + jointOnMe.x) - jointOnTrailer.x) - trailerPos.x,
			((pos.y + jointOnMe.y) - jointOnTrailer.y) - trailerPos.y,
			((pos.z + jointOnMe.z) - jointOnTrailer.z) - trailerPos.z);
		float const dist = static_cast<float>(sqrt(delta.z * delta.z + delta.y * delta.y + delta.x * delta.x));
		float const move = dist >= 1.0f ? 1.0f : dist;
		float const k = dist >= 0.001 ? move / dist : 0.0f;
		trailer->SetPosition(CVector(delta.x * k + trailerPos.x, delta.y * k + trailerPos.y, delta.z * k + trailerPos.z));
	}

	m3d::Object* ArticulatedVehicle::CreateObject()
	{
		// RVA 0x84E220
		SYS_ERROR("!\"Object cannot be created directly\"");
		return nullptr;
	}

	Vehicle* ArticulatedVehicle::_GetTrailer() const
	{
		// RVA 0x84ED30
		if (m_trailerObjId < 0)
		{
			return nullptr;
		}
		return static_cast<Vehicle*>(theObjects->GetEntityByObjId(m_trailerObjId));
	}

	Vehicle* ArticulatedVehicle::GetTrailer()
	{
		// Declared in the PDB but never emitted in the shipped build.
		return _GetTrailer();
	}
}  // namespace ai
