#include "boss04stationpart.h"

#include <stdexcept>

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Boss04StationPart)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Boss04StationPart);

	Boss04StationPartPrototypeInfo::Boss04StationPartPrototypeInfo()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04StationPartPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	Obj* Boss04StationPartPrototypeInfo::CreateTargetObject() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Boss04StationPartPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	Boss04StationPart::MeshGroupInfo::MeshGroupInfo(int, float)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04StationPart::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Class* Boss04StationPart::GetClass() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	int Boss04StationPart::GetPropertyId(char const*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	CStr Boss04StationPart::GetPropertyName(int) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	eGObjPropertySaveStatus Boss04StationPart::GetPropertySaveStatus(int) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	Boss04StationPartPrototypeInfo const* Boss04StationPart::GetPrototypeInfo() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	Boss04StationPart::Boss04StationPart(Boss04StationPartPrototypeInfo const& prototype) : VehiclePart(prototype)
	{
		throw retruxx::logic_error("Not implemented");
	}

	float Boss04StationPart::GetHealth() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04StationPart::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04StationPart::Registration()
	{
	}

	void Boss04StationPart::Update(float, unsigned)
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Class* Boss04StationPart::GetBaseClass()
	{
		return RT_CLASS_LOCAL(VehiclePart);
	}

	bool Boss04StationPart::SetPropertyById(int, m3d::AIParam const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04StationPart::_InternalPostLoad()
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Boss04StationPart::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	Boss04StationPart::~Boss04StationPart()
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Boss04StationPart::_GetPropertyInternal(int, m3d::AIParam&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Object* Boss04StationPart::CreateObject()
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Object* Boss04StationPart::Clone()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04StationPart::_UpdateMeshGroupsHealth()
	{
		throw retruxx::logic_error("Not implemented");
	}
}
