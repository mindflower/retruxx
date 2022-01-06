#include <flares.h>
#include <stdexcept>

namespace m3d
{
    CFlare::~CFlare()
    {
        throw std::logic_error("Not implemented");
    }

    int CFlare::Init()
    {
        throw std::logic_error("Not implemented");
    }

    void CFlare::Release()
    {
        throw std::logic_error("Not implemented");
    }

    CFlare::CFlare()
    {
        throw std::logic_error("Not implemented");
    }

    int CFlare::Render(FlareMode, CVector const&, float, float)
    {
        throw std::logic_error("Not implemented");
    }
}
