#pragma once
#include "settlement.h"
#include <server/components/numericinrangeregenerating.h>

namespace ai
{
    class LairPrototypeInfo : public ai::SettlementPrototypeInfo
    {
    public:
        LairPrototypeInfo();
        /* 0x00a0 */ int m_maxAttackers;
        /* 0x00a4 */ int m_maxDefenders;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
    }; /* size: 0x00a8 */

    static_assert(sizeof(LairPrototypeInfo) == 0x00a8);

    class Lair : public ai::Settlement
    {
        friend class LairPrototypeInfo;

    protected:
        virtual ~Lair() override /* 0x00 */;

    private:
        Lair(const ai::LairPrototypeInfo& prototypeInfo);
        Lair(const ai::Lair&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        RT_CLASS_DECLARE(Lair);
        virtual const ai::LairPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;

    protected:
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override /* 0x00 */;
        virtual void GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> >& Props) const override /* 0x00 */;
        virtual void GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& Props) const override /* 0x00 */;
        virtual CStr GetPropertyName(int id) const override /* 0x00 */;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override /* 0x00 */;
        virtual int GetPropertyId(const char* PropertyName) const override /* 0x00 */;

    protected:
        static inline retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int> > > m_propertiesMap;
        static inline retruxx::map<int, enum ai::eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus> > > m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;

    public:
        virtual int OnEvent(const ai::Event& evn) override /* 0x00 */;
        enum LairState
        {
            LS_REPRODUCE_WAIT = 0,
            LS_REPRODUCE = 1,
            LS_DYING = 2,
            LS_DEAD = 3,
        };
        virtual void CreateChildren() override /* 0x00 */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        static void __fastcall Registration();

    protected:
        virtual int _GetNumSpawnedVehicles() override /* 0x1b4 */;
        virtual int _GetMaxSpawnedVehicles() override /* 0x1b8 */;

    private:
        /* 0x0224 */ ai::NumericInRangeRegenerating<float> m_reproductTime;
        /* 0x02fc */ int m_maxAttackers;
        /* 0x0300 */ int m_maxDefenders;
        ai::Team* _GetAttackingTeam();
        ai::Team* _GetDefendingTeam();
        /* 0x0304 */ ai::Lair::LairState m_state;
        void _OnObjectEntersLocation(const ai::Event& evn);
    }; /* size: 0x0308 */

    static_assert(sizeof(Lair) == 0x0308);
}
