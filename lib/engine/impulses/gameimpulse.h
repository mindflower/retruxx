#pragma once
#include <map>
#include <impulses/i_impulses.h>
#include <impulses/keybindstation.h>
#include <script/scriptserver.h>

class IBase;

namespace m3d
{
    class GameImpulse : public m3d::IImpulse, public m3d::Object
    {
    public:
        int m_refCount;
        IBase* m_parent;
        virtual int DecRef();
        virtual int IncRef();
        virtual void* QueryIface(const char* ifaceName);
        virtual int BindKey1(const CStr& strGameMode, const CStr& strKey1, const CStr& strImp) override;
        virtual int BindKey2(const CStr& strGameMode, const CStr& strKey1, const CStr& strKey2, const CStr& strImp) override;
        virtual int BindKey3(const CStr& strGameMode, const CStr& strKey1, const CStr& strKey2, const CStr& strKey3, const CStr& strImp) override;
        virtual int UnbindKey1(const CStr& strGameMode, const CStr& strKey1, const CStr& strImp) override;
        virtual int UnbindKey2(const CStr& strGameMode, const CStr& strKey1, const CStr& strKey2, const CStr& strImp) override;
        virtual int UnbindKey3(const CStr& strGameMode, const CStr& strKey1, const CStr& strKey2, const CStr& strKey3, const CStr& strImp) override;
        virtual void UnbindAll() override;
        virtual int Init() override;
        virtual int Done() override;
        virtual int LoadFromDefaults() override;
        virtual int LoadFromProfile() override;
        virtual int SaveToDefaults() override;
        virtual int SaveToProfile() override;
        virtual int SetImpulseState(const m3d::AuxImpulseInfo& impInfo, m3d::ui::Wnd* causeWnd) override;
        virtual bool GetImpulseState(int impId) override;
        virtual bool GetImpulseStateAndReset(int impId) override;
        virtual void ResetImpulseWithoutNotification(int impId) override;
        virtual void ResetAllImpulses(bool bClearPressedKeys) override;
        virtual void RaiseOneTimeImpulse(const m3d::AuxImpulseInfo& impInfo) override;
        virtual int HandleKeyboardMouseEvent(const m3d::Event& ev, m3d::ui::Wnd* causeWnd) override;
        virtual int GetKeyIdByName(const CStr& keyName) override;
        virtual CStr GetKeyNameById(int keyId) override;
        virtual int GetImpulseIdByName(const CStr&) override = 0;
        virtual CStr GetImpulseNameById(int) override = 0;
        virtual int GetGameModeIdByName(const CStr&) override = 0;
        virtual CStr GetGameModeNameById(int) override = 0;
        virtual std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > > GetKeysForImpulse(int impId, int modeId) override;
        virtual int GetImpulseForKeys(std::vector<int, std::allocator<int> > keys, int modeId) override;

        using BindMap = std::map<int, m3d::KeyBindStation, std::less<int>, std::allocator<std::pair<int const, m3d::KeyBindStation> > >;
        using ImpStateMap = std::map<int, bool, std::less<int>, std::allocator<std::pair<int const, bool> > >;

    protected:
        bool m_isInited;
        bool m_isBinded;
        bool m_bSuppressEvent;
        char Padding_324;
        std::map<int, m3d::KeyBindStation, std::less<int>, std::allocator<std::pair<int const, m3d::KeyBindStation> > > m_bindings;
        std::map<int, bool, std::less<int>, std::allocator<std::pair<int const, bool> > > m_impulseStates;
        std::map<int, bool, std::less<int>, std::allocator<std::pair<int const, bool> > > m_impulseResetAfterRead;
        m3d::KeysSet m_curKeys;
        CStr m_profileFileName;
        virtual int HandleImpulse(const m3d::AuxImpulseInfo&, m3d::ui::Wnd*) = 0;
        virtual CStr GetProfileFolder() = 0;
        GameImpulse(const m3d::GameImpulse&);
        GameImpulse();

    public:
        virtual  ~GameImpulse() override;
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetRtClass() const;
        static m3d::Class m_classGameImpulse;
        CStr GetFormattedScriptErrorDesc(m3d::eScriptError err) const;
        int FilterShifts(int& keyToSearchBy, bool& state);
        int SetImpulsesStateBySet(m3d::KeysSet impSet, bool state, int curGameMode, m3d::ui::Wnd* causeWnd);
        void BindKey0(int gameMode, const m3d::KeysSet& ks, int impId);
        void UnbindKey0(int gameMode, const m3d::KeysSet& ks, int impId);
        int LoadFromFile(const CStr& bindFile);
        int SaveToFile(const CStr& fileName);
        int HandleBinding(int action, const CStr& strGameMode, const CStr& strKey1, const CStr& strKey2, const CStr& strKey3, const CStr& strImp);
    };
}
