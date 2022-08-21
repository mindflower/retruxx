#include "barricade.h"

#include <stdexcept>

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Barricade)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Barricade);

	BarricadePrototypeInfo::BarricadePrototypeInfo()
	{
		throw std::logic_error("Not implemented");
	}

	Obj* BarricadePrototypeInfo::CreateTargetObject() const
	{
		throw std::logic_error("Not implemented");
	}

	bool BarricadePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	void BarricadePrototypeInfo::_InternalCopyFrom(PrototypeInfo const&)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* Barricade::GetClass() const
	{
		throw std::logic_error("Not implemented");
	}

	int Barricade::GetPropertyId(char const*) const
	{
		throw std::logic_error("Not implemented");
	}

	eGObjPropertySaveStatus Barricade::GetPropertySaveStatus(int) const
	{
		throw std::logic_error("Not implemented");
	}

	CStr Barricade::GetPropertyName(int) const
	{
		throw std::logic_error("Not implemented");
	}

	BarricadePrototypeInfo const* Barricade::GetPrototypeInfo() const
	{
		throw std::logic_error("Not implemented");
	}

	void Barricade::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	void Barricade::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	void Barricade::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	void Barricade::Registration()
	{
		throw std::logic_error("Not implemented");
	}

	Barricade::Barricade(BarricadePrototypeInfo const& prototype) : ObjPrefab(prototype)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* Barricade::GetBaseClass()
	{
		return RT_CLASS_LOCAL(ObjPrefab);
	}

	bool Barricade::SetPropertyById(int, m3d::AIParam const&)
	{
		throw std::logic_error("Not implemented");
	}

	void Barricade::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
	{
		throw std::logic_error("Not implemented");
	}

	bool Barricade::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
	{
		throw std::logic_error("Not implemented");
	}

	Barricade::~Barricade()
	{
		throw std::logic_error("Not implemented");
	}

	bool Barricade::_GetPropertyInternal(int, m3d::AIParam&) const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* Barricade::CreateObject()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* Barricade::Clone()
	{
		throw std::logic_error("Not implemented");
	}
}
