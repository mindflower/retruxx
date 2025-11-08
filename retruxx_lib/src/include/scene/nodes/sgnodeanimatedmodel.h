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
        SgAnimatedModelNode(const m3d::SgAnimatedModelNode& node);
        virtual  ~SgAnimatedModelNode() override /* 0x00 */;

    public:
        virtual m3d::Object* Clone() override /* 0x04 */;
        static m3d::Object* __fastcall CreateObject();
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x34 */;
        static m3d::Class m_classSgAnimatedModelNode;
        virtual int ReadFromXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) override /* 0x08 */;
        virtual int ReadFromXmlNodeAfterAdd(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) override /* 0x0c */;
        virtual int WriteToXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* writeTo) override /* 0x10 */;
        virtual int SetProperty(unsigned int propId, void* property) override /* 0x14 */;
        virtual int GetProperty(unsigned int propId, void* property) const override /* 0x18 */;
        virtual int GetPropertiesList(retruxx::set<unsigned int, retruxx::less<unsigned int>, retruxx::allocator<unsigned int> >& props) const override /* 0x1c */;
        virtual m3d::DataServer* GetServer() const override /* 0x3c */;
        virtual int Think(int dt, int curTime) override /* 0x54 */;
        virtual int Render(m3d::SgNodeRenderFlags flags, void* data, int dt, int curTime) override /* 0x58 */;
        virtual int UpdateXForm(bool onlyVis, bool recombineWithParentXForm) override /* 0x50 */;

    protected:
        virtual void UpdateOwnBoundingBox() override /* 0x5c */;

    private:
        /* 0x01d4 */ ActionType m_action;
        /* 0x01d8 */ retruxx::vector<enum ActionType, retruxx::allocator<enum ActionType> > m_effectActions;
        /* 0x01e8 */ retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >* m_suppressedLPs;
        /* 0x01ec */ unsigned int m_SkinNumber;
        /* 0x01f0 */ m3d::Configuration m_cfg;
        /* 0x0214 */ bool m_manualAnimControl;
        /* 0x0215 */ bool m_castShadow;
        /* 0x0216 */ char Padding_212[2];
        /* 0x0218 */ int m_frameUpdated;
        /* 0x021c */ int m_lastTimeUpdated;
        /* 0x0220 */ int m_skipFrames;
        /* 0x0224 */ bool m_passable;
        /* 0x0225 */ char Padding_213[3];
        /* 0x0228 */ ai::Obstacle* m_obstacle;
        /* 0x022c */ bool m_imposted;
    }; /* size: 0x0230 */

    static_assert(sizeof(SgAnimatedModelNode) == 0x0230);
}
