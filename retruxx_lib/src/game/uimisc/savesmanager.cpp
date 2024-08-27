#include "savesmanager.h"
#include <stdexcept>

#include "guihelper.h"
#include "m3dapp.h"
#include "core/log.h"
#include "game/m3dgame.h"
#include "game/profile.h"

RT_CLASS_EXPORT_METHOD_DEFINE(SavesManager, AutoSave)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORTS_BEGIN(SavesManager)
	RT_CLASS_EXPORT(SavesManager, m3d::METHOD, AutoSave, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SavesManager);

SavesManager::ConstantSaveInfo::ConstantSaveInfo()
{
}

SavesManager::~SavesManager()
{
    throw std::logic_error("Not implemented");
}

int SavesManager::QuickLoad()
{
    throw std::logic_error("Not implemented");
}

CStr SavesManager::GetNewSaveDefaultName() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* SavesManager::Clone()
{
    throw std::logic_error("Not implemented");
}

CStr SavesManager::GetPathForTemporaryMaps() const
{
    auto app = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
    auto profile = app->GetProfileManager()->GetCurProfile();
    if (profile)
    {
        auto path = profile->GetFolder() + "\\__temp_maps\\";
        auto attr = GetFileAttributesA(path.c_str());
        if (attr != -1 && (attr & 0x10) != 0 || help::CreateWindowsDir(path))
        {
            return path;
        }
    	M3D_LOG_INFO("SavesManager::GetPathForTemporaryMaps error - cannot create folder " + path);
    }
    return {};
}

CStr SavesManager::GetSaveFolderPathByFolderName(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

int SavesManager::AutoSave(CStr const&)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* SavesManager::GetBaseClass()
{
    return RT_CLASS_LOCAL(Object);
}

m3d::rend::TexHandle SavesManager::GetCurGameScreenshot() const
{
    throw std::logic_error("Not implemented");
}

CStr SavesManager::GetNewSaveFolderName() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* SavesManager::CreateObject()
{
    return new SavesManager;
}

int SavesManager::LoadGame(CStr const&)
{
    throw std::logic_error("Not implemented");
}

SavesManager::SaveInfo const* SavesManager::GetSaveInfoByFolderName(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

int SavesManager::SaveGame(CStr const&, CStr const&, bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* SavesManager::GetClass() const
{
    throw std::logic_error("Not implemented");
}

int SavesManager::GetSaveFolderNames(std::vector<CStr>&) const
{
    throw std::logic_error("Not implemented");
}

int SavesManager::DeleteSaveGame(CStr const&)
{
    throw std::logic_error("Not implemented");
}

CStr SavesManager::GetNewAutoSaveFolderName() const
{
    throw std::logic_error("Not implemented");
}

CStr SavesManager::GetNewQuickSaveFolderName() const
{
    throw std::logic_error("Not implemented");
}

int SavesManager::SaveScreenshot(CStr const&)
{
    throw std::logic_error("Not implemented");
}

CStr SavesManager::GetFirstUsedSaveFolderName(std::vector<CStr> const&) const
{
    throw std::logic_error("Not implemented");
}

void SavesManager::GetSaveFoldersByPattern(CStr const&, std::vector<CStr>&) const
{
    throw std::logic_error("Not implemented");
}

CStr SavesManager::GetLastUsedSaveFolderName(std::vector<CStr> const&) const
{
    throw std::logic_error("Not implemented");
}

CStr SavesManager::GetAutoSaveName(CStr const&, CStr const&) const
{
    throw std::logic_error("Not implemented");
}

CStr SavesManager::GetQuickSaveName(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

_FILETIME SavesManager::GetSaveFileModifyTime(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

int SavesManager::SaveInfos(CStr const&, CStr const&)
{
    throw std::logic_error("Not implemented");
}

int SavesManager::MakeCurGameScreenshot()
{
    throw std::logic_error("Not implemented");
}

int SavesManager::LastLoad()
{
    throw std::logic_error("Not implemented");
}

int SavesManager::GameDataUpdate(void* data, int dataType)
{
    switch (dataType)
    {
    case 40:
    {
        LoadInfos();
        return 1;
    }
    }
    throw std::logic_error("Not implemented");
}

int SavesManager::QuickSave()
{
    throw std::logic_error("Not implemented");
}

int SavesManager::Init()
{
    throw std::logic_error("Not implemented");
}

SavesManager::ConstantSaveInfo const& SavesManager::GetConstatntSaveInfo() const
{
    throw std::logic_error("Not implemented");
}

void SavesManager::Clear()
{
    throw std::logic_error("Not implemented");
}

int SavesManager::LoadInfos()
{
    //TODO: implement SavesManager::LoadInfos
    return 1;
    //throw std::logic_error("Not implemented");
}

SavesManager::SavesManager()
{
}

SavesManager::SavesManager(SavesManager const&)
{
    throw std::logic_error("Not implemented");
}

void SavesManager::CheckAndHandleDelayedQuickSave(bool)
{
    throw std::logic_error("Not implemented");
}

void SavesManager::OnCurProfileChanged()
{
    throw std::logic_error("Not implemented");
}

void SavesManager::OnNewFrameForce()
{
    throw std::logic_error("Not implemented");
}

int SavesManager::AddSaveInfo(SaveInfo*)
{
    throw std::logic_error("Not implemented");
}
