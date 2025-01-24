#include "boss04drone.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(Boss04Drone, SetCustomControl)
{
	throw retruxx::logic_error("Not implemented");
}

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Boss04Drone)
		RT_CLASS_EXPORT(Boss04Drone, m3d::METHOD, SetCustomControl, "", "", "")
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Boss04Drone);

	Obj* Boss04DronePrototypeInfo::CreateTargetObject() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	Boss04DronePrototypeInfo::Boss04DronePrototypeInfo()
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Boss04DronePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04Drone::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04Drone::SetCustomControl(bool)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04Drone::Update(float, unsigned)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04Drone::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04Drone::InflictDamage(DamageInfo const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	Boss04Drone::Boss04Drone(Boss04DronePrototypeInfo const& prototype) : ComplexPhysicObj(prototype)
	{
		throw retruxx::logic_error("Not implemented");
	}

	Boss04DronePrototypeInfo const* Boss04Drone::GetPrototypeInfo() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Class* Boss04Drone::GetClass() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	CStr Boss04Drone::GetPropertyName(int) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Boss04Drone::CanChildBeAdded(m3d::Class*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04Drone::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04Drone::SetFlyPathName(CStr const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	int Boss04Drone::GetPropertyId(char const*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	float Boss04Drone::GetMaxHealth() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	eGObjPropertySaveStatus Boss04Drone::GetPropertySaveStatus(int) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04Drone::Registration()
	{
	}

	m3d::Class* Boss04Drone::GetBaseClass()
	{
		return RT_CLASS_LOCAL(ComplexPhysicObj);
	}

	float Boss04Drone::GetHealth() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Boss04Drone::SetPropertyById(int, m3d::AIParam const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04Drone::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	Boss04Drone::~Boss04Drone()
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Boss04Drone::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Boss04Drone::_GetPropertyInternal(int, m3d::AIParam&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Object* Boss04Drone::CreateObject()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04Drone::_RecalcFlyPath()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04Drone::_UpdateLookAtPlayer()
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Object* Boss04Drone::Clone()
	{
		throw retruxx::logic_error("Not implemented");
	}
}
