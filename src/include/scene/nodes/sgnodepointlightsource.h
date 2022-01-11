#pragma once
#include "sgnode.h"

namespace m3d
{
    class SgPointLightSourceNode : public SgNode
    {
    public:
        static Class* GetBaseClass();
        static Object* CreateObject();

    public:
        virtual Object* Clone();
        virtual DataServer* GetServer() const;
        virtual int GetProperty(unsigned int, void*) const;
        virtual int Render(SgNodeRenderFlags, void*, int, int);
        virtual int SetProperty(unsigned int, void*);
        virtual int GetPropertiesList(std::set<unsigned int>&) const;

        virtual Class* GetClass() const;
    protected:
        virtual ~SgPointLightSourceNode();
        SgPointLightSourceNode();
        SgPointLightSourceNode(SgPointLightSourceNode const&);
        virtual void UpdateOwnBoundingBox();

    public:
        RT_CLASS_INLINE_DECLARE(SgPointLightSourceNode);

    private:
        unsigned int m_props[3];
    };
}
