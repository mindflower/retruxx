#include "simplephysicobj.h"

#include "server/objects/physicbodies/physichelpers.h"
#include <stdexcept>
#include <ode/collision.h>
#include <ode/objects.h>

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
	throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(SimplePhysicObj, SetNodeAction)
{
	throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(SimplePhysicObj, SetNextForAnimation)
{
	throw retruxx::logic_error("Not implemented");
}

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(SimplePhysicObj)
		RT_CLASS_EXPORT(SimplePhysicObj, m3d::METHOD, SetMass, "", "", "")
		RT_CLASS_EXPORT(SimplePhysicObj, m3d::METHOD, SetNodeAction, "", "", "")
		RT_CLASS_EXPORT(SimplePhysicObj, m3d::METHOD, SetNextForAnimation, "", "", "")
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(SimplePhysicObj);

	SimplePhysicObjPrototypeInfo::~SimplePhysicObjPrototypeInfo()
	{
		throw retruxx::logic_error("Not implemented");
	}

	CStr const& SimplePhysicObjPrototypeInfo::GetEngineModelName() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool SimplePhysicObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
	{
        auto result = ai::PhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_massValue, xmlNode, "Mass");
            m3d::SafeStrAttrib(m_engineModelName, xmlNode, "ModelFile");
            m3d::SafeBoolAttrib(m_bCollisionTrimeshAllowed, xmlNode, "CollisionTrimeshAllowed");
            return 1;
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
		throw retruxx::logic_error("Not implemented");
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
            M3D_ASSERT(!"obsolete");

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

        // TODO: check this!!
        M3D_ASSERT(body);

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
        const auto sizes = serverAnimatedModels->GetBoundSizes(m_engineModelName.c_str());
        const auto item = serverAnimatedModels->GetItemByName(m_engineModelName.c_str(), true);
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
		throw retruxx::logic_error("Not implemented");
	}

	float SimplePhysicObjPrototypeInfo::GetMassValue() const
	{
		throw retruxx::logic_error("Not implemented");
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

	eGObjPropertySaveStatus SimplePhysicObj::GetPropertySaveStatus(int) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::Registration()
	{
		m_propertiesMap["Mass"] = 6;
		m_propertiesMap["NodeScale"] = 7;
	}

	void SimplePhysicObj::SetPassedToAnotherMapStatus()
	{
		throw retruxx::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    bool SimplePhysicObj::bDeadTimerActive()
    {
        throw std::logic_error("Not implemented");
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
		throw retruxx::logic_error("Not implemented");
	}

	SimplePhysicBody const* SimplePhysicObj::GetPhysicBody() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::SetMass(float mass)
	{
        if (m_physicBody)
        {
            m_physicBody->SetMass(mass);
            _Construct();
        }
	}

	void SimplePhysicObj::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool SimplePhysicObj::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
	{
        if (propertyId == 6)
        {
            throw retruxx::logic_error("Not implemented");
        }
        else if (propertyId == 7)
        {
            throw retruxx::logic_error("Not implemented");
        }
        else
        {
            return ai::PhysicObj::SetPropertyById(propertyId, newValue);
        }
	}

	void SimplePhysicObj::Remove()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::ReceiveNodesToLink(retruxx::list<m3d::SgNode*, retruxx::allocator<m3d::SgNode*>>&) const
	{
		throw retruxx::logic_error("Not implemented");
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

	void SimplePhysicObj::SetDeadTimer(int, bool)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::TransferToSpace(dxSpace*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::SetNodeAction(int, bool)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::SetSkin(int skin)
	{
        PhysicObj::SetSkin(skin);
        if (m_physicBody)
            m_physicBody->SetSkin(skin);
	}

	CStr SimplePhysicObj::GetPropertyName(int) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Class* SimplePhysicObj::GetClass() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	SimplePhysicObjPrototypeInfo const* SimplePhysicObj::GetPrototypeInfo() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	CVector SimplePhysicObj::GetGeometricCenter() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::RelinkGeomsToCollisionCells()
	{
        if (this->m_physicBody)
            this->m_physicBody->RelinkGeomToCollisionCells();
	}

	void SimplePhysicObj::SetNextForAnimation(int, int)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::SetInvisible()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::Update(float elapsedTime, unsigned workTime)
	{
        ai::PhysicObj::Update(elapsedTime, workTime);
        if (this->m_deadTimerActive)
        {
            auto v4 = this->m_deadTimer - elapsedTime;
            this->m_deadTimer = v4;
            if (v4 <= 0.0
                && (!this->m_testVisibility
                    || m_physicBody == 0
                    || !m_physicBody->m_Node
                    || m_physicBody->m_Node->m_frameVisible != m3d::g_Kernel->GetTimer().GetCurFrame() - 1))
            {
                Remove();
            }
        }
	}

	void SimplePhysicObj::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::EnableGeometry(bool)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool SimplePhysicObj::IsVisible()
	{
		throw retruxx::logic_error("Not implemented");
	}

	Geom::CellAabb SimplePhysicObj::GetCollisionCellAabb() const
	{
		return PhysicObj::GetCollisionCellAabb();
	}

	void SimplePhysicObj::SetScale(float scale, bool recalcMass)
	{
        // TODO: generated code
        if (!m_physicBody || !m_physicBody->m_Node)
            return;

        // Calculate scale delta from current scale
        float currentScale = m_physicBody->m_Node->GetScale().x;
        float deltaScale = scale / currentScale;

        // Only proceed if scale change is significant
        if (fabs(deltaScale - 1.0f) < 0.00001f)
            return;

        // Scale all collision info properties
        for (auto& collisionInfo : m_collisionInfos)
        {
            // Scale size
            collisionInfo.m_size.x *= deltaScale;
            collisionInfo.m_size.y *= deltaScale;
            collisionInfo.m_size.z *= deltaScale;

            // Scale offset (assuming m_offset is at p_z[1] based on decompilation)
            collisionInfo.m_radius *= deltaScale;

            // Scale relative translation
            collisionInfo.m_relTranslation.x *= deltaScale;
            collisionInfo.m_relTranslation.y *= deltaScale;
            collisionInfo.m_relTranslation.z *= deltaScale;
        }

        // Update physics geometry with new collision info
        if (m_physicBody)
        {
            m_physicBody->UpdateGeomsByCollisionInfo(m_collisionInfos);
        }

        // Update mass center (using first collision info)
        if (!m_collisionInfos.empty())
        {
            _SetMassCenter(m_collisionInfos[0].m_relTranslation);
        }

        // Recalculate mass if requested (mass scales with volume)
        if (recalcMass && m_physicBody)
        {
            float currentMass = GetMass();
            float newMass = currentMass * deltaScale * deltaScale * deltaScale;
            m_physicBody->SetMass(newMass);
            _Construct(); // Reconstruct physics object with new mass
        }

        // Update visual node scale
        if (m_physicBody->m_Node)
        {
            CVector newScale(scale, scale, scale);

            // Update transform and set new scale
            m_physicBody->m_Node->UpdateXForm(true, false);
            m_physicBody->m_Node->SetScale(newScale);
            m_physicBody->m_Node->UpdateXForm(true, false);
        }

        // Store new scale
        m_scale = scale;
	}

	void SimplePhysicObj::UnlinkGeomsFromCollisionCells()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::TransferPhysicParamsToSceneGraphNode()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::SetVisible()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::RenderDebugInfo() const
	{
		throw retruxx::logic_error("Not implemented");
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

	bool SimplePhysicObj::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
	{
		throw retruxx::logic_error("Not implemented");
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

	void SimplePhysicObj::_SetPositionToGeoms(CVector const&)
	{
		throw retruxx::logic_error("Not implemented");
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

	void SimplePhysicObj::_SetRotationToGeoms(Quaternion const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool SimplePhysicObj::_GetPropertyInternal(int, m3d::AIParam&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::_UpdatePhysicBodyByCollisionInfo(retruxx::vector<CollisionInfo> const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::_LinkBodyToGeoms()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::_UpdateFullPhysicBodyByCollisionInfo(retruxx::vector<CollisionInfo> const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
	{
		throw retruxx::logic_error("Not implemented");
	}

	SimplePhysicObj::~SimplePhysicObj()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::_UnlinkBodyFromGeoms()
	{
        ai::PhysicObj::_UnlinkBodyFromGeoms();
	}

	m3d::Object* SimplePhysicObj::Clone()
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Object* SimplePhysicObj::CreateObject()
	{
		throw retruxx::logic_error("Not implemented");
	}
}
