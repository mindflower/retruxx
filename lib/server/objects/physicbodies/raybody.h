#pragma once
#include "simplephysicbody.h"
#include "geoms/ray.h"

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

    class RayBody : public ai::SimplePhysicBody
    {
        friend class SimplePhysicObjPrototypeInfo;
    protected:
        RayBody(const ai::RayBodyPrototypeInfo& prototypeInfo);
        RayBody(float length);
        RayBody();
        RayBody(const ai::RayBody& rhs);

    public:
        virtual  ~RayBody() override /* 0x00 */;
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* CreateObject();
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classRayBody;
        float GetLength() const;
        void SetLength(float);
        CVector GetDirection() const;
        void SetDirection(const CVector&);
        virtual void SetMass(float newMassValue) override /* 0x160 */;

    private:
        const ai::Ray* _Ray() const;
        ai::Ray* _Ray();
    }; /* size: 0x0158 */
}
