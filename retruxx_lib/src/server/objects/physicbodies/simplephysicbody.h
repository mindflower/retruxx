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
        SimplePhysicBody(const ai::SimplePhysicBody&);
        SimplePhysicBody(const ai::SimplePhysicBodyPrototypeInfo& prototypeInfo);
        SimplePhysicBody();

    public:
        virtual  ~SimplePhysicBody() override /* 0x00 */;
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetRtClass() const override /* 0x00 */;
        static m3d::Class m_classSimplePhysicBody;
        virtual void SetMass(float) = 0 /* 0x160 */;
    }; /* size: 0x0158 */
}
