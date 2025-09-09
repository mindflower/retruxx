#pragma once

namespace m3d
{
    namespace rend
    {
        struct Colorf
        {
            /* 0x0000 */ float r;
            /* 0x0004 */ float g;
            /* 0x0008 */ float b;
            /* 0x000c */ float a;
            Colorf(unsigned int clr);
            Colorf(float _r, float _g, float _b, float _a);
            Colorf();
            void init(unsigned int clr);
            void init(float _r, float _g, float _b, float _a);
            unsigned int ToRgba() const;
        }; /* size: 0x0010 */
    }
}
