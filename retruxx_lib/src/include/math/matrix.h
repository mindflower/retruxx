#pragma once

class CPlane;
struct CVector4;
struct CVector;
struct Quaternion;

struct CMatrix
{
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

        float m[4][4] = {};
    };

    CMatrix() = default;

    void zero();
    void identity();

    void translation(const CVector& t);
    void translation(float x, float y, float z);

    void scaling(float x);
    void scaling(float x, float y, float z);

    void rotX(float a);
    void rotY(float a);
    void rotZ(float a);

    float calcDeterminant() const;
    float calcDeterminantSimple() const;

    CMatrix getInverse() const;
    CMatrix getInverseRot() const;
    CMatrix getInverseRotTranslate() const;
    CMatrix getInverseSimple() const;

    void rotAxis(const CVector&, float);
    void rotTranslate(const Quaternion& rot, const CVector& pos);

    void rotYPR(float y, float p, float r);
    void getYPR(float& y, float& p, float& r) const;

    void GetBasis(CVector& x, CVector& y, CVector& z) const;
    void FromBasis(const CVector& x, const CVector& y, const CVector& z);
    void GetNormalizedBasis(CVector& x, CVector& y, CVector& z) const;
    void GetInvBasis(CVector& x, CVector& y, CVector& z) const;
    void FromInvBasis(const CVector& x, const CVector& y, const CVector& z);
    void GetInvNormalizedBasis(CVector&, CVector&, CVector&) const;

    CVector getOrg() const;
    void setOrg(const CVector& t);
    CVector getOrgInv() const;

    void composeSRT(const CVector& s, const CMatrix& rot, const CVector& t);

    float GetScaleX() const;
    float GetScaleY() const;
    float GetScaleZ() const;

    void lookAtLH(const CVector& eye, const CVector& at, const CVector& up);

    CMatrix getTransposed() const;
    void transposeInplace();

    void orthoLH(float w, float h, float z0, float z1);
    void perspectiveFovLH(float fovY, float aspect, float z0, float z1);
    void perspectiveLH(float, float, float, float);

    void reflect(const CPlane& p);
    void shadow(const CVector4& light, const CPlane& plane);

    void shearXbyYZ(float, float);
    void shearYbyXZ(float, float);
    void shearZbyXY(float, float);
    void shear(float sxy, float sxz, float syx, float syz, float szx, float szy);

    CVector vecRotBack(const CVector& v) const;
    CVector vecRot(const CVector& v) const;
    CVector vecMulBack(const CVector&) const;
    CVector4 vecMul(const CVector4& v) const;
    CVector vecMul(const CVector& v) const;

    void vecRotBackInplace(const CVector&, CVector&) const;
    void vecRotInplace(const CVector&, CVector&) const;
    void vecMulBackInplace(const CVector&, CVector&) const;
    void vecMulInplace(const CVector4&, CVector4&) const;
    void vecMulInplace(const CVector&, CVector&) const;

    void DiagMatrixMul(const CVector&);

    float& operator()(int i, int j);
    float operator()(int i, int j) const;
    CMatrix& operator*=(const CMatrix& lhs);

    void DecomposeScale(float& x, float& y, float& z);

    void createPlaneTransform();
    void transformPlane(CVector4&);
};

CMatrix operator*(const CMatrix& a, const CMatrix& b);
