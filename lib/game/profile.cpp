#include "profile.h"
#include <stdexcept>
#include <core/kernel.h>
#include <core/aiparam.h>
#include <core/log.h>
#include "core/ini.h"

#include "config.h"
#include "m3dapp.h"
#include "m3dgame.h"
#include "core/scoped_ptr.h"
#include "core/console/console.h"
#include "file/fileserver.h"
#include "file/filestream.h"
#include "uimisc/guihelper.h"
#include "uimanager/uidefs.h"

RT_CLASS_EXPORTS_BEGIN(Profile)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(Profile);

namespace
{
	struct Name2Id
	{
        CStr m_name;
        int m_id;
	};

    Name2Id l_paramName2Id[] = {
        {"PP_SAVE_SORT_ARG", 0},
        {"PP_SAVE_SORT_DIR", 1},
        {"PP_NEXT_SAVE_FOLDER_NUM", 2},
        {"PP_MOTION_BLUR", 11},
        {"PP_MOTION_BLUR_ALPHA", 12},
        {"PP_BLOOM", 13},
        {"PP_QUESTS_FILTER", 3},
        {"PP_MOUSE_SENSITIVITY", 4},
        {"PP_MOUSE_YAXIS_FLIP", 5},
        {"PP_MOUSE_XAXIS_FLIP", 6},
        {"PP_WARELIST_MODE", 7},
        {"PP_INPUT_LANGUAGE", 8},
        {"PP_NUM_RADIO_REPLIES_COEFF", 9},
        {"PP_MINIMAP_ENABLE_OBJECT_NAMES", 10},
        {"PP_AUTOHELP_ENABLED", 14},
        {"PP_DIFFICULTY_LEVEL", 15},
        {"PP_HELP_ID_MAIN_GAME_INTERFACE", 16},
        {"PP_HELP_ID_INVENTORY", 17},
        {"PP_HELP_ID_VEHICLE_CHARACTERISTICS", 18},
        {"PP_HELP_ID_QUESTLOG_QUESTS", 19},
		{"PP_HELP_ID_QUESTLOG_QUEST_ITEMS", 20},
        {"PP_HELP_ID_LOCAL_MAP", 21},
        {"PP_HELP_ID_GLOBAL_MAP", 22},
        {"PP_HELP_ID_HISTORY", 23},
        {"PP_HELP_ID_BOOKS", 24},
        {"PP_HELP_ID_RELATIONS", 25},
        {"PP_HELP_ID_STATS", 26},
        {"PP_HELP_ID_ENCYCLOPAEDIA", 27},
        {"PP_HELP_ID_SHOP_GUNS", 28},
        {"PP_HELP_ID_SHOP_GOODS", 29},
		{"PP_HELP_ID_WORKSHOP", 30},
        {"PP_HELP_ID_WORKSHOP_CABINS", 31},
        {"PP_HELP_ID_WORKSHOP_BASKETS", 32},
        {"PP_HELP_ID_WORKSHOP_VEHICLE", 33},
        {"PP_HELP_ID_WORKSHOP_SKIN", 34},
        {"PP_HELP_ID_WORKSHOP_REFUEL", 35},
        {"PP_HELP_ID_WORKSHOP_REPAIR", 36},
        {"PP_HELP_ID_WORKSHOP_RECHARGE", 37},
        {"PP_HELP_ID_BUY_CABIN", 38},
        {"PP_HELP_ID_BUY_BASKET", 39},
		{"PP_HELP_ID_BUY_VEHICLE", 40},
        {"PP_HELP_ID_TOWN", 41},
        {"PP_HELP_ID_BAR", 42},
        {"PP_HELP_ID_TALK_WITH_NPC", 43},
    };
}

int Profile::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
{
    if (xmlFile && xmlNode)
    {
        Clear();
        SetParamsDefault();
        m3d::SafeStrAttrib(m_name, xmlNode, "Name");
        if (!m_name.empty())
        {
            int paramRes = 1;
            ref_ptr paramsNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
            xmlNode->GetFirstChild(paramsNode, "Params");
            if (!paramsNode->IsEmpty())
            {
                ref_ptr paramNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
                for (paramsNode->GetFirstChild(paramNode, "Param"); !paramNode->IsEmpty();paramNode->GetNextSibling(paramNode, "Param"))
                {
                    CStr idStr;
                    m3d::SafeStrAttrib(idStr, paramNode, "Id");
                    auto id = ParamName2Id(idStr);
                    if (id < PP_SAVE_SORT_ARG || id >= PP_INVALID)
                    {
                        M3D_LOG_INFO("Profile::LoadFromXml warning - invalid profile param id " + idStr);
                        paramRes = 0;
                    }
                    else
                    {
                        ref_ptr valueNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
                        paramNode->GetFirstChild(valueNode, "Value");
                        if (valueNode->IsEmpty())
                        {
                            M3D_LOG_INFO("Profile::LoadFromXml warning - invalid value node for param " + idStr);
                            paramRes = 0;
                        }
                        else
                        {
                            m3d::AIParam param;
                            param.LoadFromXML(xmlFile, valueNode);
                            SetParam(id, param);
                        }
                    }
                }
                if (!paramRes)
                {
                    M3D_LOG_INFO("Profile::LoadFromXml warning - profile params were loaded with errors");
                }
            }
            return 1;
        }
        else
        {
            M3D_LOG_INFO("Profile::LoadFromXml error - empty name");
            return 0;
        }
    }
	M3D_LOG_INFO("Profile::LoadFromXml error - invalid xmlFile or xmlNode");
	return 0;
}

CStr const& Profile::GetFolder() const
{
    return m_folder;
}

m3d::Class* Profile::GetClass() const
{
    return RT_CLASS_LOCAL(Profile);
}

m3d::Object* Profile::CreateObject()
{
    return new Profile;
}

void Profile::SetFolder(CStr const& folder)
{
    m_folder = folder;
}

m3d::Object* Profile::Clone()
{
    // RVA 0x40D840
    return new Profile(*this);
}

int Profile::GetParam(ProfileParam paramId, m3d::AIParam& param) const
{
    if (paramId >= PP_NUM_PROFILE_PARAMS)
    {
        M3D_LOG_INFO("Profile::GetParam error - invalid param");
        return 0;
    }
    auto const it = m_params.find(paramId);
    if (it != m_params.end())
    {
        param = it->second;
        return 1;
    }
    M3D_LOG_INFO("Profile::GetParam error - param " + CStr(paramId) + " not found");
    return 0;
}

m3d::Class* Profile::GetBaseClass()
{
    return RT_CLASS_LOCAL(Object);
}

void Profile::SetName(CStr const& name)
{
    // RVA 0x40DB30
    m_name = name;
}

int Profile::SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
{
    // RVA 0x40EA80
    if (!xmlFile || !xmlNode)
    {
        M3D_LOG_INFO("Profile::SaveToXml error - invalid params");
        return 0;
    }
    if (!IsValid())
    {
        M3D_LOG_INFO("Profile::SaveToXml - profile is invalid");
        return 0;
    }

    CStr const name = m_name;
    xmlNode->SetAttribute("Name", name.c_str());

    ref_ptr paramsNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Params");
    xmlNode->AddChild(paramsNode);
    for (auto const& param : m_params)
    {
        ref_ptr paramNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Param");
        paramsNode->AddChild(paramNode);
        paramNode->SetAttribute("Id", ParamId2Name(param.first).c_str());

        ref_ptr valueNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Value");
        paramNode->AddChild(valueNode);
        param.second.SaveToXML(xmlFile, valueNode);
    }
    return 1;
}

bool Profile::IsValid() const
{
    return !m_folder.empty() && !m_name.empty();
}

int Profile::SetParam(ProfileParam paramId, m3d::AIParam const& paramVal)
{
    if (paramId >= PP_NUM_PROFILE_PARAMS)
    {
        M3D_LOG_INFO("Profile::SetParam error - invalid param");
        return 0;
    }
    m_params[paramId] = paramVal;
    auto app = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
    if (app->GetProfileManager()->GetCurProfile() == this)
    {
        app->EnqueueMessage(65680, paramId, 0, 0, 0, {}, {});
    }
    return 1;
}

Profile::~Profile()
{
    // RVA 0x40DA80 - only the members are released.
}

CStr const& Profile::GetName() const
{
    return m_name;
}

int Profile::SetParamsDefault()
{
    auto res = 1;
    for (auto const& param : l_paramName2Id)
    {
        if (param.m_id > 43)
        {
            M3D_LOG_INFO("Profile::SetParamsDefault warning - param " + param.m_name + " has not been initialized");
            res = 0;
        }
        m3d::AIParam aiParam;
        switch (param.m_id)
        {
        case 0:
        case 1:
        case 7:
        case 10:
        case 14:
        case 15:
            aiParam = 1;
            break;
        case 2:
        case 5:
        case 6:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
        case 32:
        case 33:
        case 34:
        case 35:
        case 36:
        case 37:
        case 38:
        case 39:
        case 40:
        case 41:
        case 42:
        case 43:
            aiParam = 0;
            break;
        case 3:
            aiParam = 7;
            break;
        case 4:
            aiParam = static_cast<float>(1.0);
            break;
        case 8:
            aiParam = !m3d::g_Kernel->GetEngineCfg().m_input_defaultInputLanguageBase.GetB();
            break;
        case 9:
            aiParam = static_cast<float>(1.0);;
            break;
        case 11:
            aiParam = "no";
            break;
        case 12:
            aiParam = static_cast<float>(0.0);;
            break;
        case 13:
            aiParam = "no";
            break;
        }
        res &= SetParam(static_cast<ProfileParam>(param.m_id), aiParam);
    }
    return res;
}

void Profile::Clear()
{
    m_params.clear();
    m_name = "";
    m_folder = "";
}

Profile::Profile(Profile const&)
{
    // RVA 0x40DA20 - nothing is copied and, unlike the default constructor,
    // the parameters are not reset to their defaults either: a clone starts
    // with no name, no folder and an empty parameter map.
}

Profile::Profile()
{
    if (!SetParamsDefault())
    {
        M3D_LOG_INFO("Profile params inited with errors");
    }
}

ProfileParam Profile::ParamName2Id(CStr const& name) const
{
    for (auto const& param : l_paramName2Id)
    {
	    if (name == param.m_name)
	    {
            return static_cast<ProfileParam>(param.m_id);
	    }
    }
    return PP_NUM_PROFILE_PARAMS;
}

CStr Profile::ParamId2Name(ProfileParam id) const
{
    // RVA 0x40E510
    for (auto const& param : l_paramName2Id)
    {
        if (param.m_id == id)
        {
            return param.m_name;
        }
    }
    return {};
}

RT_CLASS_EXPORTS_BEGIN(ProfileManager)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ProfileManager);

Profile* ProfileManager::GetCurProfile() const
{
    return _GetProfileByName(m_curProfileName);
}

int ProfileManager::SetCurProfile(CStr const& profileName)
{
    if (m_curProfileName == profileName)
    {
        return 1;
    }
    auto res = 1;
    auto profile = _GetProfileByName(m_curProfileName);
    if (profile)
    {
        res = SaveProfile(profile);
    }
    if (_GetProfileByName(profileName))
    {
        m_curProfileName = profileName;
        m3d::g_Kernel->GetEngineCfg().m_profile.Set(profileName.c_str());
        m3d::Application::g_pApp->EnqueueMessage(65678, 0, 0, 0, 0, {}, {});
        M3D_LOG_INFO("Profile \"" + profileName + "\" set");
        return res;
    }
    M3D_LOG_INFO("ProfileManager::SetCurProfile error - profile " + profileName + " not found");
    return 0;
}

CStr ProfileManager::GetDefaultProfileName() const
{
    // RVA 0x411760 - the localised default name, numbered "00", "01", ...
    // when a profile with that name already exists.
    CStr const defProfileName = M3D_APP->GetStringByStringId0("DefaultProfileName");
    if (!_GetProfileByName(defProfileName))
    {
        return defProfileName;
    }

    for (int i = 0;; ++i)
    {
        CStr strNum;
        strNum.format("%02d", i);
        CStr const defProfileNamePlus = defProfileName + strNum;
        if (!_GetProfileByName(defProfileNamePlus))
        {
            return defProfileNamePlus;
        }
    }
}

m3d::Class* ProfileManager::GetClass() const
{
    // RVA 0x40ED60
    return RT_CLASS_LOCAL(ProfileManager);
}

int ProfileManager::DeleteProfile(CStr const& name)
{
    // RVA 0x40FD80
    // Copied first: `name` is often the doomed profile's own m_name.
    CStr const profileName = name;

    auto it = m_profiles.begin();
    for (; it != m_profiles.end(); ++it)
    {
        if (*it && (*it)->GetName() == profileName)
        {
            break;
        }
    }
    if (it == m_profiles.end())
    {
        M3D_LOG_INFO("ProfileManager::DeleteProfile error - profile with name " + profileName + " not found");
        return 0;
    }

    CStr const dirPath = M3D_APP->GetStartupFolder() + "\\" + GetProfileFolderName(profileName);
    help::DeleteWindowsDir(dirPath);

    delete *it;
    *it = nullptr;
    m_profiles.erase(it);
    M3D_APP->EnqueueMessage(65679, 0, 0, 0, 0, CStr(), m3d::AIParam());

    if (profileName == m_curProfileName)
    {
        if (!m_profiles.empty())
        {
            if (m_profiles.front())
            {
                SetCurProfile(m_profiles.front()->GetName());
            }
        }
        else
        {
            // With the "change profile" window up, the plain new-profile window
            // is shown; otherwise the stand-alone one.
            auto const changeProfileVisible = M3D_APP->m_pInterfaceManager->IsWindowVisible(IW_WND_CHANGE_PROFILE);
            M3D_APP->m_pInterfaceManager->ShowWindow(
                IW_WND_NEW_PROFILE_ALONE - (changeProfileVisible ? 1 : 0), true, true, true, true, nullptr);
        }
    }
    return 1;
}

retruxx::vector<CStr> ProfileManager::GetProfilesNames() const
{
    retruxx::vector<CStr> res;
    res.reserve(m_profiles.size());
    for (auto const& profile : m_profiles)
    {
        res.push_back(profile->GetName());
    }
    return res;
}

int ProfileManager::Done()
{
    // RVA 0x411DF0
    return SaveProfile(_GetProfileByName(m_curProfileName));
}

int ProfileManager::Init()
{
    Clear();
    auto res = LoadProfiles();
    if (!m_profiles.empty())
    {
        CStr const lastProfileName = M3D_KERNEL->GetEngineCfg().m_profile.GetS();
        auto profile = _GetProfileByName(lastProfileName);
        if (!profile)
        {
            M3D_LOG_INFO("ProfileManager::Init warning - last used profile \"" + lastProfileName + "\" does not exist");
            res = 0;
            profile = m_profiles.front();
        }
        SetCurProfile(profile->GetName());
        return res;
    }
    M3D_APP->m_pInterfaceManager->ShowWindow(110, true, true, true, true, nullptr);
    return 1;
}

Profile const* ProfileManager::CreateNewProfile(CStr const& profileName)
{
    // RVA 0x40F8E0
    auto* profile = static_cast<Profile*>(m3d::g_Kernel->New("Profile"));
    if (!profile)
    {
        M3D_LOG_INFO("ProfileManager::CreateNewProfile error - cannot instatntiate profile object");
        return nullptr;
    }

    profile->SetName(profileName);
    profile->SetFolder(GetProfileFolderName(profileName));

    // NOTE: the original logs the name of the *current* profile in both error
    // messages below, not the name of the profile that failed to be created.
    CStr const sProfileName = m_curProfileName;
    if (!AddProfile(profile))
    {
        delete profile;
        M3D_LOG_INFO("ProfileManager::CreateNewProfile error - fail to add profile \"" + sProfileName + "\"");
        return nullptr;
    }
    if (!SaveProfile(profile))
    {
        DeleteProfile(profile->GetName());
        M3D_LOG_INFO("ProfileManager::CreateNewProfile error - fail to save profile " + sProfileName + " to xml file");
        return nullptr;
    }
    return profile;
}

m3d::Class* ProfileManager::GetBaseClass()
{
    return RT_CLASS_LOCAL(Object);
}

ProfileManager::~ProfileManager()
{
    // RVA 0x40EF90
    Clear();
    m3d::g_Kernel->GetEngineCfg().m_console->UnregisterCVar(&m_cvPathToProfiles);
    m3d::g_Kernel->GetEngineCfg().m_console->UnregisterCVar(&m_cvProfileFileName);
}

m3d::Object* ProfileManager::CreateObject()
{
    return new ProfileManager;
}

int ProfileManager::LoadProfiles()
{
    Clear();
    retruxx::vector<CStr> files;
    if (!GetProfileFiles(files))
    {

        M3D_LOG_INFO("ProfileManager::LoadProfiles error - cannot load profile files");
        return 0;
    }
    for (auto& file : files)
    {
        scoped_ptr stream = m3d::g_Kernel->GetFileServer().CreateFileStream();
        if (stream->Open(file.c_str(), m3d::fs::IStream::OPEN_READ))
        {
            ref_ptr xmlFile = m3d::g_Kernel->CreateXmlFile();
            if (xmlFile->Read(*stream))
            {
                stream->Close();
                ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
                xmlFile->GetFirstChild(node, "Profile");
                if (!node->IsEmpty())
                {
                    auto profile = dynamic_cast<Profile*>(m3d::g_Kernel->New("Profile"));
                    if (profile)
                    {
	                    if (profile->LoadFromXml(xmlFile, node))
	                    {
                            profile->SetFolder(DirectoryFromFileName(file));
                            if (!AddProfile(profile))
                            {
                                M3D_LOG_INFO("ProfileManager::LoadProfiles - cannot add profile " + profile->GetName());
                            }
	                    }
                        else
                        {
                            M3D_LOG_INFO("ProfileManager::LoadProfiles - error by loading profile");
                        }
                    }
                    else
                    {
                        M3D_LOG_INFO("ProfileManager::LoadProfiles error - cannot instantiate profile object");
                    }
                }
                else
                {
                    M3D_LOG_INFO("ProfileManager::LoadProfiles - error by loading profile");
                }
            }
            else
            {
                M3D_LOG_INFO("ProfileManager::LoadProfiles error - cannot parse " + file);
            }
        }
        else
        {
            M3D_LOG_INFO("ProfileManager::LoadProfiles error - can't open file " + file + " for read.");
        }
    }
    return 1;
}

Profile* ProfileManager::GetProfileByName(CStr const& profileName) const
{
    // RVA 0x40FCC0
    return _GetProfileByName(profileName);
}

m3d::Object* ProfileManager::Clone()
{
    // RVA 0x40D870
    return new ProfileManager(*this);
}

//Verified: ProfileManager::Clear
void ProfileManager::Clear()
{
    for (auto const& profile : m_profiles)
    {
        delete profile;
    }
    m_profiles.clear();
    m_curProfileName = {};
}

CStr ProfileManager::GetProfileOwnFolderName(CStr const& profileName) const
{
    // RVA 0x4110B0 - a profile's folder is simply named after the profile.
    if (profileName.empty())
    {
        return {};
    }
    return profileName;
}

Profile const* ProfileManager::CreateDefaultProfile()
{
    // RVA 0x411720
    return CreateNewProfile(GetDefaultProfileName());
}

int ProfileManager::AddProfile(Profile* profile)
{
    if (!profile || !profile->IsValid())
    {
        M3D_LOG_INFO("ProfileManager::AddProfile error - invalid profile");
        return 0;
    }
    if (_GetProfileByName(profile->GetName()))
    {
        M3D_LOG_INFO("ProfileManager::AddProfile error - profile with name " + profile->GetName() + " already exists");
        return 0;
    }
    m_profiles.push_back(profile);
    m3d::Application::g_pApp->EnqueueMessage(65679, 0, 0, 0, 0, {}, {});
    return 1;
}

ProfileManager::ProfileManager(ProfileManager const&)
{
    // RVA 0x40EF10 - nothing is copied: the profile list and current name start
    // empty and both cvars are default-constructed and left unregistered.
}

ProfileManager::ProfileManager() :
	m_cvPathToProfiles("pathToProfiles", "data\\profiles", m3d::CVar::CVAR_STRING, m3d::CVar::CVAR_ARCHIVE),
	m_cvProfileFileName("profileFileName", "profile.xml", m3d::CVar::CVAR_STRING, m3d::CVar::CVAR_ARCHIVE)
{
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_cvPathToProfiles, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_cvProfileFileName, nullptr);
}

int ProfileManager::GetProfileFiles(retruxx::vector<CStr, retruxx::allocator<CStr>>& files) const
{
    CStr pathToProfiles = m_cvPathToProfiles.GetS();
    auto attr = GetFileAttributesA(pathToProfiles.c_str());

    auto v4 = attr != -1 && (attr & 0x10) != 0;
    auto v15 = !v4;
    if (v15)
    {
        M3D_LOG_INFO("ProfileManager::GetProfileFiles warning - profile folder is not found");
        return 0;
    }
    retruxx::vector<CStr> profileDirs;
    auto res = help::GetWindowsSubDirs(pathToProfiles, profileDirs, "*.*");
    if (!res)
    {
        return 0;
    }
    for (auto& dir : profileDirs)
    {
        auto profileFile = pathToProfiles + "\\" + dir+ CStr("\\") + m_cvProfileFileName.GetS();
        attr = GetFileAttributesA(profileFile.c_str());
        if (attr != -1 && (attr & 0x10) == 0)
        {
            files.push_back(profileFile);
        }
    }
    return 1;
}

CStr ProfileManager::GetProfileFolderName(CStr const& profileName) const
{
    // RVA 0x411130
    if (profileName.empty())
    {
        return {};
    }
    auto const ownFolderName = GetProfileOwnFolderName(profileName);
    return CStr(m_cvPathToProfiles.GetS()) + "\\" + ownFolderName;
}

CStr ProfileManager::GetProfileFilePath(CStr const& profileName) const
{
    // RVA 0x411240
    if (profileName.empty())
    {
        return {};
    }
    auto const folderName = GetProfileFolderName(profileName);
    // NOTE: the original joins the folder and the file name without a path
    // separator, yielding e.g. "data\\profiles\\Nameprofile.xml".
    // SaveProfile builds its own path and does not use this.
    return folderName + CStr(m_cvProfileFileName.GetS());
}

int ProfileManager::SaveProfile(Profile const* profile) const
{
    // RVA 0x4102A0
    if (!profile || !profile->IsValid())
    {
        M3D_LOG_INFO("ProfileManager::SaveProfile error - invalid profile");
        return 0;
    }

    auto const& folder = profile->GetFolder();
    auto const attr = GetFileAttributesA(folder.c_str());
    if ((attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY) == 0) && !help::CreateWindowsDir(folder))
    {
        M3D_LOG_INFO("ProfileManager::SaveProfile error - cannot create folder \"" + folder);
        SetCurrentDirectoryA(M3D_APP->GetStartupFolder().c_str());
        return 0;
    }

    CStr const profileFileName = folder + "\\" + CStr(m_cvProfileFileName.GetS());
    scoped_ptr stream = m3d::g_Kernel->GetFileServer().CreateFileStream();
    if (!stream->Open(profileFileName.c_str(), m3d::fs::IStream::OPEN_WRITE))
    {
        M3D_LOG_INFO("ProfileManager::SaveProfile error - can't open file " + profileFileName + " for write.");
        return 0;
    }

    ref_ptr xmlFile = m3d::g_Kernel->CreateXmlFile();
    ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Profile");
    xmlFile->AddChild(node);

    auto const res = profile->SaveToXml(xmlFile, node);
    CStr const sProfileName = profile->GetName();
    if (!res)
    {
        M3D_LOG_INFO("ProfileManager::SaveProfile - errors while saving profile " + sProfileName);
    }
    xmlFile->Write(*stream);
    stream->Close();
    return res;
}

Profile* ProfileManager::_GetProfileByName(CStr const& profileName) const
{
    //TODO: check correctness
    for (auto* profile : m_profiles)
    {
        if (profileName == profile->GetName())
        {
            return profile;
        }
    }
    return nullptr;
}
