#pragma once
#include "location.h"
#include "base/simplephysicobj.h"
#include <server/components/numericinrange.h>

namespace ai
{
    class Vehicle;
    class Team;
    class StaticAutoGun;

    class SettlementPrototypeInfo : public SimplePhysicObjPrototypeInfo
    {
    public:
        class auxZoneInfo
        {
        public:
            auxZoneInfo(auxZoneInfo const&);

        private:
            CStr m_action;
            CVector m_offset;
            float m_radius;
        };

    public:
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual void PostLoad();
        SettlementPrototypeInfo();

    private:
        std::vector<auxZoneInfo> m_zoneInfos;
        int m_vehiclesPrototypeId;
        CStr m_vehiclesPrototypeName;
    };

    class Settlement : public SimplePhysicObj
    {
    public:
        virtual m3d::Class* GetClass() const;
        static void __fastcall Registration();
        virtual void Remove();
        Settlement(SettlementPrototypeInfo const&);
        virtual void CreateChildren();
        CVector GetDeployPos() const;
        virtual void SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        CVector GetDefendPos() const;
        Location* GetLocation(Location::LocationType) const;
        virtual bool CanChildBeAdded(m3d::Class*) const;
        virtual void SetRotationSelf(Quaternion const&);
        CVector GetCaravanArrivePos() const;
        static m3d::Class* GetBaseClass();
        virtual int OnEvent(Event const&);
        virtual void SetPosition(CVector const&);
        Team* GetTeamByType(CStr const&);
        void SetTeamByType(CStr const&, Team*);
        virtual void LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual void SetPositionSelf(CVector const&);
        virtual SettlementPrototypeInfo const* GetPrototypeInfo() const;
        virtual bool RemoveChild(Obj*);
        virtual void SetRotation(Quaternion const&);
        CVector GetAttackPos() const;
        virtual void AddChild(Obj*);
        CVector GetEnterPos() const;

    protected:
        Vehicle* _SpawnVehicle();
        virtual int _GetNumSpawnedVehicles();
        int _GetOverallPopulation();
        void CreateChildLocation(Location::LocationType, CStr const&, CVector const&);
        int _GetMaxOverallPopulaition();
        virtual void _InternalPostLoad();
        virtual int _GetMaxSpawnedVehicles();
        virtual ~Settlement();

    private:
        static m3d::Object* CreateObject();
        void FillingFeedBackParam();
        virtual m3d::Object* Clone();

    public:
        RT_CLASS_INLINE_DECLARE(Settlement);

    private:
        std::set<StaticAutoGun*> m_staticAutoGuns;
        std::set<Location*> m_locations;
        std::vector<Team*> m_caravans;
        std::map<CStr, Team*> m_teams;
        NumericInRange<int> m_population;
    };
}
