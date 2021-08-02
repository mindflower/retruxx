#pragma once

namespace m3d
{
    class IImpulse
    {
    public:
        virtual ~IImpulse() = default;
        virtual int BindKey1(const CStr*, const CStr*, const CStr*) = 0;
        virtual int BindKey2(const CStr*, const CStr*, const CStr*, const CStr*) = 0;
        virtual int BindKey3(const CStr*, const CStr*, const CStr*, const CStr*, const CStr*) = 0;
        virtual int UnbindKey1(const CStr*, const CStr*, const CStr*) = 0;
        virtual int UnbindKey2(const CStr*, const CStr*, const CStr*, const CStr*) = 0;
        virtual int UnbindKey3(const CStr*, const CStr*, const CStr*, const CStr*, const CStr*) = 0;
        virtual void UnbindAll() = 0;
        virtual int Init() = 0;
        virtual int Done() = 0;
        virtual int LoadFromDefaults() = 0;
        virtual int LoadFromProfile() = 0;
        virtual int SaveToDefaults() = 0;
        virtual int SaveToProfile() = 0;
        virtual int SetImpulseState(const AuxImpulseInfo*, ui::Wnd*) = 0;
        virtual bool GetImpulseState(int) = 0;
        virtual bool GetImpulseStateAndReset(int) = 0;
        virtual void ResetImpulseWithoutNotification(int) = 0;
        virtual void ResetAllImpulses(bool) = 0;
        virtual void RaiseOneTimeImpulse(const AuxImpulseInfo*) = 0;
        virtual int HandleKeyboardMouseEvent(const Event*, ui::Wnd*) = 0;
        virtual int GetKeyIdByName(const CStr*) = 0;
        virtual CStr  GetKeyNameById(int) = 0;
        virtual int GetImpulseIdByName(const CStr*) = 0;
        virtual CStr  GetImpulseNameById(int) = 0;
        virtual int GetGameModeIdByName(const CStr*) = 0;
        virtual CStr  GetGameModeNameById(int) = 0;
        virtual std::vector<std::vector<int>>  GetKeysForImpulse(int, int) = 0;
        virtual int GetImpulseForKeys(std::vector<int>, int) = 0;
    };

    class AuxImpulseInfo
    {
    public:
        void UnpackXy(float*, float*, float*, float*) const;
        AuxImpulseInfo(int, bool, int, unsigned int, unsigned int);
        float UnpackWheel() const;

    private:
        int m_impId;
        bool m_state;
        int m_gameMode;
        unsigned int m_info0;
        unsigned int m_info1;
    };
}
