#pragma once
#include "simplephysicbody.h"

namespace ai
{
    class Box;

    class BoxyBody :  public SimplePhysicBody
    {
    public:
        BoxyBody(std::vector<CollisionInfo,std::allocator<CollisionInfo> > const &,float);
        virtual m3d::Class * GetClass() const ;
        virtual m3d::Object * Clone();
        virtual void SetMass(float);
        static m3d::Object * __fastcall CreateObject();
        static m3d::Class * __fastcall GetBaseClass();
        virtual ~BoxyBody();
        CVector GetSize() const ;

    protected:
        BoxyBody();
        BoxyBody(BoxyBody const &);

    private:
        Box const * _Box() const ;
    };
}
