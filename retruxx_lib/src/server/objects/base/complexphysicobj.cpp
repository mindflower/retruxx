#include "complexphysicobj.h"
#include <stdexcept>
#include <ode/objects.h>

#include "core/ini.h"
#include "core/kernel.h"
#include "core/log.h"
#include "thirdparty/injecttools.h"
#include <server/resourcemanager.h>

#include "config.h"
#include "m3dapp.h"
#include "prototypemanager.h"
#include "server/objects/physicbodies/vehiclepart.h"
#include "objcontainer.h"
#include "ode/odecpp.h"
#include "scene/servers/dataserver.h"
#include "server/objects/guns/gun.h"
#include "server/objects/physicbodies/compoundvehiclepart.h"

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

const CStr NO_LP("NO_LP");

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
        return new ComplexPhysicObjPartDescription;
	}

	int ComplexPhysicObjPartDescription::GetPartResourceId() const
	{
        return m_partResourceId;
	}

	unsigned ComplexPhysicObjPartDescription::GetNumLps() const
	{
		throw std::logic_error("Not implemented");
	}

	void ComplexPhysicObjPartDescription::GetPartNames(retruxx::vector<CStr, retruxx::allocator<CStr>>& partNames) const
	{
        partNames.push_back(m_name);
        for (auto child = dynamic_cast<ComplexPhysicObjPartDescription*>(GetFirstChild()); child; child = dynamic_cast<ComplexPhysicObjPartDescription*>(child->GetNextSibling()))
        {
            child->GetPartNames(partNames);
        }
	}

	void ComplexPhysicObjPartDescription::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
	{
        m3d::SafeStrAttrib(m_name, xmlNode, "id");
        if (GetParent())
        {
            auto child = GetParent()->GetChildByName(m_name);
            if (child && this != child)
            {
                M3D_LOG_INFO("Warning: when loading PartDescription: name = " + m_name + " conflicts with another child");
            }
        }

        CStr strPartResourceId;
        m3d::SafeStrAttrib(strPartResourceId, xmlNode, "partResourceType");
        m_partResourceId = theResourceManager->GetResourceId(strPartResourceId);

        CStr lpName;
        m3d::SafeStrAttrib(lpName, xmlNode, "lpName");

        retruxx::vector<CStr> names;
        m3d::Tokenize(lpName, names, "(), ;\t");
        if (names.empty())
        {
            m_lpNames.push_back(NO_LP);
        }
        else
        {
            m_lpNames = names;
        }

        ref_ptr descNode = xmlFile->CreateNode();
        for (xmlNode->GetFirstChild(descNode, "PartDescription"); !descNode->IsEmpty(); descNode->GetNextSibling(descNode, "PartDescription"))
        {
            auto desc = (ComplexPhysicObjPartDescription*)M3D_KERNEL->New("ComplexPhysicObjPartDescription");
            AddChild(desc);
            desc->LoadFromXML(xmlFile, descNode);
        }
	}

	ComplexPhysicObjPartDescription const* ComplexPhysicObjPartDescription::GetChildByNameDeep(CStr const& childName) const
	{
        if (m_name == childName)
        {
            return this;
        }

	    auto child = dynamic_cast<ai::ComplexPhysicObjPartDescription*>(GetFirstChild());
        if (!child)
            return nullptr;

        while (true)
        {
            auto result = child->GetChildByNameDeep(childName);
            if (result)
                return result;

            child = dynamic_cast<ai::ComplexPhysicObjPartDescription*>(child->GetNextSibling());
            if (!child)
                return nullptr;
        }
        return nullptr;
	}

	CStr const& ComplexPhysicObjPartDescription::GetLpName(unsigned index) const
	{
        return this->m_lpNames[index];
	}

	m3d::Object* ComplexPhysicObjPartDescription::Clone()
	{
		throw std::logic_error("Not implemented");
	}

	ComplexPhysicObjPartDescription* ComplexPhysicObjPartDescription::GetParent() const
	{
        return dynamic_cast<ComplexPhysicObjPartDescription*>(Object::GetParent());
	}

	ComplexPhysicObjPartDescription::~ComplexPhysicObjPartDescription()
	{
        RemoveAllChildren();
	}

	m3d::Class* ComplexPhysicObjPartDescription::GetBaseClass()
	{
        return RT_CLASS_LOCAL(Object);
	}

	ComplexPhysicObjPartDescription::ComplexPhysicObjPartDescription()
	{
        this->m_partResourceId = -1;
	}

	ComplexPhysicObjPartDescription::ComplexPhysicObjPartDescription(ComplexPhysicObjPartDescription const&)
	{
		throw std::logic_error("Not implemented");
	}

	ComplexPhysicObjPrototypeInfo::MassShapes ComplexPhysicObjPrototypeInfo::GetMassShape() const
	{
        return this->m_massShape;
	}

	ComplexPhysicObjPartDescription const* ComplexPhysicObjPrototypeInfo::GetPartDescriptionByName(CStr const& partName) const
	{
        return m_partDescription->GetChildByNameDeep(partName);
	}

	Obj* ComplexPhysicObjPrototypeInfo::CreateRandomTargetObject() const
	{
		throw std::logic_error("Not implemented");
	}

	bool ComplexPhysicObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
	{
        auto result = ai::PhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            ref_ptr partsRootNode = xmlFile->CreateNode();
            xmlNode->GetFirstChild(partsRootNode, "MainPartDescription");
            if (partsRootNode->IsEmpty())
            {
                if (m_parentPrototypeName.empty())
                {
                    M3D_LOG_INFO("Warning: parts description missing for prototype '" + m_prototypeName + "'");
                }
            }
            else
            {
                m_partDescription->LoadFromXML(xmlFile, partsRootNode);
            }

            xmlNode->GetFirstChild(partsRootNode, "Parts");
            if (partsRootNode->IsEmpty())
            {
                M3D_LOG_INFO("Warning: Parts description is missing for prototype '" + m_prototypeName + "'");
            }
            else
            {
                ref_ptr partNode = xmlFile->CreateNode();
                for (partsRootNode->GetFirstChild(partNode, "Part"); !partNode->IsEmpty(); partNode->GetNextSibling(partNode, "Part"))
                {
                    CStr id;
                    m3d::SafeStrAttrib(id, partNode, "id");

                    CStr proto;
                    m3d::SafeStrAttrib(proto, partNode, "Prototype");

                    // TODO: check this
                    m_partPrototypeNames[id] = proto;
                }
            }

            m3d::SafeVectorAttrib(m_massSize, xmlNode, "MassSize");
            m3d::SafeVectorAttrib(m_massTranslation, xmlNode, "MassTranslation");
            m3d::SafeEnumAttrib(m_massShape, xmlNode, "MassShape");
        }
        return result;
	}

	void ComplexPhysicObjPrototypeInfo::GetPartNames(retruxx::vector<CStr, retruxx::allocator<CStr>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	ComplexPhysicObjPrototypeInfo::~ComplexPhysicObjPrototypeInfo()
	{
        throw std::logic_error("Not implemented");
	}

	retruxx::vector<CStr, retruxx::allocator<CStr>> const& ComplexPhysicObjPrototypeInfo::GetAllPartNames() const
	{
        return m_allPartNames;
	}

	ComplexPhysicObjPrototypeInfo::ComplexPhysicObjPrototypeInfo()
	{
        this->m_massSize.x = 1.0;
        this->m_massSize.y = 1.0;
        this->m_massSize.z = 1.0;
        this->m_massTranslation = {0.0, 0.0, 0.0};
        m_partDescription = (ComplexPhysicObjPartDescription*)M3D_KERNEL->New("ComplexPhysicObjPartDescription");
        this->m_massShape = MS_BOX;
	}

	unsigned ComplexPhysicObjPrototypeInfo::GetBasePrice() const
	{
		throw std::logic_error("Not implemented");
	}

	void ComplexPhysicObjPrototypeInfo::PostLoad()
	{
        for (auto& name : m_partPrototypeNames)
        {
            m_partPrototypeIds[name.first] = thePrototypeManager->GetPrototypeId(name.second);
        }

        m_allPartNames.clear();
        m_partDescription->GetPartNames(m_allPartNames);
	}

    void ComplexPhysicObj::UnlinkGeomsFromCollisionCells()
    {
        PhysicObj::UnlinkGeomsFromCollisionCells();
    }

    void ComplexPhysicObj::GetGeoms(retruxx::vector<Geom*, retruxx::allocator<Geom*>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::SetPassedToAnotherMapStatus()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::EnableGeometry(bool changePhysicState)
    {
        PhysicObj::EnableGeometry(changePhysicState);
        if (!this->m_spaceId || !this->m_bIsSpaceOwner)
        {
            for (auto& [name, part] : m_vehicleParts)
            {
                if (part)
                {
                    part->EnableGeometry();
                }
            }
        }
    }

    void ComplexPhysicObj::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    retruxx::vector<CStr, retruxx::allocator<CStr>> ComplexPhysicObj::GetAttachedPartNames() const
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::DumpPhysicInfo(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::SetPartByName(CStr const& partName, VehiclePart* vehiclePart, bool bUnsafe)
    {
        if (vehiclePart)
        {
            if (!bUnsafe && !ai::ComplexPhysicObj::CanPartBeAttached(partName))
            {
                M3D_LOG_ERR("Warning: attaching a physic object part that can't be attached. Object desc: " + GetDebugDescription() + ", part name = '" + partName + "'");
            }

            auto it = m_vehicleParts.find(partName);
            if (it != m_vehicleParts.end())
            {
                SetPartByName(partName, nullptr, true);
            }
            vehiclePart->SetPartName(partName);
            vehiclePart->SetOwner(this);
            m_vehicleParts[partName] = vehiclePart;
            vehiclePart->LinkToParent(GetId(), HIERARCHY_COMPONENT);
            if ((GetFlags() & 1) != 0)
                vehiclePart->SetVisible();
            else
                vehiclePart->SetInvisible();
        }
        else
        {
            auto it = m_vehicleParts.find(partName);
            if (it != m_vehicleParts.end())
            {
                auto oldPart = it->second;
                m_vehicleParts.erase(it);
                oldPart->SetParentInvalid();
                oldPart->SetInvisible();
                oldPart->SetOwner(nullptr);
            }
        }

        if (!bUnsafe)
        {
            _Construct(false);
            for (auto& part : m_vehicleParts)
            {
                if (auto* node = part.second->m_Node)
                {
                    node->UpdateXForm(false, true);
                }
            }
        }

        if (m_isContoured)
        {
            _PutContour();
        }

        M3D_APP->EnqueueMessage(66545, GetId(), 0, 0, 0, partName, {});
    }

    void ComplexPhysicObj::Remove()
    {
        PhysicObj::Remove();
        for (auto& [name, part] : m_vehicleParts)
        {
            if (part)
            {
                part->Remove();
            }
        }
    }

    ComplexPhysicObj::ComplexPhysicObj(ComplexPhysicObjPrototypeInfo const& prototypeInfo) : PhysicObj(prototypeInfo)
    {
        this->m_isContoured = 0;
        this->m_contourColor = M3D_KERNEL->GetEngineCfg().m_g_contourColor.GetC();
        this->m_contourWidth = M3D_KERNEL->GetEngineCfg().m_g_contourWidth.GetF();
        this->m_targetId = -1;
        this->m_timeoutForReAimGuns = 0.0;
        this->m_currentTargetPos = {0.0, 0.0, 0.0};
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

    void ComplexPhysicObj::SetSkin(int skin)
    {
        PhysicObj::SetSkin(skin);
        for (auto& part : m_vehicleParts)
        {
            part.second->SetSkin(skin);
        }
    }

    void ComplexPhysicObj::CreateChildren()
    {
        Obj::CreateChildren();

        auto* prototypeInfo = GetPrototypeInfo();
        for (const auto& [name, protoId] : prototypeInfo->m_partPrototypeIds)
        {
            auto objId = theObjects->CreateNewObject(protoId, {}, -1, -1);
            if (objId >= 0)
            {
                auto* vehiclePart = dynamic_cast<VehiclePart*>(theObjects->GetEntityByObjId(objId));
                if (vehiclePart)
                {
                    SetPartByName(name, vehiclePart, true);
                    continue;
                }
            }
            M3D_LOG_ERR("Error: couldn't create part for " + GetDebugDescription() + " part name = " + name + ", part prototype id = " + CStr(protoId));
            //M3D_CRITICAL_ERROR("");
        }

        _Construct(false);

        // TODO: check this
        for (const auto& [name, part] : m_vehicleParts)
        {
            part->TransferPhysicParamsToSceneGraphNode();
            if (part->m_Node)
            {
                part->m_Node->UpdateXForm(false, true);
            }

            if (part->IsKindOf(&ai::CompoundVehiclePart::m_classCompoundVehiclePart))
            {
                auto* compoundVehiclePart = dynamic_cast<CompoundVehiclePart*>(part);
                for (const auto& [vehPartName, vehPart] : *compoundVehiclePart)
                {
                    vehPart.vp->TransferPhysicParamsToSceneGraphNode();
                    if (vehPart.vp->m_Node)
                    {
                        vehPart.vp->m_Node->UpdateXForm(false, true);
                    }
                }
            }
        }
    }

    void ComplexPhysicObj::TransferPhysicParamsToSceneGraphNode()
    {
    }

    void ComplexPhysicObj::ClearSavedStatus()
    {
        throw std::logic_error("Not implemented");
    }

    VehiclePart const* ComplexPhysicObj::GetPartByName(CStr const& partName) const
    {
        if (partName.empty())
        {
            return nullptr;
        }

        auto it = m_vehicleParts.find(partName);
        if (it != m_vehicleParts.end())
        {
            return it->second;
        }
        return nullptr;
    }

    VehiclePart* ComplexPhysicObj::GetPartByName(CStr const& partName)
    {
        if (partName.empty())
        {
            return nullptr;
        }

        auto it = m_vehicleParts.find(partName);
        if (it != m_vehicleParts.end())
        {
            return it->second;
        }

        for (const auto& pair : m_vehicleParts)
        {
            VehiclePart* part = pair.second;

            // Check if this part is a compound vehicle part
            if (part->IsKindOf(&ai::CompoundVehiclePart::m_classCompoundVehiclePart))
            {
                CompoundVehiclePart* compoundPart = dynamic_cast<CompoundVehiclePart*>(part);

                // Search within the compound part
                auto compoundIt = compoundPart->find(partName);
                if (compoundIt != compoundPart->end())
                {
                    return compoundIt->second.vp;  // Return the vehicle part from the compound
                }
            }
        }

        return nullptr;
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
        PhysicObj::EnablePhysics();
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

    void ComplexPhysicObj::ReceiveNodesToLink(retruxx::list<m3d::SgNode*, retruxx::allocator<m3d::SgNode*>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::RemoveContour()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::RelinkGeomsToCollisionCells()
    {
        PhysicObj::RelinkGeomsToCollisionCells();
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
        return this->m_isContoured;
    }

    void ComplexPhysicObj::DisablePhysics()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::LinkGeomsToCollisionCells()
    {
        PhysicObj::LinkGeomsToCollisionCells();
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

    void ComplexPhysicObj::SetBelong(int newBelong)
    {
        Obj::SetBelong(newBelong);
        for (auto& part : m_vehicleParts)
        {
            part.second->SetBelong(newBelong);
        }
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
        dMass mass;
        dMassSetZero(&mass);
        dMassSetZero(&mass);

        auto massValue = _CalcMassForBody();
        if (massValue < 0.1)
        {
            massValue = 1.0;
        }

        auto* protoInfo = GetPrototypeInfo();
        auto massSize = protoInfo->m_massSize;
        if (protoInfo->GetMassShape() == ComplexPhysicObjPrototypeInfo::MS_SPHERE)
        {
            // TODO: check this
            dMassSetSphereTotal(&mass, massValue, ((massSize.x + massSize.y) + massSize.z) * 0.16666667);
        }
        else
        {
            dMassSetBoxTotal(&mass, massValue, massSize.x, massSize.y, massSize.z);
        }
        dBodySetMass(this->GetBody()->id(), &mass);
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
        PhysicObj::_LinkBodyToGeoms();
    }

    void ComplexPhysicObj::_UnlinkBodyFromGeoms()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_SetCorrectBoundSphereRadius()
    {
        Aabb aabb;
        aabb.m_box[0] = 10000.0;
        aabb.m_box[3] = -10000.0;
        aabb.m_box[1] = 10000.0;
        aabb.m_box[4] = -10000.0;
        aabb.m_box[2] = 10000.0;
        aabb.m_box[5] = -10000.0;

        for (const auto& [name, part] : m_vehicleParts)
        {
            if (part->IsKindOf(&ai::CompoundVehiclePart::m_classCompoundVehiclePart))
            {
                auto* compoundVehiclePart = dynamic_cast<CompoundVehiclePart*>(part);
                for (const auto& [vehPartName, vehPart] : *compoundVehiclePart)
                {
                    for (const auto& geom : vehPart.vp->m_pGeoms)
                    {
                        const auto geomAabb = geom->GetAabb();
                        aabb.EmbraceBox(geomAabb);
                    }
                }
            }
            else
            {
                for (const auto& geom : part->m_pGeoms)
                {
                    const auto geomAabb = geom->GetAabb();
                    aabb.EmbraceBox(geomAabb);
                }
            }
        }

        float radius = sqrt(
            (aabb.m_box[5] - aabb.m_box[2]) * (aabb.m_box[5] - aabb.m_box[2])
            + (aabb.m_box[4] - aabb.m_box[1]) * (aabb.m_box[4] - aabb.m_box[1])
            + (aabb.m_box[3] - aabb.m_box[0]) * (aabb.m_box[3] - aabb.m_box[0]))
            * 0.75;
        _SetBoundSphereRadius(radius);
    }

    void ComplexPhysicObj::_RemoveContour()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_InternalCreateVisualPart()
    {
        PhysicObj::_InternalCreateVisualPart();
        for (auto& part : m_vehicleParts)
        {
            part.second->CreateVisualPart();
        }

        SetSkin(GetSkin());
        _Construct(false);

        if (m_isContoured)
        {
            _PutContour();
        }
    }

    void ComplexPhysicObj::_SetPositionToGeoms(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_ConstructVehiclePart(CStr const& name, VehiclePart* vehiclePart, int index, bool bForAnimation)
    {
        // TODO: check all this shiit
        if (vehiclePart)
        {
            vehiclePart->SetPartName(name);

            CMatrix res;
            res.identity();

            auto* partDesc = GetPrototypeInfo()->GetPartDescriptionByName(name);
            if (partDesc)
            {
                auto parentPartDescription = partDesc->GetParent();
                if (parentPartDescription)
                {
                    VehiclePart* parent = nullptr;
                    while (parentPartDescription)
                    {
                        CMatrix parentMat;
                        parentMat.identity();

                        auto it = m_vehicleParts.find(parentPartDescription->GetName());
                        if (it == m_vehicleParts.end())
                        {
                            break;
                        }

                        auto lpName = partDesc->GetLpName(index);
                        if (lpName != NO_LP)
                        {
                            m3d::AnimatedModel* mdl = nullptr;
                            if (it->second->m_Node)
                            {
                                it->second->m_Node->GetServerItemProperty(16394, &mdl);
                            }
                            else
                            {
                                auto item = M3D_APP->GetAnimatedModelsServer().GetItemByName(it->second->m_modelname.c_str(), true);
                                if (item != -1)
                                {
                                    M3D_APP->GetAnimatedModelsServer().GetItemProperty(item, 16394, &mdl);
                                }
                            }

                            if (mdl)
                            {
                                auto loadPointIdByName = mdl->GetLoadPointIdByName(lpName.c_str());
                                if (loadPointIdByName != -1)
                                {
                                    if (it->second->m_Node)
                                    {
                                        m3d::AnimInfo* anim = nullptr;
                                        it->second->m_Node->GetProperty(1, &anim);
                                        if (!anim || anim->IsEmpty())
                                        {
                                            parentMat = mdl->GetBoneMatrix(loadPointIdByName);
                                        }
                                        else
                                        {
                                            parentMat = anim->GetCurrentLoadpointMatrix(loadPointIdByName);
                                        }
                                    }
                                    else
                                    {
                                        parentMat = mdl->GetBoneMatrix(loadPointIdByName);
                                    }
                                }
                                else
                                {
                                    M3D_LOG_ERR("Error: LoadPoint not found! Model = '" + GetDebugDescription() + "', lp = " + lpName + " for " + it->second->m_modelname);
                                }
                            }
                            else
                            {
                                M3D_LOG_ERR("Error: LoadPoint not found! Model = '" + GetDebugDescription() + "', lp = " + lpName + " for " + it->second->m_modelname);
                            }
                        }

                        if (!parent)
                        {
                            parent = it->second;
                            if (IS_KIND_OF(parent, Gun))
                            {
                                throw std::logic_error("Not implemented");
                            }
                        }
                        res *= parentMat;
                        parentPartDescription = parentPartDescription->GetParent();
                    }
                }
                else
                {
                    M3D_LOG_INFO("Warning: parent part for child does not exist in object '" + GetDebugDescription() + "'");
                }

                CVector resVector = res.getOrg();
                Quaternion quat;
                quat.FromMatrix(res);
                vehiclePart->SetNodeRelativePosition(resVector);
                if ((!IS_KIND_OF(vehiclePart, Gun) || !bForAnimation) && theObjects->m_SaveType != ObjContainer::SAVE_FULL)
                {
                    vehiclePart->SetNodeRelativeRotation(quat);
                }
                vehiclePart->RelinkToSpace(m_spaceId);
            }
        }
    }

    float ComplexPhysicObj::_CalcMassForBody() const
    {
        float mass = 0.0;
        for (const auto& part : m_vehicleParts)
        {
            mass += part.second->GetMass();
        }
        return mass;
    }

    ComplexPhysicObj::~ComplexPhysicObj()
    {
    }

    void ComplexPhysicObj::_PutContour()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_SetRotationToGeoms(Quaternion const&)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_Construct(bool bForAnimation)
    {
        const auto pos = GetPosition();
        const auto rot = GetRotation();
        _CreateSpace(false);
        for (const auto&[name, part] : m_vehicleParts)
        {
            if (part->IsKindOf(&ai::CompoundVehiclePart::m_classCompoundVehiclePart))
            {
                auto* compoundVehiclePart = dynamic_cast<CompoundVehiclePart*>(part);
                _ConstructVehiclePart(name, part, 0, bForAnimation);
                for (const auto& [vehPartName, vehPart] : *compoundVehiclePart)
                {
                    _ConstructVehiclePart(vehPartName, vehPart.vp, vehPart.index, bForAnimation);
                }
            }
            else
            {
                _ConstructVehiclePart(name, part, 0, bForAnimation);
            }
        }

        RefreshMass();

        auto* prototypeInfo = GetPrototypeInfo();
        this->m_massCenter = { 0.0, 0.0, 0.0 };
        

        _SetMassCenter(prototypeInfo->m_massTranslation);
        _SetCorrectBoundSphereRadius();
        SetPosition(pos);

        for (const auto& [name, part] : m_vehicleParts)
        {
            part->TransferPhysicParamsToSceneGraphNode();
            if (part->m_Node)
            {
                part->m_Node->UpdateXForm(false, true);
            }
        }

        if ((this->GetPhysicState() & 1) != 0)
            this->EnablePhysics();
        else
            this->DisablePhysics();
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
