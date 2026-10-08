#define _USE_MATH_DEFINES

#include "math/matrix.h"
#include "math/plane.h"
#include "math/quaternion.h"
#include "math/vector.h"
#include "math/vector4.h"
#include "retruxx/common.h"

#include <cmath>
#include <utility>

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
    // RVA 0x635720. Inverse of a rigid transform (rotation + translation only):
    // transpose the 3x3 rotation and re-express the translation in that
    // transposed frame. Only valid when the rotation part is orthonormal -
    // any scale or shear is not undone.
    CMatrix res;
    res._11 = _11;
    res._12 = _21;
    res._13 = _31;
    res._14 = 0.0f;
    res._21 = _12;
    res._22 = _22;
    res._23 = _32;
    res._24 = 0.0f;
    res._31 = _13;
    res._32 = _23;
    res._33 = _33;
    res._34 = 0.0f;
    res._41 = -(_11 * _41) - (_12 * _42) - (_13 * _43);
    res._42 = -(_21 * _41) - (_22 * _42) - (_23 * _43);
    res._43 = -(_31 * _41) - (_32 * _42) - (_33 * _43);
    res._44 = 1.0f;
    return res;
}

void CMatrix::zero()
{
    memset(&m, 0, sizeof(m));
}

CMatrix CMatrix::getTransposed() const
{
    // RVA 0x4058D0
    CMatrix res;
    res._11 = _11;
    res._12 = _21;
    res._13 = _31;
    res._14 = _41;
    res._21 = _12;
    res._22 = _22;
    res._23 = _32;
    res._24 = _42;
    res._31 = _13;
    res._32 = _23;
    res._33 = _33;
    res._34 = _43;
    res._41 = _14;
    res._42 = _24;
    res._43 = _34;
    res._44 = _44;
    return res;
}

CMatrix CMatrix::getInverseRot() const
{
    // RVA 0x875E50. A plain transpose of all sixteen elements, which inverts an
    // orthonormal rotation; it is byte for byte what getTransposed does.
    return getTransposed();
}

void CMatrix::FromInvBasis(CVector const& x, CVector const& y, CVector const& z)
{
    // RVA 0x5F9D70. Writes the three vectors as the matrix ROWS, which is the
    // transpose of FromBasis and the counterpart to GetInvBasis.
    zero();
    _11 = x.x;
    _12 = x.y;
    _13 = x.z;
    _21 = y.x;
    _22 = y.y;
    _23 = y.z;
    _31 = z.x;
    _32 = z.y;
    _33 = z.z;
}

void CMatrix::DecomposeScale(float& x, float& y, float& z)
{
    x = GetScaleX();
    y = GetScaleY();
    z = GetScaleZ();
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

void CMatrix::shadow(CVector4 const& light, CPlane const& plane)
{
    // RVA 0x8A1E10 - the classic planar projection matrix. CPlane keeps the
    // plane as "n.p == m_dist", so the four component form is (n, -m_dist) and
    // the light/plane dot product picks up a minus in front of the w term.
    float const nDotL = light.x * plane.m_normal.x;
    float const dot = (light.y * plane.m_normal.y + plane.m_normal.z * light.z + nDotL) -
        plane.m_dist * light.w;

    _11 = dot - nDotL;
    _21 = -(light.x * plane.m_normal.y);
    _31 = -(light.x * plane.m_normal.z);
    _41 = plane.m_dist * light.x;

    _12 = -(light.y * plane.m_normal.x);
    _22 = dot - light.y * plane.m_normal.y;
    _32 = -(light.y * plane.m_normal.z);
    _42 = light.y * plane.m_dist;

    _13 = -(plane.m_normal.x * light.z);
    _23 = -(light.z * plane.m_normal.y);
    _33 = dot - plane.m_normal.z * light.z;
    _43 = plane.m_dist * light.z;

    _14 = -(light.w * plane.m_normal.x);
    _24 = -(light.w * plane.m_normal.y);
    _34 = -(light.w * plane.m_normal.z);
    _44 = plane.m_dist * light.w + dot;
}

void CMatrix::GetInvBasis(CVector& x, CVector& y, CVector& z) const
{
    // RVA 0x70AC10. Reads the matrix ROWS - the transposed, i.e. inverted,
    // basis. Reading columns here would just repeat GetBasis.
    x.x = _11;
    x.y = _12;
    x.z = _13;
    y.x = _21;
    y.y = _22;
    y.z = _23;
    z.x = _31;
    z.y = _32;
    z.z = _33;
}

void CMatrix::composeSRT(CVector const& s, CMatrix const& rot, CVector const& t)
{
    // RVA 0x63ECB0. Scales each row of the rotation by the matching scale
    // component and drops the translation in; the fourth column is zeroed.
    _11 = s.x * rot._11;
    _12 = rot._12 * s.x;
    _13 = rot._13 * s.x;
    _14 = 0.0f;
    _21 = rot._21 * s.y;
    _22 = rot._22 * s.y;
    _23 = rot._23 * s.y;
    _24 = 0.0f;
    _31 = rot._31 * s.z;
    _32 = rot._32 * s.z;
    _33 = rot._33 * s.z;
    _34 = 0.0f;
    _41 = t.x;
    _42 = t.y;
    _43 = t.z;
    _44 = 1.0f;
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

void CMatrix::translation(float x, float y, float z)
{
    // RVA 0x41DC80
    zero();
    _11 = 1.0f;
    _22 = 1.0f;
    _33 = 1.0f;
    _44 = 1.0f;
    _41 = x;
    _42 = y;
    _43 = z;
}

void CMatrix::translation(CVector const& t)
{
    // RVA 0x5130F0
    zero();
    _11 = 1.0f;
    _22 = 1.0f;
    _33 = 1.0f;
    _44 = 1.0f;
    _41 = t.x;
    _42 = t.y;
    _43 = t.z;
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
    // RVA 0x512F30 - the rotation matrix of a unit quaternion (row-vector convention), with
    // pos as the translation row.
    float const xx = rot.x * rot.x;
    float const yy = rot.y * rot.y;
    float const zz = rot.z * rot.z;
    float const xy = rot.x * rot.y;
    float const xz = rot.x * rot.z;
    float const yz = rot.y * rot.z;
    float const xw = rot.x * rot.w;
    float const yw = rot.y * rot.w;
    float const zw = rot.z * rot.w;

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

    float const scaleX = sqrtf(x.x * x.x + x.y * x.y + x.z * x.z + FLT_EPSILON);
    x.x = 1.0f / scaleX * x.x;
    x.y = 1.0f / scaleX * x.y;
    x.z = 1.0f / scaleX * x.z;

    float const scaleY = sqrtf(y.x * y.x + y.y * y.y + y.z * y.z + FLT_EPSILON);
    y.x = 1.0f / scaleY * y.x;
    y.y = 1.0f / scaleY * y.y;
    y.z = 1.0f / scaleY * y.z;

    float const scaleZ = sqrtf(z.x * z.x + z.y * z.y + z.z * z.z + FLT_EPSILON);
    z.x = 1.0f / scaleZ * z.x;
    z.y = 1.0f / scaleZ * z.y;
    z.z = 1.0f / scaleZ * z.z;
}

CVector CMatrix::vecMul(CVector const& v) const
{
    CVector result;
    result.x = (((_31 * v.z) + (_21 * v.y)) + (v.x * _11)) + _41;
    result.y = (((_32 * v.z) + (_22 * v.y)) + (_12 * v.x)) + _42;
    result.z = (((_33 * v.z) + (_23 * v.y)) + (_13 * v.x)) + _43;
    return result;
}

CVector4 CMatrix::vecMul(CVector4 const& v) const
{
    // RVA 0x7A46D0. Row-vector convention, same as the CVector overload but
    // with w taken from the vector rather than assumed to be one.
    CVector4 result;
    result.x = v.x * _11 + v.y * _21 + v.z * _31 + v.w * _41;
    result.y = v.x * _12 + v.y * _22 + v.z * _32 + v.w * _42;
    result.z = v.x * _13 + v.y * _23 + v.z * _33 + v.w * _43;
    result.w = v.x * _14 + v.y * _24 + v.z * _34 + v.w * _44;
    return result;
}

void CMatrix::GetBasis(CVector& x, CVector& y, CVector& z) const
{
    // RVA 0x5C74C0. The basis vectors are the matrix columns.
    x.x = _11;
    x.y = _21;
    x.z = _31;
    y.x = _12;
    y.y = _22;
    y.z = _32;
    z.x = _13;
    z.y = _23;
    z.z = _33;
}

CVector CMatrix::getOrg() const
{
    return {_41, _42, _43};
}

CVector CMatrix::vecRotBack(CVector const& v) const
{
    // RVA 0x405FC0. Rotates by the transposed 3x3, i.e. undoes vecRot for an
    // orthonormal matrix, and ignores the translation.
    CVector result;
    result.x = v.x * _11 + v.y * _12 + v.z * _13;
    result.y = v.x * _21 + v.y * _22 + v.z * _23;
    result.z = v.x * _31 + v.y * _32 + v.z * _33;
    return result;
}

void CMatrix::FromBasis(CVector const& x, CVector const& y, CVector const& z)
{
    // RVA 0x7BB020. Writes the three vectors as the matrix columns.
    zero();
    _11 = x.x;
    _21 = x.y;
    _31 = x.z;
    _12 = y.x;
    _22 = y.y;
    _32 = y.z;
    _13 = z.x;
    _23 = z.y;
    _33 = z.z;
}

// The four members below are declared in matrix.h but were never defined in retruxx; the
// renderer (dxrender9) needs them. They are restored from the original binary, where the
// engine's inline versions were instantiated into dxrender9's matrices.obj / clipPlanes.obj.

// orig 0x625930 matrix.h:222
float CMatrix::calcDeterminantSimple() const
{
    return (_11 * _22 - _12 * _21) * _33 - (_23 * _11 - _13 * _21) * _32 + (_23 * _12 - _13 * _22) * _31;
}

// orig 0x625970 matrix.h:435 - inverse of an affine (rotation / scale + translation) matrix; a
// singular matrix gives the zero matrix.
CMatrix CMatrix::getInverseSimple() const
{
    CMatrix res;
    float det = calcDeterminantSimple();
    if (det == 0.0f)
    {
        res.zero();
        return res;
    }

    float idet = 1.0f / det;

    res._11 = (_33 * _22 - _23 * _32) * idet;
    res._12 = (_13 * _32 - _33 * _12) * idet;
    res._13 = (_23 * _12 - _13 * _22) * idet;
    res._14 = 0.0f;

    res._21 = (_23 * _31 - _33 * _21) * idet;
    res._22 = (_33 * _11 - _13 * _31) * idet;
    res._23 = (_13 * _21 - _23 * _11) * idet;
    res._24 = 0.0f;

    res._31 = (_21 * _32 - _31 * _22) * idet;
    res._32 = (_12 * _31 - _32 * _11) * idet;
    res._33 = (_11 * _22 - _12 * _21) * idet;
    res._34 = 0.0f;

    res._41 = ((_33 * _42 - _43 * _32) * _21 + (_41 * _32 - _42 * _31) * _23 + (_43 * _31 - _33 * _41) * _22) * idet;
    res._42 = ((_43 * _11 - _13 * _41) * _32 + (_12 * _41 - _42 * _11) * _33 + (_13 * _42 - _12 * _43) * _31) * idet;
    res._43 = ((_12 * _21 - _11 * _22) * _43 + (_23 * _11 - _13 * _21) * _42 + (_13 * _22 - _23 * _12) * _41) * idet;
    res._44 = 1.0f;

    return res;
}

// orig 0x6278e0 matrix.h:803
void CMatrix::transposeInplace()
{
    std::swap(_12, _21);
    std::swap(_13, _31);
    std::swap(_14, _41);
    std::swap(_23, _32);
    std::swap(_24, _42);
    std::swap(_34, _43);
}

// orig 0x627950 matrix.h:1264 - the inverse transpose: the matrix that transforms plane
// coefficients the way this matrix transforms points.
void CMatrix::createPlaneTransform()
{
    CMatrix inv = getInverse();
    inv.transposeInplace();
    *this = inv;
}

CMatrix CMatrix::getInverse() const
{
    // RVA 0x512490 - Gauss-Jordan elimination on the augmented matrix [M | I] with scaled
    // partial pivoting. A row of zeros, or a zero last pivot, gives the identity.
    // NOTE: only the last pivot is checked for zero. A zero pivot earlier on is divided by
    // and the result fills with infinities and NaNs, as in the shipped code.
    float rows[4][8];
    float* s[4];
    for (int i = 0; i < 4; ++i)
    {
        s[i] = rows[i];
        for (int j = 0; j < 4; ++j)
        {
            s[i][j] = m[i][j];
            s[i][4 + j] = i == j ? 1.0f : 0.0f;
        }
    }

    CMatrix identityResult;
    identityResult.identity();

    // Each row's largest magnitude, used to scale the pivot search.
    float scale[4];
    for (int i = 0; i < 4; ++i)
    {
        scale[i] = std::fabs(s[i][0]);
        for (int j = 1; j < 4; ++j)
        {
            float const a = std::fabs(s[i][j]);
            if (a > scale[i])
            {
                scale[i] = a;
            }
        }
        if (scale[i] == 0.0f)
        {
            return identityResult;
        }
    }

    // Forward elimination.
    for (int p = 0; p < 4; ++p)
    {
        int best = p;
        float bestVal = std::fabs(s[p][p] / scale[p]);
        for (int r = p + 1; r < 4; ++r)
        {
            float const val = std::fabs(s[r][p] / scale[r]);
            if (val > bestVal)
            {
                bestVal = val;
                best = r;
            }
        }
        if (best != p)
        {
            std::swap(s[p], s[best]);
            std::swap(scale[p], scale[best]);
        }

        for (int r = p + 1; r < 4; ++r)
        {
            float const factor = s[r][p] / s[p][p];
            s[r][p] = 0.0f;
            for (int col = p + 1; col < 8; ++col)
            {
                s[r][col] = s[r][col] - s[p][col] * factor;
            }
        }
    }

    if (s[3][3] == 0.0f)
    {
        return identityResult;
    }

    // Back substitution on the unnormalised rows; each row is divided by its pivot last.
    for (int c = 3; c > 0; --c)
    {
        for (int r = c - 1; r >= 0; --r)
        {
            float const factor = s[r][c] / s[c][c];
            for (int col = r + 1; col < 8; ++col)
            {
                s[r][col] = s[r][col] - s[c][col] * factor;
            }
        }
    }

    CMatrix inverse;
    for (int i = 0; i < 4; ++i)
    {
        float const invPivot = 1.0f / s[i][i];
        for (int j = 0; j < 4; ++j)
        {
            inverse.m[i][j] = s[i][4 + j] * invPivot;
        }
    }
    return inverse;
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
    // RVA 0x41D7F0 - NOTE: the x scale is cot(fovY * aspect / 2), not cot(fovY / 2) / aspect. The
    // tangents are taken on the FPU stack at full precision, which tan(double) comes closest to.
    zero();
    _33 = z1 / (z1 - z0);
    _43 = 0.0f - _33 * z0;
    _34 = 1.0f;
    _11 = static_cast<float>(1.0 / std::tan(static_cast<double>(fovY * aspect * 0.5f)));
    _22 = static_cast<float>(1.0 / std::tan(static_cast<double>(fovY * 0.5f)));
}

void CMatrix::rotZ(float a)
{
    // RVA 0x4069A0
    float const s = std::sin(a);
    float const c = std::cos(a);
    zero();
    _11 = c;
    _12 = s;
    _21 = -s;
    _22 = c;
    _33 = 1.0f;
    _44 = 1.0f;
}

void CMatrix::rotY(float a)
{
    // RVA 0x4068F0
    float const s = std::sin(a);
    float const c = std::cos(a);
    zero();
    _11 = c;
    _13 = -s;
    _22 = 1.0f;
    _31 = s;
    _33 = c;
    _44 = 1.0f;
}

void CMatrix::rotX(float a)
{
    // RVA 0x406960
    float const s = std::sin(a);
    float const c = std::cos(a);
    zero();
    _11 = 1.0f;
    _22 = c;
    _23 = s;
    _32 = -s;
    _33 = c;
    _44 = 1.0f;
}

void CMatrix::orthoLH(float w, float h, float z0, float z1)
{
    // RVA 0x8A1DA0
    zero();
    _11 = 2.0f / w;
    _22 = 2.0f / h;
    _33 = 1.0f / (z1 - z0);
    _43 = z0 / (z0 - z1);
    _44 = 1.0f;
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

float CMatrix::operator()(int i, int j) const
{
    // RVA 0x5FEAC0. Row major, unchecked.
    return m[i][j];
}

float& CMatrix::operator()(int i, int j)
{
    // RVA 0x8C71E0. Row major, unchecked.
    return m[i][j];
}

void CMatrix::setOrg(CVector const& org)
{
    _41 = org.x;
    _42 = org.y;
    _43 = org.z;
}

void CMatrix::scaling(float x)
{
    // RVA 0x8CB7C0
    zero();
    _11 = x;
    _22 = x;
    _33 = x;
    _44 = 1.0f;
}

void CMatrix::scaling(float x, float y, float z)
{
    // RVA 0x63EC70
    zero();
    _11 = x;
    _22 = y;
    _33 = z;
    _44 = 1.0f;
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

void CMatrix::shear(float sxy, float sxz, float syx, float syz, float szx, float szy)
{
    // RVA 0x63ED80
    zero();
    _11 = 1.0f;
    _22 = 1.0f;
    _33 = 1.0f;
    _44 = 1.0f;
    _12 = sxy;
    _13 = sxz;
    _21 = syx;
    _23 = syz;
    _31 = szx;
    _32 = szy;
}
