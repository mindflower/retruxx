#include <stdexcept>
#include <impulses/i_impulses.h>
#include "gameimpulse.h"
#include <core/log.h>

#include "config.h"
#include "m3dapp.h"

namespace
{
    struct KeyToId
    {
        int m_keyId;
        char* m_keyName;
    };

    KeyToId l_keyNames[] = {
        {0x1, "KEY_ESCAPE"},
        {0x2, "KEY_1"},
        {0x3, "KEY_2"},
        {0x4, "KEY_3"},
        {0x5, "KEY_4"},
        {0x6, "KEY_5"},
        {0x7, "KEY_6"},
        {0x8, "KEY_7"},
        {0x9, "KEY_8"},
        {0x0A, "KEY_9"},
        {0x0B, "KEY_0"},
        {0x0C, "KEY_MINUS"},
        {0x0D, "KEY_EQUALS"},
        {0x0E, "KEY_BACKSPACE"},
        {0x0F, "KEY_TAB"},
        {0x10, "KEY_Q"},
        {0x11, "KEY_W"},
        {0x12, "KEY_E"},
        {0x13, "KEY_R"},
        {0x14, "KEY_T"},
        {0x15, "KEY_Y"},
        {0x16, "KEY_U"},
        {0x17, "KEY_I"},
        {0x18, "KEY_O"},
        {0x19, "KEY_P"},
        {0x1A, "KEY_LBRACKET"},
        {0x1B, "KEY_RBRACKET"},
        {0x1C, "KEY_RETURN"},
        {0x1D, "KEY_LCONTROL"},
        {0x1E, "KEY_A"},
        {0x1F, "KEY_S"},
        {0x20, "KEY_D"},
        {0x21, "KEY_F"},
        {0x22, "KEY_G"},
        {0x23, "KEY_H"},
        {0x24, "KEY_J"},
        {0x25, "KEY_K"},
        {0x26, "KEY_L"},
        {0x27, "KEY_SEMICOLON"},
        {0x28, "KEY_APOSTROPHE"},
        {0x29, "KEY_GRAVE"},
        {0x2A, "KEY_LSHIFT"},
        {0x2B, "KEY_BACKSLASH"},
        {0x2C, "KEY_Z"},
        {0x2D, "KEY_X"},
        {0x2E, "KEY_C"},
        {0x2F, "KEY_V"},
        {0x30, "KEY_B"},
        {0x31, "KEY_N"},
        {0x32, "KEY_M"},
        {0x33, "KEY_COMMA"},
        {0x34, "KEY_PERIOD"},
        {0x35, "KEY_SLASH"},
        {0x36, "KEY_RSHIFT"},
        {0x37, "KEY_NUMPADSTAR"},
        {0x38, "KEY_LALT"},
        {0x39, "KEY_SPACE"},
        {0x3A, "KEY_CAPSLOCK"},
        {0x3B, "KEY_F1"},
        {0x3C, "KEY_F2"},
        {0x3D, "KEY_F3"},
        {0x3E, "KEY_F4"},
        {0x3F, "KEY_F5"},
        {0x40, "KEY_F6"},
        {0x41, "KEY_F7"},
        {0x42, "KEY_F8"},
        {0x43, "KEY_F9"},
        {0x44, "KEY_F10"},
        {0x45, "KEY_NUMLOCK"},
        {0x46, "KEY_SCROLL"},
        {0x47, "KEY_NUMPAD7"},
        {0x48, "KEY_NUMPAD8"},
        {0x49, "KEY_NUMPAD9"},
        {0x4A, "KEY_NUMPADMINUS"},
        {0x4B, "KEY_NUMPAD4"},
        {0x4C, "KEY_NUMPAD5"},
        {0x4D, "KEY_NUMPAD6"},
        {0x4E, "KEY_NUMPADPLUS"},
        {0x4F, "KEY_NUMPAD1"},
        {0x50, "KEY_NUMPAD2"},
        {0x51, "KEY_NUMPAD3"},
        {0x52, "KEY_NUMPAD0"},
        {0x53, "KEY_NUMPADPERIOD"},
        {0x56, "KEY_OEM_102"},
        {0x57, "KEY_F11"},
        {0x58, "KEY_F12"},
        {0x64, "KEY_F13"},
        {0x65, "KEY_F14"},
        {0x66, "KEY_F15"},
        {0x70, "KEY_KANA"},
        {0x73, "KEY_ABNT_C1"},
        {0x79, "KEY_CONVERT"},
        {0x7B, "KEY_NOCONVERT"},
        {0x7D, "KEY_YEN"},
        {0x7E, "KEY_ABNT_C2"},
        {0x8D, "KEY_NUMPADEQUALS"},
        {0x90, "KEY_PREVTRACK"},
        {0x91, "KEY_AT"},
        {0x92, "KEY_COLON"},
        {0x93, "KEY_UNDERLINE"},
        {0x94, "KEY_KANJI"},
        {0x95, "KEY_STOP"},
        {0x96, "KEY_AX"},
        {0x97, "KEY_UNLABELED"},
        {0x99, "KEY_NEXTTRACK"},
        {0x9C, "KEY_NUMPADENTER"},
        {0x9D, "KEY_RCONTROL"},
        {0x0A0, "KEY_MUTE"},
        {0x0A1, "KEY_CALCULATOR"},
        {0x0A2, "KEY_PLAYPAUSE"},
        {0x0A4, "KEY_MEDIASTOP"},
        {0x0AE, "KEY_VOLUMEDOWN"},
        {0x0B0, "KEY_VOLUMEUP"},
        {0x0B2, "KEY_WEBHOME"},
        {0x0B3, "KEY_NUMPADCOMMA"},
        {0x0B5, "KEY_NUMPADSLASH"},
        {0x0B7, "KEY_SYSRQ"},
        {0x0B8, "KEY_RALT"},
        {0x0C5, "KEY_PAUSE"},
        {0x0C7, "KEY_HOME"},
        {0x0C8, "KEY_UPARROW"},
        {0x0C9, "KEY_PGUP"},
        {0x0CB, "KEY_LEFTARROW"},
        {0x0CD, "KEY_RIGHTARROW"},
        {0x0CF, "KEY_END"},
        {0x0D0, "KEY_DOWNARROW"},
        {0x0D1, "KEY_PGDN"},
        {0x0D2, "KEY_INSERT"},
        {0x0D3, "KEY_DELETE"},
        {0x0DB, "KEY_LWIN"},
        {0x0DC, "KEY_RWIN"},
        {0x0DD, "KEY_APPS"},
        {0x0DE, "KEY_POWER"},
        {0x0DF, "KEY_SLEEP"},
        {0x0E3, "KEY_WAKE"},
        {0x0E5, "KEY_WEBSEARCH"},
        {0x0E6, "KEY_WEBFAVORITES"},
        {0x0E7, "KEY_WEBREFRESH"},
        {0x0E8, "KEY_WEBSTOP"},
        {0x0E9, "KEY_WEBFORWARD"},
        {0x0EA, "KEY_WEBBACK"},
        {0x0EB, "KEY_MYCOMPUTER"},
        {0x0EC, "KEY_MAIL"},
        {0x0ED, "KEY_MEDIASELECT"},
        {0x100, "KEY_LBUTTON"},
        {0x101, "KEY_RBUTTON"},
        {0x102, "KEY_MBUTTON"},
        {0x103, "KEY_MOUSE_MOVE"},
        {0x104, "KEY_MWHEEL"},
        {0x105, "KEY_CONTROL"},
        {0x106, "KEY_ALT"},
        {0x107, "KEY_SHIFT"},
        {0x108, "KEY_MOUSE_MOVE_ON_UI"},
        {0x109, "JOY_BUTTON_0"},
        {0x10A, "JOY_BUTTON_1"},
        {0x10B, "JOY_BUTTON_2"},
        {0x10C, "JOY_BUTTON_3"},
        {0x10D, "JOY_BUTTON_4"},
        {0x10E, "JOY_BUTTON_5"},
        {0x10F, "JOY_BUTTON_6"},
        {0x110, "JOY_BUTTON_7"},
        {0x111, "JOY_BUTTON_8"},
        {0x112, "JOY_BUTTON_9"},
        {0x114, "JOY_X_AXIS_+"},
        {0x115, "JOY_X_AXIS_-"},
        {0x116, "JOY_Y_AXIS_+"},
        {0x117, "JOY_Y_AXIS_-"},
        {0x118, "JOY_Z_AXIS_+"},
        {0x119, "JOY_Z_AXIS_-"},
        {0x11A, "JOY_Z_ROT_+"},
        {0x11B, "JOY_Z_ROT_-"},
        {0x11C, "JOY_S0_+"},
        {0x11D, "JOY_S0_-"},
        {0x11E, "JOY_S1_+"},
        {0x11F, "JOY_S1_-"},
    };
}  // namespace

RT_CLASS_EXPORT_METHOD_DEFINE(GameImpulse, BindKey3)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(GameImpulse, BindKey2)
{
    auto gameImpulse = dynamic_cast<m3d::GameImpulse*>(context->asObject(0, "GameImpulse"));
    auto gameMode = context->asString(1);
    auto key1 = context->asString(2);
    auto key2 = context->asString(3);
    auto imp = context->asString(4);
    auto res = gameImpulse->BindKey2(gameMode, key1, key2, imp);
    context->pushInt(res);
    return 1;
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(GameImpulse, BindKey1)
{
    auto gameImpulse = dynamic_cast<m3d::GameImpulse*>(context->asObject(0, "GameImpulse"));
    auto gameMode = context->asString(1);
    auto key1 = context->asString(2);
    auto imp = context->asString(3);
    auto res = gameImpulse->BindKey1(gameMode, key1, imp);
    context->pushInt(res);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(GameImpulse, UnbindKey3)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(GameImpulse, UnbindKey2)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(GameImpulse, UnbindKey1)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(GameImpulse, UnbindAll)
{
    auto gameImpulse = dynamic_cast<m3d::GameImpulse*>(context->asObject(0, "GameImpulse"));
    gameImpulse->UnbindAll();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(GameImpulse, LoadFromDefaults)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(GameImpulse, SaveToDefaults)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(GameImpulse, LoadFromProfile)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(GameImpulse, SaveToProfile)
{
    RETRUXX_NOT_IMPLEMENTED;
}

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(GameImpulse)
    RT_CLASS_EXPORT(GameImpulse, METHOD, BindKey3, "", "", "")
    RT_CLASS_EXPORT(GameImpulse, METHOD, BindKey2, "", "", "")
    RT_CLASS_EXPORT(GameImpulse, METHOD, BindKey1, "", "", "")
    RT_CLASS_EXPORT(GameImpulse, METHOD, UnbindKey3, "", "", "")
    RT_CLASS_EXPORT(GameImpulse, METHOD, UnbindKey2, "", "", "")
    RT_CLASS_EXPORT(GameImpulse, METHOD, UnbindKey1, "", "", "")
    RT_CLASS_EXPORT(GameImpulse, METHOD, UnbindAll, "", "", "")
    RT_CLASS_EXPORT(GameImpulse, METHOD, LoadFromDefaults, "", "", "")
    RT_CLASS_EXPORT(GameImpulse, METHOD, SaveToDefaults, "", "", "")
    RT_CLASS_EXPORT(GameImpulse, METHOD, LoadFromProfile, "", "", "")
    RT_CLASS_EXPORT(GameImpulse, METHOD, SaveToProfile, "", "", "")
    RT_CLASS_EXPORTS_END;

    RT_CLASS_DEFINE(GameImpulse);

    void AuxImpulseInfo::UnpackXy(float* x, float* y, float* dx, float* dy) const
    {
        // TODO: generated code
        if (x)
        {
            *x = static_cast<float>(static_cast<int16_t>(m_info0 & 0xFFFF));
        }
        if (y)
        {
            *y = static_cast<float>(static_cast<int16_t>((m_info0 >> 16) & 0xFFFF));
        }
        if (dx)
        {
            *dx = static_cast<float>(static_cast<int16_t>(m_info1 & 0xFFFF));
        }
        if (dy)
        {
            *dy = static_cast<float>(static_cast<int16_t>((m_info1 >> 16) & 0xFFFF));
        }
    }

    AuxImpulseInfo::AuxImpulseInfo(int impId, bool state, int gameMode, unsigned i0, unsigned i1)
    {
        m_impId = impId;
        m_state = state;
        m_gameMode = gameMode;
        m_info0 = i0;
        m_info1 = i1;
    }

    float AuxImpulseInfo::UnpackWheel() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int GameImpulse::LoadFromFile(CStr const& bindFile)
    {
        int res = 0;
        if (m_isInited)
        {
            res = 1;
            m_bSuppressEvent = true;
            if (m_isBinded)
            {
                UnbindAll();
            }
            auto err = g_Kernel->GetScriptServer().executeScriptFile(bindFile.c_str());
            if (err)
            {
                M3D_LOG_ERR("Key bindings: loading error - " + GetFormattedScriptErrorDesc(err));
                res = 0;
            }
            else
            {
                Application::g_pApp->EnqueueMessage(46, -1, 0, 0, 0, {}, {});
                M3D_LOG_INFO("Key bindings: were loaded successfully from file " + bindFile);
            }
            m_bSuppressEvent = false;
        }
        else
        {
            M3D_LOG_INFO("Key bindings: error loading from file cause is not inited");
            res = 0;
        }
        return res;
    }

    int GameImpulse::DecRef()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GameImpulse::ResetImpulseWithoutNotification(int impId)
    {
        if (m_isInited)
        {
            auto it = m_impulseStates.find(impId);
            if (it != m_impulseStates.end())
            {
                it->second = false;
            }
        }
    }

    bool GameImpulse::GetImpulseStateAndReset(int impId)
    {
        if (!m_isInited)
        {
            return 0;
        }

        auto impIda = GetImpulseState(impId);
        m3d::AuxImpulseInfo impInfo(impId, false, -1, 0, 0);
        auto v6 = !M3D_APP->HasChildModalRunning() ? M3D_APP : 0;

        ui::Wnd* v3 = nullptr;
        if (v6)
            v3 = M3D_APP;
        this->SetImpulseState(impInfo, v3);
        return impIda;
    }

    GameImpulse::~GameImpulse()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void* GameImpulse::QueryIface(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int GameImpulse::Init()
    {
        if (m_isInited)
        {
            M3D_LOG_INFO("Key bindings: fail to init because already is inited");
            return 0;
        }
        g_Kernel->UnRegisterGlobal("IMPULSES");
        g_Kernel->RegisterGlobal(this, "IMPULSES");
        m_isInited = true;
        M3D_LOG_INFO("Key bindings: were inited successfully");
        return 1;
    }

    int GameImpulse::SaveToDefaults()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int GameImpulse::Done()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GameImpulse::ResetAllImpulses(bool bClearPressedKeys)
    {
        if (m_isInited)
        {
            for (auto const& state : m_impulseStates)
            {
                m_impulseResetAfterRead[state.first] = false;
            }
            if (bClearPressedKeys)
            {
                m_curKeys.clear();
            }
        }
    }

    int GameImpulse::SetImpulsesStateBySet(KeysSet impSet, bool state, int curGameMode, ui::Wnd* causeWnd)
    {
        if (!m_isInited || impSet.empty())
        {
            return 0;
        }

        auto it = m_bindings.find(curGameMode);
        if (it == m_bindings.end())
        {
            return 0;
        }

        // TODO: check this!!!

        auto& bindStation = it->second;
        while (!impSet.empty())
        {
            KeysSet ks;
            auto imp = bindStation.FindImpulseByLongestSetPossible(impSet, 0xFFFFFFFF, ks);
            if (imp == -1)
            {
                break;
            }

            AuxImpulseInfo info(imp, state, curGameMode, 0, 0);
            SetImpulseState(info, causeWnd);

            impSet = impSet - ks;
        }
        return 1;
    }

    int GameImpulse::SetImpulseState(AuxImpulseInfo const& impInfo, ui::Wnd* causeWnd)
    {
        if (!m_isInited)
        {
            return 0;
        }

        auto it = m_impulseStates.find(impInfo.m_impId);
        if (it == m_impulseStates.end())
        {
            return 0;
        }

        if (impInfo.m_state == it->second)
        {
            return 1;
        }
        it->second = impInfo.m_state;
        return HandleImpulse(impInfo, causeWnd);
    }

    void GameImpulse::RaiseOneTimeImpulse(AuxImpulseInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool GameImpulse::GetImpulseState(int impId)
    {
        // TODO: check this
        if (!m_isInited)
        {
            return false;
        }

        auto it = m_impulseStates.find(impId);
        if (it == m_impulseStates.end())
        {
            return false;
        }

        if (m_impulseResetAfterRead.find(impId) == m_impulseResetAfterRead.end())
        {
            m_impulseResetAfterRead[impId] = false;
        }

        if (m_impulseResetAfterRead[impId])
        {
            if (it->second)
            {
                m3d::AuxImpulseInfo impInfo(impId, false, -1, 0, 0);
                auto v6 = !M3D_APP->HasChildModalRunning() ? M3D_APP : 0;
                SetImpulseState(impInfo, v6);
            }
            m_impulseResetAfterRead[impId] = false;
        }
        return it->second;
    }

    int GameImpulse::GetImpulseForKeys(std::vector<int, std::allocator<int>>, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GameImpulse::BindKey0(int gameMode, KeysSet const& ks, int impId)
    {
        if (m_isInited)
        {
            m_bindings[gameMode].BindKeyToImpulse(ks, impId);
            m_impulseStates[impId] = 0;
            m_impulseResetAfterRead[impId] = 0;
            m_isBinded = true;
        }
    }

    int GameImpulse::BindKey1(CStr const& strGameMode, CStr const& strKey1, CStr const& strImp)
    {
        if (m_isInited)
        {
            return BindKey3(strGameMode, strKey1, {}, {}, strImp);
        }
        else
        {
            M3D_LOG_INFO("Key bindings: error bind key cause impulses were not inited");
            return 0;
        }
    }

    int GameImpulse::IncRef()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    std::vector<std::vector<int>> GameImpulse::GetKeysForImpulse(int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int GameImpulse::HandleKeyboardMouseEvent(Event const& ev, ui::Wnd* causeWnd)
    {
        if (!m_isInited)
        {
            return 0;
        }

        auto curGameMode = M3D_APP->GetCurGameMode();
        auto it = m_bindings.find(curGameMode);
        if (it == m_bindings.end())
        {
            return 0;
        }

        auto& bindStation = it->second;

        int keyToSearchBy = 0;
        bool state = false;
        bool onlyDown = false;
        unsigned i0 = 0;
        unsigned i1 = 0;

        switch (ev.m_eventType)
        {
        case EV_KEY_DOWN:
        case EV_KEY_UP:
        {
            keyToSearchBy = ev.m_byteEv[3];
            state = (ev.m_eventType == EV_KEY_DOWN);
            break;
        }

        case EV_MOUSE_MOVE:
        {
            keyToSearchBy = 259;
            state = true;
            onlyDown = true;
            i0 = ev.m_uintEv[0];
            i1 = ev.m_ushortEv[2] | (ev.m_ushortEv[3] << 16);
            break;
        }

        case EV_MOUSE_LBTN:
        {
            keyToSearchBy = 256;
            state = ev.m_ushortEv[2] != 0;
            i0 = ev.m_ushortEv[0] | (ev.m_ushortEv[1] << 16);
            break;
        }

        case EV_MOUSE_RBTN:
        {
            keyToSearchBy = 257;
            state = ev.m_ushortEv[2] != 0;
            i0 = ev.m_ushortEv[0] | (ev.m_ushortEv[1] << 16);
            break;
        }

        case EV_MOUSE_MBTN:
        {
            keyToSearchBy = 258;
            state = ev.m_ushortEv[2] != 0;
            i0 = ev.m_ushortEv[0] | (ev.m_ushortEv[1] << 16);
            break;
        }

        case EV_MOUSE_WHEEL:
        {
            keyToSearchBy = 260;
            state = true;
            onlyDown = true;
            i0 = ev.m_ushortEv[0] | (ev.m_ushortEv[1] << 16);
            i1 = ev.m_ushortEv[2];
            break;
        }

        case EV_JOYSTICK_BTN0:
        case EV_JOYSTICK_BTN1:
        case EV_JOYSTICK_BTN2:
        case EV_JOYSTICK_BTN3:
        case EV_JOYSTICK_BTN4:
        case EV_JOYSTICK_BTN5:
        case EV_JOYSTICK_BTN6:
        case EV_JOYSTICK_BTN7:
        case EV_JOYSTICK_BTN8:
        case EV_JOYSTICK_BTN9:
        {
            keyToSearchBy = ev.m_eventType + 249;
            state = ev.m_ushortEv[2] != 0;
            break;
        }

        case EV_MOUSE_MOVE_ON_UI:
        {
            keyToSearchBy = 264;
            state = true;
            onlyDown = true;
            i0 = ev.m_uintEv[0];
            i1 = ev.m_ushortEv[2] | (ev.m_ushortEv[3] << 16);
            break;
        }

        default:
            break;
        }

        // TODO: check this
        if (FilterShifts(keyToSearchBy, state))
        {
            if (state)
            {
                if (!m_curKeys.IsThere(keyToSearchBy))
                {
                    m_curKeys += keyToSearchBy;

                    KeysSet impSet;
                    auto impulse = bindStation.FindImpulseByLongestSetPossible(m_curKeys, keyToSearchBy, impSet);
                    if (impulse != -1)
                    {
                        impSet -= keyToSearchBy;
                        SetImpulsesStateBySet(impulse, false, curGameMode, causeWnd);
                        AuxImpulseInfo info(impulse, true, curGameMode, i0, i1);
                        SetImpulseState(info, causeWnd);
                    }
                    if (onlyDown)
                    {
                        state = false;
                    }
                }
            }
            if (!state)
            {
                if (m_curKeys.IsThere(keyToSearchBy))
                {
                    KeysSet impSet;
                    auto impulse = bindStation.FindImpulseByLongestSetPossible(m_curKeys, keyToSearchBy, impSet);
                    if (impulse != -1)
                    {
                        AuxImpulseInfo info(impulse, state, curGameMode, i0, i1);
                        SetImpulseState(info, causeWnd);
                        impSet -= keyToSearchBy;
                        SetImpulsesStateBySet(impSet, true, curGameMode, causeWnd);
                    }
                    m_curKeys -= keyToSearchBy;
                }
            }
        }
        return 1;
    }

    int GameImpulse::FilterShifts(int& keyToSearchBy, bool& state)
    {
        // TODO: generated code
        if (!m_isInited)
        {
            return false;
        }

        // Shift key states
        static bool downL_1 = false;  // Left Shift
        static bool downR_1 = false;  // Right Shift
        static bool prevS_1 = false;  // Previous Shift state

        static bool downL_0 = false;  // Left Alt
        static bool downR_0 = false;  // Right Alt
        static bool prevS_0 = false;  // Previous Alt state

        static bool downL = false;  // Left Control
        static bool downR = false;  // Right Control
        static bool prevS = false;  // Previous Control state

        int keyCode = keyToSearchBy;

        // Check for invalid key
        if (keyCode == -1)
        {
            return false;
        }

        // Handle Shift keys (left and right)
        if (keyCode == KEY_LSHIFT || keyCode == KEY_RSHIFT)
        {
            // Update individual shift key state
            if (keyCode == KEY_LSHIFT)
            {
                downL_1 = state;
            }
            else if (keyCode == KEY_RSHIFT)
            {
                downR_1 = state;
            }

            // Calculate combined shift state
            bool currentShiftState = downL_1 || downR_1;

            // Only process if state changed
            if (currentShiftState == prevS_1)
            {
                return false;
            }

            // Update previous state and modify output
            prevS_1 = currentShiftState;
            keyToSearchBy = (KEY_RSHIFT | KEY_LSHIFT);
            state = currentShiftState;
            return true;
        }

        // Handle Alt keys (left and right)
        if (keyCode == KEY_LALT || keyCode == KEY_RALT)
        {
            // Update individual alt key state
            if (keyCode == KEY_LALT)
            {
                downL_0 = state;
            }
            else if (keyCode == KEY_RALT)
            {
                downR_0 = state;
            }

            // Calculate combined alt state
            bool currentAltState = downL_0 || downR_0;

            // Only process if state changed
            if (currentAltState == prevS_0)
            {
                return false;
            }

            // Update previous state and modify output
            prevS_0 = currentAltState;
            keyToSearchBy = (KEY_RALT | KEY_LALT);
            state = currentAltState;
            return true;
        }

        // Handle Control keys (left and right)
        if (keyCode == KEY_LCONTROL || keyCode == KEY_RCONTROL)
        {
            // Update individual control key state
            if (keyCode == KEY_LCONTROL)
            {
                downL = state;
            }
            else if (keyCode == KEY_RCONTROL)
            {
                downR = state;
            }

            // Calculate combined control state
            bool currentControlState = downL || downR;

            // Only process if state changed
            if (currentControlState != prevS)
            {
                // Update previous state and modify output
                keyToSearchBy = (KEY_LCONTROL | KEY_RCONTROL);
                prevS = currentControlState;
                state = currentControlState;
                return true;
            }

            return false;
        }

        // For all other keys, allow processing
        return true;
    }

    int GameImpulse::BindKey2(CStr const& strGameMode, CStr const& strKey1, CStr const& strKey2, CStr const& strImp)
    {
        if (m_isInited)
        {
            return BindKey3(strGameMode, strKey1, strKey2, {}, strImp);
        }
        else
        {
            M3D_LOG_INFO("Key bindings: error bind key cause impulses were not inited");
            return 0;
        }
    }

    int GameImpulse::SaveToProfile()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int GameImpulse::HandleBinding(
        int action,
        CStr const& strGameMode,
        CStr const& strKey1,
        CStr const& strKey2,
        CStr const& strKey3,
        CStr const& strImp)
    {
        if (m_isInited)
        {
            auto gameMode = GetGameModeIdByName(strGameMode);
            if (gameMode == -1)
            {
                M3D_LOG_INFO("UnbindKey failed because invalid game mode was specified: " + strGameMode);
                return 0;
            }
            auto id1 = GetKeyIdByName(strKey1);
            auto id2 = GetKeyIdByName(strKey2);
            auto id3 = GetKeyIdByName(strKey3);
            if (id1 == -1 && id2 == -1 && id3 == -1)
            {
                M3D_LOG_INFO(
                    "UnbindKey failed because not any valid keys were specified: " + strKey1 + " " + strKey2 + " " +
                    strKey3);
                return 0;
            }
            KeysSet keySet;
            if (id1 != -1)
            {
                keySet += id1;
            }
            if (id2 != -1)
            {
                keySet += id2;
            }
            if (id3 != -1)
            {
                keySet += id3;
            }

            auto impId = GetImpulseIdByName(strImp);
            if (impId == -1)
            {
                M3D_LOG_INFO("UnbindKey failed because invalid impulse was specified: " + strImp);
                return 0;
            }
            if (!m_bSuppressEvent)
            {
                m3d::Application::g_pApp->EnqueueMessage(46, impId, 0, 0, 0, {}, {});
            }
            if (action == 1)
            {
                BindKey0(gameMode, keySet, impId);
            }
            else
            {
                UnbindKey0(gameMode, keySet, impId);
            }
            return 1;
        }
        M3D_LOG_INFO("Key bindings: error unbind key cause impulses were not inited");
        return 0;
    }

    int GameImpulse::BindKey3(
        CStr const& strGameMode,
        CStr const& strKey1,
        CStr const& strKey2,
        CStr const& strKey3,
        CStr const& strImp)
    {
        return HandleBinding(1, strGameMode, strKey1, strKey2, strKey3, strImp);
    }

    CStr GameImpulse::GetFormattedScriptErrorDesc(eScriptError) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int GameImpulse::UnbindKey3(CStr const&, CStr const&, CStr const&, CStr const&, CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Class* GameImpulse::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    int GameImpulse::LoadFromDefaults()
    {
        if (m_isInited)
        {
            auto res = LoadFromFile(g_Kernel->GetEngineCfg().m_pathToDefaultKeyBindings.GetS());
            if (res)
            {
                M3D_LOG_INFO("Key bindings: were bind from defaults");
            }
            return res;
        }
        M3D_LOG_INFO("Key bindings: error load from defaults cause impulses were not inited");
        return 0;
    }

    int GameImpulse::UnbindKey2(CStr const&, CStr const&, CStr const&, CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GameImpulse::UnbindAll()
    {
        if (m_isInited)
        {
            for (auto& bind : m_bindings)
            {
                bind.second.UnbindAll();
            }
            m_bindings.clear();
            m_impulseStates.clear();
            m_impulseResetAfterRead.clear();
            m_curKeys.clear();
            m_isBinded = false;
            if (!m_bSuppressEvent)
            {
                m3d::Application::g_pApp->EnqueueMessage(46, -1, 0, 0, 0, {}, {});
            }
            M3D_LOG_INFO("Key bindings: unbind all is done");
        }
    }

    int GameImpulse::GetKeyIdByName(CStr const& name)
    {
        if (!m_isInited || name.empty())
        {
            return -1;
        }
        for (auto const& keyName : l_keyNames)
        {
            if (keyName.m_keyName == name)
            {
                return keyName.m_keyId;
            }
        }
        return -1;
    }

    int GameImpulse::UnbindKey1(CStr const&, CStr const&, CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr GameImpulse::GetKeyNameById(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Class* GameImpulse::GetRtClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int GameImpulse::LoadFromProfile()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GameImpulse::UnbindKey0(int, KeysSet const&, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int GameImpulse::SaveToFile(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GameImpulse::GameImpulse()
    {
        this->m_isBinded = 0;
        this->m_isInited = 0;
        this->m_bSuppressEvent = 0;
        m_profileFileName = "KeyBindings.lua";
    }
}  // namespace m3d
