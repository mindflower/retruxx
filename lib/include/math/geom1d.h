#pragma once

namespace geom1d
{
    // Every comparison in the 1D and 2D geometry uses this tolerance.
    template<class T>
    T EPSILON()
    {
        // RVA 0x7D4D60
        return static_cast<T>(0.000099999997f);
    }

    template<class T>
    T Abs(T a)
    {
        // RVA 0x83B890
        return a < 0.0f ? 0.0f - a : a;
    }

    template<class T>
    T Min(T a, T b)
    {
        // RVA 0x83B990
        return a < b ? a : b;
    }

    template<class T>
    T Max(T a, T b)
    {
        // RVA 0x83B9C0
        return a < b ? b : a;
    }

    template<class T>
    bool Between(T left, T right, T point)
    {
        // RVA 0x7D4F90 - point lies in the range, within the tolerance; the ends may come in either order.
        if (right <= left)
        {
            return !(point < right - EPSILON<T>()) && !(left + EPSILON<T>() < point);
        }
        return !(point < left - EPSILON<T>()) && !(right + EPSILON<T>() < point);
    }

    template<class T>
    bool DifferentSigned(T a, T b)
    {
        // RVA 0x83B8E0 - not both clearly positive or both clearly negative.
        return (a <= EPSILON<T>() && b >= -EPSILON<T>()) || (a >= -EPSILON<T>() && b <= EPSILON<T>());
    }

    template<class T>
    class Segment1
    {
    public:
        /* 0x0000 */ T begin;
        /* 0x0004 */ T end;
        Segment1(T const& _begin, T const& _end);
        Segment1();
        bool isPointOn(T const& point) const;
    }; /* size: 0x0008 */

    template<class T>
    bool SegmentsIntersect(Segment1<T> const& seg1, Segment1<T> const& seg2);
}  // namespace geom1d
