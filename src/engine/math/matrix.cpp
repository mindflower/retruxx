#include <stdexcept>
#include <math/matrix.h>
#include <math/vector.h>
#include <math/vector4.h>

CMatrix::CMatrix(CMatrix const&)
{
    throw std::logic_error("Not implemented");
}

CMatrix::CMatrix()
{
    throw std::logic_error("Not implemented");
}

CVector CMatrix::vecRot(CVector const&) const
{
    throw std::logic_error("Not implemented");
}

CMatrix CMatrix::getInverseRotTranslate() const
{
    throw std::logic_error("Not implemented");
}

void CMatrix::zero()
{
    throw std::logic_error("Not implemented");
}

CMatrix CMatrix::getTransposed() const
{
    throw std::logic_error("Not implemented");
}

float CMatrix::GetScaleZ() const
{
    throw std::logic_error("Not implemented");
}

CMatrix CMatrix::getInverseRot() const
{
    throw std::logic_error("Not implemented");
}

void CMatrix::FromInvBasis(CVector const&, CVector const&, CVector const&)
{
    throw std::logic_error("Not implemented");
}

void CMatrix::DecomposeScale(float&, float&, float&)
{
    throw std::logic_error("Not implemented");
}

float CMatrix::GetScaleX() const
{
    throw std::logic_error("Not implemented");
}

void CMatrix::shadow(CVector4 const&, CPlane const&)
{
    throw std::logic_error("Not implemented");
}

void CMatrix::GetInvBasis(CVector&, CVector&, CVector&) const
{
    throw std::logic_error("Not implemented");
}

void CMatrix::composeSRT(CVector const&, CMatrix const&, CVector const&)
{
    throw std::logic_error("Not implemented");
}

void CMatrix::reflect(CPlane const&)
{
    throw std::logic_error("Not implemented");
}

void CMatrix::translation(CVector const&)
{
    throw std::logic_error("Not implemented");
}

void CMatrix::translation(float, float, float)
{
    throw std::logic_error("Not implemented");
}

void CMatrix::getYPR(float&, float&, float&) const
{
    throw std::logic_error("Not implemented");
}

void CMatrix::rotTranslate(Quaternion const&, CVector const&)
{
    throw std::logic_error("Not implemented");
}

void CMatrix::GetNormalizedBasis(CVector&, CVector&, CVector&) const
{
    throw std::logic_error("Not implemented");
}

CVector CMatrix::vecMul(CVector const&) const
{
    throw std::logic_error("Not implemented");
}

CVector4 CMatrix::vecMul(CVector4 const&) const
{
    throw std::logic_error("Not implemented");
}

void CMatrix::GetBasis(CVector&, CVector&, CVector&) const
{
    throw std::logic_error("Not implemented");
}

CVector CMatrix::getOrg() const
{
    throw std::logic_error("Not implemented");
}

CVector CMatrix::vecRotBack(CVector const&) const
{
    throw std::logic_error("Not implemented");
}

void CMatrix::FromBasis(CVector const&, CVector const&, CVector const&)
{
    throw std::logic_error("Not implemented");
}

CMatrix CMatrix::getInverse() const
{
    throw std::logic_error("Not implemented");
}

void CMatrix::operator*=(CMatrix const&)
{
    throw std::logic_error("Not implemented");
}

CVector CMatrix::getOrgInv() const
{
    throw std::logic_error("Not implemented");
}

void CMatrix::perspectiveFovLH(float, float, float, float)
{
    throw std::logic_error("Not implemented");
}

void CMatrix::rotZ(float)
{
    throw std::logic_error("Not implemented");
}

void CMatrix::rotY(float)
{
    throw std::logic_error("Not implemented");
}

void CMatrix::rotX(float)
{
    throw std::logic_error("Not implemented");
}

void CMatrix::orthoLH(float, float, float, float)
{
    throw std::logic_error("Not implemented");
}

void CMatrix::rotYPR(float, float, float)
{
    throw std::logic_error("Not implemented");
}

float CMatrix::operator()(int, int) const
{
    throw std::logic_error("Not implemented");
}

float& CMatrix::operator()(int, int)
{
    throw std::logic_error("Not implemented");
}

void CMatrix::setOrg(CVector const&)
{
    throw std::logic_error("Not implemented");
}

void CMatrix::scaling(float)
{
    throw std::logic_error("Not implemented");
}

void CMatrix::scaling(float, float, float)
{
    throw std::logic_error("Not implemented");
}

void CMatrix::identity()
{
    throw std::logic_error("Not implemented");
}

void CMatrix::lookAtLH(CVector const&, CVector const&, CVector const&)
{
    throw std::logic_error("Not implemented");
}

void CMatrix::shear(float, float, float, float, float, float)
{
    throw std::logic_error("Not implemented");
}
