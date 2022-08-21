#pragma once
#include <list>
#include <map>
#include <vector>
#include <core/clazz.h>

namespace m3d
{
    class SgNode;
    class AIParam;
}

namespace ai
{
    class Affix;
    class PrototypeInfo;
    class AI;
    class DamageInfo;
    class IPriceCoeffProvider;
    class Modifier;
    class GeomRepository;
    class Event;

    enum eGameEvent
    {
        GE_UNKNOWN = 0x0,
        GE_GAME_START = 0x1,
        GE_SUBSCRIBE = 0x2,
        GE_UNSUBSCRIBE = 0x3,
        GE_OBJECT_ENTERS_LOCATION = 0x4,
        GE_OBJECT_LEAVES_LOCATION = 0x5,
        GE_OBJECT_IN_LOCATION = 0x6,
        GE_OBJECT_ACTIVATED = 0x7,
        GE_OBJECT_DEACTIVATED = 0x8,
        GE_OBJECT_DIE = 0x9,
        GE_OBJECT_DIE_SENSE = 0xA,
        GE_END_TTL = 0xB,
        GE_END_ANIMATION = 0xC,
        GE_TIME_PERIOD = 0xD,
        GE_FRAMES_PASSED = 0xE,
        GE_FREE = 0xF,
        GE_ENEMY_LOST = 0x10,
        GE_ENEMY_DESTROYED = 0x11,
        GE_TUTORIAL_VEHICLE = 0x12,
        GE_TUTORIAL_INVENTORY = 0x13,
        GE_TUTORIAL_TOWN = 0x14,
        GE_TUTORIAL_BAR = 0x15,
        GE_TUTORIAL_WORKSHOP = 0x16,
        GE_TUTORIAL_SHOP = 0x17,
        GE_TUTORIAL_QUESTLOG = 0x18,
        GE_TUTORIAL_MAP = 0x19,
        GE_TUTORIAL_STATS = 0x1A,
        GE_TUTORIAL_HISTORY = 0x1B,
        GE_TUTORIAL_BOOKS = 0x1C,
        GE_TUTORIAL_ENCYCLOPAEDIA = 0x1D,
        GE_TUTORIAL_RELATIONS = 0x1E,
        GE_TUTORIAL_JOURNAL = 0x1F,
        GE_TUTORIAL_TAB_GOODS = 0x20,
        GE_TUTORIAL_TAB_WEAPON = 0x21,
        GE_TUTORIAL_CABINS = 0x22,
        GE_TUTORIAL_CABIN_SELECT = 0x23,
        GE_TUTORIAL_BASKETS = 0x24,
        GE_TUTORIAL_BASKET_SELECT = 0x25,
        GE_TUTORIAL_NEW_VEHICLE = 0x26,
        GE_TUTORIAL_NEW_VEHICLE_SELECT = 0x27,
        GE_TUTORIAL_SKIN = 0x28,
        GE_TUTORIAL_REFUEL = 0x29,
        GE_TUTORIAL_REPAIR = 0x2A,
        GE_TUTORIAL_RECHARGE = 0x2B,
        GE_PART_BROKEN = 0x2C,
        GE_RELATION_CHANGED = 0x2D,
        GE_NOTICE_SOMEONE = 0x2E,
        GE_UNDER_ATTACK = 0x2F,
        GE_TEAM_NEEDS_REINFORCEMENT = 0x30,
        GE_LOST_GUARDS_NEED_DIRECTION = 0x31,
        GE_NOTICE_ENEMY = 0x32,
        GE_TARGET_REACHED = 0x33,
        GE_TARGET_UNREACHED = 0x34,
        GE_OBJECT_ENTERS_OBJECT = 0x35,
        GE_OBJECT_LEAVES_OBJECT = 0x36,
        GE_TALK_WITH_OBJECT = 0x37,
        GE_SKIP_CINEMATIC = 0x38,
        GE_END_CINEMATIC = 0x39,
        GE_START_CINEMATIC_MSG = 0x3A,
        GE_START_CINEMATIC_FLY = 0x3B,
        GE_IN_CINEMATIC = 0x3C,
        GE_CINEMATIC_ENTER_FADE_IN = 0x3D,
        GE_CUSTOM_GUN_POINTED = 0x3E,
        GE_CUSTOM_GUN_DISPOINTED = 0x3F,
        GE_PLAYER_VEHICLE_HORN = 0x40,
        GE_PLAYER_VEHICLE_CHANGED = 0x41,
        GE_VEHICLE_WITHOUT_HEALTH = 0x42,
        GE_BOSS_CRITICAL_LOADS_EXPLODED = 0x43,
        GE_BOSS_ARM_ACTION_FINISHED = 0x44,
        GE_BOSS04_STATION_DESTROYED = 0x45,
        GE_LEAVE_TOWN = 0x46,
        GE_TOWN_CONDITIONAL_CLOSING = 0x47,
        GE_DYNAMIC_QUEST_TAKEN = 0x48,
        GE_DYNAMIC_QUEST_COMPLETE = 0x49,
        GE_DYNAMIC_QUEST_FORGOTTEN = 0x4A,
        GE_DYNAMIC_QUEST_FAILED = 0x4B,
        GE_NUM_EVENTS = 0x4C,
    };

    enum eGObjProperties
    {
        PROPERTY_BELONG = 0x0,
        PROPERTY_PROTOTYPE_NAME = 0x1,
        PROPERTY_PROTOTYPE_ID = 0x2,
        PROPERTY_NAME = 0x3,
        PROPERTY_POSITION = 0x4,
        PROPERTY_ROTATION = 0x5,
        PROPERTY_MASS = 0x6,
        PROPERTY_NODE_SCALE = 0x7,
        PROPERTY_AICONTROLLED = 0x8,
        PROPERTY_FUEL = 0x9,
        PROPERTY_MAX_FUEL = 0xA,
        PROPERTY_MONEY = 0xB,
        PROPERTY_DRIFT_COEFF = 0xC,
        PROPERTY_GADGET_ANTI_MISSILE_RADIUS = 0xD,
        PROPERTY_REMOVE_WHEN_CHILDREN_DEAD = 0xE,
        PROPERTY_TEAMTACTIC_PROTOTYPE = 0xF,
        PROPERTY_TEAMTACTIC_SHOULD_BE_ASSIGNED = 0x10,
        PROPERTY_PART_TYPE = 0x11,
        PROPERTY_ARMOR = 0x12,
        PROPERTY_DURABILITY = 0x13,
        PROPERTY_MAX_DURABILITY = 0x14,
        PROPERTY_PRICE = 0x15,
        PROPERTY_MAX_TORQUE = 0x16,
        PROPERTY_MAX_SPEED = 0x17,
        PROPERTY_FUEL_CONSUMPTION = 0x18,
        PROPERTY_CONTROL = 0x19,
        PROPERTY_HEALTH = 0x1A,
        PROPERTY_MAX_HEALTH = 0x1B,
        PROPERTY_DAMAGE = 0x1C,
        PROPERTY_FIRING_RATE = 0x1D,
        PROPERTY_FIRING_RANGE = 0x1E,
        PROPERTY_ACCURACY = 0x1F,
        PROPERTY_WITH_CHARGING = 0x20,
        PROPERTY_WITH_CHARGE_LIMIT = 0x21,
        PROPERTY_CHARGE_SIZE = 0x22,
        PROPERTY_RECHARGING_TIME = 0x23,
        PROPERTY_SHELLS_IN_POOL = 0x24,
        PROPERTY_CHARGES = 0x25,
        PROPERTY_CHARGE_STATE = 0x26,
        PROPERTY_CURRENT_CHARGE_TIME = 0x27,
        PROPERTY_ITEMS_IN_CURRENT_CHARGE = 0x28,
        PROPERTY_SLOT_NUM = 0x29,
        PROPERTY_GIVING_QUESTS = 0x2A,
        PROPERTY_DISCUSSING_QUESTS = 0x2B,
        PROPERTY_MODEL_NAME = 0x2C,
        PROPERTY_MODEL_SKIN = 0x2D,
        PROPERTY_MODEL_CONFIGURATION = 0x2E,
        PROPERTY_HELLO_REPLIES = 0x2F,
        PROPERTY_NPC_TYPE = 0x30,
        PROPERTY_RADIUS = 0x31,
        PROPERTY_TARGET_CLASSES = 0x32,
        PROPERTY_TOLERANCE = 0x33,
        PROPERTY_LOOKINGTIMEOUT = 0x34,
        PROPERTY_ACTIVE = 0x35,
        PROPERTY_PASSAGE_ADDRESS = 0x36,
        PROPERTY_CORRESPONDING_PASSAGE_LOCATION_NAME = 0x37,
        PROPERTY_PASSAGE_ACTIVE = 0x38,
        PROPERTY_TARGET_NAME = 0x39,
        PROPERTY_HIRER_NAME = 0x3A,
        PROPERTY_QUEST_STATUS = 0x3B,
        PROPERTY_REWARD = 0x3C,
        PROPERTY_HUNT_FRAGS_AT_START = 0x3D,
        PROPERTY_HUNT_START_TIME = 0x3E,
        PROPERTY_MAX_ITEMS = 0x3F,
        PROPERTY_MAX_DEFENDERS = 0x40,
        PROPERTY_OPEN_GATE_TO_PLAYER = 0x41,
        PROPERTY_MAX_ATTACKERS = 0x42,
        PROPERTY_PROBABILITY = 0x43,
        PROPERTY_WAVE_FORCE_INTENSITY = 0x44,
        PROPERTY_WAVE_DAMAGE_INTENSITY = 0x45,
        PROPERTY_MIN_DIST_TO_PLAYER = 0x46,
        PROPERTY_CRITICAL_TEAM_DIST = 0x47,
        PROPERTY_CRITICAL_TEAM_TIME = 0x48,
        PROPERTY_INFECTION_TEAM_PROTOTYPE_NAME = 0x49,
        PROPERTY_DROPOUT_TIMEOUT = 0x4A,
        PROPERTY_PLACE_POSITION = 0x4B,
        PROPERTY_PORT_POSITION = 0x4C,
        PROPERTY_PATHS_NAMES = 0x4D,
        PROPERTY_CLASS_NAME = 0x4E,
    };

    enum eGObjPropertySaveStatus
    {
        SAVE_PROP_NORMAL = 0x0,
        SAVE_PROP_ALWAYS = 0x1,
        SAVE_PROP_NEVER = 0x2,
        SAVE_PROP_SPECIAL = 0x3,
    };

    class Obj : public m3d::Object
    {
    public:
        struct EventRecipientInfo
        {
            eGameEvent m_eventId;
            std::vector<int> m_objIds;
        };

        enum HierarchyType
        {
            HIERARCHY_CHILD = 0x0,
            HIERARCHY_COMPONENT = 0x1,
        };

    public:
        static m3d::Class* GetBaseClass();
        static void Registration();
        static m3d::AIParam AIGetOwnerID(Obj* obj);
        static m3d::AIParam AIGetParentID(Obj* obj);
        static m3d::AIParam AIGetCmdParam1(Obj* obj);
        static m3d::AIParam AIGetCmdParam2(Obj* obj);
        static m3d::AIParam AIGetCmdParam3(Obj* obj);
        static m3d::AIParam AIGetMessage1Param1(Obj* obj);
        static m3d::AIParam AIGetMessage2Param1(Obj* obj);
        static m3d::AIParam AIGetState1Param1(Obj* obj);
        static m3d::AIParam AIGetState1Param2(Obj* obj);
        static m3d::AIParam AIGetState1Param3(Obj* obj);
        static m3d::AIParam AIGetState2Param1(Obj* obj);
        static m3d::AIParam AIGetState2Param2(Obj* obj);
        static m3d::AIParam AIGetState2Param3(Obj* obj);

    protected:
        static void RegisterProperty(char const* name, int id, eGObjPropertySaveStatus saveStatus);

    protected:
        static std::map<CStr, int> m_propertiesMap;
        static std::map<int, eGObjPropertySaveStatus> m_propertiesSaveStatesMap;

    public:
        virtual void RemoveComponent(Obj* component);
        virtual void LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual void SetBelong(int newBelong);
        virtual Obj* CloneObj();
        virtual int OnEvent(Event const& ev);
        virtual bool CanChildBeAdded(m3d::Class* pClass) const;
        virtual m3d::Class* GetRtClass() const;
        virtual void PostCollide();
        virtual void SetVisible();
        virtual void RenderDebugInfo() const;
        virtual void TransferPhysicParamsToSceneGraphNode();
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual bool RemoveChild(Obj*);
        virtual int RemoveChild(m3d::Object*);
        virtual bool ApplyModifier(Modifier const& modifier);
        virtual void GetPropertiesIDs(std::set<int>& props) const;
        virtual void SetInvisible();
        virtual void InflictDamage(DamageInfo const& damageInfo);
        virtual void Update(float elapsedTime, unsigned int workTime);
        virtual void GetPropertiesNames(std::set<CStr>& props) const;
        virtual void RelinkSceneGraphNode();
        virtual AI* GetAIPtr();
        virtual CStr GetPropertyName(int id) const;
        virtual PrototypeInfo const* GetPrototypeInfo() const;
        virtual void CreateChildren();
        virtual void ReceiveNodesToLink(std::list<m3d::SgNode*>&) const;
        virtual bool bIsEqualToPrototype() const;
        virtual void Dump() const;
        virtual unsigned int GetPrice(IPriceCoeffProvider const*) const;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        virtual void AddChild(Obj*);
        virtual int AddChild(m3d::Object*);
        virtual m3d::AIParam GetPropertyDefaultById(int) const;
        virtual bool SetPropertyById(int, m3d::AIParam const&);
        virtual int GetProperty(unsigned int, void*) const;
        virtual m3d::AIParam GetProperty(char const*) const;
        virtual Obj* GetParent() const;
        virtual void Remove();
        virtual bool NeedCinematicUpdate();
        virtual m3d::AIParam GetPropertyDefault(char const*) const;
        virtual Obj* GetChild(int) const;
        virtual void ClearSavedStatus();
        virtual void StackClose();
        virtual unsigned int GetSchwarz() const;
        virtual void SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const;
        virtual void StackOpen();
        virtual void SetPassedToAnotherMapStatus();
        virtual m3d::AIParam GetPropertyById(int) const;
        virtual int SetProperty(unsigned int, void*);
        virtual int GetPropertyId(char const*) const;
        virtual void StackLoop();

        Obj(PrototypeInfo const& prototypeInfo);
        void SetParentRepository(GeomRepository* parentRepository);
        float GetPriceCoeff(IPriceCoeffProvider const* priceCoeffProvider) const;
        void CauseEvent(eGameEvent eventId, float timeOut, m3d::AIParam param1, m3d::AIParam param2) const;
        CStr GetFullDescriptionWithAffixes() const;
        bool bIsEnemyWith(Obj const *) const;
        CStr GetDebugDescription() const;
        void Subscribe(eGameEvent eventId, int objId);
        void Unsubscribe(eGameEvent eventId, int objId);
        GeomRepository* GetParentRepository() const;
        void PostLoad();
        bool TimeOutFinished();
        void LinkToParent(int newParentId, HierarchyType newHierarchyType);
        void AddModifier(Modifier const& modifier);
        void AddModifier(char const* propertyName, char const* modification);
        void ValidateEventRecipientsList();
        bool ApplyAffixByName(char const *);
        bool IsAlive() const;
        void CreateVisualPart();
        void Send(Obj* receiverObj ,char const* propertyName, char const* modification);
        bool ApplyAffix(Affix const*);

        //No need to impl
        void ApplyRandomAffixes(int);
        unsigned int GetFlags() const;
        bool bIsVisible() const;
        bool bHasParent() const;
        bool IsAffixesApplied() const;
        int GetId() const;
        int TimeOutActivated() const;
        int GetBelong() const;
        void RemoveFromCinematic();
        void SetTimeOut(float);
        int GetParentId() const;
        void SetAffixesApplied(bool);
        bool GetDeletedStatus() const;
        int GetPrototypeId() const;
        std::map<int, Obj*>& GetChildren();
        void SetName(CStr const&);
        void UnlinkFromParent();
        void SetParentInvalid();
        void SetNameFromScript(CStr const&);
        bool IsUpdating() const;
        bool GetPassedToAnotherMapStatus() const;
        void AddToCinematic();
        void StopTimeOut();
        bool SetProperty(char const*, m3d::AIParam const&);

    protected:
        virtual void _SetAllPropertiesToMax();
        std::map<int,Obj *> & getAllChildren();
        Obj();
        bool _GetDeadStatus() const;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const;
        int GetLastDamageSource() const;
        void _SetDeadStatus();
        virtual void _InternalPostLoad();
        void SetLastDamageSource(int);
        virtual void _InternalCreateVisualPart();
        virtual ~Obj();
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const;

    private:
        void OnSubscribe(Event const& evn);
        void _Init();
        void OnUnsubscribe(Event const &);
        int _GetIndexByEventId(eGameEvent eventId) const;

    public:
        RT_CLASS_DECLARE(Obj);

    private:
        int m_objId;
        int m_updatingObjId;
        bool m_bIsUpdating;
        bool m_bMustBeUpdating;
        unsigned int m_flags;
        unsigned int m_actionID;
        unsigned int m_prevActionID;
        float m_timeOut;
        bool m_bNeedPostLoad;
        bool m_bMustCreateVisualPart;
        bool m_bPassedToAnotherMap;
        int m_belong;
        int m_parentId;
        GeomRepository *m_parentRepository;
        int m_LastDamageSource;
        bool m_bIsAlreadySaved;
        Obj::HierarchyType m_hierarchyType;
        int m_prototypeId;
        std::map<int,Obj *> m_allChildren;
        std::vector<int> m_appliedPrefixIds;
        std::vector<int> m_appliedSuffixIds;
        bool m_bAffixesWasApplied;
        std::vector<Modifier> m_modifiers;
        std::vector<EventRecipientInfo> m_eventRecipients;
    };
}
