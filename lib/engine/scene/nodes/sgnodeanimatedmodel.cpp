#include <scene/nodes/sgnodeanimatedmodel.h>
#include <m3dapp.h>
#include <scene/servers/dataserver.h>
#include <core/ini.h>

#include "core/kernel.h"
#include "core/timer.h"
#include "server/obstacle.h"
#include <algorithm>
#include <client.h>
#include <world.h>
#include <landscape.h>
#include <skelmodel.h>
#include <config.h>
#include <scene/scenegraph.h>
#include <scene/servers/serveranimatedmodel.h>

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
        return new SgAnimatedModelNode(*this);
    }

    int SgAnimatedModelNode::Think(int dt, int curTime)
    {
        if (m_srvId == -1)
        {
            return 0;
        }

        AnimInfo* anim = nullptr;
        GetProperty(1u, &anim);
        if (anim->IsEmpty())
        {
            return 1;
        }

        const auto curFrame = M3D_KERNEL->GetTimer().GetCurFrame();
        if (m_frameVisible != curFrame - 1)
        {
            return 0;
        }

        auto onScreenSize = M3D_APP->GetOnScreenSize(m_originWorldAbsForSphere, m_boundingRadius);
        if (onScreenSize <= 16.0)
        {
            if (onScreenSize <= 8.0)
            {
                if (onScreenSize <= 4.0)
                    m_skipFrames = 10;
                else
                    m_skipFrames = 4;
            }
            else
            {
                m_skipFrames = 2;
            }
        }
        else
        {
            m_skipFrames = 1;
        }

        if (m_frameUpdated != curFrame && m_frameUpdated + m_skipFrames <= curFrame)
        {
            auto actualDt = 0.0f;
            if (m_lastTimeUpdated < 0)
                actualDt = dt;
            else
                actualDt = curTime - m_lastTimeUpdated;

            struct RenderInfo
            {
                /* 0x0000 */ m3d::SgNode* m_node = nullptr;
                /* 0x0004 */ unsigned int m_dt = 0;
                /* 0x0008 */ unsigned int m_fps = 0;
            }; /* size: 0x000c */

            RenderInfo ri;
            ri.m_node = this;
            ri.m_dt = actualDt;
            if (actualDt)
            {
                GetServer()->UpdateItem(m_srvId, &ri);
            }
            m_lastTimeUpdated = curTime;
            m_frameUpdated = curFrame;
        }
        return 1;
    }

    namespace
    {
        // What the node hands to AnimatedModelsServer::RenderItem (SgAnimatedModelNode::Render::RenderInfo in the PDB).
        struct RenderInfo
        {
            /* 0x0000 */ CMatrix* m_localXForm;
            /* 0x0004 */ float m_alpha;
            /* 0x0008 */ m3d::SgNode* m_node;
        }; /* size: 0x000c */
    }  // namespace

    int SgAnimatedModelNode::WriteToXmlNode(cmn::XmlFile* file, cmn::XmlNode* writeTo)
    {
        // RVA 0x667060 - only the settings that differ from the model's defaults are written, the action aside.
        if (!SgNode::WriteToXmlNode(file, writeTo))
        {
            return 0;
        }

        int action = 0;
        GetProperty(PROP_DM_ACTION, &action);
        writeTo->SetAttribute("ndmAction", CStr(action).c_str());

        AnimatedModel* mdl = nullptr;
        GetServer()->GetItemProperty(m_srvId, PROP_INTERNAL_GETMODEL, &mdl);
        if (!mdl || mdl->bIsPassable() != m_passable)
        {
            // m3d::XmlNodeSetAttribute<bool>
            writeTo->SetAttribute("passable", CStr(static_cast<int>(m_passable)).c_str());
        }
        if (m_SkinNumber)
        {
            writeTo->SetAttribute("skin", CStr(m_SkinNumber).c_str());
        }
        if (m_cfg.m_num)
        {
            writeTo->SetAttribute("cfg", CStr(m_cfg.m_num).c_str());
        }
        if (!m_castShadow)
        {
            writeTo->SetAttribute("CastShadow", "no");
        }
        return 1;
    }

    int SgAnimatedModelNode::Render(SgNodeRenderFlags flags, void* data, int, int)
    {
        // RVA 0x72FC20 - with the fog of war on, models of other belongs standing on unexplored ground are skipped.
        if (m_srvId == -1)
        {
            return 0;
        }
        if ((flags & 4) == 0)
        {
            GetGraph()->LightSetupLightsForNode(this);
            M3D_RENDERER->SetCull(rend::M3DCULL_CCW, false);
        }

        CMatrix temp = MatrixFromFlags(flags, data);
        RenderInfo ri;
        ri.m_localXForm = &temp;
        ri.m_node = this;

        float const x = m_currentWorldOrigin.x;
        float const y = m_currentWorldOrigin.y;
        if (M3D_ENGINE_CFG.m_FogOfWar.GetB())
        {
            unsigned char const explored = pClient->GetWorld().GetLandscape().GetColor(x, y);
            int belong = 1000;
            GetProperty(PROP_NODE_BELONG, &belong);
            if (belong != 1000 && !explored)
            {
                return 1;
            }
        }
        ri.m_alpha = GetGraph()->GetAlphaForNode(this);
        GetServer()->RenderItem(m_srvId, &ri);
        return 1;
    }

    int SgAnimatedModelNode::SetProperty(unsigned propId, void* property)
    {
        if (SgNode::SetProperty(propId, property))
            return 1;

        switch (propId)
        {
        case 4360u:
        {
            this->m_srvId = *(int*)property;
            GetServer()->UnregisterNode(this);
            GetServer()->RegisterNode(this);
            return 1;
        }
        case 8704u:
        {
            // RVA 0x730810 - the action drives both the model's animation and its effects.
            PropSrvNodeAction ri;
            ri.m_node = this;
            ri.m_action = *(ActionType*)property;
            this->m_action = ri.m_action;
            m_effectActions.resize(1u, AT_STAND1);
            m_effectActions.front() = ri.m_action;
            GetServer()->SetItemProperty(m_srvId, 8704, &ri);
            return 1;
        }
        case 8708u:
        {
            // NOTE: only the effects change; m_action keeps the model's own action.
            PropSrvNodeAction ri;
            ri.m_node = this;
            ri.m_action = *(ActionType*)property;
            m_effectActions.resize(1u, AT_STAND1);
            m_effectActions.front() = ri.m_action;
            GetServer()->SetItemProperty(this->m_srvId, 8708, &ri);
            return 1;
        }
        case 8711u:
        {
            // RVA 0x730810 - an action already in the list is not added twice.
            ActionType const action = *(ActionType*)property;
            if (std::find(m_effectActions.begin(), m_effectActions.end(), action) == m_effectActions.end())
            {
                m_effectActions.push_back(action);
                PropSrvNodeActions ri;
                ri.m_node = this;
                ri.m_Actions = &m_effectActions;
                GetServer()->SetItemProperty(this->m_srvId, 8710, &ri);
            }
            return 1;
        }
        case 8712u:
        {
            ActionType const action = *(ActionType*)property;
            auto const it = std::find(m_effectActions.begin(), m_effectActions.end(), action);
            if (it != m_effectActions.end())
            {
                m_effectActions.erase(it);
                PropSrvNodeActions ri;
                ri.m_node = this;
                ri.m_Actions = &m_effectActions;
                GetServer()->SetItemProperty(this->m_srvId, 8710, &ri);
            }
            return 1;
        }
        case 8710:
        {
            m_effectActions = *(decltype(m_effectActions)*)property;
            PropSrvNodeActions ri;
            ri.m_node = this;
            ri.m_Actions = &m_effectActions;
            GetServer()->SetItemProperty(this->m_srvId, 8710, &ri);
            return 1;
        }
        case 8715u:
        {
            // Restarts the current effects by handing the same list over again.
            PropSrvNodeActions ri;
            ri.m_node = this;
            ri.m_Actions = &m_effectActions;
            GetServer()->SetItemProperty(m_srvId, 8710, &ri);
            return 1;
        }
        case 8709u:
        {
            PropSrvNodeAction ri;
            ri.m_node = this;
            ri.m_action = *(ActionType*)property;
            this->m_action = ri.m_action;
            GetServer()->SetItemProperty(this->m_srvId, 8709, &ri);
            return 1;
        }
        case 8706u:
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
            m_suppressedLPs = (decltype(m_suppressedLPs))property;
            return 1;

        case 8716:
            this->m_manualAnimControl = *(int*)property != 0;
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

        m_passable = *(bool*)property;
        if (m_passable || m_obstacle)
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

    int SgAnimatedModelNode::GetPropertiesList(retruxx::set<unsigned>& props) const
    {
        // RVA 0x730250
        if (!SgNode::GetPropertiesList(props))
        {
            return 0;
        }
        props.insert(PROP_NODE_HANDLE);
        props.insert(PROP_DM_ACTION);
        props.insert(PROP_DM_EFFECT_ACTION);
        props.insert(PROP_DM_MODEL_ACTION);
        props.insert(PROP_DM_SKIN);
        props.insert(PROP_DM_CFG);
        props.insert(PROP_DM_PASSABLE);
        props.insert(PROP_DM_IS_IMPOSTED);
        props.insert(PROP_DM_CAST_SHADOW);
        return 1;
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
        if (m_obstacle)
        {
            m_obstacle->UnlinkFromOwner();
            m_obstacle->DecRef();
        }

        RitualInDestructor();
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

    SgAnimatedModelNode::SgAnimatedModelNode(SgAnimatedModelNode const& node) : SgNode(node)
    {
        m_action = node.m_action;
        m_effectActions = node.m_effectActions;
        m_SkinNumber = node.m_SkinNumber;

        m_cfg.m_num = node.m_cfg.m_num;
        m_cfg.m_meshes = node.m_cfg.m_meshes;
        m_cfg.m_groupVariants = node.m_cfg.m_groupVariants;

        m_suppressedLPs = node.m_suppressedLPs;
        m_lastTimeUpdated = -1000;
        m_skipFrames = -1;
        m_frameUpdated = -100;
        m_manualAnimControl = node.m_manualAnimControl;
        m_castShadow = node.m_castShadow;
        m_passable = node.m_passable;
        m_obstacle = nullptr;
        m_imposted = true;

        RitualInConstructor(RITUAL_REGISTERED_NODE);
    }
}
