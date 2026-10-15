#pragma once
#include "simplephysicbody.h"

namespace ai
{
    class Sphere;

    class SphericBodyPrototypeInfo : public SimplePhysicBodyPrototypeInfo
    {
        friend class SphericBody;

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
        SphericBody(ai::SphericBodyPrototypeInfo const& prototypeInfo);
        SphericBody(
            std::vector<ai::CollisionInfo, std::allocator<ai::CollisionInfo>> const& collisionInfos,
            float massValue);
        SphericBody();
        SphericBody(ai::SphericBody const& rhs);

    public:
        virtual ~SphericBody() override;
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classSphericBody;
        float GetRadius() const;
        void SetRadius(float);
        virtual void SetMass(float newMassValue) override;

    private:
        ai::Sphere const* _Sphere() const;
        ai::Sphere* _Sphere();
    };
}  // namespace ai
