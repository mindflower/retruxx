#include "savesmanager.h"
#include <stdexcept>

#include "guihelper.h"
#include "m3dapp.h"
#include "core/log.h"
#include "game/m3dgame.h"
#include "game/profile.h"
#include <core/timer.h>
#include <core/ini.h>
#include <core/ref_ptr.h>
#include <core/scoped_ptr.h>
#include <file/fileserver.h>
#include <file/filestream.h>
#include "game/uimisc/levelinfo.h"
#include "server/event.h"
#include "server/objects/base/objcontainer.h"
#include <algorithm>
#include <cstring>

RT_CLASS_EXPORT_METHOD_DEFINE(SavesManager, AutoSave)
{
    // RVA 0x57B9F0
    auto* savesManager = (SavesManager*)context->asObject(0, "SavesManager");
    CStr const saveName = context->asString(1);
    context->pushInt(savesManager->AutoSave(saveName));
    return 1;
}

RT_CLASS_EXPORTS_BEGIN(SavesManager)
RT_CLASS_EXPORT(SavesManager, m3d::METHOD, AutoSave, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SavesManager);

SavesManager::ConstantSaveInfo::ConstantSaveInfo()
{
}

SavesManager::SaveInfo::SaveInfo()
{
    // RVA 0x57BCD0
    m_saveModifyLocalTime.dwLowDateTime = 0;
    m_saveModifyLocalTime.dwHighDateTime = 0;
}

SavesManager::~SavesManager()
{
    // RVA 0x57BF40
    Clear();
    M3D_RENDERER->ReleaseTexture(m_curGameScreenshot);
}

int SavesManager::QuickLoad()
{
    // RVA 0x57ED30 - load the most recent quick save.
    retruxx::vector<CStr> quickSaveFolders;
    GetSaveFoldersByPattern(m_constantSaveInfo.m_quickSavePrefix + CStr("*.*"), quickSaveFolders);
    CStr const lastQuickSaveFolderName = GetLastUsedSaveFolderName(quickSaveFolders);
    if (lastQuickSaveFolderName.empty())
    {
        return 0;
    }
    return LoadGame(lastQuickSaveFolderName);
}

CStr SavesManager::GetNewSaveDefaultName() const
{
    // RVA 0x57E560 - the current level's full name.
    CStr const name = help::GetCurrentLevelName();
    auto* levelInfoManager = M3D_APP->m_pInterfaceManager->GetLevelInfoManager();
    if (auto const* levelInfo = levelInfoManager->GetLevelInfoById(levelInfoManager->GetLevelInfoId(name)))
    {
        return levelInfo->GetFullName();
    }
    return name;
}

m3d::Object* SavesManager::Clone()
{
    // RVA 0x57B9C0
    return new SavesManager(*this);
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

CStr SavesManager::GetSaveFolderPathByFolderName(CStr const& folderName) const
{
    // RVA 0x5806E0
    auto const* profile = M3D_APP->GetProfileManager()->GetCurProfile();
    if (!profile)
    {
        return CStr();
    }
    return profile->GetFolder() + CStr("\\") + m_constantSaveInfo.m_saveFolderName + CStr("\\") + folderName;
}

int SavesManager::AutoSave(CStr const& saveName)
{
    // RVA 0x57EB30
    CStr const folderName = GetNewAutoSaveFolderName();
    if (folderName.empty())
    {
        M3D_LOG_INFO("SavesManager::AutoSave error - empty folder name");
        return 0;
    }
    CStr const name = GetAutoSaveName(folderName, saveName);
    if (name.empty())
    {
        M3D_LOG_INFO("SavesManager::AutoSave error - empty save name");
        return 0;
    }
    return MakeCurGameScreenshot() & SaveGame(name, folderName, true);
}

m3d::Class* SavesManager::GetBaseClass()
{
    return RT_CLASS_LOCAL(Object);
}

m3d::rend::TexHandle SavesManager::GetCurGameScreenshot() const
{
    // RVA 0x5806D0
    return m_curGameScreenshot;
}

CStr SavesManager::GetNewSaveFolderName() const
{
    // RVA 0x57F0D0 - the first free "%08d" folder from the profile's counter.
    auto const* profile = M3D_APP->GetProfileManager()->GetCurProfile();
    if (!profile)
    {
        return CStr();
    }

    CStr const savesDirPath = profile->GetFolder() + CStr("\\") + m_constantSaveInfo.m_saveFolderName;
    retruxx::vector<CStr> saveDirs;
    auto const attr = GetFileAttributesA(savesDirPath.c_str());
    if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY))
    {
        help::GetWindowsSubDirs(savesDirPath, saveDirs, "*.*");
    }

    m3d::AIParam param;
    if (!profile->GetParam(PP_NEXT_SAVE_FOLDER_NUM, param))
    {
        return CStr();
    }
    int num = param.GetAsID();
    CStr newDirName;
    newDirName.format("%08d", num);
    while (std::find(saveDirs.begin(), saveDirs.end(), newDirName) != saveDirs.end())
    {
        newDirName.format("%08d", ++num);
    }
    return newDirName;
}

m3d::Object* SavesManager::CreateObject()
{
    return new SavesManager;
}

int SavesManager::LoadGame(CStr const& folderName)
{
    // RVA 0x57CE80 - a pending quick save is written out first.
    CheckAndHandleDelayedQuickSave(true);
    if (folderName.empty())
    {
        M3D_LOG_INFO("SavesManager::LoadGame error - invalid param");
        return 0;
    }

    CStr const dynamicSceneFile =
        GetSaveFolderPathByFolderName(folderName) + CStr("\\") + m_constantSaveInfo.m_mapsDirName;
    if (!M3D_APP->LoadSavedGame(dynamicSceneFile))
    {
        return 0;
    }
    if (auto const* saveInfo = GetSaveInfoByFolderName(folderName))
    {
        std::vector<m3d::AIParam> params;
        params.push_back(m3d::AIParam(saveInfo->m_saveName));
        M3D_APP->m_pInterfaceManager->AddImportantFadingMsgByStrId("GameWasLoaded", params);
    }
    return 1;
}

SavesManager::SaveInfo const* SavesManager::GetSaveInfoByFolderName(CStr const& folderName) const
{
    // RVA 0x57C120
    auto const it = m_saves.find(folderName);
    return it != m_saves.end() ? it->second : nullptr;
}

int SavesManager::SaveGame(CStr const& saveName, CStr const& folderName, bool bQuiet)
{
    // RVA 0x57C180
    if (saveName.empty())
    {
        M3D_LOG_INFO("SavesManager::SaveGame error - invalid params");
        return 0;
    }
    if (!M3D_APP->GetProfileManager()->GetCurProfile())
    {
        M3D_LOG_INFO("SavesManager::SaveGame error - invalid profile");
        return 0;
    }

    CStr const saveFolderPath = GetSaveFolderPathByFolderName(folderName);
    auto const attr = GetFileAttributesA(saveFolderPath.c_str());
    if ((attr == INVALID_FILE_ATTRIBUTES || !(attr & FILE_ATTRIBUTE_DIRECTORY)) && !help::CreateWindowsDir(saveFolderPath))
    {
        M3D_LOG_INFO("SavesManager::SaveGame error - cannot create folder " + saveFolderPath);
        return 0;
    }

    int res = 1;
    if (!SaveInfos(folderName, saveName))
    {
        M3D_LOG_INFO("SavesManager::SaveGame error - cannot save interface information");
        res = 0;
    }
    if (!SaveScreenshot(folderName))
    {
        M3D_LOG_INFO("SavesManager::SaveGame error - cannot save screenshot");
        res = 0;
    }

    CStr const dynamicSceneFile = saveFolderPath + CStr("\\") + m_constantSaveInfo.m_mapsDirName;
    if (!M3D_APP->SaveGame(dynamicSceneFile, bQuiet))
    {
        M3D_LOG_INFO("SaveWnd::OnSave error - cannot save game " + saveName);
        return 0;
    }

    std::vector<m3d::AIParam> params;
    params.push_back(m3d::AIParam(saveName));
    M3D_APP->m_pInterfaceManager->AddImportantFadingMsgByStrId("GameWasSaved", params);
    return res;
}

m3d::Class* SavesManager::GetClass() const
{
    return RT_CLASS_LOCAL(SavesManager);
}

int SavesManager::GetSaveFolderNames(retruxx::vector<CStr>& folderNames) const
{
    folderNames.clear();

    auto* curProfile = M3D_APP->GetProfileManager()->GetCurProfile();
    if (curProfile)
    {
        if (help::WindowsDirExists(curProfile->GetFolder()))
        {
            CStr savePath = curProfile->GetFolder() + "\\" + m_constantSaveInfo.m_saveFolderName;
            if (help::WindowsDirExists(savePath))
            {
                if (help::GetWindowsSubDirs(savePath, folderNames, "*.*") == 0)
                {
                    M3D_LOG_INFO("SavesManager::GetSaveFolderNames error - cannot find saves");
                    return 0;
                }
            }
            return 1;
        }
        else
        {
            M3D_LOG_INFO(
                "SavesManager::GetSaveFolderNamest error - folder for profile " + curProfile->GetName() + " not found");
            return 0;
        }
    }
    M3D_LOG_INFO("SavesManager::GetSaveFolderNames error - invalid profile");
    return 0;
}

int SavesManager::DeleteSaveGame(CStr const& folderName)
{
    // RVA 0x57E670
    CStr const folderPath = M3D_APP->GetStartupFolder() + CStr("\\") + GetSaveFolderPathByFolderName(folderName);
    if (help::DeleteWindowsDir(folderPath))
    {
        return 1;
    }
    M3D_LOG_INFO("SavesManager::DeleteSaveGame error - cannot delete game " + folderPath);
    return 0;
}

CStr SavesManager::GetNewAutoSaveFolderName() const
{
    // RVA 0x57F6A0 - the first free "auto_%08d" slot, otherwise the oldest one.
    auto const* profile = M3D_APP->GetProfileManager()->GetCurProfile();
    if (!profile)
    {
        M3D_LOG_INFO("SavesManager::GetNewAutoSaveFolderName error - invalid profile");
        return CStr();
    }

    retruxx::vector<CStr> folderNames;
    CStr const savesFolderPath = profile->GetFolder() + CStr("\\") + m_constantSaveInfo.m_saveFolderName;
    auto const attr = GetFileAttributesA(savesFolderPath.c_str());
    if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY))
    {
        help::GetWindowsSubDirs(savesFolderPath, folderNames, m_constantSaveInfo.m_autoSavePrefix + CStr("*.*"));
    }

    for (int i = 0; i < m_constantSaveInfo.m_maxAutoSavesNum; ++i)
    {
        CStr strNum;
        strNum.format("%08d", i);
        CStr const newFolderName = m_constantSaveInfo.m_autoSavePrefix + strNum;
        if (std::find(folderNames.begin(), folderNames.end(), newFolderName) == folderNames.end())
        {
            return newFolderName;
        }
    }
    return GetFirstUsedSaveFolderName(folderNames);
}

CStr SavesManager::GetNewQuickSaveFolderName() const
{
    // RVA 0x57F320 - the first free "quick_%08d" slot, otherwise the oldest one.
    auto const* profile = M3D_APP->GetProfileManager()->GetCurProfile();
    if (!profile)
    {
        M3D_LOG_INFO("SavesManager::GetNewQuickSaveFolderName error - invalid profile");
        return CStr();
    }

    retruxx::vector<CStr> folderNames;
    CStr const savesFolderPath = profile->GetFolder() + CStr("\\") + m_constantSaveInfo.m_saveFolderName;
    auto const attr = GetFileAttributesA(savesFolderPath.c_str());
    if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY))
    {
        help::GetWindowsSubDirs(savesFolderPath, folderNames, m_constantSaveInfo.m_quickSavePrefix + CStr("*.*"));
    }

    for (int i = 0; i < m_constantSaveInfo.m_maxQuickSavesNum; ++i)
    {
        CStr strNum;
        strNum.format("%08d", i);
        CStr const newFolderName = m_constantSaveInfo.m_quickSavePrefix + strNum;
        if (std::find(folderNames.begin(), folderNames.end(), newFolderName) == folderNames.end())
        {
            return newFolderName;
        }
    }
    return GetFirstUsedSaveFolderName(folderNames);
}

int SavesManager::SaveScreenshot(CStr const& folderName)
{
    // RVA 0x57CC70
    if (!m_curGameScreenshot.IsValid())
    {
        return 0;
    }
    CStr const filePath = M3D_APP->GetStartupFolder() + CStr("\\") + GetSaveFolderPathByFolderName(folderName) +
        CStr("\\") + m_constantSaveInfo.m_screenshotFileName;
    auto const attr = GetFileAttributesA(filePath.c_str());
    if (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY))
    {
        // An existing screenshot is about to be overwritten: let texture users know.
        M3D_APP->ImmediateMessage(65681, 0, 0, 0, 0, filePath, m3d::AIParam());
    }
    return M3D_RENDERER->SaveTextureToTgaFile(m_curGameScreenshot, filePath.c_str());
}

CStr SavesManager::GetFirstUsedSaveFolderName(retruxx::vector<CStr> const& folderNames) const
{
    // RVA 0x57FCA0 - by SaveInfo.xml modification time; ties go to the later folder.
    _FILETIME minFileTime{0, 0};
    CStr result;
    for (size_t i = 0; i < folderNames.size(); ++i)
    {
        CStr const infoFilePath =
            GetSaveFolderPathByFolderName(folderNames[i]) + CStr("\\") + m_constantSaveInfo.m_infoFileName;
        auto const fileTime = GetSaveFileModifyTime(infoFilePath);
        if (i == 0)
        {
            minFileTime = fileTime;
            result = folderNames[0];
        }
        else if (CompareFileTime(&minFileTime, &fileTime) >= 0)
        {
            minFileTime = fileTime;
            result = folderNames[i];
        }
    }
    return result;
}

void SavesManager::GetSaveFoldersByPattern(CStr const& pattern, retruxx::vector<CStr>& saveFolders) const
{
    // RVA 0x57EF80
    saveFolders.clear();
    auto const* profile = M3D_APP->GetProfileManager()->GetCurProfile();
    if (!profile)
    {
        M3D_LOG_INFO("SavesManager::GetSaveFoldersByPattern error - invalid profile");
        return;
    }
    CStr const savePath = profile->GetFolder() + CStr("\\") + m_constantSaveInfo.m_saveFolderName;
    auto const attr = GetFileAttributesA(savePath.c_str());
    if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY))
    {
        help::GetWindowsSubDirs(savePath, saveFolders, pattern);
    }
}

CStr SavesManager::GetLastUsedSaveFolderName(retruxx::vector<CStr> const& folderNames) const
{
    // RVA 0x57FA20 - by SaveInfo.xml modification time; ties go to the later folder.
    _FILETIME maxFileTime{0, 0};
    CStr result;
    for (size_t i = 0; i < folderNames.size(); ++i)
    {
        CStr const infoFilePath =
            GetSaveFolderPathByFolderName(folderNames[i]) + CStr("\\") + m_constantSaveInfo.m_infoFileName;
        auto const fileTime = GetSaveFileModifyTime(infoFilePath);
        if (CompareFileTime(&maxFileTime, &fileTime) <= 0)
        {
            maxFileTime = fileTime;
            result = folderNames[i];
        }
    }
    return result;
}

CStr SavesManager::GetAutoSaveName(CStr const& folderName, CStr const& saveName) const
{
    // RVA 0x580280 - "<save name or level name> <AutoSave> [<n>]".
    if (folderName.empty())
    {
        M3D_LOG_INFO("SavesManager::GetAutoSaveName error - empty folder name");
        return CStr();
    }
    char const* found = strstr(folderName.c_str(), m_constantSaveInfo.m_autoSavePrefix.c_str());
    if (!found)
    {
        M3D_LOG_INFO("SavesManager::GetAutoSaveName error - invalid save folder name" + folderName);
        return CStr();
    }
    size_t const pos = found - folderName.c_str();

    // NOTE: unlike GetQuickSaveName the prefix is not skipped, so atoi() always
    // sees "auto_..." and yields 0. Only matters with more than one auto save.
    CStr const name = folderName.substr(pos, static_cast<size_t>(-1));
    int const num = m_constantSaveInfo.m_maxAutoSavesNum > 1 ? atoi(name.c_str()) + 1 : -1;
    CStr const strNum = num == -1 ? CStr("") : CStr(num);
    CStr const baseName = saveName.empty() ? GetNewSaveDefaultName() : saveName;
    return baseName + CStr(" ") + M3D_APP->GetStringByStringId0(m_constantSaveInfo.m_autoSaveNameStrId) + CStr(" ") +
        strNum;
}

CStr SavesManager::GetQuickSaveName(CStr const& folderName) const
{
    // RVA 0x57FF20 - "<level name> <QuickSave> <n>".
    if (folderName.empty())
    {
        M3D_LOG_INFO("SavesManager::GetQuickSaveName error - empty folder name");
        return CStr();
    }
    char const* found = strstr(folderName.c_str(), m_constantSaveInfo.m_quickSavePrefix.c_str());
    if (!found)
    {
        M3D_LOG_INFO("SavesManager::GetQuickSaveName error - invalid save folder name" + folderName);
        return CStr();
    }
    size_t const pos = found - folderName.c_str();
    CStr const saveName = folderName.substr(pos + m_constantSaveInfo.m_quickSavePrefix.length(), static_cast<size_t>(-1));
    int const num = atoi(saveName.c_str());
    return GetNewSaveDefaultName() + CStr(" ") + M3D_APP->GetStringByStringId0(m_constantSaveInfo.m_quickSaveNameStrId) +
        CStr(" ") + CStr(num + 1);
}

_FILETIME SavesManager::GetSaveFileModifyTime(CStr const& saveFilePath) const
{
    // RVA 0x57E4E0 - local modification time, or zero.
    _FILETIME localFileTime{0, 0};
    WIN32_FIND_DATAA findFileData;
    auto const hFind = FindFirstFileA(saveFilePath.c_str(), &findFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        return _FILETIME{0, 0};
    }
    FindClose(hFind);
    if (!FileTimeToLocalFileTime(&findFileData.ftLastWriteTime, &localFileTime))
    {
        return _FILETIME{0, 0};
    }
    return localFileTime;
}

int SavesManager::SaveInfos(CStr const& folderName, CStr const& saveName)
{
    // RVA 0x57C5A0 - writes SaveInfo.xml and records the save.
    CStr const filePath =
        GetSaveFolderPathByFolderName(folderName) + CStr("\\") + m_constantSaveInfo.m_infoFileName;
    scoped_ptr stream = m3d::g_Kernel->GetFileServer().CreateFileStream();
    if (!stream->Open(filePath.c_str(), m3d::fs::IStream::OPEN_WRITE))
    {
        M3D_LOG_INFO("SavesManager::SaveInfo error - can't open file " + filePath + " for write.");
        return 0;
    }

    ref_ptr xmlFile = m3d::g_Kernel->CreateXmlFile();
    ref_ptr rootNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "SaveInfo");
    xmlFile->AddChild(rootNode);
    rootNode->SetAttribute("Name", saveName.c_str());
    CStr const levelName = help::GetCurrentLevelName();
    rootNode->SetAttribute("LevelName", levelName.c_str());
    ref_ptr gtNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "GameTime");
    rootNode->AddChild(gtNode);
    ai::theObjects->m_GameTime.SaveToXML(xmlFile, gtNode);
    xmlFile->Write(*stream);
    stream->Close();

    auto* saveInfo = new SaveInfo;
    saveInfo->m_folderName = folderName;
    saveInfo->m_saveName = saveName;
    saveInfo->m_gameTime = ai::theObjects->m_GameTime.asAIParam();
    saveInfo->m_levelName = levelName;
    saveInfo->m_saveModifyLocalTime = GetSaveFileModifyTime(filePath);
    if (!AddSaveInfo(saveInfo))
    {
        delete saveInfo;
        return 0;
    }
    return 1;
}

int SavesManager::MakeCurGameScreenshot()
{
    return M3D_RENDERER->AddTextureFromBackBuffer(m_curGameScreenshot);
}

int SavesManager::LastLoad()
{
    // RVA 0x57EE90 - load the most recent save of any kind.
    retruxx::vector<CStr> saveFolders;
    GetSaveFoldersByPattern("*.*", saveFolders);
    CStr const lastSaveFolderName = GetLastUsedSaveFolderName(saveFolders);
    if (lastSaveFolderName.empty())
    {
        return 0;
    }
    return LoadGame(lastSaveFolderName);
}

int SavesManager::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x57D150
    switch (dataType)
    {
    case IE_IMP_IM_QUICK_SAVE:
        // Deferred by a frame so the fading message is on screen in the save.
        if (ai::theObjects->IsSaveAllowed())
        {
            m_bDelayedQuickSave = true;
            m_delayedQuickSaveFrame = M3D_KERNEL->GetTimer().GetCurFrame();
            M3D_APP->m_pInterfaceManager->AddImportantFadingMsgByStrId("GameSaving...", {});
        }
        else
        {
            M3D_APP->m_pInterfaceManager->AddImportantFadingMsgByStrId("fm_save_is_not_allowed", {});
        }
        break;
    case IE_IMP_IM_QUICK_LOAD:
        QuickLoad();
        break;
    case IE_EV_UM_CUR_PROFILE_CHANGED:
        LoadInfos();
        break;
    case IE_EV_UM_LOAD_LAST_GAME:
        LastLoad();
        break;
    case IE_CUST_NEW_FRAME_FORCE:
        OnNewFrameForce();
        break;
    default:
        break;
    }
    return 1;
}

int SavesManager::QuickSave()
{
    // RVA 0x57E940
    CStr const folderName = GetNewQuickSaveFolderName();
    if (folderName.empty())
    {
        M3D_LOG_INFO("SavesManager::QuickSave error - empty folder name");
        return 0;
    }
    CStr const saveName = GetQuickSaveName(folderName);
    if (saveName.empty())
    {
        M3D_LOG_INFO("SavesManager::QuickSave error - empty save name");
        return 0;
    }
    return MakeCurGameScreenshot() & SaveGame(saveName, folderName, true);
}

int SavesManager::Init()
{
    m_curGameScreenshot = M3D_RENDERER->AddDynamicTexture(
        "$GameScreenshot", m_constantSaveInfo.m_screenshotSz.x, m_constantSaveInfo.m_screenshotSz.y, 1);

    if (!m_curGameScreenshot.IsValid())
    {
        return 0;
    }

    M3D_RENDERER->SetTextureParameter(m_curGameScreenshot, m3d::rend::TexParam::TM_WRAP_S, 3u);
    M3D_RENDERER->SetTextureParameter(m_curGameScreenshot, m3d::rend::TexParam::TM_WRAP_T, 3u);
    return 1;
}

SavesManager::ConstantSaveInfo const& SavesManager::GetConstatntSaveInfo() const
{
    // RVA 0x57C110
    return m_constantSaveInfo;
}

void SavesManager::Clear()
{
    // RVA 0x57BFE0
    for (auto& [folderName, saveInfo] : m_saves)
    {
        delete saveInfo;
        saveInfo = nullptr;
    }
    m_saves.clear();
    m_bDelayedQuickSave = false;
    m_delayedQuickSaveFrame = 0;
}

int SavesManager::LoadInfos()
{
    // RVA 0x57D310 - rebuild the save list from every save folder's SaveInfo.xml.
    Clear();
    retruxx::vector<CStr> folderNames;
    if (!GetSaveFolderNames(folderNames))
    {
        return 0;
    }

    // NOTE: only a missing info file makes the result fail; every other problem
    // just skips that save, as in the original.
    int res = 1;
    for (auto const& folderName : folderNames)
    {
        CStr const infoFilePath =
            GetSaveFolderPathByFolderName(folderName) + CStr("\\") + m_constantSaveInfo.m_infoFileName;
        auto const attr = GetFileAttributesA(infoFilePath.c_str());
        if (attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY))
        {
            res = 0;
            continue;
        }

        scoped_ptr stream = m3d::g_Kernel->GetFileServer().CreateFileStream();
        if (!stream->Open(infoFilePath.c_str(), m3d::fs::IStream::OPEN_READ))
        {
            M3D_LOG_INFO("SavesManager::LoadInfos error - can't open file " + infoFilePath + " for read.");
            continue;
        }
        ref_ptr xmlFile = m3d::g_Kernel->CreateXmlFile();
        if (!xmlFile->Read(*stream))
        {
            M3D_LOG_INFO(
                "SavesManager::LoadInfos error - cannot parse " + infoFilePath + " (" + CStr(xmlFile->GetError()) + ") ");
            stream->Close();
            continue;
        }
        stream->Close();

        ref_ptr rootNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        xmlFile->GetFirstChild(rootNode, "SaveInfo");
        if (rootNode->IsEmpty())
        {
            M3D_LOG_INFO("SavesManager::LoadInfos error - cannot find root node \"SaveInfo\" in file " + infoFilePath);
            continue;
        }

        CStr saveName;
        m3d::SafeStrAttrib(saveName, rootNode, "Name");
        if (saveName.empty())
        {
            M3D_LOG_INFO("SavesManager::LoadInfos error - empty save name in file " + infoFilePath);
            continue;
        }
        CStr levelName;
        m3d::SafeStrAttrib(levelName, rootNode, "LevelName");

        ref_ptr gtNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        rootNode->GetFirstChild(gtNode, "GameTime");
        if (gtNode->IsEmpty())
        {
            M3D_LOG_INFO("SavesManager::LoadInfos error - cannot find GameTime in file " + infoFilePath);
            continue;
        }
        ai::GameTime gameTime;
        gameTime.LoadFromXML(xmlFile, gtNode);

        auto const modifyTime = GetSaveFileModifyTime(infoFilePath);
        if (!modifyTime.dwHighDateTime || !modifyTime.dwLowDateTime)
        {
            M3D_LOG_INFO("SavesManager::LoadInfos error - invalid file modification time for file " + infoFilePath);
            continue;
        }

        auto* saveInfo = new SaveInfo;
        saveInfo->m_folderName = folderName;
        saveInfo->m_saveName = saveName;
        saveInfo->m_gameTime = gameTime.asAIParam();
        saveInfo->m_levelName = levelName;
        saveInfo->m_saveModifyLocalTime = modifyTime;
        if (!AddSaveInfo(saveInfo))
        {
            delete saveInfo;
        }
    }
    return res;
}

SavesManager::SavesManager() = default;

SavesManager::SavesManager(SavesManager const&) :
    m3d::Object()
{
    // RVA 0x57BEE0 - nothing is copied; the clone starts with no saves.
}

void SavesManager::CheckAndHandleDelayedQuickSave(bool bIgnoreFrame)
{
    // RVA 0x5809D0 - run a pending quick save once a full frame has passed.
    if (!m_bDelayedQuickSave)
    {
        return;
    }
    auto const curFrame = M3D_KERNEL->GetTimer().GetCurFrame();
    if (bIgnoreFrame || (curFrame != m_delayedQuickSaveFrame && curFrame - m_delayedQuickSaveFrame != 1))
    {
        m_bDelayedQuickSave = false;
        m_delayedQuickSaveFrame = 0;
        QuickSave();
    }
}

void SavesManager::OnCurProfileChanged()
{
    // RVA 0x57D300
    LoadInfos();
}

void SavesManager::OnNewFrameForce()
{
    if (m_bDelayedQuickSave)
    {
        auto curFrame = M3D_KERNEL->GetTimer().GetCurFrame();
        ;
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

int SavesManager::AddSaveInfo(SaveInfo* si)
{
    // RVA 0x57DFA0 - replaces any earlier info for the same folder.
    if (!si || si->m_folderName.empty())
    {
        M3D_LOG_INFO("SavesManager::AddSaveInfo error - invalid param");
        return 0;
    }
    auto const it = m_saves.find(si->m_folderName);
    if (it != m_saves.end())
    {
        delete it->second;
        it->second = nullptr;
        m_saves.erase(it);
    }
    m_saves.insert({si->m_folderName, si});
    return 1;
}
