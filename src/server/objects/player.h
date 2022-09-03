#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"
#include <server/components/numericboundedbelow.h>
#include <server/components/numericinrangeregenerating.h>

namespace ai
{
    class InfoCone;
    class IzvratRepository;
    class Vehicle;
    class RadioManager;

    class PlayerPrototypeInfo : public PrototypeInfo
    {
    public:
        CStr const& GetModelName() const;
        PlayerPrototypeInfo();
        virtual ai::Obj* CreateTargetObject() const;
        unsigned int GetSkinNumber() const;
        unsigned int GetCfgNumber() const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        CStr m_modelName;
        unsigned int m_skinNumber;
        unsigned int m_cfgNumber;

    };

    class Player : public Obj
    {
    public:
        enum PlayerFightState
        {
            FIGHT_CLEAR = 0x0,
            FIGHT_ALARM = 0x1,
            FIGHT_BATTLE = 0x2,
            FIGHT_BATTLE_JUST_FINISHED = 0x3,
            NUM_FIGHT_STATES = 0x4,
        };

    public:
        static m3d::Class * GetBaseClass();
        int GetMoney() const ;
        void ChangeVehicleByExisting(int,bool);
        virtual void Update(float,unsigned int);
        std::vector<CStr,std::allocator<CStr> > const & GetQuestItemPrototypeNames() const ;
        float GetFuel() const ;
        bool HuntQuestIsTaken() const ;
        int RemoveQuestItem(CStr const &);
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
        unsigned int GetCfgNumber() const ;
        CStr const & GetModelName() const ;
        bool CanPlaceItemsToRepository(char const *,int);
        Player(PlayerPrototypeInfo const &);
        Vehicle * GetVehicle() const ;
        virtual int OnEvent(Event const &);
        void AddMoney(int);
        bool IsQuestItemPresent(CStr const &) const ;
        void ChangeVehicleByNew(int,bool);
        virtual int GetPropertyId(char const *) const ;
        IzvratRepository * GetRepository() const ;
        virtual void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        float GetMaxHealth() const ;
        virtual unsigned int GetPrice(IPriceCoeffProvider const *) const ;
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        float GetHealth() const ;
        virtual void AddChild(Obj *);
        float GetMaxFuel() const ;
        bool RemoveItemsFromRepository(char const *,int);
        virtual bool RemoveChild(Obj *);
        virtual PlayerPrototypeInfo const * GetPrototypeInfo() const ;
        bool AddItemsToRepository(char const *,int);
        virtual void SetPassedToAnotherMapStatus();
        virtual void RenderDebugInfo() const ;
        static void __fastcall Registration();
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        virtual unsigned int GetSchwarz() const ;
        virtual m3d::Class * GetClass() const ;
        InfoCone const & GetInfoCone() const ;
        virtual void SaveToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        int AddQuestItem(CStr const &);
        virtual void Remove();
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        bool HasAmountOfItemsInRepository(char const *,int) const ;
        CStr const & GetLastSaveDir() const ;
        void SetLastSaveDir(CStr const &);
        int GetInfoObjId() const ;
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        int GetRadioManagerId() const ;
        NumericBoundedBelow<int> & Money();
        NumericBoundedBelow<int> const & Money() const ;
        virtual CStr GetPropertyName(int) const ;
        virtual bool CanChildBeAdded(m3d::Class *) const ;
        unsigned int GetSkinNumber() const ;

    protected:
        virtual ~Player();
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;
        virtual void _InternalPostLoad();
        static void __fastcall RegisterProperty(char const *,int,eGObjPropertySaveStatus);

    private:
        void _OnDynamicQuestForgotten(Event const &);
        static m3d::Object * CreateObject();
        void _OnDynamicQuestFailed(Event const &);
        void _OnUnderAttack(Event const &);
        virtual m3d::Object * Clone();
        //MemberFunctionOneArg<Player,int,void>::MemberFunctionOneArg<Player,int,void>(Player &,void (*const)(int));
        void _OnDynamicQuestTaken(Event const &);
        void _OnMoneyValueAfterChange(int);
        void _OnNoticeEnemy(Event const &);
        void _OnObjectDie(Event const &);

    private:
        void _OnDynamicQuestComplete(Event const &);

    public:
        RT_CLASS_DECLARE(Player);

    private:
        NumericBoundedBelow<int> m_money;
        //_BYTE gapC0[120];
        int m_vehicleObjId;
        RadioManager *m_radioManager;
        std::vector<CStr> m_questItemPrototypeNames;
        int m_infoObjId;
        InfoCone *m_infoCone;
        float m_timeInfoObjTimeout;
        PlayerFightState m_playerFightState;
        PlayerFightState m_prevPlayerFightState;
        NumericInRangeRegenerating<float> m_timeOfNoBattle;
        CStr m_lastSaveDir;
        CStr m_modelName;
        unsigned int m_skinNumber;
        unsigned int m_cfgNumber;
        bool m_huntQuestIsTaken;
        int m_curNumForVehicleWithoutName;
    };
}
