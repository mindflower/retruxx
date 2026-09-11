#include "math/plane.h"
#include "retruxx/common.h"

void CPlane::buildplane(CPlane*, CVector*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CPlane::fromPointNormal(CVector const& org, CVector const& n)
{
    // RVA 0x7A4550 - neither m_type nor m_signbits is touched; callers that
    // need them run calcStuff() afterwards.
    m_normal = n;
    m_dist = (org.z * n.z + org.y * n.y) + org.x * n.x;
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
