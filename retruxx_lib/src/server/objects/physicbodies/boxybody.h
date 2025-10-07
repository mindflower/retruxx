#pragma once
#include "simplephysicbody.h"

namespace ai
{
    class Box;

    class BoxyBody : public ai::SimplePhysicBody
    {
        friend class SimplePhysicObjPrototypeInfo;
    protected:
        BoxyBody(const std::vector<ai::CollisionInfo, std::allocator<ai::CollisionInfo> >& collisionInfos, float massValue);
        BoxyBody();
        BoxyBody(const ai::BoxyBody& rhs);

    public:
        virtual  ~BoxyBody() override /* 0x00 */;
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classBoxyBody;
        CVector GetSize() const;
        void SetSize(const CVector&);
        virtual void SetMass(float newMassValue) override /* 0x160 */;

    private:
        const ai::Box* _Box() const;
        ai::Box* _Box();
    }; /* size: 0x0158 */
}
