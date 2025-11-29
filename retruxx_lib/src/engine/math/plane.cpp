#include <stdexcept>
#include <math/plane.h>

#include "retruxx/common.h"

void CPlane::buildplane(CPlane*, CVector*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CPlane::CPlane()
{
}

CPlane::CPlane(CPlane const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CPlane::fromPointNormal(CVector const&, CVector const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

float CPlane::intersectRay(CVector const&, CVector const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CPlane CPlane::Reverse()
{
    RETRUXX_NOT_IMPLEMENTED;
}

float CPlane::dist(CVector const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CVector CPlane::origin()
{
    RETRUXX_NOT_IMPLEMENTED;
}