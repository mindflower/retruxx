#include "physicbody.h"

#include "server/objects/physicbodies/physichelpers.h"
#include <stdexcept>

#include "game/m3dgame.h"
#include "math/vector.h"
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

	void PhysicBody::SetModelNameUnsafe(CStr const&)
	{
		throw std::logic_error("Not implemented");
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

	void PhysicBody::UpdateGeomsByCollisionInfo(retruxx::vector<CollisionInfo> const&)
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
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

	void PhysicBody::SetBelong(int)
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
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

	void PhysicBody::SetOwner(PhysicObj*)
	{
		throw std::logic_error("Not implemented");
	}

	PhysicObj* PhysicBody::GetOwner() const
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	float PhysicBody::GetMass() const
	{
		throw std::logic_error("Not implemented");
	}

	CVector PhysicBody::GetNodeRelativePosition() const
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::SetNodeRelativePosition(CVector const&)
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::UnlinkGeomFromCollisionCells()
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::RelinkGeomToCollisionCells()
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::DisableGeometry()
	{
		throw std::logic_error("Not implemented");
	}

	void PhysicBody::RelinkToSpace(dxSpace*)
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
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
