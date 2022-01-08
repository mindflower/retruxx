#pragma once
#include "sgnode.h"

namespace m3d
{
    class SgLoadpointNode : public SgNode
    {
    public:
        RT_CLASS_DECLARE(SgLoadpointNode);

    public:
        static Class* GetBaseClass();
        static Object* CreateObject();

    public:
        virtual Object* Clone(void);
        virtual int Think(int, int);
        virtual Class* GetClass() const;

    protected:
        virtual ~SgLoadpointNode();
        SgLoadpointNode(SgLoadpointNode const&);
        SgLoadpointNode();
    };
}
