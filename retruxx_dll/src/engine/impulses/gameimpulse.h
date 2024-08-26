#pragma once
#include <map>
#include <impulses/i_impulses.h>
#include <impulses/keybindstation.h>
#include <script/scriptserver.h>

class IBase;

namespace m3d
{
    class GameImpulse : public IImpulse, public Object
    {
    public:
        virtual ~GameImpulse();
        virtual int BindKey1(CStr const&, CStr const&, CStr const&);
        virtual int BindKey2(CStr const&, CStr const&, CStr const&, CStr const&);
        virtual int BindKey3(CStr const&, CStr const&, CStr const&, CStr const&, CStr const&);
        virtual int UnbindKey1(CStr const&, CStr const&, CStr const&);
        virtual int UnbindKey2(CStr const&, CStr const&, CStr const&, CStr const&);
        virtual int UnbindKey3(CStr const&, CStr const&, CStr const&, CStr const&, CStr const&);
        virtual void UnbindAll();
        virtual int Init();
        virtual int Done();
        virtual int LoadFromDefaults();
        virtual int LoadFromProfile();
        virtual int SaveToDefaults();
        virtual int SaveToProfile();
        virtual int SetImpulseState(AuxImpulseInfo const&, ui::Wnd*);
        virtual bool GetImpulseState(int);
        virtual bool GetImpulseStateAndReset(int);
        virtual void ResetImpulseWithoutNotification(int);
        virtual void ResetAllImpulses(bool);
        virtual void RaiseOneTimeImpulse(AuxImpulseInfo const&);
        virtual int HandleKeyboardMouseEvent(Event const&, ui::Wnd*);
        virtual int GetKeyIdByName(CStr const&);
        virtual CStr GetKeyNameById(int);
        virtual std::vector<std::vector<int>> GetKeysForImpulse(int, int);
        virtual int GetImpulseForKeys(std::vector<int>, int);


        int LoadFromFile(CStr const&);
        virtual int DecRef();
        virtual void* QueryIface(char const*);
        int SetImpulsesStateBySet(KeysSet, bool, int, ui::Wnd*);
        void BindKey0(int, KeysSet const&, int);
        virtual int IncRef();
        int FilterShifts(int&, bool&);
        int HandleBinding(int, CStr const&, CStr const&, CStr const&, CStr const&, CStr const&);
        CStr GetFormattedScriptErrorDesc(eScriptError) const;
        static Class* GetBaseClass();
        virtual Class* GetRtClass() const;
        void UnbindKey0(int, KeysSet const&, int);
        int SaveToFile(CStr const&);

    protected:
        GameImpulse();

    public:
        RT_CLASS_DECLARE(GameImpulse);

    private:
        int m_refCount;
        IBase* m_parent = nullptr;
        bool m_isInited = false;
        bool m_isBinded = false;
        bool m_bSuppressEvent = false;
        std::map<int, m3d::KeyBindStation> m_bindings;
        std::map<int, bool> m_impulseStates;
        std::map<int, bool> m_impulseResetAfterRead;
        m3d::KeysSet m_curKeys;
        CStr m_profileFileName = "KeyBindings.lua";
    };
}
