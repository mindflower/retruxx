#include <cinematic.h>
#include <stdexcept>

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

    CameraPath::CameraPath()
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
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
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

    void Cinematic::SetFolder(char const*)
    {
        throw std::logic_error("Not implemented");
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

    Cinematic::Cinematic(Cinematic const&)
    {
        throw std::logic_error("Not implemented");
    }

    Cinematic::Cinematic()
    {
        throw std::logic_error("Not implemented");
    }
}
