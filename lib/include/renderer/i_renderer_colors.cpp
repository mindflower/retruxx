#include "i_renderer_colors.h"
#include "retruxx/common.h"

m3d::rend::Colorf::Colorf(unsigned int clr)
{
    // RVA 0x8E59A0
    r = static_cast<float>((clr >> 16) & 0xFF) * (1.0f / 255.0f);
    g = static_cast<float>((clr >> 8) & 0xFF) * (1.0f / 255.0f);
    b = static_cast<float>(clr & 0xFF) * (1.0f / 255.0f);
    a = static_cast<float>((clr >> 24) & 0xFF) * (1.0f / 255.0f);
}

m3d::rend::Colorf::Colorf(float _r, float _g, float _b, float _a)
{
    // RVA 0x777EF0
    this->r = _r;
    this->g = _g;
    this->b = _b;
    this->a = _a;
}

m3d::rend::Colorf::Colorf()
{
    // RVA 0x6FDB50
}
void m3d::rend::Colorf::init(unsigned int clr)
{
    // RVA 0x7A4920
    r = static_cast<float>((clr >> 16) & 0xFF) * (1.0f / 255.0f);
    g = static_cast<float>((clr >> 8) & 0xFF) * (1.0f / 255.0f);
    b = static_cast<float>(clr & 0xFF) * (1.0f / 255.0f);
    a = static_cast<float>((clr >> 24) & 0xFF) * (1.0f / 255.0f);
}

void m3d::rend::Colorf::init(float _r, float _g, float _b, float _a)
{
    // RVA 0x777B50
    r = _r;
    g = _g;
    b = _b;
    a = _a;
}

namespace
{
    unsigned int ChannelToByte(float c)
    {
        if (c <= 0.0f)
        {
            return 0;
        }
        if (c >= 1.0f)
        {
            return 255;
        }
        return static_cast<unsigned int>(c * 255.0f + 0.5f);
    }
}  // namespace

unsigned int m3d::rend::Colorf::ToRgba() const
{
    // NOTE: not emitted in the shipped binary; packs back to the 0xAARRGGBB layout Colorf(unsigned) reads, with
    // the channels clamped to [0, 1].
    return (ChannelToByte(a) << 24) | (ChannelToByte(r) << 16) | (ChannelToByte(g) << 8) | ChannelToByte(b);
}

m3d::rend::Colori::Colori(unsigned int c)
{
    // NOTE: not emitted in the shipped binary; takes the packed value as is.
    rgba = c;
}

m3d::rend::Colori::Colori(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
{
    // NOTE: not emitted in the shipped binary.
    init(_r, _g, _b, _a);
}

m3d::rend::Colori::Colori()
{
    // RVA 0x8E1780 - leaves the colour uninitialized.
}

void m3d::rend::Colori::init(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
{
    // NOTE: not emitted in the shipped binary.
    r = _r;
    g = _g;
    b = _b;
    a = _a;
}

void m3d::rend::Colori::AddSat(const m3d::rend::Colori& c)
{
    // NOTE: not emitted in the shipped binary; adds per channel, saturating at 255.
    for (int i = 0; i < 4; ++i)
    {
        unsigned int const sum = static_cast<unsigned int>(clr[i]) + c.clr[i];
        clr[i] = static_cast<unsigned char>(sum > 255 ? 255 : sum);
    }
}
