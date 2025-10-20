#include <cinematic.h>
#include <stdexcept>

#include "m3dapp.h"
#include "core/ini.h"
#include "core/kernel.h"
#include "core/log.h"
#include "core/ref_ptr.h"
#include "core/scoped_ptr.h"
#include "file/fileserver.h"
#include "file/filestream.h"

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
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, Stop)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, InPlay)
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, LoadDefaults)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, SetAim)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, SetAimToID)
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, GetFolder)
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    unsigned CameraPath::size() const
    {
        return m_cameraPathStates.size();
    }

    void CameraPath::GetCameraForTime(float, CVector&, Quaternion&, float&) const
    {
        throw std::logic_error("Not implemented");
    }

    bool CameraPath::empty() const
    {
        throw std::logic_error("Not implemented");
    }

    void CameraPath::CalcFlyTimes(unsigned, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void CameraPath::push_back(CameraPathState const&)
    {
        throw std::logic_error("Not implemented");
    }

    void CameraPath::MovePoint(int, CameraPathState const&)
    {
        throw std::logic_error("Not implemented");
    }

    void CameraPath::CalcFullLength(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    float CameraPath::GetFullLength() const
    {
        throw std::logic_error("Not implemented");
    }

    void CameraPath::SaveToXmlRuntime(cmn::XmlFile*, cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void CameraPath::LoadFromXml(cmn::XmlFile*, cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void CameraPath::SetFullTime(float fullTime)
    {
        m_fullTime = fullTime;
    }

    void CameraPath::InitByStates(std::vector<CameraPathState, std::allocator<CameraPathState>> const&)
    {
        throw std::logic_error("Not implemented");
    }

    void CameraPath::clear()
    {
        throw std::logic_error("Not implemented");
    }

    void CameraPath::RemovePoint(int)
    {
        throw std::logic_error("Not implemented");
    }

    CameraPathState::CameraPathState(const CVector& point, const Quaternion& rotation, float zoom, float flyTime,
        float speed)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    CameraPath::CameraPath(const std::vector<m3d::CameraPathState, std::allocator<m3d::CameraPathState>>&)
    {
        throw std::logic_error("Not implemented");
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
        for (xmlNode->GetFirstChild(pointNode, "Point"); !pointNode->IsEmpty(); xmlNode->GetNextSibling(pointNode, "Point"))
        {
            CameraPathState state;
            state.LoadFromXmlRuntime(xmlFile, pointNode);
            m_cameraPathStates.push_back(state);
        }
        _Fix();
    }

    CameraPathState& CameraPath::operator[](unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    CameraPathState const& CameraPath::operator[](unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    float CameraPath::GetFullTime() const
    {
        return this->m_fullTime;
    }

    float CameraPath::_CalcSplineSegmentLength(unsigned, unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    void CameraPath::_DeFix()
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    bool Cinematic::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Cinematic::bMustBeNextCinematic() const
    {
        return !this->m_cinematicItems.empty();
    }

    void Cinematic::SetCurrentDebugPointNum(int)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    Cinematic::~Cinematic()
    {
        throw std::logic_error("Not implemented");
    }

    void Cinematic::SetCameraStates(std::vector<CameraPathState, std::allocator<CameraPathState>> const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Cinematic::SetDebugMode(bool)
    {
        throw std::logic_error("Not implemented");
    }

    bool Cinematic::GetDebugMode() const
    {
        throw std::logic_error("Not implemented");
    }

    void Cinematic::SetPathFromPos(CVector const&, Quaternion const&, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Cinematic::DumpCurrentPath(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool Cinematic::InPlay() const
    {
        return this->m_curItem.m_playType != CINEMATIC_OFF;
    }

    CinematicType Cinematic::GetPlayType() const
    {
        throw std::logic_error("Not implemented");
    }

    void Cinematic::SetLookTo(bool value)
    {
        m_curItem.m_bLookTo = value;
    }

    void Cinematic::UpdateCameraRotation(CCamera&)
    {
        throw std::logic_error("Not implemented");
    }

    void Cinematic::InsertPointToCurrentPath(CVector const&, Quaternion const&, float, float)
    {
        throw std::logic_error("Not implemented");
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
        for (pathsNode->GetFirstChild(pathNode, "Path"); !pathNode->IsEmpty(); pathsNode->GetNextSibling(pathNode, "Path"))
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
        throw std::logic_error("Not implemented");
    }

    void Cinematic::SetAimToID(int)
    {
        throw std::logic_error("Not implemented");
    }

    CStr Cinematic::GetNextFlyPathName() const
    {
        throw std::logic_error("Not implemented");
    }

    void Cinematic::AddPointToCurrentPath(CVector const&, Quaternion const&, float, float)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void Cinematic::FlyAround(float, float, float, float, CVector const&, char const*)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void Cinematic::SetRelativeRotations(bool value)
    {
        m_curItem.m_bRelativeRotations = value;
    }

    void Cinematic::SetWaitWhenStop(bool wait)
    {
        m_curItem.m_bWaitWhenStop = wait;
    }

    void Cinematic::Update(CCamera&, float)
    {
        throw std::logic_error("Not implemented");
    }

    void Cinematic::SetLerpFromPreviousItem(bool bLerp)
    {
        m_curItem.m_bLerpFromPreviousItem = bLerp;
    }

    CinematicItem const& Cinematic::GetCurItem() const
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void Cinematic::LoadFromXml(cmn::XmlFile*, cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Cinematic::SetBaseToId(int)
    {
        throw std::logic_error("Not implemented");
    }

    Object* Cinematic::Clone()
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void Cinematic::LoadDefaults()
    {
        m_cinematicItems.clear();
        throw std::logic_error("Not implemented");
    }

    void Cinematic::MoveCurrentDebugPoint(CVector const&, Quaternion const&, float)
    {
        throw std::logic_error("Not implemented");
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
