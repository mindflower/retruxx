#include "vehiclepart.h"
#include "server/objects/physicbodies/physichelpers.h"
#include <stdexcept>

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(VehiclePart)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(VehiclePart);

	CVector const& VehiclePartPrototypeInfo::GetSize() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	VehiclePartPrototypeInfo::VehiclePartPrototypeInfo()
	{
		throw retruxx::logic_error("Not implemented");
	}

	Obj* VehiclePartPrototypeInfo::CreateTargetObject() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePartPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	VehiclePartPrototypeInfo::~VehiclePartPrototypeInfo()
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool VehiclePartPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePartPrototypeInfo::_InitModelMeshes(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	VehiclePart::BreakData::BreakData()
	{
		throw retruxx::logic_error("Not implemented");
	}

	VehiclePart::BreakModelData::BreakModelData()
	{
		throw retruxx::logic_error("Not implemented");
	}

	VehiclePart::ModelPart::ModelPart()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePart::RenderDebugInfo() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePart::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	CStr const& VehiclePart::GetPartName() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePart::TransferPhysicParamsToSceneGraphNode()
	{
		throw retruxx::logic_error("Not implemented");
	}

	CompoundVehiclePart* VehiclePart::GetOwnerCompoundVehiclePart()
	{
		throw retruxx::logic_error("Not implemented");
	}

	CompoundVehiclePart const* VehiclePart::GetOwnerCompoundVehiclePart() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	float VehiclePart::GetDurabilityCoeffForDamageType(DamageType) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	CStr const& VehiclePart::GetBlowEffectName() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	unsigned VehiclePart::GetRepairPrice() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Class* VehiclePart::GetClass() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	CVector VehiclePart::GetSize() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	CStr VehiclePart::GetPropertyName(int) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePart::DumpPhysicInfo(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	VehiclePartPrototypeInfo const* VehiclePart::GetPrototypeInfo() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePart::SetPartName(CStr const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool VehiclePart::ApplyModifier(Modifier const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePart::SetOwnerCompoundVehiclePart(CompoundVehiclePart*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePart::DefineSuppressedLPs()
	{
		throw retruxx::logic_error("Not implemented");
	}

	NumericInRangeRegenerating<float> const& VehiclePart::Durability() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	NumericInRangeRegenerating<float>& VehiclePart::Durability()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePart::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePart::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePart::Update(float, unsigned)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePart::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePart::Remove()
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool VehiclePart::SetPropertyById(int, m3d::AIParam const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Class* VehiclePart::GetBaseClass()
	{
		return RT_CLASS_LOCAL(PhysicBody);
	}

	float VehiclePart::GetRepairPriceForOneUnit() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	unsigned VehiclePart::GetPrice(IPriceCoeffProvider const*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	CVector const& VehiclePart::GetLastHitPos() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePart::SetPassedToAnotherMapStatus()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePart::Registration()
	{
		m_propertiesMap["Durability"] = 19;
		m_propertiesMap["MaxDurability"] = 20;
		m_propertiesMap["Price"] = 21;
	}

	eGObjPropertySaveStatus VehiclePart::GetPropertySaveStatus(int) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	int VehiclePart::GetPropertyId(char const*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePart::BreakModel(BreakData const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	VehiclePart::VehiclePart(VehiclePartPrototypeInfo const& prototypeInfo) :
        PhysicBody(prototypeInfo),
        m_price(prototypeInfo.m_price),
        m_durability(prototypeInfo.m_durability, 0.0, prototypeInfo.m_durability, 0.0)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePart::_SetAllPropertiesToMax()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePart::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool VehiclePart::_OnDurabilityValueBeforeApplyModifier(Modifier const&, float&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool VehiclePart::_GetPropertyInternal(int, m3d::AIParam&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	float VehiclePart::_GetModelPartHealth(int) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePart::_OnDurabilityValueAfterChange(float)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePart::_InternalCreateVisualPart()
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool VehiclePart::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	VehiclePart::~VehiclePart()
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Object* VehiclePart::CreateObject()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePart::SaveDecalsRuntime(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePart::_CalcMeshToBreak(BreakModelData&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePart::_AddDecal(CVector const&, CVector const&, CVector const&, unsigned, int)
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Object* VehiclePart::Clone()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePart::_RecalcDecals(unsigned, unsigned)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePart::LoadDecalsRuntime(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}
}
