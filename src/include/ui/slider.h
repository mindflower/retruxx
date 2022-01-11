#pragma once
#include "ui.h"

namespace m3d
{
    namespace ui
    {
        class SliderWnd : public Wnd
        {
        public:
            static Object* CreateObject();
            static Class* GetBaseClass();

        public:
            int GetMax() const;
            virtual Object* Clone();
            virtual int Create(CStr const&, unsigned int, BoundsBase<float> const&, unsigned int);
            virtual int Create(float, unsigned int);
            virtual ~SliderWnd();
            int GetMin() const;
            virtual Class* GetClass() const;
            virtual void SetMinMax(int, int);
            int GetNotch() const;
            virtual void SetNotch(int);

        protected:
            virtual int OnPaint(DrawInfo const&);
            virtual int OnMouseMove(PointBase<float> const&, PointBase<float> const&);
            BoundsBase<float> GetBodyRect() const;
            virtual int OnMouseButton0(unsigned int, PointBase<float> const&);
            SliderWnd();
            SliderWnd(SliderWnd const&);

        public:
            RT_CLASS_INLINE_DECLARE(SliderWnd);

        private:
            int m_min;
            int m_max;
            int m_cur;
            bool m_tracking;
            float m_notchWidth;
        };
    }
}
