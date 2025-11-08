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
        /* 0x0038 */ int m_refCount;
        /* 0x003c */ IBase* m_parent;
        virtual int DecRef() /* 0x74 */;
        virtual int IncRef() /* 0x78 */;
        virtual void* QueryIface(const char* ifaceName) /* 0x7c */;
        virtual int BindKey1(const CStr& strGameMode, const CStr& strKey1, const CStr& strImp) override /* 0x04 */;
        virtual int BindKey2(const CStr& strGameMode, const CStr& strKey1, const CStr& strKey2, const CStr& strImp) override /* 0x08 */;
        virtual int BindKey3(const CStr& strGameMode, const CStr& strKey1, const CStr& strKey2, const CStr& strKey3, const CStr& strImp) override /* 0x0c */;
        virtual int UnbindKey1(const CStr& strGameMode, const CStr& strKey1, const CStr& strImp) override /* 0x10 */;
        virtual int UnbindKey2(const CStr& strGameMode, const CStr& strKey1, const CStr& strKey2, const CStr& strImp) override /* 0x14 */;
        virtual int UnbindKey3(const CStr& strGameMode, const CStr& strKey1, const CStr& strKey2, const CStr& strKey3, const CStr& strImp) override /* 0x18 */;
        virtual void UnbindAll() override /* 0x1c */;
        virtual int Init() override /* 0x20 */;
        virtual int Done() override /* 0x24 */;
        virtual int LoadFromDefaults() override /* 0x28 */;
        virtual int LoadFromProfile() override /* 0x2c */;
        virtual int SaveToDefaults() override /* 0x30 */;
        virtual int SaveToProfile() override /* 0x34 */;
        virtual int SetImpulseState(const m3d::AuxImpulseInfo& impInfo, m3d::ui::Wnd* causeWnd) override /* 0x38 */;
        virtual bool GetImpulseState(int impId) override /* 0x3c */;
        virtual bool GetImpulseStateAndReset(int impId) override /* 0x40 */;
        virtual void ResetImpulseWithoutNotification(int impId) override /* 0x44 */;
        virtual void ResetAllImpulses(bool bClearPressedKeys) override /* 0x48 */;
        virtual void RaiseOneTimeImpulse(const m3d::AuxImpulseInfo& impInfo) override /* 0x4c */;
        virtual int HandleKeyboardMouseEvent(const m3d::Event& ev, m3d::ui::Wnd* causeWnd) override /* 0x50 */;
        virtual int GetKeyIdByName(const CStr& keyName) override /* 0x54 */;
        virtual CStr GetKeyNameById(int keyId) override /* 0x58 */;
        virtual int GetImpulseIdByName(const CStr&) override = 0 /* 0x5c */;
        virtual CStr GetImpulseNameById(int) override = 0 /* 0x60 */;
        virtual int GetGameModeIdByName(const CStr&) override = 0 /* 0x64 */;
        virtual CStr GetGameModeNameById(int) override = 0 /* 0x68 */;
        virtual std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > > GetKeysForImpulse(int impId, int modeId) override /* 0x6c */;
        virtual int GetImpulseForKeys(std::vector<int, std::allocator<int> > keys, int modeId) override /* 0x70 */;

        using BindMap = std::map<int, m3d::KeyBindStation, std::less<int>, std::allocator<std::pair<int const, m3d::KeyBindStation> > >;
        using ImpStateMap = std::map<int, bool, std::less<int>, std::allocator<std::pair<int const, bool> > >;

    protected:
        /* 0x0040 */ bool m_isInited;
        /* 0x0041 */ bool m_isBinded;
        /* 0x0042 */ bool m_bSuppressEvent;
        /* 0x0043 */ char Padding_324;
        std::map<int, m3d::KeyBindStation, std::less<int>, std::allocator<std::pair<int const, m3d::KeyBindStation> > > m_bindings;
        std::map<int, bool, std::less<int>, std::allocator<std::pair<int const, bool> > > m_impulseStates;
        std::map<int, bool, std::less<int>, std::allocator<std::pair<int const, bool> > > m_impulseResetAfterRead;
        m3d::KeysSet m_curKeys;
        CStr m_profileFileName;
        virtual int HandleImpulse(const m3d::AuxImpulseInfo&, m3d::ui::Wnd*) = 0 /* 0x80 */;
        virtual CStr GetProfileFolder() = 0 /* 0x84 */;
        GameImpulse(const m3d::GameImpulse&);
        GameImpulse();

    public:
        virtual  ~GameImpulse() override /* 0x00 */;
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetRtClass() const /* 0x88 */;
        static m3d::Class m_classGameImpulse;
        CStr GetFormattedScriptErrorDesc(m3d::eScriptError err) const;
        int FilterShifts(int& keyToSearchBy, bool& state);
        int SetImpulsesStateBySet(m3d::KeysSet impSet, bool state, int curGameMode, m3d::ui::Wnd* causeWnd);
        void BindKey0(int gameMode, const m3d::KeysSet& ks, int impId);
        void UnbindKey0(int gameMode, const m3d::KeysSet& ks, int impId);
        int LoadFromFile(const CStr& bindFile);
        int SaveToFile(const CStr& fileName);
        int HandleBinding(int action, const CStr& strGameMode, const CStr& strKey1, const CStr& strKey2, const CStr& strKey3, const CStr& strImp);
    }; /* size: 0x0080 */
}
