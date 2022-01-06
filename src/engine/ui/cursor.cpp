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
            throw std::logic_error("Not implemented");
        }

        Cursor::Cursor()
        {
            throw std::logic_error("Not implemented");
        }

        Cursor::Cursor(Cursor const&)
        {
            throw std::logic_error("Not implemented");
        }
    }
}
