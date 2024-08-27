#pragma once
#include "complexphysicobj.h"

namespace ai
{
    class AnimatedComplexPhysicObjPrototypeInfo : public ComplexPhysicObjPrototypeInfo
    {
    public:
        AnimatedComplexPhysicObjPrototypeInfo();
        virtual Obj* CreateTargetObject() const;
    };

    class AnimatedComplexPhysicObj : public ComplexPhysicObj
    {
    public:
        AnimatedComplexPhysicObj(AnimatedComplexPhysicObjPrototypeInfo const&);
        static m3d::Class* GetBaseClass();
        virtual AnimatedComplexPhysicObjPrototypeInfo const* GetPrototypeInfo() const;
        virtual void Update(float, unsigned int);
        virtual m3d::Class* GetClass() const;

    protected:
        virtual ~AnimatedComplexPhysicObj();

    private:
        static m3d::Object* CreateObject();
        virtual m3d::Object* Clone();

    public:
        RT_CLASS_DECLARE(AnimatedComplexPhysicObj);
    };
}
