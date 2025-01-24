#include "boss04station.h"

#include <stdexcept>

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Boss04Station)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Boss04Station);

	bool Boss04StationPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	Boss04StationPrototypeInfo::Boss04StationPrototypeInfo()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04StationPrototypeInfo::PostLoad()
	{
		throw retruxx::logic_error("Not implemented");
	}

	Obj* Boss04StationPrototypeInfo::CreateTargetObject() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04Station::EvaluateToDead()
	{
		throw retruxx::logic_error("Not implemented");
	}

	Boss04Station::Boss04Station(Boss04StationPrototypeInfo const& prototype) : ComplexPhysicObj(prototype)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04Station::Update(float, unsigned)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04Station::InflictDamage(DamageInfo const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04Station::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04Station::Registration()
	{
	}

	bool Boss04Station::bDestroyed() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Class* Boss04Station::GetBaseClass()
	{
		return RT_CLASS_LOCAL(ComplexPhysicObj);
	}

	bool Boss04Station::SetPropertyById(int, m3d::AIParam const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04Station::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04Station::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Class* Boss04Station::GetClass() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	int Boss04Station::GetPropertyId(char const*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	eGObjPropertySaveStatus Boss04Station::GetPropertySaveStatus(int) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	CStr Boss04Station::GetPropertyName(int) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	Boss04StationPrototypeInfo const* Boss04Station::GetPrototypeInfo() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04Station::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04Station::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04Station::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Boss04Station::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Boss04Station::_GetPropertyInternal(int, m3d::AIParam&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04Station::_InternalPostLoad()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Boss04Station::_InternalCreateVisualPart()
	{
		throw retruxx::logic_error("Not implemented");
	}

	Boss04Station::~Boss04Station()
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Object* Boss04Station::CreateObject()
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Object* Boss04Station::Clone()
	{
		throw retruxx::logic_error("Not implemented");
	}
}
