#pragma once

namespace m3d
{
    namespace rend
    {
        struct Colorf
        {
            /* 0x0000 */ float r = 0.0;
            /* 0x0004 */ float g = 0.0;
            /* 0x0008 */ float b = 0.0;
            /* 0x000c */ float a = 0.0;
            Colorf(unsigned int clr);
            Colorf(float _r, float _g, float _b, float _a);
            Colorf();
            void init(unsigned int clr);
            void init(float _r, float _g, float _b, float _a);
            unsigned int ToRgba() const;
        }; /* size: 0x0010 */

        union Colori
        {
            union
            {
                struct
                {
                    /* 0x0000 */ unsigned char r;
                    /* 0x0001 */ unsigned char g;
                    /* 0x0002 */ unsigned char b;
                    /* 0x0003 */ unsigned char a;
                }; /* size: 0x0004 */
                /* 0x0000 */ unsigned char clr[4];
                /* 0x0000 */ unsigned int rgba;
            }; /* size: 0x0004 */
            Colori(unsigned int);
            Colori(unsigned char, unsigned char, unsigned char, unsigned char);
            Colori();
            void init(unsigned char, unsigned char, unsigned char, unsigned char);
            void AddSat(const m3d::rend::Colori&);
        }; /* size: 0x0004 */
    }
}
