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
        char constexpr* const RDL_NAMES[] = {"FwdZLink", "BackZLink", "FwdXLink", "BackXLink"};

        // Where `back` meets `fwd`: its own origin when they join end-to-end,
        // otherwise the midpoint of the side the junction presents to it.
        void GetFwdLink(RoadNode* back, RoadNode* fwd, CVector& p)
        {
            if (back->m_linkedNodes[0] == fwd)
            {
                p = back->m_origin;
                return;
            }
            float xComponent = 0.0f;
            if (back->m_linkedNodes[3] == fwd)
            {
                xComponent = back->m_maxX;
            }
            else if (back->m_linkedNodes[2] == fwd)
            {
                xComponent = back->m_minX;
            }
            else
            {
                return;
            }
            p = back->GetLinkPoint(0.5f, xComponent);
        }

        void GetBackLink(RoadNode* fwd, RoadNode* back, CVector& p)
        {
            if (fwd->m_linkedNodes[1] == back)
            {
                p = fwd->m_origin;
                return;
            }
            float xComponent = 0.0f;
            if (fwd->m_linkedNodes[3] == back)
            {
                xComponent = fwd->m_maxX;
            }
            else if (fwd->m_linkedNodes[2] == back)
            {
                xComponent = fwd->m_minX;
            }
            else
            {
                return;
            }
            p = fwd->GetLinkPoint(0.5f, xComponent);
        }
    }  // namespace

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
        // These fields are slices of a shared pool buffer, so only the slice may
        // be handed back - releasing Vb/Ib would destroy the whole pool out from
        // under every other mesh using it.
        if (m_VbPoolField.Vb.IsValid())
        {
            M3D_RENDERER->ReleaseVbPoolField(m_VbPoolField);
        }
        if (m_IbPoolField.Ib.IsValid())
        {
            M3D_RENDERER->ReleaseIbPoolField(m_IbPoolField);
        }
    }

    int RoadNode::GetSoilType()
    {
        return m_owner->m_roadSets[m_roadSetHandle]->m_soilType;
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

                auto const modelHeight = model->m_box.m_box[4] - model->m_box.m_box[1];
                m_origin.y =
                    pClient->GetWorld().GetLandscape().GetLsHeight(m_origin.x, m_origin.y) - (modelHeight * 0.5);
            }
        }
        else
        {
            m_origin.y = 0.0;
        }
        return 1;
    }

    void RoadNode::SetOwner(RoadManager* owner)
    {
        m_owner = owner;
    }

    CVector RoadNode::GetLinkPoint(float t, float xComponent)
    {
        // Catmull-Rom through the four control points, then offset sideways.
        CVector const p1 = GetPoint1();
        CVector const p2 = m_origin;
        CVector const p3 = GetPoint3();
        CVector const p4 = GetPoint4();

        auto spline = [&p1, &p2, &p3, &p4](float u)
        {
            float const c1 = ((2.0f - u) * u) * u - u;
            float const c2 = ((u * u) * u) * 3.0f - (u * u) * 5.0f + 2.0f;
            float const c3 = (((4.0f - u * 3.0f) * u) + 1.0f) * u;
            float const c4 = ((u * u) * u) - (u * u);

            CVector out;
            out.x = (p1.x * c1 + p2.x * c2 + p3.x * c3 + p4.x * c4) * 0.5f;
            out.y = (p1.y * c1 + p2.y * c2 + p3.y * c3 + p4.y * c4) * 0.5f;
            out.z = (p1.z * c1 + p2.z * c2 + p3.z * c3 + p4.z * c4) * 0.5f;
            return out;
        };

        CVector const at = spline(t);
        CVector const ahead = spline(t + 0.0099999998f);

        float const dx = ahead.x - at.x;
        float const dy = ahead.y - at.y;
        float const dz = ahead.z - at.z;
        float const invLen = 1.0f / sqrtf(dx * dx + dy * dy + dz * dz + 0.00000011920929f);
        float const tx = dx * invLen;
        float const tz = dz * invLen;

        // Tangent crossed with the up axis; which way depends on the sign.
        CVector perp;
        perp.y = 0.0f;
        if (xComponent <= 0.0f)
        {
            perp.x = -tz;
            perp.z = tx;
        }
        else
        {
            perp.x = tz;
            perp.z = -tx;
        }

        float const scale = fabsf(xComponent) * 1.05f;

        CVector result;
        result.x = at.x + perp.x * scale;
        result.y = at.y + perp.y * scale;
        result.z = at.z + perp.z * scale;
        return result;
    }

    CVector RoadNode::GetPoint1()
    {
        CVector p = m_origin;
        if (m_linkedNodes[1])
        {
            GetFwdLink(m_linkedNodes[1], this, p);
        }
        else if (m_friend)
        {
            if (m_friend->m_linkedNodes[1])
            {
                GetFwdLink(m_friend->m_linkedNodes[1], m_friend, p);
            }
            else if (m_friend->m_linkedNodes[0])
            {
                GetBackLink(m_friend->m_linkedNodes[0], m_friend, p);
            }
        }
        return p;
    }

    CVector RoadNode::GetPoint2()
    {
        return m_origin;
    }

    Object* RoadNode::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector RoadNode::GetPoint3()
    {
        CVector p = m_origin;
        if (m_linkedNodes[0])
        {
            GetBackLink(m_linkedNodes[0], this, p);
        }
        else if (m_friend)
        {
            if (m_friend->m_linkedNodes[1])
            {
                GetFwdLink(m_friend->m_linkedNodes[1], m_friend, p);
            }
            else if (m_friend->m_linkedNodes[0])
            {
                GetBackLink(m_friend->m_linkedNodes[0], m_friend, p);
            }
        }
        return p;
    }

    CVector RoadNode::GetPoint4()
    {
        CVector p = m_origin;
        RoadNode* next = m_linkedNodes[0];
        if (next)
        {
            if (next->m_linkedNodes[1] == this)
            {
                p = next->GetPoint3();
            }
            else if (next->m_linkedNodes[3] == this)
            {
                p = next->GetLinkPoint(0.5f, next->m_minX);
            }
            else if (next->m_linkedNodes[2] == this)
            {
                p = next->GetLinkPoint(0.5f, next->m_maxX);
            }
        }
        return p;
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
