#pragma once
#include "simplephysicbody.h"

namespace ai
{
    class GenericBody : public ai::SimplePhysicBody
    {
        friend class SimplePhysicObjPrototypeInfo;
    protected:
        GenericBody(float massValue);
        GenericBody();
        GenericBody(const ai::GenericBody& rhs);

    public:
        virtual  ~GenericBody() override /* 0x00 */;
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* CreateObject();
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classGenericBody;
        virtual void SetMass(float newMassValue) override /* 0x160 */;
    }; /* size: 0x0158 */
}
