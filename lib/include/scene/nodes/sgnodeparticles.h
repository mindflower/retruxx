#pragma once
#include "sgnode.h"

namespace m3d
{
    class SgParticlesNode : public SgNode
    {
    public:
        static Object* CreateObject();
        static Class* GetBaseClass();

    public:
        virtual int WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*);
        virtual int Think(int, int);
        virtual Object* Clone();
        virtual int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);
        virtual Class* GetClass() const;
        virtual bool IsFree() const;
        virtual int GetProperty(unsigned int, void*) const;
        virtual int GetPropertiesList(retruxx::set<unsigned int, retruxx::less<unsigned int>>&) const;
        virtual int SetProperty(unsigned int, void*);
        virtual void Restart();
        virtual DataServer* GetServer() const;
        virtual int Render(SgNodeRenderFlags, void*, int, int);
        virtual void CanBeFree();

    protected:
        virtual ~SgParticlesNode();
        SgParticlesNode(SgParticlesNode const&);
        SgParticlesNode();
        virtual void UpdateOwnBoundingBox();

    public:
        RT_CLASS_DECLARE(SgParticlesNode);

    private:
        int m_lastTimeUpdated;
        int m_numMesh;
        mutable unsigned int m_Parts0Times;
    };

    class SgParticlesOpaqueNode : public SgParticlesNode
    {
    public:
        static Class* GetBaseClass();
        static Object* CreateObject();

    public:
        virtual ~SgParticlesOpaqueNode();
        virtual Class* GetClass() const;
        virtual Object* Clone();

    protected:
        SgParticlesOpaqueNode(SgParticlesOpaqueNode const&);
        SgParticlesOpaqueNode();

    public:
        RT_CLASS_DECLARE(SgParticlesOpaqueNode);
    };
}
