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
        throw retruxx::logic_error("Not implemented");
    }

    float BulletLauncherPrototypeInfo::GetDamageForOneShell() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool BulletLauncherPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Obj* BulletLauncherPrototypeInfo::CreateTargetObject() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    BulletLauncherPrototypeInfo::BulletLauncherPrototypeInfo()
    {
        throw retruxx::logic_error("Not implemented");
    }

    float BulletLauncher::GetAccuracyClamped() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr BulletLauncher::GetPropertyName(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus BulletLauncher::GetPropertySaveStatus(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    BulletLauncher::BulletLauncher(BulletLauncherPrototypeInfo const& prototype) : Gun(prototype)
    {
        throw retruxx::logic_error("Not implemented");
    }

    float BulletLauncher::Accuracy2GroupingAngle(float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    float BulletLauncher::GetGroupingAngle() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    float BulletLauncher::GetDamageForOneShell() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    float BulletLauncher::GetAccuracy() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* BulletLauncher::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void BulletLauncher::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void BulletLauncher::Registration()
    {
        m_propertiesMap["Accuracy"] = 31;
    }

    BulletLauncherPrototypeInfo const* BulletLauncher::GetPrototypeInfo() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool BulletLauncher::SetPropertyById(int, m3d::AIParam const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int BulletLauncher::GetPropertyId(char const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void BulletLauncher::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    float BulletLauncher::GroupingAngle2Accuracy(float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* BulletLauncher::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Gun);
    }

    float BulletLauncher::GetGroupingAngleClamped() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void BulletLauncher::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool BulletLauncher::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void BulletLauncher::_LaunchShells()
    {
        throw retruxx::logic_error("Not implemented");
    }

    BulletLauncher::~BulletLauncher()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool BulletLauncher::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* BulletLauncher::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* BulletLauncher::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }
}
