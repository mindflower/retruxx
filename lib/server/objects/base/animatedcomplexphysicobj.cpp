#include "animatedcomplexphysicobj.h"

#include "prototypemanager.h"

#include <core/kernel.h>
#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(AnimatedComplexPhysicObj)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(AnimatedComplexPhysicObj);

    AnimatedComplexPhysicObjPrototypeInfo::AnimatedComplexPhysicObjPrototypeInfo() = default;

    Obj* AnimatedComplexPhysicObjPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x8511F0
        return new AnimatedComplexPhysicObj(*this);
    }

    AnimatedComplexPhysicObj::AnimatedComplexPhysicObj(AnimatedComplexPhysicObjPrototypeInfo const& prototypeInfo) : ComplexPhysicObj(prototypeInfo)
    {
        // RVA 0x851160 - nothing of its own; the class exists only to re-run _Construct each
        // frame so that the parts follow the model's animation.
    }

    m3d::Class* AnimatedComplexPhysicObj::GetBaseClass()
    {
        return RT_CLASS_LOCAL(ComplexPhysicObj);
    }

    AnimatedComplexPhysicObjPrototypeInfo const* AnimatedComplexPhysicObj::GetPrototypeInfo() const
    {
        // RVA 0x8515E0
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()),
            AnimatedComplexPhysicObjPrototypeInfo const);
    }

    void AnimatedComplexPhysicObj::Update(float elapsedTime, unsigned workTime)
    {
        // RVA 0x851190
        // NOTE: chains to PhysicObj rather than to ComplexPhysicObj, so the parts are not
        // updated the usual way - the _Construct below rebuilds them from the animation instead.
        PhysicObj::Update(elapsedTime, workTime);
        _Construct(true);
    }

    m3d::Class* AnimatedComplexPhysicObj::GetClass() const
    {
        // RVA 0x851130
        return RT_CLASS_LOCAL(AnimatedComplexPhysicObj);
    }

    AnimatedComplexPhysicObj::~AnimatedComplexPhysicObj()
    {
        // RVA 0x851180 - nothing of its own to release.
    }

    m3d::Object* AnimatedComplexPhysicObj::CreateObject()
    {
        // RVA 0x851420
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* AnimatedComplexPhysicObj::Clone()
    {
        // RVA 0x851260
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai
