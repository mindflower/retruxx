#pragma once
#include "location.h"
#include "base/simplephysicobj.h"
#include <server/components/numericinrange.h>

namespace ai
{
    class Vehicle;
    class Team;
    class StaticAutoGun;

    class SettlementPrototypeInfo : public ai::SimplePhysicObjPrototypeInfo
    {
    public:
        struct auxZoneInfo
        {
            CStr m_action;
            CVector m_offset;
            float m_radius;
            auxZoneInfo(ai::SettlementPrototypeInfo::auxZoneInfo const& __that);
            auxZoneInfo();
        };

    public:
        retruxx::vector<
            ai::SettlementPrototypeInfo::auxZoneInfo,
            retruxx::allocator<ai::SettlementPrototypeInfo::auxZoneInfo>>
            m_zoneInfos;
        int m_vehiclesPrototypeId;
        SettlementPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void PostLoad() override;

    private:
        CStr m_vehiclesPrototypeName;
    };

    class Settlement : public ai::SimplePhysicObj
    {
    public:
        using StrTeamMap = retruxx::
            map<CStr, ai::Team*, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, ai::Team*>>>;

    protected:
        virtual ~Settlement() override;

    protected:
        Settlement(ai::SettlementPrototypeInfo const& prototypeInfo);
        Settlement(ai::Settlement const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classSettlement;
        virtual ai::SettlementPrototypeInfo const* GetPrototypeInfo() const override;
        virtual int OnEvent(ai::Event const& evn) override;
        static void __fastcall Registration();
        virtual void Remove() override;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual void AddChild(ai::Obj* pObj) override;
        virtual bool RemoveChild(ai::Obj* pChild) override;
        virtual void SetPosition(CVector const& pos) override;
        virtual void SetPositionSelf(CVector const& pos) override;
        virtual void SetRotation(Quaternion const& rot) override;
        virtual void SetRotationSelf(Quaternion const& rot) override;
        CVector GetEnterPos() const;
        CVector GetDeployPos() const;
        CVector GetAttackPos() const;
        CVector GetDefendPos() const;
        CVector GetCaravanArrivePos() const;
        ai::Location* GetLocation(ai::Location::LocationType locationType) const;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void CreateChildren() override;
        ai::Team* GetTeamByType(CStr const& partType);
        void SetTeamByType(CStr const& partType, ai::Team* team);
        retruxx::set<ai::Location*, retruxx::less<ai::Location*>, retruxx::allocator<ai::Location*>> const&
            GetLocations() const;

    protected:
        retruxx::set<ai::StaticAutoGun*, retruxx::less<ai::StaticAutoGun*>, retruxx::allocator<ai::StaticAutoGun*>>
            m_staticAutoGuns;
        retruxx::set<ai::Location*, retruxx::less<ai::Location*>, retruxx::allocator<ai::Location*>> m_locations;
        retruxx::vector<ai::Team*, retruxx::allocator<ai::Team*>> m_caravans;
        retruxx::map<CStr, ai::Team*, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, ai::Team*>>>
            m_teams;
        ai::NumericInRange<int> m_population;
        virtual void _InternalPostLoad() override;
        ai::Vehicle* _SpawnVehicle();
        int _GetOverallPopulation();
        int _GetMaxOverallPopulaition();
        virtual int _GetNumSpawnedVehicles();
        virtual int _GetMaxSpawnedVehicles();
        void CreateChildLocation(ai::Location::LocationType locationType, CStr const& nameSuffix, CVector const& pos);

    private:
        void FillingFeedBackParam();
    };
}  // namespace ai
