#pragma once
#include "cursor.h"
#include "ui.h"
#include <core/containers.h>
#include <math/bounds2d.h>

namespace m3d
{
    namespace rend
    {
        class IRenderer;
    }

    class AIParam;
    class Event;
}

namespace m3d
{
    namespace ui
    {
        class GfxServer;
        class ModalWnd;
        class ComboBoxWnd;

        class WndStation : public Wnd
        {
        public:
            static inline Class m_classWndStation;

        public:
            int DispatchMouse(Event const&);
            bool IsWndAlive(Wnd const*, int) const;
            static Class* __fastcall GetBaseClass();
            Wnd* CaptureFocus(Wnd*);
            int Done();
            ModalWnd* GetTopModal();
            int DispatchPaint(Wnd*, BoundsBase<float> const&);
            int CheckForMouseClick(Wnd*, bool, PointBase<float> const*);
            Wnd* GetWndForMousePoint(Wnd*, PointBase<float> const&, bool);
            int DoModal(ModalWnd*);
            int PulseKeyForWindow(Wnd*, unsigned short, unsigned char);
            Cursor const& GetCurrentCursor() const;
            Wnd* GetCapture() const;
            Wnd* GetWndMouseOver();
            void EnableAnimation(bool);
            int Activate(Wnd*);
            int OnAddWnd(Wnd*, Wnd*);
            virtual Wnd* CaptureMouse(Wnd*);
            CStr GetStringByStringId0(CStr const&);
            int GetStringByStringId(CStr&, CStr const&);
            bool HasChildModalRunning();
            int AddNotifyForWnd(Wnd*, Wnd*, unsigned int, AIParam const&, bool);
            int CheckForMouseDblClick(Wnd*, PointBase<float> const&, unsigned int, PointBase<float>&);
            virtual int GetDefaultCursor(Cursor&);
            int LoadStrings(CStr const&);
            void CloseAllModalWithCancelRet();
            virtual int Create(CStr const&, unsigned int, BoundsBase<float> const&, unsigned int);
            int Create(CStr const&, CStr const&);
            virtual Class* GetRtClass() const;
            void StopAllAnimations();
            Wnd* GetFocus() const;
            int ProcessEvent(Event const&);
            Wnd* GetWndByUniqueId(int) const;
            void EndModal(ModalWnd*, unsigned int);
            Wnd* GetActive() const;
            int Repaint();
            bool IsModal(ModalWnd*);
            void OnEndAnimation(Wnd*);
            int FlushGfx(rend::IRenderer*);
            bool IsAnimationEnabled() const;
            CStr InitializeStringUsingIds(CStr const&);
            virtual ~WndStation();
            int OnRemoveWnd(Wnd*, Wnd*);

        protected:
            WndStation();

        private:
            int DispatchJoystick(Event const&);
            void CreateDefaultStrings();
            void OnCloseComboBox(ComboBoxWnd*);
            void OnOpenComboBox(ComboBoxWnd*);
            void ForEachChild(Wnd*, void (Wnd::*)());
            Wnd* ModalOverride(Wnd* w);
            void RegisterWnd(Wnd*);
            static GfxServer* __fastcall getGfxServer();
            void RemoveCurrentTooltip();
            void UpdateOnMouseInOut(Wnd* newWnd);
            void UnregisterWnd(Wnd*);
            int DispatchKey(Event const&);

        protected:
            PointBase<float> m_prevMouseCoord;

        private:
            Wnd* m_wndMouseOver;
            Wnd* m_wndMouseCapture;
            Wnd* m_wndKbdCapture;
            Wnd* m_wndActive;
            Wnd* m_wndForTooltip;
            Wnd* m_wndCandidateForDblClick;
            ComboBoxWnd* m_wndOpenedComboBox;
            std::vector<ModalWnd*> m_wndModalStack;
            unsigned int m_wndModalRetVal;
            Cursor* m_curDefault;
            Cursor m_currentCursor;
            bool m_bAnimationEnabled;
            CStrHash<CStr> m_strings;
            CIntHash<int> m_allWindows;
            CIntHash<Wnd*> m_allWindowsById;
            int m_nextUniqueId;
        };
    }
}
