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
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

int SavesManager::QuickLoad()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr SavesManager::GetNewSaveDefaultName() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* SavesManager::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

int SavesManager::AutoSave(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* SavesManager::GetBaseClass()
{
    return RT_CLASS_LOCAL(Object);
}

m3d::rend::TexHandle SavesManager::GetCurGameScreenshot() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr SavesManager::GetNewSaveFolderName() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* SavesManager::CreateObject()
{
    return new SavesManager;
}

int SavesManager::LoadGame(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

SavesManager::SaveInfo const* SavesManager::GetSaveInfoByFolderName(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SavesManager::SaveGame(CStr const&, CStr const&, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* SavesManager::GetClass() const
{
    return RT_CLASS_LOCAL(SavesManager);
}

int SavesManager::GetSaveFolderNames(retruxx::vector<CStr>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SavesManager::DeleteSaveGame(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr SavesManager::GetNewAutoSaveFolderName() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr SavesManager::GetNewQuickSaveFolderName() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SavesManager::SaveScreenshot(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr SavesManager::GetFirstUsedSaveFolderName(retruxx::vector<CStr> const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SavesManager::GetSaveFoldersByPattern(CStr const&, retruxx::vector<CStr>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr SavesManager::GetLastUsedSaveFolderName(retruxx::vector<CStr> const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr SavesManager::GetAutoSaveName(CStr const&, CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr SavesManager::GetQuickSaveName(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

_FILETIME SavesManager::GetSaveFileModifyTime(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SavesManager::SaveInfos(CStr const&, CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SavesManager::MakeCurGameScreenshot()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SavesManager::LastLoad()
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

int SavesManager::QuickSave()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SavesManager::Init()
{
    // TODO: implement SavesManager::Init
    //RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

SavesManager::ConstantSaveInfo const& SavesManager::GetConstatntSaveInfo() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SavesManager::Clear()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SavesManager::LoadInfos()
{
    // TODO: implement SavesManager::LoadInfos
    //RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

SavesManager::SavesManager()
{
}

SavesManager::SavesManager(SavesManager const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SavesManager::CheckAndHandleDelayedQuickSave(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SavesManager::OnCurProfileChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
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
            QuickSave();
        }
    }
}

int SavesManager::AddSaveInfo(SaveInfo*)
{
    RETRUXX_NOT_IMPLEMENTED;
}
