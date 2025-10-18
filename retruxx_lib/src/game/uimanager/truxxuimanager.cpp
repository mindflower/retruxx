#include "truxxuimanager.h"
#include <config.h>
#include <stdexcept>
#include <renderer/i_renderer.h>
#include <core/ini.h>
#include <core/kernel.h>
#include <core/log.h>
#include <core/console/console.h>
#include <game/uimisc/helpmanager.h>
#include <game/uimisc/levelinfo.h>
#include <game/uimisc/msgmanager.h>
#include <game/uimisc/navpoint.h>
#include <game/uimisc/questinfo.h>
#include <game/uimisc/reply.h>
#include <game/uimisc/savesmanager.h>
#include <game/uimisc/weapongroup.h>
#include <game/uiwindows/charwindows/advancedlist.h>
#include <game/uiwindows/charwindows/basketcharacteristicswnd.h>
#include <game/uiwindows/charwindows/basketlist.h>
#include <game/uiwindows/charwindows/basketwnd.h>
#include <game/uiwindows/charwindows/buyserviceslider.h>
#include <game/uiwindows/charwindows/cabincharacteristicswnd.h>
#include <game/uiwindows/charwindows/cabinlist.h>
#include <game/uiwindows/charwindows/cabinwnd.h>
#include <game/uiwindows/charwindows/cbcharacteristicswnd.h>
#include <game/uiwindows/charwindows/cblist.h>
#include <game/uiwindows/charwindows/characteristicswnd.h>
#include <game/uiwindows/charwindows/devidedlg.h>
#include <game/uiwindows/charwindows/gadgetwnd.h>
#include <game/uiwindows/charwindows/garagewnd.h>
#include <game/uiwindows/charwindows/groundwnd.h>
#include <game/uiwindows/charwindows/infownd.h>
#include <game/uiwindows/charwindows/iteminfownd.h>
#include <game/uiwindows/charwindows/izvratrepositorywnd.h>
#include <game/uiwindows/charwindows/motherpanel.h>
#include <game/uiwindows/charwindows/motherpaneltabbutton.h>
#include <game/uiwindows/charwindows/playermoneywnd.h>
#include <game/uiwindows/charwindows/playervehiclewnd.h>
#include <game/uiwindows/charwindows/rechargelist.h>
#include <game/uiwindows/charwindows/refuellist.h>
#include <game/uiwindows/charwindows/repairlist.h>
#include <game/uiwindows/charwindows/repositoryitemwnd.h>
#include <game/uiwindows/charwindows/repositorywnd.h>
#include <game/uiwindows/charwindows/salewnd.h>
#include <game/uiwindows/charwindows/shopwnd.h>
#include <game/uiwindows/charwindows/skinswnd.h>
#include <game/uiwindows/charwindows/vehiclecharacteristicswnd.h>
#include <game/uiwindows/charwindows/vehiclepartwnd.h>
#include <game/uiwindows/charwindows/vehiclewnd.h>
#include <game/uiwindows/charwindows/videownd.h>
#include <game/uiwindows/charwindows/warewnd.h>
#include <game/uiwindows/charwindows/weapongroupbuttonlist.h>
#include <game/uiwindows/charwindows/weaponslotlist.h>
#include <game/uiwindows/charwindows/weaponslotwnd.h>
#include <game/uiwindows/charwindows/workshopvehiclewnd.h>
#include <game/uiwindows/commonwindows/complexmodelwnd.h>
#include <game/uiwindows/commonwindows/contextmodelwnd.h>
#include <game/uiwindows/commonwindows/itemmodelwnd.h>
#include <game/uiwindows/commonwindows/msgbox.h>
#include <game/uiwindows/mainwindows/bossindicatorwnd.h>
#include <game/uiwindows/mainwindows/counterwnd.h>
#include <game/uiwindows/mainwindows/damageinfownd.h>
#include <game/uiwindows/mainwindows/durabilityindicatorinmaininterfacewnd.h>
#include <game/uiwindows/mainwindows/durabilityindicatorwnd.h>
#include <game/uiwindows/mainwindows/electronicdigitalwnd.h>
#include <game/uiwindows/mainwindows/fadingmsg.h>
#include <game/uiwindows/mainwindows/fuelindicatorinmaininterfacewnd.h>
#include <game/uiwindows/mainwindows/fuelindicatorwnd.h>
#include <game/uiwindows/mainwindows/healthindicatorinmaininterfacewnd.h>
#include <game/uiwindows/mainwindows/healthindicatorwnd.h>
#include <game/uiwindows/mainwindows/igrokamochatwnd.h>
#include <game/uiwindows/mainwindows/maincursorwnd.h>
#include <game/uiwindows/mainwindows/maingameinterfacewnd.h>
#include <game/uiwindows/mainwindows/radarwnd.h>
#include <game/uiwindows/mainwindows/smartcursorwnd.h>
#include <game/uiwindows/mainwindows/speedometerwnd.h>
#include <game/uiwindows/mainwindows/targetcapturedcursorwnd.h>
#include <game/uiwindows/mainwindows/targetcapturingcursorwnd.h>
#include <game/uiwindows/mainwindows/targetcursorwnd.h>
#include <game/uiwindows/mainwindows/targetinfownd.h>
#include <game/uiwindows/mainwindows/twinklinglampwnd.h>
#include <game/uiwindows/mainwindows/vehicleinfopanel.h>
#include <game/uiwindows/mainwindows/weaponinfolist.h>
#include <game/uiwindows/miscwindows/autoscrolltextwnd.h>
#include <game/uiwindows/miscwindows/bindkeyswnd.h>
#include <game/uiwindows/miscwindows/changeprofilewnd.h>
#include <game/uiwindows/miscwindows/cinemafadepanel.h>
#include <game/uiwindows/miscwindows/cinemapanel.h>
#include <game/uiwindows/miscwindows/controloptionswnd.h>
#include <game/uiwindows/miscwindows/creditswnd.h>
#include <game/uiwindows/miscwindows/demosplashwnd.h>
#include <game/uiwindows/miscwindows/fadepanelbeforenextmap.h>
#include <game/uiwindows/miscwindows/gamemenu.h>
#include <game/uiwindows/miscwindows/gameoptionswnd.h>
#include <game/uiwindows/miscwindows/helpinmaingameinterfacewnd.h>
#include <game/uiwindows/miscwindows/helpsimplemsgwnd.h>
#include <game/uiwindows/miscwindows/helpwnd.h>
#include <game/uiwindows/miscwindows/loadwnd.h>
#include <game/uiwindows/miscwindows/lswnd.h>
#include <game/uiwindows/miscwindows/mainmenu.h>
#include <game/uiwindows/miscwindows/newprofilewnd.h>
#include <game/uiwindows/miscwindows/optionswnd.h>
#include <game/uiwindows/miscwindows/requestdifficultywnd.h>
#include <game/uiwindows/miscwindows/savewnd.h>
#include <game/uiwindows/miscwindows/soundoptionswnd.h>
#include <game/uiwindows/miscwindows/splashwnd.h>
#include <game/uiwindows/miscwindows/videooptionswnd.h>
#include <game/uiwindows/palmwindows/bookswnd.h>
#include <game/uiwindows/palmwindows/encyclopaediawnd.h>
#include <game/uiwindows/palmwindows/globalmapwnd.h>
#include <game/uiwindows/palmwindows/historywnd.h>
#include <game/uiwindows/palmwindows/journalwnd.h>
#include <game/uiwindows/palmwindows/localchartwnd.h>
#include <game/uiwindows/palmwindows/localmapwnd.h>
#include <game/uiwindows/palmwindows/mapcombobox.h>
#include <game/uiwindows/palmwindows/mapinfopanel.h>
#include <game/uiwindows/palmwindows/mapmarkwnd.h>
#include <game/uiwindows/palmwindows/mapselllist.h>
#include <game/uiwindows/palmwindows/navpointbutton.h>
#include <game/uiwindows/palmwindows/questdizwnd.h>
#include <game/uiwindows/palmwindows/questitem.h>
#include <game/uiwindows/palmwindows/questitemlist.h>
#include <game/uiwindows/palmwindows/questitemswnd.h>
#include <game/uiwindows/palmwindows/reputationwnd.h>
#include <game/uiwindows/palmwindows/saveselllist.h>
#include <game/uiwindows/palmwindows/statswnd.h>
#include <game/uiwindows/palmwindows/taksebequestlogwnd.h>
#include <game/uiwindows/townwindows/bardlg.h>
#include <game/uiwindows/townwindows/buildinglist.h>
#include <game/uiwindows/townwindows/conversationwnd.h>
#include <game/uiwindows/townwindows/npclist.h>
#include <game/uiwindows/townwindows/npcmodelwnd.h>
#include <game/uiwindows/townwindows/talkwithnpcdlg.h>
#include <game/uiwindows/townwindows/towndlg.h>
#include <ui/wnd.h>

#include "game/m3dgame.h"
#include "game/uimisc/objectsicons.h"
#include "impulses/i_impulses.h"

namespace
{
    struct _Str2GuiId
    {
        CStr m_strId;
        int m_intId;
    };

    _Str2GuiId l_str2GuiId[] = {
        {"IW_DLG_BINDKEYS", 0},
        {"IW_WND_PLAYER_INVENTORY", 64},
        {"IW_WND_WORKSHOP_INVENTORY", 65},
        {"IW_DLG_DEVIDE", 1},
        {"IW_DLG_BAR", 2},
        {"IW_DLG_ADDITIONAL_BUILDING", 3},
        {"IW_DLG_TOWN", 4},
        {"IW_DLG_PALMBOARD", 5},
        {"IW_WND_WORKSHOP", 67},
        {"IW_DLG_ADMINISTRATION", 6},
        {"IW_WND_SHOP", 66},
        {"IW_WND_RADAR", 17},
        {"IW_WND_CINEMA_PANEL", 18},
        {"IW_WND_CINEMA_FADE_PANEL", 19},
        {"IW_WND_DAMAGEINFO", 26},
        {"IW_WND_DAMAGEINFO_IN_CHARACTERISTIC_WND_LEFT", 27},
        {"IW_WND_DAMAGEINFO_IN_CHARACTERISTIC_WND_RIGHT", 28},
        {"IW_WND_WEAPON_INFO_LIST", 38},
        {"IW_WND_WEAPON_INFO_LIST_IN_FIGNYA_WND", 42},
        {"IW_WND_WEAPON_INFO_LIST_IN_CHAR_WND_LEFT", 39},
        {"IW_WND_WEAPON_INFO_LIST_IN_CHAR_WND_RIGHT", 40},
        {"IW_WND_WEAPON_INFO_LIST_IN_CHAR_WND_WORKSHOP", 41},
        {"IW_WND_GROUND", 63},
        {"IW_DLG_MOTHER_PANEL", 7},
        {"IW_WND_CHARACTERISTICS_LEFT", 68},
        {"IW_WND_CHARACTERISTICS_RIGHT", 69},
        {"IW_WND_PLAYER_MONEY", 70},
        {"IW_WND_PLAYER_MONEY_IN_NPC_DIALOG", 71},
        {"IW_WND_MAINMENU", 72},
        {"IW_WND_BUY_VEHICLE", 73},
        {"IW_WND_VIDEO", 77},
        {"IW_WND_QUESTLOG", 15},
        {"IW_WND_LOCAL_MAP", 82},
        {"IW_WND_GLOBAL_MAP", 84},
        {"IW_DLG_TALK_WITH_NPC", 88},
        {"IW_DLG_WEAPON_GROUP_CHOICE", 89},
        {"IW_WND_PLAYER_CABIN", 90},
        {"IW_WND_PLAYER_BASKET", 92},
        {"IW_WND_WORKSHOP_CABIN", 91},
        {"IW_WND_WORKSHOP_BASKET", 93},
        {"IW_WND_ZNAYU_KAK_PRODAT", 94},
        {"IW_WND_JOURNAL", 16},
        {"IW_WND_HISTORY", 95},
        {"IW_WND_BOOKS", 96},
        {"IW_WND_ENCYCLOPAEDIA", 97},
        {"IW_WND_PLAYER", 104},
        {"IW_WND_STATS", 105},
        {"IW_WND_REPUTATION", 106},
        {"IW_WND_PERKS", 107},
        {"IW_WND_SPLASH", 108},
        {"IW_WND_NEW_PROFILE", 109},
        {"IW_WND_NEW_PROFILE_ALONE", 110},
        {"IW_WND_CHANGE_PROFILE", 111},
        {"IW_WND_LOAD", 112},
        {"IW_WND_SAVE", 113},
        {"IW_WND_FIGNYA_V_CENTRE", 114},
        {"IW_WND_FIGNYA_V_CENTRE_INACTIVE", 115},
        {"IW_WND_TARGET_INFO_IN_MAIN_INTERFACE", 116},
        {"IW_WND_TARGET_INFO_IN_FIGNYA_WND", 117},
        {"IW_WND_CURSOR_MAIN", 118},
        {"IW_WND_CURSOR_TARGET_CAPTURING", 119},
        {"IW_WND_CURSOR_TARGET_CAPTURED", 120},
        {"IW_WND_CURSOR_TARGET_INFO", 121},
        {"IW_WND_VEHICLE_INFO_PANEL", 124},
        {"IW_WND_SPEEDOMETER", 30},
        {"IW_WND_FADING_MSG_LIST", 125},
        {"IW_WND_IMPORTANT_FADING_MSG_LIST", 126},
        {"IW_WND_GARAGE", 127},
        {"IW_WND_REFUEL_LIST", 128},
        {"IW_WND_REPAIR_LIST", 129},
        {"IW_WND_RECHARGE_LIST", 130},
        {"IW_WND_CABIN_BUTTON_PATTERN", 131},
        {"IW_WND_BASKET_BUTTON_PATTERN", 132},
        {"IW_WND_CABIN_LIST", 133},
        {"IW_WND_BASKET_LIST", 134},
        {"IW_WND_SKINS", 144},
        {"IW_WND_IGROKA_MOCHAT", 145},
        {"IW_WND_QUEST_ITEMS", 146},
        {"IW_WND_COUNTER", 147},
        {"IW_WND_OPTIONS", 148},
        {"IW_WND_OPTIONS_VIDEO", 149},
        {"IW_WND_OPTIONS_SOUND", 150},
        {"IW_WND_OPTIONS_CONTROL", 151},
        {"IW_WND_OPTIONS_GAME", 152},
        {"IW_WND_MAIN_GAME_INTERFACE", 155},
        {"IW_WND_PLAYER_VEHICLE_CHARACTERISTICS_LEFT", 156},
        {"IW_WND_PLAYER_VEHICLE_CHARACTERISTICS_RIGHT", 157},
        {"IW_WND_WORKSHOP_VEHICLE_CHARACTERISTICS", 158},
        {"IW_WND_PLAYER_CABIN_CHARACTERISTICS_LEFT", 159},
        {"IW_WND_PLAYER_CABIN_CHARACTERISTICS_RIGHT", 160},
        {"IW_WND_WORKSHOP_CABIN_CHARACTERISTICS", 161},
        {"IW_WND_PLAYER_BASKET_CHARACTERISTICS_LEFT", 162},
        {"IW_WND_PLAYER_BASKET_CHARACTERISTICS_RIGHT", 163},
        {"IW_WND_WORKSHOP_BASKET_CHARACTERISTICS", 164},
        {"IW_WND_DEMO_SPLASH", 167},
        {"IW_WND_CREDITS", 165},
        {"IW_WND_FADE_PANEL_BEFORE_NEXT_MAP", 166},
        {"IW_WND_HELP_SIMPLE_MSG", 168},
        {"IW_WND_HELP_MAIN_GAME_INTERFACE", 169},
        {"IW_WND_MSG_BOX", 170},
        {"IW_WND_BOSS_INDICATOR", 171},
        {"IW_WND_REQUEST_DIFFICULTY_LEVEL", 172},
    };
}

CStr TruxxUiManager::GetPathToDialogsFileGlobal() const
{
    throw std::logic_error("Not implemented");
}

QuestInfoManager* TruxxUiManager::GetQuestInfoManager() const
{
    return m_questInfoManager;
}

ref_ptr<m3d::ui::Wnd> TruxxUiManager::GetWindow(int wndId) const
{
    return GUI_GetWindow(wndId);
}

int TruxxUiManager::GetDefaultFloatPrecision() const
{
    throw std::logic_error("Not implemented");
}

MsgManager* TruxxUiManager::GetMsgManager() const
{
    throw std::logic_error("Not implemented");
}

ObjectCollection const& TruxxUiManager::GetObjectCollection() const
{
    throw std::logic_error("Not implemented");
}

bool TruxxUiManager::IsHidden() const
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::AddWindow(ref_ptr<m3d::ui::Wnd> w, int& wndId, bool isPersistent, bool needShow)
{
    return GUI_AddWindow(w, wndId, isPersistent, needShow);
}

int TruxxUiManager::AddWindowById(ref_ptr<m3d::ui::Wnd> w, int wndId, bool isPersistent, bool needShow)
{
    return GUI_AddWindowById(w, wndId, isPersistent, needShow);
}

CStr TruxxUiManager::GetPathToQuestInfoFileGlobal() const
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::HandleImpulse(m3d::AuxImpulseInfo const&, m3d::ui::Wnd*)
{
    throw std::logic_error("Not implemented");
}

TruxxUiManager::TruxxUiManager()
{
    m_minDynamicId = 173;
    m_nextDynamicId = GUI_ValidateDynamicId(173);
}

bool TruxxUiManager::IsHiddenByUser() const
{
    return this->m_bIsHiddenByUser;
}

CStr TruxxUiManager::GetPathToDynamicDialogsFileGlobal() const
{
    throw std::logic_error("Not implemented");
}

bool TruxxUiManager::IsWindowVisibleAndNotAnimating(int) const
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::Load(ref_ptr<m3d::cmn::XmlFile>, ref_ptr<m3d::cmn::XmlNode>)
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::Init()
{
    auto res = GUI_Init(true) & 1;
    m_questInfoManager = new QuestInfoManager;
    if (!m_questInfoManager)
    {
        res = 0;
    }
    m_repliesManager = dynamic_cast<RepliesManager*>(m3d::g_Kernel->New("RepliesManager"));
    if (!m_repliesManager)
    {
        res = 0;
    }
    m_levelInfoManager = dynamic_cast<LevelInfoManager*>(m3d::g_Kernel->New("LevelInfoManager"));
    if (m_levelInfoManager)
    {
        m_levelInfoManager->Init();
    }
    else
    {
        res = 0;
    }
    m_navPointManager = new NavPointManager;
    if (!m_navPointManager)
    {
        res = 0;
    }
    m_weaponGroupManager = dynamic_cast<WeaponGroupManager*>(m3d::g_Kernel->New("WeaponGroupManager"));
    if (!m_weaponGroupManager)
    {
        res = 0;
    }
    m_savesManager = dynamic_cast<SavesManager*>(m3d::g_Kernel->New("SavesManager"));
    if (!m_savesManager)
    {
        res = 0;
    }
    m_msgManager = dynamic_cast<MsgManager*>(m3d::g_Kernel->New("MsgManager"));
    if (!m_msgManager)
    {
        res = 0;
    }
    m_helpManager = dynamic_cast<HelpManager*>(m3d::g_Kernel->New("HelpManager"));
    if (!m_helpManager)
    {
        res = 0;
    }
    ref_ptr<m3d::ui::Wnd> gameMenuWnd = dynamic_cast<GameMenuWnd*>(m3d::g_Kernel->New("GameMenuWnd"));
    if (gameMenuWnd)
    {
        res = AddWindowById(gameMenuWnd, 154, true, false) & res;
        if (res)
        {
            M3D_LOG_INFO("TruxxUiManager: was inited successfully");
            return res;
        }
    }
    M3D_LOG_ERR("TruxxUiManager: was inited with errors");
    return res;
}

bool TruxxUiManager::IsModalEqualWndRunning() const
{
    return GUI_IsModalEqualWndRunning();
}

LevelInfoManager* TruxxUiManager::GetLevelInfoManager() const
{
    return m_levelInfoManager;
}

void TruxxUiManager::OnEnterTown(int)
{
    throw std::logic_error("Not implemented");
}

CStr TruxxUiManager::GetPathToLevelInfoFile() const
{
    return m_cvPathToLevelInfo.GetS();
}

m3d::rend::TexHandle TruxxUiManager::GetIcoByName(CStr const& name, int mode)
{
    return m_icons->GetIcoByName(name, mode);
}

int TruxxUiManager::ShowWindow(int wndId, bool needShow, bool forceShow, bool forceModal, bool pause, int* modalRetVal)
{
    if (needShow)
    {
        return GameUiManager::GUI_ShowWindow(wndId, forceShow, forceModal, pause, modalRetVal);
    }
    return GameUiManager::GUI_HideWindow(wndId, false, modalRetVal, false);
}

void TruxxUiManager::ShowSplash(int processStatus, CStr const& text) const
{
    auto app = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
    ref_ptr wndSplash = app->m_pInterfaceManager->GetWindow(108);
    if (wndSplash)
    {
        if (wndSplash->IsKindOf(RT_CLASS_LOCAL(SplashWnd)))
        {
            auto wndCasted = dynamic_cast<SplashWnd*>(&*wndSplash);
            wndCasted->ShowSplash(processStatus, text);
        }
    }
}

int TruxxUiManager::Show(bool needShow, bool enabeleAnimation)
{
    return GUI_ShowInterface(needShow, enabeleAnimation);
}

int TruxxUiManager::CreateAndAddWindow(int)
{
    throw std::logic_error("Not implemented");
}

StringParser const& TruxxUiManager::GetStringParser() const
{
    throw std::logic_error("Not implemented");
}

ai::Workshop* TruxxUiManager::GetCurrentShop() const
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::AddFadingMsgByStrId(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam>> const&) const
{
    throw std::logic_error("Not implemented");
}

NavPointManager* TruxxUiManager::GetNavPointManager() const
{
    throw std::logic_error("Not implemented");
}

ai::Workshop* TruxxUiManager::GetCurrentWorkshop() const
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::AddFadingMsg(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam>> const&) const
{
    throw std::logic_error("Not implemented");
}

bool TruxxUiManager::IsGameModeValidForSmartCursor(GameState mode) const
{
    static const std::set<GameState> modes { GS_GAME, GS_CINEMATIC };
    return modes.find(mode) != modes.end();
}

int TruxxUiManager::Str2WndGuiId(CStr const& strId) const
{
    for (auto const& str2GuiId : l_str2GuiId)
    {
        if (str2GuiId.m_strId == strId)
        {
            return str2GuiId.m_intId;
        }
    }
    return -1;
}

ai::Vehicle* TruxxUiManager::GetVehicleSellingInWorkshop() const
{
    throw std::logic_error("Not implemented");
}

std::vector<int, std::allocator<int>> const& TruxxUiManager::GetTakenQuestIds() const
{
    throw std::logic_error("Not implemented");
}

RepliesManager* TruxxUiManager::GetRepliesManager() const
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::Reset(bool)
{
    throw std::logic_error("Not implemented");
}

WeaponGroupManager* TruxxUiManager::GetWeaponGroupManager() const
{
    return m_weaponGroupManager;
}

void TruxxUiManager::AddImportantFadingMsgByStrId(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam>> const&) const
{
    throw std::logic_error("Not implemented");
}

m3d::ui::MbRetCodes TruxxUiManager::RunMsgBoxDlg(CStr const& caption, CStr const& message, unsigned flags, bool bPause)
{
    ref_ptr box = dynamic_cast<MsgBox*>(M3D_KERNEL->New("MsgBox"));
    if (!box)
    {
        return m3d::ui::MBX_RET_CANCEL;
    }
    if (!box->CreateMsgBox(caption, message, flags))
    {
        return m3d::ui::MBX_RET_CANCEL;
    }
    int guiId = -1;
    AddWindow(&*box, guiId, false, false);

    int retVal = m3d::ui::MBX_RET_CANCEL;;
    ShowWindow(guiId, true, true, true, bPause, &retVal);
    RemoveWindow(guiId);
    GUI_EndModalDlg();
    return static_cast<m3d::ui::MbRetCodes>(retVal);
}

bool TruxxUiManager::IsInSaleMode() const
{
    throw std::logic_error("Not implemented");
}

bool TruxxUiManager::IsWindowVisible(int wndGuiId) const
{
    auto wnd = GetWindow(wndGuiId);
    if (!wnd)
    {
        return false;
    }
    if (wnd->IsChildOf(M3D_APP))
    {
        return true;
    }
    return false;
}

int TruxxUiManager::SetEventsForWindow(int, std::vector<int, std::allocator<int>> const&)
{
    throw std::logic_error("Not implemented");
}

TruxxUiManager::~TruxxUiManager()
{
    throw std::logic_error("Not implemented");
}

CStr TruxxUiManager::GetPathToSplashes() const
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::Update()
{
    auto v2 = this->LaunchEvent(89, GUI_EVENT_CUSTOM, 0) & 1;
    return v2 & this->LaunchEvent(90, GUI_EVENT_CUSTOM, 0);
}

int TruxxUiManager::LaunchEvent(int eventId, GuiEventType type, void* data)
{
    return GUI_ProcessEvent(type, eventId, data, nullptr);
}

SavesManager* TruxxUiManager::GetSavesManager() const
{
    return m_savesManager;
}

ai::Town* TruxxUiManager::GetCurrentTown() const
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::Save(ref_ptr<m3d::cmn::XmlFile>, ref_ptr<m3d::cmn::XmlNode>)
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::StartSplashing(int numSplashes) const
{
    auto app = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
	auto wnd = app->m_pInterfaceManager->GetWindow(108);
    if (wnd)
    {
	    if (wnd->IsKindOf(RT_CLASS_LOCAL(SplashWnd)))
	    {
            auto splashWnd = dynamic_cast<SplashWnd*>(&*wnd);
            splashWnd->StartSplashing(11);
	    }
    }
}

int TruxxUiManager::HandleAppEvent(m3d::Event const& appEvent)
{
    return GUI_ProcessEvent(GUI_EVENT_FROM_APPEVENT, appEvent.m_eventType, &const_cast<m3d::Event&>(appEvent), nullptr);
}

void TruxxUiManager::OnLeaveTown(bool)
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::AddImportantFadingMsg(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam>> const&) const
{
    throw std::logic_error("Not implemented");
}

HelpManager* TruxxUiManager::GetHelpManager() const
{
    return m_helpManager;
}

int TruxxUiManager::RemoveWindow(int wndId)
{
    return GUI_RemoveWindow(wndId);
}

bool TruxxUiManager::GUI_IsWndModalEqual(m3d::ui::Wnd* w) const
{
    using namespace m3d::ui;
    if (w && (w->IsKindOf(RT_CLASS_LOCAL(ModalWnd)) || w->IsKindOf(RT_CLASS_LOCAL(MainMenuUI))))
    {
        return true;
    }
    return false;
}

int TruxxUiManager::GUI_ReadFromXml(ref_ptr<m3d::cmn::XmlFile>, ref_ptr<m3d::cmn::XmlNode>)
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::IncRef()
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::GUI_UnRegisterScriptGlobals()
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::ShowGameMenu(CStr const&)
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::OnGameModeChanged(void* data)
{
    if (data)
    {
        auto ev = static_cast<m3d::Event*>(data);
        M3D_LOG_INFO("Game mode changed. Old mode = " + CStr(ev->m_intEv[1]) + ", new mode = " + CStr(ev->m_intEv[0]));
        if (ev->m_intEv[0] == 0 && ev->m_intEv[1] == 1 && m_bIsPlayerDead)
        {
            M3D_APP->EnqueueMessage(65656, 0, 0, 0, 0, GameMenuWnd::DEATH_LEVEL_NAME, {});
        }
    }
}

void TruxxUiManager::OnBeforeStartLevel()
{
    //TODO: check and refactor this
    bool bOldFirstLevelResourcesLoaded = m_bFirstLevelResourcesLoaded;
    if (!m_bFirstLevelResourcesLoaded && !GUI_IsCurrentLevelMainMenuLevel())
    {
        throw std::logic_error("Not implemented");
    }
    auto res = 1;
    if (GUI_IsCurrentLevelMainMenuLevel())
    {
        goto LABEL_18;
    }
    if (!m_bFirstLevelResourcesLoaded)
    {
        res = GUI_LoadResources(ResourceInfo::LOADTYPE_AT_FIRST_LEVEL_START) & 1;
        m_bFirstLevelResourcesLoaded = true;
    }
    if ((GUI_LoadResources(ResourceInfo::LOADTYPE_AT_LEVEL_START) & res) != 0)
    {
    LABEL_18:
        M3D_LOG_INFO("Interface: is loaded successfully");
    }
    else
    {
        M3D_LOG_INFO("Interface: is loaded with errors");
    }
    if (!bOldFirstLevelResourcesLoaded && m_bFirstLevelResourcesLoaded)
    {
        GUI_RegisterScriptGlobals();
    }
}

void TruxxUiManager::GUI_EndModalDlg()
{
    if (!IsModalEqualWndRunning() || M3D_APP->GetCurGameMode() == 1)
    {
        M3D_APP->UnPause();
    }
}

namespace
{
    //TODO: get rid of this
    retruxx::vector<int> convert(std::initializer_list<int> list)
    {
        retruxx::vector<int> result;
        result.reserve(list.size());
        for (const auto elem : list)
        {
            result.push_back(elem);
        }
        return result;
    }
}

int TruxxUiManager::GUI_BindWindowsToEvents()
{
    int res = 0;
    retruxx::vector<int> events;

    events = convert({ 17 });
    res = GUI_SetEventsForWindow(0, events) & 1;

    events = convert({ 64 });
    res = GUI_SetEventsForWindow(64, events) & res;

    events = convert({ 65, 32, 31, 34 });
    res = GUI_SetEventsForWindow(90, events) & res;

    events = convert({ 65, 32, 31, 34 });
    res = GUI_SetEventsForWindow(90, events) & res;

    events = convert({ 65, 32, 31, 34 });
    res = GUI_SetEventsForWindow(91, events) & res;

    events = convert({ 65, 32, 102, 34 });
    res = GUI_SetEventsForWindow(92, events) & res;

    events = convert({ 65, 32, 31, 34 });
    res = GUI_SetEventsForWindow(93, events) & res;

    events = convert({ 65, 32, 31, 34 });
    res = GUI_SetEventsForWindow(93, events) & res;

    events = convert({ 91, 58, 97, 98, 99, 65, 75, 89 });
    res = GUI_SetEventsForWindow(8, events) & res;

    events = convert({ 91, 58, 97, 98, 99, 65, 75, 89 });
    res = GUI_SetEventsForWindow(9, events) & res;

    events = convert({ 91, 58, 97, 98, 99, 65, 75, 89 });
    res = GUI_SetEventsForWindow(10, events) & res;

    events = convert({ 91, 58, 97, 98, 99, 65, 75, 89 });
    res = GUI_SetEventsForWindow(32, events) & res;

    events = convert({ 91, 92, 93, 94, 95, 96, 97, 98, 65 });
    res = GUI_SetEventsForWindow(11, events) & res;

    events = convert({ 64, 99, 100 });
    res = GUI_SetEventsForWindow(66, events) & res;

    events = convert({ 89, 85, 28, 29 });
    res = GUI_SetEventsForWindow(17, events) & res;

    events = convert({ 89 });
    res = GUI_SetEventsForWindow(20, events) & res;

    events = convert({ 89 });
    res = GUI_SetEventsForWindow(23, events) & res;

    events = convert({ 89 });
    res = GUI_SetEventsForWindow(135, events) & res;

    events = convert({ 89 });
    res = GUI_SetEventsForWindow(136, events) & res;

    events = convert({ 89 });
    res = GUI_SetEventsForWindow(137, events) & res;

    //TODO: check this
    events = convert({ 89, 65 });
    res = GUI_SetEventsForWindow(21, events) & res;

    events = convert({ 89, 65 });
    res = GUI_SetEventsForWindow(22, events) & res;

    events = convert({ 89, 65 });
    res = GUI_SetEventsForWindow(24, events) & res;

    events = convert({ 89, 65 });
    res = GUI_SetEventsForWindow(25, events) & res;

    events = convert({ 89, 65 });
    res = GUI_SetEventsForWindow(138, events) & res;

    events = convert({ 89, 65 });
    res = GUI_SetEventsForWindow(139, events) & res;

    events = convert({ 89, 65 });
    res = GUI_SetEventsForWindow(140, events) & res;

    events = convert({ 89, 65 });
    res = GUI_SetEventsForWindow(141, events) & res;

    events = convert({ 89, 65 });
    res = GUI_SetEventsForWindow(142, events) & res;

    events = convert({ 89, 65 });
    res = GUI_SetEventsForWindow(143, events) & res;

    events = convert({ 89 });
    res = GUI_SetEventsForWindow(29, events) & res;

    events = convert({ 89 });
    res = GUI_SetEventsForWindow(30, events) & res;

    events = convert({ 64 });
    res = GUI_SetEventsForWindow(26, events) & res;

    events = convert({ 64 });
    res = GUI_SetEventsForWindow(27, events) & res;

    events = convert({ 64 });
    res = GUI_SetEventsForWindow(28, events) & res;

    events = convert({ 89, 58 });
    res = GUI_SetEventsForWindow(36, events) & res;

    events = convert({ 89, 99, 98, 91, 95, 94, 40, 42 });
    res = GUI_SetEventsForWindow(35, events) & res;

    events = convert({ 101, 27, 26 });
    res = GUI_SetEventsForWindow(37, events) & res;

    events = convert({ 64, 65, 30, 17 });
    res = GUI_SetEventsForWindow(38, events) & res;

    events = convert({ 65 });
    res = GUI_SetEventsForWindow(39, events) & res;

    events = convert({ 65 });
    res = GUI_SetEventsForWindow(40, events) & res;

    events = convert({ 65 });
    res = GUI_SetEventsForWindow(41, events) & res;

    //TODO: check this
    for (int i = 43; i <= 62; ++i)
    {
        events = convert({ 91, 92, 93, 94, 95, 96, 97, 98, 69 });
        res = GUI_SetEventsForWindow(i, events) & res;
    }

    events = convert({ 64, 58, 85 });
    res = GUI_SetEventsForWindow(63, events) & res;

    events = convert({ 2, 7, 4, 6, 5, 3, 12, 13, 21, 24, 20, 18, 33, 34, 35, 36, 38, 39, 14 });
    res = GUI_SetEventsForWindow(7, events) & res;

    events = convert({ 64, 107, 65, 55 });
    res = GUI_SetEventsForWindow(68, events) & res;

    events = convert({ 64, 107, 65, 55 });
    res = GUI_SetEventsForWindow(69, events) & res;

    events = convert({ 75, 85 });
    res = GUI_SetEventsForWindow(70, events) & res;

    events = convert({ 75, 85 });
    res = GUI_SetEventsForWindow(71, events) & res;

    events = convert({ 89, 65 });
    res = GUI_SetEventsForWindow(74, events) & res;

    events = convert({ 75, 85 });
    res = GUI_SetEventsForWindow(75, events) & res;

    events = convert({ 75, 85 });
    res = GUI_SetEventsForWindow(76, events) & res;

    events = convert({ 67, 60, 59, 28, 29 });
    res = GUI_SetEventsForWindow(78, events) & res;

    events = convert({ 85 });
    res = GUI_SetEventsForWindow(80, events) & res;

    events = convert({ 85 });
    res = GUI_SetEventsForWindow(81, events) & res;

    events = convert({ 25, 70 });
    res = GUI_SetEventsForWindow(88, events) & res;

    events = convert({ 23, 89 });
    res = GUI_SetEventsForWindow(87, events) & res;

    events = convert({ 28, 29, 40 });
    res = GUI_SetEventsForWindow(83, events) & res;

    events = convert({ 17 });
    res = GUI_SetEventsForWindow(89, events) & res;

    events = convert({ 65, 69, 58 });
    res = GUI_SetEventsForWindow(94, events) & res;

    events = convert({ 58, 34, 65, 55 });
    res = GUI_SetEventsForWindow(73, events) & res;

    events = convert({ 89 });
    res = GUI_SetEventsForWindow(98, events) & res;

    events = convert({ 89 });
    res = GUI_SetEventsForWindow(99, events) & res;

    events = convert({ 89 });
    res = GUI_SetEventsForWindow(101, events) & res;

    events = convert({ 89 });
    res = GUI_SetEventsForWindow(102, events) & res;

    events = convert({ 89, 65 });
    res = GUI_SetEventsForWindow(100, events) & res;

    events = convert({ 89, 65 });
    res = GUI_SetEventsForWindow(103, events) & res;

    events = convert({ 89 });
    res = GUI_SetEventsForWindow(77, events) & res;

    events = convert({ 57, 104, 105 });
    res = GUI_SetEventsForWindow(4, events) & res;

    events = convert({ 85, 64, 69, 72, 73, 76, 77, 90, 51 });
    res = GUI_SetEventsForWindow(97, events) & res;

    events = convert({ 85, 71 });
    res = GUI_SetEventsForWindow(84, events) & res;

    events = convert({ 89, 85 });
    res = GUI_SetEventsForWindow(105, events) & res;

    events = convert({ 52 });
    res = GUI_SetEventsForWindow(106, events) & res;

    events = convert({ 41, 40 });
    res = GUI_SetEventsForWindow(111, events) & res;

    events = convert({ 40 });
    res = GUI_SetEventsForWindow(72, events) & res;

    events = convert({ 44, 42, 40, 46 });
    res = GUI_SetEventsForWindow(112, events) & res;

    events = convert({ 43, 42, 40, 46 });
    res = GUI_SetEventsForWindow(113, events) & res;

    events = convert({ 90 });
    res = GUI_SetEventsForWindow(117, events) & res;

    events = convert({ 90 });
    res = GUI_SetEventsForWindow(116, events) & res;

    //TODO: check this! 118
    events = convert({ 89 });
    res = GUI_SetEventsForWindow(118, events) & res;

    events = convert({ 47, 48 });
    res = GUI_SetEventsForWindow(118, events) & res;

    events = convert({ 64 });
    res = GUI_SetEventsForWindow(124, events) & res;

    events = convert({ 89, 79 });
    res = GUI_SetEventsForWindow(125, events) & res;

    events = convert({ 89, 80 });
    res = GUI_SetEventsForWindow(126, events) & res;

    events = convert({ 42, 40 });
    res = GUI_SetEventsForWindow(15, events) & res;

    events = convert({ 28, 29 });
    res = GUI_SetEventsForWindow(82, events) & res;

    events = convert({ 64, 89, 58, 34 });
    res = GUI_SetEventsForWindow(67, events) & res;

    events = convert({ 65, 64, 89 });
    res = GUI_SetEventsForWindow(128, events) & res;

    events = convert({ 65, 64, 89 });
    res = GUI_SetEventsForWindow(129, events) & res;

    events = convert({ 65, 64, 89 });
    res = GUI_SetEventsForWindow(130, events) & res;

    events = convert({ 58 });
    res = GUI_SetEventsForWindow(133, events) & res;

    events = convert({ 58 });
    res = GUI_SetEventsForWindow(134, events) & res;

    events = convert({ 76, 89 });
    res = GUI_SetEventsForWindow(145, events) & res;

    events = convert({ 77 });
    res = GUI_SetEventsForWindow(146, events) & res;

    events = convert({ 89, 67, 85 });
    res = GUI_SetEventsForWindow(147, events) & res;

    events = convert({ 28, 29 });
    res = GUI_SetEventsForWindow(153, events) & res;

    events = convert({ 89, 82, 83, 78 });
    res = GUI_SetEventsForWindow(155, events) & res;

    events = convert({ 89 });
    res = GUI_SetEventsForWindow(156, events) & res;

    events = convert({ 89 });
    res = GUI_SetEventsForWindow(157, events) & res;

    events = convert({ 89 });
    res = GUI_SetEventsForWindow(158, events) & res;

    events = convert({ 89 });
    res = GUI_SetEventsForWindow(159, events) & res;

    events = convert({ 89 });
    res = GUI_SetEventsForWindow(160, events) & res;

    events = convert({ 89 });
    res = GUI_SetEventsForWindow(161, events) & res;

    events = convert({ 89 });
    res = GUI_SetEventsForWindow(162, events) & res;

    events = convert({ 89 });
    res = GUI_SetEventsForWindow(163, events) & res;

    events = convert({ 89 });
    res = GUI_SetEventsForWindow(164, events) & res;

    events = convert({ 89 });
    res = GUI_SetEventsForWindow(165, events) & res;

    events = convert({ 89, 81 });
    res = GUI_SetEventsForWindow(166, events) & res;

    events = convert({ 89 });
    res = GUI_SetEventsForWindow(171, events) & res;

    if (!res)
    {
        M3D_LOG_INFO("Interface event map is inited with errors");
    }
    return res;
}

void TruxxUiManager::GUI_RegisterCVars()
{
    GameUiManager::GUI_RegisterCVars();
    m_cvPathToQuestInfo.Init("pathToQuestInfo", "data\\if\\diz\\QuestInfoGlobal.xml", m3d::CVar::CVAR_STRING, m3d::CVar::eFlags::CVAR_ARCHIVE);
    m_cvPathToDialogs.Init("pathToDialogs", "data\\if\\diz\\DialogsGlobal.xml", m3d::CVar::CVAR_STRING, m3d::CVar::eFlags::CVAR_ARCHIVE);
    m_cvPathToDynamicDialogs.Init("pathToDynamicDialogs", "data\\if\\diz\\DynamicDialogsGlobal.xml", m3d::CVar::CVAR_STRING, m3d::CVar::eFlags::CVAR_ARCHIVE);
    m_cvPathToLevelInfo.Init("pathToLevelInfo", "data\\if\\diz\\LevelInfo.xml", m3d::CVar::CVAR_STRING, m3d::CVar::eFlags::CVAR_ARCHIVE);
    m_cvPathToSplashes.Init("pathToSplashes", "data\\if\\ico\\splashes.xml", m3d::CVar::CVAR_STRING, m3d::CVar::eFlags::CVAR_ARCHIVE);
    m_cvDefaultFloatPrecision.Init("guiFloatPrecision", "2", m3d::CVar::CVAR_INT, m3d::CVar::eFlags::CVAR_ARCHIVE);

    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_cvPathToQuestInfo, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_cvPathToDialogs, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_cvPathToDynamicDialogs, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_cvPathToLevelInfo, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_cvPathToSplashes, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_cvDefaultFloatPrecision, nullptr);
}

int TruxxUiManager::GUI_WriteToXml(ref_ptr<m3d::cmn::XmlFile>, ref_ptr<m3d::cmn::XmlNode>)
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::OnStartLevel(void* data)
{
    m_msgManager->Init(false);
    if (*static_cast<int*>(data) != 2)
    {
        GUI_Init(false);
        if (M3D_APP->GetCurGameMode() != 1)
        {
            Show(true, true);
        }
    }
}

void TruxxUiManager::OnEndLevel(bool)
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::LoadCommonDiz()
{
    throw std::logic_error("Not implemented");
}

bool TruxxUiManager::CanLaunchModalEqualWindow()
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::PrepareMenuForShow(CStr const&)
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::DecRef()
{
    throw std::logic_error("Not implemented");
}

bool TruxxUiManager::GUI_NeedUpdateWndOnEvent(ref_ptr<m3d::ui::Wnd> wnd, int eventId, void* data)
{
    if (!wnd || eventId == -1)
    {
        return false;
    }
    auto res = true;
    if (eventId == 89)
    {
        if (!wnd->IsChildOf(M3D_APP))
        {
            res = false;
        }
    }
    return res;
}

int TruxxUiManager::GUI_HandleEvent(int guiEventId, m3d::ui::Wnd* forceWnd, void* data)
{
    if (GameUiManager::GUI_HandleEvent(guiEventId, forceWnd, data))
    {
        return 1;
    }
    if (forceWnd && guiEventId != 16)
    {
        return 0;
    }
    switch (guiEventId)
    {
    case 17:
    case 37:
    case 41:
    case 42:
    case 44:
    case 47:
    case 58:
    {
        return 0;
    }
    case 40:
    {
        m_savesManager->GameDataUpdate(data, guiEventId);
        return 0;
    }
    case 48:
    {
        OnGameModeChanged(data);
        return 0;
    }
    case 61:
    {
        if (!data)
            return 1;

        auto event = (m3d::Event*)data;
        auto entityByObjId = ai::theObjects->GetEntityByObjId(event->m_intEv[0]);
        if (!entityByObjId)
            return 1;

        auto* cls= entityByObjId->GetClass();
        if (!m_objectCollection.IsClassCollected(cls))
            return 1;
        m_objectCollection.AddObject(entityByObjId);
        return 1;
    }
    case 64:
    {
        //throw std::logic_error("Not implemented");
        //return 1;
        m_weaponGroupManager->GameDataUpdate(data, guiEventId);
        auto event = (m3d::Event*)data;
        if (event->m_intEv[0] != 1)
        {
            return 1;
        }
        m_bIsHiddenByUser = true;
        return 1;
    }
    case 84:
    {
        OnBeforeStartLevel();
        return 0;
    }
    case 85:
    {
        m_levelInfoManager->GameDataUpdate(data, guiEventId);
        m_questInfoManager->GameDataUpdate(data, guiEventId);
        OnStartLevel(data);
        return 0;
    }
    case 89:
    {
        return 0;
    }
    case 90:
    {
        m_levelInfoManager->GameDataUpdate(data, guiEventId);
        m_savesManager->GameDataUpdate(data, guiEventId);
        return 0;
    }
    default:
        throw std::logic_error("Not implemented");
    }
}

void* TruxxUiManager::QueryIface(char const*)
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::GUI_RegisterEvents()
{
    //TODO: check this
    m_eventToEvent[41] = 0;
    m_impulseToEvent[42] = 2;
    m_impulseToEvent[43] = 3;
    m_impulseToEvent[44] = 4;
    m_impulseToEvent[45] = 5;
    m_impulseToEvent[46] = 6;
    m_impulseToEvent[47] = 7;
    m_impulseToEvent[52] = 8;
    m_impulseToEvent[53] = 9;
    m_impulseToEvent[54] = 10;
    m_impulseToEvent[39] = 11;
    m_impulseToEvent[48] = 12;
    m_impulseToEvent[49] = 13;
    m_impulseToEvent[55] = 14;
    m_impulseToEvent[0] = 15;
    m_impulseToEvent[50] = 16;
    m_eventToEvent[46] = 17;
    m_eventToEvent[42] = 18;
    m_eventToEvent[65653] = 19;
    m_eventToEvent[65657] = 20;
    m_eventToEvent[65658] = 21;
    m_eventToEvent[65659] = 22;
    m_eventToEvent[65660] = 23;
    m_eventToEvent[65661] = 24;
    m_eventToEvent[65663] = 25;
    m_eventToEvent[65665] = 26;
    m_eventToEvent[65664] = 27;
    m_eventToEvent[65666] = 28;
    m_eventToEvent[65667] = 29;
    m_eventToEvent[65668] = 30;
    m_eventToEvent[65669] = 31;
    m_eventToEvent[65670] = 32;
    m_eventToEvent[65671] = 33;
    m_eventToEvent[65672] = 34;
    m_eventToEvent[65673] = 35;
    m_eventToEvent[65674] = 36;
    m_eventToEvent[65675] = 37;
    m_eventToEvent[65676] = 38;
    m_eventToEvent[65677] = 39;
    m_eventToEvent[65678] = 40;
    m_eventToEvent[65679] = 41;
    m_eventToEvent[65680] = 42;
    m_eventToEvent[65644] = 43;
    m_eventToEvent[65645] = 44;
    m_eventToEvent[65684] = 45;
    m_eventToEvent[65681] = 46;
    m_eventToEvent[65682] = 47;
    m_eventToEvent[65683] = 48;
    m_eventToEvent[65656] = 49;
    m_eventToEvent[65685] = 50;
    m_eventToEvent[65686] = 51;
    m_eventToEvent[65687] = 52;
    m_eventToEvent[65688] = 53;
    m_eventToEvent[65689] = 54;
    m_eventToEvent[65690] = 55;
    m_eventToEvent[65691] = 56;
    m_eventToEvent[66540] = 57;
    m_eventToEvent[66537] = 58;
    m_eventToEvent[66538] = 59;
    m_eventToEvent[66539] = 60;
    m_eventToEvent[66541] = 61;
    m_eventToEvent[66542] = 62;
    m_eventToEvent[66543] = 63;
    m_eventToEvent[66544] = 64;
    m_eventToEvent[66545] = 65;
    m_eventToEvent[66546] = 66;
    m_eventToEvent[66547] = 67;
    m_eventToEvent[66548] = 68;
    m_eventToEvent[66551] = 69;
    m_eventToEvent[66554] = 70;
    m_eventToEvent[66556] = 71;
    m_eventToEvent[66557] = 72;
    m_eventToEvent[66558] = 73;
    m_eventToEvent[66553] = 74;
    m_eventToEvent[66559] = 75;
    m_eventToEvent[66561] = 76;
    m_eventToEvent[66562] = 77;
    m_eventToEvent[66565] = 78;
    m_eventToEvent[66563] = 79;
    m_eventToEvent[66564] = 80;
    m_eventToEvent[66566] = 81;
    m_eventToEvent[66567] = 82;
    m_eventToEvent[66568] = 83;
}

void TruxxUiManager::GUI_RegisterClasses()
{
    m3d::g_Kernel->AddClass(&BindKeysWnd::m_classBindKeysWnd);
    m3d::g_Kernel->AddClass(&RepositoryWnd::m_classRepositoryWnd);
    m3d::g_Kernel->AddClass(&IzvratRepositoryWnd::m_classIzvratRepositoryWnd);
    m3d::g_Kernel->AddClass(&InventoryWnd::m_classInventoryWnd);
    m3d::g_Kernel->AddClass(&DragDropItemsWnd::m_classDragDropItemsWnd);
    m3d::g_Kernel->AddClass(&VehiclePartWnd::m_classVehiclePartWnd);
    m3d::g_Kernel->AddClass(&DevideDlg::m_classDevideDlg);
    m3d::g_Kernel->AddClass(&BarDlg::m_classBarDlg);
    m3d::g_Kernel->AddClass(&ConversationWnd::m_classConversationWnd);
    m3d::g_Kernel->AddClass(&TownDlg::m_classTownDlg);
    m3d::g_Kernel->AddClass(&RadarWnd::m_classRadarWnd);
    m3d::g_Kernel->AddClass(&CinemaPanel::m_classCinemaPanel);
    m3d::g_Kernel->AddClass(&CinemaFadePanel::m_classCinemaFadePanel);
    m3d::g_Kernel->AddClass(&DamageInfoWnd::m_classDamageInfoWnd);
    m3d::g_Kernel->AddClass(&HealthIndicatorWnd::m_classHealthIndicatorWnd);
    m3d::g_Kernel->AddClass(&HealthIndicatorInMainInterfaceWnd::m_classHealthIndicatorInMainInterfaceWnd);
    m3d::g_Kernel->AddClass(&DurabilityIndicatorWnd::m_classDurabilityIndicatorWnd);
    m3d::g_Kernel->AddClass(&DurabilityIndicatorInMainInterfaceWnd::m_classDurabilityIndicatorInMainInterfaceWnd);
    m3d::g_Kernel->AddClass(&JournalWnd::m_classJournalWnd);
    m3d::g_Kernel->AddClass(&HistoryWnd::m_classHistoryWnd);
    m3d::g_Kernel->AddClass(&BooksWnd::m_classBooksWnd);
    m3d::g_Kernel->AddClass(&EncyclopaediaWnd::m_classEncyclopaediaWnd);
    m3d::g_Kernel->AddClass(&SubjectButton::m_classSubjectButton);
    m3d::g_Kernel->AddClass(&SubjectList::m_classSubjectList);
    m3d::g_Kernel->AddClass(&CheckList::m_classCheckList);
    m3d::g_Kernel->AddClass(&CheckButton::m_classCheckButton);
    m3d::g_Kernel->AddClass(&InfoWnd::m_classInfoWnd);
    m3d::g_Kernel->AddClass(&ItemInfoWnd::m_classItemInfoWnd);
    m3d::g_Kernel->AddClass(&RepliesManager::m_classRepliesManager);
    m3d::g_Kernel->AddClass(&FuelIndicatorWnd::m_classFuelIndicatorWnd);
    m3d::g_Kernel->AddClass(&FuelIndicatorInMainInterfaceWnd::m_classFuelIndicatorInMainInterfaceWnd);
    m3d::g_Kernel->AddClass(&SpeedometerWnd::m_classSpeedometerWnd);
    m3d::g_Kernel->AddClass(&WeaponInfoWnd::m_classWeaponInfoWnd);
    m3d::g_Kernel->AddClass(&WeaponInfoList::m_classWeaponInfoList);
    m3d::g_Kernel->AddClass(&ItemWnd::m_classItemWnd);
    m3d::g_Kernel->AddClass(&GadgetWnd::m_classGadgetWnd);
    m3d::g_Kernel->AddClass(&MotherPanel::m_classMotherPanel);
    m3d::g_Kernel->AddClass(&ChildPanel::m_classChildPanel);
    m3d::g_Kernel->AddClass(&GroundWnd::m_classGroundWnd);
    m3d::g_Kernel->AddClass(&SaleWnd::m_classSaleWnd);
    m3d::g_Kernel->AddClass(&ShopWnd::m_classShopWnd);
    m3d::g_Kernel->AddClass(&PlayerMoneyWnd::m_classPlayerMoneyWnd);
    m3d::g_Kernel->AddClass(&MainMenuUI::m_classMainMenuUI);
    m3d::g_Kernel->AddClass(&RepositoryItemWnd::m_classRepositoryItemWnd);
    m3d::g_Kernel->AddClass(&ComplexModelWnd::m_classComplexModelWnd);
    m3d::g_Kernel->AddClass(&VideoWnd::m_classVideoWnd);
    m3d::g_Kernel->AddClass(&ItemModelWnd::m_classItemModelWnd);
    m3d::g_Kernel->AddClass(&ScreenWnd::m_classScreenWnd);
    m3d::g_Kernel->AddClass(&TakSebeQuestLogWnd::m_classTakSebeQuestLogWnd);
    m3d::g_Kernel->AddClass(&QuestItem::m_classQuestItem);
    m3d::g_Kernel->AddClass(&QuestItemList::m_classQuestItemList);
    m3d::g_Kernel->AddClass(&QuestStatusBtn::m_classQuestStatusBtn);
    m3d::g_Kernel->AddClass(&QuestLabel::m_classQuestLabel);
    m3d::g_Kernel->AddClass(&QuestDizWnd::m_classQuestDizWnd);
    m3d::g_Kernel->AddClass(&MapComboBox::m_classMapComboBox);
    m3d::g_Kernel->AddClass(&LocalMapWnd::m_classLocalMapWnd);
    m3d::g_Kernel->AddClass(&LocalChartWnd::m_classLocalChartWnd);
    m3d::g_Kernel->AddClass(&MapMarkWnd::m_classMapMarkWnd);
    m3d::g_Kernel->AddClass(&MapMarkIcoWnd::m_classMapMarkIcoWnd);
    m3d::g_Kernel->AddClass(&MapMarkNameWnd::m_classMapMarkNameWnd); 
    m3d::g_Kernel->AddClass(&PlayerMarkWnd::m_classPlayerMarkWnd);
    m3d::g_Kernel->AddClass(&MapInfoPanel::m_classMapInfoPanel);
    m3d::g_Kernel->AddClass(&MapSellItem::m_classMapSellItem);
    m3d::g_Kernel->AddClass(&MapSellList::m_classMapSellList);
    m3d::g_Kernel->AddClass(&SaveSellList::m_classSaveSellList);
    m3d::g_Kernel->AddClass(&SaveSellButton::m_classSaveSellButton); 
    m3d::g_Kernel->AddClass(&TalkWithNpcDlg::m_classTalkWithNpcDlg);
    m3d::g_Kernel->AddClass(&NpcModelWnd::m_classNpcModelWnd);
    m3d::g_Kernel->AddClass(&NavPointMarkWnd::m_classNavPointMarkWnd);
    m3d::g_Kernel->AddClass(&WeaponGroupChoiceDlg::m_classWeaponGroupChoiceDlg);
    m3d::g_Kernel->AddClass(&CBWnd::m_classCBWnd);
    m3d::g_Kernel->AddClass(&CabinWnd::m_classCabinWnd);
    m3d::g_Kernel->AddClass(&BasketWnd::m_classBasketWnd);
    m3d::g_Kernel->AddClass(&ZnayuKakProdatWnd::m_classZnayuKakProdatWnd);
    m3d::g_Kernel->AddClass(&BuildingButton::m_classBuildingButton);
    m3d::g_Kernel->AddClass(&BuildingList::m_classBuildingList);
    m3d::g_Kernel->AddClass(&NpcButton::m_classNpcButton);
    m3d::g_Kernel->AddClass(&NpcList::m_classNpcList);
    m3d::g_Kernel->AddClass(&ContextModelWnd::m_classContextModelWnd);
    m3d::g_Kernel->AddClass(&LevelInfoManager::m_classLevelInfoManager);
    m3d::g_Kernel->AddClass(&GlobalMapWnd::m_classGlobalMapWnd);
    m3d::g_Kernel->AddClass(&LevelIco::m_classLevelIco);
    m3d::g_Kernel->AddClass(&StatsWnd::m_classStatsWnd);
    m3d::g_Kernel->AddClass(&StatsButton::m_classStatsButton);
    m3d::g_Kernel->AddClass(&StatsList::m_classStatsList);
    m3d::g_Kernel->AddClass(&ReputationWnd::m_classReputationWnd);
    m3d::g_Kernel->AddClass(&ReputationButton::m_classReputationButton);
    m3d::g_Kernel->AddClass(&ReputationList::m_classReputationList);
    m3d::g_Kernel->AddClass(&SplashWnd::m_classSplashWnd);
    m3d::g_Kernel->AddClass(&NewProfileWnd::m_classNewProfileWnd);
    m3d::g_Kernel->AddClass(&ChangeProfileWnd::m_classChangeProfileWnd);
    m3d::g_Kernel->AddClass(&LSWnd::m_classLSWnd);
    m3d::g_Kernel->AddClass(&SaveButton::m_classSaveButton);
    m3d::g_Kernel->AddClass(&SaveList::m_classSaveList);
    m3d::g_Kernel->AddClass(&SaveWnd::m_classSaveWnd);
    m3d::g_Kernel->AddClass(&LoadWnd::m_classLoadWnd);
    m3d::g_Kernel->AddClass(&SavesManager::m_classSavesManager);
    m3d::g_Kernel->AddClass(&TargetInfoWnd::m_classTargetInfoWnd);
    m3d::g_Kernel->AddClass(&SmartCursorWnd::m_classSmartCursorWnd);
    m3d::g_Kernel->AddClass(&VehicleInfoPanel::m_classVehicleInfoPanel);
    m3d::g_Kernel->AddClass(&FadingMsgList::m_classFadingMsgList);
    m3d::g_Kernel->AddClass(&FadingMsgItem::m_classFadingMsgItem);
    m3d::g_Kernel->AddClass(&MsgManager::m_classMsgManager);
    m3d::g_Kernel->AddClass(&GarageWnd::m_classGarageWnd);
    m3d::g_Kernel->AddClass(&AdvancedButton::m_classAdvancedButton);
    m3d::g_Kernel->AddClass(&AdvancedList::m_classAdvancedList);
    m3d::g_Kernel->AddClass(&RefuelButton::m_classRefuelButton);
    m3d::g_Kernel->AddClass(&RefuelList::m_classRefuelList);
    m3d::g_Kernel->AddClass(&RepairButton::m_classRepairButton);
    m3d::g_Kernel->AddClass(&RepairList::m_classRepairList);
    m3d::g_Kernel->AddClass(&RechargeButton::m_classRechargeButton);
    m3d::g_Kernel->AddClass(&RechargeList::m_classRechargeList);
    m3d::g_Kernel->AddClass(&BuyServiceSlider::m_classBuyServiceSlider);
    m3d::g_Kernel->AddClass(&CBButton::m_classCBButton);
    m3d::g_Kernel->AddClass(&CBList::m_classCBList);
    m3d::g_Kernel->AddClass(&CabinButton::m_classCabinButton);
    m3d::g_Kernel->AddClass(&CabinList::m_classCabinList);
    m3d::g_Kernel->AddClass(&BasketButton::m_classBasketButton);
    m3d::g_Kernel->AddClass(&BasketList::m_classBasketList);
    m3d::g_Kernel->AddClass(&VehicleWnd::m_classVehicleWnd);
    m3d::g_Kernel->AddClass(&PlayerVehicleWnd::m_classPlayerVehicleWnd);
    m3d::g_Kernel->AddClass(&WorkshopVehicleWnd::m_classWorkshopVehicleWnd);
    m3d::g_Kernel->AddClass(&SkinsWnd::m_classSkinsWnd);
    m3d::g_Kernel->AddClass(&IgrokaMochatWnd::m_classIgrokaMochatWnd);
    m3d::g_Kernel->AddClass(&QuestItemsWnd::m_classQuestItemsWnd);
    m3d::g_Kernel->AddClass(&WareWnd::m_classWareWnd);
    m3d::g_Kernel->AddClass(&WareList::m_classWareList);
    m3d::g_Kernel->AddClass(&WareItem::m_classWareItem);
    m3d::g_Kernel->AddClass(&MotherPanelTabButton::m_classMotherPanelTabButton);
    m3d::g_Kernel->AddClass(&MainCursorWnd::m_classMainCursorWnd);
    m3d::g_Kernel->AddClass(&TargetCursorWnd::m_classTargetCursorWnd);
    m3d::g_Kernel->AddClass(&TargetCapturingCursorWnd::m_classTargetCapturingCursorWnd);
    m3d::g_Kernel->AddClass(&TargetCapturedCursorWnd::m_classTargetCapturedCursorWnd);
    m3d::g_Kernel->AddClass(&CounterWnd::m_classCounterWnd);
    m3d::g_Kernel->AddClass(&OptionsWnd::m_classOptionsWnd);
    m3d::g_Kernel->AddClass(&OptionTabButton::m_classOptionTabButton);
    m3d::g_Kernel->AddClass(&VideoOptionsWnd::m_classVideoOptionsWnd);
    m3d::g_Kernel->AddClass(&SoundOptionsWnd::m_classSoundOptionsWnd);
    m3d::g_Kernel->AddClass(&GameOptionsWnd::m_classGameOptionsWnd);
    m3d::g_Kernel->AddClass(&ControlOptionsWnd::m_classControlOptionsWnd);
    m3d::g_Kernel->AddClass(&NavPointButton::m_classNavPointButton);
    m3d::g_Kernel->AddClass(&WeaponGroupManager::m_classWeaponGroupManager);
    m3d::g_Kernel->AddClass(&WeaponGroup::m_classWeaponGroup);
    m3d::g_Kernel->AddClass(&GameMenuWnd::m_classGameMenuWnd);
    m3d::g_Kernel->AddClass(&MainGameInterfaceWnd::m_classMainGameInterfaceWnd);
    m3d::g_Kernel->AddClass(&ElectronicDigitalWnd::m_classElectronicDigitalWnd);
    m3d::g_Kernel->AddClass(&TwinklingLampWnd::m_classTwinklingLampWnd);
    m3d::g_Kernel->AddClass(&CharacteristicsWnd::m_classCharacteristicsWnd);
    m3d::g_Kernel->AddClass(&CBCharacteristicsWnd::m_classCBCharacteristicsWnd);
    m3d::g_Kernel->AddClass(&CabinCharacteristicsWnd::m_classCabinCharacteristicsWnd);
    m3d::g_Kernel->AddClass(&BasketCharacteristicsWnd::m_classBasketCharacteristicsWnd);
    m3d::g_Kernel->AddClass(&VehicleCharacteristicsWnd::m_classVehicleCharacteristicsWnd);
    m3d::g_Kernel->AddClass(&WeaponSlotWnd::m_classWeaponSlotWnd);
    m3d::g_Kernel->AddClass(&WeaponSlotList::m_classWeaponSlotList);
    m3d::g_Kernel->AddClass(&WeaponGroupButton::m_classWeaponGroupButton);
    m3d::g_Kernel->AddClass(&WeaponGroupButtonList::m_classWeaponGroupButtonList);
    m3d::g_Kernel->AddClass(&DemoSplashWnd::m_classDemoSplashWnd);
    m3d::g_Kernel->AddClass(&CreditsWnd::m_classCreditsWnd);
    m3d::g_Kernel->AddClass(&FadePanelBeforeNextMap::m_classFadePanelBeforeNextMap);
    m3d::g_Kernel->AddClass(&LevelConnectionsWnd::m_classLevelConnectionsWnd);
    m3d::g_Kernel->AddClass(&HelpManager::m_classHelpManager);
    m3d::g_Kernel->AddClass(&HelpWnd::m_classHelpWnd);
    m3d::g_Kernel->AddClass(&HelpSimpleMsgWnd::m_classHelpSimpleMsgWnd);
    m3d::g_Kernel->AddClass(&ResourceInfo::m_classResourceInfo);
    m3d::g_Kernel->AddClass(&WindowResourceInfo::m_classWindowResourceInfo);
    m3d::g_Kernel->AddClass(&IcoResourceInfo::m_classIcoResourceInfo);
    m3d::g_Kernel->AddClass(&AutoScrollTextWnd::m_classAutoScrollTextWnd);
    m3d::g_Kernel->AddClass(&HelpInMainGameInterfaceWnd::m_classHelpInMainGameInterfaceWnd);
    m3d::g_Kernel->AddClass(&MsgBox::m_classMsgBox);
    m3d::g_Kernel->AddClass(&BossIndicatorWnd::m_classBossIndicatorWnd);
    m3d::g_Kernel->AddClass(&RequestDifficultyWnd::m_classRequestDifficultyWnd);
}

void TruxxUiManager::OnChangeGameMenuMode()
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::GUI_UnregisterCVars()
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::SetGameMenuMode(bool)
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::GUI_BeginModalDlg(bool forcePause, bool forceModal)
{
    if (!forceModal)
    {
        if (M3D_APP->HasChildModalRunning() || GetLevelInfoManager())
            return 0;
    }
    auto wnd = M3D_APP->m_pInterfaceManager->GetWindow(72);
    if (!wnd || !wnd->IsChildOf(M3D_APP))
    {
        if (forcePause)
            M3D_APP->Pause();
    }
    M3D_APP->m_pImpulses->ResetAllImpulses(false);
    M3D_APP->m_gameSlideAuto.x = 0.0;
    M3D_APP->m_gameSlideAuto.y = 0.0;
    return 1;
}

int TruxxUiManager::GUI_RegisterScriptGlobals()
{
    throw std::logic_error("Not implemented");
}
