#include "animatedcomplexphysicobj.h"

#include <stdexcept>

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(AnimatedComplexPhysicObj)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(AnimatedComplexPhysicObj);

	AnimatedComplexPhysicObjPrototypeInfo::AnimatedComplexPhysicObjPrototypeInfo()
	{
		throw std::logic_error("Not implemented");
	}

	Obj* AnimatedComplexPhysicObjPrototypeInfo::CreateTargetObject() const
	{
		throw std::logic_error("Not implemented");
	}

	AnimatedComplexPhysicObj::AnimatedComplexPhysicObj(AnimatedComplexPhysicObjPrototypeInfo const& prototypeInfo) :
		ComplexPhysicObj(prototypeInfo)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* AnimatedComplexPhysicObj::GetBaseClass()
	{
		return RT_CLASS_LOCAL(ComplexPhysicObj);
	}

	AnimatedComplexPhysicObjPrototypeInfo const* AnimatedComplexPhysicObj::GetPrototypeInfo() const
	{
		throw std::logic_error("Not implemented");
	}

	void AnimatedComplexPhysicObj::Update(float, unsigned)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* AnimatedComplexPhysicObj::GetClass() const
	{
		throw std::logic_error("Not implemented");
	}

	AnimatedComplexPhysicObj::~AnimatedComplexPhysicObj()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* AnimatedComplexPhysicObj::CreateObject()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* AnimatedComplexPhysicObj::Clone()
	{
		throw std::logic_error("Not implemented");
	}
}
