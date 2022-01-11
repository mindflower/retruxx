#pragma once
#include "sgnode.h"

namespace m3d
{
    class SgSpriteNode : public SgNode
    {
    public:
        static Object* CreateObject();
        static Class* GetBaseClass();

    public:
        virtual Class* GetClass() const;
        virtual int GetPropertiesList(std::set<unsigned int>&) const;
        virtual int Render(SgNodeRenderFlags, void*, int, int);
        virtual DataServer* GetServer() const;
        virtual Object* Clone();
        virtual int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);
        virtual int SetProperty(unsigned int, void*);
        virtual int WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*);
        virtual int GetProperty(unsigned int, void*) const;

    protected:
        virtual ~SgSpriteNode();
        virtual void UpdateOwnBoundingBox();
        SgSpriteNode();
        SgSpriteNode(SgSpriteNode const&);

    public:
        RT_CLASS_INLINE_DECLARE(SgSpriteNode);

    private:
        unsigned int m_props[6];
    };
}
