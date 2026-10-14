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
        virtual  ~BoxyBody() override;
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classBoxyBody;
        CVector GetSize() const;
        void SetSize(const CVector&);
        virtual void SetMass(float newMassValue) override;

    private:
        const ai::Box* _Box() const;
        ai::Box* _Box();
    };
}
