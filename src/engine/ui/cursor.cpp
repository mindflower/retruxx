#include <m3dapp.h>
#include <stdexcept>
#include <ui/cursor.h>

namespace m3d
{
    namespace ui
    {
        int Cursor::Create(CStr const&, PointBase<float> const&, PointBase<float> const&)
        {
            throw std::logic_error("Not implemented");
        }

        int Cursor::Create(CStr const&)
        {
            throw std::logic_error("Not implemented");
        }

        bool Cursor::operator==(Cursor const&)
        {
            throw std::logic_error("Not implemented");
        }

        Cursor& Cursor::operator=(Cursor const&)
        {
            throw std::logic_error("Not implemented");
        }

        Cursor::~Cursor()
        {
            if (Application::g_pApp->m_renderer)
            {
                Application::g_pApp->m_renderer->ReleaseTexture(m_tex);
            }
        }

        Cursor::Cursor() :
            m_sz(0.0, 0.0),
            m_spot(0.0, 0.0)
        {
        }

        Cursor::Cursor(Cursor const&)
        {
            throw std::logic_error("Not implemented");
        }
    }
}
