#include "articulatedvehicle.h"

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(ArticulatedVehicle)
		RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(ArticulatedVehicle);

	void ArticulatedVehiclePrototypeInfo::PostLoad()
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool ArticulatedVehiclePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	ArticulatedVehiclePrototypeInfo::ArticulatedVehiclePrototypeInfo()
	{
		throw retruxx::logic_error("Not implemented");
	}

	Obj* ArticulatedVehiclePrototypeInfo::CreateTargetObject() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void ArticulatedVehiclePrototypeInfo::_InternalCopyFrom(PrototypeInfo const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	CStr ArticulatedVehicle::GetPropertyName(int) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	eGObjPropertySaveStatus ArticulatedVehicle::GetPropertySaveStatus(int) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	ArticulatedVehicle::ArticulatedVehicle(ArticulatedVehiclePrototypeInfo const& prototypeInfo) : Vehicle(prototypeInfo)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void ArticulatedVehicle::CreateChildren()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void ArticulatedVehicle::Remove()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void ArticulatedVehicle::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Class* ArticulatedVehicle::GetClass() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void ArticulatedVehicle::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void ArticulatedVehicle::SetPassedToAnotherMapStatus()
	{
		throw retruxx::logic_error("Not implemented");
	}

	ArticulatedVehiclePrototypeInfo const* ArticulatedVehicle::GetPrototypeInfo() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool ArticulatedVehicle::RemoveChild(Obj*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void ArticulatedVehicle::AddChild(Obj*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool ArticulatedVehicle::SetPropertyById(int, m3d::AIParam const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void ArticulatedVehicle::SetPositionSelf(CVector const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void ArticulatedVehicle::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	int ArticulatedVehicle::GetPropertyId(char const*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void ArticulatedVehicle::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void ArticulatedVehicle::SetRotationSelf(Quaternion const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void ArticulatedVehicle::Update(float, unsigned)
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Class* ArticulatedVehicle::GetBaseClass()
	{
		return RT_CLASS_LOCAL(Vehicle);
	}

	void ArticulatedVehicle::_InternalPostLoad()
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool ArticulatedVehicle::_GetPropertyInternal(int, m3d::AIParam&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	ArticulatedVehicle::~ArticulatedVehicle()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void ArticulatedVehicle::_KeepSteer(float)
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool ArticulatedVehicle::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	CStr ArticulatedVehicle::_GetTrailerName() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Object* ArticulatedVehicle::Clone()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void ArticulatedVehicle::_AdjustTrailerPosition()
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Object* ArticulatedVehicle::CreateObject()
	{
		throw retruxx::logic_error("Not implemented");
	}

	Vehicle* ArticulatedVehicle::_GetTrailer() const
	{
		throw retruxx::logic_error("Not implemented");
	}
}
