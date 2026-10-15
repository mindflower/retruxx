#pragma once
#include "sgnode.h"
#include <skelmodel.h>

namespace ai
{
    class Obstacle;
}

namespace m3d
{
    class SgAnimatedModelNode : public m3d::SgNode
    {
        friend class AnimatedModelsServer;

    protected:
        SgAnimatedModelNode();
        SgAnimatedModelNode(m3d::SgAnimatedModelNode const& node);
        virtual ~SgAnimatedModelNode() override;

    public:
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classSgAnimatedModelNode;
        virtual int ReadFromXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) override;
        virtual int ReadFromXmlNodeAfterAdd(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) override;
        virtual int WriteToXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* writeTo) override;
        virtual int SetProperty(unsigned int propId, void* property) override;
        virtual int GetProperty(unsigned int propId, void* property) const override;
        virtual int GetPropertiesList(
            retruxx::set<unsigned int, retruxx::less<unsigned int>, retruxx::allocator<unsigned int>>& props)
            const override;
        virtual m3d::DataServer* GetServer() const override;
        virtual int Think(int dt, int curTime) override;
        virtual int Render(m3d::SgNodeRenderFlags flags, void* data, int dt, int curTime) override;
        virtual int UpdateXForm(bool onlyVis, bool recombineWithParentXForm) override;

    protected:
        virtual void UpdateOwnBoundingBox() override;

    private:
        ActionType m_action;
        retruxx::vector<enum ActionType, retruxx::allocator<enum ActionType>> m_effectActions;
        retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>* m_suppressedLPs;
        unsigned int m_SkinNumber;
        m3d::Configuration m_cfg;
        bool m_manualAnimControl;
        bool m_castShadow;
        int m_frameUpdated;
        int m_lastTimeUpdated;
        int m_skipFrames;
        bool m_passable;
        ai::Obstacle* m_obstacle;
        bool m_imposted;
    };
}  // namespace m3d
