#pragma once
#include "sgnodeanimatedmodel.h"

namespace m3d
{
    class SgGameUnitNode : public m3d::SgAnimatedModelNode
    {
    protected:
        SgGameUnitNode();
        SgGameUnitNode(const m3d::SgGameUnitNode& node);
        virtual  ~SgGameUnitNode() override /* 0x00 */;

    public:
        virtual m3d::Object* Clone() override /* 0x04 */;
        static m3d::Object* __fastcall CreateObject();
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x34 */;
        static m3d::Class m_classSgGameUnitNode;
    }; /* size: 0x0230 */;
}
