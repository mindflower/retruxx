#pragma once

struct CMatrix;
struct CVector;

struct Quaternion
{
    /* 0x0000 */ float x = 0.0;
    /* 0x0004 */ float y = 0.0;
    /* 0x0008 */ float z = 0.0;
    /* 0x000c */ float w = 0.0;

    Quaternion(const CMatrix& m);
    Quaternion(float qx, float qy, float qz, float qw);
    Quaternion() = default;
    void Identity();
    void Zero();
    float Norm() const;
    void Normalize();
    Quaternion& operator*=(float v);
    void operator*=(const Quaternion& b);
    void Lerp(const Quaternion& q1, const Quaternion& q2, float k2);
    void RotX(float radians);
    void RotY(float radians);
    void RotZ(float radians);
    void ToAxisAngle(CVector& axis, float& radians);
    void FromAxisAngle(const CVector& axis, float radians);
    void FromMatrix(const CMatrix& m);
    CMatrix ToMatrix() const;
    Quaternion operator*(float v) const;
    Quaternion& operator+=(const Quaternion& q);
    Quaternion operator+(const Quaternion& q) const;
    CVector vecRot(const CVector& v) const;
    Quaternion getConjugated() const;
    Quaternion getInversed() const;
    float& operator[](unsigned int i);
    float operator[](unsigned int) const;
    void fromYPR(float Yaw, float Pitch, float Roll);
}; /* size: 0x0010 */

static_assert(sizeof(Quaternion) == 0x0010);

inline Quaternion IdentityQuaternion(0.0, 0.0, 0.0, 1.0);