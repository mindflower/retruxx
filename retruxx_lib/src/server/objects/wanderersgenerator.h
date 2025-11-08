#pragma once
#include "vehicle.h"
#include "base/prototypeinfo.h"
#include "thirdparty/containers.h"

namespace ai
{
    class WanderersGeneratorPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        class VehiclePartDescription
        {
        public:
            VehiclePartDescription(const ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& __that);
            VehiclePartDescription();
            void LoadFromXML(m3d::cmn::XmlFile* xmlfile, const m3d::cmn::XmlNode* xmlNode);
            void PostLoad();
            bool HasPrototypes() const;
            bool DoesPresent() const;
            const CStr& GetRandomPrototypeName() const;
            int GetRandomPrototypeId() const;

        private:
            /* 0x0000 */ bool m_present;
            /* 0x0001 */ char Padding_158[3];
            retruxx::vector<CStr, retruxx::allocator<CStr> > m_prototypeNames;
            retruxx::vector<int, retruxx::allocator<int> > m_prototypeIds;
        }; /* size: 0x0024 */

        class VehicleDescription
        {
        public:
            void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
            static void __fastcall LoadPartFromXML(ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& part, const char* partName, m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
            void PostLoad();
            const ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& GetCabinDescription() const;
            const ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& GetBasketDescription() const;
            const ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& GetCabinSmallGunDescription() const;
            const ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& GetCabinBigGunDescription() const;
            const ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& GetCabinSpecialWeaponDescription() const;
            const ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& GetBasketSmallGun0Description() const;
            const ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& GetBasketSmallGun1Description() const;
            const ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& GetBasketBigGun0Description() const;
            const ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& GetBasketBigGun1Description() const;
            const ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& GetBasketSideGunDescription() const;
            const CStr& GetPrototypeName() const;
            int GetPrototypeId() const;

        private:
            CStr m_prototype;
            /* 0x000c */ int m_prototypeId;
            ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription m_cabin;
            ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription m_basket;
            ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription m_cabinSmallGun;
            ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription m_cabinBigGun;
            ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription m_cabinSpecialWeapon;
            ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription m_basketSmallGun0;
            ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription m_basketSmallGun1;
            ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription m_basketBigGun0;
            ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription m_basketBigGun1;
            ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription m_basketSideGun;
        }; /* size: 0x0178 */

        using VehicleDescriptions = retruxx::vector<ai::WanderersGeneratorPrototypeInfo::VehicleDescription, retruxx::allocator<ai::WanderersGeneratorPrototypeInfo::VehicleDescription> >;

    public:
        WanderersGeneratorPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        retruxx::vector<int, retruxx::allocator<int> > Generate(int count) const;
        retruxx::vector<int, retruxx::allocator<int> > GenerateAndPlace(const CVector& pos, int count) const;
        virtual ai::Obj* CreateTargetObject() const override /* 0x10 */;
        virtual void PostLoad() override /* 0x08 */;

    private:
        void _GenerateIndexes(retruxx::vector<int, retruxx::allocator<int> >& indexes, int count) const;
        void _TuneVehicleParts(ai::Vehicle* vehicle, const ai::WanderersGeneratorPrototypeInfo::VehicleDescription& vehicleDescription) const;
        void _TunePart(ai::Vehicle* vehicle, const CStr& partName, const ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& partDescription) const;
        void _ChangePartByName(ai::Vehicle* vehicle, const CStr& partName, int partPrototypeId) const;
        void _RemovePartByName(ai::Vehicle* vehicle, const CStr& partName) const;
        retruxx::vector<ai::WanderersGeneratorPrototypeInfo::VehicleDescription, retruxx::allocator<ai::WanderersGeneratorPrototypeInfo::VehicleDescription> > m_vehicleDescriptions;
        /* 0x0050 */ int m_desiredCountLow;
        /* 0x0054 */ int m_desiredCountHigh;
    }; /* size: 0x0058 */
}
