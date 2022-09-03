#pragma once
#include "sgnodeanimatedmodel.h"

namespace m3d
{
    class SgGameUnitNode : public m3d::SgAnimatedModelNode
    {
    public:
        static Object* CreateObject();
        static Class* GetBaseClass();

    public:
        virtual Class* GetClass() const;
        virtual Object* Clone();

    protected:
        virtual ~SgGameUnitNode();
        SgGameUnitNode(m3d::SgGameUnitNode const&);
        SgGameUnitNode();

    public:
        RT_CLASS_DECLARE(SgGameUnitNode);
    };
}
