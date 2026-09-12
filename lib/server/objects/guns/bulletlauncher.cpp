#include "bulletlauncher.h"

#include "bullet.h"
#include "core/kernel.h"

#include <stdexcept>

#include "math/matrix.h"
#include "server/objects/base/globalproperties.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/base/shell.h"

#include <server/objects/base/prototypemanager.h>

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
        auto const res = GunPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
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
        return new BulletLauncher(*this);
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

    BulletLauncher::BulletLauncher(BulletLauncherPrototypeInfo const& prototypeInfo) : Gun(prototypeInfo)
    {
        m_numBulletsInShot = prototypeInfo.m_numBulletsInShot;
        m_groupingAngle = prototypeInfo.m_groupingAngle;
        m_numBulletsToTracer = 0;
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
        return RT_CLASS_LOCAL(BulletLauncher);
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
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), BulletLauncherPrototypeInfo const);
    }

    bool BulletLauncher::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        if (propertyId != 31)
            return Gun::SetPropertyById(propertyId, newValue);

        m_groupingAngle = -ai::theGlobProp.m_maxGroupingAngle * newValue.GetAsFloat() * 0.0099999998 +
            ai::theGlobProp.m_maxGroupingAngle;
        return 1;
    }

    int BulletLauncher::GetPropertyId(char const* propName) const
    {
        auto it = BulletLauncher::m_propertiesMap.find(propName);
        if (it != BulletLauncher::m_propertiesMap.end())
        {
            return it->second;
        }

        return Gun::GetPropertyId(propName);
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
        Gun::_LaunchShells();
        for (int i = 0; i < m_numBulletsInShot; ++i)
        {
            auto const shellPrototypeId = GetShellPrototypeId();
            auto const objId = theObjects->CreateNewObject(shellPrototypeId, {}, -1, -1);
            auto* shellObj = (Bullet*)theObjects->GetEntityByObjId(objId);
            shellObj->SetGunObjId(GetId());

            auto* owner = GetOwner();
            shellObj->SetBelong(owner->GetBelong());

            auto const pos = _CalcPosForNextShot();
            shellObj->SetPosition(pos);

            auto angle = m_groupingAngle;
            if (angle < 0.0)
            {
                angle = 0.0;
            }
            if (angle > theGlobProp.m_maxGroupingAngle)
            {
                angle = theGlobProp.m_maxGroupingAngle;
            }

            auto const dirForNextShot = _CalcDirForNextShot();
            auto const deviatedVector = GetRandomDeviatedVector(dirForNextShot, angle);
            shellObj->SetDirection(deviatedVector);

            auto const range = GetFiringRange();
            shellObj->SetRange(range);
            shellObj->SetParentBarrel(m_curBarrelIndex);
            shellObj->RelinkToSpace(owner->GetSpaceId());

            auto const* protoInfo = GetPrototypeInfo();

            // TODO: generated code BulletLauncher::_LaunchShells
            // Check if tracer is enabled
            if (protoInfo->m_tracerRange > 0 && !protoInfo->m_tracerEffectName.empty())
            {
                // Check if we have a valid barrel node
                auto* barrelNode = GetBarrelNode();
                if (barrelNode)
                {
                    // Update tracer counter
                    ++m_numBulletsToTracer;

                    // Check if it's time to create a tracer
                    if (m_numBulletsToTracer >= protoInfo->m_tracerRange)
                    {
                        m_numBulletsToTracer = 0;

                        // Create tracer node
                        auto* tracerNode =
                            CreateNode(protoInfo->m_tracerEffectName, 0, {1.0, 1.0, 1.0}, nullptr, false);
                        M3D_ASSERT(tracerNode);

                        tracerNode->RemoveImmediateAfterParent(false);

                        // Attach tracer to barrel
                        barrelNode->AddChild(tracerNode);

                        // Calculate tracer position along bullet trajectory
                        auto* bulletRay = shellObj->_Ray();

                        auto bulletDir = bulletRay->GetDirection();
                        auto bulletPos = shellObj->GetPosition();

                        // Calculate tracer position (20 units ahead of bullet)
                        CVector tracerWorldPos;
                        tracerWorldPos.x = bulletPos.x + (bulletDir.x * 20.0f);
                        tracerWorldPos.y = bulletPos.y + (bulletDir.y * 20.0f);
                        tracerWorldPos.z = bulletPos.z + (bulletDir.z * 20.0f);

                        // Convert to barrel-relative coordinates
                        auto barrelInverse = barrelNode->GetCurrentMatrix().getInverse();

                        CVector tracerLocalPos;
                        tracerLocalPos.x = (barrelInverse._11 * tracerWorldPos.x) +
                            (barrelInverse._21 * tracerWorldPos.y) + (barrelInverse._31 * tracerWorldPos.z) +
                            barrelInverse._41;
                        tracerLocalPos.y = (barrelInverse._12 * tracerWorldPos.x) +
                            (barrelInverse._22 * tracerWorldPos.y) + (barrelInverse._32 * tracerWorldPos.z) +
                            barrelInverse._42;
                        tracerLocalPos.z = (barrelInverse._13 * tracerWorldPos.x) +
                            (barrelInverse._23 * tracerWorldPos.y) + (barrelInverse._33 * tracerWorldPos.z) +
                            barrelInverse._43;

                        // Set tracer position
                        tracerNode->SetOriginAbs(tracerLocalPos);

                        // Update transformations
                        tracerNode->UpdateXForm(false, true);

                        // Associate tracer with bullet
                        shellObj->SetTracer(tracerNode);
                    }
                }
            }
        }
    }

    BulletLauncher::~BulletLauncher() = default;

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
}  // namespace ai
