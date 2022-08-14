#pragma once
#include <core/stringm3d.h>
#include <vector>

namespace m3d
{
    namespace ui
    {
        class Wnd;
    }

    class Event;

    class AuxImpulseInfo
    {
    public:
        void UnpackXy(float*, float*, float*, float*) const;
        AuxImpulseInfo(int, bool, int, unsigned int, unsigned int);
        float UnpackWheel() const;

    public:
        int m_impId;
        bool m_state;
        int m_gameMode;
        unsigned int m_info0;
        unsigned int m_info1;
    };

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
