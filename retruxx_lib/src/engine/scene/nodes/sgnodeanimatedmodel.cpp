#include <scene/nodes/sgnodeanimatedmodel.h>
#include <m3dapp.h>
#include <scene/servers/dataserver.h>
#include <core/ini.h>

#include "server/obstacle.h"

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
        auto result = SgNode::ReadFromXmlNodeAfterAdd(file, node);
        if (result)
        {
            SafeEnumAttrib(m_action, node, "ndmAction");
            
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
        {
            this->m_srvId = *(int*)property;
            GetServer()->UnregisterNode(this);
            GetServer()->RegisterNode(this);
            return 1;
        }
        case 0x2200u:
        {
            this->m_action = *(ActionType*)property;
            m_effectActions.resize(1);
            m_effectActions[0] = m_action;
                //TODO: check this
            GetServer()->SetItemProperty(m_srvId, 8704, this);
            return 1;
        }
        case 0x2204u:
            throw retruxx::logic_error("Not implemented");
            return 1;
        case 0x2207u:
            throw retruxx::logic_error("Not implemented");
        case 0x2208u:
            throw retruxx::logic_error("Not implemented");
        case 0x2206u:
            throw retruxx::logic_error("Not implemented");
        case 0x220Bu:
            throw retruxx::logic_error("Not implemented");
        case 0x2205u:
            throw retruxx::logic_error("Not implemented");
            return 1;
        case 0x2202u:
        {
            this->m_SkinNumber = *(int*)property;
            if (this->m_srvId != -1)
            {
                m3d::AnimatedModel* mdl = nullptr;
                GetServer()->GetItemProperty(this->m_srvId, 16394, &mdl);
                if (mdl)
                {
                    auto skin = mdl->GetNumSkins() - 1;
                    if (this->m_SkinNumber > skin)
                        this->m_SkinNumber = skin;
                }
            }
            return 1;
        }
        case 0x2203u:
            this->m_cfg.m_num = *(int*)property;
            if (this->m_srvId != -1)
            {
                m3d::AnimatedModel* mdl = nullptr;
                GetServer()->GetItemProperty(this->m_srvId, 16394, &mdl);
                if (mdl)
                {
                    auto cfgSize = mdl->GetCfgSize() - 1;
                    if (this->m_cfg.m_num > cfgSize)
                        this->m_cfg.m_num = cfgSize;
                    mdl->FromCfgNum(m_cfg);
                    mdl->CalculateMeshes(m_cfg);
                    return 1;
                }
            }
            return 0;
        case 0x220Au:
            throw retruxx::logic_error("Not implemented");
            return 1;
        case 0x220Cu:
            throw retruxx::logic_error("Not implemented");
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
        if (!this->m_passable || m_obstacle)
        {
            if (m_obstacle)
            {
                m_obstacle->DecRef();
                m_obstacle = nullptr;
            }
            return 1;
        }
        m_obstacle = new ai::Obstacle(this);
        m_obstacle->IncRef();
        return 1;
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
        return RT_CLASS_LOCAL(SgAnimatedModelNode);
    }

    SgAnimatedModelNode::~SgAnimatedModelNode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgAnimatedModelNode::UpdateOwnBoundingBox()
    {
        if (this->m_srvId == -1)
        {
            this->m_ownBoundingBox.m_box[0] = 0.0;
            this->m_ownBoundingBox.m_box[1] = 0.0;
            this->m_ownBoundingBox.m_box[2] = 0.0;
            this->m_ownBoundingBox.m_box[3] = 0.0;
            this->m_ownBoundingBox.m_box[4] = 0.0;
            this->m_ownBoundingBox.m_box[5] = 0.0;
        }
        else
        {
            m3d::PropSrvBoundingBox prop;
            prop.m_node = this;
            prop.m_destBox = &m_ownBoundingBox;
            GetServer()->GetItemProperty(m_srvId, 12288, &prop);
        }
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
