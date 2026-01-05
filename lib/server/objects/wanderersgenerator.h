#pragma once
#include "vehicle.h"
#include "retruxx/common.h"

namespace ai
{
    class WanderersGeneratorPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        class VehiclePartDescription
        {
        public:
            VehiclePartDescription();
            void LoadFromXML(m3d::cmn::XmlFile* xmlfile, m3d::cmn::XmlNode const* xmlNode);
            void PostLoad();
            bool HasPrototypes() const;
            bool DoesPresent() const;
            CStr const& GetRandomPrototypeName() const;
            int GetRandomPrototypeId() const;

        private:
            bool m_present;
            retruxx::vector<CStr, retruxx::allocator<CStr>> m_prototypeNames;
            retruxx::vector<int, retruxx::allocator<int>> m_prototypeIds;
        };

        class VehicleDescription
        {
        public:
            void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);
            static void __fastcall LoadPartFromXML(
                ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& part,
                char const* partName,
                m3d::cmn::XmlFile* xmlFile,
                m3d::cmn::XmlNode const* xmlNode);
            void PostLoad();
            ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const& GetCabinDescription() const;
            ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const& GetBasketDescription() const;
            ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const& GetCabinSmallGunDescription() const;
            ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const& GetCabinBigGunDescription() const;
            ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const& GetCabinSpecialWeaponDescription() const;
            ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const& GetBasketSmallGun0Description() const;
            ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const& GetBasketSmallGun1Description() const;
            ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const& GetBasketBigGun0Description() const;
            ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const& GetBasketBigGun1Description() const;
            ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const& GetBasketSideGunDescription() const;
            CStr const& GetPrototypeName() const;
            int GetPrototypeId() const;

        private:
            CStr m_prototype;
            int m_prototypeId;
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
        };

        using VehicleDescriptions = retruxx::
            vector<ai::WanderersGeneratorPrototypeInfo::VehicleDescription, retruxx::allocator<ai::WanderersGeneratorPrototypeInfo::VehicleDescription>>;

    public:
        WanderersGeneratorPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        retruxx::vector<int, retruxx::allocator<int>> Generate(int count) const;
        retruxx::vector<int, retruxx::allocator<int>> GenerateAndPlace(CVector const& pos, int count) const;
        virtual ai::Obj* CreateTargetObject() const override;
        virtual void PostLoad() override;

    private:
        void _GenerateIndexes(retruxx::vector<int, retruxx::allocator<int>>& indexes, int count) const;
        void _TuneVehicleParts(ai::Vehicle* vehicle, ai::WanderersGeneratorPrototypeInfo::VehicleDescription const& vehicleDescription) const;
        void _TunePart(ai::Vehicle* vehicle, CStr const& partName, ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const& partDescription) const;
        void _ChangePartByName(ai::Vehicle* vehicle, CStr const& partName, int partPrototypeId) const;
        void _RemovePartByName(ai::Vehicle* vehicle, CStr const& partName) const;

        retruxx::vector<ai::WanderersGeneratorPrototypeInfo::VehicleDescription, retruxx::allocator<ai::WanderersGeneratorPrototypeInfo::VehicleDescription>>
            m_vehicleDescriptions;
        int m_desiredCountLow;
        int m_desiredCountHigh;
    };
}  // namespace ai
