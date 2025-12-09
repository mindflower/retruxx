#include "m3dapp.h"
#include "road.h"
#include "skelmodel.h"
#include "world.h"
#include "core/ini.h"
#include <client.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(RoadNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(RoadNode);

    namespace
    {
        constexpr char* const RDL_NAMES[] = {"FwdZLink", "BackZLink", "FwdXLink", "BackXLink"};
    }

    int RoadNode::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Class* RoadNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    int RoadNode::ReadFromXmlNodeAfterAdd(cmn::XmlFile*, cmn::XmlNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RoadNode::~RoadNode()
    {
        if (m_VbPoolField.Vb.IsValid())
        {
            M3D_RENDERER->ReleaseVb(m_VbPoolField.Vb);
        }
        if (m_IbPoolField.Ib.IsValid())
        {
            M3D_RENDERER->ReleaseIb(m_IbPoolField.Ib);
        }
    }

    int RoadNode::GetSoilType()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int RoadNode::ReadFromXmlNode(cmn::XmlFile* file, cmn::XmlNode* node)
    {
        for (int i = 0; i < 4; ++i)
        {
            SafeStrAttrib(m_linkedNames[i], node, RDL_NAMES[i]);
        }

        SafeStrAttrib(m_name, node, "name");
        SafeStrAttrib(m_roadSetName, node, "roadset");
        SafeUintAttrib(m_skinNumber, node, "skinNumber");
        SafeVectorAttrib(m_origin, node, "org");
        SafeBoolAttrib(m_asCliff, node, "AsCliff");
        SafeUintAttrib(m_modelNum, node, "ModelNum");

        if (m_asCliff)
        {
            if (fabs(m_origin.y) < 0.1)
            {
                auto& roadManager = m3d::pClient->GetWorld().GetRoadManager();
                auto* roadSet = roadManager.m_roadSets[roadManager.GetRoadSetHandleByName(m_roadSetName)];
                auto* model = roadSet->m_roadModels[0][m_modelNum];

                const auto modelHeight = model->m_box.m_box[4] - model->m_box.m_box[1];
                m_origin.y = pClient->GetWorld().GetLandscape().GetLsHeight(m_origin.x, m_origin.y) - (modelHeight * 0.5);
            }
        }
        else
        {
            m_origin.y = 0.0;
        }
        return 1;
    }

    void RoadNode::SetOwner(RoadManager*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector RoadNode::GetLinkPoint(float, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector RoadNode::GetPoint1()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector RoadNode::GetPoint2()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Object* RoadNode::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector RoadNode::GetPoint3()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector RoadNode::GetPoint4()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Class* RoadNode::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Object* RoadNode::CreateObject()
    {
        return new RoadNode;
    }

    RoadNode::RoadNode(RoadNode const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RoadNode::RoadNode()
    {
        this->minb.x = 0.0;
        this->minb.y = 0.0;
        this->minb.z = 0.0;
        this->maxb.x = 0.0;
        this->maxb.y = 0.0;
        this->maxb.z = 0.0;
        this->m_frameVisible = -1;
        this->m_linkedNodes[0] = 0;
        this->m_linkedNodes[1] = 0;
        this->m_linkedNodes[2] = 0;
        this->m_linkedNodes[3] = 0;
        this->m_bRoadDrawn = 0;
        this->m_geomObject = 0;
        this->m_type = 0;
        this->m_modelNum = 0;
        this->m_skinNumber = 0;
        this->m_friend = 0;
        this->m_asCliff = 0;
        this->m_boundCenter = CVector(0.0, 0.0, 0.0);
        this->m_boundRadius = 0.0;
        this->m_cachedVertices = 0;
    }
}  // namespace m3d
