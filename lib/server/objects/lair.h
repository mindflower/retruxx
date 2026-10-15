#pragma once
#include "settlement.h"
#include <server/components/numericinrangeregenerating.h>

namespace ai
{
    class LairPrototypeInfo : public ai::SettlementPrototypeInfo
    {
    public:
        LairPrototypeInfo();
        int m_maxAttackers;
        int m_maxDefenders;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class Lair : public ai::Settlement
    {
        friend class LairPrototypeInfo;

    protected:
        virtual ~Lair() override;

    private:
        Lair(ai::LairPrototypeInfo const& prototypeInfo);
        Lair(ai::Lair const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        RT_CLASS_DECLARE(Lair);
        virtual ai::LairPrototypeInfo const* GetPrototypeInfo() const override;

    protected:
        static void RegisterProperty(char const* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override;
        virtual void GetPropertiesNames(
            retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const override;
        virtual void GetPropertiesIDs(
            retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const override;
        virtual CStr GetPropertyName(int id) const override;
        virtual bool SetPropertyById(int propertyId, m3d::AIParam const& newValue) override;
        virtual int GetPropertyId(char const* PropertyName) const override;

    protected:
        static inline retruxx::
            map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int>>>
                m_propertiesMap;
        static inline retruxx::map<
            int,
            enum ai::eGObjPropertySaveStatus,
            retruxx::less<int>,
            retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus>>>
            m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override;

    public:
        virtual int OnEvent(ai::Event const& evn) override;
        enum LairState
        {
            LS_REPRODUCE_WAIT = 0,
            LS_REPRODUCE = 1,
            LS_DYING = 2,
            LS_DEAD = 3,
        };
        virtual void CreateChildren() override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        static void Registration();

    protected:
        virtual int _GetNumSpawnedVehicles() override;
        virtual int _GetMaxSpawnedVehicles() override;

    private:
        ai::NumericInRangeRegenerating<float> m_reproductTime;
        int m_maxAttackers;
        int m_maxDefenders;
        ai::Team* _GetAttackingTeam();
        ai::Team* _GetDefendingTeam();
        ai::Lair::LairState m_state;
        void _OnObjectEntersLocation(ai::Event const& evn);
    };
}  // namespace ai
