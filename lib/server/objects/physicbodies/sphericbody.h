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

    class SphericBody : public ai::SimplePhysicBody
    {
        friend class SimplePhysicObjPrototypeInfo;
    protected:
        SphericBody(const ai::SphericBodyPrototypeInfo& prototypeInfo);
        SphericBody(const std::vector<ai::CollisionInfo, std::allocator<ai::CollisionInfo> >& collisionInfos, float massValue);
        SphericBody();
        SphericBody(const ai::SphericBody& rhs);

    public:
        virtual  ~SphericBody() override /* 0x00 */;
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classSphericBody;
        float GetRadius() const;
        void SetRadius(float);
        virtual void SetMass(float newMassValue) override /* 0x160 */;

    private:
        const ai::Sphere* _Sphere() const;
        ai::Sphere* _Sphere();
    }; /* size: 0x0158 */
}
