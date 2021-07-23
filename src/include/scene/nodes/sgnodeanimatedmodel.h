#pragma once
#include "sgnode.h"
#include <skelmodel.h>
#include <vector>

namespace ai
{
    class Obstacle;
}

namespace m3d
{
    class SgAnimatedModelNode : public SgNode
    {
    public:
        static Class* GetBaseClass();
        static class Object* CreateObject();

    public:
        virtual int UpdateXForm(bool, bool);
        virtual int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);
        virtual int ReadFromXmlNodeAfterAdd(cmn::XmlFile*, cmn::XmlNode*);
        virtual class m3d::Object* Clone();
        virtual int Think(int, int);
        virtual int WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*);
        virtual int Render(SgNodeRenderFlags, void*, int, int);
        virtual int SetProperty(unsigned int, void*);
        virtual class DataServer* GetServer() const;
        virtual int GetPropertiesList(class std::set<unsigned int>&) const;
        virtual int GetProperty(unsigned int, void*) const;
        virtual Class* GetClass() const;

    protected:
        virtual ~SgAnimatedModelNode();
        virtual void UpdateOwnBoundingBox();
        SgAnimatedModelNode();
        SgAnimatedModelNode(SgAnimatedModelNode const&);

    private:
        ActionType m_action;
        std::vector<ActionType> m_effectActions;
        std::set<int>* m_suppressedLPs;
        unsigned int m_SkinNumber;
        Configuration m_cfg;
        bool m_manualAnimControl;
        bool m_castShadow;
        int m_frameUpdated;
        int m_lastTimeUpdated;
        int m_skipFrames;
        bool m_passable;
        ai::Obstacle* m_obstacle;
        bool m_imposted;
    };
}
