#include <stdexcept>
#include <math/plane.h>

void CPlane::buildplane(CPlane*, CVector*)
{
    throw std::logic_error("Not implemented");
}

CPlane::CPlane()
{
}

CPlane::CPlane(CPlane const&)
{
    throw std::logic_error("Not implemented");
}

void CPlane::fromPointNormal(CVector const&, CVector const&)
{
    throw std::logic_error("Not implemented");
}

float CPlane::intersectRay(CVector const&, CVector const&)
{
    throw std::logic_error("Not implemented");
}

CPlane CPlane::Reverse()
{
    throw std::logic_error("Not implemented");
}

float CPlane::dist(CVector const&)
{
    throw std::logic_error("Not implemented");
}

CVector CPlane::origin()
{
    throw std::logic_error("Not implemented");
}