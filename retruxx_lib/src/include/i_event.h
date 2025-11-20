#pragma once
#include "core/aiparam.h"
#include "core/stringm3d.h"

enum MessageType
{
    UM_CLOSEMESSAGE = 0x10064,
    UM_STARTGAME = 0x10065,
    UM_ENDGAME_WIN = 0x10066,
    UM_ENDGAME_LOSS = 0x10067,
    UM_STARTSERVER = 0x10068,
    UM_STARTCLIENT = 0x10069,
    UM_CANCLOSE = 0x1006a,
    UM_STARTGAMEFAST = 0x1006b,
    UM_SAVEGAME = 0x1006c,
    UM_LOADGAME = 0x1006d,
    UM_CREATEPLAYER = 0x1006e,
    UM_SELECTPLAYER = 0x1006f,
    UM_STARTLEVEL = 0x10070,
    UM_SHOWCAMPAIGNS = 0x10071,
    UM_QUIT_TO_MAINMENU = 0x10072,
    UM_DELETEPLAYER = 0x10073,
    UM_REPLAY = 0x10074,
    UM_BINDKEYS_DLG = 0x10075,
    UM_SHOWLEVELS = 0x10077,
    UM_SHOWGAMEMENU = 0x10078,
    UM_BAR = 0x10079,
    UM_WORKSHOP = 0x1007a,
    UM_ADMINISTRATION = 0x1007b,
    UM_NPC_REPLY_SHOWN = 0x1007c,
    UM_SHOP = 0x1007d,
    UM_BAR_NPC = 0x1007f,
    UM_START_CONVERSATION = 0x10080,
    UM_END_CONVERSATION = 0x10081,
    UM_NAVPOINT_ADDED = 0x10082,
    UM_NAVPOINT_DELETED = 0x10083,
    UM_WEAPONGROUP_CHANGED = 0x10084,
    UM_GADGET_DEACTIVATE = 0x10085,
    UM_VEHICLEPART_DEACTIVATE = 0x10086,
    UM_START_TRADE = 0x10087,
    UM_FINISH_TRADE = 0x10088,
    UM_HIDE_PANEL = 0x10089,
    UM_SHOW_PANEL = 0x1008a,
    UM_KNOWN_LEVEL_ADDED = 0x1008b,
    UM_LOCAL_MAP = 0x1008c,
    UM_GLOBAL_MAP = 0x1008d,
    UM_CUR_PROFILE_CHANGED = 0x1008e,
    UM_PROFILES_LIST_CHANGED = 0x1008f,
    UM_CUR_PROFILE_PARAM_CHANGED = 0x10090,
    UM_SCREENSHOT_RELEASE = 0x10091,
    UM_SHOW_CURSOR = 0x10092,
    UM_GAME_MODE_CHANGED = 0x10093,
    UM_LOAD_LAST_GAME = 0x10094,
    UM_OPTIONS = 0x10095,
    UM_BELONG_MET = 0x10096,
    UM_KNOWN_CLANS_CHANGED = 0x10097,
    UM_GAME_MENU_MODE_ENTER = 0x10098,
    UM_GAME_MENU_MODE_EXIT = 0x10099,
    UM_CHARACTERISTIC_TAB_SEL_CHANGED = 0x1009a,
    UM_HELP = 0x1009b,
};

namespace m3d
{
    enum EventType
    {
        EV_MESSAGE = 0,
        EV_APP_QUIT = 1,
        EV_ACTIVATE_APP = 2,
        EV_CHANGE_DISPLAY_MODE = 3,
        EV_DISPLAY_CHANGED = 4,
        EV_LOOSING_FOCUS = 5,
        EV_OBTAINED_FOCUS = 6,
        EV_KEY_DOWN = 7,
        EV_KEY_UP = 8,
        EV_MOUSE_MOVE = 9,
        EV_MOUSE_LBTN = 10,
        EV_MOUSE_RBTN = 11,
        EV_MOUSE_MBTN = 12,
        EV_MOUSE_DBLCLICK = 13,
        EV_MOUSE_CLICK = 14,
        EV_MOUSE_WHEEL = 15,
        EV_JOYSTICK_BTN0 = 16,
        EV_JOYSTICK_BTN1 = 17,
        EV_JOYSTICK_BTN2 = 18,
        EV_JOYSTICK_BTN3 = 19,
        EV_JOYSTICK_BTN4 = 20,
        EV_JOYSTICK_BTN5 = 21,
        EV_JOYSTICK_BTN6 = 22,
        EV_JOYSTICK_BTN7 = 23,
        EV_JOYSTICK_BTN8 = 24,
        EV_JOYSTICK_BTN9 = 25,
        EV_JOYSTICK_X_AXIS_PLUS = 26,
        EV_JOYSTICK_X_AXIS_MINUS = 27,
        EV_JOYSTICK_Y_AXIS_PLUS = 28,
        EV_JOYSTICK_Y_AXIS_MINUS = 29,
        EV_JOYSTICK_Z_AXIS_PLUS = 30,
        EV_JOYSTICK_Z_AXIS_MINUS = 31,
        EV_JOYSTICK_Z_ROT_PLUS = 32,
        EV_JOYSTICK_Z_ROT_MINUS = 33,
        EV_JOYSTICK_S0_PLUS = 34,
        EV_JOYSTICK_S0_MINUS = 35,
        EV_JOYSTICK_S1_PLUS = 36,
        EV_JOYSTICK_S1_MINUS = 37,
        EV_MOUSE_MOVE_ON_UI = 38,
        EV_UI_CLOSE_MODAL_WND = 39,
        EV_UI_NOTIFY_WND = 40,
        EV_UI_MODAL_WND_IS_CLOSED = 41,
        EV_UI_END_WND_ANIMATION = 42,
        EV_UI_STOP_ALL_WND_ANIMATIONS = 43,
        EV_INSERT_CREATED_TOOLTIP = 44,
        EV_REMOVE_TOOLTIP = 45,
        EV_KEYBINDINGS_CHANGED = 46,
        EV_UI_COMBO_OPENED = 47,
        EV_UI_COMBO_CLOSED = 48,
        EV_USER = 0x10000,
    };

    struct Event;

    struct IEventHandler
    {
        virtual ~IEventHandler() = default /* 0x00 */;
        virtual int HandleEvent(const m3d::Event&) = 0 /* 0x04 */;
    }; /* size: 0x0004 */

    struct Event
    {
        double m_timeStamp = 0.0;
        int m_eventType = 0;
        CStr m_strEv;
        AIParam m_aiParamEv;

        union
        {
            void* m_void[4] = {};
            IEventHandler* m_handle[4];
            unsigned int m_uintEv[4];
            int m_intEv[4];
            unsigned short m_ushortEv[8];
            short m_shortEv[8];
            unsigned char m_byteEv[16];
        }; /* size: 0x0010 */

        CStr Dump() const;
    }; /* size: 0x0048 */
}  // namespace m3d
