#include "boss04station.h"

#include <stdexcept>

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Boss04Station)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Boss04Station);

	bool Boss04StationPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	Boss04StationPrototypeInfo::Boss04StationPrototypeInfo()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04StationPrototypeInfo::PostLoad()
	{
		throw std::logic_error("Not implemented");
	}

	Obj* Boss04StationPrototypeInfo::CreateTargetObject() const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04Station::EvaluateToDead()
	{
		throw std::logic_error("Not implemented");
	}

	Boss04Station::Boss04Station(Boss04StationPrototypeInfo const& prototype) : ComplexPhysicObj(prototype)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04Station::Update(float, unsigned)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04Station::InflictDamage(DamageInfo const&)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04Station::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04Station::Registration()
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss04Station::bDestroyed() const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* Boss04Station::GetBaseClass()
	{
		return RT_CLASS_LOCAL(ComplexPhysicObj);
	}

	bool Boss04Station::SetPropertyById(int, m3d::AIParam const&)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04Station::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04Station::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* Boss04Station::GetClass() const
	{
		throw std::logic_error("Not implemented");
	}

	int Boss04Station::GetPropertyId(char const*) const
	{
		throw std::logic_error("Not implemented");
	}

	eGObjPropertySaveStatus Boss04Station::GetPropertySaveStatus(int) const
	{
		throw std::logic_error("Not implemented");
	}

	CStr Boss04Station::GetPropertyName(int) const
	{
		throw std::logic_error("Not implemented");
	}

	Boss04StationPrototypeInfo const* Boss04Station::GetPrototypeInfo() const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04Station::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04Station::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04Station::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss04Station::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss04Station::_GetPropertyInternal(int, m3d::AIParam&) const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04Station::_InternalPostLoad()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04Station::_InternalCreateVisualPart()
	{
		throw std::logic_error("Not implemented");
	}

	Boss04Station::~Boss04Station()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* Boss04Station::CreateObject()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* Boss04Station::Clone()
	{
		throw std::logic_error("Not implemented");
	}
}
