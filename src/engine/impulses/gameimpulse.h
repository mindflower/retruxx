#pragma once
#include <map>
#include <impulses/i_impulses.h>
#include <impulses/keybindstation.h>
#include <script/scriptserver.h>

class IBase;

namespace m3d
{
    class Event;

    namespace ui
    {
        class Wnd;
    }

    class GameImpulse : public IImpulse, public Object
    {
    public:
        int LoadFromFile(CStr const&);
        virtual int DecRef();
        virtual void ResetImpulseWithoutNotification(int);
        virtual bool GetImpulseStateAndReset(int);
        virtual ~GameImpulse();
        virtual void* QueryIface(char const*);
        virtual int Init();
        virtual int SaveToDefaults();
        virtual int Done();
        virtual void ResetAllImpulses(bool);
        int SetImpulsesStateBySet(KeysSet, bool, int, ui::Wnd*);
        virtual int SetImpulseState(AuxImpulseInfo const&, ui::Wnd*);
        virtual void RaiseOneTimeImpulse(AuxImpulseInfo const&);
        virtual bool GetImpulseState(int);
        virtual int GetImpulseForKeys(std::vector<int, std::allocator<int> >, int);
        void BindKey0(int, KeysSet const&, int);
        virtual int BindKey1(CStr const&, CStr const&, CStr const&);
        virtual int IncRef();
        virtual std::vector<std::vector<int>> GetKeysForImpulse(int, int);
        virtual int HandleKeyboardMouseEvent(Event const&, ui::Wnd*);
        int FilterShifts(int&, bool&);
        virtual int BindKey2(CStr const&, CStr const&, CStr const&, CStr const&);
        virtual int SaveToProfile();
        int HandleBinding(int, CStr const&, CStr const&, CStr const&, CStr const&, CStr const&);
        virtual int BindKey3(CStr const&, CStr const&, CStr const&, CStr const&, CStr const&);
        CStr GetFormattedScriptErrorDesc(eScriptError) const;
        virtual int UnbindKey3(CStr const&, CStr const&, CStr const&, CStr const&, CStr const&);
        static Class* GetBaseClass();
        virtual int LoadFromDefaults();
        virtual int UnbindKey2(CStr const&, CStr const&, CStr const&, CStr const&);
        virtual void UnbindAll();
        virtual int GetKeyIdByName(CStr const&);
        virtual int UnbindKey1(CStr const&, CStr const&, CStr const&);
        virtual CStr GetKeyNameById(int);
        virtual Class* GetRtClass() const;
        virtual int LoadFromProfile();
        void UnbindKey0(int, KeysSet const&, int);
        int SaveToFile(CStr const&);

    protected:
        GameImpulse();


    public:
        RT_CLASS_DECLARE(GameImpulse);

    private:
        int m_refCount;
        IBase* m_parent;
        bool m_isInited;
        bool m_isBinded;
        bool m_bSuppressEvent;
        std::map<int, m3d::KeyBindStation> m_bindings;
        std::map<int, bool> m_impulseStates;
        std::map<int, bool> m_impulseResetAfterRead;
        m3d::KeysSet m_curKeys;
        CStr m_profileFileName;
    };
}
