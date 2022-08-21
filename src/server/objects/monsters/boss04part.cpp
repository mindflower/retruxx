#include "boss04part.h"

#include <stdexcept>

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Boss04Part)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Boss04Part);

	Obj* Boss04PartPrototypeInfo::CreateTargetObject() const
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss04PartPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* Boss04Part::GetBaseClass()
	{
		return RT_CLASS_LOCAL(VehiclePart);
	}

	Boss04Part::Boss04Part(Boss04PartPrototypeInfo const& prototype) : VehiclePart(prototype)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04Part::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	Boss04PartPrototypeInfo const* Boss04Part::GetPrototypeInfo() const
	{
		throw std::logic_error("Not implemented");
	}

	int Boss04Part::CollideBoss04PartAndVehiclePart(Boss04Part*, VehiclePart*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04Part::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* Boss04Part::GetClass() const
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss04Part::_OnDurabilityValueBeforeApplyModifier(Modifier const&, float&)
	{
		throw std::logic_error("Not implemented");
	}

	Boss04Part::~Boss04Part()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss04Part::_OnDurabilityValueAfterChange(float)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* Boss04Part::CreateObject()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* Boss04Part::Clone()
	{
		throw std::logic_error("Not implemented");
	}
}
