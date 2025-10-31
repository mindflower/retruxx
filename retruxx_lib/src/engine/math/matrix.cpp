#include <stdexcept>
#include <math/matrix.h>
#include <math/vector.h>
#include <math/vector4.h>
#include <ode/common.h>

#include "math/quaternion.h"
#include "thirdparty/containers.h"

CMatrix::CMatrix(CMatrix const& vv)
{
    *this = vv;
}

CMatrix::CMatrix()
{
}

CVector CMatrix::vecRot(CVector const& v) const
{
    CVector result;
    result.x = (float)((float)(this->_31 * v.z) + (float)(this->_21 * v.y)) + (float)(v.x * this->_11);
    result.y = (float)((float)(this->_32 * v.z) + (float)(this->_22 * v.y)) + (float)(this->_12 * v.x);
    result.z = (float)((float)(this->_33 * v.z) + (float)(this->_23 * v.y)) + (float)(this->_13 * v.x);
    return result;
}

CMatrix CMatrix::getInverseRotTranslate() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CMatrix::zero()
{
    memset(this, 0, sizeof(CMatrix));
}

CMatrix CMatrix::getTransposed() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

float CMatrix::GetScaleZ() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CMatrix CMatrix::getInverseRot() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CMatrix::FromInvBasis(CVector const&, CVector const&, CVector const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CMatrix::DecomposeScale(float& x, float& y, float& z)
{
    auto vy = this->_12;
    auto vy_4 = this->_22;
    auto vy_8 = this->_32;
    auto vz = this->_13;
    auto vz_4 = this->_23;
    auto vz_8 = this->_33;
    x = sqrt(this->_31 * this->_31 + this->_21 * this->_21 + this->_11 * this->_11);
    y = sqrt(vy_8 * vy_8 + vy_4 * vy_4 + vy * vy);
    z = sqrt(vz_8 * vz_8 + vz_4 * vz_4 + vz * vz);
}

float CMatrix::GetScaleX() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CMatrix::shadow(CVector4 const&, CPlane const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CMatrix::GetInvBasis(CVector&, CVector&, CVector&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CMatrix::composeSRT(CVector const&, CMatrix const&, CVector const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CMatrix::reflect(CPlane const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CMatrix::translation(CVector const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CMatrix::getYPR(float& y, float& p, float& r) const
{
    //TODO: check this and refactor
    long double v4; // st7
    float v5; // xmm1_4
    float v6; // xmm0_4
    long double v7; // st7
    float thetaX; // [esp+4h] [ebp-8h]
    float thetaXa; // [esp+4h] [ebp-8h]
    float thetaZ; // [esp+8h] [ebp-4h]

    v4 = asin(this->_23);
    thetaX = v4;
    v5 = thetaX;

    if (v4 >= M_PI / 2)
    {
        v7 = atan2(this->_12, this->_11);
        goto LABEL_6;
    }
    if (thetaX <= -M_PI / 2)
    {
        v7 = -atan2(this->_12, this->_11);
    LABEL_6:
        v6 = 0.0;
        thetaXa = v7;
        goto LABEL_7;
    }
    thetaXa = atan2(-this->_13, this->_33);
    thetaZ = atan2(-this->_21, this->_22);
    v6 = thetaZ;
LABEL_7:
    y = thetaXa;
    p = v5;
    r = v6;
}

void CMatrix::rotTranslate(Quaternion const& rot, CVector const& pos)
{
    // TODO: generated code
    // Calculate intermediate values for the rotation matrix
    float xx = rot.x * rot.x;
    float yy = rot.y * rot.y;
    float zz = rot.z * rot.z;
    float xy = rot.x * rot.y;
    float xz = rot.x * rot.z;
    float yz = rot.y * rot.z;
    float xw = rot.x * rot.w;
    float yw = rot.y * rot.w;
    float zw = rot.z * rot.w;

    // Build the rotation matrix from quaternion
    // First row
    this->_11 = 1.0f - 2.0f * (yy + zz);
    this->_12 = 2.0f * (xy + zw);
    this->_13 = 2.0f * (xz - yw);
    this->_14 = 0.0f;

    // Second row
    this->_21 = 2.0f * (xy - zw);
    this->_22 = 1.0f - 2.0f * (xx + zz);
    this->_23 = 2.0f * (yz + xw);
    this->_24 = 0.0f;

    // Third row
    this->_31 = 2.0f * (xz + yw);
    this->_32 = 2.0f * (yz - xw);
    this->_33 = 1.0f - 2.0f * (xx + yy);
    this->_34 = 0.0f;

    // Fourth row (translation)
    this->_41 = pos.x;
    this->_42 = pos.y;
    this->_43 = pos.z;
    this->_44 = 1.0f;
}

void CMatrix::GetNormalizedBasis(CVector& x, CVector& y , CVector& z) const
{
    x.x = this->_11;
    x.y = this->_21;
    x.z = this->_31;
    y.x = this->_12;
    y.y = this->_22;
    y.z = this->_32;
    z.x = this->_13;
    z.y = this->_23;
    z.z = this->_33;
    auto v4 = sqrt(x.x * x.x + x.y * x.y + x.z * x.z + 0.00000011920929);
    x.x = 1.0 / v4 * x.x;
    x.y = 1.0 / v4 * x.y;
    x.z = 1.0 / v4 * x.z;
    auto v5 = sqrt(y.x * y.x + y.y * y.y + y.z * y.z + 0.00000011920929);
    y.x = 1.0 / v5 * y.x;
    y.y = 1.0 / v5 * y.y;
    y.z = 1.0 / v5 * y.z;
    auto v6 = sqrt(z.x * z.x + z.y * z.y + z.z * z.z + 0.00000011920929);
    z.x = 1.0 / v6 * z.x;
    z.y = 1.0 / v6 * z.y;
    z.z = 1.0 / v6 * z.z;
}

CVector CMatrix::vecMul(CVector const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CVector4 CMatrix::vecMul(CVector4 const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CMatrix::GetBasis(CVector&, CVector&, CVector&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CVector CMatrix::getOrg() const
{
    CVector result = *(CVector*)&m[3][0];
    return result;
}

CVector CMatrix::vecRotBack(CVector const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CMatrix::FromBasis(CVector const&, CVector const&, CVector const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CMatrix CMatrix::getInverse() const
{
    CMatrix result;
    // Temporary arrays for the augmented matrix [A|I]
    float r1[8], r2[8], r3[8], r4[8];
    float* s[4] = { r1, r2, r3, r4 };

    // Initialize augmented matrix: original matrix + identity matrix
    for (int i = 0; i < 4; ++i) {
        float* row = s[i];

        // Copy original matrix row
        row[0] = this->m[i][0];
        row[1] = this->m[i][1];
        row[2] = this->m[i][2];
        row[3] = this->m[i][3];

        // Add identity matrix columns
        for (int j = 0; j < 4; ++j) {
            row[4 + j] = (i == j) ? 1.0f : 0.0f;
        }
    }

    // Scale factors for each row (for pivoting)
    float scp[4];
    for (int i = 0; i < 4; ++i) {
        float* row = s[i];
        scp[i] = std::max(std::max(std::abs(row[0]), std::abs(row[1])),
                          std::max(std::abs(row[2]), std::abs(row[3])));

        if (scp[i] == 0.0f) {
            // Matrix is singular, return identity
            result.identity();
            return result;
        }
    }

    // Gaussian elimination with partial pivoting
    for (int pivot = 0; pivot < 4; ++pivot) {
        // Find pivot row with maximum scaled value in current column
        int maxRow = pivot;
        float maxVal = std::abs(s[pivot][pivot] / scp[pivot]);

        for (int row = pivot + 1; row < 4; ++row) {
            float scaledVal = std::abs(s[row][pivot] / scp[row]);
            if (scaledVal > maxVal) {
                maxVal = scaledVal;
                maxRow = row;
            }
        }

        // Swap rows if necessary
        if (maxRow != pivot) {
            std::swap(s[pivot], s[maxRow]);
            std::swap(scp[pivot], scp[maxRow]);
        }

        // Check if pivot element is zero (matrix is singular)
        if (s[pivot][pivot] == 0.0f) {
            result.identity();
            return result;
        }

        // Eliminate entries below the pivot
        for (int row = pivot + 1; row < 4; ++row) {
            float factor = s[row][pivot] / s[pivot][pivot];
            s[row][pivot] = 0.0f;

            // Subtract factor * pivot row from current row
            for (int col = pivot + 1; col < 8; ++col) {
                s[row][col] -= factor * s[pivot][col];
            }
        }
    }

    // Check if last pivot is zero
    if (s[3][3] == 0.0f) {
        result.identity();
        return result;
    }

    // Back substitution
    CMatrix minv;
    for (int i = 3; i >= 0; --i) {
        float* row = s[i];
        float pivotInverse = 1.0f / row[i];

        // Solve for identity matrix columns
        for (int j = 0; j < 4; ++j) {
            minv.m[i][j] = row[4 + j] * pivotInverse;
        }

        // Eliminate entries above the pivot
        for (int rowAbove = 0; rowAbove < i; ++rowAbove) {
            float factor = s[rowAbove][i];
            for (int col = 0; col < 4; ++col) {
                s[rowAbove][4 + col] -= factor * minv.m[i][col];
            }
        }
    }

    return minv;
}

CMatrix& CMatrix::operator*=(CMatrix const& lhs)
{
    float _13; // xmm2_4
    float _14; // xmm3_4
    float _11; // xmm1_4
    float _12; // xmm0_4
    float v7; // xmm4_4
    float _24; // xmm3_4
    float v9; // xmm2_4
    float _21; // xmm1_4
    float v11; // xmm4_4
    float _23; // xmm2_4
    float v13; // xmm4_4
    float _22; // xmm0_4
    float v15; // xmm4_4
    float _34; // xmm3_4
    float v17; // xmm2_4
    float _31; // xmm1_4
    float v19; // xmm4_4
    float _33; // xmm2_4
    float _32; // xmm0_4
    float v22; // xmm4_4
    float _44; // xmm3_4
    float v24; // xmm2_4
    float _41; // xmm1_4
    float v26; // xmm4_4
    float _43; // xmm2_4
    float v28; // xmm4_4
    float _42; // xmm0_4

    _13 = this->_13;
    _14 = this->_14;
    _11 = this->_11;
    _12 = this->_12;
    this->_11 = (float)((float)((float)(lhs._41 * _14) + (float)(lhs._31 * _13)) + (float)(lhs._11 * this->_11))
        + (float)(_12 * lhs._21);
    this->_12 = (float)((float)((float)(lhs._12 * _11) + (float)(lhs._22 * _12)) + (float)(lhs._42 * _14))
        + (float)(lhs._32 * _13);
    this->_13 = (float)((float)((float)(lhs._13 * _11) + (float)(lhs._23 * _12)) + (float)(_14 * lhs._43))
        + (float)(_13 * lhs._33);
    v7 = (float)(lhs._44 * _14) + (float)(lhs._34 * _13);
    _24 = this->_24;
    v9 = lhs._14 * _11;
    _21 = this->_21;
    v11 = v7 + v9;
    _23 = this->_23;
    v13 = v11 + (float)(_12 * lhs._24);
    _22 = this->_22;
    this->_14 = v13;
    this->_21 = (float)((float)((float)(lhs._41 * _24) + (float)(lhs._31 * _23)) + (float)(lhs._11 * _21))
        + (float)(_22 * lhs._21);
    this->_22 = (float)((float)((float)(lhs._12 * _21) + (float)(lhs._22 * _22)) + (float)(lhs._42 * _24))
        + (float)(lhs._32 * _23);
    this->_23 = (float)((float)((float)(lhs._13 * _21) + (float)(lhs._23 * _22)) + (float)(_24 * lhs._43))
        + (float)(_23 * lhs._33);
    v15 = (float)(lhs._44 * _24) + (float)(lhs._34 * _23);
    _34 = this->_34;
    v17 = lhs._14 * _21;
    _31 = this->_31;
    v19 = v15 + v17;
    _33 = this->_33;
    this->_24 = v19 + (float)(_22 * lhs._24);
    _32 = this->_32;
    this->_31 = (float)((float)((float)(lhs._41 * _34) + (float)(lhs._31 * _33)) + (float)(lhs._11 * _31))
        + (float)(_32 * lhs._21);
    this->_32 = (float)((float)((float)(lhs._12 * _31) + (float)(lhs._22 * _32)) + (float)(lhs._42 * _34))
        + (float)(lhs._32 * _33);
    this->_33 = (float)((float)((float)(lhs._13 * _31) + (float)(lhs._23 * _32)) + (float)(_34 * lhs._43))
        + (float)(_33 * lhs._33);
    v22 = (float)(lhs._44 * _34) + (float)(lhs._34 * _33);
    _44 = this->_44;
    v24 = lhs._14 * _31;
    _41 = this->_41;
    v26 = v22 + v24;
    _43 = this->_43;
    v28 = v26 + (float)(_32 * lhs._24);
    _42 = this->_42;
    this->_34 = v28;
    this->_41 = (float)((float)((float)(lhs._41 * _44) + (float)(lhs._31 * _43)) + (float)(lhs._11 * _41))
        + (float)(_42 * lhs._21);
    this->_42 = (float)((float)((float)(lhs._12 * _41) + (float)(lhs._22 * _42)) + (float)(lhs._42 * _44))
        + (float)(lhs._32 * _43);
    this->_43 = (float)((float)((float)(lhs._13 * _41) + (float)(lhs._23 * _42)) + (float)(_44 * lhs._43))
        + (float)(_43 * lhs._33);
    this->_44 = (float)((float)((float)(lhs._44 * _44) + (float)(lhs._34 * _43)) + (float)(lhs._14 * _41))
        + (float)(_42 * lhs._24);

    return *this;
}

CVector CMatrix::getOrgInv() const
{
    CMatrix im = getInverse();
    CVector result = *(CVector*)&im.m[3][0];
    return result;
}

void CMatrix::perspectiveFovLH(float, float, float, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CMatrix::rotZ(float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CMatrix::rotY(float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CMatrix::rotX(float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CMatrix::orthoLH(float, float, float, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CMatrix::rotYPR(float y, float p, float r)
{
    //TODO: check and refactor this
    float v5; // [esp+0h] [ebp-118h]
    float v6; // [esp+4h] [ebp-114h]
    CMatrix matPitch; // [esp+8h] [ebp-110h] BYREF
    float v8; // [esp+48h] [ebp-D0h]
    float v9; // [esp+4Ch] [ebp-CCh]
    float v10; // [esp+50h] [ebp-C8h]
    CMatrix matRoll; // [esp+54h] [ebp-C4h] BYREF
    CMatrix vv; // [esp+94h] [ebp-84h] BYREF
    float v13; // [esp+D4h] [ebp-44h]
    CMatrix matYaw; // [esp+D8h] [ebp-40h] BYREF

    memset(&matYaw, 0, sizeof(matYaw));
    memset(&matPitch, 0, sizeof(matPitch));
    memset(&matRoll, 0, sizeof(matRoll));
    v8 = sin(y);
    v10 = cos(y);
    matYaw._13 = -v8;
    vv._11 = (float)((float)((float)(matPitch._41 * matYaw._14) + (float)(matPitch._21 * matYaw._12))
        + (float)(matPitch._31 * matYaw._13))
        + v10;
    v6 = sin(p);
    v9 = cos(p);
    vv._12 = (float)((float)((float)(matPitch._12 * v10) + (float)(matYaw._12 * v9)) + (float)(matPitch._42 * matYaw._14))
        + (float)((float)(0.0 - v6) * matYaw._13);
    v5 = sin(r);
    v13 = cos(r);
    matRoll._21 = -v5;
    vv._13 = matPitch._13 * v10 + matYaw._12 * v6 + matYaw._13 * v9 + matPitch._43 * matYaw._14;
    vv._14 = (float)((float)((float)(matPitch._14 * v10) + (float)(matPitch._24 * matYaw._12))
        + (float)(matPitch._34 * matYaw._13))
        + matYaw._14;
    vv._21 = (float)((float)((float)(matYaw._24 * matPitch._41) + (float)(matYaw._23 * matPitch._31)) + matYaw._21)
        + matPitch._21;
    vv._22 = (float)((float)((float)(matYaw._24 * matPitch._42) + (float)(matYaw._21 * matPitch._12))
        + (float)(matYaw._23 * (float)(0.0 - v6)))
        + v9;
    vv._24 = (float)((float)((float)(matYaw._23 * matPitch._34) + (float)(matYaw._21 * matPitch._14)) + matYaw._24)
        + matPitch._24;
    vv._23 = (float)((float)((float)(matYaw._23 * v9) + (float)(matYaw._24 * matPitch._43))
        + (float)(matYaw._21 * matPitch._13))
        + v6;
    vv._31 = (float)((float)((float)(matPitch._31 * v10) + (float)(matYaw._34 * matPitch._41))
        + (float)(matYaw._32 * matPitch._21))
        + v8;
    vv._32 = (float)((float)((float)(matYaw._32 * v9) + (float)(matPitch._12 * v8)) + (float)((float)(0.0 - v6) * v10))
        + (float)(matYaw._34 * matPitch._42);
    vv._33 = (float)((float)((float)(matYaw._32 * v6) + (float)(matPitch._13 * v8)) + (float)(matYaw._34 * matPitch._43))
        + (float)(v9 * v10);
    vv._41 = (float)((float)((float)(matYaw._43 * matPitch._31) + (float)(matYaw._42 * matPitch._21)) + matYaw._41)
        + matPitch._41;
    vv._34 = (float)((float)((float)(matPitch._34 * v10) + (float)(matPitch._14 * v8)) + (float)(matYaw._32 * matPitch._24))
        + matYaw._34;
    vv._42 = (float)((float)((float)(matYaw._42 * v9) + (float)(matYaw._41 * matPitch._12))
        + (float)(matYaw._43 * (float)(0.0 - v6)))
        + matPitch._42;
    vv._43 = (float)((float)((float)(matYaw._43 * v9) + (float)(matYaw._42 * v6)) + (float)(matYaw._41 * matPitch._13))
        + matPitch._43;
    vv._44 = (float)((float)((float)(matYaw._43 * matPitch._34) + (float)(matYaw._42 * matPitch._24))
        + (float)(matYaw._41 * matPitch._14))
        + 1.0;

    matPitch = vv;
    vv._11 = (float)((float)((float)(matPitch._11 * v13) + (float)(matRoll._41 * matPitch._14))
        + (float)(matRoll._31 * matPitch._13))
        + (float)(matPitch._12 * matRoll._21);
    vv._12 = (float)((float)((float)(matPitch._12 * v13) + (float)(matPitch._11 * v5))
        + (float)(matRoll._42 * matPitch._14))
        + (float)(matRoll._32 * matPitch._13);
    vv._13 = (float)((float)((float)(matRoll._13 * matPitch._11) + (float)(matRoll._43 * matPitch._14))
        + (float)(matRoll._23 * matPitch._12))
        + matPitch._13;
    vv._14 = (float)((float)((float)(matRoll._14 * matPitch._11) + (float)(matRoll._34 * matPitch._13))
        + (float)(matRoll._24 * matPitch._12))
        + matPitch._14;
    vv._21 = (float)((float)((float)(matPitch._21 * v13) + (float)(matPitch._24 * matRoll._41))
        + (float)(matPitch._23 * matRoll._31))
        + (float)(matPitch._22 * matRoll._21);
    vv._22 = (float)((float)((float)(matPitch._22 * v13) + (float)(matPitch._21 * v5))
        + (float)(matPitch._24 * matRoll._42))
        + (float)(matPitch._23 * matRoll._32);
    vv._23 = (float)((float)((float)(matPitch._24 * matRoll._43) + (float)(matPitch._22 * matRoll._23))
        + (float)(matPitch._21 * matRoll._13))
        + matPitch._23;
    vv._24 = (float)((float)((float)(matPitch._23 * matRoll._34) + (float)(matPitch._22 * matRoll._24))
        + (float)(matPitch._21 * matRoll._14))
        + matPitch._24;
    vv._31 = (float)((float)((float)(matPitch._31 * v13) + (float)(matPitch._34 * matRoll._41))
        + (float)(matPitch._33 * matRoll._31))
        + (float)(matPitch._32 * matRoll._21);
    vv._32 = (float)((float)((float)(matPitch._32 * v13) + (float)(matPitch._31 * v5))
        + (float)(matPitch._34 * matRoll._42))
        + (float)(matPitch._33 * matRoll._32);
    vv._34 = (float)((float)((float)(matPitch._33 * matRoll._34) + (float)(matPitch._32 * matRoll._24))
        + (float)(matPitch._31 * matRoll._14))
        + matPitch._34;
    vv._33 = (float)((float)((float)(matPitch._34 * matRoll._43) + (float)(matPitch._32 * matRoll._23))
        + (float)(matPitch._31 * matRoll._13))
        + matPitch._33;
    vv._42 = (float)((float)((float)(matPitch._42 * v13) + (float)(matPitch._41 * v5))
        + (float)(matPitch._44 * matRoll._42))
        + (float)(matPitch._43 * matRoll._32);
    vv._41 = (float)((float)((float)(matPitch._41 * v13) + (float)(matPitch._44 * matRoll._41))
        + (float)(matPitch._43 * matRoll._31))
        + (float)(matPitch._42 * matRoll._21);
    vv._43 = (float)((float)((float)(matPitch._44 * matRoll._43) + (float)(matPitch._42 * matRoll._23))
        + (float)(matPitch._41 * matRoll._13))
        + matPitch._43;
    vv._44 = (float)((float)((float)(matPitch._43 * matRoll._34) + (float)(matPitch._42 * matRoll._24))
        + (float)(matPitch._41 * matRoll._14))
        + matPitch._44;

    matYaw = vv;
    memcpy(this, &matYaw, sizeof(CMatrix));
}

float CMatrix::operator()(int, int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

float& CMatrix::operator()(int, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CMatrix::setOrg(CVector const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CMatrix::scaling(float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CMatrix::scaling(float, float, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CMatrix::identity()
{
    memset(this, 0, sizeof(CMatrix));
    _44 = 1.0;
    _33 = 1.0;
    _22 = 1.0;
    _11 = 1.0;
}

void CMatrix::lookAtLH(CVector const& eye, CVector const& at, CVector const& up)
{
    auto v4 = at.z - eye.z;
    auto v5 = at.x - eye.x;
    auto xAxis_4 = at.y - eye.y;
    auto ata = 1.0 / sqrt(v4 * v4 + xAxis_4 * xAxis_4 + v5 * v5 + 0.00000011920929);
    auto zAxis = ata * v5;
    auto v6 = xAxis_4 * ata;
    auto v7 = v4 * ata;
    auto z = up.z;
    auto v9 = (float)(up.y * v7) - (float)(z * (float)(xAxis_4 * ata));
    auto v10 = (float)(z * zAxis) - (float)(up.x * v7);
    auto xAxis_8 = (float)(up.x * (float)(xAxis_4 * ata)) - (float)(up.y * zAxis);
    auto atb = 1.0 / sqrt(xAxis_8 * xAxis_8 + v10 * v10 + v9 * v9 + 0.00000011920929);
    auto xAxis_8a = atb * xAxis_8;
    this->_11 = atb * v9;
    this->_21 = atb * v10;
    this->_31 = xAxis_8a;
    auto v11 = (float)((float)(eye.x * (float)(0.0 - (float)(atb * v9))) + (float)((float)(0.0 - (float)(atb * v10)) * eye.y))
        + (float)(eye.z * (float)(0.0 - xAxis_8a));
    auto yAxis = (float)(v6 * xAxis_8a) - (float)(v7 * (float)(atb * v10));
    this->_12 = yAxis;
    this->_41 = v11;
    auto v12 = (float)(v7 * (float)(atb * v9)) - (float)(zAxis * xAxis_8a);
    this->_22 = v12;
    auto yAxis_8 = (float)(zAxis * (float)(atb * v10)) - (float)(v6 * (float)(atb * v9));
    this->_32 = yAxis_8;
    auto v13 = (float)((float)(eye.x * (float)(0.0 - yAxis)) + (float)((float)(0.0 - v12) * eye.y))
        + (float)(eye.z * (float)(0.0 - yAxis_8));
    this->_13 = zAxis;
    this->_42 = v13;
    this->_23 = v6;
    this->_33 = v7;
    auto v14 = (float)(0.0 - v6) * eye.y;
    auto v15 = 0.0 - v7;
    auto v16 = (float)(eye.x * (float)(0.0 - zAxis)) + v14;
    auto v17 = eye.z * v15;
    this->_14 = 0.0;
    this->_24 = 0.0;
    this->_34 = 0.0;
    this->_43 = v16 + v17;
    this->_44 = 1.0;
}

void CMatrix::shear(float, float, float, float, float, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}
