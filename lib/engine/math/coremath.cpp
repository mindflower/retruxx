#include "math/coremath.h"

#include "math/vector2.h"

#include <stdexcept>

int CBrezLine::start(int srcx, int srcy, int dstx, int dsty)
{
    // RVA 0x6014D0. NOTE - original defect, kept verbatim: the error term is
    // seeded at the usual full scale (2*minor - major) but both increments are
    // HALVED (`sar edx, 1` / `sar ebp, 1` at 0x60151C..0x601551) instead of
    // doubled, so they are four times too small for that scale. The walk
    // therefore lags the true line badly - (0,0)->(10,3) ends at (10,2) - and
    // when the minor delta is 1 the minor increment floors to 0, so m_d never
    // climbs back to zero and the minor axis never advances at all. This is
    // what ai::TraceLine has always traced; do not "fix" it.
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
    // RVA 0x6015A0. The error term and the chosen y step are LOCALS here: the
    // binary only reads the members. Assigning the picked step back into
    // m_yinc1 would overwrite the major-axis increment for every later step.
    if (m_i >= this->m_numsteps)
        return 0;
    this->m_i = m_i + 1;
    curx = this->m_x;
    cury = this->m_y;

    int const d = this->m_d;
    int yinc = 0;
    if (d >= 0)
    {
        this->m_d = d + this->m_dinc1;
        this->m_x += this->m_xinc1;
        yinc = this->m_yinc1;
    }
    else
    {
        this->m_d = d + this->m_dinc0;
        this->m_x += this->m_xinc0;
        yinc = this->m_yinc0;
    }
    this->m_y += yinc;
    return 1;
}

Quaternion Exp(const Quaternion& q)
{
    // RVA 0x5FF430 - the exponential of a pure quaternion (w is ignored): rotation by the
    // angle |xyz| about xyz. When sin(angle) is tiny, xyz is passed through unscaled.
    float const angle = sqrtf(q.x * q.x + q.y * q.y + q.z * q.z);
    float const sinAngle = sinf(angle);

    Quaternion result;
    result.w = cosf(angle);
    if (fabsf(sinAngle) > 1e-5f)
    {
        float const scale = sinAngle / angle;
        result.x = q.x * scale;
        result.y = q.y * scale;
        result.z = q.z * scale;
    }
    else
    {
        result.x = q.x;
        result.y = q.y;
        result.z = q.z;
    }
    return result;
}

Quaternion Ln(const Quaternion& q)
{
    // RVA 0x5FF4D0 - the logarithm of a unit quaternion: a pure quaternion angle * axis.
    // For |w| >= 1 or a tiny sin(angle), xyz is passed through unscaled.
    Quaternion result;
    float angle = 0.0f;
    float sinAngle = 0.0f;
    if (fabsf(q.w) < 1.0f)
    {
        angle = acosf(q.w);
        sinAngle = sinf(angle);
    }

    if (fabsf(q.w) < 1.0f && sinAngle > 1e-5f)
    {
        float const scale = angle / sinAngle;
        result.x = q.x * scale;
        result.y = q.y * scale;
        result.z = q.z * scale;
    }
    else
    {
        result.x = q.x;
        result.y = q.y;
        result.z = q.z;
    }
    result.w = 0.0f;
    return result;
}

Quaternion getTangent(Quaternion const& prevQuat, Quaternion const& currentQuat, Quaternion const& nextQuat)
{
    // RVA 0x5FF600 - the SQUAD control point at currentQuat:
    //   current * exp(-(ln(current^-1 * next) + ln(current^-1 * prev)) / 4)
    // The two products are written out because the binary sums their terms in an order
    // that differs from Quaternion::operator*; keeping it keeps the rounding identical.
    Quaternion const inv = currentQuat.getInversed();
    Quaternion const& n = nextQuat;
    Quaternion toNext;
    toNext.x = ((n.z * inv.y + n.w * inv.x) + inv.w * n.x) - inv.z * n.y;
    toNext.y = ((inv.w * n.y + inv.z * n.x) + n.w * inv.y) - n.z * inv.x;
    toNext.z = ((inv.w * n.z + n.w * inv.z) + n.y * inv.x) - n.x * inv.y;
    toNext.w = ((inv.w * n.w - n.x * inv.x) - inv.y * n.y) - n.z * inv.z;
    Quaternion const lnNext = Ln(toNext);

    Quaternion const& p = prevQuat;
    Quaternion toPrev;
    toPrev.x = ((inv.x * p.w + p.x * inv.w) + inv.y * p.z) - inv.z * p.y;
    toPrev.y = ((inv.y * p.w + inv.z * p.x) + p.y * inv.w) - inv.x * p.z;
    toPrev.z = ((inv.x * p.y + inv.z * p.w) + inv.w * p.z) - inv.y * p.x;
    toPrev.w = ((inv.w * p.w - inv.x * p.x) - inv.y * p.y) - inv.z * p.z;
    Quaternion const lnPrev = Ln(toPrev);

    Quaternion exponent;
    exponent.x = (lnPrev.x + lnNext.x) * -0.25f;
    exponent.y = (lnPrev.y + lnNext.y) * -0.25f;
    exponent.z = (lnPrev.z + lnNext.z) * -0.25f;
    exponent.w = (lnPrev.w + lnNext.w) * -0.25f;

    // This product matches Quaternion::operator*= term for term.
    Quaternion result = currentQuat;
    result *= Exp(exponent);
    return result;
}

float CalculateAngle(CVector2 const& a, CVector2 const& b)
{
    // RVA 0x5C0980 - the signed angle from a to b in radians, positive counter-clockwise.
    // A vector shorter than sqrt(1e-5) counts as zero, which gives an angle of pi/2.
    float bx = 0.0f;
    float by = 0.0f;
    float const bLenSq = b.x * b.x + b.y * b.y;
    if (bLenSq > 1e-5f)
    {
        float const inv = 1.0f / sqrtf(bLenSq);
        bx = inv * b.x;
        by = inv * b.y;
    }

    float ax = 0.0f;
    float ay = 0.0f;
    float const aLenSq = a.x * a.x + a.y * a.y;
    if (aLenSq > 1e-5f)
    {
        float const inv = 1.0f / sqrtf(aLenSq);
        ax = inv * a.x;
        ay = inv * a.y;
    }

    float const angle = acosf(ay * by + ax * bx);
    if (a.x * b.y - a.y * b.x < 0.0f)
    {
        return 0.0f - angle;
    }
    return angle;
}

Quaternion SLerp(Quaternion const& a, Quaternion const& b, float t)
{
    // RVA 0x5FED10 - spherical interpolation, except close to parallel or
    // antiparallel where sin(theta) is too small to divide by; there it falls
    // back to a straight lerp and renormalises.
    float const cosTheta = ((a.w * b.w + a.z * b.z) + a.y * b.y) + a.x * b.x;

    if (fabsf(cosTheta + 1.0f) <= 0.059999999f || fabsf(cosTheta - 1.0f) <= 0.059999999f)
    {
        // Antiparallel: walk away from a rather than towards it.
        float const ka = cosTheta <= 0.0f ? t - 1.0f : 1.0f - t;

        Quaternion r;
        r.x = b.x * t + a.x * ka;
        r.y = b.y * t + a.y * ka;
        r.z = b.z * t + a.z * ka;
        r.w = b.w * t + a.w * ka;

        float const lenSq = ((r.w * r.w + r.z * r.z) + r.y * r.y) + r.x * r.x;
        if (lenSq <= 0.0f)
        {
            return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
        }
        float const inv = 1.0f / sqrtf(lenSq);
        return Quaternion(r.x * inv, r.y * inv, r.z * inv, r.w * inv);
    }

    float const theta = acosf(cosTheta);
    float const invSinTheta = 1.0f / sqrtf(1.0f - cosTheta * cosTheta);
    return a * (sinf((1.0f - t) * theta) * invSinTheta) + b * (sinf(theta * t) * invSinTheta);
}

Quaternion SLerpAcc(Quaternion const& a, Quaternion const& b, float t)
{
    // RVA 0x5FF050 - SLerp along the shorter arc: b is negated when it lies in the other
    // hemisphere. The rest is SLerp inlined.
    // NOTE: the binary computes the inlined SLerp's cosine on the x87 stack (w, x, y, z
    // order, extended precision); SLerp sums it in float, so the last bit can differ.
    float const dot = ((a.x * b.x + b.z * a.z) + b.y * a.y) + b.w * a.w;
    if (dot < 0.0f)
    {
        return SLerp(a, Quaternion(0.0f - b.x, 0.0f - b.y, 0.0f - b.z, 0.0f - b.w), t);
    }
    return SLerp(a, b, t);
}

Quaternion SQuad(float t, Quaternion const& p, Quaternion const& a, Quaternion const& b, Quaternion const& q)
{
    // RVA 0x5FF570 - spherical quadrangle interpolation from p to q with control points a
    // and b.
    Quaternion const outer = SLerpAcc(p, q, t);
    Quaternion const inner = SLerpAcc(a, b, t);
    return SLerpAcc(outer, inner, (1.0f - t) * t * 2.0f);
}

Quaternion CubicInterpolation(float t, Quaternion const& q1, Quaternion const& q2, Quaternion const& q3, Quaternion const& q4)
{
    // RVA 0x5FF9F0 - SQUAD between q2 and q3, with q1 and q4 shaping the tangents.
    Quaternion const outTangent = getTangent(q2, q3, q4);
    Quaternion const inTangent = getTangent(q1, q2, q3);
    return SQuad(t, q2, inTangent, outTangent, q3);
}
