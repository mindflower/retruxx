#include "truxxuimanager.h"

#include "uidefs.h"

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
#include "server/processmanager.h"
#include "ui/edit.h"

namespace
{
    struct _Str2GuiId
    {
        CStr m_strId;
        int m_intId;
    };

    _Str2GuiId l_str2GuiId[] = {
        {"IW_DLG_BINDKEYS", IW_DLG_BINDKEYS},
        {"IW_WND_PLAYER_INVENTORY", IW_WND_PLAYER_INVENTORY},
        {"IW_WND_WORKSHOP_INVENTORY", IW_WND_WORKSHOP_INVENTORY},
        {"IW_DLG_DEVIDE", IW_DLG_DEVIDE},
        {"IW_DLG_BAR", IW_DLG_BAR},
        {"IW_DLG_ADDITIONAL_BUILDING", IW_DLG_ADDITIONAL_BUILDING},
        {"IW_DLG_TOWN", IW_DLG_TOWN},
        {"IW_DLG_PALMBOARD", IW_DLG_PALMBOARD},
        {"IW_WND_WORKSHOP", IW_WND_WORKSHOP},
        {"IW_DLG_ADMINISTRATION", IW_DLG_ADMINISTRATION},
        {"IW_WND_SHOP", IW_WND_SHOP},
        {"IW_WND_RADAR", IW_WND_RADAR},
        {"IW_WND_CINEMA_PANEL", IW_WND_CINEMA_PANEL},
        {"IW_WND_CINEMA_FADE_PANEL", IW_WND_CINEMA_FADE_PANEL},
        {"IW_WND_DAMAGEINFO", IW_WND_DAMAGEINFO},
        {"IW_WND_DAMAGEINFO_IN_CHARACTERISTIC_WND_LEFT", IW_WND_DAMAGEINFO_IN_CHARACTERISTIC_WND_LEFT},
        {"IW_WND_DAMAGEINFO_IN_CHARACTERISTIC_WND_RIGHT", IW_WND_DAMAGEINFO_IN_CHARACTERISTIC_WND_RIGHT},
        {"IW_WND_WEAPON_INFO_LIST", IW_WND_WEAPON_INFO_LIST},
        {"IW_WND_WEAPON_INFO_LIST_IN_FIGNYA_WND", IW_WND_WEAPON_INFO_LIST_IN_FIGNYA_WND},
        {"IW_WND_WEAPON_INFO_LIST_IN_CHAR_WND_LEFT", IW_WND_WEAPON_INFO_LIST_IN_CHAR_WND_LEFT},
        {"IW_WND_WEAPON_INFO_LIST_IN_CHAR_WND_RIGHT", IW_WND_WEAPON_INFO_LIST_IN_CHAR_WND_RIGHT},
        {"IW_WND_WEAPON_INFO_LIST_IN_CHAR_WND_WORKSHOP", IW_WND_WEAPON_INFO_LIST_IN_CHAR_WND_WORKSHOP},
        {"IW_WND_GROUND", IW_WND_GROUND},
        {"IW_DLG_MOTHER_PANEL", IW_DLG_MOTHER_PANEL},
        {"IW_WND_CHARACTERISTICS_LEFT", IW_WND_CHARACTERISTICS_LEFT},
        {"IW_WND_CHARACTERISTICS_RIGHT", IW_WND_CHARACTERISTICS_RIGHT},
        {"IW_WND_PLAYER_MONEY", IW_WND_PLAYER_MONEY},
        {"IW_WND_PLAYER_MONEY_IN_NPC_DIALOG", IW_WND_PLAYER_MONEY_IN_NPC_DIALOG},
        {"IW_WND_MAINMENU", IW_WND_MAINMENU},
        {"IW_WND_BUY_VEHICLE", IW_WND_BUY_VEHICLE},
        {"IW_WND_VIDEO", IW_WND_VIDEO},
        {"IW_WND_QUESTLOG", IW_WND_QUESTLOG},
        {"IW_WND_LOCAL_MAP", IW_WND_LOCAL_MAP},
        {"IW_WND_GLOBAL_MAP", IW_WND_GLOBAL_MAP},
        {"IW_DLG_TALK_WITH_NPC", IW_DLG_TALK_WITH_NPC},
        {"IW_DLG_WEAPON_GROUP_CHOICE", IW_DLG_WEAPON_GROUP_CHOICE},
        {"IW_WND_PLAYER_CABIN", IW_WND_PLAYER_CABIN},
        {"IW_WND_PLAYER_BASKET", IW_WND_PLAYER_BASKET},
        {"IW_WND_WORKSHOP_CABIN", IW_WND_WORKSHOP_CABIN},
        {"IW_WND_WORKSHOP_BASKET", IW_WND_WORKSHOP_BASKET},
        {"IW_WND_ZNAYU_KAK_PRODAT", IW_WND_ZNAYU_KAK_PRODAT},
        {"IW_WND_JOURNAL", IW_WND_JOURNAL},
        {"IW_WND_HISTORY", IW_WND_HISTORY},
        {"IW_WND_BOOKS", IW_WND_BOOKS},
        {"IW_WND_ENCYCLOPAEDIA", IW_WND_ENCYCLOPAEDIA},
        {"IW_WND_PLAYER", IW_WND_PLAYER},
        {"IW_WND_STATS", IW_WND_STATS},
        {"IW_WND_REPUTATION", IW_WND_REPUTATION},
        {"IW_WND_PERKS", IW_WND_PERKS},
        {"IW_WND_SPLASH", IW_WND_SPLASH},
        {"IW_WND_NEW_PROFILE", IW_WND_NEW_PROFILE},
        {"IW_WND_NEW_PROFILE_ALONE", IW_WND_NEW_PROFILE_ALONE},
        {"IW_WND_CHANGE_PROFILE", IW_WND_CHANGE_PROFILE},
        {"IW_WND_LOAD", IW_WND_LOAD},
        {"IW_WND_SAVE", IW_WND_SAVE},
        {"IW_WND_FIGNYA_V_CENTRE", IW_WND_FIGNYA_V_CENTRE},
        {"IW_WND_FIGNYA_V_CENTRE_INACTIVE", IW_WND_FIGNYA_V_CENTRE_INACTIVE},
        {"IW_WND_TARGET_INFO_IN_MAIN_INTERFACE", IW_WND_TARGET_INFO_IN_MAIN_INTERFACE},
        {"IW_WND_TARGET_INFO_IN_FIGNYA_WND", IW_WND_TARGET_INFO_IN_FIGNYA_WND},
        {"IW_WND_CURSOR_MAIN", IW_WND_CURSOR_MAIN},
        {"IW_WND_CURSOR_TARGET_CAPTURING", IW_WND_CURSOR_TARGET_CAPTURING},
        {"IW_WND_CURSOR_TARGET_CAPTURED", IW_WND_CURSOR_TARGET_CAPTURED},
        {"IW_WND_CURSOR_TARGET_INFO", IW_WND_CURSOR_TARGET_INFO},
        {"IW_WND_VEHICLE_INFO_PANEL", IW_WND_VEHICLE_INFO_PANEL},
        {"IW_WND_SPEEDOMETER", IW_WND_SPEEDOMETER},
        {"IW_WND_FADING_MSG_LIST", IW_WND_FADING_MSG_LIST},
        {"IW_WND_IMPORTANT_FADING_MSG_LIST", IW_WND_IMPORTANT_FADING_MSG_LIST},
        {"IW_WND_GARAGE", IW_WND_GARAGE},
        {"IW_WND_REFUEL_LIST", IW_WND_REFUEL_LIST},
        {"IW_WND_REPAIR_LIST", IW_WND_REPAIR_LIST},
        {"IW_WND_RECHARGE_LIST", IW_WND_RECHARGE_LIST},
        {"IW_WND_CABIN_BUTTON_PATTERN", IW_WND_CABIN_BUTTON_PATTERN},
        {"IW_WND_BASKET_BUTTON_PATTERN", IW_WND_BASKET_BUTTON_PATTERN},
        {"IW_WND_CABIN_LIST", IW_WND_CABIN_LIST},
        {"IW_WND_BASKET_LIST", IW_WND_BASKET_LIST},
        {"IW_WND_SKINS", IW_WND_SKINS},
        {"IW_WND_IGROKA_MOCHAT", IW_WND_IGROKA_MOCHAT},
        {"IW_WND_QUEST_ITEMS", IW_WND_QUEST_ITEMS},
        {"IW_WND_COUNTER", IW_WND_COUNTER},
        {"IW_WND_OPTIONS", IW_WND_OPTIONS},
        {"IW_WND_OPTIONS_VIDEO", IW_WND_OPTIONS_VIDEO},
        {"IW_WND_OPTIONS_SOUND", IW_WND_OPTIONS_SOUND},
        {"IW_WND_OPTIONS_CONTROL", IW_WND_OPTIONS_CONTROL},
        {"IW_WND_OPTIONS_GAME", IW_WND_OPTIONS_GAME},
        {"IW_WND_MAIN_GAME_INTERFACE", IW_WND_MAIN_GAME_INTERFACE},
        {"IW_WND_PLAYER_VEHICLE_CHARACTERISTICS_LEFT", IW_WND_PLAYER_VEHICLE_CHARACTERISTICS_LEFT},
        {"IW_WND_PLAYER_VEHICLE_CHARACTERISTICS_RIGHT", IW_WND_PLAYER_VEHICLE_CHARACTERISTICS_RIGHT},
        {"IW_WND_WORKSHOP_VEHICLE_CHARACTERISTICS", IW_WND_WORKSHOP_VEHICLE_CHARACTERISTICS},
        {"IW_WND_PLAYER_CABIN_CHARACTERISTICS_LEFT", IW_WND_PLAYER_CABIN_CHARACTERISTICS_LEFT},
        {"IW_WND_PLAYER_CABIN_CHARACTERISTICS_RIGHT", IW_WND_PLAYER_CABIN_CHARACTERISTICS_RIGHT},
        {"IW_WND_WORKSHOP_CABIN_CHARACTERISTICS", IW_WND_WORKSHOP_CABIN_CHARACTERISTICS},
        {"IW_WND_PLAYER_BASKET_CHARACTERISTICS_LEFT", IW_WND_PLAYER_BASKET_CHARACTERISTICS_LEFT},
        {"IW_WND_PLAYER_BASKET_CHARACTERISTICS_RIGHT", IW_WND_PLAYER_BASKET_CHARACTERISTICS_RIGHT},
        {"IW_WND_WORKSHOP_BASKET_CHARACTERISTICS", IW_WND_WORKSHOP_BASKET_CHARACTERISTICS},
        {"IW_WND_DEMO_SPLASH", IW_WND_DEMO_SPLASH},
        {"IW_WND_CREDITS", IW_WND_CREDITS},
        {"IW_WND_FADE_PANEL_BEFORE_NEXT_MAP", IW_WND_FADE_PANEL_BEFORE_NEXT_MAP},
        {"IW_WND_HELP_SIMPLE_MSG", IW_WND_HELP_SIMPLE_MSG},
        {"IW_WND_HELP_MAIN_GAME_INTERFACE", IW_WND_HELP_MAIN_GAME_INTERFACE},
        {"IW_WND_MSG_BOX", IW_WND_MSG_BOX},
        {"IW_WND_BOSS_INDICATOR", IW_WND_BOSS_INDICATOR},
        {"IW_WND_REQUEST_DIFFICULTY_LEVEL", IW_WND_REQUEST_DIFFICULTY_LEVEL},
    };
}  // namespace

CStr TruxxUiManager::GetPathToDialogsFileGlobal() const
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

MsgManager* TruxxUiManager::GetMsgManager() const
{
    return m_msgManager;
}

ObjectCollection const& TruxxUiManager::GetObjectCollection() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool TruxxUiManager::IsHidden() const
{
    RETRUXX_NOT_IMPLEMENTED;
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
    return m_cvPathToQuestInfo.GetS();
}

int TruxxUiManager::HandleImpulse(m3d::AuxImpulseInfo const& impInfo, m3d::ui::Wnd* causeWnd)
{
    // TODO: check this and refactor
    if ((this->IsWindowVisible(166) || this->IsWindowVisible(19)) && !this->GUI_IsModalEqualWndRunning())
    {
        return 0;
    }

    m3d::ui::Wnd* v5 = 0;
    if (causeWnd)
    {
        auto Station = M3D_APP->GetStation();
        if (Station->HasChildModalRunning())
        {
            auto TopModal = Station->GetTopModal();
            if (TopModal)
                v5 = TopModal;
        }
        else if (this->GUI_IsWndModalEqual(causeWnd) || causeWnd->IsKindOf(&m3d::ui::EditWnd::m_classEditWnd))
        {
            v5 = causeWnd;
        }
    }

    int v9 = 0;
    if (impInfo.m_state)
    {
        auto Station = M3D_APP->GetStation();
        if (!Station->HasChildModalRunning() || v5)
            v9 = this->GUI_ProcessEvent(GUI_EVENT_FROM_IMPULSE, impInfo.m_impId, (void*)&impInfo, v5);
    }

    auto v11 = !this->GUI_IsModalEqualWndRunning();
    if (v11)
        return v9;
    return 1;
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
    RETRUXX_NOT_IMPLEMENTED;
}

bool TruxxUiManager::IsWindowVisibleAndNotAnimating(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TruxxUiManager::Load(ref_ptr<m3d::cmn::XmlFile>, ref_ptr<m3d::cmn::XmlNode>)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
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
    ref_ptr wndSplash = M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_SPLASH);
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
    RETRUXX_NOT_IMPLEMENTED;
}

StringParser const& TruxxUiManager::GetStringParser() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Workshop* TruxxUiManager::GetCurrentShop() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TruxxUiManager::AddFadingMsgByStrId(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam>> const&)
    const
{
    // TODO implement TruxxUiManager::AddFadingMsgByStrId
    // RETRUXX_NOT_IMPLEMENTED;
}

NavPointManager* TruxxUiManager::GetNavPointManager() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Workshop* TruxxUiManager::GetCurrentWorkshop() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TruxxUiManager::AddFadingMsg(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam>> const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool TruxxUiManager::IsGameModeValidForSmartCursor(GameState mode) const
{
    return mode == GS_GAME || mode == GS_CINEMATIC;
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
    RETRUXX_NOT_IMPLEMENTED;
}

std::vector<int, std::allocator<int>> const& TruxxUiManager::GetTakenQuestIds() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

RepliesManager* TruxxUiManager::GetRepliesManager() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TruxxUiManager::Reset(bool beforeContinuousLevel)
{
    auto clearRes = GUI_Clear(beforeContinuousLevel);
    // TODO: check this
    m_msgManager->Clear(true);
    m_helpManager->HideCurrentHelpWindow();
    if (!beforeContinuousLevel)
    {
        m_takenQuestIds.clear();
        m_navPointManager->Clear();
        m_weaponGroupManager->Clear();
    }
    m_currentTownId = -1;
    m_bInGameMenuMode = false;
    m_bIsPlayerDead = false;
    return clearRes != 0;
}

WeaponGroupManager* TruxxUiManager::GetWeaponGroupManager() const
{
    return m_weaponGroupManager;
}

void TruxxUiManager::AddImportantFadingMsgByStrId(
    CStr const&,
    std::vector<m3d::AIParam, std::allocator<m3d::AIParam>> const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
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

    int retVal = m3d::ui::MBX_RET_CANCEL;
    ;
    ShowWindow(guiId, true, true, true, bPause, &retVal);
    RemoveWindow(guiId);
    GUI_EndModalDlg();
    return static_cast<m3d::ui::MbRetCodes>(retVal);
}

bool TruxxUiManager::IsInSaleMode() const
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

TruxxUiManager::~TruxxUiManager()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr TruxxUiManager::GetPathToSplashes() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TruxxUiManager::Update()
{
    auto const res = LaunchEvent(IE_CUST_NEW_FRAME, GUI_EVENT_CUSTOM, nullptr) & 1;
    return res & LaunchEvent(IE_CUST_NEW_FRAME_FORCE, GUI_EVENT_CUSTOM, nullptr);
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
    RETRUXX_NOT_IMPLEMENTED;
}

int TruxxUiManager::Save(ref_ptr<m3d::cmn::XmlFile>, ref_ptr<m3d::cmn::XmlNode>)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TruxxUiManager::StartSplashing(int numSplashes) const
{
    auto wnd = M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_SPLASH);
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
    RETRUXX_NOT_IMPLEMENTED;
}

void TruxxUiManager::AddImportantFadingMsg(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam>> const&)
    const
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

int TruxxUiManager::IncRef()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TruxxUiManager::GUI_UnRegisterScriptGlobals()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TruxxUiManager::ShowGameMenu(CStr const& levelName)
{
    if (!levelName.empty())
    {
        PrepareMenuForShow(levelName);
        ShowWindow(IW_WND_GAME_MENU, true, false, false, true, nullptr);
        return;
    }

    if (ref_ptr<m3d::ui::Wnd> wnd = GetWindow(IW_WND_GAME_MENU))
    {
        if (auto* menuWnd = RT_DYNCAST(wnd.get(), GameMenuWnd))
        {
            CStr const itemName = menuWnd->GetCurrentLevelName();
            PrepareMenuForShow(levelName);
            ShowWindow(IW_WND_GAME_MENU, true, false, false, true, nullptr);
        }
    }
}

void TruxxUiManager::OnGameModeChanged(void* data)
{
    if (data)
    {
        auto const ev = static_cast<m3d::Event*>(data);
        M3D_LOG_INFO("Game mode changed. Old mode = " + CStr(ev->m_intEv[1]) + ", new mode = " + CStr(ev->m_intEv[0]));
        if (ev->m_intEv[0] == 0 && ev->m_intEv[1] == 1 && m_bIsPlayerDead)
        {
            M3D_APP->EnqueueMessage(UM_SHOWGAMEMENU, 0, 0, 0, 0, GameMenuWnd::DEATH_LEVEL_NAME, {});
        }
    }
}

void TruxxUiManager::OnBeforeStartLevel()
{
    //TODO: check and refactor this
    bool bOldFirstLevelResourcesLoaded = m_bFirstLevelResourcesLoaded;
    if (!m_bFirstLevelResourcesLoaded && !GUI_IsCurrentLevelMainMenuLevel())
    {
        if (m_repliesManager)
        {
            m_repliesManager->Init();
        }

        if (m_navPointManager)
        {
            m_navPointManager->Init();
        }

        if (m_weaponGroupManager)
        {
            m_weaponGroupManager->Init();
        }

        if (m_savesManager)
        {
            m_savesManager->Init();
        }

        if (m_msgManager)
        {
            m_msgManager->Init(true);
        }
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

int TruxxUiManager::GUI_BindWindowsToEvents()
{
    int res = 1;

    res &= GUI_SetEventsForWindow(IW_DLG_BINDKEYS, {IE_EV_EV_KEYBINDINGS_CHANGED});

    res &= GUI_SetEventsForWindow(IW_WND_PLAYER_INVENTORY, {IE_EV_SM_PLAYER_VEHICLE_CHANGED});

    res &= GUI_SetEventsForWindow(
        IW_WND_PLAYER_CABIN,
        {IE_EV_SM_VEHICLEPART_CHANGED, IE_EV_UM_GLOBAL_MAP, IE_EV_UM_GADGET_DEACTIVATE, IE_EV_UM_FINISH_TRADE});

    res &= GUI_SetEventsForWindow(
        IW_WND_WORKSHOP_CABIN,
        {IE_EV_SM_VEHICLEPART_CHANGED, IE_EV_UM_GLOBAL_MAP, IE_EV_UM_GADGET_DEACTIVATE, IE_EV_UM_FINISH_TRADE});

    res &= GUI_SetEventsForWindow(
        IW_WND_PLAYER_BASKET,
        {IE_EV_SM_VEHICLEPART_CHANGED, IE_EV_UM_GLOBAL_MAP, IE_CUST_SALETAB_CHANGED, IE_EV_UM_FINISH_TRADE});

    res &= GUI_SetEventsForWindow(
        IW_WND_WORKSHOP_BASKET,
        {IE_EV_SM_VEHICLEPART_CHANGED, IE_EV_UM_GLOBAL_MAP, IE_EV_UM_GADGET_DEACTIVATE, IE_EV_UM_FINISH_TRADE});

    res &= GUI_SetEventsForWindow(
        IW_WND_WORKSHOP_BASKET,
        {IE_EV_SM_VEHICLEPART_CHANGED, IE_EV_UM_GLOBAL_MAP, IE_EV_UM_GADGET_DEACTIVATE, IE_EV_UM_FINISH_TRADE});

    res &= GUI_SetEventsForWindow(
        IW_WND_PLAYERVEHICLE_INVENTORY,
        {IE_CUST_DD_DRAGITEM_MOVE,
         IE_EV_SM_REPOSITORY_CHANGED,
         IE_CUST_DD_DROP,
         IE_CUST_DD_ITEM_QUICK_DROP,
         IE_CUST_DD_DRAGITEM_ACCEPTED,
         IE_EV_SM_VEHICLEPART_CHANGED,
         IE_EV_SM_PLAYER_MONEY_CHANGED,
         IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(
        IW_WND_WORKSHOPVEHICLE_INVENTORY,
        {IE_CUST_DD_DRAGITEM_MOVE,
         IE_EV_SM_REPOSITORY_CHANGED,
         IE_CUST_DD_DROP,
         IE_CUST_DD_ITEM_QUICK_DROP,
         IE_CUST_DD_DRAGITEM_ACCEPTED,
         IE_EV_SM_VEHICLEPART_CHANGED,
         IE_EV_SM_PLAYER_MONEY_CHANGED,
         IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(
        IW_WND_GROUND_INVENTORY,
        {IE_CUST_DD_DRAGITEM_MOVE,
         IE_EV_SM_REPOSITORY_CHANGED,
         IE_CUST_DD_DROP,
         IE_CUST_DD_ITEM_QUICK_DROP,
         IE_CUST_DD_DRAGITEM_ACCEPTED,
         IE_EV_SM_VEHICLEPART_CHANGED,
         IE_EV_SM_PLAYER_MONEY_CHANGED,
         IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(
        IW_WND_SALE_CABINS_AND_BASKETS,
        {IE_CUST_DD_DRAGITEM_MOVE,
         IE_EV_SM_REPOSITORY_CHANGED,
         IE_CUST_DD_DROP,
         IE_CUST_DD_ITEM_QUICK_DROP,
         IE_CUST_DD_DRAGITEM_ACCEPTED,
         IE_EV_SM_VEHICLEPART_CHANGED,
         IE_EV_SM_PLAYER_MONEY_CHANGED,
         IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(
        IW_WND_VEHICLEPART_CABIN,
        {IE_CUST_DD_DRAGITEM_MOVE,
         IE_CUST_DD_START_DRAG,
         IE_CUST_DD_END_DRAG,
         IE_CUST_DD_MOUSE_IN,
         IE_CUST_DD_MOUSE_OUT,
         IE_CUST_DD_DRAGITEM_ACCEPTED,
         IE_CUST_DD_ITEM_QUICK_DROP,
         IE_CUST_DD_DROP,
         IE_EV_SM_VEHICLEPART_CHANGED});

    res &= GUI_SetEventsForWindow(
        IW_WND_SHOP, {IE_EV_SM_PLAYER_VEHICLE_CHANGED, IE_CUST_DD_DRAGITEM_ACCEPTED, IE_CUST_DD_DROP});

    res &= GUI_SetEventsForWindow(
        IW_WND_RADAR, {IE_CUST_NEW_FRAME, IE_CUST_START_LEVEL, IE_EV_UM_NAVPOINT_ADDED, IE_EV_UM_NAVPOINT_DELETED});

    res &= GUI_SetEventsForWindow(IW_WND_PLAYERVEHICLE_HEALTH, {IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(IW_WND_PLAYERVEHICLE_HEALTH_IN_FIGNYA_WND, {IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(IW_WND_HEALTH_IN_CHARACTERISTIC_WND_LEFT, {IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(IW_WND_HEALTH_IN_CHARACTERISTIC_WND_RIGHT, {IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(IW_WND_HEALTH_IN_WORKSHOP_VEHICLE_WND, {IE_CUST_NEW_FRAME});

    //TODO: check this
    res &= GUI_SetEventsForWindow(
        IW_WND_PLAYERVEHICLE_CABIN_DURABILITY, {IE_CUST_NEW_FRAME, IE_EV_SM_VEHICLEPART_CHANGED});

    res &= GUI_SetEventsForWindow(
        IW_WND_PLAYERVEHICLE_BASKET_DURABILITY, {IE_CUST_NEW_FRAME, IE_EV_SM_VEHICLEPART_CHANGED});

    res &= GUI_SetEventsForWindow(
        IW_WND_PLAYERVEHICLE_CABIN_DURABILITY_IN_FIGNYA_WND, {IE_CUST_NEW_FRAME, IE_EV_SM_VEHICLEPART_CHANGED});

    res &= GUI_SetEventsForWindow(
        IW_WND_PLAYERVEHICLE_BASKET_DURABILITY_IN_FIGNYA_WND, {IE_CUST_NEW_FRAME, IE_EV_SM_VEHICLEPART_CHANGED});

    res &= GUI_SetEventsForWindow(
        IW_WND_CABIN_DURABILITY_IN_CHARACTERISTIC_WND_LEFT, {IE_CUST_NEW_FRAME, IE_EV_SM_VEHICLEPART_CHANGED});

    res &= GUI_SetEventsForWindow(
        IW_WND_CABIN_DURABILITY_IN_CHARACTERISTIC_WND_RIGHT, {IE_CUST_NEW_FRAME, IE_EV_SM_VEHICLEPART_CHANGED});

    res &= GUI_SetEventsForWindow(
        IW_WND_CABIN_DURABILITY_IN_WORKSHOP_VEHICLE_WND, {IE_CUST_NEW_FRAME, IE_EV_SM_VEHICLEPART_CHANGED});

    res &= GUI_SetEventsForWindow(
        IW_WND_BASKET_DURABILITY_IN_CHARACTERISTIC_WND_LEFT, {IE_CUST_NEW_FRAME, IE_EV_SM_VEHICLEPART_CHANGED});

    res &= GUI_SetEventsForWindow(
        IW_WND_BASKET_DURABILITY_IN_CHARACTERISTIC_WND_RIGHT, {IE_CUST_NEW_FRAME, IE_EV_SM_VEHICLEPART_CHANGED});

    res &= GUI_SetEventsForWindow(
        IW_WND_BASKET_DURABILITY_IN_WORKSHOP_VEHICLE_WND, {IE_CUST_NEW_FRAME, IE_EV_SM_VEHICLEPART_CHANGED});

    res &= GUI_SetEventsForWindow(IW_WND_PLAYERVEHICLE_FUEL, {IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(IW_WND_SPEEDOMETER, {IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(IW_WND_DAMAGEINFO, {IE_EV_SM_PLAYER_VEHICLE_CHANGED});

    res &= GUI_SetEventsForWindow(IW_WND_DAMAGEINFO_IN_CHARACTERISTIC_WND_LEFT, {IE_EV_SM_PLAYER_VEHICLE_CHANGED});

    res &= GUI_SetEventsForWindow(IW_WND_DAMAGEINFO_IN_CHARACTERISTIC_WND_RIGHT, {IE_EV_SM_PLAYER_VEHICLE_CHANGED});

    res &= GUI_SetEventsForWindow(IW_WND_ITEM_INFO, {IE_CUST_NEW_FRAME, IE_EV_SM_REPOSITORY_CHANGED});

    res &= GUI_SetEventsForWindow(
        IW_WND_SALE_GOODS,
        {IE_CUST_NEW_FRAME,
         IE_CUST_DD_DRAGITEM_ACCEPTED,
         IE_CUST_DD_ITEM_QUICK_DROP,
         IE_CUST_DD_DRAGITEM_MOVE,
         IE_CUST_DD_MOUSE_OUT,
         IE_CUST_DD_MOUSE_IN,
         IE_EV_UM_CUR_PROFILE_CHANGED,
         IE_EV_UM_CUR_PROFILE_PARAM_CHANGED});

    res &= GUI_SetEventsForWindow(
        IW_WND_CONVERSATION, {IE_CUST_REPLIES_REINIT, IE_EV_UM_END_CONVERSATION, IE_EV_UM_START_CONVERSATION});

    res &= GUI_SetEventsForWindow(
        IW_WND_WEAPON_INFO_LIST,
        {IE_EV_SM_PLAYER_VEHICLE_CHANGED,
         IE_EV_SM_VEHICLEPART_CHANGED,
         IE_EV_UM_WEAPONGROUP_CHANGED,
         IE_EV_EV_KEYBINDINGS_CHANGED});

    res &= GUI_SetEventsForWindow(IW_WND_WEAPON_INFO_LIST_IN_CHAR_WND_LEFT, {IE_EV_SM_VEHICLEPART_CHANGED});

    res &= GUI_SetEventsForWindow(IW_WND_WEAPON_INFO_LIST_IN_CHAR_WND_RIGHT, {IE_EV_SM_VEHICLEPART_CHANGED});

    res &= GUI_SetEventsForWindow(IW_WND_WEAPON_INFO_LIST_IN_CHAR_WND_WORKSHOP, {IE_EV_SM_VEHICLEPART_CHANGED});

    //TODO: check this
    for (int i = IW_WND_PLAYER_GADGET_MIN; i <= IW_WND_PLAYER_GADGET_MAX; ++i)
    {
        res &= GUI_SetEventsForWindow(
            i,
            {IE_CUST_DD_DRAGITEM_MOVE,
             IE_CUST_DD_START_DRAG,
             IE_CUST_DD_END_DRAG,
             IE_CUST_DD_MOUSE_IN,
             IE_CUST_DD_MOUSE_OUT,
             IE_CUST_DD_DRAGITEM_ACCEPTED,
             IE_CUST_DD_ITEM_QUICK_DROP,
             IE_CUST_DD_DROP,
             IE_EV_SM_GADGET_CHANGED});
    }

    res &= GUI_SetEventsForWindow(
        IW_WND_GROUND, {IE_EV_SM_PLAYER_VEHICLE_CHANGED, IE_EV_SM_REPOSITORY_CHANGED, IE_CUST_START_LEVEL});

    res &= GUI_SetEventsForWindow(
        IW_DLG_MOTHER_PANEL,
        {IE_IMP_IM_UI_INVENTORY,
         IE_IMP_IM_UI_VEHICLE_INFO,
         IE_IMP_IM_UI_QUESTLOG,
         IE_IMP_IM_UI_MAP,
         IE_IMP_IM_UI_JOURNAL,
         IE_IMP_IM_UI_MENUBOOK,
         IE_IMP_IM_UI_QUESTLOG,
         IE_IMP_IM_UI_JOURNAL,
         IE_EV_UM_WORKSHOP,
         IE_EV_UM_SHOP,
         IE_EV_UM_BAR,
         IE_EV_UM_END_CONVERSATION,
         IE_EV_UM_START_TRADE,
         IE_EV_UM_FINISH_TRADE,
         IE_EV_UM_SHOW_PANEL,
         IE_EV_UM_HIDE_PANEL,
         IE_EV_UM_LOCAL_MAP,
         IE_EV_UM_GLOBAL_MAP,
         IE_IMP_IM_UI_PICKUP_ALL});

    res &= GUI_SetEventsForWindow(
        IW_WND_CHARACTERISTICS_LEFT,
        {IE_EV_SM_PLAYER_VEHICLE_CHANGED,
         IE_CUST_SHOW_VEHICLEPART,
         IE_EV_SM_VEHICLEPART_CHANGED,
         IE_EV_UM_CHARACTERISTIC_TAB_SEL_CHANGED});

    res &= GUI_SetEventsForWindow(
        IW_WND_CHARACTERISTICS_RIGHT,
        {IE_EV_SM_PLAYER_VEHICLE_CHANGED,
         IE_CUST_SHOW_VEHICLEPART,
         IE_EV_SM_VEHICLEPART_CHANGED,
         IE_EV_UM_CHARACTERISTIC_TAB_SEL_CHANGED});

    res &= GUI_SetEventsForWindow(IW_WND_PLAYER_MONEY, {IE_EV_SM_PLAYER_MONEY_CHANGED, IE_CUST_START_LEVEL});

    res &=
        GUI_SetEventsForWindow(IW_WND_PLAYER_MONEY_IN_NPC_DIALOG, {IE_EV_SM_PLAYER_MONEY_CHANGED, IE_CUST_START_LEVEL});

    res &= GUI_SetEventsForWindow(IW_WND_PLAYER_VEHICLE_MODEL_LEFT, {IE_CUST_NEW_FRAME, IE_EV_SM_VEHICLEPART_CHANGED});

    res &=
        GUI_SetEventsForWindow(IW_WND_PLAYER_VEHICLE_MODEL_RIGHT, {IE_EV_SM_PLAYER_MONEY_CHANGED, IE_CUST_START_LEVEL});

    res &= GUI_SetEventsForWindow(IW_WND_WORKSHOP_VEHICLE_MODEL, {IE_EV_SM_PLAYER_MONEY_CHANGED, IE_CUST_START_LEVEL});

    res &= GUI_SetEventsForWindow(
        IW_WND_QUEST_LIST,
        {IE_EV_SM_DYNAMIC_QUESTSTATE_CHANGED,
         IE_EV_SM_QUESTSTATE_CHANGED,
         IE_EV_SM_QUEST_WAS_TAKEN,
         IE_EV_UM_NAVPOINT_ADDED,
         IE_EV_UM_NAVPOINT_DELETED});

    res &= GUI_SetEventsForWindow(IW_WND_MAP_LIST_IN_QUESTS, {IE_CUST_START_LEVEL});

    res &= GUI_SetEventsForWindow(IW_WND_MAP_LIST_IN_MAP, {IE_CUST_START_LEVEL});

    res &= GUI_SetEventsForWindow(IW_DLG_WEAPON_GROUP_CHOICE, {IE_EV_UM_BAR_NPC, IE_EV_SM_LOCATION_NPC});

    res &= GUI_SetEventsForWindow(IW_WND_NPC_IMAGE_IN_CONVERSATION, {IE_EV_UM_NPC_REPLY_SHOWN, IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(
        IW_WND_LOCAL_CHART, {IE_EV_UM_NAVPOINT_ADDED, IE_EV_UM_NAVPOINT_DELETED, IE_EV_UM_CUR_PROFILE_CHANGED});

    res &= GUI_SetEventsForWindow(IW_DLG_WEAPON_GROUP_CHOICE, {IE_EV_EV_KEYBINDINGS_CHANGED});

    res &= GUI_SetEventsForWindow(
        IW_WND_ZNAYU_KAK_PRODAT, {IE_EV_SM_VEHICLEPART_CHANGED, IE_EV_SM_GADGET_CHANGED, IE_EV_SM_REPOSITORY_CHANGED});

    res &= GUI_SetEventsForWindow(
        IW_WND_BUY_VEHICLE,
        {IE_EV_SM_REPOSITORY_CHANGED,
         IE_EV_UM_FINISH_TRADE,
         IE_EV_SM_VEHICLEPART_CHANGED,
         IE_EV_UM_CHARACTERISTIC_TAB_SEL_CHANGED});

    res &= GUI_SetEventsForWindow(IW_WND_2D_MODEL, {IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(IW_WND_3D_SIMPLE_MODEL, {IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(IW_WND_ENCYCLOPAEDIA_2D_MODEL, {IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(IW_WND_ENCYCLOPAEDIA_3D_SIMPLE_MODEL, {IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(IW_WND_3D_COMPLEX_MODEL, {IE_CUST_NEW_FRAME, IE_EV_SM_VEHICLEPART_CHANGED});

    res &= GUI_SetEventsForWindow(
        IW_WND_ENCYCLOPAEDIA_3D_COMPLEX_MODEL, {IE_CUST_NEW_FRAME, IE_EV_SM_VEHICLEPART_CHANGED});

    res &= GUI_SetEventsForWindow(IW_WND_VIDEO, {IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(IW_DLG_TOWN, {IE_EV_SM_TOWN, IE_CUST_ENTER_BAR, IE_CUST_LEAVE_BAR});

    res &= GUI_SetEventsForWindow(
        IW_WND_ENCYCLOPAEDIA,
        {IE_CUST_START_LEVEL,
         IE_EV_SM_PLAYER_VEHICLE_CHANGED,
         IE_EV_SM_GADGET_CHANGED,
         IE_EV_SM_OBJ_MET,
         IE_EV_SM_PROTOTYPE_MET,
         IE_EV_SM_PLAYER_VEHICLE_DAMAGED,
         IE_EV_SM_QUEST_ITEMS_CHANGED,
         IE_CUST_NEW_FRAME_FORCE,
         IE_EV_UM_BELONG_MET});

    res &= GUI_SetEventsForWindow(IW_WND_GLOBAL_MAP, {IE_CUST_START_LEVEL, IE_EV_SM_LOCATION_STATE_CHANGED});

    res &= GUI_SetEventsForWindow(IW_WND_STATS, {IE_CUST_NEW_FRAME, IE_CUST_START_LEVEL});

    res &= GUI_SetEventsForWindow(IW_WND_REPUTATION, {IE_EV_UM_KNOWN_CLANS_CHANGED});

    res &=
        GUI_SetEventsForWindow(IW_WND_CHANGE_PROFILE, {IE_EV_UM_PROFILES_LIST_CHANGED, IE_EV_UM_CUR_PROFILE_CHANGED});

    res &= GUI_SetEventsForWindow(IW_WND_MAINMENU, {IE_EV_UM_CUR_PROFILE_CHANGED});

    res &= GUI_SetEventsForWindow(
        IW_WND_LOAD,
        {IE_EV_UM_LOADGAME,
         IE_EV_UM_CUR_PROFILE_PARAM_CHANGED,
         IE_EV_UM_CUR_PROFILE_CHANGED,
         IE_EV_UM_SCREENSHOT_RELEASE});

    res &= GUI_SetEventsForWindow(
        IW_WND_SAVE,
        {IE_EV_UM_SAVEGAME,
         IE_EV_UM_CUR_PROFILE_PARAM_CHANGED,
         IE_EV_UM_CUR_PROFILE_CHANGED,
         IE_EV_UM_SCREENSHOT_RELEASE});

    res &= GUI_SetEventsForWindow(IW_WND_TARGET_INFO_IN_FIGNYA_WND, {IE_CUST_NEW_FRAME_FORCE});

    res &= GUI_SetEventsForWindow(IW_WND_TARGET_INFO_IN_MAIN_INTERFACE, {IE_CUST_NEW_FRAME_FORCE});

    //TODO: check this! 118
    res &= GUI_SetEventsForWindow(IW_WND_CURSOR_MAIN, {IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(IW_WND_CURSOR_MAIN, {IE_EV_UM_SHOW_CURSOR, IE_EV_UM_GAME_MODE_CHANGED});

    res &= GUI_SetEventsForWindow(IW_WND_VEHICLE_INFO_PANEL, {IE_EV_SM_PLAYER_VEHICLE_CHANGED});

    res &= GUI_SetEventsForWindow(IW_WND_FADING_MSG_LIST, {IE_CUST_NEW_FRAME, IE_EV_SM_FADING_MESSAGE});

    res &= GUI_SetEventsForWindow(
        IW_WND_IMPORTANT_FADING_MSG_LIST, {IE_CUST_NEW_FRAME, IE_EV_SM_IMPORTANT_FADING_MESSAGE});

    res &= GUI_SetEventsForWindow(IW_WND_QUESTLOG, {IE_EV_UM_CUR_PROFILE_PARAM_CHANGED, IE_EV_UM_CUR_PROFILE_CHANGED});

    res &= GUI_SetEventsForWindow(IW_WND_LOCAL_MAP, {IE_EV_UM_NAVPOINT_ADDED, IE_EV_UM_NAVPOINT_DELETED});

    res &= GUI_SetEventsForWindow(
        IW_WND_WORKSHOP,
        {IE_EV_SM_PLAYER_VEHICLE_CHANGED, IE_CUST_NEW_FRAME, IE_EV_SM_REPOSITORY_CHANGED, IE_EV_UM_FINISH_TRADE});

    res &= GUI_SetEventsForWindow(
        IW_WND_REFUEL_LIST, {IE_EV_SM_VEHICLEPART_CHANGED, IE_EV_SM_PLAYER_VEHICLE_CHANGED, IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(
        IW_WND_REPAIR_LIST, {IE_EV_SM_VEHICLEPART_CHANGED, IE_EV_SM_PLAYER_VEHICLE_CHANGED, IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(
        IW_WND_RECHARGE_LIST, {IE_EV_SM_VEHICLEPART_CHANGED, IE_EV_SM_PLAYER_VEHICLE_CHANGED, IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(IW_WND_CABIN_LIST, {IE_EV_SM_REPOSITORY_CHANGED});

    res &= GUI_SetEventsForWindow(IW_WND_BASKET_LIST, {IE_EV_SM_REPOSITORY_CHANGED});

    res &= GUI_SetEventsForWindow(IW_WND_IGROKA_MOCHAT, {IE_EV_SM_PLAYER_VEHICLE_DAMAGED, IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(IW_WND_QUEST_ITEMS, {IE_EV_SM_QUEST_ITEMS_CHANGED});

    res &= GUI_SetEventsForWindow(
        IW_WND_COUNTER, {IE_CUST_NEW_FRAME, IE_EV_SM_DYNAMIC_QUESTSTATE_CHANGED, IE_CUST_START_LEVEL});

    res &= GUI_SetEventsForWindow(IW_WND_QUEST_DIZ, {IE_EV_UM_NAVPOINT_ADDED, IE_EV_UM_NAVPOINT_DELETED});

    res &= GUI_SetEventsForWindow(
        IW_WND_MAIN_GAME_INTERFACE,
        {IE_CUST_NEW_FRAME, IE_EV_SM_NEARBY_CHESTS, IE_EV_SM_APPROACH_TOWN, IE_EV_SM_TOWN_RUINED});

    res &= GUI_SetEventsForWindow(IW_WND_PLAYER_VEHICLE_CHARACTERISTICS_LEFT, {IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(IW_WND_PLAYER_VEHICLE_CHARACTERISTICS_RIGHT, {IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(IW_WND_WORKSHOP_VEHICLE_CHARACTERISTICS, {IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(IW_WND_PLAYER_CABIN_CHARACTERISTICS_LEFT, {IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(IW_WND_PLAYER_CABIN_CHARACTERISTICS_RIGHT, {IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(IW_WND_WORKSHOP_CABIN_CHARACTERISTICS, {IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(IW_WND_PLAYER_BASKET_CHARACTERISTICS_LEFT, {IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(IW_WND_PLAYER_BASKET_CHARACTERISTICS_RIGHT, {IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(IW_WND_WORKSHOP_BASKET_CHARACTERISTICS, {IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(IW_WND_CREDITS, {IE_CUST_NEW_FRAME});

    res &= GUI_SetEventsForWindow(
        IW_WND_FADE_PANEL_BEFORE_NEXT_MAP, {IE_CUST_NEW_FRAME, IE_EV_SM_PLAYER_PASS_TO_MAP_FADING});

    res &= GUI_SetEventsForWindow(IW_WND_BOSS_INDICATOR, {IE_CUST_NEW_FRAME});

    if (!res)
    {
        M3D_LOG_INFO("Interface event map is inited with errors");
    }
    return res;
}

void TruxxUiManager::GUI_RegisterCVars()
{
    GameUiManager::GUI_RegisterCVars();
    m_cvPathToQuestInfo.Init(
        "pathToQuestInfo",
        "data\\if\\diz\\QuestInfoGlobal.xml",
        m3d::CVar::CVAR_STRING,
        m3d::CVar::eFlags::CVAR_ARCHIVE);
    m_cvPathToDialogs.Init(
        "pathToDialogs", "data\\if\\diz\\DialogsGlobal.xml", m3d::CVar::CVAR_STRING, m3d::CVar::eFlags::CVAR_ARCHIVE);
    m_cvPathToDynamicDialogs.Init(
        "pathToDynamicDialogs",
        "data\\if\\diz\\DynamicDialogsGlobal.xml",
        m3d::CVar::CVAR_STRING,
        m3d::CVar::eFlags::CVAR_ARCHIVE);
    m_cvPathToLevelInfo.Init(
        "pathToLevelInfo", "data\\if\\diz\\LevelInfo.xml", m3d::CVar::CVAR_STRING, m3d::CVar::eFlags::CVAR_ARCHIVE);
    m_cvPathToSplashes.Init(
        "pathToSplashes", "data\\if\\ico\\splashes.xml", m3d::CVar::CVAR_STRING, m3d::CVar::eFlags::CVAR_ARCHIVE);
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
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

int TruxxUiManager::LoadCommonDiz()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool TruxxUiManager::CanLaunchModalEqualWindow()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TruxxUiManager::PrepareMenuForShow(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TruxxUiManager::DecRef()
{
    RETRUXX_NOT_IMPLEMENTED;
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

    if (forceWnd && guiEventId != IE_IMP_IM_UI_HELP)
    {
        return 0;
    }

    switch (guiEventId)
    {
    case IE_EV_EV_KEYBINDINGS_CHANGED:
    case IE_EV_EV_UI_END_WND_ANIMATION:
    case IE_EV_UM_WEAPONGROUP_CHANGED:
    case IE_EV_UM_KNOWN_LEVEL_ADDED:
    case IE_EV_UM_PROFILES_LIST_CHANGED:
    case IE_EV_UM_CUR_PROFILE_PARAM_CHANGED:
    case IE_EV_UM_LOADGAME:
    case IE_EV_UM_SHOW_CURSOR:
    case IE_EV_SM_REPOSITORY_CHANGED:
    case IE_EV_SM_PLAYER_MONEY_CHANGED:
    {
        return 0;
    }
    case IE_IMP_IM_MODE_GAME_MENU:
    {
        auto const* impInfo = static_cast<m3d::AuxImpulseInfo*>(data);
        M3D_APP->OnChangeMode(*impInfo);
        return 0;
    }
    case IE_EV_UM_CUR_PROFILE_CHANGED:
    {
        m_savesManager->GameDataUpdate(data, guiEventId);
        return 0;
    }
    case IE_EV_UM_GAME_MODE_CHANGED:
    {
        OnGameModeChanged(data);
        return 0;
    }
    case IE_EV_UM_SHOWGAMEMENU:
    {
        if (!data)
        {
            return 1;
        }
        auto const* event = static_cast<m3d::Event*>(data);
        ShowGameMenu(event->m_strEv);
        return 1;
    }
    case IE_EV_SM_OBJECT_CREATED:
    {
        if (!data)
        {
            return 1;
        }

        auto const* event = static_cast<m3d::Event*>(data);
        auto* entityByObjId = ai::theObjects->GetEntityByObjId(event->m_intEv[0]);
        if (!entityByObjId)
        {
            return 1;
        }

        auto const* cls = entityByObjId->GetClass();
        if (m_objectCollection.IsClassCollected(cls))
        {
            m_objectCollection.AddObject(entityByObjId);
        }
        return 1;
    }
    case IE_EV_SM_OBJECTS_CLEARED:
    {
        m_objectCollection.ClearObjects();
        M3D_APP->ImmediateMessage(ai::SM_PLAYER_VEHICLE_CHANGED, 0, 0, 0, 0, {}, {});
        return 1;
    }
    case IE_EV_SM_PLAYER_VEHICLE_CHANGED:
    {
        m_weaponGroupManager->GameDataUpdate(data, guiEventId);
        auto const event = static_cast<m3d::Event*>(data);
        if (event->m_intEv[0] != 1)
        {
            return 1;
        }
        m_bIsHiddenByUser = true;
        return 1;
    }
    case IE_EV_SM_VEHICLEPART_CHANGED:
    {
        m_weaponGroupManager->GameDataUpdate(data, guiEventId);
        return 1;
    }
    case IE_CUST_BEFORE_START_LEVEL:
    {
        OnBeforeStartLevel();
        return 0;
    }
    case IE_CUST_START_LEVEL:
    {
        m_levelInfoManager->GameDataUpdate(data, guiEventId);
        m_questInfoManager->GameDataUpdate(data, guiEventId);
        OnStartLevel(data);
        return 0;
    }
    case IE_CUST_END_LEVEL:
    {
        m_levelInfoManager->GameDataUpdate(data, guiEventId);
        m_questInfoManager->GameDataUpdate(data, guiEventId);
        Show(false, false);
        Reset(false);
        return 0;
    }
    case IE_CUST_NEW_FRAME:
    {
        return 0;
    }
    case IE_CUST_NEW_FRAME_FORCE:
    {
        m_levelInfoManager->GameDataUpdate(data, guiEventId);
        m_savesManager->GameDataUpdate(data, guiEventId);
        return 0;
    }
    default:
        RETRUXX_NOT_IMPLEMENTED;
    }
}

void* TruxxUiManager::QueryIface(char const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TruxxUiManager::GUI_RegisterEvents()
{
    // TODO: check this
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
    RETRUXX_NOT_IMPLEMENTED;
}

void TruxxUiManager::GUI_UnregisterCVars()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TruxxUiManager::SetGameMenuMode(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    int res = 1;
    auto const registerWindow = [this, &res](m3d::Class const* cls, char const* objName, int const wndId)
    {
        if (auto wnd = GetWindow(wndId))
        {
            if (wnd->IsKindOf(cls))
            {
                M3D_KERNEL->RegisterGlobal(wnd, objName);
                return;
            }
        }
        M3D_LOG_ERR("TruxxUiManager::GUI_RegisterScriptGlobals error - cannot register " + CStr(objName));
        res = 0;
    };

    registerWindow(RT_CLASS_LOCAL(CinemaPanel), "g_CinemaPanel", IW_WND_CINEMA_PANEL);
    registerWindow(RT_CLASS_LOCAL(ConversationWnd), "ConversationWnd", IW_WND_CONVERSATION);
    registerWindow(RT_CLASS_LOCAL(TalkWithNpcDlg), "TalkWithNpcDlg", IW_DLG_TALK_WITH_NPC);
    registerWindow(RT_CLASS_LOCAL(JournalWnd), "Journal", IW_WND_JOURNAL);
    registerWindow(RT_CLASS_LOCAL(RadarWnd), "Radar", IW_WND_RADAR);
    registerWindow(RT_CLASS_LOCAL(TownDlg), "TownDlg", IW_DLG_TOWN);
    registerWindow(RT_CLASS_LOCAL(MotherPanel), "MotherPanel", IW_DLG_MOTHER_PANEL);
    registerWindow(RT_CLASS_LOCAL(MainGameInterfaceWnd), "MainGameInterface", IW_WND_MAIN_GAME_INTERFACE);

    if (m_repliesManager)
    {
        M3D_KERNEL->RegisterGlobal(m_repliesManager, "RepliesManager");
    }
    else
    {
        M3D_LOG_ERR("TruxxUiManager::GUI_RegisterScriptGlobals error - cannot register RepliesManager");
        res = 0;
    }

    if (m_levelInfoManager)
    {
        M3D_KERNEL->RegisterGlobal(m_levelInfoManager, "LevelInfoManager");
    }
    else
    {
        M3D_LOG_ERR("TruxxUiManager::GUI_RegisterScriptGlobals error - cannot register LevelInfoManager");
        res = 0;
    }

    if (m_savesManager)
    {
        M3D_KERNEL->RegisterGlobal(m_savesManager, "SavesManager");
    }
    else
    {
        M3D_LOG_ERR("TruxxUiManager::GUI_RegisterScriptGlobals error - cannot register SavesManager");
        res = 0;
    }

    if (m_msgManager)
    {
        M3D_KERNEL->RegisterGlobal(m_msgManager, "MsgManager");
    }
    else
    {
        M3D_LOG_ERR("TruxxUiManager::GUI_RegisterScriptGlobals error - cannot register MsgManager");
        res = 0;
    }

    if (m_weaponGroupManager)
    {
        M3D_KERNEL->RegisterGlobal(m_weaponGroupManager, "WeaponGroupManager");
    }
    else
    {
        M3D_LOG_ERR("TruxxUiManager::GUI_RegisterScriptGlobals error - cannot register WeaponGroupManager");
        res = 0;
    }

    if (m_helpManager)
    {
        M3D_KERNEL->RegisterGlobal(m_helpManager, "HelpManager");
    }
    else
    {
        M3D_LOG_ERR("TruxxUiManager::GUI_RegisterScriptGlobals error - cannot register HelpManager");
        res = 0;
    }

    if (res == 0)
    {
        M3D_LOG_ERR("Interface: errors while register script globals");
    }

    M3D_KERNEL->GetEngineCfg().m_console->executeCommand("/conScript data\\scripts\\GuiObjects.lua");

    return res;
}
