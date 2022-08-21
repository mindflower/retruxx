#include "boss03part.h"

#include <stdexcept>

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Boss03Part)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Boss03Part);

	Obj* Boss03PartPrototypeInfo::CreateTargetObject() const
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss03PartPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	int Boss03Part::CollideBoss03PartAndVehiclePart(Boss03Part*, VehiclePart*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}

	Boss03PartPrototypeInfo const* Boss03Part::GetPrototypeInfo() const
	{
		throw std::logic_error("Not implemented");
	}

	Boss03Part::Boss03Part(Boss03PartPrototypeInfo const& prototypeInfo) : VehiclePart(prototypeInfo)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03Part::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03Part::SetDamageable(bool)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* Boss03Part::GetBaseClass()
	{
		return RT_CLASS_LOCAL(VehiclePart);
	}

	m3d::Class* Boss03Part::GetClass() const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03Part::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw std::logic_error("Not implemented");
	}

	Boss03Part::~Boss03Part()
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss03Part::_OnDurabilityValueBeforeApplyModifier(Modifier const&, float&)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss03Part::_OnDurabilityValueAfterChange(float)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* Boss03Part::Clone()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* Boss03Part::CreateObject()
	{
		throw std::logic_error("Not implemented");
	}
}
