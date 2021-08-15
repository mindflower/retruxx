#pragma once
#include "sgnode.h"

namespace m3d
{
    class SgSoundSourceNode : public SgNode
    {
    public:
        static Object* CreateObject();
        static Class* GetBaseClass();

    public:
        static Class m_classSgSoundSourceNode;

    public:
        virtual void CanBeFree();
        virtual int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);
        virtual DataServer* GetServer() const;
        virtual int GetPropertiesList(std::set<unsigned int>&) const;
        virtual Class* GetClass() const;
        virtual int WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*);
        virtual float IntersectRay(CVector const&, CVector const&, SgNode*&, Class*);
        virtual int Render(SgNodeRenderFlags, void*, int, int);
        virtual Object* Clone();
        virtual int GetProperty(unsigned int, void*) const;
        virtual int SetProperty(unsigned int, void*);
        virtual void Restart();
        virtual bool IsFree() const;

    protected:
        virtual ~SgSoundSourceNode();
        SgSoundSourceNode();
        SgSoundSourceNode(SgSoundSourceNode const&);
        virtual void UpdateOwnBoundingBox();
    private:
        bool _OnSoundStopped();
        int _InternalRender();
        unsigned int m_props[6];
        int m_currentSoundNum;
        int m_framesPassed;
    };
}
