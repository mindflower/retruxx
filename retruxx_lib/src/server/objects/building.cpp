#include "building.h"

#include <stdexcept>

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Building)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Building);

	bool BuildingPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	BuildingPrototypeInfo::BuildingPrototypeInfo()
	{
		throw retruxx::logic_error("Not implemented");
	}

	Obj* BuildingPrototypeInfo::CreateTargetObject() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	Building::Building(BuildingPrototypeInfo const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Class* Building::GetClass() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Building::RemoveChild(Obj*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	BuildingType Building::GetBuildingType() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Building::Remove()
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Class* Building::GetBaseClass()
	{
		return RT_CLASS_LOCAL(Obj);
	}

	BuildingType Building::GetBuildingTypeByName(CStr const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool Building::CanChildBeAdded(m3d::Class*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	retruxx::vector<Npc*> const& Building::GetNpcs() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void Building::AddChild(Obj*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	CStr Building::GetBuildingTypeName(BuildingType)
	{
		throw retruxx::logic_error("Not implemented");
	}

	BuildingPrototypeInfo const* Building::GetPrototypeInfo() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	Building::~Building()
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Object* Building::CreateObject()
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Object* Building::Clone()
	{
		throw retruxx::logic_error("Not implemented");
	}
}
