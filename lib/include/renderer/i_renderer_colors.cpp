#include "i_renderer_colors.h"
#include "retruxx/common.h"

m3d::rend::Colorf::Colorf(unsigned int clr)
{
    r = static_cast<float>((clr >> 16) & 0xFF) * (1.0f / 255.0f);
    g = static_cast<float>((clr >> 8) & 0xFF) * (1.0f / 255.0f);
    b = static_cast<float>(clr & 0xFF) * (1.0f / 255.0f);
    a = static_cast<float>((clr >> 24) & 0xFF) * (1.0f / 255.0f);
}

m3d::rend::Colorf::Colorf(float _r, float _g, float _b, float _a)
{
    this->r = _r;
    this->g = _g;
    this->b = _b;
    this->a = _a;
}

m3d::rend::Colorf::Colorf()
{
}
void m3d::rend::Colorf::init(unsigned int clr)
{
    RETRUXX_NOT_IMPLEMENTED;
}
void m3d::rend::Colorf::init(float _r, float _g, float _b, float _a)
{
    RETRUXX_NOT_IMPLEMENTED;
}
unsigned int m3d::rend::Colorf::ToRgba() const
{
    RETRUXX_NOT_IMPLEMENTED;
}
m3d::rend::Colori::Colori(unsigned int)
{
    RETRUXX_NOT_IMPLEMENTED;
}
m3d::rend::Colori::Colori(unsigned char, unsigned char, unsigned char, unsigned char)
{
    RETRUXX_NOT_IMPLEMENTED;
}
m3d::rend::Colori::Colori()
{
    RETRUXX_NOT_IMPLEMENTED;
}
void m3d::rend::Colori::init(unsigned char, unsigned char, unsigned char, unsigned char)
{
    RETRUXX_NOT_IMPLEMENTED;
}
void m3d::rend::Colori::AddSat(const m3d::rend::Colori&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
