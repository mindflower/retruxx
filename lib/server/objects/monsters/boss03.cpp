#include "boss03.h"

#include <cassert>
#include <cmath>
#include <stdexcept>

#include "cinematic.h"
#include "m3dapp.h"
#include "skelmodel.h"
#include "core/kernel.h"
#include "core/log.h"
#include "scene/servers/dataserver.h"
#include "server/damageinfo.h"
#include "server/dynamicscene.h"
#include "server/modifier.h"
#include "server/utils.h"
#include "server/weaponfirer.h"
#include "server/objects/team.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/base/prototypemanager.h"
#include "server/objects/monsters/boss03part.h"
#include "server/objects/physicbodies/physichelpers.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Boss03, StartPlacingDrone)
{
    // RVA 0x73BF90
    auto* boss = static_cast<ai::Boss03*>(context->asObject(0, "Boss03"));
    context->pushBool(boss->StartPlacingDrone());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Boss03, StartMoving)
{
    // RVA 0x738870
    auto* boss = static_cast<ai::Boss03*>(context->asObject(0, "Boss03"));
    CVector const finalDirection = context->asVector(2);
    CVector const destination = context->asVector(1);
    boss->StartMoving(destination, finalDirection);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Boss03, NoticePlayer)
{
    // RVA 0x73D6B0
    static_cast<ai::Boss03*>(context->asObject(0, "Boss03"))->NoticePlayer();
    return 1;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Boss03)
    RT_CLASS_EXPORT(Boss03, m3d::METHOD, StartPlacingDrone, "", "", "")
    RT_CLASS_EXPORT(Boss03, m3d::METHOD, StartMoving, "", "", "")
    RT_CLASS_EXPORT(Boss03, m3d::METHOD, NoticePlayer, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Boss03);

    namespace
    {
        // The boss is a helicopter: the drone spawner cabin is its first key part and the two wings
        // the second; once they are all shot off only the small helicopter is left.
        CStr const DRONE_LP_NAME("LP_LOAD01");
        CStr const DRONE_SPAWNER_PART_NAME("CAB04");
        CStr const MAIN_PART_NAME("MAIN");
        CStr const WING01_PART_NAME("WING01");
        CStr const WING02_PART_NAME("WING02");

        float const DRONE_PLACING_TIME = 3.0f;

        CVector const INITIAL_UP_DIRECTION(0.0f, 1.0f, 0.0f);
        CVector const INITIAL_OBJECTS_DIRECTION(0.0f, 0.0f, 1.0f);
        // up x forward
        CVector const INITIAL_LEFT_DIRECTION(
            INITIAL_UP_DIRECTION.z * INITIAL_OBJECTS_DIRECTION.y - INITIAL_UP_DIRECTION.y * INITIAL_OBJECTS_DIRECTION.z,
            INITIAL_OBJECTS_DIRECTION.z * INITIAL_UP_DIRECTION.x - INITIAL_UP_DIRECTION.z * INITIAL_OBJECTS_DIRECTION.x,
            INITIAL_UP_DIRECTION.y * INITIAL_OBJECTS_DIRECTION.x - INITIAL_OBJECTS_DIRECTION.y * INITIAL_UP_DIRECTION.x);

        // RVA 0x5CCAF0 - rand() scaled by RAND_MAX+1, so the top bound is exclusive.
        unsigned IntRandom(unsigned highBound)
        {
            return highBound * rand() / 0x8000;
        }

        // The shipped randomUniform<float>.
        float RandomUniformFloat(float left, float right)
        {
            return static_cast<float>(static_cast<double>(rand()) * 0.000030518509 * (right - left) + left);
        }

        // v rotated by q (row vector times q's rotation matrix), as expanded inline in the
        // shipped code.
        CVector RotateByQuaternion(CVector const& v, Quaternion const& q)
        {
            float const xz = q.x * q.z;
            float const xw = q.x * q.w;
            float const xx = q.x * q.x;
            float const xy = q.x * q.y;
            float const zy = q.z * q.y;
            float const yy = q.y * q.y;
            float const zz = q.z * q.z;
            float const yw = q.y * q.w;
            float const zw = q.z * q.w;
            float const m11 = 1.0f - (zz + yy) * 2.0f;
            float const m21 = (xy - zw) * 2.0f;
            float const m31 = (yw + xz) * 2.0f;
            float const m12 = (zw + xy) * 2.0f;
            float const m22 = 1.0f - (zz + xx) * 2.0f;
            float const m32 = (zy - xw) * 2.0f;
            float const m13 = (xz - yw) * 2.0f;
            float const m23 = (xw + zy) * 2.0f;
            float const m33 = 1.0f - (yy + xx) * 2.0f;
            return CVector(
                (v.x * m11 + m21 * v.y) + m31 * v.z,
                (v.x * m12 + m22 * v.y) + m32 * v.z,
                (v.x * m13 + m23 * v.y) + m33 * v.z);
        }

        // The Hamilton product a * b, as expanded inline in the shipped code.
        Quaternion QuatMul(Quaternion const& a, Quaternion const& b)
        {
            return Quaternion(
                a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
                a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
                a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
                a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z);
        }

        // The rotation by angle around axis.
        Quaternion AxisAngle(CVector const& axis, float angle)
        {
            float const halfAngle = angle * 0.5f;
            float const s = static_cast<float>(sin(halfAngle));
            return Quaternion(axis.x * s, axis.y * s, axis.z * s, static_cast<float>(cos(halfAngle)));
        }
    }  // namespace

    bool Boss03PrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x73C770
        // NOTE: calls ComplexPhysicObjPrototypeInfo::LoadFromXML directly, skipping
        // AnimatedComplexPhysicObjPrototypeInfo's.
        bool const result = ComplexPhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            CStr strDronePrototypeNames;
            m3d::SafeStrAttrib(strDronePrototypeNames, xmlNode, "DronePrototypes");
            m3d::Tokenize(strDronePrototypeNames, m_dronePrototypeNames, "(), ;\t");
            m3d::SafeIntAttrib(m_maxDrones, xmlNode, "MaxDrones");
            m3d::SafeFloatAttrib(m_maxHealth, xmlNode, "MaxHealth");
            m3d::SafeFloatAttrib(m_maxHorizAngularVelocity, xmlNode, "MaxHorizAngularVelocity");
            m3d::SafeFloatAttrib(m_horizAngularAcceleration, xmlNode, "HorizAngularAcceleration");
            m3d::SafeFloatAttrib(m_maxVertAngularVelocity, xmlNode, "MaxVertAngularVelocity");
            m3d::SafeFloatAttrib(m_vertAngularAcceleration, xmlNode, "VertAngularAcceleration");
            m3d::SafeFloatAttrib(m_maxLinearVelocity, xmlNode, "MaxLinearVelocity");
            m3d::SafeFloatAttrib(m_linearAcceleration, xmlNode, "LinearAcceleration");
            m3d::SafeFloatAttrib(m_pathTrackTiltAngle, xmlNode, "PathTrackTiltAngle");
            m_pathTrackTiltAngle *= 0.017453292f;
            m3d::SafeFloatAttrib(m_maxShootingTime, xmlNode, "MaxShootingTime");
            m3d::SafeFloatAttrib(m_defaultHover, xmlNode, "DefaultHover");
            m3d::SafeFloatAttrib(m_hoverForPlacingDrones, xmlNode, "HoverForPlacingDrones");
        }
        return result;
    }

    Boss03PrototypeInfo::Boss03PrototypeInfo()
    {
        // RVA 0x73BFC0
        m_maxDrones = 1;
        m_droneRelPosition = ZeroVector;
        m_droneRelRotation = IdentityQuaternion;
        m_maxHorizAngularVelocity = 0.0;
        m_horizAngularAcceleration = 0.0;
        m_maxVertAngularVelocity = 0.0;
        m_vertAngularAcceleration = 0.0;
        m_maxLinearVelocity = 0.0;
        m_linearAcceleration = 0.0;
        m_pathTrackTiltAngle = 0.0;
        m_maxHealth = 1.0f;
        m_maxShootingTime = 1.0f;
        m_defaultHover = 10.0f;
        m_hoverForPlacingDrones = 10.0f;
    }

    void Boss03PrototypeInfo::PostLoad()
    {
        // RVA 0x73B330
        ComplexPhysicObjPrototypeInfo::PostLoad();
        m_dronePrototypeIds.clear();
        m_dronePrototypeIds.reserve(m_dronePrototypeNames.size());
        for (auto const& name : m_dronePrototypeNames)
        {
            int const droneProototypeId = thePrototypeManager->GetPrototypeId(name);
            if (droneProototypeId == -1)
            {
                SYS_ERROR("droneProototypeId != INVALID_ID && \"Invalid drone prototype for Boss03\"");
            }
            m_dronePrototypeIds.push_back(droneProototypeId);
        }
    }

    void Boss03PrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        // RVA 0x73AA20
        // The main part loops its STAND2 and ATTACK1 animations; the spawner holds its open and
        // close ones, and its load point is where drones are released.
        // NOTE: does not chain to the base RefreshFromXml.
        m3d::DataServer* server = &M3D_APP->GetAnimatedModelsServer();
        auto const mainIt = m_partPrototypeIds.find(MAIN_PART_NAME);
        if (mainIt != m_partPrototypeIds.end())
        {
            auto const* const mainInfo = static_cast<PhysicBodyPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(mainIt->second));
            int const itemId = server->GetItemByName(mainInfo->m_engineModelName.c_str(), true);
            if (itemId != -1)
            {
                m3d::AnimatedModel* mdl = nullptr;
                server->GetItemProperty(itemId, 16394, &mdl);
                if (mdl)
                {
                    mdl->SetNextForAnimation(AT_STAND2, AT_STAND2);
                    mdl->SetNextForAnimation(AT_ATTACK1, AT_ATTACK1);
                }
            }
        }
        auto const spawnerIt = m_partPrototypeIds.find(DRONE_SPAWNER_PART_NAME);
        if (spawnerIt == m_partPrototypeIds.end())
        {
            return;
        }
        auto const* const spawnerInfo = static_cast<PhysicBodyPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(spawnerIt->second));
        int const itemId = server->GetItemByName(spawnerInfo->m_engineModelName.c_str(), true);
        if (itemId == -1)
        {
            return;
        }
        m3d::AnimatedModel* mdl = nullptr;
        server->GetItemProperty(itemId, 16394, &mdl);
        if (!mdl)
        {
            return;
        }
        mdl->SetNextForAnimation(AT_MOVE1, -1);
        mdl->SetNextForAnimation(AT_MOVE2, -1);
        CMatrix droneLpMatrix;
        if (mdl->GetBoneMatrixByName(DRONE_LP_NAME, droneLpMatrix, false))
        {
            m_droneRelPosition = CVector(droneLpMatrix._41, droneLpMatrix._42, droneLpMatrix._43);
            m_droneRelRotation.FromMatrix(droneLpMatrix);
        }
        else
        {
            M3D_LOG_ERR(CStr("Error: LoadPoint not found! Model = '") + spawnerInfo->m_engineModelName + CStr("', lp = ") + DRONE_LP_NAME);
        }
    }

    Obj* Boss03PrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x73CA10
        return new Boss03(*this);
    }

    Boss03::Boss03(Boss03PrototypeInfo const& prototype) :
        AnimatedComplexPhysicObj(prototype),
        m_health(prototype.m_maxHealth, 0.0, prototype.m_maxHealth)
    {
        // RVA 0x73C0D0
        m_health.m_BeforeValueApplyModifier =
            new BeforeApplyModifierFloatCallback(*this, &Boss03::_OnHealthValueBeforeApplyModifier);
        m_linearVelocity = ZeroVector;
        m_relAngularVelocity = ZeroVector;
        m_liveStatus = LIVE_IDLE;
        m_droneSpawningStatus = DRONE_DECIDING;
        m_dronePlacingTimeout = 0.0f;
        m_shootingTimeout = 0.0f;
        m_pathTrackingStatus = PATH_IDLE;
        m_desiredDestination = ZeroVector;
        m_desiredDirection = INITIAL_OBJECTS_DIRECTION;
        m_droneTeam = nullptr;
        m_currentFlyPath = nullptr;
        m_currentFlyTime = 0.0f;
        m_keyPartsMaxDurability = 0.0f;
        // The boss is flown by hand, never by the physics.
        ComplexPhysicObj::DisablePhysics();
        m_physicState |= 1;
        SetCorrectEnabledCellsCounter();
        ComplexPhysicObj::EnableGeometry(false);
    }

    Boss03::~Boss03()
    {
        // RVA 0x73CA40
        delete m_currentFlyPath;
        m_currentFlyPath = nullptr;
    }

    m3d::Object* Boss03::Clone()
    {
        // RVA 0x7388E0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* Boss03::CreateObject()
    {
        // RVA 0x738AA0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Class* Boss03::GetBaseClass()
    {
        // RVA 0x737A40
        return RT_CLASS_LOCAL(AnimatedComplexPhysicObj);
    }

    m3d::Class* Boss03::GetClass() const
    {
        // RVA 0x737A50
        return RT_CLASS_LOCAL(Boss03);
    }

    Boss03PrototypeInfo const* Boss03::GetPrototypeInfo() const
    {
        // RVA 0x73A1E0
        return static_cast<Boss03PrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    void Boss03::RegisterProperty(char const* name, int id, eGObjPropertySaveStatus saveStatus)
    {
        // Declared in the PDB but never emitted in the shipped build (Registration is empty).
        m_propertiesMap[CStr(name)] = id;
        m_propertiesSaveStatesMap[id] = saveStatus;
    }

    void Boss03::Registration()
    {
        // RVA 0x737B00 - registers nothing.
    }

    eGObjPropertySaveStatus Boss03::GetPropertySaveStatus(int id) const
    {
        // RVA 0x73C280
        auto it = m_propertiesSaveStatesMap.find(id);
        if (it != m_propertiesSaveStatesMap.end())
        {
            return it->second;
        }
        return PhysicObj::GetPropertySaveStatus(id);
    }

    void Boss03::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
    {
        // Declared in the PDB but never emitted in the shipped build; the usual property-map pattern.
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.first);
        }
        PhysicObj::GetPropertiesNames(Props);
    }

    void Boss03::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        // Declared in the PDB but never emitted in the shipped build; the usual property-map pattern.
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.second);
        }
        PhysicObj::GetPropertiesIDs(Props);
    }

    CStr Boss03::GetPropertyName(int id) const
    {
        // RVA 0x73C430
        for (auto const& property : m_propertiesMap)
        {
            if (property.second == id)
            {
                return property.first;
            }
        }
        return PhysicObj::GetPropertyName(id);
    }

    bool Boss03::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        // RVA 0x737A80
        return PhysicObj::SetPropertyById(propertyId, newValue);
    }

    int Boss03::GetPropertyId(char const* PropertyName) const
    {
        // RVA 0x73C2C0
        auto it = m_propertiesMap.find(PropertyName);
        if (it != m_propertiesMap.end())
        {
            return it->second;
        }
        return PhysicObj::GetPropertyId(PropertyName);
    }

    bool Boss03::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x737A90
        return PhysicObj::_GetPropertyDefaultInternal(propertyId, retVal);
    }

    bool Boss03::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x737A70
        return PhysicObj::_GetPropertyInternal(propertyId, retVal);
    }

    bool Boss03::CanChildBeAdded(m3d::Class* pClass) const
    {
        // RVA 0x737AA0 - the drone team is the only extra child.
        return Obj::CanChildBeAdded(pClass) || pClass->IsKindOf(RT_CLASS_LOCAL(Team));
    }

    void Boss03::AddChild(Obj* pObj)
    {
        // RVA 0x738C60
        ComplexPhysicObj::AddChild(pObj);
        if (!pObj || !pObj->IsKindOf(RT_CLASS_LOCAL(Team)))
        {
            return;
        }
        if (m_droneTeam)
        {
            SYS_ERROR("!m_droneTeam");
        }
        m_droneTeam = static_cast<Team*>(pObj);
        pObj->LinkToParent(GetId(), HIERARCHY_CHILD);
    }

    bool Boss03::RemoveChild(Obj* pChild)
    {
        // RVA 0x738DF0
        ComplexPhysicObj::RemoveChild(pChild);
        if (!pChild || !pChild->IsKindOf(RT_CLASS_LOCAL(Team)))
        {
            return false;
        }
        if (m_droneTeam != pChild)
        {
            SYS_ERROR("m_droneTeam == pChild");
        }
        pChild->m_parentId = -1;
        m_droneTeam = nullptr;
        return true;
    }

    void Boss03::CreateChildren()
    {
        // RVA 0x73A210 - the team the drones join.
        ComplexPhysicObj::CreateChildren();
        int const teamId = theObjects->CreateNewObject(thePrototypeManager->GetPrototypeId(CStr("settlementTeam")), "", -1, -1);
        // NOTE: a failed creation adds a null child.
        Obj* team = nullptr;
        if (teamId >= 0)
        {
            team = theObjects->GetEntityByObjId(teamId);
        }
        AddChild(team);
    }

    void Boss03::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x8D0420
        // The placed boss lists where it releases drones and where it shoots from; both are
        // hovered over the ground.
        ComplexPhysicObj::LoadFromXML(xmlFile, xmlNode);
        Boss03PrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        m3d::SafeStrAttrib(m_pathNameForFlyingWithWings, xmlNode, "PathNameForFlyingWithWings");

        ref_ptr dronePlacesNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        xmlNode->GetFirstChild(dronePlacesNode, "DronePlaces");
        if (!dronePlacesNode->IsEmpty() && dronePlacesNode->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            ref_ptr pointNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
            dronePlacesNode->GetFirstChild(pointNode, "Point");
            while (!pointNode->IsEmpty())
            {
                CVector pos = ZeroVector;
                m3d::SafeVectorAttrib(pos, pointNode, "Pos");
                float const hover = prototypeInfo->m_hoverForPlacingDrones;
                CVector const groundPos = GetGroundPos(pos, true, false);
                m_pointsForDrones.push_back(CVector(groundPos.x, hover + groundPos.y, groundPos.z));
                pointNode->GetNextSibling(pointNode, "Point");
            }
        }

        ref_ptr shootPlacesNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        xmlNode->GetFirstChild(shootPlacesNode, "ShootPlaces");
        if (!shootPlacesNode->IsEmpty() && shootPlacesNode->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            ref_ptr pointNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
            shootPlacesNode->GetFirstChild(pointNode, "Point");
            while (!pointNode->IsEmpty())
            {
                CVector pos = ZeroVector;
                m3d::SafeVectorAttrib(pos, pointNode, "Pos");
                float const hover = prototypeInfo->m_defaultHover;
                CVector const groundPos = GetGroundPos(pos, true, false);
                m_pointsForShooting.push_back(CVector(groundPos.x, hover + groundPos.y, groundPos.z));
                pointNode->GetNextSibling(pointNode, "Point");
            }
        }
    }

    void Boss03::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x73B5F0
        ComplexPhysicObj::LoadRuntimeValues(xmlFile, xmlNode);
        float health = m_health.value().get();
        m3d::SafeFloatAttrib(health, xmlNode, "Health");
        m_health.value().set(health);
        m3d::SafeVectorAttrib(m_linearVelocity, xmlNode, "CurLinearVelocity");
        m3d::SafeVectorAttrib(m_relAngularVelocity, xmlNode, "CurRelAngularVelocity");
        m3d::SafeEnumAttrib(m_liveStatus, xmlNode, "LiveStatus");
        m3d::SafeEnumAttrib(m_droneSpawningStatus, xmlNode, "DroneSpawningStatus");
        m3d::SafeFloatAttrib(m_dronePlacingTimeout, xmlNode, "DronePlacingTimeout");
        m3d::SafeFloatAttrib(m_shootingTimeout, xmlNode, "ShootingTimeout");
        m3d::SafeEnumAttrib(m_pathTrackingStatus, xmlNode, "PathTrackingStatus");
        m3d::SafeVectorAttrib(m_desiredDestination, xmlNode, "DesiredDestination");
        m3d::SafeVectorAttrib(m_desiredDirection, xmlNode, "DesiredDirection");
        m3d::SafeFloatAttrib(m_currentFlyTime, xmlNode, "CurrentFlyTime");
        m3d::SafeFloatAttrib(m_keyPartsMaxDurability, xmlNode, "KeyPartsMaxDurability");
        ref_ptr flyPathNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        xmlNode->GetFirstChild(flyPathNode, "CurrentFlyPath");
        if (!flyPathNode->IsEmpty())
        {
            // NOTE: a path the boss already has is leaked.
            m_currentFlyPath = new m3d::CameraPath();
            m_currentFlyPath->LoadFromXmlRuntime(xmlFile, flyPathNode);
        }
    }

    void Boss03::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x738F90
        ComplexPhysicObj::SaveToXML(xmlFile, xmlNode);
        xmlNode->SetAttribute("PathNameForFlyingWithWings", m_pathNameForFlyingWithWings.c_str());
        ref_ptr dronePlacesNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "DronePlaces");
        xmlNode->AddChild(dronePlacesNode);
        for (auto const& point : m_pointsForDrones)
        {
            ref_ptr pointNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Point");
            dronePlacesNode->AddChild(pointNode);
            pointNode->SetAttribute("Pos", CStr(point).c_str());
        }
        ref_ptr shootPlacesNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "ShootPlaces");
        xmlNode->AddChild(shootPlacesNode);
        for (auto const& point : m_pointsForShooting)
        {
            ref_ptr pointNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Point");
            shootPlacesNode->AddChild(pointNode);
            pointNode->SetAttribute("Pos", CStr(point).c_str());
        }
    }

    void Boss03::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x739220
        ComplexPhysicObj::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("Health", CStr(m_health.value().get()).c_str());
        xmlNode->SetAttribute("CurLinearVelocity", CStr(m_linearVelocity).c_str());
        xmlNode->SetAttribute("CurRelAngularVelocity", CStr(m_relAngularVelocity).c_str());
        xmlNode->SetAttribute("LiveStatus", CStr(static_cast<int>(m_liveStatus)).c_str());
        xmlNode->SetAttribute("DroneSpawningStatus", CStr(static_cast<int>(m_droneSpawningStatus)).c_str());
        xmlNode->SetAttribute("DronePlacingTimeout", CStr(m_dronePlacingTimeout).c_str());
        xmlNode->SetAttribute("ShootingTimeout", CStr(m_shootingTimeout).c_str());
        xmlNode->SetAttribute("PathTrackingStatus", CStr(static_cast<int>(m_pathTrackingStatus)).c_str());
        xmlNode->SetAttribute("DesiredDestination", CStr(m_desiredDestination).c_str());
        xmlNode->SetAttribute("DesiredDirection", CStr(m_desiredDirection).c_str());
        xmlNode->SetAttribute("CurrentFlyTime", CStr(m_currentFlyTime).c_str());
        xmlNode->SetAttribute("KeyPartsMaxDurability", CStr(m_keyPartsMaxDurability).c_str());
        if (m_currentFlyPath)
        {
            ref_ptr flyPathNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "CurrentFlyPath");
            m_currentFlyPath->SaveToXmlRuntime(xmlFile, flyPathNode);
            xmlNode->AddChild(flyPathNode);
        }
    }

    void Boss03::Update(float elapsedTime, unsigned workTime)
    {
        // RVA 0x73D550
        // Stage one places drones until the spawner cabin is shot off, stage two flies the path
        // until both wings are gone, and the small helicopter left dies with its health.
        AnimatedComplexPhysicObj::Update(elapsedTime, workTime);
        if (elapsedTime <= 0.0)
        {
            return;
        }
        switch (m_liveStatus)
        {
        case LIVE_PLACING_DRONES:
            if (_GetBossPartByName(DRONE_SPAWNER_PART_NAME))
            {
                _UpdateLivePlacingDrones(elapsedTime);
            }
            else
            {
                _SetLiveStatus(LIVE_FIGHTING_WITH_WINGS);
            }
            break;
        case LIVE_FIGHTING_WITH_WINGS:
            if (!_GetBossPartByName(WING01_PART_NAME) && !_GetBossPartByName(WING02_PART_NAME))
            {
                _SetLiveStatus(LIVE_DEAD);
            }
            else
            {
                _UpdateLiveFightingWithWings(elapsedTime);
            }
            break;
        case LIVE_SMALL_HELICOPTER:
            if (m_health.value().get() == m_health.minValue().get())
            {
                _SetLiveStatus(LIVE_DEAD);
            }
            break;
        default:
            break;
        }
        if (m_liveStatus != LIVE_IDLE && m_liveStatus != LIVE_DEAD)
        {
            WeaponFirer::AimAndFireFromWeapons(this, true, elapsedTime, gDynamicScene->GetVehicleControlledByPlayer());
        }
    }

    bool Boss03::ApplyModifier(Modifier const& modifier)
    {
        // RVA 0x739EB0 - NOTE: chains straight to Obj::ApplyModifier, skipping the physic object
        // classes.
        if (Obj::ApplyModifier(modifier))
        {
            return true;
        }
        if (modifier.m_PropertyName == "hp")
        {
            m_health.value().ApplyModifier(modifier, m_health.maxValue().get());
            return true;
        }
        return false;
    }

    void Boss03::InflictDamage(DamageInfo const& damageInfo)
    {
        // RVA 0x73CCF0
        // The hit part wears down, and so does the boss's own health (which only counts once the
        // small helicopter is left).
        SetLastDamageSource(damageInfo.attackerId);
        if (damageInfo.damagedPartName.empty())
        {
            M3D_LOG_ERR(CStr("Error: vehicle part with empty name damaged"));
            return;
        }
        VehiclePart* const part = GetPartByName(damageInfo.damagedPartName);
        if (!part)
        {
            M3D_LOG_ERR(CStr("Error: unknown vehicle part damaged: '") + damageInfo.damagedPartName + CStr("'"));
            return;
        }
        float const damage = damageInfo.damage;

        Modifier modToDurability;
        modToDurability.Create("dur", MO_SUB, m3d::AIParam(damage));
        part->AddModifier(modToDurability);

        Modifier modToHealth;
        modToHealth.Create("hp", MO_SUB, m3d::AIParam(damage));
        modToHealth.m_SenderID = damageInfo.attackerId;
        AddModifier(modToHealth);

        if (bIsUpdatingByODE() && damageInfo.damageType != DAMAGE_WATER && damageInfo.damageType != DAMAGE_BLAST)
        {
            VehiclePart::BreakData breakData;
            breakData.point = damageInfo.hitPos;
            breakData.dir = damageInfo.hitDir;
            breakData.normal = damageInfo.normal;
            breakData.decalId = damageInfo.decalId;
            breakData.damage = damage;
            part->BreakModel(breakData);
        }
    }

    bool Boss03::StartPlacingDrone()
    {
        // RVA 0x73B8D0
        if (!m_droneTeam)
        {
            SYS_ERROR("m_droneTeam");
        }
        if (static_cast<int>(m_droneTeam->GetNumVehicles()) >= GetPrototypeInfo()->m_maxDrones)
        {
            return false;
        }
        _SetDroneSpawningStatus(DRONE_START_PLACING);
        return true;
    }

    void Boss03::StartMoving(CVector const& destination, CVector const& finalDirection)
    {
        // RVA 0x737E60
        m_desiredDestination = destination;
        m_desiredDirection = finalDirection;
        m_pathTrackingStatus = PATH_SETTING_HORIZONTAL_BEFORE;
    }

    void Boss03::NoticePlayer()
    {
        // RVA 0x73D680
        if (m_liveStatus == LIVE_IDLE)
        {
            _GetBossPartByName(DRONE_SPAWNER_PART_NAME)->SetDamageable(true);
            m_liveStatus = LIVE_PLACING_DRONES;
        }
    }

    float Boss03::GetMaxHealth() const
    {
        // RVA 0x737B10
        return m_keyPartsMaxDurability;
    }

    float Boss03::GetHealth() const
    {
        // RVA 0x73C660 - the durability left on the key parts still attached.
        retruxx::vector<CStr> keyPartNames;
        keyPartNames.push_back(DRONE_SPAWNER_PART_NAME);
        keyPartNames.push_back(WING01_PART_NAME);
        keyPartNames.push_back(WING02_PART_NAME);
        float keyPartsDurability = 0.0f;
        for (auto const& name : keyPartNames)
        {
            if (VehiclePart const* const part = GetPartByName(name))
            {
                keyPartsDurability = part->Durability().value().get() + keyPartsDurability;
            }
        }
        return keyPartsDurability;
    }

    void Boss03::_InternalPostLoad()
    {
        // RVA 0x73CB10
        // NOTE: calls PhysicObj::_InternalPostLoad directly, skipping the complex object classes.
        PhysicObj::_InternalPostLoad();
        if (theObjects->m_SaveType != ObjContainer::SAVE_FULL)
        {
            m_keyPartsMaxDurability = CalcKeyPartsMaxDurability();
        }
    }

    void Boss03::_InternalCreateVisualPart()
    {
        // RVA 0x739F00
        ComplexPhysicObj::_InternalCreateVisualPart();
        _GetBossPartByName(MAIN_PART_NAME)->SetNodeAction(AT_ATTACK1, true);
    }

    void Boss03::_Construct(bool bForAnimation)
    {
        // RVA 0x737EB0
        ComplexPhysicObj::_Construct(bForAnimation);
        DisablePhysics();
        m_physicState |= 1;
        SetCorrectEnabledCellsCounter();
        EnableGeometry(false);
    }

    void Boss03::_UpdateLivePlacingDrones(float elapsedTime)
    {
        // RVA 0x73BAE0
        // Flies to a random drone place, opens the spawner and releases a drone, then flies to a
        // random shooting place and turns to face the player for a while. Once the team is full it
        // only shoots.
        switch (m_droneSpawningStatus)
        {
        case DRONE_DECIDING:
            if (!m_droneTeam)
            {
                SYS_ERROR("m_droneTeam");
            }
            if (static_cast<int>(m_droneTeam->GetNumVehicles()) >= GetPrototypeInfo()->m_maxDrones)
            {
                _GetBossPartByName(MAIN_PART_NAME)->SetAnimationStopped(false);
                if (!m_pointsForShooting.empty())
                {
                    CVector const destination = m_pointsForShooting[IntRandom(static_cast<unsigned>(m_pointsForShooting.size()))];
                    CVector const pos = GetPosition();
                    StartMoving(destination, CVector(destination.x - pos.x, destination.y - pos.y, destination.z - pos.z).getNormalized());
                }
                m_droneSpawningStatus = DRONE_FLYING_TO_SHOOT;
            }
            else
            {
                _SetDroneSpawningStatus(DRONE_FLYING_TO_SPAWN);
            }
            break;

        case DRONE_FLYING_TO_SPAWN:
            _UpdatePathTracking(elapsedTime);
            if (m_pathTrackingStatus == PATH_IDLE)
            {
                _SetDroneSpawningStatus(DRONE_START_PLACING);
            }
            break;

        case DRONE_START_PLACING:
            if (_GetBossPartByName(DRONE_SPAWNER_PART_NAME)->GetNodeRealAnimAction() != AT_MOVE1)
            {
                _SpawnDroneNow();
                m_dronePlacingTimeout = DRONE_PLACING_TIME;
                m_droneSpawningStatus = DRONE_PLACING_NOW;
            }
            break;

        case DRONE_PLACING_NOW:
            m_dronePlacingTimeout -= elapsedTime;
            if (m_dronePlacingTimeout < 0.0)
            {
                _GetBossPartByName(DRONE_SPAWNER_PART_NAME)->SetNodeAction(AT_MOVE2, true);
                m_droneSpawningStatus = DRONE_END_PLACING;
            }
            break;

        case DRONE_END_PLACING:
            if (_GetBossPartByName(DRONE_SPAWNER_PART_NAME)->GetNodeRealAnimAction() != AT_MOVE2)
            {
                _GetBossPartByName(MAIN_PART_NAME)->SetAnimationStopped(false);
                if (!m_pointsForShooting.empty())
                {
                    CVector const destination = m_pointsForShooting[IntRandom(static_cast<unsigned>(m_pointsForShooting.size()))];
                    CVector const pos = GetPosition();
                    StartMoving(destination, CVector(destination.x - pos.x, destination.y - pos.y, destination.z - pos.z).getNormalized());
                }
                m_droneSpawningStatus = DRONE_FLYING_TO_SHOOT;
            }
            break;

        case DRONE_FLYING_TO_SHOOT:
            _UpdatePathTracking(elapsedTime);
            if (m_pathTrackingStatus == PATH_IDLE)
            {
                m_shootingTimeout = RandomUniformFloat(0.69999999f, 1.0f) * GetPrototypeInfo()->m_maxShootingTime;
                m_droneSpawningStatus = DRONE_SHOOTING;
            }
            break;

        case DRONE_SHOOTING:
            if (Vehicle* const playerVehicle = gDynamicScene->GetVehicleControlledByPlayer())
            {
                CVector const pos = GetPosition();
                CVector const playerPos = playerVehicle->GetPosition();
                _ProceedSettingHorizontalDirection(elapsedTime, CVector(playerPos.x - pos.x, 0.0f, playerPos.z - pos.z));
                m_shootingTimeout -= elapsedTime;
                if (m_shootingTimeout < 0.0)
                {
                    m_droneSpawningStatus = DRONE_DECIDING;
                }
            }
            break;

        default:
            break;
        }
    }

    void Boss03::_UpdateLiveFightingWithWings(float elapsedTime)
    {
        // RVA 0x73D080 - flies the named path over and over, facing the player.
        if (!m_currentFlyPath)
        {
            SYS_ERROR("m_currentFlyPath");
        }
        CVector pos = GetPosition();
        Quaternion rot;
        float zoom;
        m_currentFlyPath->GetCameraForTime(m_currentFlyTime, pos, rot, zoom);
        SetPosition(pos);
        m_currentFlyTime += elapsedTime;
        if (m_currentFlyTime >= m_currentFlyPath->GetFullTime())
        {
            _RecalcFlyPath();
        }
        _UpdateLookAtPlayer(elapsedTime);
    }

    void Boss03::_UpdatePathTracking(float elapsedTime)
    {
        // RVA 0x739560
        // Levels out, turns to the destination, tilts forward, flies there, levels out again and
        // turns to the final direction.
        Boss03PrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        switch (m_pathTrackingStatus)
        {
        case PATH_IDLE:
            return;

        case PATH_SETTING_HORIZONTAL_BEFORE:
            if (_ProceedSettingVerticalAngle(elapsedTime, 0.0f))
            {
                m_relAngularVelocity = ZeroVector;
                m_pathTrackingStatus = PATH_SETTING_DIRECTION_TO_MOVE;
            }
            break;

        case PATH_SETTING_DIRECTION_TO_MOVE:
        {
            CVector const pos = GetPosition();
            CVector const desiredDir(m_desiredDestination.x - pos.x, m_desiredDestination.y - pos.y, m_desiredDestination.z - pos.z);
            if (_ProceedSettingHorizontalDirection(elapsedTime, desiredDir))
            {
                m_relAngularVelocity = ZeroVector;
                m_pathTrackingStatus = PATH_TILTING;
            }
            break;
        }

        case PATH_TILTING:
            if (_ProceedSettingVerticalAngle(elapsedTime, 0.0f - prototypeInfo->m_pathTrackTiltAngle))
            {
                m_relAngularVelocity = ZeroVector;
                m_pathTrackingStatus = PATH_MOVING;
            }
            break;

        case PATH_MOVING:
        {
            Boss03PrototypeInfo const* const info = GetPrototypeInfo();
            CVector const pos = GetPosition();
            CVector dirToMove(m_desiredDestination.x - pos.x, m_desiredDestination.y - pos.y, m_desiredDestination.z - pos.z);
            float const distSq = dirToMove.z * dirToMove.z + dirToMove.y * dirToMove.y + dirToMove.x * dirToMove.x;
            dirToMove.normalizeInplace();
            float const dist = static_cast<float>(sqrt(distSq));
            float const curVel = m_linearVelocity.z * dirToMove.z + m_linearVelocity.y * dirToMove.y + m_linearVelocity.x * dirToMove.x;
            float step = 0.0f;
            float newVel = 0.0f;
            bool const arrived =
                GetSmoothAcceleratedValue(elapsedTime, dist, 0.0f, curVel, info->m_linearAcceleration, info->m_maxLinearVelocity, step, newVel);
            m_linearVelocity = CVector(dirToMove.x * newVel, dirToMove.y * newVel, dirToMove.z * newVel);
            CVector const curPos = GetPosition();
            SetPosition(CVector(curPos.x + dirToMove.x * step, curPos.y + dirToMove.y * step, curPos.z + dirToMove.z * step));
            if (arrived)
            {
                m_linearVelocity = ZeroVector;
                m_pathTrackingStatus = PATH_SETTING_HORIZONTAL_AFTER;
            }
            break;
        }

        case PATH_SETTING_HORIZONTAL_AFTER:
            if (_ProceedSettingVerticalAngle(elapsedTime, 0.0f))
            {
                m_relAngularVelocity = ZeroVector;
                m_pathTrackingStatus = PATH_SETTING_FINAL_DIRECTION;
            }
            break;

        case PATH_SETTING_FINAL_DIRECTION:
            if (_ProceedSettingHorizontalDirection(elapsedTime, m_desiredDirection))
            {
                m_relAngularVelocity = ZeroVector;
                m_pathTrackingStatus = PATH_IDLE;
            }
            break;

        default:
            SYS_ERROR("0");
            break;
        }
    }

    void Boss03::_UpdateSwinging(float)
    {
        // RVA 0x737AD0 - does nothing.
    }

    void Boss03::_SetLiveStatus(LiveStatus newLiveStatus)
    {
        // RVA 0x73D2C0
        switch (newLiveStatus)
        {
        case LIVE_PLACING_DRONES:
            _GetBossPartByName(DRONE_SPAWNER_PART_NAME)->SetDamageable(true);
            break;

        case LIVE_FIGHTING_WITH_WINGS:
            _GetBossPartByName(MAIN_PART_NAME)->SetAnimationStopped(false);
            _GetBossPartByName(WING01_PART_NAME)->SetDamageable(true);
            _GetBossPartByName(WING02_PART_NAME)->SetDamageable(true);
            _RecalcFlyPath();
            break;

        case LIVE_SMALL_HELICOPTER:
            break;

        case LIVE_DEAD:
        {
            // Stop firing, char the model and blow every part off.
            WeaponFirer::FireFromWeaponsIfPossible(this, false, ZeroVector, nullptr);
            SetSkin(8);
            retruxx::vector<VehiclePart*> allParts;
            for (auto const& part : m_vehicleParts)
            {
                allParts.push_back(part.second);
            }
            for (VehiclePart* const part : allParts)
            {
                Flow(part, 15.0f);
            }
            Remove();
            break;
        }

        default:
            SYS_ERROR("0");
            break;
        }
        m_liveStatus = newLiveStatus;
    }

    void Boss03::_SetDroneSpawningStatus(DroneSpawningStatus newSpawinigStatus)
    {
        // RVA 0x73ACE0
        switch (newSpawinigStatus)
        {
        case DRONE_DECIDING:
            break;

        case DRONE_FLYING_TO_SPAWN:
            _GetBossPartByName(MAIN_PART_NAME)->SetAnimationStopped(false);
            if (!m_pointsForDrones.empty())
            {
                CVector const pos = m_pointsForDrones[IntRandom(static_cast<unsigned>(m_pointsForDrones.size()))];
                CVector const curPos = GetPosition();
                StartMoving(pos, CVector(pos.x - curPos.x, pos.y - curPos.y, pos.z - curPos.z).getNormalized());
            }
            break;

        case DRONE_START_PLACING:
            _GetBossPartByName(MAIN_PART_NAME)->SetAnimationStopped(true);
            if (m_liveStatus != LIVE_PLACING_DRONES)
            {
                SYS_ERROR("m_liveStatus == LIVE_PLACING_DRONES");
            }
            if (m_droneSpawningStatus != DRONE_FLYING_TO_SPAWN)
            {
                SYS_ERROR("m_droneSpawningStatus == DRONE_FLYING_TO_SPAWN");
            }
            _GetBossPartByName(DRONE_SPAWNER_PART_NAME)->SetNodeAction(AT_MOVE1, true);
            break;

        case DRONE_PLACING_NOW:
            _SpawnDroneNow();
            m_droneSpawningStatus = newSpawinigStatus;
            m_dronePlacingTimeout = DRONE_PLACING_TIME;
            return;

        case DRONE_END_PLACING:
            _GetBossPartByName(DRONE_SPAWNER_PART_NAME)->SetNodeAction(AT_MOVE2, true);
            break;

        case DRONE_FLYING_TO_SHOOT:
            _GetBossPartByName(MAIN_PART_NAME)->SetAnimationStopped(false);
            if (!m_pointsForShooting.empty())
            {
                CVector const pos = m_pointsForShooting[IntRandom(static_cast<unsigned>(m_pointsForShooting.size()))];
                CVector const curPos = GetPosition();
                StartMoving(pos, CVector(pos.x - curPos.x, pos.y - curPos.y, pos.z - curPos.z).getNormalized());
            }
            break;

        case DRONE_SHOOTING:
            m_droneSpawningStatus = newSpawinigStatus;
            m_shootingTimeout = RandomUniformFloat(0.69999999f, 1.0f) * GetPrototypeInfo()->m_maxShootingTime;
            return;

        default:
            SYS_ERROR("0");
            break;
        }
        m_droneSpawningStatus = newSpawinigStatus;
    }

    void Boss03::_SpawnDroneNow()
    {
        // RVA 0x73A2D0
        // A random drone is released at the spawner's load point, level and falling slowly, and
        // joins the drone team.
        Boss03PrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        if (prototypeInfo->m_dronePrototypeIds.empty())
        {
            return;
        }
        if (!m_droneTeam)
        {
            SYS_ERROR("m_droneTeam");
        }
        if (static_cast<int>(m_droneTeam->GetNumVehicles()) >= prototypeInfo->m_maxDrones)
        {
            return;
        }
        unsigned const idx = (static_cast<unsigned>(prototypeInfo->m_dronePrototypeIds.size()) * rand()) >> 15;
        int const droneId = theObjects->CreateNewObject(prototypeInfo->m_dronePrototypeIds[idx], "", -1, GetBelong());
        PhysicObj* drone = nullptr;
        if (droneId >= 0)
        {
            drone = static_cast<PhysicObj*>(theObjects->GetEntityByObjId(droneId));
        }
        Boss03Part* const spawner = _GetBossPartByName(DRONE_SPAWNER_PART_NAME);
        CVector const droneSpawnerPos = spawner->GetNodeAbsolutePosition();
        Quaternion const droneSpawnerRot = spawner->GetNodeAbsoluteRotation();
        // NOTE: the drone is used without a null check.
        CVector const offset = RotateByQuaternion(prototypeInfo->m_droneRelPosition, droneSpawnerRot);
        drone->SetPosition(CVector(offset.x + droneSpawnerPos.x, droneSpawnerPos.y + offset.y, droneSpawnerPos.z + offset.z));
        drone->SetRotation(QuatMul(droneSpawnerRot, prototypeInfo->m_droneRelRotation));
        CVector droneDir = drone->GetDirection();
        droneDir.y = 0.0f;
        float const invLen = static_cast<float>(1.0 / sqrt(droneDir.z * droneDir.z + droneDir.x * droneDir.x + 1.1920929e-7));
        drone->SetDirection(CVector(droneDir.x * invLen, invLen * 0.0f, droneDir.z * invLen));
        drone->SetLinearVelocity(CVector(0.0f, -2.0f, 0.0f));
        m_droneTeam->AddChild(drone);
    }

    void Boss03::_SetPathTrackingStatus(PathTrackingStatus newPathTrackingStatus)
    {
        // RVA 0x737AE0
        m_pathTrackingStatus = newPathTrackingStatus;
    }

    bool Boss03::_ProceedSettingVerticalAngle(float elapsedTime, float desiredAngle)
    {
        // RVA 0x737EF0 - pitches around the boss's own left axis; true once the angle is reached.
        Boss03PrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        CVector const dir = GetDirection();
        float const angle = static_cast<float>(asin(dir.y / sqrt(dir.z * dir.z + dir.x * dir.x + dir.y * dir.y)));
        float newAngle = 0.0f;
        float newAngularVel = 0.0f;
        bool const reached = GetSmoothAcceleratedValue(
            elapsedTime,
            desiredAngle,
            angle,
            m_relAngularVelocity.x,
            prototypeInfo->m_vertAngularAcceleration,
            prototypeInfo->m_maxVertAngularVelocity,
            newAngle,
            newAngularVel);
        m_relAngularVelocity.x = newAngularVel;
        SetRotation(QuatMul(GetRotation(), AxisAngle(INITIAL_LEFT_DIRECTION, newAngle - angle)));
        return reached;
    }

    bool Boss03::_ProceedSettingHorizontalDirection(float elapsedTime, CVector const& desiredDir)
    {
        // RVA 0x738100 - yaws around the world up axis; true once facing desiredDir.
        Boss03PrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        CVector const dir = GetDirection();
        // desiredDir in the boss's own heading frame, so the target yaw is relative.
        Quaternion const toLocal = AxisAngle(INITIAL_UP_DIRECTION, static_cast<float>(-atan2(dir.x, dir.z)));
        CVector const dirToMove = RotateByQuaternion(desiredDir, toLocal);
        float const destination = static_cast<float>(atan2(dirToMove.x, dirToMove.z));
        float newAngle = 0.0f;
        float newAngularVel = 0.0f;
        bool const reached = GetSmoothAcceleratedValue(
            elapsedTime,
            destination,
            0.0f,
            m_relAngularVelocity.y,
            prototypeInfo->m_horizAngularAcceleration,
            prototypeInfo->m_maxHorizAngularVelocity,
            newAngle,
            newAngularVel);
        m_relAngularVelocity.y = newAngularVel;
        SetRotation(QuatMul(AxisAngle(INITIAL_UP_DIRECTION, newAngle), GetRotation()));
        return reached;
    }

    void Boss03::_UpdateLookAtPlayer(float elapsedTime)
    {
        // RVA 0x738580 - turns to the player and pitches towards them, within the tilt limit.
        Vehicle* const playerVehicle = gDynamicScene->GetVehicleControlledByPlayer();
        if (!playerVehicle)
        {
            return;
        }
        Boss03PrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        CVector const pos = GetPosition();
        CVector const playerPos = playerVehicle->GetPosition();
        CVector const dirToPlayer(playerPos.x - pos.x, playerPos.y - pos.y, playerPos.z - pos.z);
        _ProceedSettingHorizontalDirection(elapsedTime, dirToPlayer);
        float const maxTilt = prototypeInfo->m_pathTrackTiltAngle;
        float angle = static_cast<float>(atan2(dirToPlayer.y, sqrt(dirToPlayer.x * dirToPlayer.x + dirToPlayer.z * dirToPlayer.z)));
        if (0.0f - maxTilt > angle)
        {
            angle = 0.0f - maxTilt;
        }
        if (angle > maxTilt)
        {
            angle = maxTilt;
        }
        _ProceedSettingVerticalAngle(elapsedTime, angle);
    }

    Boss03Part* Boss03::_GetBossPartByName(CStr const& partName)
    {
        // RVA 0x739AA0
        VehiclePart* const res = GetPartByName(partName);
        if (res && !res->IsKindOf(RT_CLASS_LOCAL(Boss03Part)))
        {
            SYS_ERROR("!res || IS_KIND_OF( res, Boss03Part )");
        }
        return static_cast<Boss03Part*>(res);
    }

    Boss03Part* Boss03::_GetDroneSpawner()
    {
        // RVA 0x739C10
        return _GetBossPartByName(DRONE_SPAWNER_PART_NAME);
    }

    Boss03Part* Boss03::_GetMainPart()
    {
        // RVA 0x739C20
        return _GetBossPartByName(MAIN_PART_NAME);
    }

    void Boss03::_SetAllPartsNotDamageable()
    {
        // RVA 0x73B260
        for (auto const& part : m_vehicleParts)
        {
            if (part.second->IsKindOf(RT_CLASS_LOCAL(Boss03Part)))
            {
                static_cast<Boss03Part*>(part.second)->SetDamageable(false);
            }
        }
    }

    void Boss03::_RecalcFlyPath()
    {
        // RVA 0x73CB40
        // A fresh copy of the named path starting from where the boss is now, timed for its
        // maximum speed.
        // NOTE: the camera paths file is reloaded every time.
        m3d::Cinematic* const cinematic = M3D_APP->m_cinematic;
        cinematic->Load("camera_paths.xml");
        delete m_currentFlyPath;
        m_currentFlyPath = nullptr;
        m_currentFlyPath = new m3d::CameraPath(cinematic->GetPathByName(m_pathNameForFlyingWithWings));
        m3d::CameraPathState state;
        state.m_rotation = GetRotation();
        state.m_point = GetPosition();
        state.m_zoom = 1.0f;
        state.m_speed = 1.0f;
        m_currentFlyPath->insert(0, state);
        m_currentFlyPath->CalcFullLength(1);
        m_currentFlyPath->SetFullTime(m_currentFlyPath->GetFullLength() / GetPrototypeInfo()->m_maxLinearVelocity);
        m_currentFlyPath->CalcFlyTimes(1, false);
        m_currentFlyTime = 0.0f;
    }

    bool Boss03::_OnHealthValueBeforeApplyModifier(Modifier const&, float&)
    {
        // RVA 0x737AF0 - health only takes damage once the small helicopter is left.
        return m_liveStatus != LIVE_SMALL_HELICOPTER;
    }

    CStr const& Boss03::_LiveStatusToStr(LiveStatus status)
    {
        // RVA 0x739C30
        static CStr const LIVE_STATUS_NAMES[] = {
            CStr("LIVE_PLACING_DRONES"),
            CStr("LIVE_FIGHTING_WITH_WINGS"),
            CStr("LIVE_SMALL_HELICOPTER"),
            CStr("LIVE_DEAD"),
            CStr(),
        };
        return LIVE_STATUS_NAMES[status];
    }

    CStr const& Boss03::_DroneSpawningStatusToStr(DroneSpawningStatus status)
    {
        // RVA 0x739CC0
        static CStr const DRONE_STATUS_NAMES[] = {
            CStr("DRONE_DECIDING"),
            CStr("DRONE_FLYING_TO_SPAWN"),
            CStr("DRONE_START_PLACING"),
            CStr("DRONE_PLACING_NOW"),
            CStr("DRONE_END_PLACING"),
            CStr("DRONE_FLYING_TO_SHOOT"),
            CStr("DRONE_SHOOTING"),
        };
        return DRONE_STATUS_NAMES[status];
    }

    float Boss03::CalcKeyPartsMaxDurability() const
    {
        // RVA 0x73C4F0 - the prototype durability of the key parts the boss is built with.
        Boss03PrototypeInfo const* const pi = GetPrototypeInfo();
        if (!pi)
        {
            return 0.0f;
        }
        retruxx::vector<CStr> keyPartNames;
        keyPartNames.push_back(DRONE_SPAWNER_PART_NAME);
        keyPartNames.push_back(WING01_PART_NAME);
        keyPartNames.push_back(WING02_PART_NAME);
        float keyPartsMaxDurability = 0.0f;
        for (auto const& name : keyPartNames)
        {
            auto const it = pi->m_partPrototypeIds.find(name);
            if (it == pi->m_partPrototypeIds.end())
            {
                continue;
            }
            PrototypeInfo const* const partInfo = thePrototypeManager->GetPrototypeInfo(it->second);
            if (partInfo && partInfo->IsPrototypeOf(RT_CLASS_LOCAL(VehiclePart)))
            {
                keyPartsMaxDurability = static_cast<VehiclePartPrototypeInfo const*>(partInfo)->m_durability + keyPartsMaxDurability;
            }
        }
        return keyPartsMaxDurability;
    }
}  // namespace ai
