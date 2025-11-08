#pragma once
#include <map>
#include <vector>
#include <core/clazz.h>
#include <core/console/cvar.h>

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
    const CStr& GetName() const;
    void SetName(const CStr& name);
    const CStr& GetFolder() const;
    void SetFolder(const CStr& folder);
    bool IsValid() const;
    int GetParam(ProfileParam paramId, m3d::AIParam& paramVal) const;
    int SetParam(ProfileParam paramId, const m3d::AIParam& paramVal);
    int LoadFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
    int SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;

    using ParamsMap = retruxx::map<ProfileParam, m3d::AIParam, retruxx::less<enum ProfileParam>, retruxx::allocator<retruxx::pair<enum ProfileParam const, m3d::AIParam> > >;
    using ParamsPair = retruxx::pair<ProfileParam, m3d::AIParam>;

protected:
    ProfileParam ParamName2Id(const CStr& name) const;
    CStr ParamId2Name(ProfileParam id) const;
    int SetParamsDefault();
    void Clear();
    /* 0x0034 */ CStr m_name;
    /* 0x0040 */ CStr m_folder;
    /* 0x004c */ retruxx::map<enum ProfileParam, m3d::AIParam, retruxx::less<enum ProfileParam>, retruxx::allocator<retruxx::pair<enum ProfileParam const, m3d::AIParam> > > m_params;
    Profile();
    Profile(const Profile& rhs);

public:
    virtual  ~Profile() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classProfile;
}; /* size: 0x0058 */

class ProfileManager : public m3d::Object
{
public:
    Profile* GetCurProfile() const;
    int SetCurProfile(const CStr& profileName);
    const Profile* CreateNewProfile(const CStr& profileName);
    retruxx::vector<CStr, retruxx::allocator<CStr> > GetProfilesNames() const;
    Profile* GetProfileByName(const CStr& profileName) const;
    int DeleteProfile(const CStr& name);
    int Init();
    int Done();
    int SaveProfile(const Profile* profile) const;
    int LoadProfiles();
    int GameDataUpdate(void*, int);
    CStr GetDefaultProfileName() const;

    using ProfileVector = retruxx::vector<Profile*, retruxx::allocator<Profile*> >;

protected:
    void Clear();
    Profile* _GetProfileByName(const CStr& profileName) const;
    CStr GetProfileOwnFolderName(const CStr& profileName) const;
    CStr GetProfileFolderName(const CStr& profileName) const;
    CStr GetProfileFilePath(const CStr& profileName) const;
    int AddProfile(Profile* profile);
    const Profile* CreateDefaultProfile();
    int GetProfileFiles(retruxx::vector<CStr, retruxx::allocator<CStr> >& profileFiles) const;
    /* 0x0034 */ retruxx::vector<Profile*, retruxx::allocator<Profile*> > m_profiles;
    /* 0x0044 */ CStr m_curProfileName;
    /* 0x0050 */ m3d::CVar m_cvPathToProfiles;
    /* 0x007c */ m3d::CVar m_cvProfileFileName;
    ProfileManager();
    ProfileManager(const ProfileManager& rhs);

public:
    virtual  ~ProfileManager() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classProfileManager;
}; /* size: 0x00a8 */