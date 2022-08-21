#include "building.h"

#include <stdexcept>

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Building)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Building);

	bool BuildingPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	BuildingPrototypeInfo::BuildingPrototypeInfo()
	{
		throw std::logic_error("Not implemented");
	}

	Obj* BuildingPrototypeInfo::CreateTargetObject() const
	{
		throw std::logic_error("Not implemented");
	}

	Building::Building(BuildingPrototypeInfo const&)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* Building::GetClass() const
	{
		throw std::logic_error("Not implemented");
	}

	bool Building::RemoveChild(Obj*)
	{
		throw std::logic_error("Not implemented");
	}

	BuildingType Building::GetBuildingType() const
	{
		throw std::logic_error("Not implemented");
	}

	void Building::Remove()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* Building::GetBaseClass()
	{
		return RT_CLASS_LOCAL(Obj);
	}

	BuildingType Building::GetBuildingTypeByName(CStr const&)
	{
		throw std::logic_error("Not implemented");
	}

	bool Building::CanChildBeAdded(m3d::Class*) const
	{
		throw std::logic_error("Not implemented");
	}

	std::vector<Npc*> const& Building::GetNpcs() const
	{
		throw std::logic_error("Not implemented");
	}

	void Building::AddChild(Obj*)
	{
		throw std::logic_error("Not implemented");
	}

	CStr Building::GetBuildingTypeName(BuildingType)
	{
		throw std::logic_error("Not implemented");
	}

	BuildingPrototypeInfo const* Building::GetPrototypeInfo() const
	{
		throw std::logic_error("Not implemented");
	}

	Building::~Building()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* Building::CreateObject()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* Building::Clone()
	{
		throw std::logic_error("Not implemented");
	}
}
