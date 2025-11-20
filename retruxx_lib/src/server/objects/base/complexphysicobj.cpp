#include "complexphysicobj.h"
#include <stdexcept>
#include <ode/objects.h>

#include "core/ini.h"
#include "core/kernel.h"
#include "core/log.h"
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
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ComplexPhysicObj, SetPartByName)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ComplexPhysicObj, SetNewPart)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ComplexPhysicObj, TakeOffPart)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ComplexPhysicObj, GetPartByName)
{
    RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
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
		RETRUXX_NOT_IMPLEMENTED;
	}

    ComplexPhysicObjPrototypeInfo::~ComplexPhysicObjPrototypeInfo() = default;

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
		RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ComplexPhysicObj::SetPassedToAnotherMapStatus()
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    retruxx::vector<CStr, retruxx::allocator<CStr>> ComplexPhysicObj::GetAttachedPartNames() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ComplexPhysicObj::DumpPhysicInfo(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool ComplexPhysicObj::IsVisible()
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ComplexPhysicObj::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ComplexPhysicObj::EnablePhysics()
    {
        PhysicObj::EnablePhysics();
    }

    VehiclePart* ComplexPhysicObj::TakeOffPart(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ComplexPhysicObj::AddChild(Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned ComplexPhysicObj::GetPrice(IPriceCoeffProvider const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ComplexPhysicObj::ReceiveNodesToLink(retruxx::list<m3d::SgNode*, retruxx::allocator<m3d::SgNode*>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ComplexPhysicObj::RemoveContour()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ComplexPhysicObj::RelinkGeomsToCollisionCells()
    {
        PhysicObj::RelinkGeomsToCollisionCells();
    }

    ComplexPhysicObjPrototypeInfo const* ComplexPhysicObj::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ComplexPhysicObj::DisableGeometry(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ComplexPhysicObj::SetContourWidth(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ComplexPhysicObj::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned ComplexPhysicObj::size() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ComplexPhysicObj::TransferToSpace(dxSpace*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool ComplexPhysicObj::bIsContoured() const
    {
        return this->m_isContoured;
    }

    void ComplexPhysicObj::DisablePhysics()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ComplexPhysicObj::LinkGeomsToCollisionCells()
    {
        PhysicObj::LinkGeomsToCollisionCells();
    }

    void ComplexPhysicObj::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obj* ComplexPhysicObj::CloneObj()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ComplexPhysicObj::SetRandomSkin()
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* ComplexPhysicObj::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Geom::CellAabb ComplexPhysicObj::GetCollisionCellAabb() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ComplexPhysicObj::SetContourColor(unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int ComplexPhysicObj::GetNumPhysicBodies() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool ComplexPhysicObj::RemoveChild(Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool ComplexPhysicObj::CanPartBeAttached(CStr const&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int ComplexPhysicObj::GetGunHorizontalStopAngles(const CStr& gunPartName, int index, float& leftStopAngle, float& rightStopAngle) const
    {
        leftStopAngle = 0.0;
        rightStopAngle = 0.0;

        const auto* prototypeInfo = GetPrototypeInfo();
        if (prototypeInfo)
        {
            const auto* partDesc = prototypeInfo->GetPartDescriptionByName(gunPartName);
            if (!partDesc)
            {
                return 0;
            }

            const auto resourceId = theResourceManager->GetResourceId("GUN");
            if (!theResourceManager->bResourceIsKindOf(partDesc->GetPartResourceId(), resourceId))
            {
                return 0;
            }

            const auto* parent = partDesc->GetParent();
            if (!parent)
            {
                return 0;
            }

            const auto* partByName = GetPartByName(parent->GetName());
            if (!partByName)
            {
                return 0;
            }

            auto& animatedModelsServer = M3D_APP->GetAnimatedModelsServer();
            const auto item = animatedModelsServer.GetItemByName(partByName->m_modelname.c_str(), true);
            if (item == -1)
            {
                return 0;
            }

            m3d::AnimatedModel* model = nullptr;
            animatedModelsServer.GetItemProperty(item, m3d::PROP_INTERNAL_GETMODEL, &model);
            if (!model)
            {
                return 0;
            }

            const auto loadPoint = model->GetLoadPointIdByName(partDesc->GetLpName(index).c_str());
            const auto& boneBounds = model->GetBoneBounds(loadPoint);
            leftStopAngle = 0.0 - boneBounds.MaxRot.y;
            rightStopAngle = 0.0 - boneBounds.MinRot.y;
            return 1;
        }
        return 0;
    }

    void ComplexPhysicObj::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // TODO: generated code
        ai::Obj::LoadFromXML(xmlFile, xmlNode);

        // TODO: implement ComplexPhysicObj::LoadFromXML
        const auto prototypeInfo = GetPrototypeInfo();

        ref_ptr partsNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(partsNode, "Parts");
        for (auto& partName : prototypeInfo->GetAllPartNames())
        {

            // Create temporary node for this part
            ref_ptr<m3d::cmn::XmlNode> partNode(xmlFile->CreateNode());

            // Find the prototype ID for this part
            auto partPrototypeIt = prototypeInfo->m_partPrototypeIds.find(partName);
            bool partPresent = (partPrototypeIt != prototypeInfo->m_partPrototypeIds.end());

            // Check if part is present in XML
            if (!partsNode->IsEmpty())
            {
                partsNode->GetFirstChild(partNode, partName.c_str());
                if (!partNode->IsEmpty())
                {
                    m3d::SafeBoolAttrib(partPresent, partNode, "present");
                }
            }

            // If part doesn't exist in prototype or isn't present in XML, skip
            if (!partPresent)
            {
                continue;
            }

            int objectId = -1;

            if (partNode->IsEmpty())
            {
                // Create new object if we're doing a full save
                if (ai::theObjects->m_SaveType == ObjContainer::SAVE_FULL)
                {
                    continue;
                }

                objectId = theObjects->CreateNewObjectWithSuspendedPostLoad(
                    partPrototypeIt->second,
                    {},
                    -1,
                    -1);
            }
            else
            {
                // Read object from XML
                objectId = gDynamicScene->ReadNewObjectFromXml(
                    xmlFile,
                    partNode,
                    {});

            }

            if (objectId == -1)
            {
                M3D_LOG_ERR("Error: could not read object part from XML, part name = '" + partName + "'");
                continue;
            }

            // Get the object from the container
            m3d::Object* partObject = theObjects->GetEntityByObjId(objectId);


            // Verify it's a VehiclePart and set it
            if (IS_KIND_OF(partObject, VehiclePart))
            {
                SetPartByName(partName, RT_DYNCAST(partObject, VehiclePart), true);
            }
            else
            {
                CStr errorMsg = "Error: the part '" + partName + "' isn't a VehiclePart";
                M3D_LOG_ERR("Error: could not read object part from XML, part name = '" + partName + "'");
            }
        }

        // Final construction
        _Construct(nullptr);
    }

    void ComplexPhysicObj::Blow(Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ComplexPhysicObj::Flow(Obj*, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned ComplexPhysicObj::GetRepairPrice() const
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    CVector ComplexPhysicObj::GetSmoothTargetPointForObj(Obj const*, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ComplexPhysicObj::FlowUnattachableParts(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool ComplexPhysicObj::SetNewPart(CStr const&, CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ComplexPhysicObj::RemoveComponent(Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ComplexPhysicObj::_DestroyHierarchy()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ComplexPhysicObj::_LinkBodyToGeoms()
    {
        PhysicObj::_LinkBodyToGeoms();
    }

    void ComplexPhysicObj::_UnlinkBodyFromGeoms()
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
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

                        index = 0;
                        if (!parent)
                        {
                            parent = it->second;
                            if (IS_KIND_OF(vehiclePart, Gun))
                            {
                                // TODO: check this
                                float leftStopAngle = 0.0;
                                float rightStopAngle = 0.0;
                                GetGunHorizontalStopAngles(name, 0, leftStopAngle, rightStopAngle);

                                CVector org = parentMat.getOrg();
                                Quaternion gunRotation;
                                gunRotation.FromMatrix(parentMat);

                                auto gunInitAngle = (rightStopAngle + leftStopAngle) * 0.5;

                                const CVector INITIAL_UP_DIRECTION_15(0.0, 1.0, 0.0);
                                gunRotation.FromAxisAngle(INITIAL_UP_DIRECTION_15, gunInitAngle);

                                CMatrix vv;
                                vv._11 = 1.0 - (((gunRotation.z * gunRotation.z) + (gunRotation.y * gunRotation.y)) * 2.0);
                                vv._21 = ((gunRotation.y * gunRotation.x) - (gunRotation.z * gunRotation.w)) * 2.0;
                                vv._12 = ((gunRotation.z * gunRotation.w) + (gunRotation.y * gunRotation.x)) * 2.0;
                                vv._31 = ((gunRotation.y * gunRotation.w) + (gunRotation.z * gunRotation.x)) * 2.0;
                                vv._22 = 1.0 - (((gunRotation.z * gunRotation.z) + (gunRotation.x * gunRotation.x)) * 2.0);
                                vv._33 = 1.0 - (((gunRotation.y * gunRotation.y) + (gunRotation.x * gunRotation.x)) * 2.0);
                                vv._32 = ((gunRotation.z * gunRotation.y) - (gunRotation.x * gunRotation.w)) * 2.0;
                                vv._13 = ((gunRotation.z * gunRotation.x) - (gunRotation.y * gunRotation.w)) * 2.0;
                                vv._23 = ((gunRotation.x * gunRotation.w) + (gunRotation.z * gunRotation.y)) * 2.0;
                                vv._14 = 0.0;
                                vv._24 = 0.0;
                                memset(&vv.m[2][3], 0, 16);
                                vv._44 = 1.0;

                                parentMat = vv;
                                parentMat.setOrg(org);

                                auto* gun = RT_DYNCAST(vehiclePart, Gun);
                                gun->SetHorizontalStopAngles(leftStopAngle - gunInitAngle, rightStopAngle - gunInitAngle);
                                gun->SetInitialHorizAngle(gunInitAngle);
                            }
                        }
                        res = res * parentMat;
                        partDesc = parentPartDescription;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ComplexPhysicObj::_SetRotationToGeoms(Quaternion const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        SetRotation(rot);

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
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* ComplexPhysicObj::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ComplexPhysicObj::_CreateSplinterFromSgNode(VehiclePart*, int, CVector const&, float, m3d::SgNode*, CollisionInfo const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ComplexPhysicObj::_TearOffPart(VehiclePart*, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    std::map<CStr, VehiclePart*>::const_iterator ComplexPhysicObj::begin() const
    {
        return m_vehicleParts.begin();
    }

    std::map<CStr, VehiclePart*>::iterator ComplexPhysicObj::begin()
    {
        return m_vehicleParts.begin();
    }

    std::map<CStr, VehiclePart*>::const_iterator ComplexPhysicObj::end() const
    {
        return m_vehicleParts.end();
    }

    std::map<CStr, VehiclePart*>::iterator ComplexPhysicObj::end()
    {
        return m_vehicleParts.end();
    }
}  // namespace ai
