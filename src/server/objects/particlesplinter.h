#pragma once
#include "dummyobject.h"

namespace ai
{
    class ParticleSplinterPrototypeInfo : public DummyObjectPrototypeInfo
    {
    public:
        virtual Obj* CreateTargetObject() const;
    };

    class ParticleSplinter : public DummyObject
    {
    public:
        static m3d::Class m_classParticleSplinter;

    public:
        static m3d::Class* __fastcall GetBaseClass();
        ParticleSplinter(ParticleSplinterPrototypeInfo const&);
        virtual ParticleSplinterPrototypeInfo const* GetPrototypeInfo() const;
        virtual m3d::Class* GetClass() const;
        virtual void Update(float, unsigned int);

    protected:
        virtual ~ParticleSplinter();

    private:
        static m3d::Object* __fastcall CreateObject();
        virtual m3d::Object* Clone();
        void CheckDisablePhysics();
    };
}
