#pragma once
#include <math/point2d.h>
#include <renderer/i_renderer.h>

namespace m3d
{
    namespace ui
    {
        class Cursor
        {
        public:
            int Create(CStr const&, PointBase<float> const&, PointBase<float> const&);
            int Create(CStr const&);
            bool operator==(Cursor const&);
            Cursor& operator=(Cursor const&);
            ~Cursor();
            Cursor();
            Cursor(Cursor const&);

        private:
            rend::TexHandle m_tex;
            PointBase<float> m_sz;
            PointBase<float> m_spot;
        };
    }
}
