#include "physicbody.h"

#include "server/objects/physicbodies/physichelpers.h"
#include <stdexcept>
#include <ode/objects.h>

#include "game/m3dgame.h"
#include "geoms/box.h"
#include "math/vector.h"
#include "ode/odecpp.h"
#include "scene/nodes/sgnode.h"
#include "scene/servers/DataServer.h"
#include "thirdparty/injecttools.h"

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(PhysicBody)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(PhysicBody);

	void PhysicBodyPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	bool PhysicBodyPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	PhysicBodyPrototypeInfo::PhysicBodyPrototypeInfo()
	{
		throw std::logic_error("Not implemented");
	}

    PhysicBody::PhysicBody(PhysicBody const&)
    {
    }

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION(0x0061D180, PhysicBody::SetEffectActions)
    void PhysicBody::SetEffectActions(retruxx::vector<ActionType>&)
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::SetModelName(CStr const&)
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::SetModelNameUnsafe(CStr const& newModelName)
	{
        m_modelname = newModelName;
	}

	int PhysicBody::GetNodeAnimAction() const
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::SetNodeAnimAction(int, bool)
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::SetNextForAnimation(int, int)
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::ChangePhysicBodyByCollisionInfo(retruxx::vector<CollisionInfo> const&)
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::SetSgNode(m3d::SgNode*)
	{
		throw std::logic_error("Not implemented");
	}

	CVector PhysicBody::GetPosition() const
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::SetPosition(CVector const&)
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::UpdateGeomsByCollisionInfo(retruxx::vector<CollisionInfo> const& collisionInfos)
	{
        auto pos = GetNodeRelativePosition();
        m_collisionInfos = collisionInfos;
        for (int i = 0; i < m_pGeoms.size(); ++i)
        {
            auto geom = m_pGeoms[i]->GetGeom();
            if (geom)
            {
                dGeomSetPosition(geom->GetGeomId(), m_collisionInfos[i].m_relTranslation.x, m_collisionInfos[i].m_relTranslation.y, m_collisionInfos[i].m_relTranslation.z);

                dQuaternion quat;
                quat[0] = m_collisionInfos[i].m_relRotation.x;
                quat[1] = m_collisionInfos[i].m_relRotation.y;
                quat[2] = m_collisionInfos[i].m_relRotation.z;
                quat[3] = m_collisionInfos[i].m_relRotation.w;
                dGeomSetQuaternion(geom->GetGeomId(), quat);

                auto cl = dGeomGetClass(geom->GetGeomId());
                if (cl)
                {
                    if (cl == 1)
                        ((ai::Box*)geom)->SetSize(m_collisionInfos[i].m_size);
                }
                else
                {
                    ((ai::Sphere*)geom)->SetRadius(m_collisionInfos[i].m_radius);
                }
            }
        }
        SetNodeRelativePosition(pos);
	}

	void PhysicBody::SetNodeAction(int, bool)
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::GetGeoms(retruxx::vector<Geom*, retruxx::allocator<Geom*>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::SgNode* PhysicBody::CreateNode(CStr const&, int, CVector const&, PhysicBody*, bool)
	{
		throw std::logic_error("Not implemented");
	}

	PhysicBody::PhysicBody(PhysicBodyPrototypeInfo const&)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* PhysicBody::GetBaseClass()
	{
		return RT_CLASS_LOCAL(Obj);
	}

	void PhysicBody::TransferPhysicParamsToSceneGraphNode()
	{
        if (this->m_Node)
        {
            auto NodeRelativeRotation = ai::PhysicBody::GetNodeRelativeRotation();
            auto Rotation = ai::PhysicBody::GetRotation();
            auto v4 = Rotation.w * NodeRelativeRotation.y;

            Quaternion rot; // [esp+10h] [ebp-30h] BYREF
            rot.x = (float)((float)((float)(NodeRelativeRotation.z * Rotation.y)
                + (float)(Rotation.w * NodeRelativeRotation.x))
                + (float)(NodeRelativeRotation.w * Rotation.x))
                - (float)(Rotation.z * NodeRelativeRotation.y);
            auto v5 = (float)((float)((float)(NodeRelativeRotation.w * Rotation.y) + v4)
                + (float)(Rotation.z * NodeRelativeRotation.x))
                - (float)(NodeRelativeRotation.z * Rotation.x);
            auto v6 = NodeRelativeRotation.w * Rotation.z;
            rot.y = v5;
            auto v7 = (float)((float)((float)(NodeRelativeRotation.z * Rotation.w) + v6)
                + (float)(Rotation.x * NodeRelativeRotation.y))
                - (float)(Rotation.y * NodeRelativeRotation.x);
            auto v8 = Rotation.x * NodeRelativeRotation.x;
            rot.z = v7;
            rot.w = (float)((float)((float)(NodeRelativeRotation.w * Rotation.w) - v8)
                - (float)(Rotation.y * NodeRelativeRotation.y))
                - (float)(NodeRelativeRotation.z * Rotation.z);
            auto NodeAbsolutePosition = ai::PhysicBody::GetNodeAbsolutePosition();
            ai::PhysicBody::_SetScenegraphNode(NodeAbsolutePosition, rot);
        }
	}

	void PhysicBody::_ClearGeoms()
	{
		throw std::logic_error("Not implemented");
	}

	CVector PhysicBody::GetDirection() const
	{
		throw std::logic_error("Not implemented");
	}

	unsigned PhysicBody::GetNumGeoms() const
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::SgNode* PhysicBody::CreateEffectNode(CStr const&, CVector const&, Quaternion const&, bool, float)
	{
		throw std::logic_error("Not implemented");
	}

	Quaternion PhysicBody::GetRotation() const
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::SetRotation(Quaternion const*)
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::SetRotation(Quaternion const&)
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::UnlinkGeomsFromBody()
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::SetAnimationStopped(bool)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::DbgCounter* PhysicBody::GetCountNodeRelinks()
	{
        return m_countNodeRelinks;
	}

	Geom* PhysicBody::GetGeom(unsigned) const
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::RelinkSceneGraphNode()
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::SetNodeEffectAction(int)
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::DumpPhysicInfo(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw std::logic_error("Not implemented");
	}

	int PhysicBody::GetNodeEffectAction() const
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::LinkGeomToCollisionCells()
	{
		throw std::logic_error("Not implemented");
	}

    retruxx::vector<CollisionInfo, retruxx::allocator<CollisionInfo>> const& PhysicBody::GetCollisionInfo() const
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::SetNodeAbsolutePosition(CVector const&)
	{
		throw std::logic_error("Not implemented");
	}

	CVector PhysicBody::GetNodeAbsolutePosition() const
	{
		throw std::logic_error("Not implemented");
	}

	PhysicBody::~PhysicBody()
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::SetBelong(int newBelong)
	{
        ai::Obj::SetBelong(newBelong);
        if (m_Node)
            m_Node->SetProperty(4353u, &newBelong);
	}

	void PhysicBody::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw std::logic_error("Not implemented");
	}

	int PhysicBody::GetSkin() const
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::SetSkin(int)
	{
		throw std::logic_error("Not implemented");
	}

	int PhysicBody::GetNodeCfgNum() const
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::RenderDebugInfo() const
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::SetNodeCfgNum(int)
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::SetPassedToAnotherMapStatus()
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::ApplyCurrentModelCollision()
	{
		throw std::logic_error("Not implemented");
	}

	CVector PhysicBody::GetNodeAbsoluteDirection() const
	{
		throw std::logic_error("Not implemented");
	}

	Geom::CellAabb PhysicBody::GetCollisionCellAabb() const
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::SetNodeAbsoluteDirection(CVector const&)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* PhysicBody::GetRtClass() const
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::SetOwnerBodyToGeoms()
	{
        m_ownerPhysicObj = this->m_ownerPhysicObj;

        dBodyID id = 0;
        if (m_ownerPhysicObj)
            id = m_ownerPhysicObj->GetBody()->id();

        auto v4 = 0;
        if (m_ownerPhysicObj)
        {
            v4 = 1;
            if ((m_ownerPhysicObj->GetPhysicState() & 1) != 0 || dBodyGetAutoDisableFlag(id))
                v4 = 0;
        }
        for (auto& geom : m_pGeoms)
        {
            dGeomSetData(geom->GetGeomId(), this);
            dGeomSetBody(geom->GetGeomId(), id);
            if (v4)
                dGeomSetBody(geom->GetGeomId(), id);
            geom->UnlinkFromBody();
        }
	}

	void PhysicBody::Registration()
	{
		m_countNodeRelinks = M3D_APP->GetDbgCounterStack().GetCounter(M3D_APP->GetDbgCounterStack().AddCounter("node relinks in server"));
		m_countNodeRelinks->SetI(0);
	}

	int PhysicBody::GetNodeRealAnimAction() const
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::SetCollisionTrimeshAllowed(bool)
	{
		throw std::logic_error("Not implemented");
	}

	Quaternion PhysicBody::GetNodeRelativeRotation() const
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::SetNodeRelativeRotation(Quaternion const&)
	{
		throw std::logic_error("Not implemented");
	}

	int PhysicBody::GetNodeRealAction() const
	{
		throw std::logic_error("Not implemented");
	}

	bool PhysicBody::bNeedToRelinkNode() const
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::SetVisible()
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::EnableGeometry()
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::SetOwner(PhysicObj* owner)
	{
        if (owner != this->m_ownerPhysicObj)
        {
            this->m_ownerPhysicObj = owner;
            SetOwnerBodyToGeoms();
        }
	}

	PhysicObj* PhysicBody::GetOwner() const
	{
        return this->m_ownerPhysicObj;
	}

	void PhysicBody::SetInvisible()
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::ReceiveNodesToLink(retruxx::list<m3d::SgNode*, retruxx::allocator<m3d::SgNode*>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::SetNodeAbsoluteRotation(Quaternion const&)
	{
		throw std::logic_error("Not implemented");
	}

	Quaternion PhysicBody::GetNodeAbsoluteRotation() const
	{
		throw std::logic_error("Not implemented");
	}

	bool PhysicBody::CanChildBeAdded(m3d::Class*) const
	{
		throw std::logic_error("Not implemented");
	}

	CVector PhysicBody::GetNodeRelativeDirection() const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::AnimatedModel* PhysicBody::GetModel() const
	{
		if (m_Node == nullptr)
		{
            return nullptr;
		}

        auto* server = m_Node->GetServer();

        m3d::AnimatedModel* mdl = nullptr;
        server->GetItemProperty(this->m_Node->GetServerHandle(), 16394, &mdl);
        return mdl;
	}

	float PhysicBody::GetMass() const
	{
		throw std::logic_error("Not implemented");
	}

	CVector PhysicBody::GetNodeRelativePosition() const
	{
        if (!m_pGeoms.empty() && m_pGeoms.front()->GetGeom() != nullptr)
        {
            auto& relPos = m_collisionInfos.front().m_relTranslation;
            auto inner = m_pGeoms.front()->GetGeom();
            auto pos = dGeomGetPosition(inner->GetGeomId());
            return { pos[0] - relPos.x, pos[1] - relPos.y , pos[2] - relPos.z };
        }
        return { 0.0, 0.0, 0.0 };
	}

	void PhysicBody::SetNodeRelativePosition(CVector const& pos)
	{
        for (int i = 0; i < m_pGeoms.size(); ++i)
        {
            auto x = m_collisionInfos[i].m_relTranslation.x + pos.x;
            auto y = m_collisionInfos[i].m_relTranslation.y + pos.y;
            auto z = m_collisionInfos[i].m_relTranslation.z + pos.z;
            dGeomSetPosition(m_pGeoms[i]->GetGeom()->GetGeomId(), x, y, z);
        }
	}

	void PhysicBody::UnlinkGeomFromCollisionCells()
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::RelinkGeomToCollisionCells()
	{
		for (auto& geom : m_pGeoms)
		{
            auto objId = -1;
            if (m_ownerPhysicObj)
                objId = m_ownerPhysicObj->GetId();
            geom->RelinkToCollisionCells(objId);
		}
	}

	void PhysicBody::DisableGeometry()
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::RelinkToSpace(dxSpace* newSpace)
	{
        for (auto& geom : m_pGeoms)
        {
            if (dGeomGetSpace(geom->GetGeomId()))
            {
                auto Space = dGeomGetSpace(geom->GetGeomId());
                dSpaceRemove(Space, geom->GetGeomId());
            }
            dSpaceAdd(newSpace, geom->GetGeomId());
        }
	}

	int PhysicBody::GetOwnerId() const
	{
		throw std::logic_error("Not implemented");
	}

	int PhysicBody::_GetCurAnimationFrame() const
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::_SetScenegraphNode(CVector const&, Quaternion const&)
	{
		throw std::logic_error("Not implemented");
	}

	PhysicBody::PhysicBody()
	{
        dMassSetZero(&m_mass);
        m_modelname = "BOX";
        auto obj = GeomTransform::CreateObject(nullptr, CommonGeomMovedCallback);
        m_pGeoms.push_back(obj);

        CollisionInfo info;
        m_collisionInfos.push_back(std::move(info));

        this->m_bCollisionTrimeshAllowed = 0;
        this->m_Node = 0;
        this->m_ownerPhysicObj = 0;
        this->m_animAction = 0;
        this->m_effectAction = 0;
        this->m_cfgNum = 0;
        this->m_bAnimationIsStopped = 0;
        this->m_loadedAnimTime = 0;
        this->m_mU = 1.0;
        this->m_bNeedToRelinkNode = 1;
	}

	int PhysicBody::_GetNodeRealAnimAction() const
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::_InternalCreateVisualPart()
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::_ApplyCurrentModelName()
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::_DeleteNode()
	{
		throw std::logic_error("Not implemented");
	}
}
