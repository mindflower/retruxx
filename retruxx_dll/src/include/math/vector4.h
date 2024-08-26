#pragma once

class CVector;

class CVector4
{
public:
    CVector4();
    CVector4(float, float, float, float);
    CVector4(CVector const&, float);

private:
    float x;
    float y;
    float z;
    float w;
};
