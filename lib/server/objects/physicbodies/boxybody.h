#pragma once
#include "simplephysicbody.h"

namespace ai
{
    class Box;

    class BoxyBody : public ai::SimplePhysicBody
    {
        friend class SimplePhysicObjPrototypeInfo;

    protected:
        BoxyBody(
            std::vector<ai::CollisionInfo, std::allocator<ai::CollisionInfo>> const& collisionInfos,
            float massValue);
        BoxyBody();
        BoxyBody(ai::BoxyBody const& rhs);

    public:
        virtual ~BoxyBody() override;
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classBoxyBody;
        CVector GetSize() const;
        void SetSize(CVector const&);
        virtual void SetMass(float newMassValue) override;

    private:
        ai::Box const* _Box() const;
        ai::Box* _Box();
    };
}  // namespace ai
