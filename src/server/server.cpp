#include "server.h"
#include <stdexcept>

namespace ai
{
    CServer* CServer::pServer = nullptr;

    void CServer::AddToCinematic(Obj*, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::AddToCinematic(int, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::SetPause(bool)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Profiler* CServer::GetTmpProfiler()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Profiler* CServer::GetBulletProfiler()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Profiler* CServer::GetCollideProfiler()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Profiler* CServer::GetObjectsUpdateProfiler()
    {
        throw std::logic_error("Not implemented");
    }

    ExternalPaths const* CServer::GetExternalPaths() const
    {
        throw std::logic_error("Not implemented");
    }

    PlayerPassMap const* CServer::GetPlayerPassMap() const
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::SetLastId(int)
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::Load(StartupMode, m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*, bool, ObjContainer::eSAVE_TYPES)
    {
        throw std::logic_error("Not implemented");
    }

    bool CServer::GetPause() const
    {
        throw std::logic_error("Not implemented");
    }

    int CServer::GetPrototypeId(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    PrototypeInfo* CServer::CreatePrototypeInfoByClassName(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::CWorld* CServer::GetWorld()
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::LoadVisitedMap(CStr const&, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::Register()
    {
        throw std::logic_error("Not implemented");
    }

    float CServer::GetLevelSize() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Level* CServer::GetLevel() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Profiler* CServer::GetPathFindingProfiler()
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::RelinkSceneGraphNodes()
    {
        throw std::logic_error("Not implemented");
    }

    CServer::CServer()
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::LoadTriggersFromXML(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::PostPlayerEvent(eGameEvent)
    {
        throw std::logic_error("Not implemented");
    }

    int CServer::GetLastId()
    {
        throw std::logic_error("Not implemented");
    }

    CStr CServer::GetPrototypeFullName(int)
    {
        throw std::logic_error("Not implemented");
    }

    CStr CServer::GetPrototypeFullName(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::Update(float)
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::LoadGlobalMapFromRawFile(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::ClearOnce()
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::EndCinematic()
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::GetControlData()
    {
        throw std::logic_error("Not implemented");
    }

    unsigned CServer::GetGlobalMapValue(long, long)
    {
        throw std::logic_error("Not implemented");
    }

    int CServer::GetPathFindQuant()
    {
        throw std::logic_error("Not implemented");
    }

    CStr CServer::GetFullNameByObjID(int)
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::Clear()
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::ResetCinematicObjects()
    {
        throw std::logic_error("Not implemented");
    }

    PrototypeInfo const* CServer::GetPrototypeInfo(int)
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::InitOnce()
    {
        throw std::logic_error("Not implemented");
    }

    eTolerance CServer::CheckTolerance(int, int)
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::LoadPrototypeNamesFromXML(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::LoadGlobalPropertiesFromXML(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    AffixManager* CServer::GetAffixManager() const
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::StartCinematic()
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::Init(m3d::CWorld*)
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::PutGameData()
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::HandleCommand(int, m3d::CConsoleParams const&)
    {
        throw std::logic_error("Not implemented");
    }

    CServer::~CServer()
    {
        throw std::logic_error("Not implemented");
    }

    bool CServer::HandleCVar(m3d::CVar const*, m3d::CConsoleParams const&)
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::SaveVisitedMap(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::_PostProcessConsoleCommands()
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::_RegisterConsoleCommands()
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::RestorePrevCinematicState()
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::SavePrevCinematicState()
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::_SetLevel(m3d::Level*)
    {
        throw std::logic_error("Not implemented");
    }
}
