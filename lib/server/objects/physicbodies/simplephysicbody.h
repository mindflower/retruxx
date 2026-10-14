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

    class SimplePhysicBody : public ai::PhysicBody
    {
    protected:
        SimplePhysicBody(ai::SimplePhysicBody const&);
        SimplePhysicBody(ai::SimplePhysicBodyPrototypeInfo const& prototypeInfo);
        SimplePhysicBody();

    public:
        virtual ~SimplePhysicBody() override;
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetRtClass() const override;
        static m3d::Class m_classSimplePhysicBody;
        virtual void SetMass(float) = 0;
    };
}  // namespace ai
