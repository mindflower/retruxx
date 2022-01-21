#pragma once
#include <game/profile.h>

class HelpManager :  public m3d::Object
{
public:
    enum HelpId
    {
        HELP_ID_MAIN_GAME_INTERFACE = 0x0,
        HELP_ID_INVENTORY = 0x1,
        HELP_ID_VEHICLE_CHARACTERISTICS = 0x2,
        HELP_ID_QUESTLOG_QUESTS = 0x3,
        HELP_ID_QUESTLOG_QUEST_ITEMS = 0x4,
        HELP_ID_LOCAL_MAP = 0x5,
        HELP_ID_GLOBAL_MAP = 0x6,
        HELP_ID_HISTORY = 0x7,
        HELP_ID_BOOKS = 0x8,
        HELP_ID_RELATIONS = 0x9,
        HELP_ID_STATS = 0xA,
        HELP_ID_ENCYCLOPAEDIA = 0xB,
        HELP_ID_SHOP_GUNS = 0xC,
        HELP_ID_SHOP_GOODS = 0xD,
        HELP_ID_WORKSHOP = 0xE,
        HELP_ID_WORKSHOP_CABINS = 0xF,
        HELP_ID_WORKSHOP_BASKETS = 0x10,
        HELP_ID_WORKSHOP_VEHICLE = 0x11,
        HELP_ID_WORKSHOP_SKIN = 0x12,
        HELP_ID_WORKSHOP_REFUEL = 0x13,
        HELP_ID_WORKSHOP_REPAIR = 0x14,
        HELP_ID_WORKSHOP_RECHARGE = 0x15,
        HELP_ID_BUY_CABIN = 0x16,
        HELP_ID_BUY_BASKET = 0x17,
        HELP_ID_BUY_VEHICLE = 0x18,
        HELP_ID_TOWN = 0x19,
        HELP_ID_BAR = 0x1A,
        HELP_ID_TALK_WITH_NPC = 0x1B,
        HELP_ID_RADAR = 0x1C,
        HELP_ID_SPEEDOMETER = 0x1D,
        HELP_ID_HP = 0x1E,
        HELP_ID_FUEL = 0x1F,
        HELP_ID_DURABILITY = 0x20,
        HELP_ID_TIMER = 0x21,
        HELP_ID_WEAPON = 0x22,
        HELP_ID_FADING_MESSAGES = 0x23,
        HELP_ID_IMPORTANT_FADING_MESSAGES = 0x24,
        HELP_ID_TARGET_INFO = 0x25,
        HELP_ID_TARGET_CAPTURE = 0x26,
        HELP_ID_ACTION_ICONS = 0x27,
        HELP_ID_HOW_TO_USE_MAIN_GAME_INTERFACE_HELP = 0x28,
        NUM_HELP_IDS = 0x29,
        HELP_ID_INVALID = 0x29,
    };

    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_strTitleSuffix;
    };

public:
    CStr GetHelpMsgByHelpId(CStr const &) const ;
    static CStr __fastcall Enumered2StrHelpId(HelpId);
    void ShowHelp(CStr const &,bool);
    static m3d::Object * CreateObject();
    static HelpId __fastcall Str2EnumeredHelpId(CStr const &);
    void Clear();
    virtual m3d::Class * GetClass() const ;
    void ResetAutoHelp();
    static m3d::Class * GetBaseClass();
    void EnableAutoHelp(bool);
    virtual m3d::Object * Clone();
    CStr GetHelpTitleByHelpId(CStr const &) const ;
    virtual ~HelpManager();
    bool IsAutoHelpEnabled() const ;
    int GameDataUpdate(void *,int);

protected:
    HelpManager();
    HelpManager(HelpManager const &);
    void ShowHelpEnumered(HelpId,bool);
    void HideCurrentHelpWindow();
    void MarkHelpAsShown(HelpId);
    ProfileParam GetProfileParamIdByHelpId(HelpId) const ;
    int GetPredefinedWndGuiIdByHelpId(HelpId) const ;
    int GetCurHelpWndGuiId() const ;
    void GetAllHelpWndGuiIds(std::vector<int> &) const ;
    void OnHelpImpulse();
    HelpId CalcHelpIdForCurrentInterfaceState() const ;
    bool IsHelpIdValidForCurrentInterfaceState(HelpId) const ;
    bool WasHelpShown(HelpId) const ;
    void OnHelpEvent(void *);

public:
    RT_CLASS_DECLARE(HelpManager);

private:
    std::map<HelpId,int> m_predefinedHelpWindows;
    AuxInfo m_aif;
};
