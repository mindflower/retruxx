#define _USE_MATH_DEFINES

#include "math/matrix.h"
#include "math/plane.h"
#include "math/quaternion.h"
#include "math/vector.h"
#include "math/vector4.h"
#include "retruxx/common.h"

#include <cmath>

CVector CMatrix::vecRot(CVector const& v) const
{
    CVector result;
    result.x = _11 * v.x + _21 * v.y + _31 * v.z;
    result.y = _12 * v.x + _22 * v.y + _32 * v.z;
    result.z = _13 * v.x + _23 * v.y + _33 * v.z;
    return result;
}

CMatrix CMatrix::getInverseRotTranslate() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CMatrix::zero()
{
    memset(&m, 0, sizeof(m));
}

CMatrix CMatrix::getTransposed() const
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
    x = GetScaleX();
    y = GetScaleY();
    z = GetScaleZ();
}

CMatrix operator*(CMatrix const& a, CMatrix const& b)
{
    CMatrix res;

    // Row 1
    res._11 = a._11 * b._11 + a._12 * b._21 + a._13 * b._31 + a._14 * b._41;
    res._12 = a._11 * b._12 + a._12 * b._22 + a._13 * b._32 + a._14 * b._42;
    res._13 = a._11 * b._13 + a._12 * b._23 + a._13 * b._33 + a._14 * b._43;
    res._14 = a._11 * b._14 + a._12 * b._24 + a._13 * b._34 + a._14 * b._44;

    // Row 2
    res._21 = a._21 * b._11 + a._22 * b._21 + a._23 * b._31 + a._24 * b._41;
    res._22 = a._21 * b._12 + a._22 * b._22 + a._23 * b._32 + a._24 * b._42;
    res._23 = a._21 * b._13 + a._22 * b._23 + a._23 * b._33 + a._24 * b._43;
    res._24 = a._21 * b._14 + a._22 * b._24 + a._23 * b._34 + a._24 * b._44;

    // Row 3
    res._31 = a._31 * b._11 + a._32 * b._21 + a._33 * b._31 + a._34 * b._41;
    res._32 = a._31 * b._12 + a._32 * b._22 + a._33 * b._32 + a._34 * b._42;
    res._33 = a._31 * b._13 + a._32 * b._23 + a._33 * b._33 + a._34 * b._43;
    res._34 = a._31 * b._14 + a._32 * b._24 + a._33 * b._34 + a._34 * b._44;

    // Row 4
    res._41 = a._41 * b._11 + a._42 * b._21 + a._43 * b._31 + a._44 * b._41;
    res._42 = a._41 * b._12 + a._42 * b._22 + a._43 * b._32 + a._44 * b._42;
    res._43 = a._41 * b._13 + a._42 * b._23 + a._43 * b._33 + a._44 * b._43;
    res._44 = a._41 * b._14 + a._42 * b._24 + a._43 * b._34 + a._44 * b._44;

    return res;
}

CMatrix operator+(CMatrix const& a, CMatrix const& b)
{
    CMatrix res;

    // Row 1
    res._11 = a._11 + b._11;
    res._12 = a._12 + b._12;
    res._13 = a._13 + b._13;
    res._14 = a._14 + b._14;

    // Row 2
    res._21 = a._21 + b._21;
    res._22 = a._22 + b._22;
    res._23 = a._23 + b._23;
    res._24 = a._24 + b._24;

    // Row 3
    res._31 = a._31 + b._31;
    res._32 = a._32 + b._32;
    res._33 = a._33 + b._33;
    res._34 = a._34 + b._34;

    // Row 4
    res._41 = a._41 + b._41;
    res._42 = a._42 + b._42;
    res._43 = a._43 + b._43;
    res._44 = a._44 + b._44;

    return res;
}

float CMatrix::GetScaleX() const
{
    return sqrt(_31 * _31 + _21 * _21 + _11 * _11);
}

float CMatrix::GetScaleY() const
{
    return sqrt(_32 * _32 + _22 * _22 + _12 * _12);
}

float CMatrix::GetScaleZ() const
{
    return sqrt(_33 * _33 + _23 * _23 + _13 * _13);
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

void CMatrix::reflect(CPlane const& p)
{
    identity();

    float const nx2 = p.m_normal.x * p.m_normal.x * 2.0f;
    float const ny2 = p.m_normal.y * p.m_normal.y * 2.0f;
    float const nz2 = p.m_normal.z * p.m_normal.z * 2.0f;
    float const nxy2 = p.m_normal.x * p.m_normal.y * 2.0f;
    float const nxz2 = p.m_normal.x * p.m_normal.z * 2.0f;
    float const nyz2 = p.m_normal.y * p.m_normal.z * 2.0f;

    _11 = 1.0f - nx2;
    _22 = 1.0f - ny2;
    _33 = 1.0f - nz2;

    _12 = _21 = -nxy2;
    _13 = _31 = -nxz2;
    _23 = _32 = -nyz2;

    _41 = p.m_normal.x * p.m_dist * 2.0f;
    _42 = p.m_normal.y * p.m_dist * 2.0f;
    _43 = p.m_normal.z * p.m_dist * 2.0f;
}

void CMatrix::translation(CVector const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CMatrix::getYPR(float& y, float& p, float& r) const
{
    // Calculate pitch from _23 element (sin(pitch))
    p = asin(_23);

    // Handle gimbal lock cases (pitch near +-90 degrees)
    if (p >= M_PI_2)
    {
        // Gimbal lock at +90 degrees
        y = atan2(_12, _11);
        r = 0.0f;
    }
    else if (p <= -M_PI_2)
    {
        // Gimbal lock at -90 degrees
        y = -atan2(_12, _11);
        r = 0.0f;
    }
    else
    {
        // Normal case - no gimbal lock
        y = atan2(-_13, _33);
        r = atan2(-_21, _22);
    }
}

void CMatrix::rotTranslate(Quaternion const& rot, CVector const& pos)
{
    // TODO: generated code
    // Calculate intermediate values for the rotation matrix
    float const xx = rot.x * rot.x;
    float const yy = rot.y * rot.y;
    float const zz = rot.z * rot.z;
    float const xy = rot.x * rot.y;
    float const xz = rot.x * rot.z;
    float const yz = rot.y * rot.z;
    float const xw = rot.x * rot.w;
    float const yw = rot.y * rot.w;
    float const zw = rot.z * rot.w;

    // Build the rotation matrix from quaternion
    // First row
    _11 = 1.0f - 2.0f * (yy + zz);
    _12 = 2.0f * (xy + zw);
    _13 = 2.0f * (xz - yw);
    _14 = 0.0f;

    // Second row
    _21 = 2.0f * (xy - zw);
    _22 = 1.0f - 2.0f * (xx + zz);
    _23 = 2.0f * (yz + xw);
    _24 = 0.0f;

    // Third row
    _31 = 2.0f * (xz + yw);
    _32 = 2.0f * (yz - xw);
    _33 = 1.0f - 2.0f * (xx + yy);
    _34 = 0.0f;

    // Fourth row (translation)
    _41 = pos.x;
    _42 = pos.y;
    _43 = pos.z;
    _44 = 1.0f;
}

void CMatrix::GetNormalizedBasis(CVector& x, CVector& y, CVector& z) const
{
    x.x = _11;
    x.y = _21;
    x.z = _31;
    y.x = _12;
    y.y = _22;
    y.z = _32;
    z.x = _13;
    z.y = _23;
    z.z = _33;

    float const scaleX = sqrt(x.x * x.x + x.y * x.y + x.z * x.z + FLT_EPSILON);
    x.x = 1.0 / scaleX * x.x;
    x.y = 1.0 / scaleX * x.y;
    x.z = 1.0 / scaleX * x.z;

    float const scaleY = sqrt(y.x * y.x + y.y * y.y + y.z * y.z + FLT_EPSILON);
    y.x = 1.0 / scaleY * y.x;
    y.y = 1.0 / scaleY * y.y;
    y.z = 1.0 / scaleY * y.z;

    float const scaleZ = sqrt(z.x * z.x + z.y * z.y + z.z * z.z + FLT_EPSILON);
    z.x = 1.0 / scaleZ * z.x;
    z.y = 1.0 / scaleZ * z.y;
    z.z = 1.0 / scaleZ * z.z;
}

CVector CMatrix::vecMul(CVector const& v) const
{
    CVector result;
    result.x = (((_31 * v.z) + (_21 * v.y)) + (v.x * _11)) + _41;
    result.y = (((_32 * v.z) + (_22 * v.y)) + (_12 * v.x)) + _42;
    result.z = (((_33 * v.z) + (_23 * v.y)) + (_13 * v.x)) + _43;
    return result;
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
    return {_41, _42, _43};
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
    // TODO: generated code
    CMatrix result;
    // Temporary arrays for the augmented matrix [A|I]
    float r1[8], r2[8], r3[8], r4[8];
    float* s[4] = {r1, r2, r3, r4};

    // Initialize augmented matrix: original matrix + identity matrix
    for (int i = 0; i < 4; ++i)
    {
        float* row = s[i];

        // Copy original matrix row
        row[0] = this->m[i][0];
        row[1] = this->m[i][1];
        row[2] = this->m[i][2];
        row[3] = this->m[i][3];

        // Add identity matrix columns
        for (int j = 0; j < 4; ++j)
        {
            row[4 + j] = (i == j) ? 1.0f : 0.0f;
        }
    }

    // Scale factors for each row (for pivoting)
    float scp[4];
    for (int i = 0; i < 4; ++i)
    {
        float* row = s[i];
        scp[i] = std::max(std::max(std::abs(row[0]), std::abs(row[1])), std::max(std::abs(row[2]), std::abs(row[3])));

        if (scp[i] == 0.0f)
        {
            // Matrix is singular, return identity
            result.identity();
            return result;
        }
    }

    // Gaussian elimination with partial pivoting
    for (int pivot = 0; pivot < 4; ++pivot)
    {
        // Find pivot row with maximum scaled value in current column
        int maxRow = pivot;
        float maxVal = std::abs(s[pivot][pivot] / scp[pivot]);

        for (int row = pivot + 1; row < 4; ++row)
        {
            float scaledVal = std::abs(s[row][pivot] / scp[row]);
            if (scaledVal > maxVal)
            {
                maxVal = scaledVal;
                maxRow = row;
            }
        }

        // Swap rows if necessary
        if (maxRow != pivot)
        {
            std::swap(s[pivot], s[maxRow]);
            std::swap(scp[pivot], scp[maxRow]);
        }

        // Check if pivot element is zero (matrix is singular)
        if (s[pivot][pivot] == 0.0f)
        {
            result.identity();
            return result;
        }

        // Eliminate entries below the pivot
        for (int row = pivot + 1; row < 4; ++row)
        {
            float factor = s[row][pivot] / s[pivot][pivot];
            s[row][pivot] = 0.0f;

            // Subtract factor * pivot row from current row
            for (int col = pivot + 1; col < 8; ++col)
            {
                s[row][col] -= factor * s[pivot][col];
            }
        }
    }

    // Check if last pivot is zero
    if (s[3][3] == 0.0f)
    {
        result.identity();
        return result;
    }

    // Back substitution
    CMatrix minv;
    for (int i = 3; i >= 0; --i)
    {
        float* row = s[i];
        float pivotInverse = 1.0f / row[i];

        // Solve for identity matrix columns
        for (int j = 0; j < 4; ++j)
        {
            minv.m[i][j] = row[4 + j] * pivotInverse;
        }

        // Eliminate entries above the pivot
        for (int rowAbove = 0; rowAbove < i; ++rowAbove)
        {
            float factor = s[rowAbove][i];
            for (int col = 0; col < 4; ++col)
            {
                s[rowAbove][4 + col] -= factor * minv.m[i][col];
            }
        }
    }

    return minv;
}

CMatrix& CMatrix::operator*=(CMatrix const& lhs)
{
    *this = *this * lhs;
    return *this;
}

CVector CMatrix::getOrgInv() const
{
    CMatrix im = getInverse();
    return {im._41, im._42, im._43};
}

void CMatrix::perspectiveFovLH(float fovY, float aspect, float z0, float z1)
{
    memset(this, 0, sizeof(CMatrix));
    auto v5 = z1 / (float)(z1 - z0);
    this->_33 = v5;
    this->_43 = 0.0 - (float)(v5 * z0);
    this->_34 = 1.0;
    this->_11 = 1.0 / tan(fovY * aspect * 0.5);
    this->_22 = 1.0 / tan(fovY * 0.5);
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
    // Clear the current matrix to identity
    identity();

    // Create individual rotation matrices
    CMatrix matYaw, matPitch, matRoll;

    // Yaw rotation around Y axis
    matYaw.identity();

    auto const cosY = cos(y);
    auto const sinY = sin(y);
    matYaw._11 = cosY;
    matYaw._13 = -sinY;
    matYaw._31 = sinY;
    matYaw._33 = cosY;

    // Pitch rotation around X axis
    matPitch.identity();

    auto const cosP = cos(p);
    auto const sinP = sin(p);
    matPitch._22 = cosP;
    matPitch._23 = sinP;
    matPitch._32 = -sinP;
    matPitch._33 = cosP;

    // Roll rotation around Z axis
    matRoll.identity();

    auto const cosR = cos(r);
    auto const sinR = sin(r);
    matRoll._11 = cosR;
    matRoll._12 = sinR;
    matRoll._21 = -sinR;
    matRoll._22 = cosR;

    // Combine rotations: yaw * pitch * rol
    *this = matYaw * matPitch * matRoll;
}

float CMatrix::operator()(int, int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

float& CMatrix::operator()(int, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CMatrix::setOrg(CVector const& org)
{
    _41 = org.x;
    _42 = org.y;
    _43 = org.z;
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
    zero();
    _44 = 1.0;
    _33 = 1.0;
    _22 = 1.0;
    _11 = 1.0;
}

void CMatrix::lookAtLH(CVector const& eye, CVector const& at, CVector const& up)
{
    // Calculate forward vector (z-axis)
    CVector forward;
    forward.x = at.x - eye.x;
    forward.y = at.y - eye.y;
    forward.z = at.z - eye.z;

    // Normalize forward vector
    float forwardLength = sqrt(forward.x * forward.x + forward.y * forward.y + forward.z * forward.z + FLT_EPSILON);
    float invForwardLength = 1.0f / forwardLength;
    forward.x *= invForwardLength;
    forward.y *= invForwardLength;
    forward.z *= invForwardLength;

    // Calculate right vector (x-axis) = up * forward
    CVector right;
    right.x = up.y * forward.z - up.z * forward.y;
    right.y = up.z * forward.x - up.x * forward.z;
    right.z = up.x * forward.y - up.y * forward.x;

    // Normalize right vector
    float rightLength = sqrt(right.x * right.x + right.y * right.y + right.z * right.z + FLT_EPSILON);
    float invRightLength = 1.0f / rightLength;
    right.x *= invRightLength;
    right.y *= invRightLength;
    right.z *= invRightLength;

    // Calculate up vector (y-axis) = forward * right
    CVector newUp;
    newUp.x = forward.y * right.z - forward.z * right.y;
    newUp.y = forward.z * right.x - forward.x * right.z;
    newUp.z = forward.x * right.y - forward.y * right.x;

    // Set rotation basis vectors (transposed for view matrix)
    _11 = right.x;
    _12 = newUp.x;
    _13 = forward.x;
    _14 = 0.0f;
    _21 = right.y;
    _22 = newUp.y;
    _23 = forward.y;
    _24 = 0.0f;
    _31 = right.z;
    _32 = newUp.z;
    _33 = forward.z;
    _34 = 0.0f;

    // Set translation (negated dot products for view matrix)
    _41 = -(right.x * eye.x + right.y * eye.y + right.z * eye.z);
    _42 = -(newUp.x * eye.x + newUp.y * eye.y + newUp.z * eye.z);
    _43 = -(forward.x * eye.x + forward.y * eye.y + forward.z * eye.z);
    _44 = 1.0f;
}

void CMatrix::shear(float, float, float, float, float, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}
