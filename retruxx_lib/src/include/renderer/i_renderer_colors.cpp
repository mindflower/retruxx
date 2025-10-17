#include "i_renderer_colors.h"
#include "thirdparty/containers.h"

m3d::rend::Colorf::Colorf(unsigned int clr)
{
    r = static_cast<float>((clr >> 16) & 0xFF) * (1.0f / 255.0f);
    g = static_cast<float>((clr >> 8) & 0xFF) * (1.0f / 255.0f);
    b = static_cast<float>(clr & 0xFF) * (1.0f / 255.0f);
    a = static_cast<float>((clr >> 24) & 0xFF) * (1.0f / 255.0f);
}

m3d::rend::Colorf::Colorf(float _r, float _g, float _b, float _a)
{
    throw retruxx::logic_error("Not implemented");
}

m3d::rend::Colorf::Colorf()
{
}
