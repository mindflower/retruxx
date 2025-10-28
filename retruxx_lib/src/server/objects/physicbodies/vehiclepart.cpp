#include "vehiclepart.h"
#include "server/objects/physicbodies/physichelpers.h"
#include <stdexcept>

#include "m3dapp.h"
#include "core/log.h"
#include "geoms/box.h"
#include "geoms/trimesh.h"
#include "scene/servers/dataserver.h"
#include "server/objects/basket.h"

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

	void VehiclePartPrototypeInfo::_InitModelMeshes(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
	{
		if (m_engineModelId != -1)
		{
			m3d::AnimatedModel* mdl = nullptr;
			M3D_APP->GetAnimatedModelsServer().GetItemProperty(m_engineModelId, 16394, &mdl);
			if (mdl)
			{
			    if (m_modelMeshes.empty())
			    {
					M3D_ASSERT(m_boundsForMeshes.empty());
					M3D_ASSERT(m_verts.empty());
			        M3D_ASSERT(m_vertsStride.empty());
					M3D_ASSERT(m_inds.empty());
					M3D_ASSERT(m_numsTris.empty());

					for (int i = 0; i < mdl->GetNumMeshes(); ++i)
					{
						auto& mesh = mdl->GetMesh(i);

						const auto trisCount = mesh.m_numFaces * 3;
						auto trimeshIndices = new int[trisCount];

						for (int trimeshIdx = 0; trimeshIdx < trisCount; ++trimeshIdx)
						{
							trimeshIndices[trimeshIdx] = mesh.m_tris[trimeshIdx];
						}

						auto trimesh = TriMesh::CreateObject(
							0,
							mesh.m_verts,
							mesh.m_numVertices,
							trimeshIndices,
							trisCount,
							nullptr,
                            mesh.m_VertexTypeSize);

						const auto bound = trimesh->GetAabb();
						CVector size;
						size.x = bound.m_box[3] - bound.m_box[0];
						size.y = bound.m_box[4] - bound.m_box[1];
						size.z = bound.m_box[5] - bound.m_box[2];

						auto box = ai::Box::CreateObject(nullptr, size, nullptr);
						const auto geomX = (bound.m_box[3] + bound.m_box[0]) * 0.5;
						const auto geomY = (bound.m_box[1] + bound.m_box[4]) * 0.5;
						const auto geomZ = (bound.m_box[2] + bound.m_box[5]) * 0.5;
						dGeomSetPosition(box->GetGeomId(), geomX, geomY, geomZ);

						float quat[4];
						quat[0] = 1.0;
						quat[1] = 0.0;
						quat[2] = 0.0;
						quat[3] = 0.0;
						dGeomSetQuaternion(box->GetGeomId(), quat);

						m_modelMeshes.push_back(trimesh);
						m_boundsForMeshes.push_back(box);
						m_verts.push_back(mesh.m_verts);
                        m_vertsStride.push_back(mesh.m_VertexTypeSize);
						m_inds.push_back(trimeshIndices);
						m_numsTris.push_back(mesh.m_numFaces);
					}

					m_groupHealthes.resize(mdl->GetGroupsNum());
                    auto dur = m_durability / mdl->GetGroupsNum();

					ref_ptr groupsHealthNode = xmlFile->CreateNode();
					xmlNode->GetFirstChild(groupsHealthNode, "GroupsHealth");
                    for (int i = 0; i < mdl->GetGroupsNum(); ++i)
                    {
						m_groupHealthes[i] = dur;
						auto& group =mdl->GetGroup(i);
						m3d::SafeFloatAttrib(m_groupHealthes[i], groupsHealthNode, group.Name.c_str());
                    }

			    }
			}
		}
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
		this->maxHealth = 0.0;
		this->health = 0.0;
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
		if (this->m_Node)
		{
			auto nodeAbsoluteRotation = ai::PhysicBody::GetNodeAbsoluteRotation();
			auto nodeAbsolutePosition = ai::PhysicBody::GetNodeAbsolutePosition();
			ai::PhysicBody::_SetScenegraphNode(nodeAbsolutePosition, nodeAbsoluteRotation);
		}
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
		const auto* box = RT_DYNCAST(m_pGeoms.front()->GetGeom(), Box);
		if (box)
		{
			return box->GetSize();
		}

		M3D_LOG_INFO("GetSize or empty node");
		return { 0.0, 0.0, 0.0 };
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

	void VehiclePart::SetPartName(CStr const& newName)
	{
		m_partName = newName;
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
		if (m_Node)
		{
			m3d::AnimatedModel* mdl = nullptr;
			m_Node->GetServer()->GetItemProperty(m_Node->GetServerHandle(), 16394, &mdl);
			if (mdl)
			{
				m3d::Configuration* cfg = nullptr;
				m_Node->GetProperty(8707, &cfg);
				m_suppressedLPs.clear();

                // TODO: check this!!!
				for (int nodeNum = 0; nodeNum < mdl->GetHeader().m_numNodes; ++nodeNum)
				{
					bool found = false;
					for (int meshNum = 0;  meshNum < mdl->GetNumMeshes(); ++meshNum)
					{
						if (mdl->GetBone(nodeNum).m_parentIdx == mdl->GetMesh(meshNum).m_numNode)
						{
							for (auto& cfgMesh : cfg->m_meshes)
							{
								if (cfgMesh == &mdl->GetMesh(meshNum))
								{
									found = true;
									break;
								}
							}
							if (found)
							{
								break;
                            }
						}

						if (found)
						{
						    m_suppressedLPs.insert(nodeNum);
						}
					}

					if (found)
					{
						m_suppressedLPs.insert(nodeNum);
					}
                }

				for (int nodeNum = 0; nodeNum < mdl->GetHeader().m_numNodes; ++nodeNum)
				{
					CStr name = mdl->GetBone(nodeNum).m_boneName;
					if (name.findsubstr("LP_LIGHT") != CStr_npos)
					{
						m_suppressedLPs.insert(nodeNum);
					}
				}

				for (int nodeNum = 0; nodeNum < mdl->GetHeader().m_numNodes; ++nodeNum)
				{
					CStr name = mdl->GetBone(nodeNum).m_boneName;
					if (name.findsubstr("LP_PROJECTOR") != CStr_npos)
					{
						m_suppressedLPs.insert(nodeNum);
					}
				}

				if (!m_suppressedLPs.empty())
				{
					m_Node->SetProperty(8714, &m_suppressedLPs);
				}
				else
				{
					m_Node->SetProperty(8714, nullptr);
				}
				m_Node->SetProperty(8715u, 0);
			}
		}
	}

	NumericInRangeRegenerating<float> const& VehiclePart::Durability() const
	{
		return this->m_durability;
	}

	NumericInRangeRegenerating<float>& VehiclePart::Durability()
	{
		return this->m_durability;
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
        m_durability.m_AfterValueChange = new AfterChangeFloatCallback(*this, &VehiclePart::_OnDurabilityValueAfterChange);
		m_durability.m_BeforeValueApplyModifier = new BeforeApplyModifierFloatCallback(*this, &VehiclePart::_OnDurabilityValueBeforeApplyModifier);

		this->m_blowEffectName = prototypeInfo.m_blowEffectName;
		this->m_durabilityCoeffsForDamageTypes[0] = prototypeInfo.m_durabilityCoeffsForDamageTypes[0];
		this->m_durabilityCoeffsForDamageTypes[1] = prototypeInfo.m_durabilityCoeffsForDamageTypes[1];
		this->m_durabilityCoeffsForDamageTypes[2] = prototypeInfo.m_durabilityCoeffsForDamageTypes[2];
		this->m_durabilityCoeffsForDamageTypes[3] = prototypeInfo.m_durabilityCoeffsForDamageTypes[3];
		this->m_lastHitPos = {0.0, 0.0, 0.0};
		this->m_passToAnotherMapData = 0;
		this->m_ownerCompoundPart = 0;


	    auto size = prototypeInfo.m_collisionInfos.front().m_size;
		if (sqrt(size.x * size.x + size.y * size.y + size.z * size.z) < 0.001)
		{
			size.x = 0.1;
			size.y = 0.1;
			size.z = 0.1;
		}

		ai::PhysicBody::ChangePhysicBodyByCollisionInfo(this->m_collisionInfos);
		dMassSetBoxTotal(&this->m_mass, prototypeInfo.m_massValue, size.x, size.y, size.z);
		this->m_SplashEffect = 0;
		this->m_MakeSplash = 0;

		m_modelParts.resize(prototypeInfo.m_groupHealthes.size());
		for (int i = 0; i < prototypeInfo.m_groupHealthes.size(); ++i)
		{
			m_modelParts[i].health = prototypeInfo.m_groupHealthes[i];
			m_modelParts[i].maxHealth = prototypeInfo.m_groupHealthes[i];
			m_modelParts[i].jadedEffect = 0;
        }
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
		PhysicBody::_InternalCreateVisualPart();
		DefineSuppressedLPs();
		if (m_Node)
		{
		    if (GetPassedToAnotherMapStatus())
		    {
				throw retruxx::logic_error("Not implemented");
				M3D_ASSERT(m_passToAnotherMapData);
                for (int i = 0; i < m_passToAnotherMapData->jadedEffects.size(); ++i)
                {
                    
                }
		    }
			else
			{
				// TODO: check this!!
			    for (int i = 0; i < m_loadDecalsData.size(); ++i)
			    {
					auto& data = m_loadDecalsData[i];
					m3d::AnimInfo* anim = nullptr;
					m_Node->GetProperty(1u, &anim);

					data.dd.toPutOn.transform = nullptr;
					if (anim && !anim->IsEmpty())
					{
						auto& mesh = anim->GetMesh(data.meshNum);
						if (mesh.m_meshType == 1 && mesh.m_numNode >=0)
						{
							data.dd.toPutOn.transform = &anim->GetBoneAnim(mesh.m_numNode).m_curMatrix;
						}
					}

					for (auto& decal : m_decals)
					{
						if (decal.second)
						{
							m_Node->AddChild(decal.second);
							decal.second->UpdateXForm(false, true);
						}
					}
			    }
			}
		}

		m_loadDecalsData.clear();
		delete m_passToAnotherMapData;
		m_passToAnotherMapData = nullptr;
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
