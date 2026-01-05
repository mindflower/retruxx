#pragma once
#include <core/stringm3d.h>
#include <vector>

namespace m3d
{
    namespace ui
    {
        class Wnd;
    }

    struct Event;

    struct AuxImpulseInfo
    {
        AuxImpulseInfo(int impId, bool state, int gameMode, unsigned int i0, unsigned int i1);
        void UnpackXy(float* x, float* y, float* dx, float* dy) const;
        float UnpackWheel() const;
        /* 0x0000 */ int m_impId;
        /* 0x0004 */ bool m_state;
        /* 0x0005 */ char Padding_319[3];
        /* 0x0008 */ int m_gameMode;
        /* 0x000c */ unsigned int m_info0;
        /* 0x0010 */ unsigned int m_info1;
    }; /* size: 0x0014 */

    class IImpulse
    {
    public:
        virtual ~IImpulse() = default;
        virtual int BindKey1(CStr const&, CStr const&, CStr const&) = 0;
        virtual int BindKey2(CStr const&, CStr const&, CStr const&, CStr const&) = 0;
        virtual int BindKey3(CStr const&, CStr const&, CStr const&, CStr const&, CStr const&) = 0;
        virtual int UnbindKey1(CStr const&, CStr const&, CStr const&) = 0;
        virtual int UnbindKey2(CStr const&, CStr const&, CStr const&, CStr const&) = 0;
        virtual int UnbindKey3(CStr const&, CStr const&, CStr const&, CStr const&, CStr const&) = 0;
        virtual void UnbindAll() = 0;
        virtual int Init() = 0;
        virtual int Done() = 0;
        virtual int LoadFromDefaults() = 0;
        virtual int LoadFromProfile() = 0;
        virtual int SaveToDefaults() = 0;
        virtual int SaveToProfile() = 0;
        virtual int SetImpulseState(AuxImpulseInfo const&, ui::Wnd*) = 0;
        virtual bool GetImpulseState(int) = 0;
        virtual bool GetImpulseStateAndReset(int) = 0;
        virtual void ResetImpulseWithoutNotification(int) = 0;
        virtual void ResetAllImpulses(bool) = 0;
        virtual void RaiseOneTimeImpulse(AuxImpulseInfo const&) = 0;
        virtual int HandleKeyboardMouseEvent(Event const&, ui::Wnd*) = 0;
        virtual int GetKeyIdByName(CStr const&) = 0;
        virtual CStr GetKeyNameById(int) = 0;
        virtual int GetImpulseIdByName(CStr const&) = 0;
        virtual CStr GetImpulseNameById(int) = 0;
        virtual int GetGameModeIdByName(CStr const&) = 0;
        virtual CStr GetGameModeNameById(int) = 0;
        virtual std::vector<std::vector<int>> GetKeysForImpulse(int, int) = 0;
        virtual int GetImpulseForKeys(std::vector<int>, int) = 0;
    };
}
