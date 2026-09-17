#include "simplephysicobj.h"

#include "server/objects/physicbodies/physichelpers.h"
#include <stdexcept>
#include <ode/collision.h>
#include <ode/objects.h>

#include "server/objects/base/objcontainer.h"
#include "server/objects/base/prototypemanager.h"
#include "core/ini.h"
#include "core/kernel.h"
#include "core/log.h"
#include "core/timer.h"
#include "game/m3dgame.h"
#include "ode/odecpp.h"
#include "scene/servers/serveranimatedmodel.h"
#include "server/dynamicscene.h"
#include "server/objects/physicbodies/boxybody.h"
#include "server/objects/physicbodies/genericbody.h"
#include "server/objects/physicbodies/raybody.h"
#include "server/objects/physicbodies/sphericbody.h"
#include "server/objects/physicbodies/geoms/box.h"

RT_CLASS_EXPORT_METHOD_DEFINE(SimplePhysicObj, SetMass)
{
    // RVA 0x7F5B10
    auto* const obj = static_cast<ai::SimplePhysicObj*>(context->asObject(0, "SimplePhysicObj"));
    obj->SetMass(context->asFloat(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(SimplePhysicObj, SetNodeAction)
{
    auto obj = (ai::SimplePhysicObj*)context->asObject(0, "SimplePhysicObj");
    auto action = context->asInt(1);
    auto restart = context->asBool(2);
    obj->SetNodeAction(action, restart);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(SimplePhysicObj, SetNextForAnimation)
{
    auto obj = (ai::SimplePhysicObj*)context->asObject(0, "SimplePhysicObj");
    auto action = context->asInt(1);
    auto next = context->asInt(2);
    obj->SetNextForAnimation(action, next);
    return 1;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(SimplePhysicObj)
    RT_CLASS_EXPORT(SimplePhysicObj, m3d::METHOD, SetMass, "", "", "")
    RT_CLASS_EXPORT(SimplePhysicObj, m3d::METHOD, SetNodeAction, "", "", "")
    RT_CLASS_EXPORT(SimplePhysicObj, m3d::METHOD, SetNextForAnimation, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SimplePhysicObj);

    SimplePhysicObjPrototypeInfo::~SimplePhysicObjPrototypeInfo() = default;

    CStr const& SimplePhysicObjPrototypeInfo::GetEngineModelName() const
    {
        return m_engineModelName;
    }

    bool SimplePhysicObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        int const result = ai::PhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_massValue, xmlNode, "Mass");
            m3d::SafeStrAttrib(m_engineModelName, xmlNode, "ModelFile");
            m3d::SafeBoolAttrib(m_bCollisionTrimeshAllowed, xmlNode, "CollisionTrimeshAllowed");
        }
        return result;
    }

    SimplePhysicObjPrototypeInfo::SimplePhysicObjPrototypeInfo()
    {
        this->m_bCollisionTrimeshAllowed = 0;
        this->m_geomType = GEOM_TYPE_NONE;
        this->m_size = CVector(0.0, 0.0, 0.0);
        this->m_radius = 1.0;
        this->m_massValue = 1.0;
    }

    float SimplePhysicObjPrototypeInfo::GetRadius() const
    {
        // RVA 0x719180
        return m_radius;
    }

    SimplePhysicBody* SimplePhysicObjPrototypeInfo::CreatePhysicBody() const
    {
        SimplePhysicBody* body = nullptr;
        switch (this->m_geomType)
        {
        case GEOM_TYPE_BOX:
            body = new BoxyBody(m_collisionInfos, m_massValue);
            break;

        case GEOM_TYPE_SPHERE:
            body = new SphericBody(m_collisionInfos, m_massValue);
            break;

        case GEOM_TYPE_RAY:
            body = new RayBody(1000.0);
            break;

        case GEOM_TYPE_TRIMESH:
            SYS_ERROR("!\"obsolete\"");
            break;

        case GEOM_TYPE_FROM_MODEL:
        {
            body = new GenericBody(1.0);
            body->SetCollisionTrimeshAllowed(m_bCollisionTrimeshAllowed);
            body->ChangePhysicBodyByCollisionInfo(m_collisionInfos);
            body->SetMass(m_massValue);
            break;
        }

        default:
            break;
        }

        // RVA 0x7F6570 - an unsupported geometry type still gets a (disabled) box body.
        if (!body)
        {
            SYS_ERROR("body");
            body = new BoxyBody(m_collisionInfos, m_massValue);
            body->DisableGeometry();
        }

        if (body->m_pGeoms.empty())
        {
            auto obj = ai::GeomTransform::CreateObject(ai::gGlobalSpace, ai::CommonGeomMovedCallback);
            body->m_pGeoms.push_back(obj);

            auto box = ai::Box::CreateObject(0, {1.0, 1.0, 1.0}, 0);
            obj->SetGeom(box);
        }
        body->SetModelNameUnsafe(m_engineModelName);
        for (auto& geom : body->m_pGeoms)
        {
            dGeomSetData(geom->GetGeomId(), body);
        }
        return body;
    }

    void SimplePhysicObjPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto* serverAnimatedModels = (m3d::AnimatedModelsServer*)&M3D_APP->GetAnimatedModelsServer();
        auto const sizes = serverAnimatedModels->GetBoundSizes(m_engineModelName.c_str());
        auto const item = serverAnimatedModels->GetItemByName(m_engineModelName.c_str(), true);
        GetCollisionInfoByServerHandle(item, m_collisionInfos, m_bCollisionTrimeshAllowed);
        m_size = m_collisionInfos[0].m_size;
        m3d::SafeVectorAttrib(m_size, xmlNode, "Size");
        m_radius = sizes.y * 0.5;
        m3d::SafeFloatAttrib(m_radius, xmlNode, "Radius");
        if (m_geomType)
        {
            _SetGeomType(m_geomType);
        }
    }

    CVector SimplePhysicObjPrototypeInfo::GetSize() const
    {
        return m_size;
    }

    float SimplePhysicObjPrototypeInfo::GetMassValue() const
    {
        return m_massValue;
    }

    void SimplePhysicObjPrototypeInfo::_SetGeomType(GeomType geomType)
    {
        m_geomType = geomType;
        if (geomType == GEOM_TYPE_FROM_MODEL)
            return;

        m_collisionInfos.clear();

        CollisionInfo ci;
        ci.Init();
        if (m_geomType == GEOM_TYPE_BOX)
        {
            ci.m_geomType = GEOM_TYPE_BOX;
            ci.m_size.x = m_size.x;
            ci.m_size.y = m_size.y;
            ci.m_size.z = m_size.z;
        }
        else if (m_geomType == GEOM_TYPE_SPHERE)
        {
            m_radius = m_radius;
            ci.m_geomType = GEOM_TYPE_SPHERE;
            ci.m_radius = m_radius;
        }
        else
        {
            if (m_geomType == GEOM_TYPE_TRIMESH)
            {
                M3D_ASSERT(!"obsolete");
            }
            if (ci.m_geomType == GEOM_TYPE_NONE)
            {
                return;
            }
        }
        m_collisionInfos.push_back(ci);
    }

    eGObjPropertySaveStatus SimplePhysicObj::GetPropertySaveStatus(int id) const
    {
        // RVA 0x5EF3C0
        auto const it = m_propertiesSaveStatesMap.find(id);
        return it != m_propertiesSaveStatesMap.end() ? it->second : PhysicObj::GetPropertySaveStatus(id);
    }

    void SimplePhysicObj::Registration()
    {
        m_propertiesMap["Mass"] = 6;
        m_propertiesMap["NodeScale"] = 7;
    }

    void SimplePhysicObj::SetPassedToAnotherMapStatus()
    {
        // RVA 0x7F5900
        PhysicObj::SetPassedToAnotherMapStatus();
        if (m_physicBody)
        {
            m_physicBody->SetPassedToAnotherMapStatus();
        }
    }

    SimplePhysicObj::SimplePhysicObj(SimplePhysicObjPrototypeInfo const& prototypeInfo) : PhysicObj(prototypeInfo)
    {
        this->m_collisionInfos = prototypeInfo.m_collisionInfos;
        this->m_scale = 1.0;
        this->m_physicBody = prototypeInfo.CreatePhysicBody();
        this->m_physicBody->SetOwner(this);
        this->m_deadTimerActive = 0;
        this->m_deadTimer = 0.0;
        this->m_testVisibility = 0;
        ai::SimplePhysicObj::_Construct();
    }

    float SimplePhysicObj::GetScale()
    {
        return m_scale;
    }

    bool SimplePhysicObj::bDeadTimerActive()
    {
        return m_deadTimerActive;
    }

    int SimplePhysicObj::GetPropertyId(char const* propName) const
    {
        auto it = SimplePhysicObj::m_propertiesMap.find(propName);
        if (it != SimplePhysicObj::m_propertiesMap.end())
        {
            return it->second;
        }

        return ai::PhysicObj::GetPropertyId(propName);
    }

    void SimplePhysicObj::LinkGeomsToCollisionCells()
    {
        if (this->m_physicBody)
            this->m_physicBody->LinkGeomToCollisionCells();
    }

    void SimplePhysicObj::DisableGeometry(bool changePhysicState)
    {
        ai::PhysicObj::DisableGeometry(changePhysicState);
        if (!this->m_spaceId || !this->m_bIsSpaceOwner)
        {
            if (this->m_physicBody)
                this->m_physicBody->DisableGeometry();
        }
    }

    SimplePhysicBody* SimplePhysicObj::GetPhysicBody()
    {
        return this->m_physicBody;
    }

    SimplePhysicBody const* SimplePhysicObj::GetPhysicBody() const
    {
        return this->m_physicBody;
    }

    void SimplePhysicObj::SetMass(float mass)
    {
        if (m_physicBody)
        {
            m_physicBody->SetMass(mass);
            _Construct();
        }
    }

    void SimplePhysicObj::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x7F5BD0
        PhysicObj::SaveRuntimeValues(xmlFile, xmlNode);
        if (m_deadTimerActive)
        {
            // m3d::XmlNodeSetAttribute<bool>, inlined.
            xmlNode->SetAttribute("DeadTimeActive", CStr(static_cast<int>(m_deadTimerActive)).c_str());
            xmlNode->SetAttribute("DeadTimer", CStr(m_deadTimer).c_str());
            xmlNode->SetAttribute("TestVisibility", CStr(static_cast<int>(m_testVisibility)).c_str());
        }
        if (m_physicBody)
        {
            ref_ptr bodyNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "PhysicBody");
            xmlNode->AddChild(bodyNode);
            m_physicBody->SaveRuntimeValues(xmlFile, bodyNode);
        }
    }

    bool SimplePhysicObj::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        if (propertyId == 6)
        {
            if (m_physicBody)
            {
                m_physicBody->SetMass(newValue.GetAsFloat());
                _Construct();
            }
            return 1;
        }
        else if (propertyId == 7)
        {
            m_scale = newValue.GetAsFloat();
            SetScale(m_scale, 1);
            return 1;
        }
        return ai::PhysicObj::SetPropertyById(propertyId, newValue);
    }

    void SimplePhysicObj::Remove()
    {
        ai::PhysicObj::Remove();
        if (this->m_physicBody)
            this->m_physicBody->Remove();
    }

    void SimplePhysicObj::ReceiveNodesToLink(retruxx::list<m3d::SgNode*, retruxx::allocator<m3d::SgNode*>>& nodelist) const
    {
        // RVA 0x7F57A0
        if (m_physicBody)
        {
            m_physicBody->ReceiveNodesToLink(nodelist);
        }
    }

    m3d::Class* SimplePhysicObj::GetBaseClass()
    {
        return RT_CLASS_LOCAL(PhysicObj);
    }

    void SimplePhysicObj::RelinkSceneGraphNode()
    {
        if (this->m_physicBody)
            this->m_physicBody->RelinkSceneGraphNode();
    }

    void SimplePhysicObj::SetDeadTimer(int resttime, bool testVisibility)
    {
        // RVA 0x7F6B20 - the object is removed once resttime milliseconds have passed.
        m_testVisibility = testVisibility;
        m_deadTimerActive = true;
        m_deadTimer = static_cast<float>(resttime) * 0.001f;
        theObjects->AddObjToUpdate(this);
    }

    void SimplePhysicObj::TransferToSpace(dxSpace* newSpace)
    {
        if (this->m_physicBody)
            this->m_physicBody->RelinkToSpace(newSpace);
        ai::PhysicObj::TransferToSpace(newSpace);
    }

    void SimplePhysicObj::SetNodeAction(int action, bool forceRestartAction)
    {
        if (this->m_physicBody)
            this->m_physicBody->SetNodeAction(action, forceRestartAction);
    }

    void SimplePhysicObj::SetSkin(int skin)
    {
        PhysicObj::SetSkin(skin);
        if (m_physicBody)
            m_physicBody->SetSkin(skin);
    }

    CStr SimplePhysicObj::GetPropertyName(int id) const
    {
        // RVA 0x5EF750
        for (auto const& [name, propId] : m_propertiesMap)
        {
            if (propId == id)
            {
                return name;
            }
        }
        return PhysicObj::GetPropertyName(id);
    }

    m3d::Class* SimplePhysicObj::GetClass() const
    {
        // RVA 0x7F56F0
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    SimplePhysicObjPrototypeInfo const* SimplePhysicObj::GetPrototypeInfo() const
    {
        // RVA 0x7F6540
        return static_cast<SimplePhysicObjPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    CVector SimplePhysicObj::GetGeometricCenter() const
    {
        // RVA 0x7F5750
        return m_physicBody ? m_physicBody->GetNodeAbsolutePosition() : PhysicObj::GetGeometricCenter();
    }

    void SimplePhysicObj::RelinkGeomsToCollisionCells()
    {
        if (this->m_physicBody)
            this->m_physicBody->RelinkGeomToCollisionCells();
    }

    void SimplePhysicObj::SetNextForAnimation(int action, int nextAction)
    {
        if (this->m_physicBody)
            this->m_physicBody->SetNextForAnimation(action, nextAction);
    }

    void SimplePhysicObj::SetInvisible()
    {
        // RVA 0x7F5950
        PhysicObj::SetInvisible();
        if (m_physicBody)
        {
            m_physicBody->SetInvisible();
        }
    }

    void SimplePhysicObj::Update(float elapsedTime, unsigned workTime)
    {
        ai::PhysicObj::Update(elapsedTime, workTime);
        if (this->m_deadTimerActive)
        {
            auto v4 = this->m_deadTimer - elapsedTime;
            this->m_deadTimer = v4;
            if (v4 <= 0.0 &&
                (!this->m_testVisibility || m_physicBody == 0 || !m_physicBody->m_Node ||
                 m_physicBody->m_Node->m_frameVisible != m3d::g_Kernel->GetTimer().GetCurFrame() - 1))
            {
                Remove();
            }
        }
    }

    void SimplePhysicObj::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
    {
        // RVA 0x5EF690
        for (auto const& prop : m_propertiesMap)
        {
            Props.insert(prop.first);
        }
        PhysicObj::GetPropertiesNames(Props);
    }

    void SimplePhysicObj::EnableGeometry(bool changePhysicState)
    {
        ai::PhysicObj::EnableGeometry(changePhysicState);
        if (!this->m_spaceId || !this->m_bIsSpaceOwner)
        {
            if (this->m_physicBody)
                this->m_physicBody->EnableGeometry();
        }
    }

    void SimplePhysicObj::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        // RVA 0x5EF6F0
        for (auto const& prop : m_propertiesMap)
        {
            Props.insert(prop.second);
        }
        PhysicObj::GetPropertiesIDs(Props);
    }

    bool SimplePhysicObj::IsVisible()
    {
        // RVA 0x7F5E20 - drawn in the current frame.
        return m_physicBody && m_physicBody->m_Node &&
            m_physicBody->m_Node->m_frameVisible == M3D_KERNEL->GetTimer().GetCurFrame();
    }

    Geom::CellAabb SimplePhysicObj::GetCollisionCellAabb() const
    {
        return PhysicObj::GetCollisionCellAabb();
    }

    void SimplePhysicObj::SetScale(float scale, bool recalcMass)
    {
        // RVA 0x7F75B0 - the collision shapes, the centre of mass, optionally the mass and the model all follow the
        // new scale.
        if (!m_physicBody || !m_physicBody->m_Node)
        {
            return;
        }
        float const deltaScale = scale / m_physicBody->m_Node->GetScale().x;
        if (fabs(deltaScale - 1.0) < 0.0000099999997f)
        {
            return;
        }

        for (auto& collisionInfo : m_collisionInfos)
        {
            collisionInfo.m_size.x = collisionInfo.m_size.x * deltaScale;
            collisionInfo.m_size.y = deltaScale * collisionInfo.m_size.y;
            collisionInfo.m_size.z = deltaScale * collisionInfo.m_size.z;
            collisionInfo.m_radius = deltaScale * collisionInfo.m_radius;
            collisionInfo.m_relTranslation.x = collisionInfo.m_relTranslation.x * deltaScale;
            collisionInfo.m_relTranslation.y = collisionInfo.m_relTranslation.y * deltaScale;
            collisionInfo.m_relTranslation.z = collisionInfo.m_relTranslation.z * deltaScale;
        }
        _UpdatePhysicBodyByCollisionInfo(m_collisionInfos);

        if (recalcMass)
        {
            float const mass = GetMass();
            if (m_physicBody)
            {
                m_physicBody->SetMass(mass * deltaScale * deltaScale * deltaScale);
                _Construct();
            }
        }

        m_physicBody->m_Node->UpdateXForm(true, false);
        m_physicBody->m_Node->SetScale(CVector(scale, scale, scale));
        m_physicBody->m_Node->UpdateXForm(true, false);
        m_scale = scale;
    }

    void SimplePhysicObj::UnlinkGeomsFromCollisionCells()
    {
        if (this->m_physicBody)
            this->m_physicBody->UnlinkGeomFromCollisionCells();
    }

    void SimplePhysicObj::TransferPhysicParamsToSceneGraphNode()
    {
        if (this->m_physicBody)
            this->m_physicBody->TransferPhysicParamsToSceneGraphNode();
    }

    void SimplePhysicObj::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x7F6A00
        PhysicObj::LoadRuntimeValues(xmlFile, xmlNode);
        m3d::SafeBoolAttrib(m_deadTimerActive, xmlNode, "DeadTimeActive");
        if (m_deadTimerActive)
        {
            theObjects->AddObjToUpdate(this);
            m3d::SafeFloatAttrib(m_deadTimer, xmlNode, "DeadTimer");
            m3d::SafeBoolAttrib(m_testVisibility, xmlNode, "TestVisibility");
        }
        if (m_physicBody)
        {
            ref_ptr bodyNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
            xmlNode->GetFirstChild(bodyNode, "PhysicBody");
            if (!bodyNode->IsEmpty())
            {
                m_physicBody->LoadRuntimeValues(xmlFile, bodyNode);
            }
        }
    }

    void SimplePhysicObj::SetVisible()
    {
        ai::PhysicObj::SetVisible();
        if (this->m_physicBody)
            this->m_physicBody->SetVisible();
    }

    void SimplePhysicObj::RenderDebugInfo() const
    {
        if (!m_physicBody->m_Node || m_physicBody->m_Node->m_frameVisible == m3d::g_Kernel->GetTimer().GetCurFrame())
        {
            PhysicObj::RenderDebugInfo();
            if (!m_spaceId || !m_bIsSpaceOwner || dGeomIsEnabled(m_spaceId))
            {
                if (m_physicBody)
                    m_physicBody->RenderDebugInfo();
            }
        }
    }

    void SimplePhysicObj::SetBelong(int newBelong)
    {
        ai::Obj::SetBelong(newBelong);
        if (this->m_physicBody)
            this->m_physicBody->SetBelong(newBelong);
    }

    void SimplePhysicObj::_InternalCreateVisualPart()
    {
        ai::PhysicObj::_InternalCreateVisualPart();
        if (m_physicBody)
            m_physicBody->CreateVisualPart();
        this->SetSkin(GetSkin());
        SetScale(this->m_scale, false);
    }

    bool SimplePhysicObj::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x7F73E0
        switch (propertyId)
        {
        case 6:
            retVal = GetPrototypeInfo()->GetMassValue();
            return true;
        case 7:
            retVal = 1.0f;
            return true;
        default:
            return PhysicObj::_GetPropertyDefaultInternal(propertyId, retVal);
        }
    }

    void SimplePhysicObj::_UpdateCollisionInfoFromPhysicBody()
    {
        if (m_physicBody)
        {
            m_collisionInfos = m_physicBody->GetCollisionInfo();
            _SetMassCenter(m_collisionInfos.front().m_relTranslation);
        }
    }

    void SimplePhysicObj::_InternalPostLoad()
    {
        ai::PhysicObj::_InternalPostLoad();
        this->_Construct();
    }

    void SimplePhysicObj::_SetPositionToGeoms(CVector const& pos)
    {
        // RVA 0x7F59A0
        PhysicObj::_SetPositionToGeoms(pos);
    }

    void SimplePhysicObj::_Construct()
    {
        if (this->m_physicBody)
        {
            ai::PhysicObj::_CreateSpace(0);

            dMass mass;
            dMassSetZero(&mass);
            dMassSetZero(&mass);
            this->m_physicBody->RelinkToSpace(this->m_spaceId);
            dMassAdd(&mass, &this->m_physicBody->m_mass);
            if (mass.mass < 0.001)
            {
                M3D_LOG_ERR("Error: mass is too low: " + CStr(mass.mass) + " for " + GetDebugDescription());
                dMassSetSphereTotal(&mass, 1.0, 1.0);
            }
            dBodySetMass(this->m_body->id(), &mass);

            auto v6 = this->m_physicBody->GetModel();
            if (v6)
            {
                auto v7 = v6->m_box.m_box[1];
                auto v8 = v6->m_box.m_box[4];
                auto v9 = v6->m_box.m_box[0];
                auto v10 = v6->m_box.m_box[3];

                float b[3];
                b[2] = v6->m_box.m_box[5] - v6->m_box.m_box[2];
                b[1] = v8 - v7;
                b[0] = v10 - v9;
                auto radius = sqrt(b[2] * b[2] + b[1] * b[1] + b[0] * b[0]) * 0.5;
                ai::PhysicObj::_SetBoundSphereRadius(radius);
            }
            else
            {
                ai::PhysicObj::_SetBoundSphereRadius(15.0);
            }
        }
    }

    void SimplePhysicObj::_SetRotationToGeoms(Quaternion const& rot)
    {
        // RVA 0x7F59B0
        PhysicObj::_SetRotationToGeoms(rot);
    }

    bool SimplePhysicObj::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x7F7320
        switch (propertyId)
        {
        case 6:
            retVal = GetMass();
            return true;
        case 7:
            retVal = m_scale;
            return true;
        default:
            return PhysicObj::_GetPropertyInternal(propertyId, retVal);
        }
    }

    void SimplePhysicObj::_UpdatePhysicBodyByCollisionInfo(retruxx::vector<CollisionInfo> const& collisionInfos)
    {
        // RVA 0x7F7470 - as _UpdateFullPhysicBodyByCollisionInfo, but the existing geoms are only resized.
        m_collisionInfos = collisionInfos;
        if (m_physicBody)
        {
            m_physicBody->UpdateGeomsByCollisionInfo(collisionInfos);
        }
        // NOTE: the first collision info is used without checking that there is one.
        _SetMassCenter(collisionInfos.front().m_relTranslation);
    }

    void SimplePhysicObj::_LinkBodyToGeoms()
    {
        PhysicObj::_LinkBodyToGeoms();
    }

    void SimplePhysicObj::_UpdateFullPhysicBodyByCollisionInfo(retruxx::vector<CollisionInfo> const& collisionInfos)
    {
        // RVA 0x7F74B0
        m_collisionInfos = collisionInfos;
        if (m_physicBody)
        {
            m_physicBody->ChangePhysicBodyByCollisionInfo(collisionInfos);
        }
        // NOTE: the first collision info is used without checking that there is one.
        _SetMassCenter(collisionInfos.front().m_relTranslation);
    }

    void SimplePhysicObj::RegisterProperty(char const* name, int id, eGObjPropertySaveStatus saveStatus)
    {
        // RVA 0x7F7050
        m_propertiesMap[name] = id;
        if (saveStatus != SAVE_PROP_NORMAL)
        {
            m_propertiesSaveStatesMap[id] = saveStatus;
        }
    }

    SimplePhysicObj::~SimplePhysicObj()
    {
        // TODO: check this
        delete m_physicBody;
    }

    void SimplePhysicObj::_UnlinkBodyFromGeoms()
    {
        ai::PhysicObj::_UnlinkBodyFromGeoms();
    }

    m3d::Object* SimplePhysicObj::Clone()
    {
        // RVA 0x7F5EC0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* SimplePhysicObj::CreateObject()
    {
        // RVA 0x7F6080
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }
}  // namespace ai
