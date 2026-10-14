#pragma once

#include <cstdint>

namespace m3d
{
    namespace rend
    {
        struct Colorf
        {
            float r = 0.0;
            float g = 0.0;
            float b = 0.0;
            float a = 0.0;
            Colorf(unsigned int clr);
            Colorf(float _r, float _g, float _b, float _a);
            Colorf();
            void init(unsigned int clr);
            void init(float _r, float _g, float _b, float _a);
            unsigned int ToRgba() const;
        };

        static_assert(sizeof(Colorf) == 0x0010);

        union Colori
        {
            union
            {
                struct
                {
                    uint8_t r;
                    uint8_t g;
                    uint8_t b;
                    uint8_t a;
                };
                uint8_t clr[4];
                uint32_t rgba;
            };
            Colori(unsigned int);
            Colori(unsigned char, unsigned char, unsigned char, unsigned char);
            Colori();
            void init(unsigned char, unsigned char, unsigned char, unsigned char);
            void AddSat(m3d::rend::Colori const&);
        };

        static_assert(sizeof(Colori) == 0x0004);
    }  // namespace rend
}  // namespace m3d
