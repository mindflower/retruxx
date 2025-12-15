#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"
#include "guns/gun.h"
#include "core/aiparam.h"

namespace ai
{
    class VehiclePart;
    class Vehicle;

    class GadgetPrototypeInfo : public PrototypeInfo
    {
    public:
        enum GadgetAppliers
        {
            GA_VEHICLE = 0,
            GA_OBJECT_BY_RESOURCE = 1,
            GA_GUN_BY_TYPE = 2,
        };

        struct GadgetApplicationInfo
        {
            GadgetAppliers applierType;
            int targetResourceId;
            FiringTypes targetFiringType;

            GadgetApplicationInfo();
        };

        struct ModificationInfo
        {
            enum ModificationType
            {
                MULTIPLY = 0,
                ADD = 1,
            };

            GadgetApplicationInfo m_applierInfo;
            CStr m_propertyName;
            ModificationType m_modificationType;
            m3d::AIParam m_value;

            ModificationInfo(CStr const& str, GadgetPrototypeInfo const* gadgetPrototype);
            bool ApplyToObj(ai::Obj* pObj, bool enable) const;
        };

        using ModificationInfoVector = retruxx::vector<GadgetPrototypeInfo::ModificationInfo>;

    public:
        GadgetPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
        virtual bool ApplyToVehicle(ai::Vehicle* pVehicle, bool enable) const;
        bool ApplyToVp(ai::VehiclePart* vp, bool enable) const;
        CStr const& GetModelName() const;
        int GetSkinNum() const;
        ModificationInfoVector const& GetModifications() const;

    private:
        ModificationInfoVector m_modifications;
        CStr m_modelName;
        int m_skinNum;
    };

    static_assert(sizeof(GadgetPrototypeInfo) == 0x0060);

    class Gadget : public Obj
    {
    protected:
        virtual ~Gadget() override;

    private:
        Gadget(GadgetPrototypeInfo const& prototypeInfo);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classGadget;
        virtual ai::GadgetPrototypeInfo const* GetPrototypeInfo() const override;

    protected:
        static void __fastcall RegisterProperty(char const* Name, int id, eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override;
        virtual void GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const override;
        virtual void GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const override;
        virtual CStr GetPropertyName(int id) const override;
        virtual bool SetPropertyById(int propertyId, m3d::AIParam const& newValue) override;
        virtual int GetPropertyId(char const* PropertyName) const override;

    protected:
        static inline retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int>>> m_propertiesMap;
        static inline retruxx::
            map<int, enum ai::eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus>>>
                m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override;

    public:
        int GetSlotNum() const;
        void SetSlotNum(int slotNum);
        bool ApplyToVehicle(ai::Vehicle* pVehicle, bool enable) const;
        bool ApplyToVp(ai::VehiclePart* vp, bool enable) const;
        static void __fastcall Registration();

    private:
        int m_slotNum;
    };

    static_assert(sizeof(Gadget) == 0x00c4);
}  // namespace ai
