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
        struct auxZoneInfo;

    public:
        /* 0x0080 */ retruxx::vector<ai::SettlementPrototypeInfo::auxZoneInfo, retruxx::allocator<ai::SettlementPrototypeInfo::auxZoneInfo> > m_zoneInfos;
        /* 0x0090 */ int m_vehiclesPrototypeId;
        SettlementPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void PostLoad() override /* 0x00 */;

    private:
        /* 0x0094 */ CStr m_vehiclesPrototypeName;
    }; /* size: 0x00a0 */

    static_assert(sizeof(SettlementPrototypeInfo) == 0x00a0);

    class Settlement : public ai::SimplePhysicObj
    {
    public:
        using StrTeamMap = retruxx::map<CStr, ai::Team*, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, ai::Team*> > >;

    protected:
        virtual  ~Settlement() override /* 0x00 */;

    private:
        Settlement(const ai::SettlementPrototypeInfo& prototypeInfo);
        Settlement(const ai::Settlement&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classSettlement;
        virtual const ai::SettlementPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;
        virtual int OnEvent(const ai::Event& evn) override /* 0x00 */;
        static void __fastcall Registration();
        virtual void Remove() override /* 0x00 */;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override /* 0x00 */;
        virtual void AddChild(ai::Obj* pObj) override /* 0x00 */;
        virtual bool RemoveChild(ai::Obj* pChild) override /* 0x00 */;
        virtual void SetPosition(const CVector& pos) override /* 0x00 */;
        virtual void SetPositionSelf(const CVector& pos) override /* 0x00 */;
        virtual void SetRotation(const Quaternion& rot) override /* 0x00 */;
        virtual void SetRotationSelf(const Quaternion& rot) override /* 0x00 */;
        CVector GetEnterPos() const;
        CVector GetDeployPos() const;
        CVector GetAttackPos() const;
        CVector GetDefendPos() const;
        CVector GetCaravanArrivePos() const;
        ai::Location* GetLocation(ai::Location::LocationType locationType) const;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual void CreateChildren() override /* 0x00 */;
        ai::Team* GetTeamByType(const CStr& partType);
        void SetTeamByType(const CStr& partType, ai::Team* team);
        const retruxx::set<ai::Location*, retruxx::less<ai::Location*>, retruxx::allocator<ai::Location*> >& GetLocations() const;

    protected:
        /* 0x0144 */ retruxx::set<ai::StaticAutoGun*, retruxx::less<ai::StaticAutoGun*>, retruxx::allocator<ai::StaticAutoGun*> > m_staticAutoGuns;
        /* 0x0150 */ retruxx::set<ai::Location*, retruxx::less<ai::Location*>, retruxx::allocator<ai::Location*> > m_locations;
        /* 0x015c */ retruxx::vector<ai::Team*, retruxx::allocator<ai::Team*> > m_caravans;
        /* 0x016c */ retruxx::map<CStr, ai::Team*, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, ai::Team*> > > m_teams;
        /* 0x0178 */ ai::NumericInRange<int> m_population;
        virtual void _InternalPostLoad() override /* 0x00 */;
        ai::Vehicle* _SpawnVehicle();
        int _GetOverallPopulation();
        int _GetMaxOverallPopulaition();
        virtual int _GetNumSpawnedVehicles() /* 0x1b4 */;
        virtual int _GetMaxSpawnedVehicles() /* 0x1b8 */;
        void CreateChildLocation(ai::Location::LocationType locationType, const CStr& nameSuffix, const CVector& pos);

    private:
        void FillingFeedBackParam();
    }; /* size: 0x0224 */

    static_assert(sizeof(Settlement) == 0x0224);
}
