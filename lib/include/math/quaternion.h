#pragma once

struct CMatrix;
struct CVector;

struct Quaternion
{
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
    float w = 0.0;

    Quaternion(CMatrix const& m);
    Quaternion(float qx, float qy, float qz, float qw);
    Quaternion() = default;
    void Identity();
    void Zero();
    float Norm() const;
    void Normalize();
    Quaternion& operator*=(float v);
    void operator*=(Quaternion const& b);
    void Lerp(Quaternion const& q1, Quaternion const& q2, float k2);
    void RotX(float radians);
    void RotY(float radians);
    void RotZ(float radians);
    void ToAxisAngle(CVector& axis, float& radians);
    void FromAxisAngle(CVector const& axis, float radians);
    void FromMatrix(CMatrix const& m);
    CMatrix ToMatrix() const;
    Quaternion operator*(float v) const;
    Quaternion& operator+=(Quaternion const& q);
    Quaternion operator+(Quaternion const& q) const;
    CVector vecRot(CVector const& v) const;
    Quaternion getConjugated() const;
    Quaternion getInversed() const;
    float& operator[](unsigned int i);
    float operator[](unsigned int) const;
    void fromYPR(float Yaw, float Pitch, float Roll);
};

static_assert(sizeof(Quaternion) == 0x0010);

// The Hamilton product a*b. Same terms as Quaternion::operator*=, summed in a different order.
Quaternion operator*(Quaternion const& a, Quaternion const& b);

inline Quaternion IdentityQuaternion(0.0, 0.0, 0.0, 1.0);