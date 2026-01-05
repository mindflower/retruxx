#include "math/coremath.h"

#include "math/vector2.h"

#include <stdexcept>

int CBrezLine::start(int srcx, int srcy, int dstx, int dsty)
{
    this->m_y1 = dsty;
    auto v5 = dstx - srcx;
    auto v6 = dsty - srcy;
    this->m_x0 = srcx;
    this->m_x1 = dstx;
    this->m_y0 = srcy;
    if (dstx - srcx < 0)
        v5 = srcx - dstx;
    if (v6 < 0)
        v6 = srcy - dsty;

    int v7 = 0;
    if (v5 <= v6)
    {
        this->m_numsteps = v6 + 1;
        this->m_d = 2 * v5 - v6;
        this->m_dinc1 = (v5 - v6) >> 1;
        v7 = v5 >> 1;
        this->m_xinc1 = 1;
        this->m_yinc1 = 1;
        this->m_yinc0 = 1;
        this->m_xinc0 = 0;
    }
    else
    {
        this->m_numsteps = v5 + 1;
        this->m_d = 2 * v6 - v5;
        v7 = v6 >> 1;
        this->m_xinc0 = 1;
        this->m_xinc1 = 1;
        this->m_yinc1 = 1;
        this->m_dinc1 = (v6 - v5) >> 1;
        this->m_yinc0 = 0;
    }
    this->m_dinc0 = v7;
    if (srcx > dstx)
    {
        this->m_xinc0 = -this->m_xinc0;
        this->m_xinc1 = -1;
    }
    if (srcy > dsty)
    {
        this->m_yinc0 = -this->m_yinc0;
        this->m_yinc1 = -1;
    }
    this->m_x = srcx;
    this->m_y = srcy;
    this->m_i = 0;
    return this->m_numsteps;
}

int CBrezLine::step(int& curx, int& cury)
{
    if (m_i >= this->m_numsteps)
        return 0;
    this->m_i = m_i + 1;
    curx = this->m_x;
    cury = this->m_y;
    m_d = this->m_d;
    if (m_d >= 0)
    {
        auto v8 = m_d + this->m_dinc1;
        this->m_x += this->m_xinc1;
        this->m_d = v8;
        m_yinc1 = this->m_yinc1;
    }
    else
    {
        auto v6 = m_d + this->m_dinc0;
        this->m_x += this->m_xinc0;
        this->m_d = v6;
        m_yinc1 = this->m_yinc0;
    }
    this->m_y += m_yinc1;
    return 1;
}

Quaternion Exp(const Quaternion& q)
{
    // TODO: check this
    Quaternion result;
    auto v3 = sqrt(q.x * q.x + q.y * q.y + q.z * q.z);
    auto Angle = v3;
    auto v4 = sin(v3);
    auto sinAngle = v4;
    auto v5 = fabs(v4);
    auto v6 = cos(Angle);
    if (v5 <= 0.0000099999997)
    {
        auto x = q.x;
        auto z = q.z;
        auto y = q.y;
        result.w = v6;
        result.x = x;
        result.y = y;
        result.z = z;
    }
    else
    {
        auto v7 = q.x;
        auto v8 = (sinAngle / Angle) * q.z;
        auto v9 = (sinAngle / Angle) * q.y;
        result.w = v6;
        result.x = v7 * (sinAngle / Angle);
        result.y = v9;
        result.z = v8;
    }
    return result;
}

Quaternion Ln(const Quaternion& q)
{
    // TODO: check this
    Quaternion a1;
    auto v2 = acos(q.w);
    auto fAngle = v2;
    auto fSinAngle = sin(v2);
    if (fabs(q.w) >= 1.0 || fSinAngle <= 0.0000099999997)
    {
        auto z = q.z;
        auto y = q.y;
        a1.x = q.x;
        a1.y = y;
        a1.z = z;
    }
    else
    {
        auto v3 = q.z * (fAngle / fSinAngle);
        auto v4 = q.y * (fAngle / fSinAngle);
        a1.x = q.x * (fAngle / fSinAngle);
        a1.y = v4;
        a1.z = v3;
    }
    a1.w = 0.0;
    return a1;
}

Quaternion getTangent(Quaternion const& q1, Quaternion const& q2, Quaternion const& q3)
{
    // TODO: check this
    auto Inversed = q2.getInversed();
    auto v8 = Inversed.z * q3.x;

    Quaternion q;
    q.x = (((q3.z * Inversed.y) + (q3.w * Inversed.x)) + (Inversed.w * q3.x)) - (Inversed.z * q3.y);
    auto v9 = (((Inversed.w * q3.y) + v8) + (q3.w * Inversed.y)) - (q3.z * Inversed.x);
    auto v10 = q3.w * Inversed.z;
    q.y = v9;
    auto v11 = (((Inversed.w * q3.z) + v10) + (q3.y * Inversed.x)) - (q3.x * Inversed.y);
    auto v12 = q3.x * Inversed.x;
    q.z = v11;
    q.w = (((Inversed.w * q3.w) - v12) - (Inversed.y * q3.y)) - (q3.z * Inversed.z);

    Quaternion l1 = Ln(q);
    auto v13 = q2.getInversed();
    auto z = v13.z;
    q.x = (((v13.x * q1.w) + (q1.x * v13.w)) + (v13.y * q1.z)) - (z * q1.y);
    auto v15 = (((v13.y * q1.w) + (z * q1.x)) + (q1.y * v13.w)) - (v13.x * q1.z);
    auto v16 = v13.z * q1.w;
    q.y = v15;
    auto v17 = (((v13.x * q1.y) + v16) + (v13.w * q1.z)) - (v13.y * q1.x);
    auto v18 = v13.x * q1.x;
    q.z = v17;
    q.w = (((v13.w * q1.w) - v18) - (v13.y * q1.y)) - (v13.z * q1.z);

    auto e = Ln(q);
    q.x = (e.x + l1.x) * -0.25;
    q.y = (e.y + l1.y) * -0.25;
    q.z = (e.z + l1.z) * -0.25;
    q.w = (e.w + l1.w) * -0.25;
    e = q;
    auto v19 = Exp(e);
    auto v20 = (((v19.w * q2.y) + (q2.w * v19.y)) + (q2.z * v19.x)) - (v19.z * q2.x);
    auto v21 = (((q2.x * v19.y) + (v19.w * q2.z)) + (v19.z * q2.w)) - (v19.x * q2.y);
    auto v22 = (((v19.w * q2.w) - (v19.x * q2.x)) - (v19.y * q2.y)) - (q2.z * v19.z);

    Quaternion a1;
    a1.x = (((v19.z * q2.y) + (v19.x * q2.w)) + (v19.w * q2.x)) - (q2.z * v19.y);
    a1.y = v20;
    a1.z = v21;
    a1.w = v22;
    return a1;
}
float CalculateAngle(CVector2 const& a, CVector2 const& b)
{
    // TODO: check and refactor this
    long double v2;  // st7
    long double v3;  // st7
    float angle;     // [esp+0h] [ebp-10h]
    float anglea;    // [esp+0h] [ebp-10h]
    float angleb;    // [esp+0h] [ebp-10h]
    float v8;        // [esp+4h] [ebp-Ch]
    float v9;        // [esp+8h] [ebp-8h]
    float v10;       // [esp+Ch] [ebp-4h]

    if ((float)((float)(b.x * b.x) + (float)(b.y * b.y)) <= 0.0000099999997)
    {
        v10 = 0.0;
        v9 = 0.0;
    }
    else
    {
        v2 = 1.0 / sqrt((float)((float)(b.x * b.x) + (float)(b.y * b.y)));
        v9 = v2 * b.x;
        v10 = v2 * b.y;
    }
    angle = (float)(a.x * a.x) + (float)(a.y * a.y);
    if (angle <= 0.0000099999997)
    {
        v8 = 0.0;
        anglea = 0.0;
    }
    else
    {
        v3 = 1.0 / sqrt(angle);
        anglea = v3 * a.x;
        v8 = v3 * a.y;
    }
    angleb = acos(v8 * v10 + anglea * v9);
    if ((float)((float)(a.x * b.y) - (float)(a.y * b.x)) < 0.0)
        return (float)(0.0 - angleb);
    return angleb;
}

Quaternion SLerpAcc(Quaternion const& a, Quaternion const& b, float t)
{
    // TODO: check this
    auto v5 = (a.x * b.x) + (b.z * a.z);
    auto v6 = b.y * a.y;
    auto x = b.x;
    auto y = b.y;
    auto z = b.z;
    auto w = b.w;
    auto v12 = (v5 + v6) + (w * a.w);

    Quaternion realB;
    realB.x = b.x;
    realB.y = y;
    realB.z = z;
    realB.w = w;
    if (v12 < 0.0)
    {
        x = 0.0 - b.x;
        y = 0.0 - b.y;
        z = 0.0 - b.z;
        w = 0.0 - b.w;
        realB.x = x;
        realB.y = y;
        realB.z = z;
        realB.w = w;
    }
    auto v13 = realB.w * a.w + realB.x * a.x + realB.y * a.y + realB.z * a.z;
    auto ba = v13;
    if (fabs(v13 + 1.0) <= 0.059999999 || fabs(ba - 1.0) <= 0.059999999)
    {
        float v16 = 0.0;
        if (ba <= 0.0)
            v16 = t - 1.0;
        else
            v16 = 1.0 - t;

        Quaternion v33;
        v33.y = y * t;
        auto v17 = a.x;
        v33.x = x * t;
        v33.z = z * t;
        auto v18 = a.y;
        auto v19 = v17;
        auto v20 = a.w;
        v33.w = w * t;
        auto result_8a = a.z * v16;
        realB.y = (y * t) + (v18 * v16);
        realB.z = (z * t) + result_8a;
        realB.w = (w * t) + (v20 * v16);
        auto result = v33.x + (v19 * v16);
        auto cosTheta = (((realB.w * realB.w) + (realB.z * realB.z)) + (realB.y * realB.y)) + (result * result);

        auto resulta = 0.0;
        auto result_4 = 0.0;
        auto result_8 = 0.0;
        auto result_12 = 0.0;
        if (cosTheta <= 0.0)
        {
            resulta = 0.0;
            result_4 = 0.0;
            result_8 = 0.0;
            result_12 = 1.0;
        }
        else
        {
            auto v21 = 1.0 / sqrt(cosTheta);
            resulta = result * v21;
            result_4 = realB.y * v21;
            result_8 = realB.z * v21;
            result_12 = realB.w * v21;
        }
        Quaternion a1;
        a1.x = resulta;
        a1.y = result_4;
        a1.z = result_8;
        a1.w = result_12;
        return a1;
    }
    else
    {
        auto theta = acos(ba);
        auto bb = 1.0 / sqrt(1.0 - ba * ba);
        auto v = sin(theta * t) * bb;
        auto va = realB *  v;
        auto v22 = sin((1.0 - t) * theta) * bb;
        auto v14 = a * v22;
        return v14 + va;
    }
}

Quaternion SQuad(float t, Quaternion const& p, Quaternion const& a, Quaternion const& b, Quaternion const& q)
{
    // TODO: check this
    auto c = SLerpAcc( p, q, t);
    auto d = SLerpAcc(a, b, t);
    auto res = SLerpAcc(c, d, ((1.0 - t) * t) * 2.0);
    auto y = res.y;

    Quaternion result;
    result.x = res.x;
    auto z = res.z;
    result.y = y;
    auto w = res.w;
    result.z = z;
    result.w = w;
    return result;
}

Quaternion CubicInterpolation(float t, Quaternion const& q1, Quaternion const& q2, Quaternion const& q3, Quaternion const& q4)
{
    // TODO: check this
    auto tangent = getTangent(q2, q3, q4);
    auto v8 = getTangent(q1, q2, q3);
    
    return SQuad(t, q2, v8, tangent, q3);
}
