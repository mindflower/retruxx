#include "profile.h"
#include <stdexcept>
#include <core/kernel.h>
#include <core/aiparam.h>
#include <core/log.h>

#include "config.h"
#include "m3dapp.h"
#include "m3dgame.h"
#include "core/scoped_ptr.h"
#include "core/console/console.h"
#include "file/fileserver.h"
#include "file/filestream.h"

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
            xmlNode->GetFirstChild_(paramsNode, "Params");
            if (!paramsNode->IsEmpty())
            {
                ref_ptr paramNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
                for (paramsNode->GetFirstChild_(paramNode, "Param"); !paramNode->IsEmpty();paramNode->GetNextSibling_(paramNode, "Param"))
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
                        paramNode->GetFirstChild_(valueNode, "Value");
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
    throw std::logic_error("Not implemented");
}

m3d::Class* Profile::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* Profile::CreateObject()
{
    throw std::logic_error("Not implemented");
}

void Profile::SetFolder(CStr const&)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* Profile::Clone()
{
    throw std::logic_error("Not implemented");
}

int Profile::GetParam(ProfileParam, m3d::AIParam&) const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* Profile::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

void Profile::SetName(CStr const&)
{
    throw std::logic_error("Not implemented");
}

int Profile::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
{
    throw std::logic_error("Not implemented");
}

bool Profile::IsValid() const
{
    throw std::logic_error("Not implemented");
}

int Profile::SetParam(ProfileParam, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

Profile::~Profile()
{
    throw std::logic_error("Not implemented");
}

CStr const& Profile::GetName() const
{
    throw std::logic_error("Not implemented");
}

int Profile::SetParamsDefault()
{
    throw std::logic_error("Not implemented");
}

void Profile::Clear()
{
    throw std::logic_error("Not implemented");
}

Profile::Profile(Profile const&)
{
    throw std::logic_error("Not implemented");
}

Profile::Profile()
{
    throw std::logic_error("Not implemented");
}

ProfileParam Profile::ParamName2Id(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

CStr Profile::ParamId2Name(ProfileParam) const
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORTS_BEGIN(ProfileManager)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ProfileManager);

Profile* ProfileManager::GetCurProfile() const
{
    return _GetProfileByName(m_curProfileName);
}

int ProfileManager::SetCurProfile(CStr const&)
{
    throw std::logic_error("Not implemented");
}

CStr ProfileManager::GetDefaultProfileName() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ProfileManager::GetClass() const
{
    throw std::logic_error("Not implemented");
}

int ProfileManager::DeleteProfile(CStr const&)
{
    throw std::logic_error("Not implemented");
}

std::vector<CStr> ProfileManager::GetProfilesNames() const
{
    throw std::logic_error("Not implemented");
}

int ProfileManager::Done()
{
    throw std::logic_error("Not implemented");
}

int ProfileManager::Init()
{
    Clear();
    auto res = LoadProfiles();
    if (!m_profiles.empty())
    {
        CStr lastProfileName = m3d::g_Kernel->GetEngineCfg().m_profile.GetS();
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
    auto app = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
    app->m_pInterfaceManager->ShowWindow(110, true, true, true, true, nullptr);
    return 1;

    throw std::logic_error("Not implemented");
}

Profile const* ProfileManager::CreateNewProfile(CStr const&)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ProfileManager::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

ProfileManager::~ProfileManager()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* ProfileManager::CreateObject()
{
    return new ProfileManager;
}

int ProfileManager::LoadProfiles()
{
    Clear();
    std::vector<CStr> files;
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
                xmlFile->GetFirstChild_(node, "Profile");
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

Profile* ProfileManager::GetProfileByName(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* ProfileManager::Clone()
{
    throw std::logic_error("Not implemented");
}

void ProfileManager::Clear()
{
    for (auto profile : m_profiles)
    {
        //TODO: check this
        delete profile;
    }
    m_profiles.clear();
    m_curProfileName = "";
}

CStr ProfileManager::GetProfileOwnFolderName(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

Profile const* ProfileManager::CreateDefaultProfile()
{
    throw std::logic_error("Not implemented");
}

int ProfileManager::AddProfile(Profile*)
{
    throw std::logic_error("Not implemented");
}

ProfileManager::ProfileManager(ProfileManager const&)
{
    throw std::logic_error("Not implemented");
}

ProfileManager::ProfileManager() :
	m_cvPathToProfiles("pathToProfiles", "data\\profiles", m3d::CVar::CVAR_STRING, m3d::CVar::CVAR_ARCHIVE),
	m_cvProfileFileName("profileFileName", "profile.xml", m3d::CVar::CVAR_STRING, m3d::CVar::CVAR_ARCHIVE)
{
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_cvPathToProfiles, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_cvProfileFileName, nullptr);
}

int ProfileManager::GetProfileFiles(std::vector<CStr, std::allocator<CStr>>& files) const
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

    throw std::logic_error("Not implemented");
}

CStr ProfileManager::GetProfileFolderName(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

CStr ProfileManager::GetProfileFilePath(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

int ProfileManager::SaveProfile(Profile const* profile) const
{
    if (profile && profile->IsValid())
    {
        throw std::logic_error("Not implemented");
        //TODO: ...
    }
    else
    {
        M3D_LOG_ERR("ProfileManager::SaveProfile error - invalid profile");
        return 0;
    }
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
