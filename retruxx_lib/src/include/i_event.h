#pragma once
#include <stdexcept>
#include <core/aiparam.h>
#include <core/stringm3d.h>

namespace m3d
{
    enum EventType
    {
        EV_MESSAGE = 0x0,
        EV_APP_QUIT = 0x1,
        EV_ACTIVATE_APP = 0x2,
        EV_CHANGE_DISPLAY_MODE = 0x3,
        EV_DISPLAY_CHANGED = 0x4,
        EV_LOOSING_FOCUS = 0x5,
        EV_OBTAINED_FOCUS = 0x6,
        EV_KEY_DOWN = 0x7,
        EV_KEY_UP = 0x8,
        EV_MOUSE_MOVE = 0x9,
        EV_MOUSE_LBTN = 0xA,
        EV_MOUSE_RBTN = 0xB,
        EV_MOUSE_MBTN = 0xC,
        EV_MOUSE_DBLCLICK = 0xD,
        EV_MOUSE_CLICK = 0xE,
        EV_MOUSE_WHEEL = 0xF,
        EV_JOYSTICK_BTN0 = 0x10,
        EV_JOYSTICK_BTN1 = 0x11,
        EV_JOYSTICK_BTN2 = 0x12,
        EV_JOYSTICK_BTN3 = 0x13,
        EV_JOYSTICK_BTN4 = 0x14,
        EV_JOYSTICK_BTN5 = 0x15,
        EV_JOYSTICK_BTN6 = 0x16,
        EV_JOYSTICK_BTN7 = 0x17,
        EV_JOYSTICK_BTN8 = 0x18,
        EV_JOYSTICK_BTN9 = 0x19,
        EV_JOYSTICK_X_AXIS_PLUS = 0x1A,
        EV_JOYSTICK_X_AXIS_MINUS = 0x1B,
        EV_JOYSTICK_Y_AXIS_PLUS = 0x1C,
        EV_JOYSTICK_Y_AXIS_MINUS = 0x1D,
        EV_JOYSTICK_Z_AXIS_PLUS = 0x1E,
        EV_JOYSTICK_Z_AXIS_MINUS = 0x1F,
        EV_JOYSTICK_Z_ROT_PLUS = 0x20,
        EV_JOYSTICK_Z_ROT_MINUS = 0x21,
        EV_JOYSTICK_S0_PLUS = 0x22,
        EV_JOYSTICK_S0_MINUS = 0x23,
        EV_JOYSTICK_S1_PLUS = 0x24,
        EV_JOYSTICK_S1_MINUS = 0x25,
        EV_MOUSE_MOVE_ON_UI = 0x26,
        EV_UI_CLOSE_MODAL_WND = 0x27,
        EV_UI_NOTIFY_WND = 0x28,
        EV_UI_MODAL_WND_IS_CLOSED = 0x29,
        EV_UI_END_WND_ANIMATION = 0x2A,
        EV_UI_STOP_ALL_WND_ANIMATIONS = 0x2B,
        EV_INSERT_CREATED_TOOLTIP = 0x2C,
        EV_REMOVE_TOOLTIP = 0x2D,
        EV_KEYBINDINGS_CHANGED = 0x2E,
        EV_UI_COMBO_OPENED = 0x2F,
        EV_UI_COMBO_CLOSED = 0x30,
        EV_USER = 0x10000,
    };

    class Event;

    struct IEventHandler
    {
        /* 0x0000 */;
        virtual ~IEventHandler() = default /* 0x00 */;
        virtual int HandleEvent(const m3d::Event&) = 0 /* 0x04 */;
    }; /* size: 0x0004 */

    //IMPORTANT: fields and member order is strict!
    class Event
    {
    public:
        long double m_timeStamp = 0.0;
        int m_eventType = 0;
        CStr m_strEv;
        AIParam m_aiParamEv;
        //TODO: union
        union
        {
            void* m_void[4] = {0};
            IEventHandler* m_handle[4];
            unsigned int m_uintEv[4];
            int m_intEv[4];
            unsigned __int16 m_ushortEv[8];
            __int16 m_shortEv[8];
            unsigned __int8 m_byteEv[16];
        };
        Event() {}
        ~Event() {}
    };
}
