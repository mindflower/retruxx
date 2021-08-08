#pragma once
#include <core/clazz.h>

namespace m3d
{
    class AIParam;
}

enum ProfileParam
{
    PP_SAVE_SORT_ARG = 0x0,
    PP_SAVE_SORT_DIR = 0x1,
    PP_NEXT_SAVE_FOLDER_NUM = 0x2,
    PP_QUESTS_FILTER = 0x3,
    PP_MOUSE_SENSITIVITY = 0x4,
    PP_MOUSE_YAXIS_FLIP = 0x5,
    PP_MOUSE_XAXIS_FLIP = 0x6,
    PP_WARELIST_MODE = 0x7,
    PP_INPUT_LANGUAGE = 0x8,
    PP_NUM_RADIO_REPLIES_COEFF = 0x9,
    PP_MINIMAP_ENABLE_OBJECT_NAMES = 0xA,
    PP_MOTION_BLUR = 0xB,
    PP_MOTION_BLUR_ALPHA = 0xC,
    PP_BLOOM = 0xD,
    PP_AUTOHELP_ENABLED = 0xE,
    PP_DIFFICULTY_LEVEL = 0xF,
    PP_HELP_ID_MAIN_GAME_INTERFACE = 0x10,
    PP_HELP_ID_INVENTORY = 0x11,
    PP_HELP_ID_VEHICLE_CHARACTERISTICS = 0x12,
    PP_HELP_ID_QUESTLOG_QUESTS = 0x13,
    PP_HELP_ID_QUESTLOG_QUEST_ITEMS = 0x14,
    PP_HELP_ID_LOCAL_MAP = 0x15,
    PP_HELP_ID_GLOBAL_MAP = 0x16,
    PP_HELP_ID_HISTORY = 0x17,
    PP_HELP_ID_BOOKS = 0x18,
    PP_HELP_ID_RELATIONS = 0x19,
    PP_HELP_ID_STATS = 0x1A,
    PP_HELP_ID_ENCYCLOPAEDIA = 0x1B,
    PP_HELP_ID_SHOP_GUNS = 0x1C,
    PP_HELP_ID_SHOP_GOODS = 0x1D,
    PP_HELP_ID_WORKSHOP = 0x1E,
    PP_HELP_ID_WORKSHOP_CABINS = 0x1F,
    PP_HELP_ID_WORKSHOP_BASKETS = 0x20,
    PP_HELP_ID_WORKSHOP_VEHICLE = 0x21,
    PP_HELP_ID_WORKSHOP_SKIN = 0x22,
    PP_HELP_ID_WORKSHOP_REFUEL = 0x23,
    PP_HELP_ID_WORKSHOP_REPAIR = 0x24,
    PP_HELP_ID_WORKSHOP_RECHARGE = 0x25,
    PP_HELP_ID_BUY_CABIN = 0x26,
    PP_HELP_ID_BUY_BASKET = 0x27,
    PP_HELP_ID_BUY_VEHICLE = 0x28,
    PP_HELP_ID_TOWN = 0x29,
    PP_HELP_ID_BAR = 0x2A,
    PP_HELP_ID_TALK_WITH_NPC = 0x2B,
    PP_NUM_PROFILE_PARAMS = 0x2C,
    PP_INVALID = 0x2C,
};

class Profile : public m3d::Object
{
public:
    static m3d::Class m_classProfile;

public:
    int LoadFromXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
    CStr const & GetFolder() const ;
    virtual m3d::Class* GetClass() const ;
    static m3d::Object * __fastcall CreateObject();
    void SetFolder(CStr const &);
    virtual m3d::Object * Clone();
    int GetParam(ProfileParam, m3d::AIParam &) const ;
    static m3d::Class* __fastcall GetBaseClass();
    void SetName(CStr const &);
    int SaveToXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
    bool IsValid() const ;
    int SetParam(ProfileParam, m3d::AIParam const &);
    virtual ~Profile();
    CStr const & GetName() const ;

protected:
    int SetParamsDefault();
    void Clear();
    Profile(Profile const &);
    Profile();
    ProfileParam ParamName2Id(CStr const &) const ;
    CStr ParamId2Name(ProfileParam) const ;

private:
    CStr m_name;
    CStr m_folder;
    std::map<ProfileParam,m3d::AIParam> m_params;
};

class ProfileManager :  public m3d::Object
{
public:
    static m3d::Class m_classProfileManager;

public:
    Profile * GetCurProfile() const ;
    int SetCurProfile(CStr const &);
    CStr GetDefaultProfileName() const ;
    virtual m3d::Class* GetClass() const ;
    int DeleteProfile(CStr const &);
    std::vector<CStr> GetProfilesNames() const ;
    int Done();
    int Init();
    Profile const * CreateNewProfile(CStr const &);
    static m3d::Class* __fastcall GetBaseClass();
    virtual ~ProfileManager();
    static m3d::Object * __fastcall CreateObject();
    int SaveProfile(Profile const *) const ;
    int LoadProfiles();
    Profile * GetProfileByName(CStr const &) const ;
    virtual m3d::Object * Clone();

protected:
    void Clear();
    CStr GetProfileOwnFolderName(CStr const &) const ;
    Profile const * CreateDefaultProfile();
    int AddProfile(Profile *);
    ProfileManager(ProfileManager const &);
    ProfileManager();
    int GetProfileFiles(std::vector<CStr,std::allocator<CStr> > &) const ;
    CStr GetProfileFolderName(CStr const &) const ;
    CStr GetProfileFilePath(CStr const &) const ;
    Profile* _GetProfileByName(CStr const &) const ;

protected:
    CStr m_curProfileName;

private:
    std::vector<Profile *> m_profiles;
    m3d::CVar m_cvPathToProfiles;
    m3d::CVar m_cvProfileFileName;
};
