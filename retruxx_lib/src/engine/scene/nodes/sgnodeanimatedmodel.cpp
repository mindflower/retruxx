#include <scene/nodes/sgnodeanimatedmodel.h>
#include <m3dapp.h>
#include <scene/servers/dataserver.h>
#include <core/ini.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(SgAnimatedModelNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgAnimatedModelNode);

    Class* SgAnimatedModelNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    Object* SgAnimatedModelNode::CreateObject()
    {
        return new SgAnimatedModelNode;
    }

    int SgAnimatedModelNode::UpdateXForm(bool onlyVis, bool recombineWithParentXForm)
    {
        int result = SgNode::UpdateXForm(onlyVis, recombineWithParentXForm);
        if (this->m_imposted
            && (fabs(this->m_currentWorldRotation.x) >= 1.0e-10 || fabs(this->m_currentWorldRotation.z) >= 1.0e-10))
        {
            this->m_imposted = 0;
        }
        return result;
    }

    int SgAnimatedModelNode::ReadFromXmlNode(cmn::XmlFile* file, cmn::XmlNode* node)
    {
        auto result = SgNode::ReadFromXmlNode(file, node);
        if (result)
        {
            int skin = 0;
            SafeIntAttrib(skin, node, "skin");
            SetProperty(8706, &skin);

            int cfg = 0;
            SafeIntAttrib(cfg, node, "cfg");
            SetProperty(8707, &cfg);

            SafeBoolAttrib(this->m_castShadow, node, "CastShadow");
        }
        return result;
    }

    int SgAnimatedModelNode::ReadFromXmlNodeAfterAdd(cmn::XmlFile* file, cmn::XmlNode* node)
    {
        auto result = SgNode::ReadFromXmlNode(file, node);
        if (result)
        {
            SafeIntAttrib((int&)m_action, node, "ndmAction");
            
            m3d::AnimatedModel* mdl = nullptr;
            GetServer()->GetItemProperty(m_srvId, 16394, &mdl);
            if (mdl)
            {
                m_passable = mdl->bIsPassable();
            }
            m3d::SafeBoolAttrib(this->m_passable, node, "passable");
            SetProperty(8704, &m_action);
        }
        return result;
    }

    m3d::Object* SgAnimatedModelNode::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgAnimatedModelNode::Think(int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgAnimatedModelNode::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgAnimatedModelNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgAnimatedModelNode::SetProperty(unsigned propId, void* property)
    {
        if (SgNode::SetProperty(propId, property))
            return 1;

        switch (propId)
        {
        case 0x1108u:
            retruxx::logic_error("Not implemented");
            return 1;
        case 0x2200u:
            retruxx::logic_error("Not implemented");
            return 1;
        case 0x2204u:
            retruxx::logic_error("Not implemented");
            return 1;
        case 0x2207u:
            retruxx::logic_error("Not implemented");
        case 0x2208u:
            retruxx::logic_error("Not implemented");
        case 0x2206u:
            retruxx::logic_error("Not implemented");
        case 0x220Bu:
            retruxx::logic_error("Not implemented");
        case 0x2205u:
            retruxx::logic_error("Not implemented");
            return 1;
        case 0x2202u:
            retruxx::logic_error("Not implemented");
            return 1;
        case 0x2203u:
            retruxx::logic_error("Not implemented");
            return 0;
        case 0x220Au:
            retruxx::logic_error("Not implemented");
            return 1;
        case 0x220Cu:
            retruxx::logic_error("Not implemented");
            return 1;
        }

        if (propId != 8717)
        {
            if (propId == 8719)
            {
                this->m_lastTimeUpdated = *(int*)property;
                return 1;
            }
            if (propId == 8720)
            {
                this->m_imposted = *(bool*)property;
                return 1;
            }
            if (propId != 8721)
                return 0;
            this->m_castShadow = *(bool*)property;
            return 1;
        }

        this->m_passable = *(bool*)property;
        retruxx::logic_error("Not implemented");
    }

    DataServer* SgAnimatedModelNode::GetServer() const
    {
        return &M3D_APP->GetAnimatedModelsServer();
    }

    int SgAnimatedModelNode::GetPropertiesList(retruxx::set<unsigned>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgAnimatedModelNode::GetProperty(unsigned propId, void* property) const
    {
        if (SgNode::GetProperty(propId, property))
            return 1;

        // TODO: check this
        switch (propId)
        {
        case 0x1108u:
            *(int*)property = this->m_srvId;
            return 1;
        case 0x2200u:
        case 0x2205u:
            *(int*)property = this->m_action;
            return 1;
        case 0x2204u:
            *(ActionType*)property = this->m_effectActions[0];
            return 1;
        case 0x2206u:
            *(retruxx::vector<ActionType>*)property = this->m_effectActions;
            return 1;
        case 0x2202u:
            *(int*)property = this->m_SkinNumber;
            return 1;
        case 0x2203u:
            *(const Configuration**)property = &(this->m_cfg);
            return 1;
        case 0x2209u:
            GetServer()->GetItemProperty(this->m_srvId, 8713, property);
            return 1;
        case 0x220Au:
            *(retruxx::set<int>**)property = this->m_suppressedLPs;
            return 1;
        case 0x220Cu:
            *(int*)property = this->m_manualAnimControl;
            return 1;
        case 0x220Du:
            *(bool*)property = this->m_passable;
            return 1;
        case 0x220Eu:
            *(ai::Obstacle**)property = this->m_obstacle;
            return 1;
        case 0x2210u:
            *(bool*)property = this->m_imposted;
            return 1;
        case 0x2211u:
            *(bool*)property = this->m_castShadow;
            return 1;
        default:
            return 0;
        }
    }

    Class* SgAnimatedModelNode::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgAnimatedModelNode::~SgAnimatedModelNode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgAnimatedModelNode::UpdateOwnBoundingBox()
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgAnimatedModelNode::SgAnimatedModelNode()
    {
        this->m_cfg.m_num = 0;
        this->m_action = AT_STAND1;
        m_effectActions.push_back(AT_STAND1);
        this->m_SkinNumber = 0;
        this->m_suppressedLPs = 0;
        this->m_lastTimeUpdated = -1000;
        this->m_skipFrames = -1;
        this->m_frameUpdated = -100;
        this->m_manualAnimControl = 0;
        this->m_castShadow = 1;
        this->m_passable = 0;
        this->m_obstacle = 0;
        this->m_imposted = 1;
        RitualInConstructor(RITUAL_REGISTERED_NODE);
    }

    SgAnimatedModelNode::SgAnimatedModelNode(SgAnimatedModelNode const&)
    {
        throw retruxx::logic_error("Not implemented");
    }
}
