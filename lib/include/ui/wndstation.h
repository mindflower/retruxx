#pragma once
#include "cursor.h"
#include "ui.h"
#include <core/containers.h>
#include <math/bounds2d.h>

namespace ai
{
    struct Event;
}

namespace m3d
{
    namespace rend
    {
        class IRenderer;
    }

    class AIParam;
    struct Event;
}  // namespace m3d

namespace m3d
{
    namespace ui
    {
        class GfxServer;
        class ModalWnd;
        class ComboBoxWnd;

        class WndStation : public Wnd
        {
            friend class Wnd;

        protected:
            WndStation(m3d::ui::WndStation const&);
            WndStation();

        public:
            virtual ~WndStation() override;
            static m3d::Class* GetBaseClass();
            virtual m3d::Class* GetRtClass() const;
            static m3d::Class m_classWndStation;
            m3d::ui::Wnd* m_wndMouseOver = nullptr;
            m3d::ui::Wnd* m_wndMouseCapture = nullptr;
            m3d::ui::Wnd* m_wndKbdCapture = nullptr;
            m3d::ui::Wnd* m_wndActive = nullptr;
            m3d::ui::Wnd* m_wndForTooltip = nullptr;
            m3d::ui::Wnd* m_wndCandidateForDblClick = nullptr;
            m3d::ui::ComboBoxWnd* m_wndOpenedComboBox = nullptr;
            retruxx::vector<m3d::ui::ModalWnd*> m_wndModalStack;
            unsigned int m_wndModalRetVal;
            m3d::ui::Cursor* m_curDefault = nullptr;
            m3d::ui::Cursor m_currentCursor;
            bool m_bAnimationEnabled = false;
            int DispatchMouse(m3d::Event const& event);
            m3d::ui::Wnd* GetWndMouseOver();
            int DispatchPaint(m3d::ui::Wnd* curWnd, BoundsBase<float> const& clipTo);

        private:
            int DispatchKey(m3d::Event const& event);
            int DispatchJoystick(m3d::Event const& event);
            void UpdateOnMouseInOut(m3d::ui::Wnd* newWnd);
            m3d::ui::Wnd* ModalOverride(m3d::ui::Wnd* w);
            void RemoveCurrentTooltip();
            static m3d::ui::GfxServer* __fastcall getGfxServer();
            void OnOpenComboBox(m3d::ui::ComboBoxWnd* combo);
            void OnCloseComboBox(m3d::ui::ComboBoxWnd* combo);

        public:
            m3d::ui::Wnd* GetWndForMousePoint(m3d::ui::Wnd* curWnd, PointBase<float> const& pt, bool affectAll);
            int CheckForMouseDblClick(
                m3d::ui::Wnd* w,
                PointBase<float> const& pt,
                unsigned int mouseState,
                PointBase<float>& prevClickPt);
            int CheckForMouseClick(m3d::ui::Wnd* w, bool set, PointBase<float> const* pt);

        protected:
            PointBase<float> m_prevMouseCoord;

        private:
            void ForEachChild(m3d::ui::Wnd* curWnd, void (m3d::ui::Wnd::*fn)());
            m3d::CStrHash<CStr> m_strings;
            void CreateDefaultStrings();

        public:
            int LoadStrings(CStr const& stringsName);
            int GetStringByStringId(CStr& dest, CStr const& id);
            CStr GetStringByStringId0(CStr const& id);
            CStr InitializeStringUsingIds(CStr const& src);
            int ProcessEvent(m3d::Event const& event);
            virtual int OnEvent(m3d::Event const&) = 0;
            int OnRemoveWnd(m3d::ui::Wnd* parent, m3d::ui::Wnd* wnd);
            int OnAddWnd(m3d::ui::Wnd* parent, m3d::ui::Wnd* wnd);
            int AddNotifyForWnd(
                m3d::ui::Wnd* from,
                m3d::ui::Wnd* to,
                unsigned int msg,
                m3d::AIParam const& data,
                bool urgent);
            int Activate(m3d::ui::Wnd* wnd);
            m3d::ui::Wnd* GetActive() const;

            // Not virtual, although the PDB class dump lists it as an override: the mangled
            // name (?Create@WndStation@ui@m3d@@QAEHABVCStr@@0@Z, RVA 0x58DC50) is a plain
            // public member.
            int Create(CStr const& stringsName, CStr const& schemaName);

            virtual int Create(CStr const& caption, unsigned int style, BoundsBase<float> const& rc, unsigned int id)
                override;
            int Done();
            int Repaint();
            virtual m3d::ui::Wnd* CaptureMouse(m3d::ui::Wnd* wnd);
            m3d::ui::Wnd* GetCapture() const;
            m3d::ui::Wnd* CaptureFocus(m3d::ui::Wnd* wnd);
            m3d::ui::Wnd* GetFocus() const;
            int FlushGfx(m3d::rend::IRenderer* rend);
            bool IsModal(m3d::ui::ModalWnd* wnd);
            bool HasChildModalRunning();
            m3d::ui::ModalWnd* GetTopModal();
            void CloseAllModalWithCancelRet();
            int DoModal(m3d::ui::ModalWnd* wnd);
            void EndModal(m3d::ui::ModalWnd* wnd, unsigned int toRet);
            int PulseKeyForWindow(m3d::ui::Wnd* w, unsigned short key, unsigned char scanCode);
            virtual int GetDefaultCursor(m3d::ui::Cursor& cur);
            m3d::ui::Cursor const& GetCurrentCursor() const;
            bool IsAnimationEnabled() const;
            void EnableAnimation(bool bEnable);
            void StopAllAnimations();
            void OnEndAnimation(m3d::ui::Wnd* wndThatWasAnimated);
            void OnBearWindow(m3d::ui::Wnd*);

        private:
            // Keyed by the window's address; a machine word so that it is not truncated on x64.
            m3d::CIntHash<int, std::uintptr_t> m_allWindows;
            m3d::CIntHash<m3d::ui::Wnd*> m_allWindowsById;
            void RegisterWnd(m3d::ui::Wnd* w);
            void UnregisterWnd(m3d::ui::Wnd* w);
            int m_nextUniqueId = 0;

        public:
            bool IsWndAlive(m3d::ui::Wnd const* w, int uniqueId) const;
            m3d::ui::Wnd* GetWndByUniqueId(int uniqueId) const;
        };
    }  // namespace ui
}  // namespace m3d
