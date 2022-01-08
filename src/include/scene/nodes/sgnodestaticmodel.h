#pragma once
#include "sgnode.h"

namespace m3d
{
    class SgStaticModelNode : public SgNode
    {
    public:
        RT_CLASS_DECLARE(SgStaticModelNode);

    public:
        static Object* CreateObject();
        static Class* GetBaseClass();

    public:
        virtual Class* GetClass() const;
        virtual DataServer* GetServer() const;
        virtual int GetPropertiesList(class std::set<unsigned int>&) const;
        virtual int Render(SgNodeRenderFlags, void*, int, int);
        virtual Object* Clone();
        virtual int GetProperty(unsigned int, void*) const;
        virtual int SetProperty(unsigned int, void*);

    protected:
        SgStaticModelNode();
        SgStaticModelNode(SgStaticModelNode const&);
        virtual ~SgStaticModelNode();
        virtual void UpdateOwnBoundingBox();
    };
}
