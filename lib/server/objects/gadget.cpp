#include "gadget.h"
#include "base/prototypemanager.h"
#include "guns/compoundgun.h"
#include "physicbodies/vehiclepart.h"
#include "vehicle.h"
#include <server/resourcemanager.h>

#include "core/kernel.h"
#include <core/log.h>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Gadget)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Gadget);

    GadgetPrototypeInfo::GadgetApplicationInfo::GadgetApplicationInfo()
    {
        applierType = GA_VEHICLE;
        targetResourceId = -1;
        targetFiringType = FT_MACHINE_GUN;
    }

    bool GadgetPrototypeInfo::ModificationInfo::ApplyToObj(ai::Obj* pObj, bool enable) const
    {
        // RVA 0x6DC3B0 - puts one modification onto an object, or takes it off again when enable is false.
        // Only float properties can be modified. A few maxima have a matching current value (health 20/19,
        // fuel 27/26) which is moved along with the maximum so that a fully repaired part stays full.
        if (!pObj)
        {
            return false;
        }

        int const propId = pObj->GetPropertyId(m_propertyName.c_str());
        if (propId == -1)
        {
            M3D_LOG_ERR(
                "Error: attempt to apply gadget for property '" + m_propertyName + CStr("' of ") +
                pObj->GetDebugDescription());
            return false;
        }

        if (m_modificationType != MULTIPLY && m_modificationType != ADD)
        {
            SYS_ERROR("0");
            return true;
        }

        m3d::AIParam const propDefault = pObj->GetPropertyDefaultById(propId);
        if (propDefault.GetType() != m3d::AIPARAM_FLOAT)
        {
            M3D_LOG_ERR(
                "Error: attempt to apply gadget to non-float property '" + m_propertyName + CStr("' of ") +
                pObj->GetDebugDescription());
            return false;
        }

        int proportionalPropId = -1;
        if (propId == 20)
        {
            proportionalPropId = 19;
        }
        else if (propId == 27)
        {
            proportionalPropId = 26;
        }

        float newValue = 0.0f;
        if (m_modificationType == MULTIPLY)
        {
            float const oldPropValue = pObj->GetPropertyById(propId).GetAsFloat();

            float factor = m_value.GetAsFloat();
            if (!enable)
            {
                factor = -factor;
            }
            newValue = propDefault.GetAsFloat() * factor + pObj->GetPropertyById(propId).GetAsFloat();

            // The current value keeps the same fraction of the maximum it had before.
            if (proportionalPropId != -1 && fabs(oldPropValue) > 0.0000099999997)
            {
                float const proportional =
                    newValue * (pObj->GetPropertyById(proportionalPropId).GetAsFloat() / oldPropValue);
                pObj->SetPropertyById(proportionalPropId, m3d::AIParam(proportional));
            }
        }
        else
        {
            float adder = m_value.GetAsFloat();
            if (!enable)
            {
                adder = -adder;
            }

            // The current value moves by the same amount the maximum does.
            if (proportionalPropId != -1)
            {
                float const proportional = pObj->GetPropertyById(proportionalPropId).GetAsFloat() + adder;
                pObj->SetPropertyById(proportionalPropId, m3d::AIParam(proportional));
            }
            newValue = pObj->GetPropertyById(propId).GetAsFloat() + adder;
        }

        pObj->SetPropertyById(propId, m3d::AIParam(newValue));
        return true;
    }

    GadgetPrototypeInfo::ModificationInfo::ModificationInfo(CStr const& str, ai::GadgetPrototypeInfo const* gadgetPrototype)
    {
        // RVA 0x6DBCA0
        static CStr const PLUS("+=");

        retruxx::vector<CStr> tokens;
        m3d::Tokenize(str, tokens, "(), ;\t");
        M3D_ASSERT(tokens.size() == 3 || tokens.size() == 4);

        int idx = 2;
        m_modificationType = MULTIPLY;
        if (tokens.size() == 4)
        {
            M3D_ASSERT(tokens[2] == PLUS);
            m_modificationType = ADD;
            idx = 3;
        }

        CStr const& firstToken = tokens.front();
        if (firstToken == "VEHICLE")
        {
            m_applierInfo.applierType = GA_VEHICLE;
        }
        else if (theResourceManager->GetResourceId(firstToken) == -1)
        {
            m_applierInfo.applierType = GA_GUN_BY_TYPE;
            m_applierInfo.targetFiringType = GunPrototypeInfo::Str2FiringType(firstToken);
            if (m_applierInfo.targetFiringType == 13)
            {
                M3D_LOG_INFO("Warning: Unknown firing type: '" + gadgetPrototype->GetDebugDescription() + "'");
            }
        }
        else
        {
            m_applierInfo.applierType = GA_OBJECT_BY_RESOURCE;
            m_applierInfo.targetResourceId = theResourceManager->GetResourceId(firstToken);
        }

        m_propertyName = tokens[1];
        if (m_modificationType)
        {
            if (m_modificationType == ADD)
            {
                m_value = tokens[idx];
            }
            else
            {
                M3D_ASSERT(0);
            }
        }
        else
        {
            m_value = strToFloat(tokens[idx]) * 0.0099999998f;
        }
    }

    ai::Obj* GadgetPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x6DB3E0
        return new Gadget(*this);
    }

    bool GadgetPrototypeInfo::ApplyToVp(ai::VehiclePart* vp, bool enable) const
    {
        // RVA 0x6DCAC0 - a modification reaches a part either because the part's resource matches, or
        // because the part is a gun of the right firing type. A compound gun passes it on to its own parts.
        if (!vp)
        {
            return false;
        }
        int const vpResourceId = vp->GetPrototypeInfo()->m_resourceId;

        for (auto const& modification : m_modifications)
        {
            if (modification.m_applierInfo.applierType == GA_OBJECT_BY_RESOURCE)
            {
                if (theResourceManager->bResourceIsKindOf(
                        vpResourceId, modification.m_applierInfo.targetResourceId) &&
                    !modification.ApplyToObj(vp, enable))
                {
                    M3D_LOG_ERR(
                        "Error: could not apply gadget '" + m_prototypeName + CStr("' to ") +
                        vp->GetDebugDescription());
                    return false;
                }
            }
            else if (modification.m_applierInfo.applierType == GA_GUN_BY_TYPE)
            {
                if (vp->IsKindOf(&ai::Gun::m_classGun))
                {
                    auto const* gunPrototypeInfo = ((Gun const*)vp)->GetPrototypeInfo();
                    if (gunPrototypeInfo->m_firingType == modification.m_applierInfo.targetFiringType &&
                        !modification.ApplyToObj(vp, enable))
                    {
                        M3D_LOG_ERR(
                            "Error: could not apply gadget '" + m_prototypeName + CStr("' to ") +
                            vp->GetDebugDescription());
                        return false;
                    }
                }
                else if (vp->IsKindOf(&ai::CompoundGun::m_classCompoundGun))
                {
                    auto* const compoundGun = (CompoundGun*)vp;
                    auto const* compoundGunPrototypeInfo =
                        (CompoundGunPrototypeInfo const*)compoundGun->GetPrototypeInfo();
                    if (compoundGunPrototypeInfo->GetFiringType() != modification.m_applierInfo.targetFiringType)
                    {
                        continue;
                    }
                    for (auto const& part : compoundGun->m_vehicleParts)
                    {
                        if (!modification.ApplyToObj(part.second.vp, enable))
                        {
                            M3D_LOG_ERR(
                                "Error: could not apply gadget '" + m_prototypeName + CStr("' to ") +
                                part.second.vp->GetDebugDescription());
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }

    GadgetPrototypeInfo::GadgetPrototypeInfo()
    {
        m_skinNum = 0;
        m_bIsUpdating = false;
    }

    int GadgetPrototypeInfo::GetSkinNum() const
    {
        // RVA 0x6DB120
        return m_skinNum;
    }

    CStr const& GadgetPrototypeInfo::GetModelName() const
    {
        // RVA 0x6DB110
        return m_modelName;
    }

    bool GadgetPrototypeInfo::ApplyToVehicle(ai::Vehicle* pVehicle, bool enable) const
    {
        // RVA 0x6DCEA0 - the whole-vehicle modifications go onto the vehicle itself; everything else is
        // offered to each of its parts in turn.
        if (!pVehicle)
        {
            return false;
        }

        for (auto const& modification : m_modifications)
        {
            if (modification.m_applierInfo.applierType != GA_VEHICLE)
            {
                continue;
            }
            if (!modification.ApplyToObj(pVehicle, enable))
            {
                M3D_LOG_ERR(
                    "Error: could not apply gadget '" + m_prototypeName + CStr("' to ") +
                    pVehicle->GetDebugDescription());
                return false;
            }
        }

        // NOTE: a part that refuses the gadget is not reported here; only the vehicle-wide pass can fail.
        for (auto const& partName : pVehicle->GetAttachedPartNames())
        {
            ApplyToVp(pVehicle->GetPartByName(partName), enable);
        }
        return true;
    }

    bool GadgetPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            CStr strModifications;
            m3d::SafeStrAttrib(strModifications, xmlNode, "Modifications");

            std::vector<CStr> tokens;
            m3d::Tokenize(strModifications, tokens, ";");

            for (CStr const& token : tokens)
            {
                ModificationInfo info(token, this);
                m_modifications.push_back(std::move(info));
            }

            m3d::SafeStrAttrib(m_modelName, xmlNode, "ModelFile");
            m3d::SafeIntAttrib(m_skinNum, xmlNode, "SkinNum");
        }
        return result;
    }

    retruxx::vector<GadgetPrototypeInfo::ModificationInfo, retruxx::allocator<GadgetPrototypeInfo::ModificationInfo>> const&
        GadgetPrototypeInfo::GetModifications() const
    {
        // RVA 0x6DB130
        return m_modifications;
    }

    int Gadget::GetPropertyId(char const* PropertyName) const
    {
        // RVA 0x6DB9B0
        auto const it = m_propertiesMap.find(PropertyName);
        if (it != m_propertiesMap.end())
        {
            return it->second;
        }
        return Obj::GetPropertyId(PropertyName);
    }

    int Gadget::GetSlotNum() const
    {
        // RVA 0x48C1C0
        return m_slotNum;
    }

    void Gadget::SetSlotNum(int slotNum)
    {
        // RVA 0x4451C0
        m_slotNum = slotNum;
    }

    GadgetPrototypeInfo const* Gadget::GetPrototypeInfo() const
    {
        // RVA 0x6DBA20
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), GadgetPrototypeInfo const);
    }

    Gadget::Gadget(GadgetPrototypeInfo const& prototypeInfo) : Obj(prototypeInfo)
    {
        // RVA 0x6DB140 - a gadget starts out fitted to no slot.
        m_slotNum = -1;
    }

    bool Gadget::ApplyToVehicle(Vehicle* pVehicle, bool enable) const
    {
        // RVA 0x6DB180
        return GetPrototypeInfo()->ApplyToVehicle(pVehicle, enable);
    }

    bool Gadget::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        // RVA 0x6DB190
        if (propertyId != 41)
        {
            return Obj::SetPropertyById(propertyId, newValue);
        }
        m_slotNum = newValue.GetAsID();
        return true;
    }

    m3d::Class* Gadget::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    bool Gadget::ApplyToVp(VehiclePart* vp, bool enable) const
    {
        // RVA 0x6DD0B0
        return GetPrototypeInfo()->ApplyToVp(vp, enable);
    }

    void Gadget::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
    {
        // RVA 0x6DBA90
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.first);
        }
        Obj::GetPropertiesNames(Props);
    }

    eGObjPropertySaveStatus Gadget::GetPropertySaveStatus(int id) const
    {
        // RVA 0x6DBA50
        auto const it = m_propertiesSaveStatesMap.find(id);
        if (it != m_propertiesSaveStatesMap.end())
        {
            return it->second;
        }
        return Obj::GetPropertySaveStatus(id);
    }

    CStr Gadget::GetPropertyName(int id) const
    {
        // RVA 0x6DBB90
        for (auto const& property : m_propertiesMap)
        {
            if (property.second == id)
            {
                return property.first;
            }
        }
        return Obj::GetPropertyName(id);
    }

    m3d::Class* Gadget::GetClass() const
    {
        // RVA 0x6DB0E0
        return RT_CLASS_LOCAL(Gadget);
    }

    void Gadget::Registration()
    {
        m_propertiesMap["SlotNum"] = 41;
    }

    void Gadget::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        // RVA 0x6DBB10
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.second);
        }
        Obj::GetPropertiesIDs(Props);
    }

    bool Gadget::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x6DD0D0
        if (propertyId != 41)
        {
            return Obj::_GetPropertyInternal(propertyId, retVal);
        }
        retVal = m3d::AIParam(m_slotNum);
        return true;
    }

    bool Gadget::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x6DD130 - a gadget defaults to being in no slot.
        if (propertyId != 41)
        {
            return Obj::_GetPropertyDefaultInternal(propertyId, retVal);
        }
        retVal = m3d::AIParam(-1);
        return true;
    }

    Gadget::~Gadget()
    {
        // RVA 0x6DB170 - the base class does all the work.
    }

    void Gadget::RegisterProperty(char const* Name, int id, eGObjPropertySaveStatus saveStatus)
    {
        // RVA 0x6DBC00 - SAVE_PROP_NORMAL is the default and is not recorded.
        m_propertiesMap[Name] = id;
        if (saveStatus)
        {
            m_propertiesSaveStatesMap[id] = saveStatus;
        }
    }

    m3d::Object* Gadget::Clone()
    {
        // RVA 0x6DB5C0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* Gadget::CreateObject()
    {
        // RVA 0x6DB780
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }
}  // namespace ai
