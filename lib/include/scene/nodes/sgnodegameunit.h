#pragma once
#include "sgnodeanimatedmodel.h"

namespace m3d
{
    class SgGameUnitNode : public m3d::SgAnimatedModelNode
    {
    protected:
        SgGameUnitNode();
        SgGameUnitNode(const m3d::SgGameUnitNode& node);

    public:
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classSgGameUnitNode;
    };;
}
