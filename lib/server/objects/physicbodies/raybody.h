#pragma once
#include "simplephysicbody.h"
#include "geoms/ray.h"

namespace ai
{
    class RayBodyPrototypeInfo : public SimplePhysicBodyPrototypeInfo
    {
        friend class RayBody;

    public:
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        RayBodyPrototypeInfo();

    private:
        float m_length;
    };

    class RayBody : public ai::SimplePhysicBody
    {
        friend class SimplePhysicObjPrototypeInfo;

    protected:
        RayBody(ai::RayBodyPrototypeInfo const& prototypeInfo);
        RayBody(float length);
        RayBody();
        RayBody(ai::RayBody const& rhs);

    public:
        virtual ~RayBody() override;
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classRayBody;
        float GetLength() const;
        void SetLength(float);
        CVector GetDirection() const;
        void SetDirection(CVector const&);
        virtual void SetMass(float newMassValue) override;

    private:
        ai::Ray const* _Ray() const;
        ai::Ray* _Ray();
    };
}  // namespace ai
