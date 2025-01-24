#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"
#include "guns/gun.h"
#include <core/aiparam.h>

namespace ai
{
    class VehiclePart;
    class Vehicle;

    class GadgetPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        enum GadgetAppliers;
        struct GadgetApplicationInfo;
        struct ModificationInfo;
        using ModificationInfoVector = retruxx::vector<ai::GadgetPrototypeInfo::ModificationInfo, retruxx::allocator<ai::GadgetPrototypeInfo::ModificationInfo> >;

    public:
        GadgetPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x10 */;
        virtual bool ApplyToVehicle(ai::Vehicle* pVehicle, bool enable) const /* 0x1c */;
        bool ApplyToVp(ai::VehiclePart* vp, bool enable) const;
        const CStr& GetModelName() const;
        int GetSkinNum() const;
        const retruxx::vector<ai::GadgetPrototypeInfo::ModificationInfo, retruxx::allocator<ai::GadgetPrototypeInfo::ModificationInfo> >& GetModifications() const;

    private:
        /* 0x0040 */ retruxx::vector<ai::GadgetPrototypeInfo::ModificationInfo, retruxx::allocator<ai::GadgetPrototypeInfo::ModificationInfo> > m_modifications;
        /* 0x0050 */ CStr m_modelName;
        /* 0x005c */ int m_skinNum;
    }; /* size: 0x0060 */

    static_assert(sizeof(GadgetPrototypeInfo) == 0x0060);

    class Gadget : public ai::Obj
    {
    protected:
        virtual  ~Gadget() override /* 0x00 */;

    private:
        Gadget(const ai::GadgetPrototypeInfo& prototypeInfo);
        Gadget(const ai::Gadget&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classGadget;
        virtual const ai::GadgetPrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;

    protected:
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override /* 0x58 */;
        virtual void GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> >& Props) const override /* 0x5c */;
        virtual void GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& Props) const override /* 0x60 */;
        virtual CStr GetPropertyName(int id) const override /* 0x78 */;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override /* 0x7c */;
        virtual int GetPropertyId(const char* PropertyName) const override /* 0x74 */;

    protected:
        static retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int> > > m_propertiesMap;
        static retruxx::map<int, enum ai::eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus> > > m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x10c */;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x108 */;

    public:
        int GetSlotNum() const;
        void SetSlotNum(int slotNum);
        bool ApplyToVehicle(ai::Vehicle* pVehicle, bool enable) const;
        bool ApplyToVp(ai::VehiclePart* vp, bool enable) const;
        static void __fastcall Registration();

    private:
        /* 0x00c0 */ int m_slotNum;
    }; /* size: 0x00c4 */

    static_assert(sizeof(Gadget) == 0x00c4);
}
