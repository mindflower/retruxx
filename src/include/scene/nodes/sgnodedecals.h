#pragma once
#include "sgnode.h"
#include <decals.h>

namespace m3d
{
    class SgDecalsNode : public SgNode
    {
    public:
        static Class* GetBaseClass();
        static Object* CreateObject();

    public:
        virtual int GetProperty(unsigned int, void*) const;
        unsigned int GetNumDecals();
        virtual int SetProperty(unsigned int, void*);
        DecalInfo const& GetDecal(int);
        virtual DataServer* GetServer() const;
        virtual Object* Clone();
        virtual int Render(SgNodeRenderFlags, void*, int, int);
        virtual int GetPropertiesList(std::set<unsigned int>&) const;
        virtual Class* GetClass() const;

    protected:
        virtual ~SgDecalsNode();
        SgDecalsNode(SgDecalsNode const&);
        SgDecalsNode();
        virtual void UpdateOwnBoundingBox();

    public:
        RT_CLASS_DECLARE(SgDecalsNode);

    private:
        DecalsList m_decals;
    };
}
