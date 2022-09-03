#include "bulletlauncher.h"

#include <stdexcept>

#include "math/matrix.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(BulletLauncher)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(BulletLauncher);

    void BulletLauncherPrototypeInfo::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    float BulletLauncherPrototypeInfo::GetDamageForOneShell() const
    {
        throw std::logic_error("Not implemented");
    }

    bool BulletLauncherPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    Obj* BulletLauncherPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    BulletLauncherPrototypeInfo::BulletLauncherPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    float BulletLauncher::GetAccuracyClamped() const
    {
        throw std::logic_error("Not implemented");
    }

    CStr BulletLauncher::GetPropertyName(int) const
    {
        throw std::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus BulletLauncher::GetPropertySaveStatus(int) const
    {
        throw std::logic_error("Not implemented");
    }

    BulletLauncher::BulletLauncher(BulletLauncherPrototypeInfo const& prototype) : Gun(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    float BulletLauncher::Accuracy2GroupingAngle(float)
    {
        throw std::logic_error("Not implemented");
    }

    float BulletLauncher::GetGroupingAngle() const
    {
        throw std::logic_error("Not implemented");
    }

    float BulletLauncher::GetDamageForOneShell() const
    {
        throw std::logic_error("Not implemented");
    }

    float BulletLauncher::GetAccuracy() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* BulletLauncher::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void BulletLauncher::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void BulletLauncher::Registration()
    {
        throw std::logic_error("Not implemented");
    }

    BulletLauncherPrototypeInfo const* BulletLauncher::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    bool BulletLauncher::SetPropertyById(int, m3d::AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    int BulletLauncher::GetPropertyId(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    void BulletLauncher::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    float BulletLauncher::GroupingAngle2Accuracy(float)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* BulletLauncher::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Gun);
    }

    float BulletLauncher::GetGroupingAngleClamped() const
    {
        throw std::logic_error("Not implemented");
    }

    void BulletLauncher::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw std::logic_error("Not implemented");
    }

    bool BulletLauncher::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    void BulletLauncher::_LaunchShells()
    {
        throw std::logic_error("Not implemented");
    }

    BulletLauncher::~BulletLauncher()
    {
        throw std::logic_error("Not implemented");
    }

    bool BulletLauncher::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* BulletLauncher::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* BulletLauncher::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }
}
