#include "vehiclepart.h"
#include "server/objects/physicbodies/physichelpers.h"
#include <stdexcept>

namespace ai
{
	CVector const& VehiclePartPrototypeInfo::GetSize() const
	{
		throw std::logic_error("Not implemented");
	}

	VehiclePartPrototypeInfo::VehiclePartPrototypeInfo()
	{
		throw std::logic_error("Not implemented");
	}

	Obj* VehiclePartPrototypeInfo::CreateTargetObject() const
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclePartPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	VehiclePartPrototypeInfo::~VehiclePartPrototypeInfo()
	{
		throw std::logic_error("Not implemented");
	}

	bool VehiclePartPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclePartPrototypeInfo::_InitModelMeshes(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	VehiclePart::BreakData::BreakData()
	{
		throw std::logic_error("Not implemented");
	}

	VehiclePart::BreakModelData::BreakModelData()
	{
		throw std::logic_error("Not implemented");
	}

	VehiclePart::ModelPart::ModelPart()
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclePart::RenderDebugInfo() const
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclePart::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	CStr const& VehiclePart::GetPartName() const
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclePart::TransferPhysicParamsToSceneGraphNode()
	{
		throw std::logic_error("Not implemented");
	}

	CompoundVehiclePart* VehiclePart::GetOwnerCompoundVehiclePart()
	{
		throw std::logic_error("Not implemented");
	}

	CompoundVehiclePart const* VehiclePart::GetOwnerCompoundVehiclePart() const
	{
		throw std::logic_error("Not implemented");
	}

	float VehiclePart::GetDurabilityCoeffForDamageType(DamageType) const
	{
		throw std::logic_error("Not implemented");
	}

	CStr const& VehiclePart::GetBlowEffectName() const
	{
		throw std::logic_error("Not implemented");
	}

	unsigned VehiclePart::GetRepairPrice() const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* VehiclePart::GetClass() const
	{
		throw std::logic_error("Not implemented");
	}

	CVector VehiclePart::GetSize() const
	{
		throw std::logic_error("Not implemented");
	}

	CStr VehiclePart::GetPropertyName(int) const
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclePart::DumpPhysicInfo(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw std::logic_error("Not implemented");
	}

	VehiclePartPrototypeInfo const* VehiclePart::GetPrototypeInfo() const
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclePart::SetPartName(CStr const&)
	{
		throw std::logic_error("Not implemented");
	}

	bool VehiclePart::ApplyModifier(Modifier const&)
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclePart::SetOwnerCompoundVehiclePart(CompoundVehiclePart*)
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclePart::DefineSuppressedLPs()
	{
		throw std::logic_error("Not implemented");
	}

	NumericInRangeRegenerating<float> const& VehiclePart::Durability() const
	{
		throw std::logic_error("Not implemented");
	}

	NumericInRangeRegenerating<float>& VehiclePart::Durability()
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclePart::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclePart::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclePart::Update(float, unsigned)
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclePart::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclePart::Remove()
	{
		throw std::logic_error("Not implemented");
	}

	bool VehiclePart::SetPropertyById(int, m3d::AIParam const&)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* VehiclePart::GetBaseClass()
	{
		throw std::logic_error("Not implemented");
	}

	float VehiclePart::GetRepairPriceForOneUnit() const
	{
		throw std::logic_error("Not implemented");
	}

	unsigned VehiclePart::GetPrice(IPriceCoeffProvider const*) const
	{
		throw std::logic_error("Not implemented");
	}

	CVector const& VehiclePart::GetLastHitPos() const
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclePart::SetPassedToAnotherMapStatus()
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclePart::Registration()
	{
		throw std::logic_error("Not implemented");
	}

	eGObjPropertySaveStatus VehiclePart::GetPropertySaveStatus(int) const
	{
		throw std::logic_error("Not implemented");
	}

	int VehiclePart::GetPropertyId(char const*) const
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclePart::BreakModel(BreakData const&)
	{
		throw std::logic_error("Not implemented");
	}

	VehiclePart::VehiclePart(VehiclePartPrototypeInfo const& prototypeInfo) : PhysicBody(prototypeInfo)
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclePart::_SetAllPropertiesToMax()
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclePart::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
	{
		throw std::logic_error("Not implemented");
	}

	bool VehiclePart::_OnDurabilityValueBeforeApplyModifier(Modifier const&, float&)
	{
		throw std::logic_error("Not implemented");
	}

	bool VehiclePart::_GetPropertyInternal(int, m3d::AIParam&) const
	{
		throw std::logic_error("Not implemented");
	}

	float VehiclePart::_GetModelPartHealth(int) const
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclePart::_OnDurabilityValueAfterChange(float)
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclePart::_InternalCreateVisualPart()
	{
		throw std::logic_error("Not implemented");
	}

	bool VehiclePart::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
	{
		throw std::logic_error("Not implemented");
	}

	VehiclePart::~VehiclePart()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* VehiclePart::CreateObject()
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclePart::SaveDecalsRuntime(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclePart::_CalcMeshToBreak(BreakModelData&)
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclePart::_AddDecal(CVector const&, CVector const&, CVector const&, unsigned, int)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* VehiclePart::Clone()
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclePart::_RecalcDecals(unsigned, unsigned)
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclePart::LoadDecalsRuntime(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}
}
