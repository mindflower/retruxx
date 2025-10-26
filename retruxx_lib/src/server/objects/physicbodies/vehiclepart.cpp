#include "vehiclepart.h"
#include "server/objects/physicbodies/physichelpers.h"
#include <stdexcept>

#include "core/log.h"

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
		m_blowEffectName = "ET_PS_HARD_BLOW";
		m_canBeUsedInAutogenerating = 1;
		m_weaponPrototypeId = -1;
		m_durability = 0.0;
		m_repairCoef = 1.0;
		m_durabilityCoeffsForDamageTypes[0] = 0.0;
		m_durabilityCoeffsForDamageTypes[1] = 0.0;
		m_durabilityCoeffsForDamageTypes[2] = 0.0;
		m_durabilityCoeffsForDamageTypes[3] = 0.0;
	}

	Obj* VehiclePartPrototypeInfo::CreateTargetObject() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclePartPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
	{
		ai::PhysicBodyPrototypeInfo::RefreshFromXml(xmlFile, xmlNode);
		if (m_collisionInfos.front().m_geomType != GEOM_TYPE_BOX)
		{
			M3D_LOG_ERR("Error: collision geom in vehicle part name = '" + m_engineModelName + "' is not BOX");
		}
		m3d::SafeVectorAttrib(this->m_collisionInfos.front().m_size, xmlNode, "Size");
		_InitModelMeshes(xmlFile, xmlNode);
	}

	VehiclePartPrototypeInfo::~VehiclePartPrototypeInfo()
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool VehiclePartPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
	{
		auto result = ai::PhysicBodyPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
		if (result)
		{
			m3d::SafeStrAttrib(this->m_blowEffectName, xmlNode, "BlowEffect");
			m3d::SafeFloatAttrib(this->m_durability, xmlNode, "Durability");

			CStr strDurabilityCoeffs;
			m3d::SafeStrAttrib(strDurabilityCoeffs, xmlNode, "DurCoeffsForDamageTypes");
			if (!strDurabilityCoeffs.empty())
			{
				std::vector<CStr> strs;
				m3d::Tokenize(strDurabilityCoeffs, strs, "(), ;\t");
				for (int i = 0; i < 4 && i < strs.size(); ++i)
				{
					m_durabilityCoeffsForDamageTypes[i] = strToFloat(strs[i]);
					if (m_durabilityCoeffsForDamageTypes[i] < -25.1 || m_durabilityCoeffsForDamageTypes[i] > 25.0)
					{
						M3D_CRITICAL_ERROR("invalid DurCoeffsForDamageTypes for '" + m_prototypeName + "'");
					}

                }
			}
			else
			{
				m_durabilityCoeffsForDamageTypes[0] = 0.0;
				m_durabilityCoeffsForDamageTypes[1] = 0.0;
				m_durabilityCoeffsForDamageTypes[2] = 0.0;
				m_durabilityCoeffsForDamageTypes[3] = 0.0;
			}

			std::vector<CStr> strs;
			CStr strLoadPoints;
			m3d::SafeStrAttrib(strLoadPoints, xmlNode, "LoadPoints");
			m3d::Tokenize(strDurabilityCoeffs, strs, "(), ;\t");
			m_loadPoints.insert(strs.begin(), strs.end());

			m3d::SafeUintAttrib(this->m_price, xmlNode, "Price");
			m3d::SafeFloatAttrib(this->m_repairCoef, xmlNode, "RepairCoef");
			m3d::SafeBoolAttrib(this->m_canBeUsedInAutogenerating, xmlNode, "CanBeUsedInAutogenerating");
		}

		return result;
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
