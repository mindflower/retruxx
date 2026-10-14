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
            auxZoneInfo(const ai::SettlementPrototypeInfo::auxZoneInfo& __that);
            auxZoneInfo();
        };

    public:
        retruxx::vector<ai::SettlementPrototypeInfo::auxZoneInfo, retruxx::allocator<ai::SettlementPrototypeInfo::auxZoneInfo> > m_zoneInfos;
        int m_vehiclesPrototypeId;
        SettlementPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void PostLoad() override;

    private:
        CStr m_vehiclesPrototypeName;
    };

    class Settlement : public ai::SimplePhysicObj
    {
    public:
        using StrTeamMap = retruxx::map<CStr, ai::Team*, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, ai::Team*> > >;

    protected:
        virtual  ~Settlement() override;

    protected:
        Settlement(const ai::SettlementPrototypeInfo& prototypeInfo);
        Settlement(const ai::Settlement&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classSettlement;
        virtual const ai::SettlementPrototypeInfo* GetPrototypeInfo() const override;
        virtual int OnEvent(const ai::Event& evn) override;
        static void __fastcall Registration();
        virtual void Remove() override;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual void AddChild(ai::Obj* pObj) override;
        virtual bool RemoveChild(ai::Obj* pChild) override;
        virtual void SetPosition(const CVector& pos) override;
        virtual void SetPositionSelf(const CVector& pos) override;
        virtual void SetRotation(const Quaternion& rot) override;
        virtual void SetRotationSelf(const Quaternion& rot) override;
        CVector GetEnterPos() const;
        CVector GetDeployPos() const;
        CVector GetAttackPos() const;
        CVector GetDefendPos() const;
        CVector GetCaravanArrivePos() const;
        ai::Location* GetLocation(ai::Location::LocationType locationType) const;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void CreateChildren() override;
        ai::Team* GetTeamByType(const CStr& partType);
        void SetTeamByType(const CStr& partType, ai::Team* team);
        const retruxx::set<ai::Location*, retruxx::less<ai::Location*>, retruxx::allocator<ai::Location*> >& GetLocations() const;

    protected:
        retruxx::set<ai::StaticAutoGun*, retruxx::less<ai::StaticAutoGun*>, retruxx::allocator<ai::StaticAutoGun*> > m_staticAutoGuns;
        retruxx::set<ai::Location*, retruxx::less<ai::Location*>, retruxx::allocator<ai::Location*> > m_locations;
        retruxx::vector<ai::Team*, retruxx::allocator<ai::Team*> > m_caravans;
        retruxx::map<CStr, ai::Team*, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, ai::Team*> > > m_teams;
        ai::NumericInRange<int> m_population;
        virtual void _InternalPostLoad() override;
        ai::Vehicle* _SpawnVehicle();
        int _GetOverallPopulation();
        int _GetMaxOverallPopulaition();
        virtual int _GetNumSpawnedVehicles();
        virtual int _GetMaxSpawnedVehicles();
        void CreateChildLocation(ai::Location::LocationType locationType, const CStr& nameSuffix, const CVector& pos);

    private:
        void FillingFeedBackParam();
    };
}
