#include "boss03part.h"

#include <stdexcept>

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Boss03Part)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Boss03Part);

	Obj* Boss03PartPrototypeInfo::CreateTargetObject() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Boss03PartPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int Boss03Part::CollideBoss03PartAndVehiclePart(Boss03Part*, VehiclePart*, dContact*, unsigned&, bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	Boss03PartPrototypeInfo const* Boss03Part::GetPrototypeInfo() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	Boss03Part::Boss03Part(Boss03PartPrototypeInfo const& prototypeInfo) : VehiclePart(prototypeInfo)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Boss03Part::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Boss03Part::SetDamageable(bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Class* Boss03Part::GetBaseClass()
	{
		return RT_CLASS_LOCAL(VehiclePart);
	}

	m3d::Class* Boss03Part::GetClass() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Boss03Part::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	Boss03Part::~Boss03Part()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Boss03Part::_OnDurabilityValueBeforeApplyModifier(Modifier const&, float&)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Boss03Part::_OnDurabilityValueAfterChange(float)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Object* Boss03Part::Clone()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Object* Boss03Part::CreateObject()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}
}
