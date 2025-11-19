#include "bulletlauncher.h"

#include "core/kernel.h"

#include <stdexcept>

#include "math/matrix.h"
#include "server/objects/base/globalproperties.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(BulletLauncher)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(BulletLauncher);

    void BulletLauncherPrototypeInfo::PostLoad()
    {
        GunPrototypeInfo::PostLoad();
    }

    float BulletLauncherPrototypeInfo::GetDamageForOneShell() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool BulletLauncherPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        const auto res = GunPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (res)
        {
            m3d::SafeIntAttrib(m_numBulletsInShot, xmlNode, "NumBulletsInShot");
            m3d::SafeFloatAttrib(m_groupingAngle, xmlNode, "GroupingAngle");
            m3d::SafeStrAttrib(m_BlastWavePrototypeName, xmlNode, "BlastWavePrototype");
            m3d::SafeIntAttrib(m_tracerRange, xmlNode, "TracerRange");
            m3d::SafeStrAttrib(m_tracerEffectName, xmlNode, "TracerEffect");

            m_groupingAngle = m_groupingAngle * 0.017453292 * 0.5;
            M3D_ASSERT(m_groupingAngle >= 0.f && m_groupingAngle <= ai::theGlobProp.m_maxGroupingAngle);
        }
        return res;
    }

    Obj* BulletLauncherPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    BulletLauncherPrototypeInfo::BulletLauncherPrototypeInfo()
    {
        this->m_groupingAngle = 0.0;
        this->m_numBulletsInShot = 1;
        this->m_tracerRange = 1;
        this->m_damageType = DAMAGE_PIERCING;
    }

    float BulletLauncher::GetAccuracyClamped() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr BulletLauncher::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    eGObjPropertySaveStatus BulletLauncher::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    BulletLauncher::BulletLauncher(BulletLauncherPrototypeInfo const& prototype) : Gun(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float BulletLauncher::Accuracy2GroupingAngle(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float BulletLauncher::GetGroupingAngle() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float BulletLauncher::GetDamageForOneShell() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float BulletLauncher::GetAccuracy() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* BulletLauncher::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BulletLauncher::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BulletLauncher::Registration()
    {
        m_propertiesMap["Accuracy"] = 31;
    }

    BulletLauncherPrototypeInfo const* BulletLauncher::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool BulletLauncher::SetPropertyById(int, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int BulletLauncher::GetPropertyId(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BulletLauncher::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float BulletLauncher::GroupingAngle2Accuracy(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* BulletLauncher::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Gun);
    }

    float BulletLauncher::GetGroupingAngleClamped() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BulletLauncher::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool BulletLauncher::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BulletLauncher::_LaunchShells()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    BulletLauncher::~BulletLauncher()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool BulletLauncher::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* BulletLauncher::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* BulletLauncher::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
