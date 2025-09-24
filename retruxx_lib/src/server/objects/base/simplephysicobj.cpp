#include "simplephysicobj.h"

#include "server/objects/physicbodies/physichelpers.h"
#include <stdexcept>

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

	bool SimplePhysicObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	SimplePhysicObjPrototypeInfo::SimplePhysicObjPrototypeInfo()
	{
		throw retruxx::logic_error("Not implemented");
	}

	float SimplePhysicObjPrototypeInfo::GetRadius() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	SimplePhysicBody* SimplePhysicObjPrototypeInfo::CreatePhysicBody() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObjPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	CVector SimplePhysicObjPrototypeInfo::GetSize() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	float SimplePhysicObjPrototypeInfo::GetMassValue() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObjPrototypeInfo::_SetGeomType(GeomType)
	{
		throw retruxx::logic_error("Not implemented");
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
		throw retruxx::logic_error("Not implemented");
	}

    float SimplePhysicObj::GetScale()
    {
        throw std::logic_error("Not implemented");
    }

    bool SimplePhysicObj::bDeadTimerActive()
    {
        throw std::logic_error("Not implemented");
    }

    int SimplePhysicObj::GetPropertyId(char const*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::LinkGeomsToCollisionCells()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::DisableGeometry(bool)
	{
		throw retruxx::logic_error("Not implemented");
	}

	SimplePhysicBody* SimplePhysicObj::GetPhysicBody()
	{
		throw retruxx::logic_error("Not implemented");
	}

	SimplePhysicBody const* SimplePhysicObj::GetPhysicBody() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::SetMass(float)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool SimplePhysicObj::SetPropertyById(int, m3d::AIParam const&)
	{
		throw retruxx::logic_error("Not implemented");
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
		throw retruxx::logic_error("Not implemented");
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

	void SimplePhysicObj::SetSkin(int)
	{
		throw retruxx::logic_error("Not implemented");
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
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::SetNextForAnimation(int, int)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::SetInvisible()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::Update(float, unsigned)
	{
		throw retruxx::logic_error("Not implemented");
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
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::SetScale(float, bool)
	{
		throw retruxx::logic_error("Not implemented");
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

	void SimplePhysicObj::SetBelong(int)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::_InternalCreateVisualPart()
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool SimplePhysicObj::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::_UpdateCollisionInfoFromPhysicBody()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::_InternalPostLoad()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::_SetPositionToGeoms(CVector const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void SimplePhysicObj::_Construct()
	{
		throw retruxx::logic_error("Not implemented");
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
		throw retruxx::logic_error("Not implemented");
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
