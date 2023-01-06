#include "boss04drone.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(Boss04Drone, SetCustomControl)
{
	throw std::logic_error("Not implemented");
}

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Boss04Drone)
		RT_CLASS_EXPORT(Boss04Drone, m3d::METHOD, SetCustomControl, "", "", "")
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Boss04Drone);

	Obj* Boss04DronePrototypeInfo::CreateTargetObject() const
	{
		throw std::logic_error("Not implemented");
	}

	Boss04DronePrototypeInfo::Boss04DronePrototypeInfo()
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss04DronePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04Drone::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04Drone::SetCustomControl(bool)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04Drone::Update(float, unsigned)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04Drone::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04Drone::InflictDamage(DamageInfo const&)
	{
		throw std::logic_error("Not implemented");
	}

	Boss04Drone::Boss04Drone(Boss04DronePrototypeInfo const& prototype) : ComplexPhysicObj(prototype)
	{
		throw std::logic_error("Not implemented");
	}

	Boss04DronePrototypeInfo const* Boss04Drone::GetPrototypeInfo() const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* Boss04Drone::GetClass() const
	{
		throw std::logic_error("Not implemented");
	}

	CStr Boss04Drone::GetPropertyName(int) const
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss04Drone::CanChildBeAdded(m3d::Class*) const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04Drone::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04Drone::SetFlyPathName(CStr const&)
	{
		throw std::logic_error("Not implemented");
	}

	int Boss04Drone::GetPropertyId(char const*) const
	{
		throw std::logic_error("Not implemented");
	}

	float Boss04Drone::GetMaxHealth() const
	{
		throw std::logic_error("Not implemented");
	}

	eGObjPropertySaveStatus Boss04Drone::GetPropertySaveStatus(int) const
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	bool Boss04Drone::SetPropertyById(int, m3d::AIParam const&)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04Drone::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw std::logic_error("Not implemented");
	}

	Boss04Drone::~Boss04Drone()
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss04Drone::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss04Drone::_GetPropertyInternal(int, m3d::AIParam&) const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* Boss04Drone::CreateObject()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04Drone::_RecalcFlyPath()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04Drone::_UpdateLookAtPlayer()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* Boss04Drone::Clone()
	{
		throw std::logic_error("Not implemented");
	}
}
