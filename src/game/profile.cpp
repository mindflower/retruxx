#include "profile.h"
#include <stdexcept>
#include <core/kernel.h>
#include <core/aiparam.h>
#include <core/log.h>

int Profile::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
}

int ProfileManager::LoadProfiles()
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
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

ProfileManager::ProfileManager()
{
    throw std::logic_error("Not implemented");
}

int ProfileManager::GetProfileFiles(std::vector<CStr, std::allocator<CStr>>&) const
{
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
