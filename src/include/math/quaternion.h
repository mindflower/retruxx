#pragma once

class CMatrix;

class Quaternion
{
public:
    void FromMatrix(CMatrix const&);
    void Normalize();
    void ToAxisAngle(CVector&, float&);
    void fromYPR(float, float, float);
    Quaternion operator*(float) const;
    Quaternion operator+(Quaternion const&) const;
    Quaternion(CMatrix const&);
    Quaternion(float, float, float, float);
    Quaternion();
    CMatrix ToMatrix() const;
    Quaternion getConjugated() const;
    void Zero();
    Quaternion getInversed() const;
    void Lerp(Quaternion const&, Quaternion const&, float);
    void RotZ(float);
    void FromAxisAngle(CVector const&, float);
    void RotX(float);
    void RotY(float);
    CVector vecRot(CVector const&) const;
    float& operator[](unsigned int);
    void Identity();
    float Norm() const;
    Quaternion& operator+=(Quaternion const&);
    Quaternion& operator*=(float);
    void operator*=(Quaternion const&);

private:
    float x;
    float y;
    float z;
    float w;
};
