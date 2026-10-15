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

    class PlayerPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        PlayerPrototypeInfo();
        virtual ai::Obj* CreateTargetObject() const override;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        CStr const& GetModelName() const;
        unsigned int GetSkinNumber() const;
        unsigned int GetCfgNumber() const;

    private:
        CStr m_modelName;
        unsigned int m_skinNumber;
        unsigned int m_cfgNumber;
    };

    class Player : public Obj
    {
        friend class PlayerPrototypeInfo;

    protected:
        virtual ~Player() override;

    private:
        Player(ai::PlayerPrototypeInfo const& prototypeInfo);
        Player(ai::Player const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classPlayer;
        virtual ai::PlayerPrototypeInfo const* GetPrototypeInfo() const override;

    protected:
        static void RegisterProperty(char const* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override;
        virtual void GetPropertiesNames(retruxx::set<CStr>& Props) const override;
        virtual void GetPropertiesIDs(retruxx::set<int>& Props) const override;
        virtual CStr GetPropertyName(int id) const override;
        virtual bool SetPropertyById(int propertyId, m3d::AIParam const& newValue) override;
        virtual int GetPropertyId(char const* PropertyName) const override;

    protected:
        static inline retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr> m_propertiesMap;
        static inline retruxx::map<int, enum ai::eGObjPropertySaveStatus> m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override;

    public:
        virtual int OnEvent(ai::Event const& evn) override;

        enum PlayerFightState
        {
            FIGHT_CLEAR = 0,
            FIGHT_ALARM = 1,
            FIGHT_BATTLE = 2,
            FIGHT_BATTLE_JUST_FINISHED = 3,
            NUM_FIGHT_STATES = 4,
        };

    public:
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual void AddChild(ai::Obj* pObj) override;
        virtual bool RemoveChild(ai::Obj* pChild) override;
        virtual void Remove() override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        virtual void SetPassedToAnotherMapStatus() override;
        ai::Vehicle* GetVehicle() const;
        void ChangeVehicleByNew(int newVehiclePrototypeId, bool bDeleteOldVehicle);
        void ChangeVehicleByExisting(int newVehicleObjId, bool bDeleteOldVehicle);
        ai::NumericBoundedBelow<int> const& Money() const;
        ai::NumericBoundedBelow<int>& Money();
        int GetMoney() const;
        void AddMoney(int amount);
        float GetHealth() const;
        float GetMaxHealth() const;
        float GetFuel() const;
        float GetMaxFuel() const;
        virtual unsigned int GetPrice(ai::IPriceCoeffProvider const* priceCoeffProvider) const override;
        virtual unsigned int GetSchwarz() const override;
        bool AddItemsToRepository(char const* prototypeName, int amount);
        bool RemoveItemsFromRepository(char const* prototypeName, int amount);
        bool HasAmountOfItemsInRepository(char const* prototypeName, int amount) const;
        bool CanPlaceItemsToRepository(char const* prototypeName, int amount);
        ai::IzvratRepository* GetRepository() const;
        CStr const& GetLastSaveDir() const;
        void SetLastSaveDir(CStr const& saveDir);
        static void Registration();
        bool IsQuestItemPresent(CStr const& itemPrototypeName) const;
        int AddQuestItem(CStr const& itemPrototypeName);
        int RemoveQuestItem(CStr const& itemPrototypeName);
        retruxx::vector<CStr> const& GetQuestItemPrototypeNames() const;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        ai::InfoCone const& GetInfoCone() const;
        int GetInfoObjId() const;
        int GetRadioManagerId() const;
        CStr const& GetModelName() const;
        unsigned int GetSkinNumber() const;
        unsigned int GetCfgNumber() const;
        bool HuntQuestIsTaken() const;
        virtual void RenderDebugInfo() const override;

    protected:
        virtual void _InternalPostLoad() override;

        using AfterChangeFloatCallback = ai::MemberFunctionOneArg<ai::Player, int, void>;

    private:
        void _OnMoneyValueAfterChange(int oldMoneyValue);
        ai::NumericBoundedBelow<int> m_money;
        int m_vehicleObjId;
        ai::RadioManager* m_radioManager;
        retruxx::vector<CStr> m_questItemPrototypeNames;
        int m_infoObjId;
        ai::InfoCone* m_infoCone;
        float m_timeInfoObjTimeout;
        ai::Player::PlayerFightState m_playerFightState;
        ai::Player::PlayerFightState m_prevPlayerFightState;
        ai::NumericInRangeRegenerating<float> m_timeOfNoBattle;
        CStr m_lastSaveDir;
        CStr m_modelName;
        unsigned int m_skinNumber;
        unsigned int m_cfgNumber;
        bool m_huntQuestIsTaken;
        int m_curNumForVehicleWithoutName;
        void _OnObjectDie(ai::Event const& evn);
        void _OnNoticeEnemy(ai::Event const& evn);
        void _OnUnderAttack(ai::Event const& evn);
        void _OnDynamicQuestTaken(ai::Event const& evn);
        void _OnDynamicQuestComplete(ai::Event const& evn);
        void _OnDynamicQuestForgotten(ai::Event const& evn);
        void _OnDynamicQuestFailed(ai::Event const& evn);
    };

    inline Player* thePlayer = nullptr;
}  // namespace ai
