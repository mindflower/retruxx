#pragma once
#include "simplephysicbody.h"

namespace ai
{
    class RayBodyPrototypeInfo : public SimplePhysicBodyPrototypeInfo
    {
    public:
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        RayBodyPrototypeInfo();

    private:
        float m_length;
    };

    class RayBody :  public SimplePhysicBody
    {
    public:
        virtual void SetMass(float);
        static m3d::Class * GetBaseClass();
        virtual m3d::Object * Clone();
        static m3d::Object * CreateObject();
        virtual m3d::Class * GetClass() const ;
        RayBody(RayBodyPrototypeInfo const &);
        RayBody(float);
        virtual ~RayBody();

    protected:
        RayBody(RayBody const &);
        RayBody();

    };
}
