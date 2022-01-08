#pragma once
#include "ui.h"

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;

        class ScrollWnd : public Wnd
        {
        public:
            RT_CLASS_DECLARE(ScrollWnd);

        public:
            static Object* CreateObject();
            static Class* GetBaseClass();

        public:
            virtual Object* Clone();
            float GetCurPos() const;
            void SetCurPos(float);
            virtual void EnableWindow(bool);
            int Create(BoundsBase<float> const&, int);
            virtual int Create(CStr const&, unsigned int, BoundsBase<float> const&, unsigned int);
            virtual ~ScrollWnd();
            float GetMaxPos() const;
            virtual void SetScrollPane(CStr const&);
            void SetScrollRect(float, float);
            virtual Class* GetClass() const;
            virtual void SetBounds(BoundsBase<float> const&, bool);
            virtual void ShowWindow(bool);

        protected:
            virtual int OnPaint(DrawInfo const&);
            void RecalcLayot();
            virtual int OnMouseMove(PointBase<float> const&, PointBase<float> const&);
            ScrollWnd();
            ScrollWnd(ScrollWnd const&);
            virtual int OnWndNotify(Wnd*, unsigned int, unsigned int, AIParam const&);
            BoundsBase<float> GetThumbRect() const;
            BoundsBase<float> GetBodyRect() const;
            virtual int OnMouseButton0(unsigned int, PointBase<float> const&);

        private:
            int m_vertical;
            bool m_tracking;
            float m_maxPos;
            float m_curPos;
            float m_thumbSz;
            float m_hitPosInThumb;
            ButtonWnd* m_btn0;
            ButtonWnd* m_btn1;
        };
    }
}
