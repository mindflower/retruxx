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

    void translation(CVector const& t);
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

    void rotAxis(CVector const&, float);
    void rotTranslate(Quaternion const& rot, CVector const& pos);

    void rotYPR(float y, float p, float r);
    void getYPR(float& y, float& p, float& r) const;

    void GetBasis(CVector& x, CVector& y, CVector& z) const;
    void FromBasis(CVector const& x, CVector const& y, CVector const& z);
    void GetNormalizedBasis(CVector& x, CVector& y, CVector& z) const;
    void GetInvBasis(CVector& x, CVector& y, CVector& z) const;
    void FromInvBasis(CVector const& x, CVector const& y, CVector const& z);
    void GetInvNormalizedBasis(CVector&, CVector&, CVector&) const;

    CVector getOrg() const;
    void setOrg(CVector const& t);
    CVector getOrgInv() const;

    void composeSRT(CVector const& s, CMatrix const& rot, CVector const& t);

    float GetScaleX() const;
    float GetScaleY() const;
    float GetScaleZ() const;

    void lookAtLH(CVector const& eye, CVector const& at, CVector const& up);

    CMatrix getTransposed() const;
    void transposeInplace();

    void orthoLH(float w, float h, float z0, float z1);
    void perspectiveFovLH(float fovY, float aspect, float z0, float z1);
    void perspectiveLH(float, float, float, float);

    void reflect(CPlane const& p);
    void shadow(CVector4 const& light, CPlane const& plane);

    void shearXbyYZ(float, float);
    void shearYbyXZ(float, float);
    void shearZbyXY(float, float);
    void shear(float sxy, float sxz, float syx, float syz, float szx, float szy);

    CVector vecRotBack(CVector const& v) const;
    CVector vecRot(CVector const& v) const;
    CVector vecMulBack(CVector const&) const;
    CVector4 vecMul(CVector4 const& v) const;
    CVector vecMul(CVector const& v) const;

    void vecRotBackInplace(CVector const&, CVector&) const;
    void vecRotInplace(CVector const&, CVector&) const;
    void vecMulBackInplace(CVector const&, CVector&) const;
    void vecMulInplace(CVector4 const&, CVector4&) const;
    void vecMulInplace(CVector const&, CVector&) const;

    void DiagMatrixMul(CVector const&);

    float& operator()(int i, int j);
    float operator()(int i, int j) const;
    CMatrix& operator*=(CMatrix const& lhs);

    void DecomposeScale(float& x, float& y, float& z);

    void createPlaneTransform();
    void transformPlane(CVector4&);
};

CMatrix operator*(CMatrix const& a, CMatrix const& b);
CMatrix operator+(CMatrix const& a, CMatrix const& b);
