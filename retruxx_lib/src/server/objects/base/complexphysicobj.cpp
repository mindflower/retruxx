#include "complexphysicobj.h"
#include <stdexcept>
#include "thirdparty/injecttools.h"

RT_CLASS_EXPORT_METHOD_DEFINE(ComplexPhysicObj, CanPartBeAttached)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ComplexPhysicObj, SetPartByName)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ComplexPhysicObj, SetNewPart)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ComplexPhysicObj, TakeOffPart)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ComplexPhysicObj, GetPartByName)
{
    throw std::logic_error("Not implemented");
}


namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(ComplexPhysicObj)
        RT_CLASS_EXPORT(ComplexPhysicObj, m3d::METHOD, CanPartBeAttached, "", "", "")
        RT_CLASS_EXPORT(ComplexPhysicObj, m3d::METHOD, SetPartByName, "", "", "")
        RT_CLASS_EXPORT(ComplexPhysicObj, m3d::METHOD, SetNewPart, "", "", "")
        RT_CLASS_EXPORT(ComplexPhysicObj, m3d::METHOD, TakeOffPart, "", "", "")
        RT_CLASS_EXPORT(ComplexPhysicObj, m3d::METHOD, GetPartByName, "", "", "")
	RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(ComplexPhysicObj);

    RT_CLASS_EXPORTS_BEGIN(ComplexPhysicObjPartDescription)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(ComplexPhysicObjPartDescription);

	m3d::Class* ComplexPhysicObjPartDescription::GetClass() const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* ComplexPhysicObjPartDescription::CreateObject()
	{
		throw std::logic_error("Not implemented");
	}

	int ComplexPhysicObjPartDescription::GetPartResourceId() const
	{
		throw std::logic_error("Not implemented");
	}

	unsigned ComplexPhysicObjPartDescription::GetNumLps() const
	{
		throw std::logic_error("Not implemented");
	}

	void ComplexPhysicObjPartDescription::GetPartNames(std::vector<CStr, std::allocator<CStr>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	void ComplexPhysicObjPartDescription::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	ComplexPhysicObjPartDescription const* ComplexPhysicObjPartDescription::GetChildByNameDeep(CStr const&) const
	{
		throw std::logic_error("Not implemented");
	}

	CStr const& ComplexPhysicObjPartDescription::GetLpName(unsigned) const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* ComplexPhysicObjPartDescription::Clone()
	{
		throw std::logic_error("Not implemented");
	}

	ComplexPhysicObjPartDescription* ComplexPhysicObjPartDescription::GetParent() const
	{
		throw std::logic_error("Not implemented");
	}

	ComplexPhysicObjPartDescription::~ComplexPhysicObjPartDescription()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* ComplexPhysicObjPartDescription::GetBaseClass()
	{
        return RT_CLASS_LOCAL(Object);
	}

	ComplexPhysicObjPartDescription::ComplexPhysicObjPartDescription()
	{
		throw std::logic_error("Not implemented");
	}

	ComplexPhysicObjPartDescription::ComplexPhysicObjPartDescription(ComplexPhysicObjPartDescription const&)
	{
		throw std::logic_error("Not implemented");
	}

	ComplexPhysicObjPrototypeInfo::MassShapes ComplexPhysicObjPrototypeInfo::GetMassShape() const
	{
		throw std::logic_error("Not implemented");
	}

	ComplexPhysicObjPartDescription const* ComplexPhysicObjPrototypeInfo::GetPartDescriptionByName(CStr const&) const
	{
		throw std::logic_error("Not implemented");
	}

	Obj* ComplexPhysicObjPrototypeInfo::CreateRandomTargetObject() const
	{
		throw std::logic_error("Not implemented");
	}

	bool ComplexPhysicObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	void ComplexPhysicObjPrototypeInfo::GetPartNames(std::vector<CStr, std::allocator<CStr>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	ComplexPhysicObjPrototypeInfo::~ComplexPhysicObjPrototypeInfo()
	{
		throw std::logic_error("Not implemented");
	}

	std::vector<CStr, std::allocator<CStr>> const& ComplexPhysicObjPrototypeInfo::GetAllPartNames() const
	{
		throw std::logic_error("Not implemented");
	}

	ComplexPhysicObjPrototypeInfo::ComplexPhysicObjPrototypeInfo()
	{
		throw std::logic_error("Not implemented");
	}

	unsigned ComplexPhysicObjPrototypeInfo::GetBasePrice() const
	{
		throw std::logic_error("Not implemented");
	}

	void ComplexPhysicObjPrototypeInfo::PostLoad()
	{
		throw std::logic_error("Not implemented");
	}

    void ComplexPhysicObj::UnlinkGeomsFromCollisionCells()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::GetGeoms(std::vector<Geom*, std::allocator<Geom*>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::SetPassedToAnotherMapStatus()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::EnableGeometry(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    std::vector<CStr, std::allocator<CStr>> ComplexPhysicObj::GetAttachedPartNames() const
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::DumpPhysicInfo(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::SetPartByName(CStr const&, VehiclePart*, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::Remove()
    {
        throw std::logic_error("Not implemented");
    }

    ComplexPhysicObj::ComplexPhysicObj(ComplexPhysicObjPrototypeInfo const& prototypeInfo) : PhysicObj(prototypeInfo)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* ComplexPhysicObj::GetBaseClass()
    {
        return RT_CLASS_LOCAL(PhysicObj);
    }

    void ComplexPhysicObj::PutContour()
    {
        throw std::logic_error("Not implemented");
    }

    bool ComplexPhysicObj::IsVisible()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::SetSkin(int)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::CreateChildren()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::TransferPhysicParamsToSceneGraphNode()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::ClearSavedStatus()
    {
        throw std::logic_error("Not implemented");
    }

    VehiclePart const* ComplexPhysicObj::GetPartByName(CStr const&) const
    {
        throw std::logic_error("Not implemented");
    }

    VehiclePart* ComplexPhysicObj::GetPartByName(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::SetVisible()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::EnablePhysics()
    {
        throw std::logic_error("Not implemented");
    }

    VehiclePart* ComplexPhysicObj::TakeOffPart(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::AddChild(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned ComplexPhysicObj::GetPrice(IPriceCoeffProvider const*) const
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::ReceiveNodesToLink(std::list<m3d::SgNode*, std::allocator<m3d::SgNode*>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::RemoveContour()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::RelinkGeomsToCollisionCells()
    {
        throw std::logic_error("Not implemented");
    }

    ComplexPhysicObjPrototypeInfo const* ComplexPhysicObj::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::DisableGeometry(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::SetContourWidth(float)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned ComplexPhysicObj::size() const
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::TransferToSpace(dxSpace*)
    {
        throw std::logic_error("Not implemented");
    }

    bool ComplexPhysicObj::bIsContoured() const
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::DisablePhysics()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::LinkGeomsToCollisionCells()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    Obj* ComplexPhysicObj::CloneObj()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::SetRandomSkin()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::SetBelong(int)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::SetInvisible()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* ComplexPhysicObj::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    Geom::CellAabb ComplexPhysicObj::GetCollisionCellAabb() const
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::SetContourColor(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    int ComplexPhysicObj::GetNumPhysicBodies() const
    {
        throw std::logic_error("Not implemented");
    }

    bool ComplexPhysicObj::RemoveChild(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    bool ComplexPhysicObj::CanPartBeAttached(CStr const&) const
    {
        throw std::logic_error("Not implemented");
    }

    int ComplexPhysicObj::GetGunHorizontalStopAngles(CStr const&, int, float&, float&) const
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::Blow(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::Flow(Obj*, float)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned ComplexPhysicObj::GetRepairPrice() const
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::RefreshMass()
    {
        throw std::logic_error("Not implemented");
    }

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION(0x006BCC10, ComplexPhysicObj::GetSmoothTargetPointForObj)
    CVector ComplexPhysicObj::GetSmoothTargetPointForObj(Obj const*, float)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::FlowUnattachableParts(float)
    {
        throw std::logic_error("Not implemented");
    }

    bool ComplexPhysicObj::SetNewPart(CStr const&, CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::RemoveComponent(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_DestroyHierarchy()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_LinkBodyToGeoms()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_UnlinkBodyFromGeoms()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_SetCorrectBoundSphereRadius()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_RemoveContour()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_InternalCreateVisualPart()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_SetPositionToGeoms(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_ConstructVehiclePart(CStr const&, VehiclePart*, int, bool)
    {
        throw std::logic_error("Not implemented");
    }

    float ComplexPhysicObj::_CalcMassForBody() const
    {
        throw std::logic_error("Not implemented");
    }

    ComplexPhysicObj::~ComplexPhysicObj()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_PutContour()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_SetRotationToGeoms(Quaternion const&)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_Construct(bool)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* ComplexPhysicObj::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* ComplexPhysicObj::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_CreateSplinterFromSgNode(VehiclePart*, int, CVector const&, float, m3d::SgNode*, CollisionInfo const*)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_TearOffPart(VehiclePart*, float)
    {
        throw std::logic_error("Not implemented");
    }
}
