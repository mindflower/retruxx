#pragma once
#include "simplephysicbody.h"

namespace ai
{
    class GenericBody :  public SimplePhysicBody
    {
    public:
        GenericBody(float);
        virtual void SetMass(float);
        virtual m3d::Object * Clone();
        static m3d::Class * GetBaseClass();
        virtual m3d::Class * GetClass() const ;
        static m3d::Object * CreateObject();
        virtual ~GenericBody();

    protected:
        GenericBody();
        GenericBody(GenericBody const &);
    };
}
