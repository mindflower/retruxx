#pragma once
#include "sgnode.h"

namespace m3d
{
    class SgProjectorNode : public SgNode
    {
    public:
        static Class* GetBaseClass();
        static Object* CreateObject();

    public:
        virtual Class* GetClass() const;
        virtual int Render(SgNodeRenderFlags, void*, int, int);
        virtual int SetProperty(unsigned int, void*);
        virtual DataServer* GetServer() const;
        virtual int GetProperty(unsigned int, void*) const;
        virtual int GetPropertiesList(class std::set<unsigned int, struct std::less<unsigned int>, class std::allocator<unsigned int> >&) const;
        virtual Object* Clone();

    protected:
        virtual ~SgProjectorNode();
        virtual void UpdateOwnBoundingBox();
        SgProjectorNode(SgProjectorNode const&);
        SgProjectorNode();
    private:
        unsigned int m_props[2];
    };
}
