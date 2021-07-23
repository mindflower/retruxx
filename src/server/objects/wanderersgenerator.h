#pragma once
#include "base/prototypeinfo.h"
#include <vector>

namespace ai
{
    class Vehicle;
    class Obj;

    class WanderersGeneratorPrototypeInfo : public PrototypeInfo
    {
    public:
        class VehiclePartDescription
        {
        public:
            VehiclePartDescription();
            CStr const& GetRandomPrototypeName() const;
            bool HasPrototypes() const;
            void PostLoad();
            bool DoesPresent() const;
            int GetRandomPrototypeId() const;
            void LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

        private:
            bool m_present;
            std::vector<CStr> m_prototypeNames;
            std::vector<int> m_prototypeIds;
        };

        class VehicleDescription
        {
        public:
            VehiclePartDescription const& GetCabinBigGunDescription() const;
            VehiclePartDescription const& GetCabinSmallGunDescription() const;
            void LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
            VehiclePartDescription const& GetBasketSideGunDescription() const;
            void PostLoad();
            VehiclePartDescription const& GetCabinDescription() const;
            static void __fastcall LoadPartFromXML(VehiclePartDescription&, char const*, m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
            VehiclePartDescription const& GetBasketDescription() const;
            int GetPrototypeId() const;
            VehiclePartDescription const& GetBasketBigGun0Description() const;
            VehiclePartDescription const& GetBasketBigGun1Description() const;
            VehiclePartDescription const& GetCabinSpecialWeaponDescription() const;
            VehiclePartDescription const& GetBasketSmallGun1Description() const;
            VehiclePartDescription const& GetBasketSmallGun0Description() const;

        private:
            CStr m_prototype;
            int m_prototypeId;
            VehiclePartDescription m_cabin;
            VehiclePartDescription m_basket;
            VehiclePartDescription m_cabinSmallGun;
            VehiclePartDescription m_cabinBigGun;
            VehiclePartDescription m_cabinSpecialWeapon;
            VehiclePartDescription m_basketSmallGun0;
            VehiclePartDescription m_basketSmallGun1;
            VehiclePartDescription m_basketBigGun0;
            VehiclePartDescription m_basketBigGun1;
            VehiclePartDescription m_basketSideGun;
        };
    public:
        virtual Obj* CreateTargetObject() const;
        std::vector<int, std::allocator<int> > GenerateAndPlace(CVector const&, int) const;
        std::vector<int, std::allocator<int> > Generate(int) const;
        virtual void PostLoad();
        WanderersGeneratorPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        void _ChangePartByName(Vehicle*, CStr const&, int) const;
        void _GenerateIndexes(std::vector<int, std::allocator<int> >&, int) const;
        void _TuneVehicleParts(Vehicle*, VehicleDescription const&) const;
        void _RemovePartByName(Vehicle*, CStr const&) const;
        void _TunePart(Vehicle*, CStr const&, VehiclePartDescription const&) const;

    private:
        std::vector<VehicleDescription> m_vehicleDescriptions;
        int m_desiredCountLow;
        int m_desiredCountHigh;
    };
}
