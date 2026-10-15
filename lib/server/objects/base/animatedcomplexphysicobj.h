#pragma once
#include "complexphysicobj.h"

namespace ai
{
    class AnimatedComplexPhysicObjPrototypeInfo : public ai::ComplexPhysicObjPrototypeInfo
    {
    public:
        AnimatedComplexPhysicObjPrototypeInfo();
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class AnimatedComplexPhysicObj : public ai::ComplexPhysicObj
    {
        // CreateTargetObject builds one of these.
        friend class AnimatedComplexPhysicObjPrototypeInfo;

    protected:
        virtual ~AnimatedComplexPhysicObj() override;

    protected:
        AnimatedComplexPhysicObj(ai::AnimatedComplexPhysicObjPrototypeInfo const& prototypeInfo);
        AnimatedComplexPhysicObj(ai::AnimatedComplexPhysicObj const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classAnimatedComplexPhysicObj;
        virtual ai::AnimatedComplexPhysicObjPrototypeInfo const* GetPrototypeInfo() const override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
    };
}  // namespace ai
