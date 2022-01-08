#pragma once
#include "sgnodeanimatedmodel.h"

namespace m3d
{
    class SgGameUnitNode : public m3d::SgAnimatedModelNode
    {
    public:
        RT_CLASS_DECLARE(SgGameUnitNode);

    public:
        static Object* CreateObject();
        static Class* GetBaseClass();

    public:
        virtual Class* GetClass() const;
        virtual Object* Clone();

    protected:
        virtual ~SgGameUnitNode();
        SgGameUnitNode(class m3d::SgGameUnitNode const&);
        SgGameUnitNode();
    };
}
