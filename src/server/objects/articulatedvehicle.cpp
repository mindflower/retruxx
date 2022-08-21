#include "articulatedvehicle.h"

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(ArticulatedVehicle)
		RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(ArticulatedVehicle);

	void ArticulatedVehiclePrototypeInfo::PostLoad()
	{
		throw std::logic_error("Not implemented");
	}

	bool ArticulatedVehiclePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	ArticulatedVehiclePrototypeInfo::ArticulatedVehiclePrototypeInfo()
	{
		throw std::logic_error("Not implemented");
	}

	Obj* ArticulatedVehiclePrototypeInfo::CreateTargetObject() const
	{
		throw std::logic_error("Not implemented");
	}

	void ArticulatedVehiclePrototypeInfo::_InternalCopyFrom(PrototypeInfo const&)
	{
		throw std::logic_error("Not implemented");
	}

	CStr ArticulatedVehicle::GetPropertyName(int) const
	{
		throw std::logic_error("Not implemented");
	}

	eGObjPropertySaveStatus ArticulatedVehicle::GetPropertySaveStatus(int) const
	{
		throw std::logic_error("Not implemented");
	}

	ArticulatedVehicle::ArticulatedVehicle(ArticulatedVehiclePrototypeInfo const& prototypeInfo) : Vehicle(prototypeInfo)
	{
		throw std::logic_error("Not implemented");
	}

	void ArticulatedVehicle::CreateChildren()
	{
		throw std::logic_error("Not implemented");
	}

	void ArticulatedVehicle::Remove()
	{
		throw std::logic_error("Not implemented");
	}

	void ArticulatedVehicle::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* ArticulatedVehicle::GetClass() const
	{
		throw std::logic_error("Not implemented");
	}

	void ArticulatedVehicle::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	void ArticulatedVehicle::SetPassedToAnotherMapStatus()
	{
		throw std::logic_error("Not implemented");
	}

	ArticulatedVehiclePrototypeInfo const* ArticulatedVehicle::GetPrototypeInfo() const
	{
		throw std::logic_error("Not implemented");
	}

	bool ArticulatedVehicle::RemoveChild(Obj*)
	{
		throw std::logic_error("Not implemented");
	}

	void ArticulatedVehicle::AddChild(Obj*)
	{
		throw std::logic_error("Not implemented");
	}

	bool ArticulatedVehicle::SetPropertyById(int, m3d::AIParam const&)
	{
		throw std::logic_error("Not implemented");
	}

	void ArticulatedVehicle::SetPositionSelf(CVector const&)
	{
		throw std::logic_error("Not implemented");
	}

	void ArticulatedVehicle::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	int ArticulatedVehicle::GetPropertyId(char const*) const
	{
		throw std::logic_error("Not implemented");
	}

	void ArticulatedVehicle::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	void ArticulatedVehicle::SetRotationSelf(Quaternion const&)
	{
		throw std::logic_error("Not implemented");
	}

	void ArticulatedVehicle::Update(float, unsigned)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* ArticulatedVehicle::GetBaseClass()
	{
		return RT_CLASS_LOCAL(Vehicle);
	}

	void ArticulatedVehicle::_InternalPostLoad()
	{
		throw std::logic_error("Not implemented");
	}

	bool ArticulatedVehicle::_GetPropertyInternal(int, m3d::AIParam&) const
	{
		throw std::logic_error("Not implemented");
	}

	ArticulatedVehicle::~ArticulatedVehicle()
	{
		throw std::logic_error("Not implemented");
	}

	void ArticulatedVehicle::_KeepSteer(float)
	{
		throw std::logic_error("Not implemented");
	}

	bool ArticulatedVehicle::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
	{
		throw std::logic_error("Not implemented");
	}

	CStr ArticulatedVehicle::_GetTrailerName() const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* ArticulatedVehicle::Clone()
	{
		throw std::logic_error("Not implemented");
	}

	void ArticulatedVehicle::_AdjustTrailerPosition()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* ArticulatedVehicle::CreateObject()
	{
		throw std::logic_error("Not implemented");
	}

	Vehicle* ArticulatedVehicle::_GetTrailer() const
	{
		throw std::logic_error("Not implemented");
	}
}
