#pragma once

struct CVector;

struct CVector4
{
    /* 0x0000 */ float x = 0.f;
    /* 0x0004 */ float y = 0.f;
    /* 0x0008 */ float z = 0.f;
    /* 0x000c */ float w = 0.f;

    CVector4(const CVector& v, float ww);
    CVector4(float xx, float yy, float zz, float ww);
    CVector4() = default;
}; /* size: 0x0010 */
