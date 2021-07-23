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
        static m3d::Class* __fastcall GetBaseClass();
        virtual AnimatedComplexPhysicObjPrototypeInfo const* GetPrototypeInfo() const;
        virtual void Update(float, unsigned int);
        virtual m3d::Class* GetClass() const;

    protected:
        virtual ~AnimatedComplexPhysicObj();

    private:
        static m3d::Object* __fastcall CreateObject();
        virtual m3d::Object* Clone();
    };
}
