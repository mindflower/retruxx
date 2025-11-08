#pragma once
#include "complexphysicobj.h"

namespace ai
{
    class AnimatedComplexPhysicObjPrototypeInfo : public ai::ComplexPhysicObjPrototypeInfo
    {
    public:
        AnimatedComplexPhysicObjPrototypeInfo();
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
    }; /* size: 0x0090 */

    static_assert(sizeof(AnimatedComplexPhysicObjPrototypeInfo) == 0x0090);

    class AnimatedComplexPhysicObj : public ai::ComplexPhysicObj
    {
    protected:
        virtual  ~AnimatedComplexPhysicObj() override /* 0x00 */;

    protected:
        AnimatedComplexPhysicObj(const ai::AnimatedComplexPhysicObjPrototypeInfo& prototypeInfo);
        AnimatedComplexPhysicObj(const ai::AnimatedComplexPhysicObj&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classAnimatedComplexPhysicObj;
        virtual const ai::AnimatedComplexPhysicObjPrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
    }; /* size: 0x014c */

    static_assert(sizeof(AnimatedComplexPhysicObj) == 0x014c);
}
