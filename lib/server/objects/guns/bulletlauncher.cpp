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
        // RVA 0x7467B0 - the listed damage is for the whole shot, shared out over its bullets.
        return m_damage / m_numBulletsInShot;
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
        // RVA 0x7459E0
        float const accuracy = GetAccuracy();
        if (accuracy < 0.0f)
        {
            return 0.0f;
        }
        if (accuracy > 100.0f)
        {
            return 100.0f;
        }
        return accuracy;
    }

    CStr BulletLauncher::GetPropertyName(int id) const
    {
        // RVA 0x746680
        for (auto const& property : m_propertiesMap)
        {
            if (property.second == id)
            {
                return property.first;
            }
        }
        return Gun::GetPropertyName(id);
    }

    eGObjPropertySaveStatus BulletLauncher::GetPropertySaveStatus(int id) const
    {
        // RVA 0x7464D0 - NOTE: Registration only ever fills m_propertiesMap, so this class's
        // save-status map is always empty and every id falls through to the base class.
        auto const it = m_propertiesSaveStatesMap.find(id);
        if (it != m_propertiesSaveStatesMap.end())
        {
            return it->second;
        }
        return Gun::GetPropertySaveStatus(id);
    }

    BulletLauncher::BulletLauncher(BulletLauncherPrototypeInfo const& prototypeInfo) : Gun(prototypeInfo)
    {
        m_numBulletsInShot = prototypeInfo.m_numBulletsInShot;
        m_groupingAngle = prototypeInfo.m_groupingAngle;
        m_numBulletsToTracer = 0;
    }

    float BulletLauncher::Accuracy2GroupingAngle(float accuracy)
    {
        // RVA 0x7459A0 - the inverse of GroupingAngle2Accuracy: 100% accuracy is a zero spread.
        return -theGlobProp.m_maxGroupingAngle * accuracy * 0.0099999998f + theGlobProp.m_maxGroupingAngle;
    }

    float BulletLauncher::GetGroupingAngle() const
    {
        // RVA 0x745890
        return m_groupingAngle;
    }

    float BulletLauncher::GetDamageForOneShell() const
    {
        return m_damage / m_numBulletsInShot;
    }

    float BulletLauncher::GetAccuracy() const
    {
        // RVA 0x7459C0
        return 100.0f - m_groupingAngle / theGlobProp.m_maxGroupingAngle * 100.0f;
    }

    m3d::Class* BulletLauncher::GetClass() const
    {
        return RT_CLASS_LOCAL(BulletLauncher);
    }

    void BulletLauncher::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        // RVA 0x746600
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.second);
        }
        Gun::GetPropertiesIDs(Props);
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

    void BulletLauncher::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
    {
        // RVA 0x746580
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.first);
        }
        Gun::GetPropertiesNames(Props);
    }

    float BulletLauncher::GroupingAngle2Accuracy(float groupingAngle)
    {
        // RVA 0x745980 - accuracy is the spread expressed as a percentage of the worst allowed.
        return 100.0f - groupingAngle / theGlobProp.m_maxGroupingAngle * 100.0f;
    }

    m3d::Class* BulletLauncher::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Gun);
    }

    float BulletLauncher::GetGroupingAngleClamped() const
    {
        // RVA 0x745940
        if (m_groupingAngle < 0.0f)
        {
            return 0.0f;
        }
        if (m_groupingAngle > theGlobProp.m_maxGroupingAngle)
        {
            return theGlobProp.m_maxGroupingAngle;
        }
        return m_groupingAngle;
    }

    void BulletLauncher::RegisterProperty(char const* Name, int id, eGObjPropertySaveStatus saveStatus)
    {
        // RVA 0x7466F0 - SAVE_PROP_NORMAL is the default and is not recorded.
        m_propertiesMap[Name] = id;
        if (saveStatus)
        {
            m_propertiesSaveStatesMap[id] = saveStatus;
        }
    }

    bool BulletLauncher::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x746840 - accuracy is not stored; it is derived from the current spread.
        if (propertyId != 31)
        {
            return Gun::_GetPropertyInternal(propertyId, retVal);
        }
        retVal = 100.0f - m_groupingAngle / theGlobProp.m_maxGroupingAngle * 100.0f;
        return true;
    }

    void BulletLauncher::_LaunchShells()
    {
        // RVA 0x746070
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

            // Only every m_tracerRange'th bullet gets a visible tracer.
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

    bool BulletLauncher::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x7468C0 - the default accuracy is the one the prototype's spread gives.
        BulletLauncherPrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        if (propertyId != 31)
        {
            return Gun::_GetPropertyDefaultInternal(propertyId, retVal);
        }
        retVal = 100.0f - prototypeInfo->m_groupingAngle / theGlobProp.m_maxGroupingAngle * 100.0f;
        return true;
    }

    m3d::Object* BulletLauncher::Clone()
    {
        // RVA 0x745A30
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* BulletLauncher::CreateObject()
    {
        // RVA 0x745BF0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }
}  // namespace ai
