#include "animatedcomplexphysicobj.h"

#include <stdexcept>

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(AnimatedComplexPhysicObj)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(AnimatedComplexPhysicObj);

	AnimatedComplexPhysicObjPrototypeInfo::AnimatedComplexPhysicObjPrototypeInfo()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	Obj* AnimatedComplexPhysicObjPrototypeInfo::CreateTargetObject() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	AnimatedComplexPhysicObj::AnimatedComplexPhysicObj(AnimatedComplexPhysicObjPrototypeInfo const& prototypeInfo) :
		ComplexPhysicObj(prototypeInfo)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Class* AnimatedComplexPhysicObj::GetBaseClass()
	{
		return RT_CLASS_LOCAL(ComplexPhysicObj);
	}

	AnimatedComplexPhysicObjPrototypeInfo const* AnimatedComplexPhysicObj::GetPrototypeInfo() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void AnimatedComplexPhysicObj::Update(float, unsigned)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Class* AnimatedComplexPhysicObj::GetClass() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	AnimatedComplexPhysicObj::~AnimatedComplexPhysicObj()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Object* AnimatedComplexPhysicObj::CreateObject()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Object* AnimatedComplexPhysicObj::Clone()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}
}
