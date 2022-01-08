#pragma once
#include "physicbody.h"

namespace ai
{
    class SimplePhysicBodyPrototypeInfo : public PhysicBodyPrototypeInfo
    {
    public:
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        SimplePhysicBodyPrototypeInfo();
    };

    class SimplePhysicBody : public PhysicBody
    {
    public:
        SimplePhysicBody(ai::SimplePhysicBodyPrototypeInfo const&);
        virtual ~SimplePhysicBody();
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetRtClass() const;

    protected:
        SimplePhysicBody();
    };
}
