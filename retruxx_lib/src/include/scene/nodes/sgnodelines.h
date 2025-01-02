#pragma once
#include "sgnode.h"

namespace m3d
{
    class SgLinesNode : public SgNode
    {
    public:
        static Class*  GetBaseClass();
        static Object*  CreateObject();

    public:
        virtual int Render(SgNodeRenderFlags, void*, int, int);
        virtual int SetProperty(unsigned int, void*);
        virtual DataServer* GetServer() const;
        virtual int GetProperty(unsigned int, void*) const;
        virtual int GetPropertiesList(retruxx::set<unsigned int>&) const;
        virtual Class* GetClass() const;
        virtual Object* Clone();

    protected:
        virtual ~SgLinesNode();
        SgLinesNode(SgLinesNode const&);
        SgLinesNode();
        virtual void UpdateOwnBoundingBox();

    public:
        RT_CLASS_DECLARE(SgLinesNode);

    private:
        unsigned int m_props[2];
    };
}
