#include "barricade.h"

#include <stdexcept>

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Barricade)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Barricade);

	BarricadePrototypeInfo::BarricadePrototypeInfo()
	{
		throw retruxx::logic_error("Not implemented");
	}

	Obj* BarricadePrototypeInfo::CreateTargetObject() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool BarricadePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void BarricadePrototypeInfo::_InternalCopyFrom(PrototypeInfo const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Class* Barricade::GetClass() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	int Barricade::GetPropertyId(char const*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	eGObjPropertySaveStatus Barricade::GetPropertySaveStatus(int) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	CStr Barricade::GetPropertyName(int) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	BarricadePrototypeInfo const* Barricade::GetPrototypeInfo() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Barricade::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Barricade::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Barricade::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Barricade::Registration()
	{
		m_propertiesMap["Probability"] = 67;
	}

	Barricade::Barricade(BarricadePrototypeInfo const& prototype) : 
        ObjPrefab(prototype),
        m_probability(prototype.m_probability, 0.0, 1.0)
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Class* Barricade::GetBaseClass()
	{
		return RT_CLASS_LOCAL(ObjPrefab);
	}

	bool Barricade::SetPropertyById(int, m3d::AIParam const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Barricade::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Barricade::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	Barricade::~Barricade()
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Barricade::_GetPropertyInternal(int, m3d::AIParam&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Object* Barricade::CreateObject()
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Object* Barricade::Clone()
	{
		throw retruxx::logic_error("Not implemented");
	}
}
