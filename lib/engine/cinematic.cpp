#include <cinematic.h>
#include <stdexcept>

#include "config.h"
#include "core/ini.h"
#include "core/kernel.h"
#include "core/log.h"
#include "core/ref_ptr.h"
#include "core/scoped_ptr.h"
#include "file/fileserver.h"
#include "file/filestream.h"
#include "landscape.h"
#include "m3dapp.h"
#include "math/coremath.h"
#include "math/matrix.h"
#include <client.h>

#include "world.h"
#include <server/objects/base/objcontainer.h>
#include <server/objects/base/physicobj.h>
#include <server/utils.h>
#include <renderer/i_renderer.h>

#include <cmath>

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, StartCinematic)
{
    auto cinematic = (m3d::Cinematic*)context->asObject(0, "Cinematic");
    cinematic->StartCinematic();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, Play)
{
    auto cinematic = (m3d::Cinematic*)context->asObject(0, "Cinematic");
    auto time = context->asFloat(1);
    cinematic->Play(time);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, PlayFromPoint)
{
    // RVA 0x62C060 - NOTE: the play time and the point number are both read from argument 1.
    auto cinematic = (m3d::Cinematic*)context->asObject(0, "Cinematic");
    auto pointNum = context->asInt(1);
    auto playTime = context->asFloat(1);
    cinematic->PlayFromPoint(playTime, pointNum);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, Stop)
{
    // RVA 0x626360
    auto cinematic = (m3d::Cinematic*)context->asObject(0, "Cinematic");
    cinematic->Stop();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, InPlay)
{
    // RVA 0x625910
    auto cinematic = (m3d::Cinematic*)context->asObject(0, "Cinematic");
    context->pushBool(cinematic->InPlay());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, SetPath)
{
    auto cinematic = (m3d::Cinematic*)context->asObject(0, "Cinematic");
    auto path = context->asString(1);
    cinematic->SetPath(path);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, SetPathFromPos)
{
    // RVA 0x62A880
    auto cinematic = (m3d::Cinematic*)context->asObject(0, "Cinematic");
    auto pathName = context->asString(3);
    auto const& rotation = context->asQuaternion(2);
    auto const& pos = context->asVector(1);
    cinematic->SetPathFromPos(pos, rotation, pathName);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, LoadDefaults)
{
    // RVA 0x62CAA0
    auto cinematic = (m3d::Cinematic*)context->asObject(0, "Cinematic");
    cinematic->LoadDefaults();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, SetAim)
{
    // RVA 0x625940
    auto cinematic = (m3d::Cinematic*)context->asObject(0, "Cinematic");
    cinematic->SetAim(context->asVector(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, SetAimToID)
{
    auto cinematic = (m3d::Cinematic*)context->asObject(0, "Cinematic");
    auto id = context->asInt(1);
    cinematic->SetAimToID(id);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, SetRelativePoints)
{
    auto cinematic = (m3d::Cinematic*)context->asObject(0, "Cinematic");
    auto val = context->asBool(1);
    cinematic->SetRelativePoints(val);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, SetRelativeRotations)
{
    auto cinematic = (m3d::Cinematic*)context->asObject(0, "Cinematic");
    auto val = context->asBool(1);
    cinematic->SetRelativeRotations(val);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, SetBaseToId)
{
    auto cinematic = (m3d::Cinematic*)context->asObject(0, "Cinematic");
    auto id = context->asInt(1);
    cinematic->SetBaseToId(id);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, SetLookTo)
{
    auto cinematic = (m3d::Cinematic*)context->asObject(0, "Cinematic");
    auto value = context->asBool(1);
    cinematic->SetLookTo(value);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, SetWaitWhenStop)
{
    auto cinematic = (m3d::Cinematic*)context->asObject(0, "Cinematic");
    auto value = context->asBool(1);
    cinematic->SetWaitWhenStop(value);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, SetLerpFromPreviousItem)
{
    auto cinematic = (m3d::Cinematic*)context->asObject(0, "Cinematic");
    auto value = context->asBool(1);
    cinematic->SetLerpFromPreviousItem(value);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, SetFolder)
{
    // RVA 0x625B10
    auto cinematic = (m3d::Cinematic*)context->asObject(0, "Cinematic");
    cinematic->SetFolder(context->asString(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, GetFolder)
{
    // RVA 0x625AE0
    auto cinematic = (m3d::Cinematic*)context->asObject(0, "Cinematic");
    context->pushString(cinematic->GetFolder());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, Load)
{
    auto cinematic = (m3d::Cinematic*)context->asObject(0, "Cinematic");
    auto fileName = context->asString(1);
    auto res = cinematic->Load(fileName);
    context->pushBool(res);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, FlyAround)
{
    // RVA 0x62C0A0 - arguments: phi, theta, radius, play time, current position, fly path name.
    auto cinematic = (m3d::Cinematic*)context->asObject(0, "Cinematic");
    auto flyPathName = context->asString(6);
    auto const& curPos = context->asVector(5);
    auto playTime = context->asFloat(4);
    auto radius = context->asFloat(3);
    auto theta = context->asFloat(2);
    auto phi = context->asFloat(1);
    cinematic->FlyAround(phi, theta, radius, playTime, curPos, flyPathName);
    return 1;
}

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(Cinematic)
    RT_CLASS_EXPORT(Cinematic, METHOD, StartCinematic, "void", "", "")
    RT_CLASS_EXPORT(Cinematic, METHOD, Play, "void", "float playTime", "")
    RT_CLASS_EXPORT(Cinematic, METHOD, PlayFromPoint, "void", "float playTime, int PointNumber", "")
    RT_CLASS_EXPORT(Cinematic, METHOD, Stop, "", "", "")
    RT_CLASS_EXPORT(Cinematic, METHOD, InPlay, "", "", "")
    RT_CLASS_EXPORT(Cinematic, METHOD, SetPath, "", "", "")
    RT_CLASS_EXPORT(Cinematic, METHOD, SetPathFromPos, "", "", "")
    RT_CLASS_EXPORT(Cinematic, METHOD, LoadDefaults, "", "", "")
    RT_CLASS_EXPORT(Cinematic, METHOD, SetAim, "", "", "")
    RT_CLASS_EXPORT(Cinematic, METHOD, SetAimToID, "", "", "")
    RT_CLASS_EXPORT(Cinematic, METHOD, SetRelativePoints, "", "", "")
    RT_CLASS_EXPORT(Cinematic, METHOD, SetRelativeRotations, "", "", "")
    RT_CLASS_EXPORT(Cinematic, METHOD, SetBaseToId, "", "", "")
    RT_CLASS_EXPORT(Cinematic, METHOD, SetLookTo, "", "", "")
    RT_CLASS_EXPORT(Cinematic, METHOD, SetWaitWhenStop, "", "", "")
    RT_CLASS_EXPORT(Cinematic, METHOD, SetLerpFromPreviousItem, "", "", "")
    RT_CLASS_EXPORT(Cinematic, METHOD, SetFolder, "", "", "")
    RT_CLASS_EXPORT(Cinematic, METHOD, GetFolder, "", "", "")
    RT_CLASS_EXPORT(Cinematic, METHOD, Load, "", "", "")
    RT_CLASS_EXPORT(Cinematic, METHOD, FlyAround, "", "", "")
    RT_CLASS_EXPORTS_END;

    RT_CLASS_DEFINE(Cinematic);
}  // namespace m3d

namespace m3d
{
    CameraPath const CameraPath::m_emptyPath;

    void CameraPath::insert(int pointNum, CameraPathState const& state)
    {
        // RVA 0x62ADB0 - the guard points are dropped while editing and rebuilt afterwards.
        _DeFix();
        if (pointNum < 0 || pointNum > static_cast<int>(m_cameraPathStates.size()))
        {
            M3D_ENGINE_CFG.m_console->PrintF(CStr("Error: invalid point number"));
        }
        else
        {
            m_cameraPathStates.insert(m_cameraPathStates.begin() + pointNum, state);
        }
        _Fix();
    }

    unsigned CameraPath::size() const
    {
        return m_cameraPathStates.size();
    }

    void CameraPath::GetCameraForTime(float curTime, CVector& pos, Quaternion& rot, float& zoom) const
    {
        // TODO: generated code
        // Find the segment that contains the current time
        int segmentIndex = 1;
        for (; segmentIndex < this->m_cameraPathStates.size(); ++segmentIndex)
        {
            if (curTime <= this->m_cameraPathStates[segmentIndex].m_flyTime)
                break;
        }

        // Handle case where time is beyond the last keyframe
        if (segmentIndex == this->m_cameraPathStates.size())
        {
            pos = this->m_cameraPathStates.back().m_point;
            rot = this->m_cameraPathStates.back().m_rotation;
            zoom = this->m_cameraPathStates.back().m_zoom;
            return;
        }
        // Adjust segment index to ensure we have enough points for interpolation
        if (segmentIndex > 1)
            segmentIndex--;

        // Ensure we have valid control points for Catmull-Rom interpolation
        while (segmentIndex > 0)
        {
            if (segmentIndex + 2 < this->m_cameraPathStates.size())
                break;
            segmentIndex--;
        }

        // Get the four control points for Catmull-Rom interpolation
        auto const& p0 = this->m_cameraPathStates[segmentIndex - 1];
        auto const& p1 = this->m_cameraPathStates[segmentIndex];
        auto const& p2 = this->m_cameraPathStates[segmentIndex + 1];
        auto const& p3 = this->m_cameraPathStates[segmentIndex + 2];

        // Calculate normalized time within the segment [0,1]
        float segmentStartTime = p1.m_flyTime;
        float segmentEndTime = p2.m_flyTime;
        float normalizedTime = (curTime - segmentStartTime) / (segmentEndTime - segmentStartTime);

        // Calculate speed-based interpolation parameter
        float speedFactor = ((p2.m_speed - p1.m_speed) * normalizedTime + (p1.m_speed * 2.0f)) * normalizedTime;
        speedFactor /= (p1.m_speed + p2.m_speed);

        // Check if points are too close (degenerate case)
        float distance = (p1.m_point - p2.m_point).length();
        if (distance <= 0.001f)
        {
            // Use exact point if too close
            pos = p1.m_point;
            zoom = p1.m_zoom;
        }
        else
        {
            // Interpolate position using Catmull-Rom spline
            pos = CatmullRomSubdivide(speedFactor, p0.m_point, p1.m_point, p2.m_point, p3.m_point);

            // Interpolate zoom using Catmull-Rom spline
            zoom = CatmullRomSubdivide(speedFactor, p0.m_zoom, p1.m_zoom, p2.m_zoom, p3.m_zoom);
        }

        // Interpolate rotation using cubic interpolation
        rot = CubicInterpolation(speedFactor, p0.m_rotation, p1.m_rotation, p2.m_rotation, p3.m_rotation);
    }

    bool CameraPath::empty() const
    {
        return m_cameraPathStates.size();
    }

    void CameraPath::CalcFlyTimes(unsigned pointNum, bool recalcFullLength)
    {
        M3D_ASSERT(pointNum >= 1);
        if (recalcFullLength)
        {
            CalcFullLength(pointNum);
        }

        // TODO: generated code

        // Get the camera path states
        auto& states = this->m_cameraPathStates;
        if (states.empty())
            return;

        // Reset fly times for the specified number of points
        if (pointNum < 4)
        {
            for (int i = 0; i < pointNum && i < states.size(); ++i)
            {
                states[i].m_flyTime = 0.0f;
            }
        }
        else
        {
            // For larger point counts, distribute times evenly
            // Set fly times for intermediate points using linear interpolation
            int startIndex = pointNum;
            for (unsigned int i = 0; startIndex < (states.size() - 2); ++i)
            {
                states[startIndex].m_flyTime = (i * this->m_fullTime) / (states.size() - pointNum - 2);
                ++startIndex;
            }
        }

        // Set fly times for the last two points
        states.back().m_flyTime = this->m_fullTime;
        if (states.size() >= 2)
        {
            states[states.size() - 2].m_flyTime = this->m_fullTime;
        }

        // Validate minimum state count
        M3D_ASSERT(empty() || size() >= 4);

        // Calculate average speed for time adjustment
        float totalSpeed = 0.0f;
        int speedCount = 0;

        // Sum speeds of intermediate points (excluding first and last two points)
        for (int i = 1; i < (states.size() - 2); ++i)
        {
            totalSpeed += states[i].m_speed;
            ++speedCount;
        }

        if (speedCount > 0)
        {
            float averageSpeed = totalSpeed / speedCount;
            float previousFlyTime = states[1].m_flyTime;

            // Adjust fly times based on speed variations
            for (int i = 2; i < (states.size() - 2); ++i)
            {
                float currentFlyTime = states[i].m_flyTime;
                float segmentSpeed = (states[i].m_speed + states[i - 1].m_speed) * 0.5f;

                // Adjust time based on speed ratio
                states[i].m_flyTime =
                    states[i - 1].m_flyTime + ((currentFlyTime - previousFlyTime) * averageSpeed / segmentSpeed) * 2.0f;

                previousFlyTime = currentFlyTime;
            }
        }
    }

    void CameraPath::push_back(CameraPathState const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CameraPath::MovePoint(int, CameraPathState const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CameraPath::CalcFullLength(unsigned pointNum)
    {
        M3D_ASSERT(pointNum >= 1);
        m_fullLength = 0.0;
        for (; pointNum < m_cameraPathStates.size(); ++pointNum)
        {
            m_fullLength += _CalcSplineSegmentLength(pointNum - 1, pointNum);
        }
    }

    float CameraPath::GetFullLength() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CameraPath::SaveToXmlRuntime(cmn::XmlFile* xmlFile, cmn::XmlNode* xmlNode) const
    {
        // RVA 0x6268A0 - only the real points are written, not the guard points _Fix adds at
        // either end.
        xmlNode->SetAttribute("FullTime", CStr(m_fullTime).c_str());
        xmlNode->SetAttribute("FullLength", CStr(m_fullLength).c_str());
        for (int i = 1; i < static_cast<int>(m_cameraPathStates.size()) - 1; ++i)
        {
            ref_ptr pointNode = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Point");
            xmlNode->AddChild(pointNode);
            m_cameraPathStates[i].SaveToXmlRuntime(xmlFile, pointNode);
        }
    }

    void CameraPath::LoadFromXml(cmn::XmlFile*, cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CameraPath::SetFullTime(float fullTime)
    {
        m_fullTime = fullTime;
    }

    void CameraPath::InitByStates(std::vector<CameraPathState, std::allocator<CameraPathState>> const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CameraPath::clear()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CameraPath::RemovePoint(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CameraPathState::CameraPathState(
        CVector const& point,
        Quaternion const& rotation,
        float zoom,
        float flyTime,
        float speed)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CameraPathState::CameraPathState()
    {
        m_point = {0.0, 0.0, 0.0};
        m_rotation = {0.0, 0.0, 0.0, 1.0};
        m_zoom = 1.0;
        m_speed = 1.0;
        m_flyTime = 0.0;
    }

    void CameraPathState::LoadFromXmlRuntime(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        m3d::SafeVectorAttrib(m_point, xmlNode, "coord");
        m3d::SafeQuaternionAttrib(m_rotation, xmlNode, "rotation");
        m3d::SafeFloatAttrib(m_zoom, xmlNode, "zoom");
        m3d::SafeFloatAttrib(m_speed, xmlNode, "speed");
        m3d::SafeFloatAttrib(m_flyTime, xmlNode, "flyTime");
    }

    void CameraPathState::SaveToXmlRuntime(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x625BE0
        xmlNode->SetAttribute("coord", CStr(m_point).c_str());
        xmlNode->SetAttribute("rotation", CStr(m_rotation).c_str());
        xmlNode->SetAttribute("zoom", CStr(m_zoom).c_str());
        xmlNode->SetAttribute("speed", CStr(m_speed).c_str());
        xmlNode->SetAttribute("flyTime", CStr(m_flyTime).c_str());
    }

    CameraPath::CameraPath(std::vector<m3d::CameraPathState, std::allocator<m3d::CameraPathState>> const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CameraPath::CameraPath()
    {
        this->m_fullTime = 1.0;
        this->m_fullLength = 0.0;
    }

    void CameraPath::LoadFromXmlRuntime(cmn::XmlFile* xmlFile, cmn::XmlNode const* xmlNode)
    {
        m_cameraPathStates.clear();
        ref_ptr pointNode = xmlFile->CreateNode();
        for (xmlNode->GetFirstChild(pointNode, "Point"); !pointNode->IsEmpty();
             pointNode->GetNextSibling(pointNode, "Point"))
        {
            CameraPathState state;
            state.LoadFromXmlRuntime(xmlFile, pointNode);
            m_cameraPathStates.push_back(state);
        }
        _Fix();
    }

    CameraPathState& CameraPath::operator[](unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CameraPathState const& CameraPath::operator[](unsigned) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float CameraPath::GetFullTime() const
    {
        return this->m_fullTime;
    }

    float CameraPath::_CalcSplineSegmentLength(unsigned startPointIndex, unsigned endPointIndex) const
    {
        // TODO: generated code
        auto& states = this->m_cameraPathStates;
        // Get references to start and end points
        auto const& startState = states[startPointIndex];
        auto const& endState = states[endPointIndex];

        // Calculate direct distance between points
        CVector delta = startState.m_point - endState.m_point;
        float directDistance = sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);

        // If points are very close, return 0 length
        if (directDistance < 0.001f)
            return 0.0f;

        // Calculate step size for numerical integration
        float stepSize = fabs(1.0f - 1.0f / directDistance) * 0.01f;
        int numSteps = static_cast<int>(1.0f / stepSize);

        // Initialize position tracking
        CVector currentPos = startState.m_point;
        float t = 0.0f;
        float totalLength = 0.0f;

        // Get control points for cubic Hermite spline
        auto const& prevControlPoint = states[startPointIndex - 1];  // Previous control point
        auto const& nextControlPoint = states[endPointIndex + 1];    // Next control point

        // Numerical integration along the spline
        for (int step = 0; step < numSteps; ++step)
        {
            // Calculate cubic Hermite spline basis functions
            float t2 = t * t;
            float t3 = t2 * t;

            // Basis functions for cubic Hermite spline:
            // h1 = 2t� - 3t� + 1  (h00)
            // h2 = -2t� + 3t�      (h01)
            // h3 = t� - 2t� + t    (h10)
            // h4 = t� - t�         (h11)

            float h1 = 2.0f * t3 - 3.0f * t2 + 1.0f;
            float h2 = -2.0f * t3 + 3.0f * t2;
            float h3 = t3 - 2.0f * t2 + t;
            float h4 = t3 - t2;

            // Calculate spline position using Hermite interpolation
            CVector newPos;
            newPos.x = 0.5f *
                ((prevControlPoint.m_point.x * h3) + (startState.m_point.x * h1) + (endState.m_point.x * h2) +
                 (nextControlPoint.m_point.x * h4));

            newPos.y = 0.5f *
                ((prevControlPoint.m_point.y * h3) + (startState.m_point.y * h1) + (endState.m_point.y * h2) +
                 (nextControlPoint.m_point.y * h4));

            newPos.z = 0.5f *
                ((prevControlPoint.m_point.z * h3) + (startState.m_point.z * h1) + (endState.m_point.z * h2) +
                 (nextControlPoint.m_point.z * h4));

            // Calculate segment length from previous position
            CVector segmentDelta = currentPos - newPos;
            float segmentLength = sqrt(
                segmentDelta.x * segmentDelta.x + segmentDelta.y * segmentDelta.y + segmentDelta.z * segmentDelta.z);

            totalLength += segmentLength;

            // Update for next iteration
            currentPos = newPos;
            t += stepSize;
        }

        // Add final segment to reach exact end point
        CVector finalDelta = currentPos - endState.m_point;
        float finalSegmentLength =
            sqrt(finalDelta.x * finalDelta.x + finalDelta.y * finalDelta.y + finalDelta.z * finalDelta.z);
        totalLength += finalSegmentLength;

        return totalLength;
    }

    void CameraPath::_DeFix()
    {
        // RVA 0x629E90 - drops the first and the last point.
        if (!m_cameraPathStates.empty())
        {
            m_cameraPathStates.erase(m_cameraPathStates.begin());
        }
        if (!m_cameraPathStates.empty())
        {
            m_cameraPathStates.erase(m_cameraPathStates.end() - 1);
        }
    }

    void CameraPath::_Fix()
    {
        if (!m_cameraPathStates.empty())
        {
            // TODO: check this
            m_cameraPathStates.insert(m_cameraPathStates.begin(), m_cameraPathStates.front());
            if (m_cameraPathStates.size() < 4)
            {
                m_cameraPathStates.push_back(this->m_cameraPathStates.back());
                if (m_cameraPathStates.size() < 4)
                {
                    m_cameraPathStates.push_back(this->m_cameraPathStates.back());
                }
            }
        }

        // TODO: generated code
        // Fix quaternion continuity by ensuring consecutive rotations have positive dot product
        for (size_t i = 1; i < this->m_cameraPathStates.size(); ++i)
        {
            m3d::CameraPathState& prevState = this->m_cameraPathStates[i - 1];
            m3d::CameraPathState& currentState = this->m_cameraPathStates[i];

            // Calculate dot product between consecutive quaternions
            float dotProduct = (prevState.m_rotation.x * currentState.m_rotation.x) +
                (prevState.m_rotation.y * currentState.m_rotation.y) +
                (prevState.m_rotation.z * currentState.m_rotation.z) +
                (prevState.m_rotation.w * currentState.m_rotation.w);

            // If dot product is negative, flip the current quaternion to maintain continuity
            if (dotProduct < 0.0f)
            {
                currentState.m_rotation.x = -currentState.m_rotation.x;
                currentState.m_rotation.y = -currentState.m_rotation.y;
                currentState.m_rotation.z = -currentState.m_rotation.z;
                currentState.m_rotation.w = -currentState.m_rotation.w;
            }
        }
    }

    bool Cinematic::SkipCinematic()
    {
        if (m_cinematicItems.empty())
        {
            return false;
        }
        _TakeNextCinematicItem();
        return true;
    }

    bool Cinematic::RenderDebugInfo() const
    {
        // RVA 0x6289C0 - the current path as a cyan line sampled about every 2 units with blue
        // view directions, plus every inner control point numbered, circled (green when it is the
        // debug point being edited) and given its own green direction line.
        auto const& states = m_curItem.m_cameraPath.m_cameraPathStates;
        if (states.empty())
        {
            return true;
        }

        int const numSamples = static_cast<int>(m_curItem.m_cameraPath.GetFullLength() * 0.5 + 1.0);
        CameraPathState oldPoint = _GetPathState(0.0f);
        for (int i = 0; i < numSamples; ++i)
        {
            float const curTime =
                static_cast<float>(double(i) * m_curItem.m_cameraPath.GetFullTime() / static_cast<float>(numSamples));
            CameraPathState const newPoint = _GetPathState(curTime);
            M3D_APP->DrawLine(oldPoint.m_point, newPoint.m_point, 0xFF00FFFF);

            CMatrix const m = newPoint.m_rotation.ToMatrix();
            CVector to;
            to.x = m._13 * 5.0f + newPoint.m_point.x;
            to.y = newPoint.m_point.y + m._23 * 5.0f;
            to.z = newPoint.m_point.z + m._33 * 5.0f;
            M3D_APP->DrawLine(newPoint.m_point, to, 0xFF0000FF);
            oldPoint = newPoint;
        }

        for (int i = 1; i < static_cast<int>(states.size()) - 1; ++i)
        {
            auto* renderer = M3D_RENDERER;
            CameraPathState const& state = states[i];

            // The point's number, drawn over everything a little above it.
            renderer->PushZbState(rend::ZB_DISABLE);
            renderer->PushLighting(false);
            renderer->PushBlend(rend::BM_NONE);
            renderer->SetAlphaTest(1);
            renderer->SetTexture(0, {}, -1.0);
            CMatrix worldMat;
            worldMat.identity();
            renderer->MatPush(worldMat);

            CVector const orgInv = renderer->MatGetOrgInv();
            CVector const labelPos(
                state.m_point.x - orgInv.x, state.m_point.y + 5.0f - orgInv.y, state.m_point.z - orgInv.z);
            CVector const v = renderer->Project(labelPos);
            M3D_APP->SetFont(CStr("Tahoma"), 14.0f, 1, M3D_APP->m_codePage.CodePage);
            M3D_APP->DrawTextRel(v.x, v.y, 0xFFFFFFFF, CStr(i), 0, -1);

            renderer->PopZbState();
            renderer->PopLighting();
            renderer->PopBlend();
            renderer->MatPop(false);

            renderer->PushZbState(rend::ZB_NOWRITE);
            renderer->PushLighting(false);
            renderer->PushBlend(rend::BM_NONE);
            renderer->SetAlphaTest(0);
            renderer->SetTexture(0, {}, -1.0);

            float const radius = state.m_zoom * 3.0f;
            M3D_APP->DrawBoundingRadius(state.m_point, radius, i == m_curDebugPointNum + 1 ? 0xFF00FF00 : 0xFFFF0000);

            Quaternion const& r = state.m_rotation;
            CVector dir;
            dir.x = (r.x * r.z - r.w * r.y) * 2.0f * 5.0f + state.m_point.x;
            dir.y = state.m_point.y + (r.y * r.z + r.w * r.x) * 2.0f * 5.0f;
            dir.z = state.m_point.z + (1.0f - (r.y * r.y + r.x * r.x) * 2.0f) * 5.0f;
            M3D_APP->DrawLine(state.m_point, dir, 0xFF00FF00);

            renderer->PopZbState();
            renderer->PopLighting();
            renderer->PopBlend();
        }
        return true;
    }

    bool Cinematic::bMustBeNextCinematic() const
    {
        return !this->m_cinematicItems.empty();
    }

    void Cinematic::SetCurrentDebugPointNum(int pointNum)
    {
        // RVA 0x625DD0 - point numbers are 1-based; the result is clamped to the path.
        m_curDebugPointNum = pointNum - 1;
        int const lastIndex = static_cast<int>(m_curItem.m_cameraPath.size()) - 1;
        if (m_curDebugPointNum < 0)
        {
            m_curDebugPointNum = 0;
        }
        if (m_curDebugPointNum > lastIndex)
        {
            m_curDebugPointNum = lastIndex;
        }
    }

    float Cinematic::GetFadePeriodForState(CinematicState state) const
    {
        float result = 0.0;
        switch (state)
        {
        case CINEMATIC_ENTER_FADE_OUT:
        case CINEMATIC_ENTER_FADE_IN:
            if (!this->m_bWasSkipped && (this->m_curItem.m_flags & 1) == 0)
            {
                result = 0.0;
                break;
            }
            result = m_fadePeriod.GetF();
            break;

        case CINEMATIC_EXIT_FADE_OUT:
        case CINEMATIC_EXIT_FADE_IN:
            if (!this->m_bWasSkipped && (this->m_curItem.m_flags & 2) == 0)
            {
                result = 0.0;
                break;
            }
            result = m_fadePeriod.GetF();
            break;

        default:
            result = 0.0;
            break;
        }
        return result;
    }

    Class* Cinematic::GetClass() const
    {
        return RT_CLASS_LOCAL(Cinematic);
    }

    void Cinematic::SetRelativePoints(bool value)
    {
        m_curItem.m_bRelativePoints = value;
    }

    void Cinematic::SaveToXml(cmn::XmlFile*, cmn::XmlNode*) const
    {
        // RVA 0x624D40 - nothing is saved.
    }

    Cinematic::~Cinematic() = default;

    void Cinematic::SetCameraStates(std::vector<CameraPathState, std::allocator<CameraPathState>> const& states)
    {
        // RVA 0x62AFD0 - NOTE: unlike the point editing functions, the fly times are not recalculated.
        m_curItem.m_cameraPath.m_cameraPathStates = states;
        m_curItem.m_cameraPath._Fix();
    }

    void Cinematic::SetDebugMode(bool bDebug)
    {
        // RVA 0x629970 - debug mode keeps the current path playing so it can be edited.
        m_bDebugMode = bDebug;
        if (bDebug)
        {
            m_curItem.m_playType = CINEMATIC_PLAY_PATH;
            m_curItem.m_cameraPath.CalcFlyTimes(1, true);
        }
    }

    bool Cinematic::GetDebugMode() const
    {
        // RVA 0x624D50
        return m_bDebugMode;
    }

    void Cinematic::SetPathFromPos(CVector const& pos, Quaternion const& rotation, char const* pathName)
    {
        // RVA 0x62A550
        SetPath(pathName);
        SetFromPos(pos, rotation);
    }

    void Cinematic::DumpCurrentPath(CStr const& filename)
    {
        // RVA 0x626FA0 - writes the current path as a one-path file in the format Load reads.
        scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
        if (!stream->Open(filename.c_str(), fs::IStream::OPEN_WRITE))
        {
            return;
        }

        ref_ptr xmlFile = M3D_KERNEL->CreateXmlFile();
        ref_ptr pathNode = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Path");
        xmlFile->AddChild(pathNode);
        if (!m_curItem.m_cameraPath.empty())
        {
            pathNode->SetAttribute("Name", m_curItem.m_cameraPathName.c_str());
            m_curItem.m_cameraPath.SaveToXmlRuntime(xmlFile, pathNode);
        }
        xmlFile->Write(*stream);
        stream->Close();
    }

    bool Cinematic::InPlay() const
    {
        return this->m_curItem.m_playType != CINEMATIC_OFF;
    }

    CinematicType Cinematic::GetPlayType() const
    {
        // RVA 0x624BE0
        return m_curItem.m_playType;
    }

    void Cinematic::SetLookTo(bool value)
    {
        m_curItem.m_bLookTo = value;
    }

    void Cinematic::UpdateCameraRotation(CCamera& cam)
    {
        if (this->m_curItem.m_playType == CINEMATIC_PLAY_PATH)
        {
            if (!this->m_curItem.m_bLerpFromPreviousItem && this->m_curItem.m_bLookTo)
            {
                auto pointToLookAt = _GetPointToLookAt();
                cam.lookAt(pointToLookAt);
            }
        }
        else if (this->m_curItem.m_playType == CINEMATIC_FLY_AROUND)
        {
            auto pointToLookAt = _GetPointToLookAt();
            cam.lookAt(pointToLookAt);
        }
    }

    void Cinematic::InsertPointToCurrentPath(CVector const& point, Quaternion const& rot, float zoom, float)
    {
        // RVA 0x62B090 - inserts after the current debug point, which then moves onto the new one.
        // NOTE: the speed argument is ignored; the new point always gets speed 1.
        ++m_curDebugPointNum;
        CameraPathState const state(point, rot, zoom, 0.0f, 1.0f);
        m_curItem.m_cameraPath.insert(m_curDebugPointNum, state);
        m_curItem.m_cameraPath.CalcFlyTimes(1, true);
    }

    bool Cinematic::Load(char const* fileName)
    {
        CStr fullName = m_folder + fileName;
        scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
        if (!stream->Open(fullName.c_str(), fs::IStream::OPEN_READ) && !stream->Open(fileName, fs::IStream::OPEN_READ))
        {
            M3D_LOG_ERR("Error:Cinematic can't read file " + CStr(fileName));
            return false;
        }

        ref_ptr xmlFile = M3D_KERNEL->CreateXmlFile();
        if (!xmlFile->Read(*stream))
        {
            M3D_LOG_ERR("Error:Cinematic can't read file" + CStr(fileName) + "(" + CStr(xmlFile->GetError()) + ")");
            return false;
        }

        ref_ptr pathsNode = xmlFile->CreateNode();
        xmlFile->GetFirstChild(pathsNode, "Paths");
        if (pathsNode->IsEmpty())
        {
            M3D_LOG_ERR("Error:Cinematic can't find root node Paths in file " + CStr(fileName));
            return false;
        }

        ref_ptr pathNode = xmlFile->CreateNode();
        for (pathsNode->GetFirstChild(pathNode, "Path"); !pathNode->IsEmpty();
             pathNode->GetNextSibling(pathNode, "Path"))
        {
            CStr name = pathNode->GetAttribute("Name");
            if (name.empty())
            {
                continue;
            }

            CameraPath newPath;
            newPath.LoadFromXmlRuntime(xmlFile, pathNode);
            m_paths.emplace(std::move(name), std::move(newPath));
        }
        return true;
    }

    int Cinematic::GetFlags() const
    {
        return this->m_curItem.m_flags;
    }

    void Cinematic::SetFlags(int flags)
    {
        this->m_curItem.m_flags = flags;
    }

    void Cinematic::PlayFromPoint(float playTime, int pointNum)
    {
        // RVA 0x62BC40
        m_curItem.m_cameraPath.SetFullTime(playTime);
        if (static_cast<int>(m_curItem.m_cameraPath.size()) >= pointNum - 1)
        {
            m_curItem.m_cameraPath.CalcFlyTimes(pointNum, true);
            m_curItem.m_playType = CINEMATIC_PLAY_PATH;
            m_cinematicItems.push_back(m_curItem);
        }
        else
        {
            m_curItem.m_playType = CINEMATIC_OFF;
        }
    }

    void Cinematic::SetAim(CVector const& lookAt)
    {
        // RVA 0x624B50 - a fixed point replaces any object being looked at.
        m_curItem.m_pointToLookAt = lookAt;
        m_curItem.m_idToLookAt = -1;
    }

    void Cinematic::SetAimToID(int objId)
    {
        this->m_curItem.m_idToLookAt = objId;
    }

    CStr Cinematic::GetNextFlyPathName() const
    {
        // RVA 0x627A90 - the path of the next queued item, or the current one if none is queued.
        if (!m_cinematicItems.empty())
        {
            return m_cinematicItems.front().m_cameraPathName;
        }
        return m_curItem.m_cameraPathName;
    }

    void Cinematic::AddPointToCurrentPath(CVector const& point, Quaternion const& rot, float zoom, float)
    {
        // RVA 0x62AFF0 - NOTE: the speed argument is ignored; the new point always gets speed 1.
        CameraPathState const state(point, rot, zoom, 0.0f, 1.0f);
        CameraPath& path = m_curItem.m_cameraPath;
        path._DeFix();
        path.m_cameraPathStates.push_back(state);
        path._Fix();
        path.CalcFlyTimes(1, true);
    }

    void Cinematic::Play(float playTime)
    {
        PlayFromPoint(playTime, 1);
    }

    Class* Cinematic::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    void Cinematic::Stop()
    {
        // RVA 0x625D00
        M3D_APP->setZoom(1.0);
        if (!this->m_bDebugMode)
            this->m_curItem.m_playType = CINEMATIC_OFF;
    }

    void Cinematic::RemoveCurrentDebugPoint()
    {
        // RVA 0x62B1C0
        m_curItem.m_cameraPath.RemovePoint(m_curDebugPointNum);
        m_curItem.m_cameraPath.CalcFlyTimes(1, true);
    }

    void Cinematic::FlyAround(
        float phi,
        float theta,
        float radius,
        float playTime,
        CVector const& curPos,
        char const* flyPathName)
    {
        // RVA 0x62BCE0 - queues an orbit around the look-at point that starts where the camera is
        // now and turns by phi and theta; a negative radius keeps the current distance.
        m_curItem.m_cameraPath.SetFullTime(playTime);
        CVector const pointToLookAt = _GetPointToLookAt();

        float const dz = curPos.z - pointToLookAt.z;
        float const dy = curPos.y - pointToLookAt.y;
        float const dx = curPos.x - pointToLookAt.x;
        float const startRadius = static_cast<float>(std::sqrt(double(dz) * dz + double(dy) * dy + double(dx) * dx));
        m_curItem.m_startRadius = startRadius;

        double const startTheta = std::asin(1.0 / startRadius * (curPos.y - pointToLookAt.y));
        m_curItem.m_startTheta = static_cast<float>(startTheta);
        // Straight above or below the target the heading is undefined.
        if (std::fabs(std::cos(startTheta)) >= 0.0099999998)
        {
            m_curItem.m_startPhi =
                static_cast<float>(std::atan2(curPos.z - pointToLookAt.z, curPos.x - pointToLookAt.x));
        }
        else
        {
            m_curItem.m_startPhi = 0.0f;
        }

        m_curItem.m_finalPhi = phi + m_curItem.m_startPhi;
        m_curItem.m_finalTheta = m_curItem.m_startTheta + theta;
        m_curItem.m_finalRadius = radius < 0.0f ? startRadius : radius;
        m_curItem.m_cameraPathName = flyPathName;
        m_curItem.m_playType = CINEMATIC_FLY_AROUND;
        m_cinematicItems.push_back(m_curItem);
    }

    float Cinematic::GetTimeToTheEnd() const
    {
        if (this->m_curItem.m_playType)
            return this->m_curItem.m_cameraPath.GetFullTime() - this->m_curTime;
        else
            return -1.0;
    }

    Object* Cinematic::CreateObject()
    {
        return new Cinematic;
    }

    void Cinematic::SetFromPos(CVector const& pos, Quaternion const& rotation)
    {
        // RVA 0x626F30 - moves the first point of the current path.
        auto& states = m_curItem.m_cameraPath.m_cameraPathStates;
        if (!states.empty())
        {
            states.front().m_point = pos;
            states.front().m_rotation = rotation;
        }
    }

    void Cinematic::SetRelativeRotations(bool value)
    {
        m_curItem.m_bRelativeRotations = value;
    }

    void Cinematic::SetWaitWhenStop(bool wait)
    {
        m_curItem.m_bWaitWhenStop = wait;
    }

    void Cinematic::Update(CCamera& cam, float deltaTime)
    {
        // RVA 0x627E70
        if (m_curItem.m_playType == CINEMATIC_PLAY_PATH)
        {
            // The zoom travels in and out of the path: whatever the camera is
            // on now is the starting point, and the path hands back the value
            // for this instant.
            float zoom = M3D_APP->getZoom();
            CVector cameraPosition{0.0f, 0.0f, 0.0f};
            Quaternion cameraRotation{0.0f, 0.0f, 0.0f, 1.0f};
            m_curItem.m_cameraPath.GetCameraForTime(m_curTime, cameraPosition, cameraRotation, zoom);

            // Where the camera is right now, to spring away from.
            CVector const currentCamPos = cam.m_worldOrigin;

            CMatrix currentCamMatrix;
            currentCamMatrix.rotYPR(cam.m_rotYaw, cam.m_rotPitch, cam.m_rotRoll);
            Quaternion currentCamRot;
            currentCamRot.FromMatrix(currentCamMatrix);

            float interpolationFactor = M3D_ENGINE_CFG.m_cinematic_spring_coeff.GetF() * deltaTime;
            if (interpolationFactor > 1.0f)
            {
                interpolationFactor = 1.0f;
            }

            bool lerpPosition = m_curItem.m_bLerpFromPreviousItem && m_numConsecutiveItemPlayingNow >= 2;

            if (m_curItem.m_bRelativePoints)
            {
                // The path is authored in the base object's frame.
                CMatrix const rotationMatrix = _GetBaseRotation().ToMatrix();
                cameraPosition = rotationMatrix.vecRot(cameraPosition);
                cameraPosition = cameraPosition + _GetBasePoint();

                // Past the first tenth of a second the camera always springs
                // towards the relative path rather than snapping onto it.
                if (m_curTime > 0.1f)
                {
                    lerpPosition = true;
                }
            }

            if (lerpPosition)
            {
                cameraPosition = lerp(currentCamPos, cameraPosition, interpolationFactor);
            }
            cam.m_worldOrigin = cameraPosition;

            Quaternion newCamRot{0.0f, 0.0f, 0.0f, 1.0f};
            if (!m_curItem.m_bLookTo)
            {
                if (m_curItem.m_bRelativeRotations)
                {
                    cameraRotation *= _GetBaseRotation().getInversed();
                }
                newCamRot = cameraRotation;
            }
            else
            {
                // Aim at the target, then read the resulting angles back out so
                // they go through the same smoothing as a path rotation would.
                cam.lookAt(_GetPointToLookAt());

                CMatrix lookAtMatrix;
                lookAtMatrix.rotYPR(cam.m_rotYaw, cam.m_rotPitch, cam.m_rotRoll);
                newCamRot.FromMatrix(lookAtMatrix);
            }

            if ((m_curItem.m_bRelativePoints && m_curTime > 0.1f) ||
                (m_curItem.m_bLerpFromPreviousItem && m_numConsecutiveItemPlayingNow >= 2))
            {
                newCamRot = SLerpAcc(currentCamRot, newCamRot, interpolationFactor);
            }

            newCamRot.ToMatrix().getYPR(cam.m_rotYaw, cam.m_rotPitch, cam.m_rotRoll);

            M3D_APP->setZoom(zoom);
        }
        else if (m_curItem.m_playType == CINEMATIC_FLY_AROUND)
        {
            // A spherical orbit around the target: the two angles and the radius
            // are interpolated straight across the item's full time.
            float const t = m_curTime / m_curItem.m_cameraPath.GetFullTime();
            float const curPhi = m_curItem.m_startPhi * (1.0f - t) + m_curItem.m_finalPhi * t;
            float const curTheta = m_curItem.m_startTheta * (1.0f - t) + m_curItem.m_finalTheta * t;
            float const curRadius = m_curItem.m_startRadius * (1.0f - t) + m_curItem.m_finalRadius * t;

            CVector const pointToLookAt = _GetPointToLookAt();

            float const cosTheta = cosf(curTheta);
            cam.m_worldOrigin.x = cosf(curPhi) * cosTheta * curRadius + pointToLookAt.x;
            cam.m_worldOrigin.z = cosTheta * sinf(curPhi) * curRadius + pointToLookAt.z;
            cam.m_worldOrigin.y = sinf(curTheta) * curRadius + pointToLookAt.y;

            // Never orbit through the ground.
            float const minY =
                pClient->GetWorld().GetLandscape().GetLsHeight(cam.m_worldOrigin.x, cam.m_worldOrigin.z) + 3.0f;
            if (minY > cam.m_worldOrigin.y)
            {
                cam.m_worldOrigin.y = minY;
            }

            cam.lookAt(pointToLookAt);
        }

        if (m_curTime > m_curItem.m_cameraPath.GetFullTime() && !m_curItem.m_bWaitWhenStop)
        {
            M3D_APP->setZoom(1.0f);
            if (!m_bDebugMode)
            {
                m_curItem.m_playType = CINEMATIC_OFF;
            }
        }

        m_curTime += deltaTime;
    }

    void Cinematic::SetLerpFromPreviousItem(bool bLerp)
    {
        m_curItem.m_bLerpFromPreviousItem = bLerp;
    }

    CinematicItem const& Cinematic::GetCurItem() const
    {
        return m_curItem;
    }

    CVector Cinematic::_GetPointToLookAt() const
    {
        using namespace ai;

        if (m_curItem.m_idToLookAt != -1)
        {
            auto obj = theObjects->GetEntityByObjId(m_curItem.m_idToLookAt);

            if (obj && IS_KIND_OF(obj, PhysicObj))
            {
                auto* physObj = RT_DYNCAST(obj, PhysicObj);
                return physObj->GetPosition();
            }
        }
        return m_curItem.m_pointToLookAt;
    }

    CVector Cinematic::_GetBasePoint() const
    {
        using namespace ai;

        if (m_curItem.m_baseId != -1)
        {
            auto obj = theObjects->GetEntityByObjId(m_curItem.m_baseId);

            if (obj && IS_KIND_OF(obj, PhysicObj))
            {
                auto* physObj = RT_DYNCAST(obj, PhysicObj);
                return physObj->GetPosition();
            }
            m_curItem.m_baseId = -1;
        }
        return {0.0, 0.0, 0.0};
    }

    Quaternion Cinematic::_GetBaseRotation() const
    {
        using namespace ai;

        if (m_curItem.m_baseId != -1)
        {
            auto obj = theObjects->GetEntityByObjId(m_curItem.m_baseId);

            if (obj && IS_KIND_OF(obj, PhysicObj))
            {
                auto* physObj = RT_DYNCAST(obj, PhysicObj);
                return physObj->GetRotation();
            }
            m_curItem.m_baseId = -1;
        }
        return {0.0, 0.0, 0.0, 1.0};
    }

    m3d::CameraPathState Cinematic::_GetPathState(float curTime) const
    {
        // RVA 0x6272D0 - where the current item puts the camera at curTime, as RenderDebugInfo
        // draws it.
        CameraPathState result;
        result.m_point = ZeroVector;
        result.m_rotation = IdentityQuaternion;
        result.m_zoom = 1.0f;

        if (m_curItem.m_playType == CINEMATIC_PLAY_PATH)
        {
            float zoom = M3D_APP->getZoom();
            Quaternion rot = IdentityQuaternion;
            CVector pos = ZeroVector;
            m_curItem.m_cameraPath.GetCameraForTime(curTime, pos, rot, zoom);

            if (m_curItem.m_bRelativePoints)
            {
                pos = _GetBaseRotation().ToMatrix().vecRot(pos) + _GetBasePoint();
            }

            if (m_curItem.m_bLookTo)
            {
                rot = ai::GetRotationByDirection(_GetPointToLookAt() - pos);
            }
            else if (m_curItem.m_bRelativeRotations)
            {
                // NOTE: the path rotation is taken into the base frame and then back out again
                // (rot * base, then inverse(base) * rot), unlike Update, which only applies the
                // inverse.
                rot *= _GetBaseRotation();
                Quaternion inverted = _GetBaseRotation().getInversed();
                inverted *= rot;
                rot = inverted;
            }

            result.m_point = pos;
            result.m_rotation = rot;
            result.m_zoom = zoom;
        }
        else if (m_curItem.m_playType == CINEMATIC_FLY_AROUND)
        {
            // NOTE: this does not match the orbit Update flies: phi is used for both x and y, and
            // theta alone for z.
            float const t = curTime / m_curItem.m_cameraPath.GetFullTime();
            float const curPhi = m_curItem.m_startPhi * (1.0f - t) + m_curItem.m_finalPhi * t;
            float const curTheta = m_curItem.m_startTheta * (1.0f - t) + m_curItem.m_finalTheta * t;
            float const curRadius = m_curItem.m_startRadius * (1.0f - t) + m_curItem.m_finalRadius * t;
            CVector const pointToLookAt = _GetPointToLookAt();
            result.m_point.x = std::cos(curPhi) * curRadius + pointToLookAt.x;
            result.m_point.y = std::sin(curPhi) * curRadius + pointToLookAt.y;
            result.m_point.z = std::sin(curTheta) * curRadius + pointToLookAt.z;
        }

        result.m_speed = 1.0f;
        result.m_flyTime = curTime;
        return result;
    }

    void Cinematic::_PushCinematicItem()
    {
        // RVA 0x62B9F0
        m_cinematicItems.push_back(m_curItem);
    }

    bool Cinematic::_bIsFirstItemPlayingNow() const
    {
        // RVA 0x624DB0
        return m_numConsecutiveItemPlayingNow < 2;
    }

    bool Cinematic::_TakeNextCinematicItem()
    {
        ++this->m_numConsecutiveItemPlayingNow;
        if (this->m_cinematicItems.empty())
            return 0;

        m_curItem = m_cinematicItems.front();
        m_cinematicItems.erase(m_cinematicItems.begin());
        this->m_playTime = 0;
        this->m_fadeStartTime = 0;
        this->m_state = CINEMATIC_ENTER_FADE_OUT;
        this->m_bWasSkipped = 0;
        this->m_bWasSkippedInEnterFadeOut = 0;
        this->m_curTime = 0.0;
        return 1;
    }

    bool Cinematic::bCanUpdate() const
    {
        return m_state == CINEMATIC_ENTER_FADE_IN || m_state == CINEMATIC_IS_PLAYING ||
            m_state == CINEMATIC_EXIT_FADE_OUT;
    }

    void Cinematic::StartCinematic()
    {
        _TakeNextCinematicItem();
    }

    void Cinematic::SetFolder(char const* folder)
    {
        m_folder = folder;
        UnifyFileName(m_folder);
        if (!m_folder.empty())
        {
            if (m_folder[m_folder.length() - 1] != '/')
            {
                m_folder += "/";
            }
        }
    }

    char const* Cinematic::GetFolder() const
    {
        // RVA 0x624C20
        return m_folder.c_str();
    }

    void Cinematic::LoadFromXml(cmn::XmlFile*, cmn::XmlNode const*)
    {
        // RVA 0x624D30 - nothing is loaded.
    }

    void Cinematic::SetBaseToId(int objId)
    {
        this->m_curItem.m_baseId = objId;
    }

    Object* Cinematic::Clone()
    {
        // RVA 0x62D060 - NOTE: the original builds a copy and then returns nullptr anyway, leaking the
        // copy; cloning simply fails here.
        return nullptr;
    }

    bool Cinematic::SetPath(char const* pathName)
    {
        M3D_APP->setZoom(1.0);
        if (!m_bDebugMode)
        {
            m_curItem.m_playType = CINEMATIC_OFF;
        }

        auto const it = m_paths.find(pathName);
        if (it == m_paths.end())
        {
            m_curItem.m_cameraPath.clear();
            m_curItem.m_cameraPathName = {};
            return false;
        }

        m_curItem.m_cameraPathName = it->first;
        m_curItem.m_cameraPath = it->second;

        return true;
    }

    bool Cinematic::bWaitWhenStop() const
    {
        return this->m_curItem.m_bWaitWhenStop;
    }

    CameraPath const& Cinematic::GetPathByName(CStr const& pathName) const
    {
        // RVA 0x628960
        auto it = m_paths.find(pathName);
        if (it == m_paths.end())
        {
            return CameraPath::m_emptyPath;
        }
        return it->second;
    }

    void Cinematic::LoadDefaults()
    {
        // RVA 0x62C880 - resets to a single idle item.
        m_cinematicItems.clear();
        m3d::CinematicItem item;
        m_cinematicItems.push_back(std::move(item));
        _TakeNextCinematicItem();
        m_numConsecutiveItemPlayingNow = 0;
        m_state = CINEMATIC_NOT_INITED;
    }

    void Cinematic::MoveCurrentDebugPoint(CVector const& point, Quaternion const& rot, float zoom)
    {
        // RVA 0x62B130
        CameraPathState const state(point, rot, zoom, 0.0f, 1.0f);
        m_curItem.m_cameraPath.MovePoint(m_curDebugPointNum, state);
        m_curItem.m_cameraPath.CalcFlyTimes(1, true);
    }

    Cinematic::Cinematic() : m_fadePeriod("cinematicFadePeriod", "1", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE)
    {
        this->m_curItem.m_flags = 0;
        this->m_curItem.m_bLookTo = 0;
        this->m_curItem.m_idToLookAt = -1;
        this->m_curItem.m_pointToLookAt = CVector(0.0, 0.0, 0.0);
        this->m_curItem.m_baseId = -1;
        this->m_curItem.m_bRelativeRotations = 0;
        this->m_curItem.m_bRelativePoints = 0;
        this->m_curItem.m_playType = CINEMATIC_OFF;
        this->m_curItem.m_bWaitWhenStop = 0;
        this->m_curItem.m_finalPhi = 0.0;
        this->m_curItem.m_finalTheta = 0.0;
        this->m_curItem.m_finalRadius = 0.0;
        this->m_curItem.m_startPhi = 0.0;
        this->m_curItem.m_startTheta = 0.0;
        this->m_curItem.m_startRadius = 0.0;
        this->m_curItem.m_bLerpFromPreviousItem = 0;
        this->m_playTime = 0;
        this->m_fadeStartTime = 0;
        this->m_bWasSkipped = 0;
        this->m_bWasSkippedInEnterFadeOut = 0;
        this->m_bDebugMode = 0;
        this->m_numConsecutiveItemPlayingNow = 0;
        this->m_state = CINEMATIC_NOT_INITED;
        this->m_curTime = 0.0;
        this->m_curDebugPointNum = -1;
    }
}  // namespace m3d
