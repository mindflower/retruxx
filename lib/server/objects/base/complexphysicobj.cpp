#include "complexphysicobj.h"
#include <stdexcept>
#include <ode/objects.h>

#include "core/ini.h"
#include "core/kernel.h"
#include "core/log.h"
#include <server/resourcemanager.h>

#include "config.h"
#include "globalproperties.h"
#include "m3dapp.h"
#include "prototypemanager.h"
#include "server/objects/physicbodies/vehiclepart.h"
#include "objcontainer.h"
#include "core/timer.h"
#include "ode/odecpp.h"
#include "scene/servers/dataserver.h"
#include "server/objects/vehicle.h"
#include "server/objects/guns/gun.h"
#include "server/objects/physicbodies/compoundvehiclepart.h"
#include "server/objects/guns/compoundgun.h"
#include "client.h"
#include "world.h"
#include "scene/scenegraph.h"
#include "server/objects/dummyobject.h"
#include <server/server.h>
#include "server/dynamicscene.h"

extern "C" void __cdecl _assert(char const* message, char const* file, unsigned line);

RT_CLASS_EXPORT_METHOD_DEFINE(ComplexPhysicObj, CanPartBeAttached)
{
    // RVA 0x6C1A10
    auto* const obj = static_cast<ai::ComplexPhysicObj*>(context->asObject(0, "ComplexPhysicObj"));
    CStr const partName(context->asString(1));
    context->pushBool(obj->CanPartBeAttached(partName));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ComplexPhysicObj, SetPartByName)
{
    // RVA 0x6BC8B0
    auto* const obj = static_cast<ai::ComplexPhysicObj*>(context->asObject(0, "ComplexPhysicObj"));
    CStr const partName(context->asString(1));
    auto* const part = static_cast<ai::VehiclePart*>(context->asObject(2, "VehiclePart"));
    obj->SetPartByName(partName, part, false);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ComplexPhysicObj, SetNewPart)
{
    // RVA 0x6C1A80
    auto* const obj = static_cast<ai::ComplexPhysicObj*>(context->asObject(0, "ComplexPhysicObj"));
    CStr const newPartPrototypeName(context->asString(2));
    CStr const partName(context->asString(1));
    context->pushBool(obj->SetNewPart(partName, newPartPrototypeName));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ComplexPhysicObj, TakeOffPart)
{
    // RVA 0x6C0BE0
    auto* const obj = static_cast<ai::ComplexPhysicObj*>(context->asObject(0, "ComplexPhysicObj"));
    CStr const partName(context->asString(1));
    ai::VehiclePart* const part = obj->GetPartByName(partName);
    obj->SetPartByName(partName, nullptr, false);
    context->pushObject(part);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(ComplexPhysicObj, GetPartByName)
{
    // RVA 0x6C0C60
    auto* const obj = static_cast<ai::ComplexPhysicObj*>(context->asObject(0, "ComplexPhysicObj"));
    CStr const partName(context->asString(1));
    context->pushObject(obj->GetPartByName(partName));
    return 1;
}

CStr const NO_LP("NO_LP");

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
        // RVA 0x6BC010
        return RT_CLASS_LOCAL(ComplexPhysicObjPartDescription);
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
        // RVA 0x6BCBF0
        return m_lpNames.size();
    }

    void ComplexPhysicObjPartDescription::GetPartNames(retruxx::vector<CStr, retruxx::allocator<CStr>>& partNames) const
    {
        partNames.push_back(m_name);
        for (auto child = dynamic_cast<ComplexPhysicObjPartDescription*>(GetFirstChild()); child;
             child = dynamic_cast<ComplexPhysicObjPartDescription*>(child->GetNextSibling()))
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
                M3D_LOG_INFO(
                    "Warning: when loading PartDescription: name = " + m_name + " conflicts with another child");
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
        for (xmlNode->GetFirstChild(descNode, "PartDescription"); !descNode->IsEmpty();
             descNode->GetNextSibling(descNode, "PartDescription"))
        {
            auto desc = (ComplexPhysicObjPartDescription*)M3D_KERNEL->New("ComplexPhysicObjPartDescription");
            AddChild(desc);
            desc->LoadFromXML(xmlFile, descNode);
        }
    }

    ComplexPhysicObjPartDescription const* ComplexPhysicObjPartDescription::GetChildByNameDeep(
        CStr const& childName) const
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
        // RVA 0x6C2D30
        // The shipped code allocates, zeroes m_lpNames, then asserts "0" and returns
        // null - a part description is not cloneable.
        return nullptr;
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
        // no body in the binary
        // Elided from the shipped build: Clone() asserts before it would ever be
        // reached, so the copy leaves everything default-constructed.
    }

    ComplexPhysicObjPrototypeInfo::MassShapes ComplexPhysicObjPrototypeInfo::GetMassShape() const
    {
        return this->m_massShape;
    }

    ComplexPhysicObjPartDescription const* ComplexPhysicObjPrototypeInfo::GetPartDescriptionByName(
        CStr const& partName) const
    {
        return m_partDescription->GetChildByNameDeep(partName);
    }

    Obj* ComplexPhysicObjPrototypeInfo::CreateRandomTargetObject() const
    {
        // RVA 0x6C1B20 - every slot without a fixed part gets a random prototype whose resource fits it.
        int const objId = theObjects->CreateNewObject(m_prototypeId, "", -1, -1);
        // NOTE: the new object is used without a null check.
        auto* const obj = static_cast<ComplexPhysicObj*>(theObjects->GetEntityByObjId(objId));
        int const numOfPrototypes = thePrototypeManager->GetNumOfPrototypes();

        retruxx::vector<ComplexPhysicObjPartDescription const*> stack;
        stack.push_back(m_partDescription);
        while (!stack.empty())
        {
            ComplexPhysicObjPartDescription const* const desc = stack.back();
            stack.pop_back();
            for (auto* child = static_cast<ComplexPhysicObjPartDescription const*>(desc->GetFirstChild()); child;
                 child = static_cast<ComplexPhysicObjPartDescription const*>(child->GetNextSibling()))
            {
                CStr const childName(child->GetName());
                if (m_partPrototypeIds.find(childName) == m_partPrototypeIds.end() && obj->CanPartBeAttached(childName))
                {
                    int const resId = child->GetPartResourceId();
                    retruxx::vector<PrototypeInfo const*> candidatePrototypes;
                    for (int i = 0; i < numOfPrototypes; ++i)
                    {
                        PrototypeInfo const* const info = thePrototypeManager->GetPrototypeInfo(i);
                        if (!info)
                        {
                            SYS_ERROR("info");
                        }
                        if (theResourceManager->bResourceIsKindOf(info->m_resourceId, resId))
                        {
                            candidatePrototypes.push_back(info);
                        }
                    }

                    if (candidatePrototypes.empty())
                    {
                        // NOTE: the closing quote is missing from the message.
                        M3D_LOG_ERR("Error: no candidates for resource '" + theResourceManager->GetResourceName(resId));
                    }
                    else
                    {
                        // A uniform pick, rounded to the nearest index.
                        float const zero = 0.0f;
                        float const last = static_cast<float>(static_cast<double>(candidatePrototypes.size()) - 1.0);
                        float const lo = last >= 0.0f ? zero : last;
                        float const hi = last <= 0.0f ? zero : last;
                        int const index = static_cast<int>(
                            static_cast<double>(rand()) * (hi - lo) * 0.000030518509f + lo + 0.5);
                        PrototypeInfo const* const partInfo = candidatePrototypes[index];
                        int const partId = theObjects->CreateNewObject(partInfo->m_prototypeId, "", -1, -1);
                        // NOTE: a part that could not be created is type checked through a null pointer.
                        Obj* const part = theObjects->GetEntityByObjId(partId);
                        if (static_cast<m3d::Object*>(part)->IsKindOf(RT_CLASS_LOCAL(VehiclePart)))
                        {
                            obj->SetPartByName(childName, static_cast<VehiclePart*>(part), false);
                        }
                        else
                        {
                            M3D_LOG_ERR("Error: prototype '" + partInfo->m_prototypeName + "' is not prototype of VehiclePart");
                        }
                    }
                }
                if (child->GetFirstChild())
                {
                    stack.push_back(child);
                }
            }
        }

        obj->SetRandomSkin();
        obj->PostLoad();
        return obj;
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
                for (partsRootNode->GetFirstChild(partNode, "Part"); !partNode->IsEmpty();
                     partNode->GetNextSibling(partNode, "Part"))
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

    void ComplexPhysicObjPrototypeInfo::GetPartNames(retruxx::vector<CStr, retruxx::allocator<CStr>>& partNames) const
    {
        // inlined in the binary
        // NOTE: no standalone body survives in the shipped build (inlined at every
        // call site). PostLoad fills m_allPartNames from the part tree, so hand that
        // cached list back, appending the way the part-description walker does.
        partNames.insert(partNames.end(), m_allPartNames.begin(), m_allPartNames.end());
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
        // RVA 0x6BE960
        // The base price of a complex object is the sum of its parts' base prices.
        unsigned price = 0;
        for (auto const& [partName, prototypeId] : m_partPrototypeIds)
        {
            if (auto const* partInfo = thePrototypeManager->GetPrototypeInfo(prototypeId))
            {
                price += partInfo->GetBasePrice();
            }
        }
        return price;
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

    void ComplexPhysicObj::GetGeoms(retruxx::vector<Geom*, retruxx::allocator<Geom*>>& geoms) const
    {
        // RVA 0x6BFC40
        for (auto const& [name, part] : m_vehicleParts)
        {
            part->GetGeoms(geoms);
        }
    }

    void ComplexPhysicObj::SetPassedToAnotherMapStatus()
    {
        // RVA 0x6BEB00
        PhysicObj::SetPassedToAnotherMapStatus();
        for (auto& [name, part] : m_vehicleParts)
        {
            if (part)
            {
                part->SetPassedToAnotherMapStatus();
            }
        }
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
        if (m_vehicleParts.empty() || !m_vehicleParts.begin()->second->m_Node ||
            m_vehicleParts.begin()->second->m_Node->m_frameVisible == M3D_KERNEL->GetTimer().GetCurFrame())
        {
            PhysicObj::RenderDebugInfo();
            if (!m_spaceId || !m_bIsSpaceOwner || dGeomIsEnabled(m_spaceId))
            {
                for (auto const& [name, part] : m_vehicleParts)
                {
                    if (part)
                    {
                        part->RenderDebugInfo();
                    }
                }
            }
        }
    }

    retruxx::vector<CStr, retruxx::allocator<CStr>> ComplexPhysicObj::GetAttachedPartNames() const
    {
        // RVA 0x6C2A80
        retruxx::vector<CStr> res;
        for (auto const& [name, part] : m_vehicleParts)
        {
            res.push_back(name);
        }
        return res;
    }

    void ComplexPhysicObj::DumpPhysicInfo(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x6BFDB0
        PhysicObj::DumpPhysicInfo(xmlFile, xmlNode);
        for (auto const& [name, part] : m_vehicleParts)
        {
            ref_ptr partNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Part");
            xmlNode->AddChild(partNode);
            part->DumpPhysicInfo(xmlFile, partNode);
        }
    }

    void ComplexPhysicObj::SetPartByName(CStr const& partName, VehiclePart* vehiclePart, bool bUnsafe)
    {
        if (vehiclePart)
        {
            if (!bUnsafe && !ai::ComplexPhysicObj::CanPartBeAttached(partName))
            {
                M3D_LOG_ERR(
                    "Warning: attaching a physic object part that can't be attached. Object desc: " +
                    GetDebugDescription() + ", part name = '" + partName + "'");
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
        // RVA 0x6BBFD0
        m_isContoured = true;
        _PutContour();
    }

    bool ComplexPhysicObj::IsVisible()
    {
        // RVA 0x6C09E0
        // Visible when any part's scene node was drawn this frame.
        for (auto const& [name, part] : m_vehicleParts)
        {
            if (part->m_Node && part->m_Node->m_frameVisible == M3D_KERNEL->GetTimer().GetCurFrame())
            {
                return true;
            }
        }
        return false;
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
        for (auto const& [name, protoId] : prototypeInfo->m_partPrototypeIds)
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
            M3D_LOG_ERR(
                "Error: couldn't create part for " + GetDebugDescription() + " part name = " + name +
                ", part prototype id = " + CStr(protoId));
            //M3D_CRITICAL_ERROR("");
        }

        _Construct(false);

        // TODO: check this
        for (auto const& [name, part] : m_vehicleParts)
        {
            part->TransferPhysicParamsToSceneGraphNode();
            if (part->m_Node)
            {
                part->m_Node->UpdateXForm(false, true);
            }

            if (part->IsKindOf(&ai::CompoundVehiclePart::m_classCompoundVehiclePart))
            {
                auto* compoundVehiclePart = dynamic_cast<CompoundVehiclePart*>(part);
                for (auto const& [vehPartName, vehPart] : *compoundVehiclePart)
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
        // RVA 0x6BFD30
        Obj::ClearSavedStatus();
        for (auto& [name, part] : m_vehicleParts)
        {
            if (part)
            {
                part->ClearSavedStatus();
            }
        }
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

        for (auto const& pair : m_vehicleParts)
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
        // RVA 0x6BFB40
        PhysicObj::SetVisible();
        for (auto& [name, part] : m_vehicleParts)
        {
            part->SetVisible();
        }
    }

    void ComplexPhysicObj::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x6BC920
        PhysicObj::LoadRuntimeValues(xmlFile, xmlNode);
        m3d::SafeIntAttrib(m_targetId, xmlNode, "TargetId");
        m3d::SafeFloatAttrib(m_timeoutForReAimGuns, xmlNode, "TimeOutForReAimGuns");
        m3d::SafeVectorAttrib(m_currentTargetPos, xmlNode, "TargetPos");
    }

    void ComplexPhysicObj::EnablePhysics()
    {
        PhysicObj::EnablePhysics();
    }

    VehiclePart* ComplexPhysicObj::TakeOffPart(CStr const& partName)
    {
        // RVA 0x6BF980
        auto* part = GetPartByName(partName);
        SetPartByName(partName, nullptr, false);
        return part;
    }

    void ComplexPhysicObj::AddChild(Obj* pObj)
    {
        // RVA 0x6BD8D0
        Obj::AddChild(pObj);
        if (pObj && IS_KIND_OF(pObj, VehiclePart))
        {
            // A VehiclePart must be attached with SetPartByName, never AddChild - the
            // shipped code raises an unconditional SysError here and then links anyway.
            M3D_ASSERT(!"0");
            pObj->LinkToParent(GetId(), HIERARCHY_CHILD);
        }
    }

    unsigned ComplexPhysicObj::GetPrice(IPriceCoeffProvider const* priceCoeffProvider) const
    {
        // RVA 0x6BFA30
        // NOTE: the object itself is worth nothing - only the sum of its parts.
        unsigned price = 0;
        for (auto const& [name, part] : m_vehicleParts)
        {
            price += part->GetPrice(priceCoeffProvider);
        }
        return price;
    }

    void ComplexPhysicObj::ReceiveNodesToLink(retruxx::list<m3d::SgNode*, retruxx::allocator<m3d::SgNode*>>&) const
    {
        // RVA 0x6BBEF0 - the parts link their own nodes.
    }

    void ComplexPhysicObj::RemoveContour()
    {
        // RVA 0x6BBFE0
        m_isContoured = false;
        _RemoveContour();
    }

    void ComplexPhysicObj::RelinkGeomsToCollisionCells()
    {
        PhysicObj::RelinkGeomsToCollisionCells();
    }

    ComplexPhysicObjPrototypeInfo const* ComplexPhysicObj::GetPrototypeInfo() const
    {
        // RVA 0x6BE930
        return static_cast<ComplexPhysicObjPrototypeInfo const*>(Obj::GetPrototypeInfo());
    }

    void ComplexPhysicObj::DisableGeometry(bool changePhysicState)
    {
        PhysicObj::DisableGeometry(changePhysicState);
        if (!m_spaceId || !m_bIsSpaceOwner)
        {
            for (auto& [name, part] : m_vehicleParts)
            {
                if (part)
                {
                    part->DisableGeometry();
                }
            }
        }
    }

    void ComplexPhysicObj::SetContourWidth(float contourWidth)
    {
        // RVA 0x529330
        m_contourWidth = contourWidth;
    }

    void ComplexPhysicObj::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x6BC9A0
        PhysicObj::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("TargetId", CStr(m_targetId).c_str());
        xmlNode->SetAttribute("TimeOutForReAimGuns", CStr(m_timeoutForReAimGuns).c_str());
        xmlNode->SetAttribute("TargetPos", CStr(m_currentTargetPos).c_str());
    }

    unsigned ComplexPhysicObj::size() const
    {
        // RVA 0x73E1E0
        return m_vehicleParts.size();
    }

    void ComplexPhysicObj::TransferToSpace(dxSpace* newSpace)
    {
        // RVA 0x6BF840
        // NOTE: the parts are relinked before the base moves the object itself.
        for (auto& [name, part] : m_vehicleParts)
        {
            if (part)
            {
                part->RelinkToSpace(newSpace);
            }
        }
        PhysicObj::TransferToSpace(newSpace);
    }

    bool ComplexPhysicObj::bIsContoured() const
    {
        return this->m_isContoured;
    }

    void ComplexPhysicObj::DisablePhysics()
    {
        PhysicObj::DisablePhysics();
    }

    void ComplexPhysicObj::LinkGeomsToCollisionCells()
    {
        PhysicObj::LinkGeomsToCollisionCells();
    }

    void ComplexPhysicObj::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x6C4DA0 - a prototype's default part is written only when it differs from what loading would create;
        // parts in other slots are always written.
        Obj::SaveToXML(xmlFile, xmlNode);
        ref_ptr partsNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Parts");
        xmlNode->AddChild(partsNode);

        retruxx::set<CStr> savedParts;
        for (auto const& [partName, prototypeId] : GetPrototypeInfo()->m_partPrototypeIds)
        {
            auto const part = m_vehicleParts.find(partName);
            savedParts.insert(partName);
            bool const present = part != m_vehicleParts.end();
            if (theObjects->m_SaveType == ObjContainer::SAVE_FULL || !present ||
                part->second->GetPrototypeInfo()->m_prototypeId != prototypeId || !part->second->bIsEqualToPrototype())
            {
                ref_ptr partNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, partName.c_str());
                partNode->SetAttribute("present", CStr(static_cast<int>(present)).c_str());
                if (present)
                {
                    part->second->SaveToXML(xmlFile, partNode);
                }
                partsNode->AddChild(partNode);
            }
        }

        for (auto const& [partName, part] : m_vehicleParts)
        {
            if (savedParts.find(partName) == savedParts.end())
            {
                ref_ptr partNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, partName.c_str());
                partNode->SetAttribute("present", "yes");
                part->SaveToXML(xmlFile, partNode);
                partsNode->AddChild(partNode);
            }
        }
    }

    Obj* ComplexPhysicObj::CloneObj()
    {
        // RVA 0x6BFCC0
        auto* clone = RT_DYNCAST(Obj::CloneObj(), ComplexPhysicObj);
        if (!clone)
        {
            return nullptr;
        }
        // The base clone copies the parts but not their visual halves.
        for (auto& [name, part] : clone->m_vehicleParts)
        {
            if (part)
            {
                part->PostLoad();
                part->CreateVisualPart();
            }
        }
        return clone;
    }

    void ComplexPhysicObj::SetRandomSkin()
    {
        // no-op
        // RVA 0x6C2F40: empty in the shipped build - subclasses override it.
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
        // RVA 0x6BFBC0
        PhysicObj::SetInvisible();
        for (auto& [name, part] : m_vehicleParts)
        {
            part->SetInvisible();
        }
    }

    m3d::Class* ComplexPhysicObj::GetClass() const
    {
        // RVA 0x6BBEE0
        return RT_CLASS_LOCAL(ComplexPhysicObj);
    }

    Geom::CellAabb ComplexPhysicObj::GetCollisionCellAabb() const
    {
        return PhysicObj::GetCollisionCellAabb();
    }

    void ComplexPhysicObj::SetContourColor(unsigned contourColor)
    {
        // RVA 0x529320
        m_contourColor = contourColor;
    }

    int ComplexPhysicObj::GetNumPhysicBodies() const
    {
        // RVA 0x7BB860
        return static_cast<int>(m_vehicleParts.size());
    }

    bool ComplexPhysicObj::RemoveChild(Obj* pObj)
    {
        // RVA 0x6BDA50
        Obj::RemoveChild(pObj);
        // A VehiclePart must be detached with SetPartByName, never RemoveChild - the
        // shipped code raises an unconditional SysError on this path.
        M3D_ASSERT(!(pObj && pObj->GetParentId() == GetId() && IS_KIND_OF(pObj, VehiclePart)));
        return false;
    }

    bool ComplexPhysicObj::CanPartBeAttached(CStr const& partName) const
    {
        // RVA 0x6C11E0
        // A part can hang off the object only when every ancestor in the part tree is
        // itself attached and actually offers the load points this part needs.
        auto const* protoInfo = GetPrototypeInfo();
        auto const* partDescription = protoInfo->GetPartDescriptionByName(partName);
        if (!partDescription)
        {
            return false;
        }

        auto const* parentDescription = partDescription->GetParent();
        while (parentDescription)
        {
            auto const it = m_vehicleParts.find(parentDescription->GetName());
            if (it == m_vehicleParts.end())
            {
                return false;
            }
            auto const& parentLoadPoints = it->second->GetPrototypeInfo()->m_loadPoints;
            for (unsigned i = 0; i < partDescription->GetNumLps(); ++i)
            {
                if (parentLoadPoints.find(partDescription->GetLpName(i)) == parentLoadPoints.end())
                {
                    return false;
                }
            }
            partDescription = parentDescription;
            parentDescription = parentDescription->GetParent();
        }
        return true;
    }

    int ComplexPhysicObj::GetGunHorizontalStopAngles(
        CStr const& gunPartName,
        int index,
        float& leftStopAngle,
        float& rightStopAngle) const
    {
        leftStopAngle = 0.0;
        rightStopAngle = 0.0;

        auto const* prototypeInfo = GetPrototypeInfo();
        if (prototypeInfo)
        {
            auto const* partDesc = prototypeInfo->GetPartDescriptionByName(gunPartName);
            if (!partDesc)
            {
                return 0;
            }

            auto const resourceId = theResourceManager->GetResourceId("GUN");
            if (!theResourceManager->bResourceIsKindOf(partDesc->GetPartResourceId(), resourceId))
            {
                return 0;
            }

            auto const* parent = partDesc->GetParent();
            if (!parent)
            {
                return 0;
            }

            auto const* partByName = GetPartByName(parent->GetName());
            if (!partByName)
            {
                return 0;
            }

            auto& animatedModelsServer = M3D_APP->GetAnimatedModelsServer();
            auto const item = animatedModelsServer.GetItemByName(partByName->m_modelname.c_str(), true);
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

            auto const loadPoint = model->GetLoadPointIdByName(partDesc->GetLpName(index).c_str());
            auto const& boneBounds = model->GetBoneBounds(loadPoint);
            leftStopAngle = 0.0 - boneBounds.MaxRot.y;
            rightStopAngle = 0.0 - boneBounds.MinRot.y;
            return 1;
        }
        return 0;
    }

    void ComplexPhysicObj::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x6C0CD0 - each part comes from the Parts node, or from the prototype's default for that slot.
        Obj::LoadFromXML(xmlFile, xmlNode);
        ComplexPhysicObjPrototypeInfo const* const prototypeInfo = GetPrototypeInfo();

        ref_ptr partsNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        xmlNode->GetFirstChild(partsNode, "Parts");
        for (CStr const& partName : prototypeInfo->GetAllPartNames())
        {
            ref_ptr partNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
            auto const defaultPart = prototypeInfo->m_partPrototypeIds.find(partName);
            bool present = defaultPart != prototypeInfo->m_partPrototypeIds.end();
            if (!partsNode->IsEmpty())
            {
                partsNode->GetFirstChild(partNode, partName.c_str());
                if (!partNode->IsEmpty())
                {
                    m3d::SafeBoolAttrib(present, partNode, "present");
                }
            }
            if (!present)
            {
                continue;
            }

            int objId;
            if (partNode->IsEmpty())
            {
                // A full save lists every part, so a missing one is not recreated from the prototype.
                if (theObjects->m_SaveType == ObjContainer::SAVE_FULL)
                {
                    continue;
                }
                objId = theObjects->CreateNewObjectWithSuspendedPostLoad(defaultPart->second, "", -1, -1);
            }
            else
            {
                objId = gDynamicScene->ReadNewObjectFromXml(xmlFile, partNode, {});
                if (objId == -1)
                {
                    M3D_LOG_ERR("Error: could not read object part from XML, part name = '" + partName + "'");
                }
            }
            if (objId == -1)
            {
                continue;
            }

            // NOTE: a stale id leaves a null object, which is type checked through a null pointer.
            Obj* const part = theObjects->GetEntityByObjId(objId);
            if (static_cast<m3d::Object*>(part)->IsKindOf(RT_CLASS_LOCAL(VehiclePart)))
            {
                SetPartByName(partName, static_cast<VehiclePart*>(part), true);
            }
            else
            {
                M3D_LOG_ERR("Error: the part '" + partName + "' isn't a VehiclePart");
            }
        }
        _Construct(false);
    }

    void ComplexPhysicObj::Blow(Obj* partToBlow)
    {
        // RVA 0x6C2770 - the part bursts into its blow effect and is gone.
        if ((GetFlags() & 2) != 0 || !partToBlow)
        {
            return;
        }
        if (m_isContoured)
        {
            m_isContoured = false;
            _RemoveContour();
        }
        for (auto const& [name, part] : m_vehicleParts)
        {
            if (part == partToBlow)
            {
                Quaternion const rotation = part->GetNodeAbsoluteRotation();
                CVector const position = part->GetNodeAbsolutePosition();
                PhysicBody::CreateEffectNode(part->GetBlowEffectName(), position, rotation, true, 1.0f);
                part->Remove();
                SetPartByName(part->GetPartName(), nullptr, true);
                return;
            }
        }
    }

    void ComplexPhysicObj::Flow(Obj* partToFlow, float averageSpeed)
    {
        // RVA 0x6C1380
        if ((GetFlags() & 2) != 0 || !partToFlow)
        {
            return;
        }
        auto* part = RT_DYNCAST(partToFlow, VehiclePart);
        if (!part || m_vehicleParts.find(part->GetPartName()) == m_vehicleParts.end())
        {
            return;
        }
        if (m_isContoured)
        {
            m_isContoured = false;
            _RemoveContour();
        }
        _TearOffPart(part, averageSpeed);
    }

    unsigned ComplexPhysicObj::GetRepairPrice() const
    {
        // RVA 0x6BFAB0 - the shipped build only asserts.
        _assert("!\"not implemented\"", "e:\\Builders\\ExMachina\\tmpBuildDir5084\\truxx\\Server\\Objects\\Base\\ComplexPhysicObj.cpp", 1217);
        return 0;
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

    CVector ComplexPhysicObj::GetSmoothTargetPointForObj(Obj const* target, float elapsedTime)
    {
        // TODO: generated code ComplexPhysicObj::GetSmoothTargetPointForObj
        CVector* currentTargetPosPtr = &this->m_currentTargetPos;

        if (target)
        {
            // Get target's geometric center
            CVector targetPos = ai::getPhysicObjOrPhysicBodyGeometricCenter(target);

            // Check if target is a Vehicle and handle targeting logic
            if (IS_KIND_OF(target, Vehicle))
            {
                // Update target ID and timeout
                if (target->GetId() == m_targetId)
                {
                    this->m_timeoutForReAimGuns -= elapsedTime;
                }
                else
                {
                    this->m_timeoutForReAimGuns = -1.0f;
                    this->m_targetId = target->GetId();
                }

                // Return current target if timeout hasn't expired
                if (this->m_timeoutForReAimGuns >= 0.0f)
                {
                    return this->m_currentTargetPos;
                }

                // Reset timeout and calculate new target position
                this->m_timeoutForReAimGuns = ai::theGlobProp.m_timeOutForReAimGuns;

                // Get recollection position with prediction
                ai::GlobalProperties::CoeffsForDifficultyLevel const& difficultyCoeffs =
                    ai::theGlobProp.GetCoeffsForCurrentDifficultyLevel();

                auto* vehicle = RT_DYNCAST(target, Vehicle const);
                CVector recollectionPos = vehicle->GetRecollectionPosition(difficultyCoeffs.m_enemiesShootingDelay);

                targetPos = recollectionPos;

                // Get velocities for both objects
                CVector targetVel = vehicle->GetLinearVelocity();
                CVector sourcePos = GetGeometricCenter();
                CVector sourceVel = GetLinearVelocity();

                // Calculate relative speed and distance
                float relativeSpeed = sqrtf(
                    (targetVel.x - sourceVel.x) * (targetVel.x - sourceVel.x) +
                    (targetVel.y - sourceVel.y) * (targetVel.y - sourceVel.y) +
                    (targetVel.z - sourceVel.z) * (targetVel.z - sourceVel.z));

                float distance = sqrtf(
                    (targetPos.x - sourcePos.x) * (targetPos.x - sourcePos.x) +
                    (targetPos.y - sourcePos.y) * (targetPos.y - sourcePos.y) +
                    (targetPos.z - sourcePos.z) * (targetPos.z - sourcePos.z));

                // Calculate randomY using exponential distribution - FIXED VERSION
                double exponentValue = -sqrtf(relativeSpeed * 0.1f + distance * 0.033333335f) * 1.442695040888963407;

                // This replicates: _ST6 = v11; __asm { frndint }
                double integerPart = floor(exponentValue + 0.5);  // Round to nearest integer
                double fractionalPart = exponentValue - integerPart;

                // This replicates: __FSCALE__(__F2XM1__(v11 - _ST6) + 1.0, _ST6)
                // __F2XM1__ calculates 2^x - 1 for x in [-0.5, 0.5]
                // __FSCALE__ scales by 2^integerPart
                double temp = pow(2.0, fractionalPart) - 1.0 + 1.0;  // 2^fractionalPart
                double scaledValue = ldexp(temp, (int)integerPart);  // Multiply by 2^integerPart

                float randomY = (float)(1.0 - scaledValue + 0.2);

                // Get target vehicle size
                CVector vehicleSize = vehicle->GetSize();

                // Apply random offset to Y coordinate
                float yRandomSum = 0.0f;
                for (int i = 0; i < 5; i++)
                {
                    yRandomSum += (float)rand() * 0.000030518509f;
                }
                targetPos.y += ((yRandomSum * 0.4f - 1.0f) * (float)randomY * vehicleSize.y);

                // Determine largest dimension (X or Z)
                float linSize = (vehicleSize.z <= vehicleSize.x) ? vehicleSize.x : vehicleSize.z;

                // Apply random offset to X coordinate
                float xRandomSum = 0.0f;
                for (int i = 0; i < 5; i++)
                {
                    xRandomSum += (float)rand() * 0.000030518509f;
                }
                targetPos.x += ((xRandomSum * 0.4f - 1.0f) * (float)randomY * linSize);

                // Apply random offset to Z coordinate
                float zRandomSum = 0.0f;
                for (int i = 0; i < 5; i++)
                {
                    zRandomSum += (float)rand() * 0.000030518509f;
                }
                targetPos.z += ((zRandomSum * 0.4f - 1.0f) * (float)randomY * linSize);

                // Update current target position
                this->m_currentTargetPos = targetPos;
                currentTargetPosPtr = &this->m_currentTargetPos;
            }
        }
        else
        {
            // No target - reset to zero vector
            this->m_targetId = -1;
            this->m_currentTargetPos = ZeroVector;
            currentTargetPosPtr = &this->m_currentTargetPos;
        }

        return *currentTargetPosPtr;
    }

    void ComplexPhysicObj::FlowUnattachableParts(float averageSpeed)
    {
        // RVA 0x6C1400
        // Shed every part the object can no longer carry, then rebuild the hierarchy.
        // NOTE: the shipped code advances the map iterator before calling Flow, which
        // erases the entry.
        for (auto it = m_vehicleParts.begin(); it != m_vehicleParts.end();)
        {
            auto* part = it->second;
            ++it;
            if (!CanPartBeAttached(part->GetPartName()))
            {
                Flow(part, averageSpeed);
            }
        }
        _Construct(false);
    }

    bool ComplexPhysicObj::SetNewPart(CStr const& partName, CStr const& newPartPrototypeName)
    {
        // RVA 0x6C14A0
        if (!CanPartBeAttached(partName))
        {
            return false;
        }
        int const prototypeId = thePrototypeManager->GetPrototypeId(newPartPrototypeName);
        if (prototypeId == -1)
        {
            return false;
        }
        int const objId = theObjects->CreateNewObject(prototypeId, "", -1, GetBelong());
        if (auto* oldPart = GetPartByName(partName))
        {
            oldPart->Remove();
        }
        SetPartByName(partName, static_cast<VehiclePart*>(theObjects->GetEntityByObjId(objId)), false);
        return true;
    }

    void ComplexPhysicObj::RemoveComponent(Obj* pComponent)
    {
        // RVA 0x6BCA70
        Obj::RemoveComponent(pComponent);
        if (pComponent && pComponent->GetParentId() == GetId())
        {
            if (auto* part = RT_DYNCAST(pComponent, VehiclePart))
            {
                SetPartByName(part->GetPartName(), nullptr, false);
            }
        }
    }

    void ComplexPhysicObj::_DestroyHierarchy()
    {
        // no-op
        // RVA 0x6BBFC0: empty in the shipped build.
    }

    void ComplexPhysicObj::_LinkBodyToGeoms()
    {
        PhysicObj::_LinkBodyToGeoms();
    }

    void ComplexPhysicObj::_UnlinkBodyFromGeoms()
    {
        PhysicObj::_UnlinkBodyFromGeoms();
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

        for (auto const& [name, part] : m_vehicleParts)
        {
            if (part->IsKindOf(&ai::CompoundVehiclePart::m_classCompoundVehiclePart))
            {
                auto* compoundVehiclePart = dynamic_cast<CompoundVehiclePart*>(part);
                for (auto const& [vehPartName, vehPart] : *compoundVehiclePart)
                {
                    for (auto const& geom : vehPart.vp->m_pGeoms)
                    {
                        auto const geomAabb = geom->GetAabb();
                        aabb.EmbraceBox(geomAabb);
                    }
                }
            }
            else
            {
                for (auto const& geom : part->m_pGeoms)
                {
                    auto const geomAabb = geom->GetAabb();
                    aabb.EmbraceBox(geomAabb);
                }
            }
        }

        float radius = sqrt(
                           (aabb.m_box[5] - aabb.m_box[2]) * (aabb.m_box[5] - aabb.m_box[2]) +
                           (aabb.m_box[4] - aabb.m_box[1]) * (aabb.m_box[4] - aabb.m_box[1]) +
                           (aabb.m_box[3] - aabb.m_box[0]) * (aabb.m_box[3] - aabb.m_box[0])) *
            0.75;
        _SetBoundSphereRadius(radius);
    }

    void ComplexPhysicObj::_RemoveContour()
    {
        // RVA 0x6C08A0
        auto& graph = m3d::pClient->GetWorld().GetGraph();
        for (auto const& [name, part] : m_vehicleParts)
        {
            graph.DeleteFromContourList(part->m_Node);
            if (auto* gun = RT_DYNCAST(part, Gun))
            {
                graph.DeleteFromContourList(gun->GetBarrelNode());
            }
            else if (auto* compoundGun = RT_DYNCAST(part, CompoundGun))
            {
                for (auto it = compoundGun->begin(); it != compoundGun->end(); ++it)
                {
                    auto* subGun = static_cast<Gun*>(it->second.vp);
                    graph.DeleteFromContourList(subGun->m_Node);
                    graph.DeleteFromContourList(subGun->GetBarrelNode());
                }
            }
        }
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

    void ComplexPhysicObj::_SetPositionToGeoms(CVector const& pos)
    {
        // RVA 0x6BBFA0 (thunk)
        PhysicObj::_SetPositionToGeoms(pos);
    }

    void ComplexPhysicObj::_ConstructVehiclePart(
        CStr const& name,
        VehiclePart* vehiclePart,
        int index,
        bool bForAnimation)
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
                            M3D_LOG_INFO(
                                "Warning: parent part for child does not exist in object '" + GetDebugDescription() + "'");
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
                                auto item = M3D_APP->GetAnimatedModelsServer().GetItemByName(
                                    it->second->m_modelname.c_str(), true);
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
                                    M3D_LOG_ERR(
                                        "Error: LoadPoint not found! Model = '" + GetDebugDescription() +
                                        "', lp = " + lpName + " for " + it->second->m_modelname);
                                }
                            }
                            else
                            {
                                M3D_LOG_ERR(
                                    "Error: LoadPoint not found! Model = '" + GetDebugDescription() +
                                    "', lp = " + lpName + " for " + it->second->m_modelname);
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

                                CVector const INITIAL_UP_DIRECTION_15(0.0, 1.0, 0.0);
                                gunRotation.FromAxisAngle(INITIAL_UP_DIRECTION_15, gunInitAngle);

                                CMatrix vv;
                                vv._11 =
                                    1.0 - (((gunRotation.z * gunRotation.z) + (gunRotation.y * gunRotation.y)) * 2.0);
                                vv._21 = ((gunRotation.y * gunRotation.x) - (gunRotation.z * gunRotation.w)) * 2.0;
                                vv._12 = ((gunRotation.z * gunRotation.w) + (gunRotation.y * gunRotation.x)) * 2.0;
                                vv._31 = ((gunRotation.y * gunRotation.w) + (gunRotation.z * gunRotation.x)) * 2.0;
                                vv._22 =
                                    1.0 - (((gunRotation.z * gunRotation.z) + (gunRotation.x * gunRotation.x)) * 2.0);
                                vv._33 =
                                    1.0 - (((gunRotation.y * gunRotation.y) + (gunRotation.x * gunRotation.x)) * 2.0);
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
                                gun->SetHorizontalStopAngles(
                                    leftStopAngle - gunInitAngle, rightStopAngle - gunInitAngle);
                                gun->SetInitialHorizAngle(gunInitAngle);
                            }
                        }
                        res = res * parentMat;
                        partDesc = parentPartDescription;
                        parentPartDescription = parentPartDescription->GetParent();
                    }
                }

                CVector resVector = res.getOrg();
                Quaternion quat;
                quat.FromMatrix(res);
                vehiclePart->SetNodeRelativePosition(resVector);
                if ((!IS_KIND_OF(vehiclePart, Gun) || !bForAnimation) &&
                    theObjects->m_SaveType != ObjContainer::SAVE_FULL)
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
        for (auto const& part : m_vehicleParts)
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
        // RVA 0x6C0710
        // Outline every part - and, for guns, their barrels too.
        auto& graph = m3d::pClient->GetWorld().GetGraph();
        for (auto const& [name, part] : m_vehicleParts)
        {
            graph.InsertInContourList(part->m_Node, m_contourColor, m_contourWidth);
            if (auto* gun = RT_DYNCAST(part, Gun))
            {
                graph.InsertInContourList(gun->GetBarrelNode(), m_contourColor, m_contourWidth);
            }
            else if (auto* compoundGun = RT_DYNCAST(part, CompoundGun))
            {
                for (auto it = compoundGun->begin(); it != compoundGun->end(); ++it)
                {
                    auto* subGun = static_cast<Gun*>(it->second.vp);
                    graph.InsertInContourList(subGun->m_Node, m_contourColor, m_contourWidth);
                    graph.InsertInContourList(subGun->GetBarrelNode(), m_contourColor, m_contourWidth);
                }
            }
        }
    }

    void ComplexPhysicObj::_SetRotationToGeoms(Quaternion const& rot)
    {
        // RVA 0x6BBFB0 (thunk)
        PhysicObj::_SetRotationToGeoms(rot);
    }

    void ComplexPhysicObj::_Construct(bool bForAnimation)
    {
        auto const pos = GetPosition();
        auto const rot = GetRotation();
        _CreateSpace(false);
        for (auto const& [name, part] : m_vehicleParts)
        {
            if (part->IsKindOf(&ai::CompoundVehiclePart::m_classCompoundVehiclePart))
            {
                auto* compoundVehiclePart = dynamic_cast<CompoundVehiclePart*>(part);
                _ConstructVehiclePart(name, part, 0, bForAnimation);
                for (auto const& [vehPartName, vehPart] : *compoundVehiclePart)
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
        this->m_massCenter = {0.0, 0.0, 0.0};

        _SetMassCenter(prototypeInfo->m_massTranslation);
        _SetCorrectBoundSphereRadius();
        SetPosition(pos);
        SetRotation(rot);

        for (auto const& [name, part] : m_vehicleParts)
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
        // RVA 0x6BD710
        // The shipped code raises a SysError ("Object cannot be created directly")
        // and returns null - a ComplexPhysicObj only comes from its prototype.
        return nullptr;
    }

    m3d::Object* ComplexPhysicObj::Clone()
    {
        // RVA 0x6BD550
        // The shipped code raises a SysError ("Object cannot be cloned") and returns
        // null; CloneObj() is the supported path.
        return nullptr;
    }

    void ComplexPhysicObj::_CreateSplinterFromSgNode(
        VehiclePart* vp,
        int splinterPrototypeId,
        CVector const& dir,
        float averageSpeed,
        m3d::SgNode* sgNode,
        CollisionInfo const* collisionInfo)
    {
        // RVA 0x6C01C0 - a free-flying debris object that takes over the part's scene graph node.
        int const splinterId = theObjects->CreateNewObject(splinterPrototypeId, "", -1, -1);
        // NOTE: the splinter is used without a null check.
        auto* const splinter = static_cast<DummyObject*>(theObjects->GetEntityByObjId(splinterId));
        splinter->SetBelong(GetBelong());
        splinter->SetSgNodeAndCollision(sgNode, collisionInfo);
        splinter->SetMass(vp->m_mass.mass);
        splinter->SetSkin(8);
        splinter->GetPhysicBody()->SetNodeAction(0, true);
        splinter->GetPhysicBody()->SetNodeAction(8, true);
        splinter->GetPhysicBody()->SetNodeEffectAction(2 * rand() / 0x8000 == 1 ? 8 : 9);
        splinter->SetPosition(vp->GetNodeAbsolutePosition());
        splinter->SetRotation(vp->GetNodeAbsoluteRotation());

        // Flies away from the owner's centre (straight up if the part sits on it), at 0.5 to 1.5 times the speed.
        CVector const ownerPos = GetPosition();
        CVector const partPos = vp->GetNodeAbsolutePosition();
        CVector away(partPos.x - ownerPos.x, partPos.y - ownerPos.y, partPos.z - ownerPos.z);
        float const horizontalSq = away.x * away.x + away.z * away.z;
        if (sqrt(static_cast<double>(away.y) * away.y + horizontalSq) < 0.0001)
        {
            away.y = 1.0f;
        }
        double const invLen = 1.0 / sqrt(static_cast<double>(away.y) * away.y + horizontalSq + 0.00000011920929f);
        CVector const flyDir = GetRandomDeviatedVector(
            CVector(static_cast<float>(away.x * invLen), static_cast<float>(away.y * invLen), static_cast<float>(invLen * away.z)),
            1.0f);

        float const lowSpeed = averageSpeed * 0.5f;
        float const highSpeed = averageSpeed * 1.5f;
        float const minSpeed = (std::min)(lowSpeed, highSpeed);
        float const maxSpeed = (std::max)(lowSpeed, highSpeed);
        float const speed = static_cast<float>(rand()) * (maxSpeed - minSpeed) * 0.000030518509f + minSpeed;
        splinter->SetLinearVelocity(CVector(flyDir.x * speed, speed * flyDir.y, flyDir.z * speed));

        // Tumbles about the axis perpendicular to both the flight and the owner's heading, at 2 to 6 rad/s.
        float const side = dir.x * flyDir.x + dir.y * flyDir.y + dir.z * flyDir.z < 0.0f ? -1.0f : 1.0f;
        float const hx = dir.x * side;
        float const hy = dir.y * side;
        float const hz = dir.z * side;
        float const spinX = hz * flyDir.y - hy * flyDir.z;
        float const spinY = hx * flyDir.z - hz * flyDir.x;
        float const spinZ = hy * flyDir.x - hx * flyDir.y;
        float const invSpinLen = static_cast<float>(
            1.0 / sqrt(static_cast<double>(spinZ) * spinZ + static_cast<double>(spinY) * spinY + static_cast<double>(spinX) * spinX +
                       0.00000011920929f));
        float const spinSpeed = static_cast<float>(rand()) * 0.00012207404f + 2.0f;
        splinter->SetAngularVelocity(
            CVector(invSpinLen * spinX * spinSpeed, invSpinLen * spinY * spinSpeed, invSpinLen * spinZ * spinSpeed));
        splinter->SetAutoDisabling(true, 0.1f, 0.1f, 5);
        splinter->SetDeadTimer(60000, true);
    }

    void ComplexPhysicObj::_TearOffPart(VehiclePart* vp, float averageSpeed)
    {
        // RVA 0x6C0600 - the part's model flies off as a splinter (a gun's barrel as a second one) and the part is removed.
        int const splinterPrototypeId = thePrototypeManager->GetPrototypeId(CStr("vehicleSplinter"));
        CVector const dir = GetDirection();
        // The shipped code passes the start of the collision info vector as is, null when it is empty.
        _CreateSplinterFromSgNode(
            vp,
            splinterPrototypeId,
            dir,
            averageSpeed,
            vp->m_Node,
            vp->m_collisionInfos.empty() ? nullptr : vp->m_collisionInfos.data());
        if (vp->IsKindOf(RT_CLASS_LOCAL(Gun)))
        {
            m3d::SgNode* const barrelNode = static_cast<Gun*>(vp)->GetBarrelNode();
            vp->m_Node->RemoveChild(barrelNode);
            m3d::SceneGraph& graph = pServer->GetWorld()->GetGraph();
            graph.GetRootNode()->AddChild(barrelNode);
            graph.LinkNode(barrelNode);
            _CreateSplinterFromSgNode(vp, splinterPrototypeId, dir, averageSpeed, barrelNode, nullptr);
        }
        vp->m_Node = nullptr;
        SetPartByName(vp->GetPartName(), nullptr, true);
        vp->Remove();
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
