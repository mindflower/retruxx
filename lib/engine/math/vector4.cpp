#include <stdexcept>
#include <math/vector4.h>
#include <math/vector.h>

CVector4::CVector4(float xx, float yy, float zz, float ww) :
    x(xx),
    y(yy),
    z(zz),
    w(ww)
{
}

CVector4::CVector4(const CVector& v, float ww) :
    x(v.x),
    y(v.y),
    z(v.z),
    w(ww)

{
}
