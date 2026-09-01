#include "cinematicpath.h"

#include <cmath>

#include <config.h>
#include <m3dapp.h>
#include <core/ini.h>
#include <core/kernel.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <math/vector.h>

namespace ai
{
    void CinematicPath::LoadFromXml(
        CStr const& debugDescription,
        m3d::cmn::XmlFile* xmlFile,
        m3d::cmn::XmlNode const* xmlNode)
    {
        if (xmlNode->IsEmpty())
        {
            return;
        }

        ref_ptr pointNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        for (xmlNode->GetFirstChild(pointNode, "Point"); !pointNode->IsEmpty();
             pointNode->GetNextSibling(pointNode, "Point"))
        {
            CVector2 pos;
            if (!m3d::SafeVector2Attrib(pos, pointNode, "Pos"))
            {
                M3D_LOG_ERR("Error reading vehicle path for " + debugDescription);
                continue;
            }

            // Skip a point that is within 0.1 of the previous one.
            if (!m_vehiclePoints.empty())
            {
                CVector2 const& last = m_vehiclePoints.back();
                float const dx = last.x - pos.x;
                float const dy = last.y - pos.y;
                if (std::sqrt(dx * dx + dy * dy) <= 0.1)
                {
                    continue;
                }
            }
            m_vehiclePoints.push_back(pos);
        }

        ref_ptr cameraNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        for (xmlNode->GetFirstChild(cameraNode, "CameraPoint"); !cameraNode->IsEmpty();
             cameraNode->GetNextSibling(cameraNode, "CameraPoint"))
        {
            CVector pos;
            if (m3d::SafeVectorAttrib(pos, cameraNode, "Pos"))
            {
                m_cameraPoints.push_back(pos);
            }
            else
            {
                M3D_LOG_ERR("Error reading camera path for " + debugDescription);
            }
        }
    }

    void CinematicPath::SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        for (auto const& point : m_vehiclePoints)
        {
            ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Point");
            node->SetAttribute("Pos", CStr::format_("%.3f %.3f", point.x, point.y).c_str());
            xmlNode->AddChild(node);
        }

        for (auto const& point : m_cameraPoints)
        {
            ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "CameraPoint");
            node->SetAttribute("Pos", CStr(point).c_str());
            xmlNode->AddChild(node);
        }
    }

    void CinematicPath::clear()
    {
        m_cameraPoints.clear();
        m_vehiclePoints.clear();
    }

    std::vector<CVector2, std::allocator<CVector2>> const& CinematicPath::GetVehiclePoints() const
    {
        return m_vehiclePoints;
    }

    std::vector<CVector, std::allocator<CVector>> const& CinematicPath::GetCameraPoints() const
    {
        return m_cameraPoints;
    }

    void CinematicPath::SetVehiclePoints(std::vector<CVector2, std::allocator<CVector2>> const& vehiclePoints)
    {
        m_vehiclePoints = vehiclePoints;
    }

    void CinematicPath::SetCameraPoints(std::vector<CVector, std::allocator<CVector>> const& cameraPoints)
    {
        m_cameraPoints = cameraPoints;
    }

    void CinematicPath::ShiftCameraPoints(CVector const& shiftPos)
    {
        for (auto& point : m_cameraPoints)
        {
            point.x += shiftPos.x;
            point.y += shiftPos.y;
            point.z += shiftPos.z;
        }
    }

    void CinematicPath::RenderDebugInfo() const
    {
        if (!m_vehiclePoints.empty())
        {
            CVector prevPoint;
            prevPoint.x = m_vehiclePoints[0].x;
            prevPoint.y = 0.0f;
            prevPoint.z = m_vehiclePoints[0].y;

            unsigned int color = 0xFFFF0000;  // red, +1285 per point
            for (auto const& vehiclePoint : m_vehiclePoints)
            {
                CVector point;
                point.x = vehiclePoint.x;
                point.y = 0.0f;
                point.z = vehiclePoint.y;
                point.y = M3D_ENGINE_CFG.GetHeight(point.x, point.z) + 10.0f;

                M3D_APP->DrawCross(point, 2.0f, color);
                M3D_APP->DrawLine(prevPoint, point, 0xFF00FFFF);
                prevPoint = point;
                color += 1285;
            }
        }

        if (!m_cameraPoints.empty())
        {
            CVector prevPoint = m_cameraPoints[0];

            unsigned int color = 0xFFFFFF00;  // yellow, +1285 per point
            for (auto const& point : m_cameraPoints)
            {
                M3D_APP->DrawCross(point, 2.0f, color);
                M3D_APP->DrawLine(prevPoint, point, 0xFF00FFFF);
                prevPoint = point;
                color += 1285;
            }
        }
    }
}  // namespace ai
