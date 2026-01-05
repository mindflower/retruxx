#include "gadget.h"
#include "core/kernel.h"
#include <server/resourcemanager.h>
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

    bool GadgetPrototypeInfo::ModificationInfo::ApplyToObj(ai::Obj*, bool) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GadgetPrototypeInfo::ModificationInfo::ModificationInfo(CStr const& str, ai::GadgetPrototypeInfo const* gadgetPrototype)
    {
        // TODO: check this function
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool GadgetPrototypeInfo::ApplyToVp(ai::VehiclePart*, bool) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GadgetPrototypeInfo::GadgetPrototypeInfo()
    {
        m_skinNum = 0;
        m_bIsUpdating = false;
    }

    int GadgetPrototypeInfo::GetSkinNum() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr const& GadgetPrototypeInfo::GetModelName() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool GadgetPrototypeInfo::ApplyToVehicle(ai::Vehicle*, bool) const
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Gadget::GetPropertyId(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Gadget::GetSlotNum() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gadget::SetSlotNum(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GadgetPrototypeInfo const* Gadget::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Gadget::Gadget(GadgetPrototypeInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gadget::ApplyToVehicle(Vehicle*, bool) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gadget::SetPropertyById(int, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Gadget::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    bool Gadget::ApplyToVp(VehiclePart*, bool) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gadget::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    eGObjPropertySaveStatus Gadget::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr Gadget::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Gadget::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gadget::Registration()
    {
        m_propertiesMap["SlotNum"] = 41;
    }

    void Gadget::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gadget::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gadget::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Gadget::~Gadget()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gadget::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Gadget::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Gadget::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
