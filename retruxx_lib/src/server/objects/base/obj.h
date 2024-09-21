#pragma once
#include <core/clazz.h>
#include "thirdparty/stl/map.hpp"
#include "thirdparty/stl/vector.hpp"
#include "thirdparty/stl/list.hpp"
#include "thirdparty/stl/set.hpp"
#include <map>
#include <set>
#include <list>
#include <vector>

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
    protected:
        Obj(const ai::Obj&);
        Obj(const ai::PrototypeInfo& prototypeInfo);
        Obj();
        virtual  ~Obj() override /* 0x00 */;

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetRtClass() const /* 0x3c */;
        static inline m3d::Class m_classObj;
        virtual int OnEvent(const ai::Event& evn) /* 0x40 */;

        struct LessNoCaseCStr : public oldstd::binary_function<CStr, CStr, bool>
        {
            bool operator()(const CStr& a1, const CStr& a2) const
            {
                if (a2.empty())
                    return false;
                if (!a1.empty())
                    return stricmp(a1.c_str(), a2.c_str()) < 0;
                return true;
            }
        }; /* size: 0x0001 */

        enum HierarchyType
        {
            HIERARCHY_CHILD = 0,
            HIERARCHY_COMPONENT = 1,
        };

    public:
        int GetId() const;
        bool IsUpdating() const;
        unsigned int GetFlags() const;
        int GetParentId() const;
        int GetPrototypeId() const;
        void SetParentInvalid();
        oldstd::map<int, ai::Obj*, oldstd::less<int>, oldstd::allocator<oldstd::pair<int const, ai::Obj*> > >& GetChildren();
        void UnlinkFromParent();
        void LinkToParent(int newParentID, ai::Obj::HierarchyType newHierarchyType);
        bool bHasParent() const;
        void SetName(const CStr& str);
        void SetNameFromScript(const CStr& str);
        ai::GeomRepository* GetParentRepository() const;
        void SetParentRepository(ai::GeomRepository* parentRepository);
        virtual ai::AI* GetAIPtr() /* 0x44 */;
        virtual bool NeedCinematicUpdate() /* 0x48 */;
        void AddToCinematic();
        void RemoveFromCinematic();
        virtual const ai::PrototypeInfo* GetPrototypeInfo() const /* 0x4c */;
        void SetTimeOut(float TimeOut);
        void StopTimeOut();
        bool GetPassedToAnotherMapStatus() const;
        virtual void SetPassedToAnotherMapStatus() /* 0x50 */;
        void CreateVisualPart();
        void RefreshAfterPassageToAnotherMap();
        virtual void Remove() /* 0x54 */;
        void SetToMap();
        bool TimeOutFinished();
        int TimeOutActivated() const;
        static void __fastcall Registration();
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const /* 0x58 */;
        virtual void GetPropertiesNames(oldstd::set<CStr, oldstd::less<CStr>, oldstd::allocator<CStr> >& Props) const /* 0x5c */;
        virtual void GetPropertiesIDs(oldstd::set<int, oldstd::less<int>, oldstd::allocator<int> >& Props) const /* 0x60 */;
        virtual int GetProperty(unsigned int propId, void* property) const /* 0x64 */;
        virtual m3d::AIParam GetProperty(const char* PropertyName) const /* 0x64 */;
        virtual m3d::AIParam GetPropertyDefault(const char* PropertyName) const /* 0x68 */;
        virtual m3d::AIParam GetPropertyDefaultById(int propertyId) const /* 0x6c */;
        virtual m3d::AIParam GetPropertyById(int propertyId) const /* 0x70 */;
        virtual int GetPropertyId(const char* PropertyName) const /* 0x74 */;
        virtual CStr GetPropertyName(int id) const /* 0x78 */;
        int SetProperty(unsigned int propId, void* property);
        bool SetProperty(const char* PropertyName, const m3d::AIParam& newValue);
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) /* 0x7c */;
        virtual void Update(float elapsedTime, unsigned int workTime) /* 0x80 */;
        virtual void PostCollide() /* 0x84 */;
        void AddModifier(const ai::Modifier& modifier);
        void AddModifier(const char* PropertyName, const char* modification);
        void Send(ai::Obj* pReceiverObj, const char* PropertyName, const char* modification);
        virtual bool ApplyModifier(const ai::Modifier& modifier) /* 0x88 */;
        bool ApplyAffix(const ai::Affix* affix);
        void ApplyRandomAffixes(int numAffixes);
        bool ApplyAffixByName(const char* affixName);
        virtual void InflictDamage(const ai::DamageInfo& damageInfo) /* 0x8c */;
        CStr GetFullDescriptionWithAffixes() const;
        CStr GetDebugDescription() const;
        void Subscribe(ai::eGameEvent eventId, int objId);
        void Unsubscribe(ai::eGameEvent eventId, int objId);
        void CauseEvent(ai::eGameEvent eventId, float timeOut, m3d::AIParam param1, m3d::AIParam param2) const;
        void ValidateEventRecipientsList();
        virtual ai::Obj* GetChild(int num) const /* 0x90 */;
        virtual int AddChild(m3d::Object* node) /* 0x94 */;
        virtual void AddChild(ai::Obj* pObj) /* 0x94 */;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const /* 0x98 */;
        virtual int RemoveChild(m3d::Object* node) /* 0x9c */;
        virtual bool RemoveChild(ai::Obj* pChild) /* 0x9c */;
        virtual void RemoveComponent(ai::Obj* pComponent) /* 0xa0 */;
        virtual void CreateChildren() /* 0xa4 */;
        virtual void Dump() const /* 0xa8 */;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) /* 0xac */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) /* 0xb0 */;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const /* 0xb4 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const /* 0xb8 */;
        virtual bool bIsEqualToPrototype() const /* 0xbc */;
        void PostLoad();
        virtual void SetBelong(int newBelong) /* 0xc0 */;
        int GetBelong() const;
        virtual ai::Obj* GetParent() const /* 0xc4 */;
        bool IsAlive() const;
        bool bIsEnemyWith(const ai::Obj* pObj) const;
        virtual void SetVisible() /* 0xc8 */;
        virtual void SetInvisible() /* 0xcc */;
        bool bIsVisible() const;
        virtual void StackOpen() /* 0xd0 */;
        virtual void StackClose() /* 0xd4 */;
        virtual void StackLoop() /* 0xd8 */;
        virtual void TransferPhysicParamsToSceneGraphNode() /* 0xdc */;
        virtual void RelinkSceneGraphNode() /* 0xe0 */;
        virtual void RenderDebugInfo() const /* 0xe4 */;
        virtual void ReceiveNodesToLink(oldstd::list<m3d::SgNode*, oldstd::allocator<m3d::SgNode*> >& nodes) const /* 0xe8 */;
        bool GetDeletedStatus() const;
        virtual unsigned int GetPrice(const ai::IPriceCoeffProvider* priceCoeffProvider) const /* 0xec */;
        float GetPriceCoeff(const ai::IPriceCoeffProvider* priceCoeffProvider) const;
        virtual unsigned int GetSchwarz() const /* 0xf0 */;
        virtual ai::Obj* CloneObj() /* 0xf4 */;
        virtual void ClearSavedStatus() /* 0xf8 */;
        bool IsAffixesApplied() const;
        void SetAffixesApplied(bool bApplied);

        using PropertyMapType = oldstd::map<CStr, int, ai::Obj::LessNoCaseCStr, oldstd::allocator<oldstd::pair<CStr const, int> > >;
        using PropertySaveStatesMapType = oldstd::map<int, enum ai::eGObjPropertySaveStatus, oldstd::less<int>, oldstd::allocator<oldstd::pair<int const, enum ai::eGObjPropertySaveStatus> > >;
        class PropertyMapIterator;

    protected:
        static inline PropertyMapType m_propertiesMap;
        static inline PropertySaveStatesMapType m_propertiesSaveStatesMap;
        void _SetDeadStatus();
        bool _GetDeadStatus() const;
        virtual void _InternalPostLoad() /* 0xfc */;
        virtual void _InternalCreateVisualPart() /* 0x100 */;
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus SaveStatus);
        virtual void _SetAllPropertiesToMax() /* 0x104 */;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const /* 0x108 */;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const /* 0x10c */;
        oldstd::map<int, ai::Obj*, oldstd::less<int>, oldstd::allocator<oldstd::pair<int const, ai::Obj*> > >& getAllChildren();
        int GetLastDamageSource() const;
        void SetLastDamageSource(int objId);

    private:
        /* 0x0034 */ int m_objId;
        /* 0x0038 */ int m_updatingObjId;
        /* 0x003c */ bool m_bIsUpdating;
        /* 0x003d */ bool m_bMustBeUpdating;
        /* 0x003e */ char Padding_13[2];
        /* 0x0040 */ unsigned int m_flags;
        /* 0x0044 */ unsigned int m_actionID;
        /* 0x0048 */ unsigned int m_prevActionID;
        /* 0x004c */ float m_timeOut;
        /* 0x0050 */ bool m_bNeedPostLoad;
        /* 0x0051 */ bool m_bMustCreateVisualPart;
        /* 0x0052 */ bool m_bPassedToAnotherMap;
        /* 0x0053 */ char Padding_14;
        /* 0x0054 */ int m_belong;
        /* 0x0058 */ int m_parentId;
        /* 0x005c */ ai::GeomRepository* m_parentRepository;
        /* 0x0060 */ int m_LastDamageSource;
        /* 0x0064 */ bool m_bIsAlreadySaved;
        /* 0x0065 */ char Padding_15[3];
        /* 0x0068 */ ai::Obj::HierarchyType m_hierarchyType;
        /* 0x006c */ int m_prototypeId;
        /* 0x0070 */ oldstd::map<int, ai::Obj*, oldstd::less<int>, oldstd::allocator<oldstd::pair<int const, ai::Obj*> > > m_allChildren;
        /* 0x007c */ oldstd::vector<int, oldstd::allocator<int> > m_appliedPrefixIds;
        /* 0x008c */ oldstd::vector<int, oldstd::allocator<int> > m_appliedSuffixIds;
        /* 0x009c */ bool m_bAffixesWasApplied;
        /* 0x009d */ char Padding_16[3];
        /* 0x00a0 */ oldstd::vector<ai::Modifier, oldstd::allocator<ai::Modifier> > m_modifiers;

        struct EventRecipientInfo
        {
            /* 0x0000 */ ai::eGameEvent m_eventId;
            /* 0x0004 */ oldstd::vector<int> m_objIds;
        }; /* size: 0x0014 */

        using EventRecipientInfoVector = oldstd::vector<ai::Obj::EventRecipientInfo, oldstd::allocator<ai::Obj::EventRecipientInfo> >;

    private:
        /* 0x00b0 */ oldstd::vector<ai::Obj::EventRecipientInfo, oldstd::allocator<ai::Obj::EventRecipientInfo> > m_eventRecipients;
        int _GetIndexByEventId(ai::eGameEvent eventId) const;
        void _Init();
        void OnSubscribe(const ai::Event& evn);
        void OnUnsubscribe(const ai::Event& evn);

    public:
        static m3d::AIParam __fastcall AIGetCmdParam1(ai::Obj* pObj);
        static m3d::AIParam __fastcall AIGetCmdParam2(ai::Obj* pObj);
        static m3d::AIParam __fastcall AIGetCmdParam3(ai::Obj* pObj);
        static m3d::AIParam __fastcall AIGetState2Param1(ai::Obj* pObj);
        static m3d::AIParam __fastcall AIGetState2Param2(ai::Obj* pObj);
        static m3d::AIParam __fastcall AIGetState2Param3(ai::Obj* pObj);
        static m3d::AIParam __fastcall AIGetState1Param1(ai::Obj* pObj);
        static m3d::AIParam __fastcall AIGetState1Param2(ai::Obj* pObj);
        static m3d::AIParam __fastcall AIGetState1Param3(ai::Obj* pObj);
        static m3d::AIParam __fastcall AIGetMessage2Param1(ai::Obj* pObj);
        static m3d::AIParam __fastcall AIGetMessage1Param1(ai::Obj* pObj);
        static m3d::AIParam __fastcall AIGetParentID(ai::Obj* pObj);
        static m3d::AIParam __fastcall AIGetOwnerID(ai::Obj* pObj);
    }; /* size: 0x00c0 */
}
