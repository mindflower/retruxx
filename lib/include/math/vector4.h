#pragma once

struct CVector;

struct CVector4
{
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
    float w = 0.f;

    CVector4(CVector const& v, float ww);
    CVector4(float xx, float yy, float zz, float ww);
    CVector4() = default;
};

// Passed to the renderer DLL (effect parameters).
static_assert(sizeof(CVector4) == 0x0010);
