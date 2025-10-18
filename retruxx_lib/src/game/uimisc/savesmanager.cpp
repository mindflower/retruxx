#include "savesmanager.h"
#include <stdexcept>

#include "guihelper.h"
#include "m3dapp.h"
#include "core/log.h"
#include "game/m3dgame.h"
#include "game/profile.h"
#include <core/timer.h>

RT_CLASS_EXPORT_METHOD_DEFINE(SavesManager, AutoSave)
{
    throw retruxx::logic_error("Not implemented");
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
    throw retruxx::logic_error("Not implemented");
}

int SavesManager::QuickLoad()
{
    throw retruxx::logic_error("Not implemented");
}

CStr SavesManager::GetNewSaveDefaultName() const
{
    throw retruxx::logic_error("Not implemented");
}

m3d::Object* SavesManager::Clone()
{
    throw retruxx::logic_error("Not implemented");
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
    throw retruxx::logic_error("Not implemented");
}

int SavesManager::AutoSave(CStr const&)
{
    throw retruxx::logic_error("Not implemented");
}

m3d::Class* SavesManager::GetBaseClass()
{
    return RT_CLASS_LOCAL(Object);
}

m3d::rend::TexHandle SavesManager::GetCurGameScreenshot() const
{
    throw retruxx::logic_error("Not implemented");
}

CStr SavesManager::GetNewSaveFolderName() const
{
    throw retruxx::logic_error("Not implemented");
}

m3d::Object* SavesManager::CreateObject()
{
    return new SavesManager;
}

int SavesManager::LoadGame(CStr const&)
{
    throw retruxx::logic_error("Not implemented");
}

SavesManager::SaveInfo const* SavesManager::GetSaveInfoByFolderName(CStr const&) const
{
    throw retruxx::logic_error("Not implemented");
}

int SavesManager::SaveGame(CStr const&, CStr const&, bool)
{
    throw retruxx::logic_error("Not implemented");
}

m3d::Class* SavesManager::GetClass() const
{
    throw retruxx::logic_error("Not implemented");
}

int SavesManager::GetSaveFolderNames(retruxx::vector<CStr>&) const
{
    throw retruxx::logic_error("Not implemented");
}

int SavesManager::DeleteSaveGame(CStr const&)
{
    throw retruxx::logic_error("Not implemented");
}

CStr SavesManager::GetNewAutoSaveFolderName() const
{
    throw retruxx::logic_error("Not implemented");
}

CStr SavesManager::GetNewQuickSaveFolderName() const
{
    throw retruxx::logic_error("Not implemented");
}

int SavesManager::SaveScreenshot(CStr const&)
{
    throw retruxx::logic_error("Not implemented");
}

CStr SavesManager::GetFirstUsedSaveFolderName(retruxx::vector<CStr> const&) const
{
    throw retruxx::logic_error("Not implemented");
}

void SavesManager::GetSaveFoldersByPattern(CStr const&, retruxx::vector<CStr>&) const
{
    throw retruxx::logic_error("Not implemented");
}

CStr SavesManager::GetLastUsedSaveFolderName(retruxx::vector<CStr> const&) const
{
    throw retruxx::logic_error("Not implemented");
}

CStr SavesManager::GetAutoSaveName(CStr const&, CStr const&) const
{
    throw retruxx::logic_error("Not implemented");
}

CStr SavesManager::GetQuickSaveName(CStr const&) const
{
    throw retruxx::logic_error("Not implemented");
}

_FILETIME SavesManager::GetSaveFileModifyTime(CStr const&) const
{
    throw retruxx::logic_error("Not implemented");
}

int SavesManager::SaveInfos(CStr const&, CStr const&)
{
    throw retruxx::logic_error("Not implemented");
}

int SavesManager::MakeCurGameScreenshot()
{
    throw retruxx::logic_error("Not implemented");
}

int SavesManager::LastLoad()
{
    throw retruxx::logic_error("Not implemented");
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
    case 90:
    {
        OnNewFrameForce();
        return 1;
    }
    }
    throw retruxx::logic_error("Not implemented");
}

int SavesManager::QuickSave()
{
    throw retruxx::logic_error("Not implemented");
}

int SavesManager::Init()
{
    throw retruxx::logic_error("Not implemented");
}

SavesManager::ConstantSaveInfo const& SavesManager::GetConstatntSaveInfo() const
{
    throw retruxx::logic_error("Not implemented");
}

void SavesManager::Clear()
{
    throw retruxx::logic_error("Not implemented");
}

int SavesManager::LoadInfos()
{
    // TODO: implement SavesManager::LoadInfos
    //throw retruxx::logic_error("Not implemented");
    return 0;
}

SavesManager::SavesManager()
{
}

SavesManager::SavesManager(SavesManager const&)
{
    throw retruxx::logic_error("Not implemented");
}

void SavesManager::CheckAndHandleDelayedQuickSave(bool)
{
    throw retruxx::logic_error("Not implemented");
}

void SavesManager::OnCurProfileChanged()
{
    throw retruxx::logic_error("Not implemented");
}

void SavesManager::OnNewFrameForce()
{
    if (m_bDelayedQuickSave)
    {
        auto curFrame = M3D_KERNEL->GetTimer().GetCurFrame();;
        auto v4 = curFrame == m_delayedQuickSaveFrame;
        auto v3 = curFrame - m_delayedQuickSaveFrame;
        if (!v4 && v3 != 1)
        {
            m_bDelayedQuickSave = 0;
            m_delayedQuickSaveFrame = 0;
            SavesManager::QuickSave();
        }
    }
}

int SavesManager::AddSaveInfo(SaveInfo*)
{
    throw retruxx::logic_error("Not implemented");
}
