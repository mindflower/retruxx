#include "articulatedvehicle.h"

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(ArticulatedVehicle)
		RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(ArticulatedVehicle);

	void ArticulatedVehiclePrototypeInfo::PostLoad()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool ArticulatedVehiclePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	ArticulatedVehiclePrototypeInfo::ArticulatedVehiclePrototypeInfo()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	Obj* ArticulatedVehiclePrototypeInfo::CreateTargetObject() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void ArticulatedVehiclePrototypeInfo::_InternalCopyFrom(PrototypeInfo const&)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	CStr ArticulatedVehicle::GetPropertyName(int) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	eGObjPropertySaveStatus ArticulatedVehicle::GetPropertySaveStatus(int) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	ArticulatedVehicle::ArticulatedVehicle(ArticulatedVehiclePrototypeInfo const& prototypeInfo) : Vehicle(prototypeInfo)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void ArticulatedVehicle::CreateChildren()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void ArticulatedVehicle::Remove()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void ArticulatedVehicle::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Class* ArticulatedVehicle::GetClass() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void ArticulatedVehicle::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void ArticulatedVehicle::SetPassedToAnotherMapStatus()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	ArticulatedVehiclePrototypeInfo const* ArticulatedVehicle::GetPrototypeInfo() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool ArticulatedVehicle::RemoveChild(Obj*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void ArticulatedVehicle::AddChild(Obj*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool ArticulatedVehicle::SetPropertyById(int, m3d::AIParam const&)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void ArticulatedVehicle::SetPositionSelf(CVector const&)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void ArticulatedVehicle::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int ArticulatedVehicle::GetPropertyId(char const*) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void ArticulatedVehicle::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void ArticulatedVehicle::SetRotationSelf(Quaternion const&)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void ArticulatedVehicle::Update(float, unsigned)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Class* ArticulatedVehicle::GetBaseClass()
	{
		return RT_CLASS_LOCAL(Vehicle);
	}

	void ArticulatedVehicle::_InternalPostLoad()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool ArticulatedVehicle::_GetPropertyInternal(int, m3d::AIParam&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	ArticulatedVehicle::~ArticulatedVehicle()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void ArticulatedVehicle::_KeepSteer(float)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool ArticulatedVehicle::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	CStr ArticulatedVehicle::_GetTrailerName() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Object* ArticulatedVehicle::Clone()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void ArticulatedVehicle::_AdjustTrailerPosition()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Object* ArticulatedVehicle::CreateObject()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	Vehicle* ArticulatedVehicle::_GetTrailer() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}
}
