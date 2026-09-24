#include "compoundvehiclepart.h"

#include <stdexcept>

#include "core/kernel.h"
#include "core/log.h"
#include "m3dapp.h"
#include "scene/nodes/sgnode.h"
#include "server/dynamicscene.h"
#include "server/objects/ware.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/base/prototypemanager.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(CompoundVehiclePart)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(CompoundVehiclePart);

    CompoundVehiclePartPrototypeInfo::CompoundVehiclePartPrototypeInfo() = default;

    void CompoundVehiclePartPrototypeInfo::PostLoad()
    {
        // RVA 0x6F56F0 - resolves each part's prototype name. NOTE: VehiclePartPrototypeInfo::PostLoad
        // is not called.
        for (auto& [id, partInfo] : m_PartInfo)
        {
            partInfo.prototypeId = thePrototypeManager->GetPrototypeId(partInfo.prototypeName);
        }
    }

    bool CompoundVehiclePartPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x6F6C60 - the parts are <Part id="..." Prototype="..."/> children, indexed in file order.
        bool const result = ai::VehiclePartPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (!result)
        {
            return result;
        }

        m_PartInfo.clear();
        if (xmlNode->IsEmpty())
        {
            M3D_LOG_INFO(CStr("Warning: Parts description is missing for prototype '") + m_prototypeName + CStr("'"));
            return true;
        }

        unsigned index = 0;
        ref_ptr partNode = xmlFile->CreateNode();
        for (xmlNode->GetFirstChild(partNode, "Part"); !partNode->IsEmpty(); partNode->GetNextSibling(partNode, "Part"))
        {
            CStr id;
            m3d::SafeStrAttrib(id, partNode, "id");
            CStr prototypeName;
            m3d::SafeStrAttrib(prototypeName, partNode, "Prototype");

            // NOTE: the shipped code also copies the prototype id of a freshly constructed
            // TPartInfo, which is uninitialized; PostLoad fills the real id in later.
            TPartInfo& partInfo = m_PartInfo[id];
            partInfo.prototypeName = prototypeName;
            partInfo.index = index++;
        }
        return true;
    }

    CompoundVehiclePart::TVehiclePart::TVehiclePart(ai::VehiclePart* _vp, unsigned _index) : vp(_vp), index(_index)
    {
        // RVA 0x6F4360
    }

    CompoundVehiclePart::~CompoundVehiclePart()
    {
        // RVA 0x6F6F90 - the parts are detached from this owner but not destroyed.
        for (auto& [name, part] : m_vehicleParts)
        {
            if (part.vp)
            {
                part.vp->SetOwner(nullptr);
            }
        }
        m_vehicleParts.clear();
    }

    CompoundVehiclePart::CompoundVehiclePart(CompoundVehiclePartPrototypeInfo const& prototype) : VehiclePart(prototype)
    {
        // RVA 0x6F7D60
    }

    m3d::Object* CompoundVehiclePart::Clone()
    {
        // RVA 0x6F49A0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* __fastcall CompoundVehiclePart::CreateObject()
    {
        // RVA 0x6F4B60
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Class* __fastcall CompoundVehiclePart::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehiclePart);
    }

    m3d::Class* CompoundVehiclePart::GetClass() const
    {
        // RVA 0x6F4390
        return RT_CLASS_LOCAL(CompoundVehiclePart);
    }

    ai::CompoundVehiclePartPrototypeInfo const* CompoundVehiclePart::GetPrototypeInfo() const
    {
        // RVA 0x6F56C0 - NOTE: the prototype is cast without a type check.
        return static_cast<CompoundVehiclePartPrototypeInfo const*>(
            thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    CompoundVehiclePart::TVehiclePart::TVehiclePart()
    {
        // NOTE: declared in the PDB but never instantiated in the shipped binary; the fields are left
        // uninitialized, as the compiler-generated default would.
    }

    void CompoundVehiclePart::SetVisible()
    {
        // RVA 0x6F58F0
        PhysicBody::SetVisible();
        for (auto& [name, part] : m_vehicleParts)
        {
            part.vp->SetVisible();
        }
    }

    void CompoundVehiclePart::SetInvisible()
    {
        // RVA 0x6F5970
        PhysicBody::SetInvisible();
        for (auto& [name, part] : m_vehicleParts)
        {
            part.vp->SetInvisible();
        }
    }

    void CompoundVehiclePart::SetBelong(int newBelong)
    {
        // RVA 0x6F5770
        PhysicBody::SetBelong(newBelong);
        for (auto& [name, part] : m_vehicleParts)
        {
            part.vp->SetBelong(newBelong);
        }
    }

    void CompoundVehiclePart::Remove()
    {
        // RVA 0x6F57F0
        VehiclePart::Remove();
        for (auto& [name, part] : m_vehicleParts)
        {
            if (part.vp)
            {
                part.vp->Remove();
            }
        }
    }

    void CompoundVehiclePart::SetPassedToAnotherMapStatus()
    {
        // RVA 0x6F5870
        VehiclePart::SetPassedToAnotherMapStatus();
        for (auto& [name, part] : m_vehicleParts)
        {
            if (part.vp)
            {
                part.vp->SetPassedToAnotherMapStatus();
            }
        }
    }

    void CompoundVehiclePart::CreateChildren()
    {
        // RVA 0x6F7070 - creates one child object per part listed in the prototype.
        Obj::CreateChildren();

        CompoundVehiclePartPrototypeInfo const* prototypeInfo = GetPrototypeInfo();
        for (auto const& [name, partInfo] : prototypeInfo->m_PartInfo)
        {
            int const partId = theObjects->CreateNewObject(partInfo.prototypeId, name.c_str(), GetId(), -1);
            VehiclePart* vp = static_cast<VehiclePart*>(theObjects->GetEntityByObjId(partId));
            if (vp)
            {
                vp->SetOwnerCompoundVehiclePart(this);
                m_vehicleParts.insert({name, TVehiclePart(vp, partInfo.index)});
                M3D_APP->EnqueueMessage(66545, GetId(), 0, 0, 0, name, m3d::AIParam());
            }
            else
            {
                PrototypeInfo const* partPrototype = thePrototypeManager->GetPrototypeInfo(partInfo.prototypeId);
                CStr const prototypeNameSuffix = partPrototype ?
                    CStr(", part prototype name = '") + partPrototype->m_prototypeName + CStr("'") :
                    CStr(", part prototype is NULL");
                M3D_LOG_ERR(
                    CStr("Error: couldn't create part for ") + GetDebugDescription() + CStr(" part name = ") + name +
                    CStr(", part prototype id = ") + CStr(partInfo.prototypeId) + prototypeNameSuffix);
                SYS_ERROR("0");
            }
        }

        for (auto& [name, part] : m_vehicleParts)
        {
            part.vp->TransferPhysicParamsToSceneGraphNode();
            if (part.vp->m_Node)
            {
                part.vp->m_Node->UpdateXForm(false, true);
            }
        }
    }

    void CompoundVehiclePart::SetOwner(ai::PhysicObj* owner)
    {
        // RVA 0x6F59F0 - the parts follow only when the owner actually changes.
        PhysicObj* const oldOwner = GetOwner();
        PhysicBody::SetOwner(owner);
        if (owner != oldOwner)
        {
            for (auto& [name, part] : m_vehicleParts)
            {
                part.vp->SetOwner(owner);
            }
        }
    }

    void CompoundVehiclePart::RelinkToSpace(dxSpace* newSpace)
    {
        // RVA 0x6F5A50
        PhysicBody::RelinkToSpace(newSpace);
        for (auto& [name, part] : m_vehicleParts)
        {
            part.vp->RelinkToSpace(newSpace);
        }
    }

    float CompoundVehiclePart::GetMass() const
    {
        // RVA 0x6F5060 - NOTE: only the parts count; the compound's own body mass is not added.
        float res = 0.0f;
        for (auto const& [name, part] : m_vehicleParts)
        {
            res = part.vp->GetMass() + res;
        }
        return res;
    }

    float CompoundVehiclePart::GetDurability() const
    {
        // RVA 0x6F50F0
        float res = 0.0f;
        for (auto const& [name, part] : m_vehicleParts)
        {
            res = part.vp->Durability().value().get() + res;
        }
        return res;
    }

    float CompoundVehiclePart::GetMaxDurability() const
    {
        // RVA 0x6F5260
        float res = 0.0f;
        for (auto const& [name, part] : m_vehicleParts)
        {
            res = part.vp->Durability().maxValue().get() + res;
        }
        return res;
    }

    void CompoundVehiclePart::SetDurability(float durability)
    {
        // RVA 0x6F5170 - the total is shared out equally between the parts.
        float const partDurability = static_cast<float>(durability / static_cast<double>(m_vehicleParts.size()));
        for (auto& [name, part] : m_vehicleParts)
        {
            part.vp->Durability().value().set(partDurability);
        }
    }

    void CompoundVehiclePart::RegenerateDurability(float elapsedTime)
    {
        // RVA 0x6F5B40
        for (auto& [name, part] : m_vehicleParts)
        {
            if (part.vp)
            {
                part.vp->Durability().regenerate(elapsedTime);
            }
        }
    }

    void CompoundVehiclePart::SetDurabilityRegeneration(float value)
    {
        // RVA 0x6F5C30
        for (auto& [name, part] : m_vehicleParts)
        {
            if (part.vp)
            {
                part.vp->Durability().regeneration().set(value);
            }
        }
    }

    unsigned int CompoundVehiclePart::GetPrice(ai::IPriceCoeffProvider const* priceCoeffProvider) const
    {
        // RVA 0x6F62A0 - like VehiclePart::GetPrice, with the wear taken from the parts' totals.
        float const maxDurability = GetMaxDurability();
        NumericInRange<float> const durability(GetDurability(), 0.0f, maxDurability);
        float const price = m_price.get();
        float const priceCoeff = GetPriceCoeff(priceCoeffProvider);
        return GetIntPrice(GetDurabilityPriceCoeff(durability) * priceCoeff * price);
    }

    void CompoundVehiclePart::_InternalCreateVisualPart()
    {
        // RVA 0x6F53D0
        VehiclePart::_InternalCreateVisualPart();
        for (auto& [name, part] : m_vehicleParts)
        {
            if (part.vp)
            {
                part.vp->CreateVisualPart();
            }
        }
    }

    void CompoundVehiclePart::_InternalPostLoad()
    {
        // RVA 0x6F5AD0 - NOTE: VehiclePart::_InternalPostLoad is not called; only the parts are
        // post-loaded.
        for (auto& [name, part] : m_vehicleParts)
        {
            if (part.vp)
            {
                part.vp->PostLoad();
            }
        }
    }

    // NOTE: begin/end/find are declared in the PDB but never instantiated in the shipped binary;
    // they are the obvious wrappers over m_vehicleParts.
    CompoundVehiclePart::VehiclePartsMap::const_iterator CompoundVehiclePart::begin() const
    {
        return m_vehicleParts.begin();
    }

    CompoundVehiclePart::VehiclePartsMap::iterator CompoundVehiclePart::begin()
    {
        return m_vehicleParts.begin();
    }

    CompoundVehiclePart::VehiclePartsMap::const_iterator CompoundVehiclePart::end() const
    {
        return m_vehicleParts.end();
    }

    CompoundVehiclePart::VehiclePartsMap::iterator CompoundVehiclePart::end()
    {
        return m_vehicleParts.end();
    }

    CompoundVehiclePart::VehiclePartsMap::const_iterator CompoundVehiclePart::find(CStr const& s) const
    {
        return m_vehicleParts.find(s);
    }

    CompoundVehiclePart::VehiclePartsMap::iterator CompoundVehiclePart::find(CStr const& s)
    {
        return m_vehicleParts.find(s);
    }

    void CompoundVehiclePart::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x6F7DB0 - the mirror of SaveToXML. A part with a saved node is read from it; a part
        // with no node (or no <Parts> at all) is recreated from its prototype, unless this is a full
        // save, where a missing node means the part was not saved; present="no" skips the part.
        Obj::LoadFromXML(xmlFile, xmlNode);

        CompoundVehiclePartPrototypeInfo::StrPartInfoMap const partInfo = GetPrototypeInfo()->m_PartInfo;
        ref_ptr partsNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(partsNode, "Parts");

        // NOTE: filled in but never read.
        retruxx::set<CStr> loadedParts;
        for (auto const& [name, info] : partInfo)
        {
            ref_ptr partNode = xmlFile->CreateNode();
            loadedParts.insert(name);

            bool present = true;
            if (!partsNode->IsEmpty())
            {
                partsNode->GetFirstChild(partNode, name.c_str());
                if (!partNode->IsEmpty())
                {
                    m3d::SafeBoolAttrib(present, partNode, "present");
                    if (!present)
                    {
                        continue;
                    }
                }
            }

            int partId;
            if (partNode->IsEmpty())
            {
                if (theObjects->m_SaveType == ObjContainer::SAVE_FULL)
                {
                    continue;
                }
                partId = theObjects->CreateNewObjectWithSuspendedPostLoad(info.prototypeId, "", GetId(), 0);
            }
            else
            {
                partId = gDynamicScene->ReadNewObjectFromXml(xmlFile, partNode, retruxx::vector<m3d::Class*>());
                if (partId == -1)
                {
                    M3D_LOG_ERR(CStr("Error: could not read object part from XML, part name = '") + name + CStr("'"));
                }
            }
            if (partId == -1)
            {
                continue;
            }

            // NOTE: IsKindOf is called without a null check, as in the shipped code.
            VehiclePart* vp = static_cast<VehiclePart*>(theObjects->GetEntityByObjId(partId));
            if (vp->IsKindOf(RT_CLASS_LOCAL(VehiclePart)))
            {
                AddChild(vp);
                vp->SetOwnerCompoundVehiclePart(this);
                m_vehicleParts.insert({name, TVehiclePart(vp, info.index)});
            }
            else
            {
                M3D_LOG_ERR(CStr("Error: the part '") + name + CStr("' isn't a VehiclePart"));
            }
        }
    }

    void CompoundVehiclePart::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x6F83E0 - a part that still matches its prototype is left out (unless this is a full
        // save); a missing one is written as present="no". Parts not listed in the prototype are
        // written after the listed ones.
        Obj::SaveToXML(xmlFile, xmlNode);

        ref_ptr partsNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Parts");
        xmlNode->AddChild(partsNode);

        retruxx::set<CStr> savedParts;
        CompoundVehiclePartPrototypeInfo const* prototypeInfo = GetPrototypeInfo();
        for (auto const& [name, info] : prototypeInfo->m_PartInfo)
        {
            auto const it = m_vehicleParts.find(name);
            savedParts.insert(name);

            bool const present = it != m_vehicleParts.end();
            if (theObjects->m_SaveType == ObjContainer::SAVE_FULL || !present ||
                it->second.vp->GetPrototypeInfo()->m_prototypeId != info.prototypeId ||
                !it->second.vp->bIsEqualToPrototype())
            {
                ref_ptr partNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, name.c_str());
                partNode->SetAttribute("present", present ? "yes" : "no");
                if (present)
                {
                    it->second.vp->SaveToXML(xmlFile, partNode);
                    partNode->SetAttribute("CompoundVehiclePartIndex", CStr(info.index).c_str());
                }
                partsNode->AddChild(partNode);
            }
        }

        for (auto const& [name, part] : m_vehicleParts)
        {
            if (savedParts.find(name) == savedParts.end())
            {
                ref_ptr partNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, name.c_str());
                partNode->SetAttribute("present", "yes");
                part.vp->SaveToXML(xmlFile, partNode);
                partsNode->AddChild(partNode);
            }
        }
    }

    void CompoundVehiclePart::AddChild(ai::Obj* pObj)
    {
        // RVA 0x6F4D20 - only records the hierarchy link; the parts map is managed separately.
        pObj->LinkToParent(GetId(), HIERARCHY_CHILD);
    }

    bool CompoundVehiclePart::RemoveChild(ai::Obj*)
    {
        // RVA 0x6F43A0
        return false;
    }

    bool CompoundVehiclePart::CanChildBeAdded(m3d::Class*) const
    {
        // RVA 0x6F43B0
        return false;
    }

    void CompoundVehiclePart::RenderDebugInfo() const
    {
        // RVA 0x6F52E0 - NOTE: the compound's own debug info is not rendered.
        for (auto const& [name, part] : m_vehicleParts)
        {
            part.vp->RenderDebugInfo();
        }
    }

    void CompoundVehiclePart::ClearSavedStatus()
    {
        // RVA 0x6F5350
        Obj::ClearSavedStatus();
        for (auto& [name, part] : m_vehicleParts)
        {
            if (part.vp)
            {
                part.vp->ClearSavedStatus();
            }
        }
    }
}  // namespace ai
