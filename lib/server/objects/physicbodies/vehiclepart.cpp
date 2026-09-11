#include "vehiclepart.h"
#include "server/objects/physicbodies/physichelpers.h"
#include <stdexcept>

#include "m3dapp.h"
#include "core/log.h"
#include "geoms/box.h"
#include "geoms/trimesh.h"
#include "scene/scenegraph.h"
#include "scene/servers/dataserver.h"
#include "server/objects/basket.h"
#include "server/objects/ware.h"
#include "server/objects/vehicle.h"
#include "server/objects/base/prototypemanager.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehiclePart)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehiclePart);

    CVector const& VehiclePartPrototypeInfo::GetSize() const
    {
        // RVA 0x6D3320 - a part's nominal size is its first collision shape's.
        // NOTE: the shipped code does not check the vector is non-empty.
        return m_collisionInfos.front().m_size;
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
        return new VehiclePart(*this);
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
        for (auto& ind : m_inds)
        {
            delete[] ind;
        }

        for (auto& mesh : m_modelMeshes)
        {
            delete mesh;
        }

        for (auto& bound : m_boundsForMeshes)
        {
            delete bound;
        }
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

                        auto const trisCount = mesh.m_numFaces * 3;
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

                        auto const bound = trimesh->GetAabb();
                        CVector size;
                        size.x = bound.m_box[3] - bound.m_box[0];
                        size.y = bound.m_box[4] - bound.m_box[1];
                        size.z = bound.m_box[5] - bound.m_box[2];

                        auto box = ai::Box::CreateObject(nullptr, size, nullptr);
                        auto const geomX = (bound.m_box[3] + bound.m_box[0]) * 0.5;
                        auto const geomY = (bound.m_box[1] + bound.m_box[4]) * 0.5;
                        auto const geomZ = (bound.m_box[2] + bound.m_box[5]) * 0.5;
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
                        auto& group = mdl->GetGroup(i);
                        m3d::SafeFloatAttrib(m_groupHealthes[i], groupsHealthNode, group.Name.c_str());
                    }
                }
            }
        }
    }

    VehiclePart::BreakData::BreakData()
    {
        // RVA 0x5CE770 - the default hit is at the part's own origin travelling
        // along +X, with a matching normal, so a BreakData that is only
        // partially filled in still describes a usable impact rather than a
        // degenerate one. A decalId of -1 means "leave no mark".
        this->point.x = 0.0;
        this->point.y = 0.0;
        this->point.z = 0.0;
        this->dir.x = 1.0;
        this->dir.y = 0.0;
        this->dir.z = 0.0;
        this->normal.x = 1.0;
        this->normal.y = 0.0;
        this->normal.z = 0.0;
        this->damage = 0.0;
        this->decalId = -1;
    }

    VehiclePart::BreakData::BreakData(BreakData const& other)
    {
        // NOTE: the header declares this but the shipped build never emitted
        // it - every caller passes a BreakData by const reference - so unlike
        // the default constructor its body does not come from the binary. A
        // memberwise copy is the only thing it can have been.
        this->point = other.point;
        this->dir = other.dir;
        this->normal = other.normal;
        this->damage = other.damage;
        this->decalId = other.decalId;
    }

    VehiclePart::BreakModelData::BreakModelData()
    {
        // RVA 0x6CEF90 - NOTE: groupId, pos, dir and normal are deliberately
        // left uninitialised; BreakModel fills them in before the struct is
        // handed to _CalcMeshToBreak.
        this->mdl = nullptr;
        this->cfg = nullptr;
        this->meshId = -1;
    }

    VehiclePart::ModelPart::ModelPart()
    {
        this->maxHealth = 0.0;
        this->health = 0.0;
    }

    void VehiclePart::RenderDebugInfo() const
    {
        PhysicBody::RenderDebugInfo();
    }

    void VehiclePart::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr const& VehiclePart::GetPartName() const
    {
        return m_partName;
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
        // RVA 0x6CDC70
        return m_ownerCompoundPart;
    }

    CompoundVehiclePart const* VehiclePart::GetOwnerCompoundVehiclePart() const
    {
        return m_ownerCompoundPart;
    }

    float VehiclePart::GetDurabilityCoeffForDamageType(DamageType damageType) const
    {
        return m_durabilityCoeffsForDamageTypes[damageType];
    }

    CStr const& VehiclePart::GetBlowEffectName() const
    {
        // RVA 0x6BC5E0
        return m_blowEffectName;
    }

    unsigned VehiclePart::GetRepairPrice() const
    {
        // RVA 0x6CF110 - what it costs to bring this part back to full.
        if (m_durability.maxValue().get() < 0.001f)
        {
            return 0;
        }

        auto const* proto = GetPrototypeInfo();

        // A part worth less than one unit of currency is still charged for as
        // if it cost one, so trivially priced parts are not repaired for free.
        float const price = m_price.get();
        float const priceMult = (price >= 1.0f) ? price : 1.0f;

        return GetIntRepairPrice(GetDurabilityRepairCoeff(m_durability) * proto->m_repairCoef * priceMult);
    }

    m3d::Class* VehiclePart::GetClass() const
    {
        return RT_CLASS_LOCAL(VehiclePart);
    }

    CVector VehiclePart::GetSize() const
    {
        auto const* box = RT_DYNCAST(m_pGeoms.front()->GetGeom(), Box);
        if (box)
        {
            return box->GetSize();
        }

        M3D_LOG_INFO("GetSize or empty node");
        return {0.0, 0.0, 0.0};
    }

    CStr VehiclePart::GetPropertyName(int id) const
    {
        // RVA 0x6BB600 - a linear scan, since only the name->id direction of
        // the map is indexed.
        for (auto const& entry : m_propertiesMap)
        {
            if (entry.second == id)
            {
                return entry.first;
            }
        }
        return Obj::GetPropertyName(id);
    }

    void VehiclePart::DumpPhysicInfo(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x6D20A0
        PhysicBody::DumpPhysicInfo(xmlFile, xmlNode);
        xmlNode->SetAttribute("PartName", m_partName.c_str());
        xmlNode->SetAttribute("Size", CStr(GetSize()).c_str());
    }

    VehiclePartPrototypeInfo const* VehiclePart::GetPrototypeInfo() const
    {
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), VehiclePartPrototypeInfo const);
    }

    void VehiclePart::SetPartName(CStr const& newName)
    {
        m_partName = newName;
    }

    bool VehiclePart::ApplyModifier(Modifier const& modifier)
    {
        // RVA 0x6D0D00 - the base class gets first refusal, then the two
        // properties a part owns itself. Returning false means no one has
        // claimed the property.
        if (Obj::ApplyModifier(modifier))
        {
            return true;
        }

        if (modifier.m_PropertyName == "price")
        {
            // Price is modified relative to the prototype's, so a part that has
            // already been repriced does not compound the modifier.
            float const base = static_cast<float>(GetPrototypeInfo()->m_price);
            m_price.ApplyModifier(modifier, base);
            return true;
        }

        if (modifier.m_PropertyName == "dur")
        {
            // Durability is relative to this part's own maximum rather than to
            // the prototype's, so upgrades that raised the maximum scale with it.
            m_durability.value().ApplyModifier(modifier, m_durability.maxValue().get());
            return true;
        }

        return false;
    }

    void VehiclePart::SetOwnerCompoundVehiclePart(CompoundVehiclePart* compoundVehiclePart)
    {
        // RVA 0x6CDC90
        m_ownerCompoundPart = compoundVehiclePart;
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
                    for (int meshNum = 0; meshNum < mdl->GetNumMeshes(); ++meshNum)
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

    void VehiclePart::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        // RVA 0x6BB5A0 - this class's own properties, then the base class's.
        for (auto const& entry : m_propertiesMap)
        {
            Props.insert(entry.second);
        }
        Obj::GetPropertiesIDs(Props);
    }

    void VehiclePart::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
    {
        // RVA 0x6BB540
        for (auto const& entry : m_propertiesMap)
        {
            Props.insert(entry.first);
        }
        Obj::GetPropertiesNames(Props);
    }

    void VehiclePart::Update(float elapsedTime, unsigned workTime)
    {
        Obj::Update(elapsedTime, workTime);
        if (!m_MakeSplash)
        {
            if (m_SplashEffect)
            {
                // TODO: check this
                // Process children using iterative DFS
                std::vector<m3d::Object*> stack;
                stack.push_back(dynamic_cast<m3d::Object*>(m_SplashEffect));

                while (!stack.empty())
                {
                    m3d::Object* current = stack.back();
                    stack.pop_back();

                    // Process all siblings of the current node
                    m3d::SgNode* sibling = dynamic_cast<m3d::SgNode*>(current->GetFirstChild());
                    while (sibling)
                    {
                        sibling->CanBeFree();

                        // If this sibling has children, add to stack for processing
                        if (sibling->GetFirstChild())
                        {
                            stack.push_back(sibling->GetFirstChild());
                        }

                        // Move to next sibling
                        sibling = dynamic_cast<m3d::SgNode*>(sibling->GetNextSibling());
                    }
                }

                m_SplashEffect->GetGraph()->InsertInRemoveIfFree(m_SplashEffect);
                m_SplashEffect = nullptr;
            }
        }
        m_MakeSplash = false;
    }

    void VehiclePart::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehiclePart::Remove()
    {
        Obj::Remove();
    }

    bool VehiclePart::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        // TODO: check and refactor
        if (propertyId == 19)
        {
            m_durability.value().SetUnsafe(newValue.GetAsFloat());
        }
        else
        {
            if (propertyId == 20)
            {
                m_durability.maxValue().set(newValue.GetAsFloat());
            }
            else
            {
                if (propertyId != 21)
                    return ai::Obj::SetPropertyById(propertyId, newValue);

                m_price.set(newValue.GetAsFloat());
            }
        }
        return 1;
    }

    m3d::Class* VehiclePart::GetBaseClass()
    {
        return RT_CLASS_LOCAL(PhysicBody);
    }

    float VehiclePart::GetRepairPriceForOneUnit() const
    {
        // RVA 0x6D33A0 - the rate quoted to the player. NOTE: it is worked out
        // from a stand-in part sitting at durability 1, not from this part's
        // actual condition, so the quote does not change as the part wears.
        if (m_durability.maxValue().get() < 0.001f)
        {
            return 0.0f;
        }

        auto const* proto = GetPrototypeInfo();
        if (!proto)
        {
            return 0.0f;
        }

        NumericInRange<float> const oneUnit(1.0f, m_durability.minValue().get(), m_durability.maxValue().get());
        return GetDurabilityRepairCoeff(oneUnit) * proto->m_repairCoef * m_price.get();
    }

    unsigned VehiclePart::GetPrice(IPriceCoeffProvider const* priceCoeffProvider) const
    {
        // RVA 0x6CF0C0 - a worn part fetches proportionally less.
        float const durabilityCoeff = GetDurabilityPriceCoeff(m_durability);
        return GetIntPrice(GetPriceCoeff(priceCoeffProvider) * durabilityCoeff * m_price.get());
    }

    CVector const& VehiclePart::GetLastHitPos() const
    {
        // RVA 0x602650
        return m_lastHitPos;
    }

    void VehiclePart::SetPassedToAnotherMapStatus()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehiclePart::Registration()
    {
        m_propertiesMap["Durability"] = 19;
        m_propertiesMap["MaxDurability"] = 20;
        m_propertiesMap["Price"] = 21;
    }

    eGObjPropertySaveStatus VehiclePart::GetPropertySaveStatus(int id) const
    {
        // RVA 0x6BB000
        auto const it = m_propertiesSaveStatesMap.find(id);
        if (it == m_propertiesSaveStatesMap.end())
        {
            return Obj::GetPropertySaveStatus(id);
        }
        return it->second;
    }

    int VehiclePart::GetPropertyId(char const* propName) const
    {
        auto it = VehiclePart::m_propertiesMap.find(propName);
        if (it != VehiclePart::m_propertiesMap.end())
        {
            return it->second;
        }

        return PhysicBody::GetPropertyId(propName);
    }

    namespace
    {
        // RVA 0xA02258 - picked at random when a piece is destroyed outright.
        char const* const JADED_EFFECT_NAMES[3] = {
            "ET_PS_VEH_PART_JADED_FIRE",
            "ET_PS_VEH_PART_JADED_SMOKE",
            "ET_PS_VEH_PART_JADED_SPARKLE",
        };

        // RVA 0x5CCAF0 - note this is rand() scaled by RAND_MAX+1, so the top
        // bound is exclusive.
        int IntRandom(int highBound)
        {
            return highBound * rand() / 0x8000;
        }
    }  // namespace

    void VehiclePart::BreakModel(BreakData const& breakData)
    {
        // RVA 0x6D7DF0 - a part with no engine model has nothing to break.
        if (GetPrototypeInfo()->m_engineModelId == -1)
        {
            return;
        }

        BreakModelData modelData;
        modelData.mdl = nullptr;
        modelData.cfg = nullptr;
        modelData.meshId = -1;
        modelData.pos = breakData.point;
        modelData.dir = breakData.dir;
        modelData.normal = breakData.normal;

        // Casts a ray along the impact direction to find which mesh was hit and
        // rewrites pos/dir/normal into that mesh's frame.
        _CalcMeshToBreak(modelData);
        if (modelData.meshId == -1)
        {
            return;
        }

        auto const& group = modelData.mdl->GetGroup(modelData.groupId);
        auto& part = m_modelParts[modelData.groupId];
        unsigned const curVar = modelData.cfg->m_groupVariants[modelData.groupId];

        CVector const pos = modelData.pos;

        // cross(dir, dir + up) written out as the shipped code has it. It
        // reduces to cross(dir, up) = (-dir.z, 0, dir.x), but the literal form
        // is kept because the float rounding differs.
        CVector tangent;
        tangent.x = (modelData.dir.y * modelData.dir.z) - (modelData.dir.z * (modelData.dir.y + 1.0f));
        tangent.y = (modelData.dir.z * modelData.dir.x) - (modelData.dir.z * modelData.dir.x);
        tangent.z = ((modelData.dir.y + 1.0f) * modelData.dir.x) - (modelData.dir.y * modelData.dir.x);

        // A hit straight down the up axis leaves no usable tangent.
        if (sqrt(tangent.x * tangent.x + tangent.z * tangent.z + tangent.y * tangent.y) < 0.0000099999997f)
        {
            tangent.x = 1.0f;
            tangent.y = 0.0f;
            tangent.z = 0.0f;
        }

        float const tangentScale =
            1.0f / sqrt(tangent.x * tangent.x + tangent.z * tangent.z + tangent.y * tangent.y + 0.00000011920929f);
        CVector const decalTangent(tangent.x * tangentScale, tangent.y * tangentScale, tangent.z * tangentScale);

        // NOTE: the decal is oriented by the impact direction, not by the
        // surface normal the ray came back with.
        _AddDecal(pos, modelData.dir, decalTangent, modelData.meshId, breakData.decalId);

        // A group with a single variant has no damaged state to show.
        if (group.m_variants.size() <= 1)
        {
            return;
        }

        part.health = part.health - breakData.damage;
        if (part.health < 0.0f)
        {
            part.health = 0.0f;
        }
        if (part.health > part.maxHealth)
        {
            part.health = part.maxHealth;
        }

        unsigned const numVariants = group.m_variants.size();
        int const healthSteps = static_cast<int>(
            static_cast<double>(numVariants - 1) / part.maxHealth * part.health + 1.0 - 0.001);
        unsigned newVar = numVariants - healthSteps - 1;
        if (newVar >= numVariants)
        {
            newVar = numVariants - 1;
        }

        if (curVar != newVar)
        {
            auto* owner = GetOwner();
            if (owner && IS_KIND_OF(owner, Vehicle))
            {
                owner->CauseEvent(GE_PART_BROKEN, 0.0f, m3d::AIParam(), m3d::AIParam());
            }

            modelData.cfg->m_groupVariants[modelData.groupId] = static_cast<unsigned char>(newVar);
            modelData.mdl->FromGroupVariants(*modelData.cfg);
            modelData.mdl->CalculateMeshes(*modelData.cfg);
            m_cfgNum = modelData.cfg->m_num;
            DefineSuppressedLPs();

            // The effect is spawned in world space, so both the hit point and
            // the impact direction go through the node's current transform -
            // the direction without the translation.
            auto const& xf = m_Node->GetCurrentMatrix();
            CVector breakPos;
            breakPos.x = xf._31 * modelData.pos.z + xf._21 * modelData.pos.y + xf._11 * modelData.pos.x + xf._41;
            breakPos.y = xf._32 * modelData.pos.z + xf._22 * modelData.pos.y + xf._12 * modelData.pos.x + xf._42;
            breakPos.z = xf._33 * modelData.pos.z + xf._23 * modelData.pos.y + xf._13 * modelData.pos.x + xf._43;

            CVector worldDir;
            worldDir.x = xf._31 * modelData.dir.z + xf._21 * modelData.dir.y + xf._11 * modelData.dir.x;
            worldDir.y = xf._32 * modelData.dir.z + xf._22 * modelData.dir.y + xf._12 * modelData.dir.x;
            worldDir.z = xf._33 * modelData.dir.z + xf._23 * modelData.dir.y + xf._13 * modelData.dir.x;

            // The effect faces back along the incoming shot.
            CMatrix rot;
            rot.lookAtLH(
                CVector(0.0f, 0.0f, 0.0f),
                CVector(-worldDir.x, -worldDir.y, -worldDir.z),
                CVector(0.0f, 1.0f, 0.0f));
            Quaternion q;
            q.FromMatrix(rot);
            PhysicBody::CreateEffectNode(CStr("ET_PS_VEH_PART_BROKEN"), breakPos, q, true, 1.0f);

            // Decals were placed against the old mesh, so move them onto the
            // first mesh of the variant that just replaced it.
            auto const& newVariant = group.m_variants[newVar];
            if (!newVariant.empty())
            {
                _RecalcDecals(modelData.meshId, group.MeshesId[newVariant.front()]);
            }
        }

        // Once a piece is destroyed it burns, smokes or sparks until repaired.
        if (part.health < 0.0099999998f && part.maxHealth > 0.0099999998f && !part.jadedEffect)
        {
            CStr const effectName(JADED_EFFECT_NAMES[IntRandom(3)]);
            part.jadedEffect = PhysicBody::CreateNode(effectName, 0, CVector(1.0f, 1.0f, 1.0f), nullptr, false);
            m_Node->AddChild(part.jadedEffect);
            part.jadedEffect->SetOriginAbs(pos);
            part.jadedEffect->UpdateXForm(0, true);
        }
    }

    VehiclePart::VehiclePart(VehiclePartPrototypeInfo const& prototypeInfo) :
        PhysicBody(prototypeInfo),
        m_price(prototypeInfo.m_price),
        m_durability(prototypeInfo.m_durability, 0.0, prototypeInfo.m_durability, 0.0)
    {
        m_durability.m_AfterValueChange =
            new AfterChangeFloatCallback(*this, &VehiclePart::_OnDurabilityValueAfterChange);
        m_durability.m_BeforeValueApplyModifier =
            new BeforeApplyModifierFloatCallback(*this, &VehiclePart::_OnDurabilityValueBeforeApplyModifier);

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
        // RVA 0x6D0480 - durability is the only property a plain part has.
        m_durability.setToMax();
    }

    void VehiclePart::RegisterProperty(char const* Name, int id, eGObjPropertySaveStatus saveStatus)
    {
        // RVA 0x6D9060 - a save status of 0 is the default and is not recorded,
        // so GetPropertySaveStatus falls through to the base class for it.
        m_propertiesMap[CStr(Name)] = id;
        if (saveStatus)
        {
            m_propertiesSaveStatesMap[id] = saveStatus;
        }
    }

    bool VehiclePart::_OnDurabilityValueBeforeApplyModifier(Modifier const&, float&)
    {
        return false;
    }

    bool VehiclePart::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        float val = 0.0;
        switch (propertyId)
        {
        case 19:
            val = m_durability.value().get();
            break;
        case 20:
            val = m_durability.maxValue().get();
            break;
        case 21:
            val = m_price.get();
            break;
        default:
            return ai::Obj::_GetPropertyInternal(propertyId, retVal);
        }

        retVal = val;
        return true;
    }

    float VehiclePart::_GetModelPartHealth(int groupId) const
    {
        // RVA 0x6D0E50 - NOTE: the index is unchecked, as shipped.
        return m_modelParts[groupId].health;
    }

    void VehiclePart::_OnDurabilityValueAfterChange(float oldDurabilityValue)
    {
        // RVA 0x6D50B0 - this is the repair path only. Damage picks its mesh
        // variants through BreakModel, which knows where it was hit; a repair
        // has no impact point, so the whole part is reassessed from its new
        // durability and every piece of battle damage is thrown away.
        if (m_durability.value().get() <= oldDurabilityValue)
        {
            return;
        }

        if (!m_Node)
        {
            return;
        }

        m3d::Configuration* cfg = nullptr;
        m_Node->GetProperty(8707, &cfg);

        m3d::AnimatedModel* mdl = nullptr;
        m_Node->GetServer()->GetItemProperty(m_Node->GetServerHandle(), 16394, &mdl);
        if (!mdl)
        {
            return;
        }

        // The two run in lockstep - one model part per mesh group - so a
        // mismatch means the visual model is not the one this part was built
        // against and nothing can be matched up safely.
        if (m_modelParts.size() != mdl->GetGroupsNum())
        {
            return;
        }

        float const delta = m_durability.value().get() - oldDurabilityValue;
        bool variantsChanged = false;

        for (unsigned i = 0; i < m_modelParts.size(); ++i)
        {
            auto const& group = mdl->GetGroup(i);
            auto& part = m_modelParts[i];
            unsigned const curVar = cfg->m_groupVariants[i];

            // Every piece heals by the same absolute amount, not in proportion
            // to its own share of the part's durability.
            part.health = part.health + delta;
            if (part.health < 0.0f)
            {
                part.health = 0.0f;
            }
            if (part.health > part.maxHealth)
            {
                part.health = part.maxHealth;
            }

            // Variant 0 is the intact mesh and the last is the most battered,
            // so the index tracks damage taken rather than health left.
            unsigned const numVariants = group.m_variants.size();
            unsigned newVar = static_cast<unsigned>(
                static_cast<double>(part.maxHealth - part.health) / part.maxHealth *
                static_cast<double>(numVariants));

            // NOTE: an empty variant list underflows here exactly as it does in
            // the shipped build, leaving newVar at 0xFFFFFFFF.
            if (newVar >= group.m_variants.size())
            {
                newVar = group.m_variants.size() - 1;
            }

            if (curVar != newVar)
            {
                // NOTE: m_groupVariants holds bytes, so only the low byte of
                // the index is stored while the comparison above uses all 32
                // bits. Groups never carry more than 256 variants in practice.
                cfg->m_groupVariants[i] = static_cast<unsigned char>(newVar);
                variantsChanged = true;
            }

            // The scorch/smoke effect belongs to the damage that is being
            // repaired away.
            if (part.jadedEffect)
            {
                part.jadedEffect->GetGraph()->RemoveNode(part.jadedEffect);
                part.jadedEffect = nullptr;
            }
        }

        if (variantsChanged)
        {
            mdl->FromGroupVariants(*cfg);
            mdl->CalculateMeshes(*cfg);
            m_cfgNum = cfg->m_num;
            DefineSuppressedLPs();
        }

        // Bullet holes and dents go with the damage too.
        for (auto& [id, node] : m_decals)
        {
            if (node)
            {
                node->GetGraph()->RemoveNode(node);
                node = nullptr;
            }
        }
        m_decals.clear();

        auto* owner = GetOwner();
        if (owner && IS_KIND_OF(owner, Vehicle))
        {
            static_cast<Vehicle*>(owner)->HealWheels();
        }
    }

    void VehiclePart::_InternalCreateVisualPart()
    {
        PhysicBody::_InternalCreateVisualPart();
        DefineSuppressedLPs();
        if (m_Node)
        {
            if (GetPassedToAnotherMapStatus())
            {
                RETRUXX_NOT_IMPLEMENTED;
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
                        if (mesh.m_meshType == 1 && mesh.m_numNode >= 0)
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

    bool VehiclePart::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        auto prototypeInfo = (ai::VehiclePartPrototypeInfo const*)GetPrototypeInfo();
        switch (propertyId)
        {
        case 19:
            retVal = prototypeInfo->m_durability;
            return true;

        case 20:
            retVal = prototypeInfo->m_durability;
            return true;

        case 21:
            retVal = prototypeInfo->m_price;
            return true;

        default:
            return ai::Obj::_GetPropertyDefaultInternal(propertyId, retVal);
        }
        return false;
    }

    VehiclePart::~VehiclePart()
    {
        if (m_SplashEffect)
        {
            // TODO: check this
            // Process children using iterative DFS
            std::vector<m3d::Object*> stack;
            stack.push_back(dynamic_cast<m3d::Object*>(m_SplashEffect));

            while (!stack.empty())
            {
                m3d::Object* current = stack.back();
                stack.pop_back();

                // Process all siblings of the current node
                m3d::SgNode* sibling = dynamic_cast<m3d::SgNode*>(current->GetFirstChild());
                while (sibling)
                {
                    sibling->CanBeFree();

                    // If this sibling has children, add to stack for processing
                    if (sibling->GetFirstChild())
                    {
                        stack.push_back(sibling->GetFirstChild());
                    }

                    // Move to next sibling
                    sibling = dynamic_cast<m3d::SgNode*>(sibling->GetNextSibling());
                }
            }

            m_SplashEffect->GetGraph()->InsertInRemoveIfFree(m_SplashEffect);
            m_SplashEffect = nullptr;
        }
    }

    m3d::Object* VehiclePart::CreateObject()
    {
        // RVA 0x6D01C0 - parts only ever come from a prototype.
        SYS_ERROR(CStr("!\"Object cannot be created directly\""));
        return nullptr;
    }

    void VehiclePart::SaveDecalsRuntime(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehiclePart::_CalcMeshToBreak(BreakModelData&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehiclePart::_AddDecal(CVector const&, CVector const&, CVector const&, unsigned, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* VehiclePart::Clone()
    {
        // RVA 0x6D0000
        SYS_ERROR(CStr("!\"Object cannot be cloned\""));
        return nullptr;
    }

    void VehiclePart::_RecalcDecals(unsigned, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehiclePart::LoadDecalsRuntime(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
