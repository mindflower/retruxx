#pragma once

class CPlane;
class CVector4;

class CMatrix
{
public:
    CMatrix(CMatrix const&);
    CMatrix();

    CVector vecRot(CVector const&) const;
    CMatrix getInverseRotTranslate() const;
    void zero();
    CMatrix getTransposed() const;
    float GetScaleZ() const;
    CMatrix getInverseRot() const;
    void FromInvBasis(CVector const&, CVector const&, CVector const&);
    void DecomposeScale(float&, float&, float&);
    float GetScaleX() const;
    void shadow(CVector4 const&, CPlane const&);
    void GetInvBasis(CVector&, CVector&, CVector&) const;
    void composeSRT(CVector const&, CMatrix const&, CVector const&);
    void reflect(CPlane const&);
    void translation(CVector const&);
    void translation(float, float, float);
    void getYPR(float&, float&, float&) const;
    void rotTranslate(Quaternion const&, CVector const&);
    void GetNormalizedBasis(CVector&, CVector&, CVector&) const;
    CVector vecMul(CVector const&) const;
    CVector4 vecMul(CVector4 const&) const;
    void GetBasis(CVector&, CVector&, CVector&) const;
    CVector getOrg() const;
    CVector vecRotBack(CVector const&) const;
    void FromBasis(CVector const&, CVector const&, CVector const&);
    CMatrix getInverse() const;
    void operator*=(CMatrix const&);
    CVector getOrgInv() const;
    void perspectiveFovLH(float, float, float, float);
    void rotZ(float);
    void rotY(float);
    void rotX(float);
    void orthoLH(float, float, float, float);
    void rotYPR(float, float, float);
    float operator()(int, int) const;
    float& operator()(int, int);
    void setOrg(CVector const&);
    void scaling(float);
    void scaling(float, float, float);
    void identity();
    void lookAtLH(CVector const&, CVector const&, CVector const&);
    void shear(float, float, float, float, float, float);

private:
    //$1B3C71F1CD740A568635C239F882810D ___u0;
};
