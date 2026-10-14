#pragma once
#include <map>
#include <impulses/i_impulses.h>
#include <impulses/keybindstation.h>
#include <script/scriptserver.h>

struct IBase;

namespace m3d
{
    class GameImpulse : public m3d::IImpulse, public m3d::Object
    {
    public:
        int m_refCount;
        IBase* m_parent;
        virtual int DecRef();
        virtual int IncRef();
        virtual void* QueryIface(char const* ifaceName);
        virtual int BindKey1(CStr const& strGameMode, CStr const& strKey1, CStr const& strImp) override;
        virtual int BindKey2(CStr const& strGameMode, CStr const& strKey1, CStr const& strKey2, CStr const& strImp)
            override;
        virtual int BindKey3(
            CStr const& strGameMode,
            CStr const& strKey1,
            CStr const& strKey2,
            CStr const& strKey3,
            CStr const& strImp) override;
        virtual int UnbindKey1(CStr const& strGameMode, CStr const& strKey1, CStr const& strImp) override;
        virtual int UnbindKey2(CStr const& strGameMode, CStr const& strKey1, CStr const& strKey2, CStr const& strImp)
            override;
        virtual int UnbindKey3(
            CStr const& strGameMode,
            CStr const& strKey1,
            CStr const& strKey2,
            CStr const& strKey3,
            CStr const& strImp) override;
        virtual void UnbindAll() override;
        virtual int Init() override;
        virtual int Done() override;
        virtual int LoadFromDefaults() override;
        virtual int LoadFromProfile() override;
        virtual int SaveToDefaults() override;
        virtual int SaveToProfile() override;
        virtual int SetImpulseState(m3d::AuxImpulseInfo const& impInfo, m3d::ui::Wnd* causeWnd) override;
        virtual bool GetImpulseState(int impId) override;
        virtual bool GetImpulseStateAndReset(int impId) override;
        virtual void ResetImpulseWithoutNotification(int impId) override;
        virtual void ResetAllImpulses(bool bClearPressedKeys) override;
        virtual void RaiseOneTimeImpulse(m3d::AuxImpulseInfo const& impInfo) override;
        virtual int HandleKeyboardMouseEvent(m3d::Event const& ev, m3d::ui::Wnd* causeWnd) override;
        virtual int GetKeyIdByName(CStr const& keyName) override;
        virtual CStr GetKeyNameById(int keyId) override;
        virtual int GetImpulseIdByName(CStr const&) override = 0;
        virtual CStr GetImpulseNameById(int) override = 0;
        virtual int GetGameModeIdByName(CStr const&) override = 0;
        virtual CStr GetGameModeNameById(int) override = 0;
        virtual std::
            vector<std::vector<int, std::allocator<int>>, std::allocator<std::vector<int, std::allocator<int>>>>
            GetKeysForImpulse(int impId, int modeId) override;
        virtual int GetImpulseForKeys(std::vector<int, std::allocator<int>> keys, int modeId) override;

        using BindMap = std::
            map<int, m3d::KeyBindStation, std::less<int>, std::allocator<std::pair<int const, m3d::KeyBindStation>>>;
        using ImpStateMap = std::map<int, bool, std::less<int>, std::allocator<std::pair<int const, bool>>>;

    protected:
        bool m_isInited;
        bool m_isBinded;
        bool m_bSuppressEvent;
        std::map<int, m3d::KeyBindStation, std::less<int>, std::allocator<std::pair<int const, m3d::KeyBindStation>>>
            m_bindings;
        std::map<int, bool, std::less<int>, std::allocator<std::pair<int const, bool>>> m_impulseStates;
        std::map<int, bool, std::less<int>, std::allocator<std::pair<int const, bool>>> m_impulseResetAfterRead;
        m3d::KeysSet m_curKeys;
        CStr m_profileFileName;
        virtual int HandleImpulse(m3d::AuxImpulseInfo const&, m3d::ui::Wnd*) = 0;
        virtual CStr GetProfileFolder() = 0;
        GameImpulse(m3d::GameImpulse const&);
        GameImpulse();

    public:
        virtual ~GameImpulse() override;
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetRtClass() const;
        static m3d::Class m_classGameImpulse;
        CStr GetFormattedScriptErrorDesc(m3d::eScriptError err) const;
        int FilterShifts(int& keyToSearchBy, bool& state);
        int SetImpulsesStateBySet(m3d::KeysSet impSet, bool state, int curGameMode, m3d::ui::Wnd* causeWnd);
        void BindKey0(int gameMode, m3d::KeysSet const& ks, int impId);
        void UnbindKey0(int gameMode, m3d::KeysSet const& ks, int impId);
        int LoadFromFile(CStr const& bindFile);
        int SaveToFile(CStr const& fileName);
        int HandleBinding(
            int action,
            CStr const& strGameMode,
            CStr const& strKey1,
            CStr const& strKey2,
            CStr const& strKey3,
            CStr const& strImp);
    };
}  // namespace m3d
