#include "profile.h"
#include <stdexcept>
#include <core/kernel.h>
#include <core/aiparam.h>

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
