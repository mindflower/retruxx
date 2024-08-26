#include "simplephysicobj.h"

#include "server/objects/physicbodies/physichelpers.h"
#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(SimplePhysicObj, SetMass)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(SimplePhysicObj, SetNodeAction)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(SimplePhysicObj, SetNextForAnimation)
{
	throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	CStr const& SimplePhysicObjPrototypeInfo::GetEngineModelName() const
	{
		throw std::logic_error("Not implemented");
	}

	bool SimplePhysicObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	SimplePhysicObjPrototypeInfo::SimplePhysicObjPrototypeInfo()
	{
		throw std::logic_error("Not implemented");
	}

	float SimplePhysicObjPrototypeInfo::GetRadius() const
	{
		throw std::logic_error("Not implemented");
	}

	SimplePhysicBody* SimplePhysicObjPrototypeInfo::CreatePhysicBody() const
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObjPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	CVector SimplePhysicObjPrototypeInfo::GetSize() const
	{
		throw std::logic_error("Not implemented");
	}

	float SimplePhysicObjPrototypeInfo::GetMassValue() const
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObjPrototypeInfo::_SetGeomType(GeomType)
	{
		throw std::logic_error("Not implemented");
	}

	eGObjPropertySaveStatus SimplePhysicObj::GetPropertySaveStatus(int) const
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::Registration()
	{
		m_propertiesMap["Mass"] = 6;
		m_propertiesMap["NodeScale"] = 7;
	}

	void SimplePhysicObj::SetPassedToAnotherMapStatus()
	{
		throw std::logic_error("Not implemented");
	}

	SimplePhysicObj::SimplePhysicObj(SimplePhysicObjPrototypeInfo const& prototypeInfo) : PhysicObj(prototypeInfo)
	{
		throw std::logic_error("Not implemented");
	}

	int SimplePhysicObj::GetPropertyId(char const*) const
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::LinkGeomsToCollisionCells()
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::DisableGeometry(bool)
	{
		throw std::logic_error("Not implemented");
	}

	SimplePhysicBody* SimplePhysicObj::GetPhysicBody()
	{
		throw std::logic_error("Not implemented");
	}

	SimplePhysicBody const* SimplePhysicObj::GetPhysicBody() const
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::SetMass(float)
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw std::logic_error("Not implemented");
	}

	bool SimplePhysicObj::SetPropertyById(int, m3d::AIParam const&)
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::Remove()
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::ReceiveNodesToLink(std::list<m3d::SgNode*, std::allocator<m3d::SgNode*>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* SimplePhysicObj::GetBaseClass()
	{
		return RT_CLASS_LOCAL(PhysicObj);
	}

	void SimplePhysicObj::RelinkSceneGraphNode()
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::SetDeadTimer(int, bool)
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::TransferToSpace(dxSpace*)
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::SetNodeAction(int, bool)
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::SetSkin(int)
	{
		throw std::logic_error("Not implemented");
	}

	CStr SimplePhysicObj::GetPropertyName(int) const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* SimplePhysicObj::GetClass() const
	{
		throw std::logic_error("Not implemented");
	}

	SimplePhysicObjPrototypeInfo const* SimplePhysicObj::GetPrototypeInfo() const
	{
		throw std::logic_error("Not implemented");
	}

	CVector SimplePhysicObj::GetGeometricCenter() const
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::RelinkGeomsToCollisionCells()
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::SetNextForAnimation(int, int)
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::SetInvisible()
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::Update(float, unsigned)
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::EnableGeometry(bool)
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	bool SimplePhysicObj::IsVisible()
	{
		throw std::logic_error("Not implemented");
	}

	Geom::CellAabb SimplePhysicObj::GetCollisionCellAabb() const
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::SetScale(float, bool)
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::UnlinkGeomsFromCollisionCells()
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::TransferPhysicParamsToSceneGraphNode()
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::SetVisible()
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::RenderDebugInfo() const
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::SetBelong(int)
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::_InternalCreateVisualPart()
	{
		throw std::logic_error("Not implemented");
	}

	bool SimplePhysicObj::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::_UpdateCollisionInfoFromPhysicBody()
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::_InternalPostLoad()
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::_SetPositionToGeoms(CVector const&)
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::_Construct()
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::_SetRotationToGeoms(Quaternion const&)
	{
		throw std::logic_error("Not implemented");
	}

	bool SimplePhysicObj::_GetPropertyInternal(int, m3d::AIParam&) const
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::_UpdatePhysicBodyByCollisionInfo(std::vector<CollisionInfo> const&)
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::_LinkBodyToGeoms()
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::_UpdateFullPhysicBodyByCollisionInfo(std::vector<CollisionInfo> const&)
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
	{
		throw std::logic_error("Not implemented");
	}

	SimplePhysicObj::~SimplePhysicObj()
	{
		throw std::logic_error("Not implemented");
	}

	void SimplePhysicObj::_UnlinkBodyFromGeoms()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* SimplePhysicObj::Clone()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* SimplePhysicObj::CreateObject()
	{
		throw std::logic_error("Not implemented");
	}
}
