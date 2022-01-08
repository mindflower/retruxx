#pragma once
#include "settlement.h"
#include <server/components/numericinrangeregenerating.h>

namespace ai
{
    class LairPrototypeInfo : public SettlementPrototypeInfo
    {
    public:
        LairPrototypeInfo();
        virtual Obj* CreateTargetObject() const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        int m_maxAttackers;
        int m_maxDefenders;
    };

    class Lair :  public Settlement
    {
    public:
        enum LairState
        {
            LS_REPRODUCE_WAIT = 0x0,
            LS_REPRODUCE = 0x1,
            LS_DYING = 0x2,
            LS_DEAD = 0x3,
        };

    public:
        Lair(LairPrototypeInfo const &);
        virtual int GetPropertyId(char const *) const ;
        virtual LairPrototypeInfo const * GetPrototypeInfo() const ;
        virtual CStr GetPropertyName(int) const ;
        virtual void CreateChildren();
        static m3d::Class * GetBaseClass();
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        virtual int OnEvent(Event const &);
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        virtual m3d::Class * GetClass() const ;
        static void __fastcall Registration();
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
        virtual void Update(float,unsigned int);

    protected:
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;
        virtual int _GetMaxSpawnedVehicles();
        virtual ~Lair();
        virtual int _GetNumSpawnedVehicles();
        static void __fastcall RegisterProperty(char const *,int,eGObjPropertySaveStatus);
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;

    private:
        virtual m3d::Object * Clone();
        void _OnObjectEntersLocation(Event const &);
        static m3d::Object * CreateObject();
        Team * _GetDefendingTeam();
        Team * _GetAttackingTeam();

    private:
        NumericInRangeRegenerating<float> m_reproductTime;
        int m_maxAttackers;
        int m_maxDefenders;
        LairState m_state;
    };
}
