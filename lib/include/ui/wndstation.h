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
            friend class Wnd;
        protected:
            WndStation(const m3d::ui::WndStation&);
            WndStation();

        public:
            virtual  ~WndStation() override /* 0x00 */;
            static m3d::Class* GetBaseClass();
            virtual m3d::Class* GetRtClass() const /* 0x11c */;
            static m3d::Class m_classWndStation;
            /* 0x0220 */ m3d::ui::Wnd* m_wndMouseOver = nullptr;
            /* 0x0224 */ m3d::ui::Wnd* m_wndMouseCapture = nullptr;
            /* 0x0228 */ m3d::ui::Wnd* m_wndKbdCapture = nullptr;
            /* 0x022c */ m3d::ui::Wnd* m_wndActive = nullptr;
            /* 0x0230 */ m3d::ui::Wnd* m_wndForTooltip = nullptr;
            /* 0x0234 */ m3d::ui::Wnd* m_wndCandidateForDblClick = nullptr;
            /* 0x0238 */ m3d::ui::ComboBoxWnd* m_wndOpenedComboBox = nullptr;
            /* 0x023c */ retruxx::vector<m3d::ui::ModalWnd*> m_wndModalStack;
            /* 0x024c */ unsigned int m_wndModalRetVal;
            /* 0x0250 */ m3d::ui::Cursor* m_curDefault = nullptr;
            /* 0x0254 */ m3d::ui::Cursor m_currentCursor;
            /* 0x0268 */ bool m_bAnimationEnabled = false;
            int DispatchMouse(const m3d::Event& event);
            m3d::ui::Wnd* GetWndMouseOver();
            int DispatchPaint(m3d::ui::Wnd* curWnd, const BoundsBase<float>& clipTo);

        private:
            int DispatchKey(const m3d::Event& event);
            int DispatchJoystick(const m3d::Event& event);
            void UpdateOnMouseInOut(m3d::ui::Wnd* newWnd);
            m3d::ui::Wnd* ModalOverride(m3d::ui::Wnd* w);
            void RemoveCurrentTooltip();
            static m3d::ui::GfxServer* __fastcall getGfxServer();
            void OnOpenComboBox(m3d::ui::ComboBoxWnd* combo);
            void OnCloseComboBox(m3d::ui::ComboBoxWnd* combo);

        public:
            m3d::ui::Wnd* GetWndForMousePoint(m3d::ui::Wnd* curWnd, const PointBase<float>& pt, bool affectAll);
            int CheckForMouseDblClick(m3d::ui::Wnd* w, const PointBase<float>& pt, unsigned int mouseState, PointBase<float>& prevClickPt);
            int CheckForMouseClick(m3d::ui::Wnd* w, bool set, const PointBase<float>* pt);
            /* 0x0269 */ char Padding_77[3];

        protected:
            /* 0x026c */ PointBase<float> m_prevMouseCoord;

        private:
            void ForEachChild(m3d::ui::Wnd* curWnd, void (m3d::ui::Wnd::* fn)());
            /* 0x0274 */ m3d::CStrHash<CStr> m_strings;
            void CreateDefaultStrings();

        public:
            int LoadStrings(const CStr& stringsName);
            int GetStringByStringId(CStr& dest, const CStr& id);
            CStr GetStringByStringId0(const CStr& id);
            CStr InitializeStringUsingIds(const CStr& src);
            int ProcessEvent(const m3d::Event& event);
            virtual int OnEvent(const m3d::Event&) = 0 /* 0x120 */;
            int OnRemoveWnd(m3d::ui::Wnd* parent, m3d::ui::Wnd* wnd);
            int OnAddWnd(m3d::ui::Wnd* parent, m3d::ui::Wnd* wnd);
            int AddNotifyForWnd(m3d::ui::Wnd* from, m3d::ui::Wnd* to, unsigned int msg, const m3d::AIParam& data, bool urgent);
            int Activate(m3d::ui::Wnd* wnd);
            m3d::ui::Wnd* GetActive() const;

            // TODO: check if this virtual or not
            /* virtual */ int Create(const CStr& stringsName, const CStr& schemaName) /* 0x00 */;

            virtual int Create(const CStr& caption, unsigned int style, const BoundsBase<float>& rc, unsigned int id) override /* 0x00 */;
            int Done();
            int Repaint();
            virtual m3d::ui::Wnd* CaptureMouse(m3d::ui::Wnd* wnd) /* 0x124 */;
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
            virtual int GetDefaultCursor(m3d::ui::Cursor& cur) /* 0x128 */;
            const m3d::ui::Cursor& GetCurrentCursor() const;
            bool IsAnimationEnabled() const;
            void EnableAnimation(bool bEnable);
            void StopAllAnimations();
            void OnEndAnimation(m3d::ui::Wnd* wndThatWasAnimated);
            void OnBearWindow(m3d::ui::Wnd*);

        private:
            /* 0x0280 */ m3d::CIntHash<int> m_allWindows;
            /* 0x02a8 */ m3d::CIntHash<m3d::ui::Wnd*> m_allWindowsById;
            void RegisterWnd(m3d::ui::Wnd* w);
            void UnregisterWnd(m3d::ui::Wnd* w);
            /* 0x02d0 */ int m_nextUniqueId = 0;

        public:
            bool IsWndAlive(const m3d::ui::Wnd* w, int uniqueId) const;
            m3d::ui::Wnd* GetWndByUniqueId(int uniqueId) const;
        }; /* size: 0x02d4 */

        static_assert(sizeof(WndStation) == 0x02d4);
    }
}
