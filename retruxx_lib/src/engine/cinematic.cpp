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
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, Stop)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, InPlay)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, LoadDefaults)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, SetAim)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, GetFolder)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
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
}

namespace m3d
{
    void CameraPath::insert(int, CameraPathState const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        const auto& p0 = this->m_cameraPathStates[segmentIndex - 1];
        const auto& p1 = this->m_cameraPathStates[segmentIndex];
        const auto& p2 = this->m_cameraPathStates[segmentIndex + 1];
        const auto& p3 = this->m_cameraPathStates[segmentIndex + 2];

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
                states[i].m_flyTime = states[i - 1].m_flyTime +
                    ((currentFlyTime - previousFlyTime) * averageSpeed / segmentSpeed) * 2.0f;

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

    void CameraPath::SaveToXmlRuntime(cmn::XmlFile*, cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    CameraPathState::CameraPathState(const CVector& point, const Quaternion& rotation, float zoom, float flyTime,
        float speed)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CameraPathState::CameraPathState()
    {
        m_point = {0.0, 0.0, 0.0};
        m_rotation = { 0.0, 0.0, 0.0, 1.0 };
        m_zoom = 1.0;
        m_speed = 1.0;
        m_flyTime = 0.0;
    }

    void CameraPathState::LoadFromXmlRuntime(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        m3d::SafeVectorAttrib(m_point, xmlNode, "coord");
        m3d::SafeQuaternionAttrib(m_rotation, xmlNode, "rotation");
        m3d::SafeFloatAttrib(m_zoom, xmlNode, "zoom");
        m3d::SafeFloatAttrib(m_speed, xmlNode, "speed");
        m3d::SafeFloatAttrib(m_flyTime, xmlNode, "flyTime");
    }

    void CameraPathState::SaveToXmlRuntime(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CameraPath::CameraPath(const std::vector<m3d::CameraPathState, std::allocator<m3d::CameraPathState>>&)
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
        for (xmlNode->GetFirstChild(pointNode, "Point"); !pointNode->IsEmpty(); pointNode->GetNextSibling(pointNode, "Point"))
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
        const auto& startState = states[startPointIndex];
        const auto& endState = states[endPointIndex];

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
        const auto& prevControlPoint = states[startPointIndex - 1];  // Previous control point
        const auto& nextControlPoint = states[endPointIndex + 1];    // Next control point

        // Numerical integration along the spline
        for (int step = 0; step < numSteps; ++step)
        {
            // Calculate cubic Hermite spline basis functions
            float t2 = t * t;
            float t3 = t2 * t;

            // Basis functions for cubic Hermite spline:
            // h1 = 2t³ - 3t² + 1  (h00)
            // h2 = -2t³ + 3t²      (h01)  
            // h3 = t³ - 2t² + t    (h10)
            // h4 = t³ - t²         (h11)

            float h1 = 2.0f * t3 - 3.0f * t2 + 1.0f;
            float h2 = -2.0f * t3 + 3.0f * t2;
            float h3 = t3 - 2.0f * t2 + t;
            float h4 = t3 - t2;

            // Calculate spline position using Hermite interpolation
            CVector newPos;
            newPos.x = 0.5f * ((prevControlPoint.m_point.x * h3) +
                               (startState.m_point.x * h1) +
                               (endState.m_point.x * h2) +
                               (nextControlPoint.m_point.x * h4));

            newPos.y = 0.5f * ((prevControlPoint.m_point.y * h3) +
                               (startState.m_point.y * h1) +
                               (endState.m_point.y * h2) +
                               (nextControlPoint.m_point.y * h4));

            newPos.z = 0.5f * ((prevControlPoint.m_point.z * h3) +
                               (startState.m_point.z * h1) +
                               (endState.m_point.z * h2) +
                               (nextControlPoint.m_point.z * h4));

            // Calculate segment length from previous position
            CVector segmentDelta = currentPos - newPos;
            float segmentLength = sqrt(segmentDelta.x * segmentDelta.x +
                                       segmentDelta.y * segmentDelta.y +
                                       segmentDelta.z * segmentDelta.z);

            totalLength += segmentLength;

            // Update for next iteration
            currentPos = newPos;
            t += stepSize;
        }

        // Add final segment to reach exact end point
        CVector finalDelta = currentPos - endState.m_point;
        float finalSegmentLength = sqrt(finalDelta.x * finalDelta.x +
                                        finalDelta.y * finalDelta.y +
                                        finalDelta.z * finalDelta.z);
        totalLength += finalSegmentLength;

        return totalLength;
    }

    void CameraPath::_DeFix()
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Cinematic::RenderDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Cinematic::bMustBeNextCinematic() const
    {
        return !this->m_cinematicItems.empty();
    }

    void Cinematic::SetCurrentDebugPointNum(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    Cinematic::~Cinematic()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Cinematic::SetCameraStates(std::vector<CameraPathState, std::allocator<CameraPathState>> const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Cinematic::SetDebugMode(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Cinematic::GetDebugMode() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Cinematic::SetPathFromPos(CVector const&, Quaternion const&, char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Cinematic::DumpCurrentPath(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Cinematic::InPlay() const
    {
        return this->m_curItem.m_playType != CINEMATIC_OFF;
    }

    CinematicType Cinematic::GetPlayType() const
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    void Cinematic::InsertPointToCurrentPath(CVector const&, Quaternion const&, float, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        for (pathsNode->GetFirstChild(pathNode, "Path"); !pathNode->IsEmpty(); pathNode->GetNextSibling(pathNode, "Path"))
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
        m_curItem.m_cameraPath.SetFullTime(playTime);
        const auto size = m_curItem.m_cameraPath.size() / 40;
        if (size >= (pointNum - 1))
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

    void Cinematic::SetAim(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Cinematic::SetAimToID(int objId)
    {
        this->m_curItem.m_idToLookAt = objId;
    }

    CStr Cinematic::GetNextFlyPathName() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Cinematic::AddPointToCurrentPath(CVector const&, Quaternion const&, float, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        // TODO: check this
        M3D_APP->setZoom(1.0);
        if (!this->m_bDebugMode)
            this->m_curItem.m_playType = CINEMATIC_OFF;
    }

    void Cinematic::RemoveCurrentDebugPoint()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Cinematic::FlyAround(float, float, float, float, CVector const&, char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    void Cinematic::SetFromPos(CVector const&, Quaternion const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        // TODO: generated code
        if (this->m_curItem.m_playType == CINEMATIC_PLAY_PATH)
        {
            // Get camera position and rotation from camera path
            CVector cameraPosition{0.0, 0.0, 0.0};
            Quaternion cameraRotation{ 0.0, 0.0, 0.0, 1.0 };
            float zoom = M3D_APP->getZoom();

            m_curItem.m_cameraPath.GetCameraForTime(this->m_curTime,cameraPosition, cameraRotation, zoom);

            // Store current camera state for interpolation
            CVector currentCamPos = cam.m_worldOrigin;

            CMatrix currentCamMatrix;
            currentCamMatrix.rotYPR(cam.m_rotYaw, cam.m_rotPitch, cam.m_rotRoll);

            Quaternion currentCamRot;
            currentCamRot.FromMatrix(currentCamMatrix);

            // Calculate interpolation factor based on spring coefficient and delta time
            float springCoeff = M3D_ENGINE_CFG.m_cinematic_spring_coeff.GetF();
            float interpolationFactor = springCoeff * deltaTime;
            interpolationFactor = (interpolationFactor <= 1.0f) ? interpolationFactor : 1.0f;

            // Handle relative coordinate transformations
            if (this->m_curItem.m_bRelativePoints)
            {
                // Transform camera position by base rotation and translation
                Quaternion baseRotation = m3d::Cinematic::_GetBaseRotation();
                CVector basePoint = m3d::Cinematic::_GetBasePoint();

                // Rotate camera position by base rotation
                CMatrix rotationMatrix = baseRotation.ToMatrix();
                cameraPosition = rotationMatrix.vecRot(cameraPosition);

                // Translate by base point
                cameraPosition = cameraPosition + basePoint;

                // Apply interpolation if needed
                if (this->m_curTime > 0.1f ||
                    (this->m_curItem.m_bLerpFromPreviousItem && this->m_numConsecutiveItemPlayingNow >= 2))
                {
                    cameraPosition = lerp(currentCamPos, cameraPosition, interpolationFactor);
                }
            }
            else
            {
                // Apply interpolation for non-relative points with consecutive items
                if (this->m_curItem.m_bLerpFromPreviousItem && this->m_numConsecutiveItemPlayingNow >= 2)
                {
                    cameraPosition = lerp(currentCamPos, cameraPosition, interpolationFactor);
                }
            }

            // Update camera position
            cam.m_worldOrigin = cameraPosition;

            // Handle camera rotation
            if (!this->m_curItem.m_bLookTo)
            {
                if (this->m_curItem.m_bRelativeRotations)
                {
                    // Transform rotation by base rotation
                    Quaternion baseRotation = m3d::Cinematic::_GetBaseRotation();
                    Quaternion inverseBaseRotation = baseRotation.getInversed();
                    cameraRotation = inverseBaseRotation;
                    cameraRotation *= cameraRotation;
                }

                // Apply interpolation to rotation if needed
                if ((this->m_curItem.m_bRelativePoints && this->m_curTime > 0.1f) ||
                    (this->m_curItem.m_bLerpFromPreviousItem && this->m_numConsecutiveItemPlayingNow >= 2))
                {
                    cameraRotation.Lerp(currentCamRot, cameraRotation, interpolationFactor);
                }

                // Convert quaternion to Euler angles and update camera
                CMatrix rotationMatrix = cameraRotation.ToMatrix();

                float yaw, pitch, roll;
                rotationMatrix.getYPR(yaw, pitch, roll);

                cam.m_rotYaw = yaw;
                cam.m_rotPitch = pitch;
                cam.m_rotRoll = roll;
            }
            else
            {
                // Look at specific point
                CVector targetPoint = m3d::Cinematic::_GetPointToLookAt();
                cam.lookAt(targetPoint);
            }
        }
        else if (this->m_curItem.m_playType == CINEMATIC_FLY_AROUND)
        {

            RETRUXX_NOT_IMPLEMENTED;
        }

        // Handle cinematic completion
        if (this->m_curTime > this->m_curItem.m_cameraPath.GetFullTime() && !this->m_curItem.m_bWaitWhenStop)
        {
            // Reset zoom and stop cinematic if not in debug mode
            M3D_APP->setZoom(1.0f);
            if (!this->m_bDebugMode)
            {
                this->m_curItem.m_playType = CINEMATIC_OFF;
            }
        }

        // Update current time
        this->m_curTime += deltaTime;
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
        return { 0.0, 0.0, 0.0 };
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
        return { 0.0, 0.0, 0.0, 1.0 };
    }

    m3d::CameraPathState Cinematic::_GetPathState(float curTime) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Cinematic::_PushCinematicItem()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Cinematic::_bIsFirstItemPlayingNow() const
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        return m_state == CINEMATIC_ENTER_FADE_IN || m_state == CINEMATIC_IS_PLAYING || m_state == CINEMATIC_EXIT_FADE_OUT;
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
            if (m_folder[m_folder.length() -1] != '/')
            {
                m_folder+= "/";
            }
        }
    }

    char const* Cinematic::GetFolder() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Cinematic::LoadFromXml(cmn::XmlFile*, cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Cinematic::SetBaseToId(int objId)
    {
        this->m_curItem.m_baseId = objId;
    }

    Object* Cinematic::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Cinematic::SetPath(char const* pathName)
    {
        M3D_APP->setZoom(1.0);
        if (!m_bDebugMode)
        {
            m_curItem.m_playType = CINEMATIC_OFF;
        }

        const auto it = m_paths.find(pathName);
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

    CameraPath const& Cinematic::GetPathByName(CStr const&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Cinematic::LoadDefaults()
    {
        // TODO: check this
        m_cinematicItems.clear();
        m3d::CinematicItem item;
        m_cinematicItems.push_back(std::move(item));
        _TakeNextCinematicItem();
        m_numConsecutiveItemPlayingNow = 0;
        m_state = CINEMATIC_NOT_INITED;
    }

    void Cinematic::MoveCurrentDebugPoint(CVector const&, Quaternion const&, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Cinematic::Cinematic() :
        m_fadePeriod("cinematicFadePeriod", "1", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE)
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
}
