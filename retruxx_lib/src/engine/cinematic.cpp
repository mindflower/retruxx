#include <cinematic.h>
#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, StartCinematic)
{
    auto cinematic = (m3d::Cinematic*)context->asObject(0, "Cinematic");
    cinematic->StartCinematic();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, Play)
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, SetRelativeRotations)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, SetBaseToId)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, SetLookTo)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, SetWaitWhenStop)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, SetLerpFromPreviousItem)
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
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

    void CameraPath::SetFullTime(float)
    {
        throw std::logic_error("Not implemented");
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

    void CameraPath::LoadFromXmlRuntime(cmn::XmlFile*, cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
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

    void Cinematic::SetRelativePoints(bool)
    {
        throw std::logic_error("Not implemented");
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

    void Cinematic::SetLookTo(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void Cinematic::UpdateCameraRotation(CCamera&)
    {
        throw std::logic_error("Not implemented");
    }

    void Cinematic::InsertPointToCurrentPath(CVector const&, Quaternion const&, float, float)
    {
        throw std::logic_error("Not implemented");
    }

    bool Cinematic::Load(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int Cinematic::GetFlags() const
    {
        throw std::logic_error("Not implemented");
    }

    void Cinematic::SetFlags(int)
    {
        throw std::logic_error("Not implemented");
    }

    void Cinematic::PlayFromPoint(float, int)
    {
        throw std::logic_error("Not implemented");
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

    void Cinematic::Play(float)
    {
        throw std::logic_error("Not implemented");
    }

    Class* Cinematic::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    void Cinematic::Stop()
    {
        throw std::logic_error("Not implemented");
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

    void Cinematic::SetRelativeRotations(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void Cinematic::SetWaitWhenStop(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void Cinematic::Update(CCamera&, float)
    {
        // TODO: implement Cinematic::Update
        //throw std::logic_error("Not implemented");
    }

    void Cinematic::SetLerpFromPreviousItem(bool)
    {
        throw std::logic_error("Not implemented");
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

    bool Cinematic::SetPath(char const*)
    {
        throw std::logic_error("Not implemented");
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
