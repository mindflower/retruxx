#pragma once
#include "simplephysicbody.h"

namespace ai
{
    class Sphere;

    class SphericBodyPrototypeInfo : public SimplePhysicBodyPrototypeInfo
    {
    public:
        SphericBodyPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        float m_radius;
    };

    class SphericBody :  public SimplePhysicBody
    {
    public:
        virtual m3d::Object * Clone();
        static m3d::Class * __fastcall GetBaseClass();
        virtual void SetMass(float);
        SphericBody(std::vector<CollisionInfo,std::allocator<CollisionInfo> > const &,float);
        SphericBody(SphericBodyPrototypeInfo const &);
        float GetRadius() const ;
        virtual ~SphericBody();
        virtual m3d::Class * GetClass() const ;
        static m3d::Object * __fastcall CreateObject();

    protected:
        SphericBody();
        SphericBody(SphericBody const &);
    private:
        Sphere const * _Sphere() const ;
    };
}
