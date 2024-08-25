#pragma once

class CPlane;
class CVector4;
class CVector;
class Quaternion;

class CMatrix
{
public:
    CMatrix(CMatrix const&);
    CMatrix();
    void zero();
    void identity();
    void translation(CVector const&);
    void translation(float, float, float);
    void scaling(float);
    void scaling(float, float, float);
    void rotX(float);
    void rotY(float);
    void rotZ(float);
    float calcDeterminant() const;
    float calcDeterminantSimple() const;
    CMatrix getInverse() const;
    CMatrix getInverseRot() const;
    CMatrix getInverseRotTranslate() const;
    CMatrix getInverseSimple() const;
    void rotAxis(CVector const&, float);
    void rotTranslate(Quaternion const&, CVector const&);
    void rotYPR(float, float, float);
    void getYPR(float&, float&, float&) const;
    void GetBasis(CVector&, CVector&, CVector&) const;
    void FromBasis(CVector const&, CVector const&, CVector const&);
    void GetNormalizedBasis(CVector&, CVector&, CVector&) const;
    void GetInvBasis(CVector&, CVector&, CVector&) const;
    void FromInvBasis(CVector const&, CVector const&, CVector const&);
    void GetInvNormalizedBasis(CVector&, CVector&, CVector&) const;
    CVector getOrg() const;
    void setOrg(CVector const&);
    CVector getOrgInv() const;
    void composeSRT(CVector const&, CMatrix const&, CVector const&);
    float GetScaleX() const;
    float GetScaleY() const;
    float GetScaleZ() const;
    void lookAtLH(CVector const&, CVector const&, CVector const&);
    CMatrix getTransposed() const;
    void transposeInplace();
    void orthoLH(float, float, float, float);
    void perspectiveFovLH(float, float, float, float);
    void perspectiveLH(float, float, float, float);
    void reflect(CPlane const&);
    void shadow(CVector4 const&, CPlane const&);

    void shearXbyYZ(float, float, float, float, float, float);
    void shearYbyXZ(float, float, float, float, float, float);
    void shearZbyXY(float, float, float, float, float, float);

    void shear(float, float, float, float, float, float);
    CVector vecRotBack(CVector const&) const;
    CVector vecRot(CVector const&) const;
    CVector vecMulBack(CVector const&) const;
    CVector vecMul(CVector const&) const;
    CVector4 vecMul(CVector4 const&) const;
    CVector4 vecRotBackInplace(CVector4 const&) const;
    CVector4 vecRotInplace(CVector4 const&) const;
    CVector4 vecMulBackInplace(CVector4 const&) const;
    CVector4 vecMulInplace(CVector4 const&) const;
    CVector vecMulInplace(CVector const&) const;
    CVector DiagMatrixMul(CVector const&) const;
    float operator()(int, int) const;
    float& operator()(int, int);
    void operator*=(CMatrix const&);
    void DecomposeScale(float&, float&, float&);
    void createPlaneTransform();
    void transformPlane();

public:
    union
    {
        struct
        {
            float _11;
            float _12;
            float _13;
            float _14;
            float _21;
            float _22;
            float _23;
            float _24;
            float _31;
            float _32;
            float _33;
            float _34;
            float _41;
            float _42;
            float _43;
            float _44;
        };
        float m[4][4];
    };
};
