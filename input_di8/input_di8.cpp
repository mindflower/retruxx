// The DirectInput 8 input driver: keyboard, mouse and joystick read on a worker thread, a keyboard
// event queue with autorepeat, key translation through the Windows keyboard layouts.
// Ported from the original input_di8/input_di8.cpp; the functions are in the order of their
// original source lines. Each definition carries the RVA and start line of its original in the
// original binary.
//
// Differences from the original compiland, all forced by the executable the DLL is loaded into:
//  - The original linked the driver into the executable and reached the kernel, the log and the asserts
//    directly. Hard Truck Apocalypse hands the kernel to createIInput and the log callback to
//    Init(kernel, logFunc), so the M3D_LOG_INFO / M3D_LOG_ERR calls of the original go through that
//    callback (the executable logs them as "input: ..." at its own level) and M3D_ASSERT reaches
//    the kernel's SysError(whence, descr) with the original assertion text, file and line.
//  - The original's IInput has a parameterless Init() and a virtual IsKeyDown(); the HTA interface has
//    Init(kernel, logFunc) and no IsKeyDown slot, so IsKeyDown is a plain member here. The
//    vtable matches HTA's input_di8.dll: the 18 IInput slots followed by JoystickAttached.
#define DIRECTINPUT_VERSION 0x0800
#include <windows.h>
#include <dinput.h>

#include <string.h>
#include <stdlib.h>

#include <config.h>
#include <core/console/cvar.h>
#include <core/kernel.h>
#include <core/stringm3d.h>
#include <core/timer.h>
#include <i_event.h>
#include <iface.h>

#include "log.h"

// The original assertion texts name the kernel the way the statically linked driver saw it.
using m3d::g_Kernel;

namespace
{
    // The failure path of the original M3D_ASSERT: Kernel::SysError(assertion, file, line). HTA's
    // kernel has SysError(whence, descr); the message is assembled the way HTA's SYS_ERROR does.
    void inputAssertFailed(char const* assertion, char const* file, int line)
    {
        g_kernel->SysError(CStr(file) + CStr(":") + CStr(line), CStr(assertion));
    }
}

// The original M3D_ASSERT with the file name and line number the original binary carries.
#define INPUT_ASSERT(cond, line)                                    \
    if (!(cond))                                                    \
    {                                                               \
        inputAssertFailed(#cond, ".\\input_di8.cpp", (line));       \
    }

// orig input_di8.cpp:44
class AutoLockMutex
{
public:
    // orig 0x56e790 input_di8.cpp:48
    AutoLockMutex(HANDLE mutex) : m_mutex(mutex)
    {
        // 49
        WaitForSingleObject(m_mutex, INFINITE);
    }

    // orig 0x56e7b0 input_di8.cpp:53
    ~AutoLockMutex()
    {
        // 54
        ReleaseMutex(m_mutex);
    }

private:
    HANDLE m_mutex;
};

// orig input_di8.cpp:58
class CInput_di8 : public m3d::input::IInput
{
public:
    enum
    {
        KBD_QUEUE_SIZE = 0x1000,
    };

    struct CKbdEvent
    {
        /* 0x0000 */ uint16_t m_key;
        /* 0x0008 */ double m_time;
        /* 0x0010 */ int m_state;
    }; /* size: 0x0018 */

    // 61: IBase
    int IncRef() override;
    int DecRef() override;
    void* QueryIface(const char* ifaceName) override;

    CInput_di8();
    ~CInput_di8() override;

    // IInput, in the order of Hard Truck Apocalypse's vtable
    int Init(m3d::Kernel* kernel, void(__fastcall* logFunc)(CStr const&)) override;
    void NewFrame() override;
    void SetAutorepeatTime(int msecs) override;
    void ClearBuffer() override;
    bool GetLastKbdEvent(unsigned short& key, unsigned char& scanCode, bool& down, double& time,
                         bool removeFromQueue) override;
    int GetMouseX() override;
    int GetMouseY() override;
    int GetMouseZ() override;
    int GetMouseB(int num) override;
    int GetParam(m3d::input::DeviceParam what) override;
    int SetActiveState(int state) override;
    void ChangeLanguage() override;
    void SetLanguage(m3d::input::Language language) override;
    m3d::input::Language GetLanguage() const override;

    // Not part of HTA's IInput; the one virtual of the class itself (slot 18, as in HTA's DLL).
    virtual bool JoystickAttached();

    // original: virtual IInput::IsKeyDown; HTA's interface has no such slot.
    int IsKeyDown(int key);

    int InitKeyboardLayots();
    void ClearKeyboardLayots();
    CPINFOEXA GetCodePage();

    static DWORD WINAPI diThread(void* param);
    void done();
    void copyKbdMap(uint8_t* buf);
    void addKbdEvent(unsigned int scanCode, double time, int state);
    void enqueueKbdEvent(CKbdEvent* ev);

    /* 0x0004 */ int m_refCount;
    /* 0x0008 */ IBase* m_parent;
    /* 0x000c */ HWND m_forWnd;
    /* 0x0010 */ char m_keyMap[256];
    /* 0x0110 */ int m_mouse0[3];
    /* 0x011c */ int m_mouse1[3];
    /* 0x0128 */ int m_params[m3d::input::DP_NUM_PARAMS];
    /* 0x016c */ int m_isActive;
    /* 0x0170 */ m3d::input::Language m_language;

    static HANDLE eventShutdown;
    static HANDLE eventActiveStateChanged;
    static HANDLE threadH;
    static DWORD threadId;

    /* 0x0174 */ HANDLE m_mutex0;
    /* 0x0178 */ HANDLE m_mutex1;
    /* 0x017c */ HANDLE m_mutex2;
    /* 0x0180 */ CKbdEvent* m_kbdQueue;
    /* 0x0184 */ int m_kbdQueueHead;
    /* 0x0188 */ int m_kbdQueueTail;
    /* 0x018c */ int m_autorepeatTime;
    /* 0x0190 */ bool m_resetBuffSignalled;
    /* 0x0191 */ uint8_t m_asciiToChar[256];
    /* 0x0294 */ HKL m_hBaseLocalKeyboardLayot;
    /* 0x0298 */ HKL m_hAdditionalLocalKeyboardLayot;
    /* 0x029c */ bool m_bNeedUnloadBaseKeyboardLayot;
    /* 0x029d */ bool m_bNeedUnloadAdditionalKeyboardLayot;
    /* 0x02a0 */ void* m_pDi;
    /* 0x02a4 */ void* m_joystick;
}; /* size: 0x02a8 */

// Didnt true for a x64
// static_assert(sizeof(CInput_di8::CKbdEvent) == 0x18);
// static_assert(sizeof(CInput_di8) == 0x2a8);
// static_assert(offsetof(CInput_di8, m_params) == 0x128);
// static_assert(offsetof(CInput_di8, m_asciiToChar) == 0x191);
// static_assert(offsetof(CInput_di8, m_joystick) == 0x2a4);

// orig 0x8997e0 input_di8.cpp (static member)
HANDLE CInput_di8::eventShutdown = 0;
// orig 0x8997e4 input_di8.cpp (static member)
HANDLE CInput_di8::eventActiveStateChanged = 0;
// orig 0x8997e8 input_di8.cpp (static member)
HANDLE CInput_di8::threadH = 0;
// orig 0x8997ec input_di8.cpp (static member)
DWORD CInput_di8::threadId = 0;

// orig 0x56e7c0 input_di8.cpp:61
int CInput_di8::IncRef()
{
    if (m_parent)
    {
        m_parent->IncRef();
    }
    return ++m_refCount;
}

// orig 0x56e7e0 input_di8.cpp:61
int CInput_di8::DecRef()
{
    int refs = --m_refCount;
    if (m_parent)
    {
        m_parent->DecRef();
    }
    if (m_refCount <= 0)
    {
        delete this;
    }
    return refs;
}

// orig 0x56e810 input_di8.cpp:61
void* CInput_di8::QueryIface(const char* ifaceName)
{
    if (m_parent)
    {
        return m_parent->QueryIface(ifaceName);
    }
    return 0;
}

// orig 0x56f1c0 input_di8.cpp:337
CInput_di8::CInput_di8()
{
    // 338
    m_refCount = 0;
    m_parent = 0;

    // 340
    m_kbdQueue = new CKbdEvent[KBD_QUEUE_SIZE];
    m_kbdQueueHead = 0;
    m_kbdQueueTail = 0;
    m_autorepeatTime = 200;

    // 346
    memset(m_keyMap, 0, sizeof(m_keyMap));
    // 347
    memset(m_params, 0, sizeof(m_params));

    // 351
    m_isActive = 1;
    m_mouse0[0] = m_mouse0[1] = m_mouse0[2] = 0;
    // 352
    m_mouse1[0] = m_mouse1[1] = m_mouse1[2] = 0;

    m_resetBuffSignalled = false;
    m_language = m3d::input::LANGUAGE_ADDITIONAL;
    m_pDi = 0;
    m_joystick = 0;
    m_hBaseLocalKeyboardLayot = 0;
    m_hAdditionalLocalKeyboardLayot = 0;
    m_bNeedUnloadBaseKeyboardLayot = false;
    m_bNeedUnloadAdditionalKeyboardLayot = false;

    // 370: the scan code -> engine key code table (m3d::KBD_xxx and the ASCII of the numeric pad);
    // a zero entry means the key is translated through the keyboard layout in GetLastKbdEvent.
    memset(m_asciiToChar, 0, sizeof(m_asciiToChar));
    m_asciiToChar[DIK_ESCAPE] = m3d::KBD_ESC;
    m_asciiToChar[DIK_BACK] = m3d::KBD_BKSPACE;
    m_asciiToChar[DIK_TAB] = m3d::KBD_TAB;
    m_asciiToChar[DIK_RETURN] = m3d::KBD_ENTER;
    m_asciiToChar[DIK_LCONTROL] = m3d::KBD_CTRL;
    // 383
    m_asciiToChar[DIK_LSHIFT] = m3d::KBD_SHIFT;
    // 384
    m_asciiToChar[DIK_RSHIFT] = m3d::KBD_SHIFT;
    m_asciiToChar[DIK_MULTIPLY] = '*';
    // 386
    m_asciiToChar[DIK_LMENU] = m3d::KBD_ALT;
    // 387
    m_asciiToChar[DIK_CAPITAL] = m3d::KBD_CAPSLOCK;
    // 388
    m_asciiToChar[DIK_F1] = m3d::KBD_F1;
    m_asciiToChar[DIK_F2] = m3d::KBD_F2;
    m_asciiToChar[DIK_F3] = m3d::KBD_F3;
    m_asciiToChar[DIK_F4] = m3d::KBD_F4;
    m_asciiToChar[DIK_F5] = m3d::KBD_F5;
    m_asciiToChar[DIK_F6] = m3d::KBD_F6;
    m_asciiToChar[DIK_F7] = m3d::KBD_F7;
    m_asciiToChar[DIK_F8] = m3d::KBD_F8;
    m_asciiToChar[DIK_F9] = m3d::KBD_F9;
    // 397
    m_asciiToChar[DIK_F10] = m3d::KBD_F10;
    // 398
    m_asciiToChar[DIK_NUMLOCK] = m3d::KBD_NUMLOCK;
    // 399
    m_asciiToChar[DIK_NUMPAD7] = '7';
    m_asciiToChar[DIK_NUMPAD8] = '8';
    m_asciiToChar[DIK_NUMPAD9] = '9';
    // 402
    m_asciiToChar[DIK_SUBTRACT] = '-';
    m_asciiToChar[DIK_NUMPAD4] = '4';
    m_asciiToChar[DIK_NUMPAD5] = '5';
    m_asciiToChar[DIK_NUMPAD6] = '6';
    // 406
    m_asciiToChar[DIK_ADD] = '+';
    m_asciiToChar[DIK_NUMPAD1] = '1';
    m_asciiToChar[DIK_NUMPAD2] = '2';
    m_asciiToChar[DIK_NUMPAD3] = '3';
    m_asciiToChar[DIK_NUMPAD0] = '0';
    m_asciiToChar[DIK_DECIMAL] = '.';
    m_asciiToChar[DIK_F11] = m3d::KBD_F11;
    m_asciiToChar[DIK_F12] = m3d::KBD_F12;
    m_asciiToChar[DIK_RCONTROL] = m3d::KBD_CTRL;
    m_asciiToChar[DIK_NUMPADCOMMA] = ',';
    m_asciiToChar[DIK_DIVIDE] = '/';
    m_asciiToChar[DIK_SYSRQ] = 0xfe;
    // 418
    m_asciiToChar[DIK_RMENU] = m3d::KBD_ALT;
    // 419
    m_asciiToChar[DIK_HOME] = m3d::KBD_HOME;
    // 420
    m_asciiToChar[DIK_UP] = m3d::KBD_UP;
    // 421
    m_asciiToChar[DIK_PRIOR] = m3d::KBD_PGUP;
    // 422
    m_asciiToChar[DIK_LEFT] = m3d::KBD_LEFT;
    // 423
    m_asciiToChar[DIK_RIGHT] = m3d::KBD_RIGHT;
    // 424
    m_asciiToChar[DIK_END] = m3d::KBD_END;
    // 425
    m_asciiToChar[DIK_DOWN] = m3d::KBD_DOWN;
    // 426
    m_asciiToChar[DIK_NEXT] = m3d::KBD_PGDN;
    // 427
    m_asciiToChar[DIK_INSERT] = m3d::KBD_INSERT;
    // 428
    m_asciiToChar[DIK_DELETE] = m3d::KBD_DELETE;
}

// orig 0x56f3f0 input_di8.cpp:439
CInput_di8::~CInput_di8()
{
    // 440
    done();
    // 441
    if (m_kbdQueue)
    {
        delete[] m_kbdQueue;
    }
}

// orig 0x56e830 input_di8.cpp:447
bool CInput_di8::JoystickAttached()
{
    // 448
    return m_joystick != 0;
}

// orig 0x56e840 input_di8.cpp:457
void CInput_di8::NewFrame()
{
    // 458
    AutoLockMutex lock(m_mutex2);

    // 462
    for (int i = 0; i < 3; ++i)
    {
        m_params[m3d::input::DP_MOUSE_X + i] = m_mouse1[i] - m_mouse0[i];
        // 463
        m_mouse0[i] = m_mouse1[i];
    }

    // 469
    if (m_pDi && m_joystick && m_isActive)
    {
        // 471
        AutoLockMutex lock(m_mutex2);

        // 477
        IDirectInputDevice8A* joystick = (IDirectInputDevice8A*)m_joystick;
        // 478
        HRESULT hr = joystick->Poll();
        // 479
        if (FAILED(hr))
        {
            // 482
            hr = joystick->Acquire();
            // 497
            for (int i = 0; FAILED(hr) && i < 10; ++i)
            {
                // 499
                Sleep(1);
                // 500
                hr = joystick->Acquire();
            }
            // 509
            if (FAILED(hr))
            {
                return;
            }
        }

        // 511
        DIJOYSTATE2 js;
        hr = joystick->GetDeviceState(sizeof(DIJOYSTATE2), &js);

        // 517
        m_params[m3d::input::DP_JOY_X] = js.lX;
        // 518
        m_params[m3d::input::DP_JOY_Y] = js.lY;
        m_params[m3d::input::DP_JOY_Z] = js.lZ;
        // 520
        m_params[m3d::input::DP_JOY_RX] = js.lRx;
        // 521
        m_params[m3d::input::DP_JOY_RY] = js.lRy;
        m_params[m3d::input::DP_JOY_RZ] = js.lRz;
        m_params[m3d::input::DP_JOY_S0] = js.rglSlider[0];
        // 524
        m_params[m3d::input::DP_JOY_S1] = js.rglSlider[1];
        m_params[m3d::input::DP_JOY_P0] = js.rgdwPOV[0];
        m_params[m3d::input::DP_JOY_P1] = js.rgdwPOV[1];
        // 527
        m_params[m3d::input::DP_JOY_P2] = js.rgdwPOV[2];
        m_params[m3d::input::DP_JOY_P3] = js.rgdwPOV[3];

        // 529
        m_params[m3d::input::DP_JOY_B] = 0;
        // 531
        for (int i = 0; i < 32; ++i)
        {
            if (js.rgbButtons[i])
            {
                // 532
                m_params[m3d::input::DP_JOY_B] |= 1 << i;
            }
        }
        // 535
    }
    // 536
}

// orig 0x570bb0 input_di8.cpp:541
// original: int CInput_di8::Init() on the statically linked driver. Hard Truck Apocalypse's interface
// hands over the kernel and the log callback here; its own input_di8.dll stores both before the
// The original body, which is kept unchanged.
int CInput_di8::Init(m3d::Kernel* kernel, void(__fastcall* logFunc)(CStr const&))
{
    g_kernel = kernel;
    m3d::g_Kernel = kernel;
    SetLogFunc(logFunc);

    // 542
    INPUT_ASSERT(IsWindow( g_Kernel->GetEngineCfg().m_mainWnd ), 542);

    // 544
    m_forWnd = g_kernel->GetEngineCfg().m_mainWnd;

    // 546
    m_mutex0 = CreateMutexA(0, 0, 0);
    // 547
    m_mutex1 = CreateMutexA(0, 0, 0);
    // 548
    m_mutex2 = CreateMutexA(0, 0, 0);

    // 550
    eventShutdown = CreateEventA(0, 0, 0, 0);
    // 551
    eventActiveStateChanged = CreateEventA(0, 0, 0, 0);

    // 554
    threadH = CreateThread(0, 0, diThread, this, 0, &threadId);
    // 555
    INPUT_ASSERT(eventShutdown && threadH, 555);

    // 557
    InitKeyboardLayots();
    // 558
    m_language = g_kernel->GetEngineCfg().m_input_defaultInputLanguageBase.GetB()
                     ? m3d::input::LANGUAGE_BASE
                     : m3d::input::LANGUAGE_ADDITIONAL;

    // 560
    return 1;
}

// orig 0x56ef20 input_di8.cpp:566
void CInput_di8::done()
{
    // 568
    SetEvent(eventShutdown);

    // 571
    WaitForSingleObject(threadH, 3000);

    // 577
    CloseHandle(eventShutdown);
    eventShutdown = 0;
    // 581
    CloseHandle(threadH);
    threadH = 0;
    // 585
    CloseHandle(eventActiveStateChanged);
    // 586
    eventActiveStateChanged = 0;

    // 589
    CloseHandle(m_mutex0);
    // 590
    CloseHandle(m_mutex1);
    // 591
    CloseHandle(m_mutex2);

    // 596
    ClearKeyboardLayots();
}

// orig 0x56efe0 input_di8.cpp:602
void CInput_di8::addKbdEvent(unsigned int scanCode, double time, int state)
{
    CKbdEvent ev;

    // 615
    ev.m_key = (uint16_t)(scanCode
                          | (IsKeyDown(DIK_LCONTROL) ? m3d::KBD_LCTRL : 0)
                          | (IsKeyDown(DIK_RCONTROL) ? m3d::KBD_RCTRL : 0)
                          | (IsKeyDown(DIK_LSHIFT) ? m3d::KBD_LSHIFT : 0)
                          | (IsKeyDown(DIK_RSHIFT) ? m3d::KBD_RSHIFT : 0)
                          | (IsKeyDown(DIK_LMENU) ? m3d::KBD_LALT : 0)
                          | (IsKeyDown(DIK_RMENU) ? m3d::KBD_RALT : 0)
                          | (IsKeyDown(DIK_LWIN) ? m3d::KBD_LWIN : 0)
                          | (IsKeyDown(DIK_RWIN) ? m3d::KBD_RWIN : 0));
    // 627
    ev.m_time = time;
    ev.m_state = state;
    enqueueKbdEvent(&ev);
}

// orig 0x56ea40 input_di8.cpp:633
void CInput_di8::copyKbdMap(uint8_t* buf)
{
    // 634
    AutoLockMutex lock(m_mutex0);

    // 636
    memcpy(m_keyMap, buf, sizeof(m_keyMap));
    // 638
}

// orig 0x56ea70 input_di8.cpp:644
int CInput_di8::IsKeyDown(int key)
{
    int res;
    {
        // 647
        AutoLockMutex lock(m_mutex0);

        // 649
        res = m_keyMap[key];
        // 650
    }

    // 652
    return res;
}

// orig 0x56eaa0 input_di8.cpp:658
void CInput_di8::enqueueKbdEvent(CKbdEvent* ev)
{
    // 659
    AutoLockMutex lock(m_mutex1);

    // 662
    int newHead = m_kbdQueueHead + 1;
    // 663
    if (newHead >= KBD_QUEUE_SIZE)
    {
        // 664
        newHead = 0;
    }

    // 667
    if (m_kbdQueueTail != newHead)
    {
        // 669
        m_kbdQueue[m_kbdQueueHead] = *ev;
        // 670
        m_kbdQueueHead = newHead;
    }
    // 676
}

// orig 0x56f420 input_di8.cpp:681
bool CInput_di8::GetLastKbdEvent(unsigned short& key, unsigned char& scanCode, bool& down, double& time,
                                 bool removeFromQueue)
{
    // 686
    static uint8_t kbState[256];
    memset(kbState, 0, sizeof(kbState));

    bool res = false;

    // 690
    AutoLockMutex lock(m_mutex1);

    // 693
    if (m_kbdQueueTail != m_kbdQueueHead)
    {
        // 698
        key = m_kbdQueue[m_kbdQueueTail].m_key;
        // 699
        down = m_kbdQueue[m_kbdQueueTail].m_state != 0;
        // 700
        time = m_kbdQueue[m_kbdQueueTail].m_time;
        // 701
        scanCode = (uint8_t)key;

        // 723
        uint16_t ascii = m_asciiToChar[(uint8_t)key];

        // 725
        if (ascii == 0)
        {
            // 727
            HKL layout = m_hBaseLocalKeyboardLayot;

            // 729
            if (m_language == m3d::input::LANGUAGE_ADDITIONAL && m_hAdditionalLocalKeyboardLayot)
            {
                // 732
                CPINFOEXA codePage = GetCodePage();
                // 733
                if (codePage.MaxCharSize == 1)
                {
                    // 734
                    layout = m_hAdditionalLocalKeyboardLayot;
                }
            }

            // 739
            UINT virtualKey = MapVirtualKeyExA((uint8_t)key, MAPVK_VSC_TO_VK, layout);
            // 740
            if (virtualKey != 0)
            {
                // 746
                uint16_t chars[2] = { 0, 0 };
                if (key & (m3d::KBD_LSHIFT | m3d::KBD_RSHIFT))
                {
                    // 747
                    kbState[VK_SHIFT] = 0x80;
                }

                // 764
                if (ToAsciiEx(virtualKey, (uint8_t)key, kbState, chars, 0, layout) == 0)
                {
                    // 766: M3D_LOG_INFO in the original
                    LogMsg(CStr("CInput_di8::GetLastKbdEvent() error - cannot translate key wit virtual key code ") +
                           CStr(virtualKey));
                }

                // 769
                ascii = chars[0];
                // 771
            }
            else
            {
                // 776: M3D_LOG_INFO in the original
                LogMsg(CStr("CInput_di8::GetLastKbdEvent() error - cannot translate key wit scan code ") +
                       CStr((int)key));
            }
        }

        // 781
        key = (uint16_t)((key & 0xff00) | ascii);

        // 784
        if (removeFromQueue)
        {
            // 786
            int newTail = m_kbdQueueTail + 1;
            // 787
            if (newTail >= KBD_QUEUE_SIZE)
            {
                // 788
                newTail = 0;
            }
            // 790
            m_kbdQueueTail = newTail;
        }

        // 794
        res = true;
    }

    // 797
    return res;
}

// orig 0x56eb20 input_di8.cpp:803
void CInput_di8::ClearBuffer()
{
    // 804
    m_kbdQueueHead = 0;
    // 805
    m_kbdQueueTail = 0;
    // 806
    m_resetBuffSignalled = true;
}

// orig 0x56eb40 input_di8.cpp:812
// events[1] is the duplicated shutdown event of diThread; events[0] the device's own event.
static bool AcquireDevice(IDirectInputDevice8A* dev, HANDLE* events)
{
    // 814
    for (unsigned int i = 0; i < 1000; ++i)
    {
        // 817
        HRESULT hr = dev->Acquire();
        // 818
        if (SUCCEEDED(hr))
        {
            break;
        }

        // 823
        if (WaitForSingleObject(events[1], 0) == WAIT_OBJECT_0)
        {
            // 827
            if (events[0])
            {
                // 828
                CloseHandle(events[0]);
            }
            // 831
            return false;
        }

        // 835
        Sleep(1);
    }

    // 838
    return true;
}

struct JoyEnumContext
{
    /* 0x0000 */ IDirectInputDevice8A** m_joystick;
    /* 0x0004 */ IDirectInput8A* m_pDi;
}; /* size: 0x0008 */

// orig 0x56ebb0 input_di8.cpp:870
static BOOL CALLBACK EnumJoysticksCallback(DIDEVICEINSTANCEA const* instance, void* context)
{
    JoyEnumContext* cc = (JoyEnumContext*)context;

    // 876
    HRESULT hr = cc->m_pDi->CreateDevice(instance->guidInstance, cc->m_joystick, 0);
    // 877
    if (FAILED(hr))
    {
        return DIENUM_CONTINUE;
    }

    // 883
    return DIENUM_STOP;
}

struct JoyAxesEnumContext
{
    /* 0x0000 */ HWND m_wnd;
    /* 0x0004 */ IDirectInputDevice8A* m_joystick;
    /* 0x0008 */ IDirectInput8A* m_pDi;
    /* 0x000c */ int m_numSliders;
    /* 0x0010 */ int m_numPovs;
    /* 0x0014 */ int m_axisX;
    /* 0x0018 */ int m_axisY;
    /* 0x001c */ int m_axisZ;
    /* 0x0020 */ int m_axisRx;
    /* 0x0024 */ int m_axisRy;
    /* 0x0028 */ int m_axisRz;
}; /* size: 0x002c */

// orig 0x56ebe0 input_di8.cpp:888
static BOOL CALLBACK EnumAxesCallback(DIDEVICEOBJECTINSTANCEA const* doi, void* context)
{
    JoyAxesEnumContext* ccc = (JoyAxesEnumContext*)context;

    // 893
    if (doi->dwType & DIDFT_AXIS)
    {
        DIPROPRANGE diprg;
        diprg.diph.dwSize = sizeof(DIPROPRANGE);
        diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        diprg.diph.dwObj = doi->dwType;
        diprg.diph.dwHow = DIPH_BYID;
        diprg.lMin = -1000;
        diprg.lMax = +1000;

        // 904
        if (FAILED(ccc->m_joystick->SetProperty(DIPROP_RANGE, &diprg.diph)))
        {
            // 905
            return DIENUM_STOP;
        }
    }

    // 909
    if (doi->guidType == GUID_XAxis)
    {
        // 911
        ccc->m_axisX = 1;
    }
    // 913
    else if (doi->guidType == GUID_YAxis)
    {
        // 915
        ccc->m_axisY = 1;
    }
    // 917
    else if (doi->guidType == GUID_ZAxis)
    {
        // 919
        ccc->m_axisZ = 1;
    }
    // 921
    else if (doi->guidType == GUID_RxAxis)
    {
        // 923
        ccc->m_axisRx = 1;
    }
    // 925
    else if (doi->guidType == GUID_RyAxis)
    {
        // 927
        ccc->m_axisRy = 1;
    }
    // 929
    else if (doi->guidType == GUID_RzAxis)
    {
        // 931
        ccc->m_axisRz = 1;
    }
    // 933
    else if (doi->guidType == GUID_Slider)
    {
        // 935
        ccc->m_numSliders++;
    }
    // 937
    else if (doi->guidType == GUID_POV)
    {
        // 939
        ccc->m_numPovs++;
    }

    // 943
    return DIENUM_CONTINUE;
}

// orig 0x56f700 input_di8.cpp:948
DWORD WINAPI CInput_di8::diThread(void* param)
{
    CInput_di8* input = (CInput_di8*)param;
    IDirectInput8A* pDi = 0;
    IDirectInputDevice8A* kbd = 0;
    IDirectInputDevice8A* mouse = 0;
    IDirectInputDevice8A* joystick = 0;
    // events: 0 keyboard, 1 shutdown (duplicated), 2 mouse, 3 active state changed (duplicated)
    HANDLE events[4];
    uint8_t buf[256];     // the keyboard state just read
    uint8_t buffer[256];  // the keyboard state of the previous read
    HRESULT hr;

    // 974
    hr = DirectInput8Create(GetModuleHandleA(0), DIRECTINPUT_VERSION, IID_IDirectInput8A, (void**)&pDi, 0);
    if (FAILED(hr))
    {
        // 976
        INPUT_ASSERT(!"Cant create di8", 976);
        // 977
        return hr;
    }

    // 983
    if (!DuplicateHandle(GetCurrentProcess(), eventShutdown, GetCurrentProcess(), &events[1], 0, FALSE,
                         DUPLICATE_SAME_ACCESS))
    {
        // 985
        INPUT_ASSERT(!"Cant duplicate handle 1", 985);
        // 986
        return E_FAIL;
    }

    // 992
    if (!DuplicateHandle(GetCurrentProcess(), eventActiveStateChanged, GetCurrentProcess(), &events[3], 0, FALSE,
                         DUPLICATE_SAME_ACCESS))
    {
        // 994
        INPUT_ASSERT(!"Cant duplicate handle 2", 994);
        // 995
        return E_FAIL;
    }

    // 1003
    hr = pDi->CreateDevice(GUID_SysKeyboard, &kbd, 0);
    if (FAILED(hr))
    {
        // 1005
        INPUT_ASSERT(!"Cant create device", 1005);
        // 1006
        return hr;
    }

    // 1010
    hr = kbd->SetDataFormat(&c_dfDIKeyboard);
    if (FAILED(hr))
    {
        // 1012
        INPUT_ASSERT(!"Cant set dtf", 1012);
        // 1013
        return hr;
    }

    // 1017
    hr = kbd->SetCooperativeLevel(input->m_forWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
    if (FAILED(hr))
    {
        // 1019
        INPUT_ASSERT(!"Cant set coop level", 1019);
        // 1020
        return hr;
    }

    // 1024
    events[0] = CreateEventA(0, 0, 0, 0);
    // 1026
    if (!events[0])
    {
        // 1028
        INPUT_ASSERT(!"Cant create event", 1028);
        // 1029
        return E_FAIL;
    }

    // 1033
    hr = kbd->SetEventNotification(events[0]);
    // 1034
    if (FAILED(hr))
    {
        // 1036
        INPUT_ASSERT(!"Cant set notify", 1036);
        // 1037
        return E_FAIL;
    }

    {
        // 1044
        DIPROPDWORD prop;
        prop.diph.dwObj = 0;
        prop.diph.dwHow = DIPH_DEVICE;
        // 1047
        prop.diph.dwSize = sizeof(DIPROPDWORD);
        prop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        prop.dwData = 1023;
        hr = kbd->SetProperty(DIPROP_BUFFERSIZE, &prop.diph);
        // 1048
        if (FAILED(hr))
        {
            // 1050
            INPUT_ASSERT(!"Cant set kbd buffersize", 1050);
            // 1051
            return E_FAIL;
        }
    }

    // 1055
    if (!AcquireDevice(kbd, events))
    {
        // 1057
        kbd->Release();
        // 1058
        pDi->Release();
        // 1061: M3D_LOG_INFO in the original
        LogMsg("Di thread kbd acquisition failed, leaving by command\n");
        // 1535
        return 0;
    }

    // 1073
    hr = pDi->CreateDevice(GUID_SysMouse, &mouse, 0);
    if (FAILED(hr))
    {
        // 1075
        INPUT_ASSERT(!"Cant create mouse", 1075);
        // 1076
        return hr;
    }

    // 1080
    hr = mouse->SetDataFormat(&c_dfDIMouse2);
    if (FAILED(hr))
    {
        // 1082
        INPUT_ASSERT(!"Cant set ms dtf", 1082);
        // 1083
        return hr;
    }

    // 1089
    hr = mouse->SetCooperativeLevel(input->m_forWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(hr))
    {
        // 1091
        INPUT_ASSERT(!"Cant set ms coop level", 1091);
        // 1092
        return hr;
    }

    // 1096
    events[2] = CreateEventA(0, 0, 0, 0);
    // 1098
    if (!events[2])
    {
        // 1100
        INPUT_ASSERT(!"Cant create ms event", 1100);
        // 1101
        return E_FAIL;
    }

    // 1105
    hr = mouse->SetEventNotification(events[2]);
    // 1106
    if (FAILED(hr))
    {
        // 1108
        INPUT_ASSERT(!"Cant set ms notfi", 1108);
        // 1109
        return E_FAIL;
    }

    {
        // 1120
        DIPROPDWORD prop;
        prop.diph.dwSize = sizeof(DIPROPDWORD);
        prop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        prop.diph.dwObj = 0;
        prop.diph.dwHow = DIPH_DEVICE;
        prop.dwData = 1023;
        hr = mouse->SetProperty(DIPROP_BUFFERSIZE, &prop.diph);
        // 1121
        if (FAILED(hr))
        {
            // 1123
            INPUT_ASSERT(!"Cant set ms buffersize", 1123);
            // 1124
            return E_FAIL;
        }
    }

    // 1128
    if (!AcquireDevice(mouse, events))
    {
        // 1130
        kbd->Release();
        // 1131
        mouse->Release();
        // 1132
        pDi->Release();
        // 1135: M3D_LOG_INFO in the original
        LogMsg("Di thread mouse acquisition failed, leaving by command");
        // 1535
        return 0;
    }

    // 1149
    JoyEnumContext cc;
    cc.m_joystick = &joystick;
    // 1150
    cc.m_pDi = pDi;
    // 1152
    pDi->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, &cc, DIEDFL_ATTACHEDONLY);

    // 1155
    if (joystick == 0)
    {
        // 1157: M3D_LOG_INFO in the original
        LogMsg("Joystick not found");
        // 1159
    }
    else
    {
        // 1162: M3D_LOG_INFO in the original
        LogMsg("Joystick found");

        // 1165
        if (FAILED(joystick->SetDataFormat(&c_dfDIJoystick2)))
        {
            // 1167: M3D_LOG_INFO in the original
            LogMsg("Cant set joystick dataformat");
            // 1535
            return 0;
        }

        // 1172
        if (FAILED(joystick->SetCooperativeLevel(input->m_forWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
        {
            // 1174: M3D_LOG_INFO in the original
            LogMsg("Cant set joystick coop level");
            // 1535
            return 0;
        }

        // 1180
        DIDEVCAPS jcaps;
        memset(&jcaps, 0, sizeof(jcaps));
        jcaps.dwSize = sizeof(DIDEVCAPS);
        hr = joystick->GetCapabilities(&jcaps);
        // 1182
        if (FAILED(hr))
        {
            // M3D_LOG_INFO in the original
            LogMsg("Cant get caps for joystick");
            // 1535
            return 0;
        }

        // 1186
        g_kernel->KernelLog("input: dwAxis = %d", jcaps.dwAxes);
        // 1187
        g_kernel->KernelLog("input: dwButtons = %d", jcaps.dwButtons);
        // 1188
        g_kernel->KernelLog("input: dwPOVs = %d", jcaps.dwPOVs);

        // 1190
        if (jcaps.dwFlags & DIDC_FORCEFEEDBACK)
        {
            // 1191: M3D_LOG_INFO in the original
            LogMsg("Force feedback = yes");
        }

        // 1199
        JoyAxesEnumContext ccc;
        memset(&ccc, 0, sizeof(ccc));
        ccc.m_wnd = input->m_forWnd;
        // 1200
        ccc.m_joystick = joystick;
        ccc.m_pDi = pDi;
        // 1201
        hr = joystick->EnumObjects(EnumAxesCallback, &ccc, DIDFT_AXIS);
        // 1203
        if (FAILED(hr))
        {
            // M3D_LOG_INFO in the original
            LogMsg("Cant setup axes");
            // 1535
            return 0;
        }

        // 1208
        g_kernel->KernelLog("input: Axes X:%d Y:%d Z:%d", ccc.m_axisX, ccc.m_axisY, ccc.m_axisZ);
        // 1209
        g_kernel->KernelLog("input: RotAxes Rx:%d Ry:%d Rz:%d", ccc.m_axisRx, ccc.m_axisRy, ccc.m_axisRz);
        // 1210
        g_kernel->KernelLog("input: NumSliders: %d", ccc.m_numSliders);
        // 1211
        g_kernel->KernelLog("input: NumPOVS: %d", ccc.m_numPovs);
    }

    // 1218
    input->m_pDi = pDi;
    // 1219
    input->m_joystick = joystick;

    // 1226
    int firstTime = 1;
    int lastKey = -1;
    // 1227
    DWORD waitTime = INFINITE;
    int tt;

    for (;;)
    {
        // 1239
        switch (WaitForMultipleObjects(4, events, FALSE, waitTime))
        {
        default:
            // 1243
            INPUT_ASSERT(!"Sick shit", 1243);
            // 1244
            break;

        case WAIT_OBJECT_0 + 0:
            // 1250: keyboard
            if (!input->m_isActive)
            {
                break;
            }

            // 1254
            tt = g_kernel->GetTimer().GetFrameStartTime();

            // 1256
            if (input->m_resetBuffSignalled)
            {
                // 1259
                memset(buffer, 0, sizeof(buffer));
                // 1260
                input->m_resetBuffSignalled = false;
                // 1261
                firstTime = 1;
            }

            do
            {
                // 1269
                hr = kbd->GetDeviceState(sizeof(buf), buf);

                // 1272
                if (hr != DIERR_NOTACQUIRED && hr != DIERR_INPUTLOST)
                {
                    if (hr != DI_OK)
                    {
                        // 1276
                        INPUT_ASSERT(!"Sick shit in input", 1276);
                        // 1277
                    }
                }
                else
                {
                    // 1286
                    if (input->m_isActive)
                    {
                        // 1287
                        kbd->Acquire();
                    }
                }

                // 1292
                if (WaitForSingleObject(events[1], 0) == WAIT_OBJECT_0)
                {
                    goto shutdown;
                }
                // 1313
            } while (hr != DI_OK && input->m_isActive);

            // 1316
            if (!input->m_isActive)
            {
                break;
            }

            // 1320
            input->copyKbdMap(buf);

            // 1325
            if (!firstTime)
            {
                for (int i = 0; i < 256; ++i)
                {
                    // 1327
                    if (buf[i] != buffer[i])
                    {
                        // 1329
                        if (buf[i] & 0x80)
                        {
                            // 1332
                            input->addKbdEvent(i, (double)tt, 1);
                            // 1336
                            lastKey = i;
                            waitTime = input->m_autorepeatTime;
                            // 1338
                        }
                        else
                        {
                            // 1341
                            input->addKbdEvent(i, (double)tt, 0);
                        }
                    }
                }
                // 1346
            }
            else
            {
                // 1351
                for (int i = 0; i < 256; ++i)
                {
                    if (buf[i] & 0x80)
                    {
                        // 1354
                        input->addKbdEvent(i, (double)tt, 1);
                        // 1358
                        lastKey = i;
                        waitTime = input->m_autorepeatTime;
                    }
                }
            }

            // 1367
            memcpy(buffer, buf, sizeof(buffer));
            // 1370
            firstTime = 0;
            break;

        case WAIT_TIMEOUT:
            // 1376: autorepeat of the last key
            if (!input->m_isActive)
            {
                // 1380
                lastKey = -1;
                waitTime = INFINITE;
                break;
            }

            // 1385
            if (lastKey < 0)
            {
                // 1390
                lastKey = -1;
                waitTime = INFINITE;
                break;
            }

            // 1394
            tt = g_kernel->GetTimer().GetFrameStartTime();

            do
            {
                // 1400
                hr = kbd->GetDeviceState(sizeof(buf), buf);

                // 1403
                if (hr != DIERR_NOTACQUIRED && hr != DIERR_INPUTLOST)
                {
                    if (hr != DI_OK)
                    {
                        // 1407
                        INPUT_ASSERT(!"Sick shit", 1407);
                        // 1408
                    }
                }
                else
                {
                    // 1417
                    if (input->m_isActive)
                    {
                        // 1418
                        kbd->Acquire();
                    }
                }

                // 1423
                if (WaitForSingleObject(events[1], 0) == WAIT_OBJECT_0)
                {
                    goto shutdown;
                }
                // 1426
            } while (hr != DI_OK && input->m_isActive);

            // 1429
            if (!input->m_isActive)
            {
                break;
            }

            // 1433
            if (buf[lastKey] & 0x80)
            {
                // 1436
                waitTime = input->m_autorepeatTime;
                input->addKbdEvent(lastKey, (double)tt, 1);
                // 1438
            }
            else
            {
                lastKey = -1;
                waitTime = INFINITE;
            }
            break;

        case WAIT_OBJECT_0 + 2:
        {
            // 1450: mouse
            if (!input->m_isActive)
            {
                break;
            }

            // 1454
            DIMOUSESTATE2 dims2;
            memset(&dims2, 0, sizeof(dims2));

            do
            {
                // 1459
                hr = mouse->GetDeviceState(sizeof(DIMOUSESTATE2), &dims2);

                // 1462
                if (hr != DIERR_NOTACQUIRED && hr != DIERR_INPUTLOST)
                {
                    if (hr != DI_OK)
                    {
                        // 1465
                        INPUT_ASSERT(!"Sick shit", 1465);
                        // 1466
                    }
                }
                else
                {
                    // 1470
                    if (input->m_isActive)
                    {
                        // 1471
                        mouse->Acquire();
                    }
                }

                // 1478
                if (WaitForSingleObject(events[1], 0) == WAIT_OBJECT_0)
                {
                    goto shutdown;
                }
                // 1481
            } while (hr != DI_OK && input->m_isActive);

            // 1484
            if (!input->m_isActive)
            {
                break;
            }

            {
                // 1490
                AutoLockMutex lock(input->m_mutex2);

                // 1493
                input->m_mouse1[0] += dims2.lX;
                // 1494
                input->m_mouse1[1] += dims2.lY;
                // 1495
                input->m_mouse1[2] += dims2.lZ;

                // 1498
                input->m_params[m3d::input::DP_MOUSE_B] = 0;
                // 1501
                for (unsigned int i = 0; i < 8; ++i)
                {
                    if (dims2.rgbButtons[i])
                    {
                        // 1502
                        input->m_params[m3d::input::DP_MOUSE_B] |= 1 << i;
                    }
                }
                // 1504
            }
            // 1506
            break;
        }

        case WAIT_OBJECT_0 + 1:
            // 1514: shutdown
        shutdown:
            kbd->Unacquire();
            // 1515
            mouse->Unacquire();
            // 1516
            if (joystick)
            {
                // 1517
                joystick->Unacquire();
            }

            // 1520
            CloseHandle(events[0]);
            // 1521
            CloseHandle(events[1]);
            // 1522
            CloseHandle(events[2]);
            // 1523
            CloseHandle(events[3]);

            // 1526
            kbd->Release();
            // 1527
            mouse->Release();
            // 1528
            if (joystick)
            {
                joystick->Release();
                joystick = 0;
            }
            // 1529
            pDi->Release();
            // 1563
            return 0;

        case WAIT_OBJECT_0 + 3:
            // 1542: active state changed
            if (input->m_isActive)
            {
                // 1544
                kbd->Acquire();
                // 1545
                mouse->Acquire();
                // 1546
                if (joystick)
                {
                    // 1547
                    joystick->Acquire();
                }
                // 1549
            }
            else
            {
                // 1551
                kbd->Unacquire();
                // 1552
                mouse->Unacquire();
                // 1553
                if (joystick)
                {
                    // 1554
                    joystick->Unacquire();
                }
            }
            // 1562
            break;
        }
    }
}

// orig 0x56ed40 input_di8.cpp:1568
int CInput_di8::GetParam(m3d::input::DeviceParam what)
{
    // 1569
    AutoLockMutex lock(m_mutex2);

    // 1572
    if (what < 0 || what >= m3d::input::DP_NUM_PARAMS)
    {
        // 1574
        g_kernel->KernelLog("input: Invalid param query in GetParam(): %d", what);
        // 1575
        return 0;
    }

    // 1578
    return m_params[what];
}

// orig 0x56eda0 input_di8.cpp:1584
int CInput_di8::GetMouseX()
{
    // 1585
    return GetParam(m3d::input::DP_MOUSE_X);
}

// orig 0x56edb0 input_di8.cpp:1591
int CInput_di8::GetMouseY()
{
    // 1592
    return GetParam(m3d::input::DP_MOUSE_Y);
}

// orig 0x56edc0 input_di8.cpp:1598
int CInput_di8::GetMouseZ()
{
    // 1599
    return GetParam(m3d::input::DP_MOUSE_Z);
}

// orig 0x56edd0 input_di8.cpp:1605
int CInput_di8::GetMouseB(int num)
{
    // 1606
    return GetParam(m3d::input::DP_MOUSE_B) & (1 << num);
}

// orig 0x56f0c0 input_di8.cpp:1612
void CInput_di8::SetAutorepeatTime(int msecs)
{
    // 1613
    if (msecs < 100)
    {
        msecs = 100;
    }
    else if (msecs > 1000)
    {
        msecs = 1000;
    }
    // 1615
    m_autorepeatTime = msecs;
}

// orig 0x56edf0 input_di8.cpp:1621
int CInput_di8::SetActiveState(int state)
{
    // 1622
    m_isActive = state;
    // 1624
    SetEvent(eventActiveStateChanged);
    // 1630
    return 1;
}

// orig 0x56ee10 input_di8.cpp:1636
void CInput_di8::ChangeLanguage()
{
    // 1637
    AutoLockMutex lock(m_mutex1);
    // 1638
    m_language = (m_language == m3d::input::LANGUAGE_BASE) ? m3d::input::LANGUAGE_ADDITIONAL
                                                             : m3d::input::LANGUAGE_BASE;
    // 1639
}

// orig 0x56ee40 input_di8.cpp:1644
void CInput_di8::SetLanguage(m3d::input::Language language)
{
    // 1645
    AutoLockMutex lock(m_mutex1);
    // 1646
    m_language = language;
    // 1647
}

// orig 0x56ee70 input_di8.cpp:1652
m3d::input::Language CInput_di8::GetLanguage() const
{
    // 1653
    return m_language;
}

// orig 0x56ee80 input_di8.cpp:1680
void CInput_di8::ClearKeyboardLayots()
{
    // 1684
    if (m_bNeedUnloadBaseKeyboardLayot)
    {
        // 1685
        UnloadKeyboardLayout(m_hBaseLocalKeyboardLayot);
    }
    // 1687
    if (m_bNeedUnloadAdditionalKeyboardLayot)
    {
        // 1688
        UnloadKeyboardLayout(m_hAdditionalLocalKeyboardLayot);
    }

    // 1691
    m_hBaseLocalKeyboardLayot = 0;
    // 1692
    m_hAdditionalLocalKeyboardLayot = 0;

    // 1694
    m_bNeedUnloadBaseKeyboardLayot = false;
    // 1695
    m_bNeedUnloadAdditionalKeyboardLayot = false;
}

namespace
{
    // orig 0x570520 input_di8.cpp:1703
    CStr KbLayotName2Id(CStr const& name)
    {
        // 1704
        if (name == "userDefault" || name == "default")
        {
            // 1705
            return CStr("00000400");
        }
        // 1707
        if (name == "systemDefault")
        {
            // 1708
            return CStr("00000800");
        }
        // 1711
        return name;
    }
}

// orig 0x570590 input_di8.cpp:1719
int CInput_di8::InitKeyboardLayots()
{
    int res = 1;

    // 1722
    ClearKeyboardLayots();

    // 1726
    int numKbLayots = GetKeyboardLayoutList(0, 0);
    // 1727
    HKL* hKbLayotIdentifiers = new HKL[numKbLayots];
    // 1728
    memset(hKbLayotIdentifiers, 0, numKbLayots * sizeof(HKL));
    // 1729
    GetKeyboardLayoutList(numKbLayots, hKbLayotIdentifiers);

    // 1733
    CStr baseKbLayotId = KbLayotName2Id(CStr(g_kernel->GetEngineCfg().m_input_baseKeyboardLayot.GetS()));

    // 1735
    m_hBaseLocalKeyboardLayot = LoadKeyboardLayoutA(baseKbLayotId.c_str(), KLF_SUBSTITUTE_OK | KLF_REPLACELANG);
    // 1736
    if (m_hBaseLocalKeyboardLayot == 0)
    {
        // 1739: M3D_LOG_INFO in the original
        LogMsg(CStr("CInput_di8::InitKeyboardLayots error: cannot load keyboard layot ") + baseKbLayotId +
               CStr(" for base language. Usse user default"));

        // 1741
        m_hBaseLocalKeyboardLayot = LoadKeyboardLayoutA("00000400", KLF_SUBSTITUTE_OK | KLF_REPLACELANG);

        // 1743
        if (m_hBaseLocalKeyboardLayot == 0)
        {
            // 1745
            DWORD err = GetLastError();
            // 1746: M3D_LOG_ERR in the original
            LogMsg(CStr("CInput_di8::InitKeyboardLayots error: cannot load default keyboard layout, GetLastError = ") +
                   CStr((unsigned long)err));
        }

        // 1750
        INPUT_ASSERT(m_hBaseLocalKeyboardLayot, 1750);
        // 1751
        res = 0;
    }

    // 1757
    CStr additionalKbLayotId =
        KbLayotName2Id(CStr(g_kernel->GetEngineCfg().m_input_additionalKeyboardLayot.GetS()));
    // 1758
    if (!additionalKbLayotId.empty() && additionalKbLayotId != baseKbLayotId)
    {
        // 1760
        m_hAdditionalLocalKeyboardLayot =
            LoadKeyboardLayoutA(additionalKbLayotId.c_str(), KLF_SUBSTITUTE_OK | KLF_REPLACELANG);
        // 1761
        if (m_hAdditionalLocalKeyboardLayot == 0)
        {
            // 1763: M3D_LOG_INFO in the original
            LogMsg(CStr("CInput_di8::InitKeyboardLayots error: cannot load keyboard layot ") + additionalKbLayotId +
                   CStr(" for additional language."));
            // 1764
            res = 0;
        }
    }

    // 1774
    bool baseFound = false;
    bool additionalFound = false;
    for (int i = 0; i < numKbLayots; ++i)
    {
        // 1776
        if (hKbLayotIdentifiers[i] == m_hBaseLocalKeyboardLayot)
        {
            // 1777
            baseFound = true;
        }
        // 1779
        if (hKbLayotIdentifiers[i] == m_hAdditionalLocalKeyboardLayot)
        {
            // 1780
            additionalFound = true;
        }
    }

    // 1783
    m_bNeedUnloadBaseKeyboardLayot = !baseFound;
    // 1784
    m_bNeedUnloadAdditionalKeyboardLayot = !additionalFound;

    // 1786
    delete[] hKbLayotIdentifiers;

    // 1788
    return res;
}

// orig 0x56f0f0 input_di8.cpp:1794
CPINFOEXA CInput_di8::GetCodePage()
{
    // 1798
    char const* codePageName = g_kernel->GetEngineCfg().m_ui_codePageName.GetS();
    unsigned int codePageId = 0;

    // 1800
    if (codePageName && strlen(codePageName) != 0)
    {
        // 1803
        if (strcmp(codePageName, "CP_UTF8") == 0)
        {
            // 1805
            codePageId = CP_UTF8;
            // 1807
        }
        // 1810
        else if (strstr(codePageName, "windows-"))
        {
            // 1812
            codePageId = atoi(codePageName + strlen("windows-"));
        }
    }

    // 1817
    CPINFOEXA codePage;
    if (!GetCPInfoExA(codePageId, 0, &codePage))
    {
        // 1820
        GetCPInfoExA(CP_ACP, 0, &codePage);
    }

    // 1822
    return codePage;
}

namespace m3d
{
    // orig 0x570b40 input_di8.cpp:1829
    input::IInput* InputFactory()
    {
        // 1830
        return new CInput_di8;
    }
}
