#include <cinematic.h>
#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(Cinematic, StartCinematic)
{
    throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
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

    float Cinematic::GetFadePeriodForState(CinematicState) const
    {
        throw std::logic_error("Not implemented");
    }

    Class* Cinematic::GetClass() const
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void Cinematic::SetLerpFromPreviousItem(bool)
    {
        throw std::logic_error("Not implemented");
    }

    CinematicItem const& Cinematic::GetCurItem() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Cinematic::bCanUpdate() const
    {
        throw std::logic_error("Not implemented");
    }

    void Cinematic::StartCinematic()
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
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
    }
}
