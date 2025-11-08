#pragma once
#include <game/profile.h>

class HelpManager : public m3d::Object
{
    friend class TruxxUiManager;
public:
    enum HelpId
    {
        HELP_ID_MAIN_GAME_INTERFACE = 0,
        HELP_ID_INVENTORY = 1,
        HELP_ID_VEHICLE_CHARACTERISTICS = 2,
        HELP_ID_QUESTLOG_QUESTS = 3,
        HELP_ID_QUESTLOG_QUEST_ITEMS = 4,
        HELP_ID_LOCAL_MAP = 5,
        HELP_ID_GLOBAL_MAP = 6,
        HELP_ID_HISTORY = 7,
        HELP_ID_BOOKS = 8,
        HELP_ID_RELATIONS = 9,
        HELP_ID_STATS = 10,
        HELP_ID_ENCYCLOPAEDIA = 11,
        HELP_ID_SHOP_GUNS = 12,
        HELP_ID_SHOP_GOODS = 13,
        HELP_ID_WORKSHOP = 14,
        HELP_ID_WORKSHOP_CABINS = 15,
        HELP_ID_WORKSHOP_BASKETS = 16,
        HELP_ID_WORKSHOP_VEHICLE = 17,
        HELP_ID_WORKSHOP_SKIN = 18,
        HELP_ID_WORKSHOP_REFUEL = 19,
        HELP_ID_WORKSHOP_REPAIR = 20,
        HELP_ID_WORKSHOP_RECHARGE = 21,
        HELP_ID_BUY_CABIN = 22,
        HELP_ID_BUY_BASKET = 23,
        HELP_ID_BUY_VEHICLE = 24,
        HELP_ID_TOWN = 25,
        HELP_ID_BAR = 26,
        HELP_ID_TALK_WITH_NPC = 27,
        HELP_ID_RADAR = 28,
        HELP_ID_SPEEDOMETER = 29,
        HELP_ID_HP = 30,
        HELP_ID_FUEL = 31,
        HELP_ID_DURABILITY = 32,
        HELP_ID_TIMER = 33,
        HELP_ID_WEAPON = 34,
        HELP_ID_FADING_MESSAGES = 35,
        HELP_ID_IMPORTANT_FADING_MESSAGES = 36,
        HELP_ID_TARGET_INFO = 37,
        HELP_ID_TARGET_CAPTURE = 38,
        HELP_ID_ACTION_ICONS = 39,
        HELP_ID_HOW_TO_USE_MAIN_GAME_INTERFACE_HELP = 40,
        NUM_HELP_IDS = 41,
        HELP_ID_INVALID = 41,
    };

public:
    void ShowHelp(const CStr& strHelpId, bool bAllowDisableAutoHelp);
    int GameDataUpdate(void* data, int dataType);
    void Clear();
    void EnableAutoHelp(bool bEnable);
    bool IsAutoHelpEnabled() const;
    CStr GetHelpMsgByHelpId(const CStr& strHelpId) const;
    CStr GetHelpTitleByHelpId(const CStr& strHelpId) const;
    static HelpManager::HelpId __fastcall Str2EnumeredHelpId(const CStr& strHelpId);
    static CStr __fastcall Enumered2StrHelpId(HelpManager::HelpId helpId);
    void ResetAutoHelp();

    using PredefinedHelpWindowsMap = std::map<enum HelpManager::HelpId, int, std::less<enum HelpManager::HelpId>, std::allocator<std::pair<enum HelpManager::HelpId const, int> > >;
    using PredefinedHelpWindowsPair = std::pair<enum HelpManager::HelpId, int>;
    struct AuxInfo
    {
        /* 0x0000 */ CStr m_strTitleSuffix;
        AuxInfo(const HelpManager::AuxInfo&);
        AuxInfo();
    }; /* size: 0x000c */

protected:
    void ShowHelpEnumered(HelpManager::HelpId helpId, bool bAllowDisableAutoHelp);
    int GetPredefinedWndGuiIdByHelpId(HelpManager::HelpId helpId) const;
    void OnHelpImpulse();
    void OnHelpEvent(void* data);
    HelpManager::HelpId CalcHelpIdForCurrentInterfaceState() const;
    bool IsHelpIdValidForCurrentInterfaceState(HelpManager::HelpId helpId) const;
    void HideCurrentHelpWindow();
    int GetCurHelpWndGuiId() const;
    void GetAllHelpWndGuiIds(std::vector<int, std::allocator<int> >& guiIds) const;
    bool WasHelpShown(HelpManager::HelpId helpId) const;
    ProfileParam GetProfileParamIdByHelpId(HelpManager::HelpId helpId) const;
    void MarkHelpAsShown(HelpManager::HelpId helpId);
    /* 0x0034 */ std::map<enum HelpManager::HelpId, int, std::less<enum HelpManager::HelpId>, std::allocator<std::pair<enum HelpManager::HelpId const, int> > > m_predefinedHelpWindows;
    /* 0x0040 */ HelpManager::AuxInfo m_aif;
    HelpManager();
    HelpManager(const HelpManager& rhs);

public:
    virtual  ~HelpManager() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classHelpManager;
}; /* size: 0x004c */
