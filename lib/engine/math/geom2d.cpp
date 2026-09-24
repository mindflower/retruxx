#include "math/geom2d.h"

#include <cmath>

// NOTE: the shipped code evaluates the square roots and some sums on the x87 stack at extended
// precision. Here they are done in double and rounded back to float at the same points, which can
// differ from the original in the last bit.

namespace
{
    // Every comparison in the 1D and 2D geometry uses the same tolerance.
    float constexpr EPS = 0.000099999997f;

    template<class T>
    T Abs(T v)
    {
        return v < 0.0f ? 0.0f - v : v;
    }
}  // namespace

namespace geom1d
{
    template<class T>
    Segment1<T>::Segment1(T const& _begin, T const& _end) : begin(_begin), end(_end)
    {
    }

    template<class T>
    Segment1<T>::Segment1()
    {
    }

    template<class T>
    bool Segment1<T>::isPointOn(T const& point) const
    {
        // Inlined into geom2d::AABBIntersects and geom2d::Segment2::isPointOn. The ends may come
        // in either order.
        if (end <= begin)
        {
            return !(point < end - EPS) && begin + EPS >= point;
        }
        return !(point < begin - EPS) && end + EPS >= point;
    }

    template<class T>
    bool SegmentsIntersect(Segment1<T> const& seg1, Segment1<T> const& seg2)
    {
        // Inlined into geom2d::AABBIntersects: the ranges overlap when either one holds an end
        // of the other.
        return seg1.isPointOn(seg2.begin) || seg1.isPointOn(seg2.end) || seg2.isPointOn(seg1.begin) ||
            seg2.isPointOn(seg1.end);
    }

    template class Segment1<float>;
    template bool SegmentsIntersect<float>(Segment1<float> const&, Segment1<float> const&);
}  // namespace geom1d

namespace geom2d
{
    template<class T>
    Point2<T>::Point2(T x_, T y_) : x(x_), y(y_)
    {
        // RVA 0x83B650
    }

    template<class T>
    Point2<T>::Point2()
    {
        // RVA 0x83B640 - the coordinates are left uninitialized.
    }

    template<class T>
    bool Point2<T>::operator==(Point2<T> const& point) const
    {
        // RVA 0x83C020
        return Abs(point.x - x) < EPS && Abs(point.y - y) < EPS;
    }

    template<class T>
    bool Point2<T>::operator!=(Point2<T> const& point) const
    {
        // RVA 0x83C080
        return !(Abs(point.x - x) < EPS && Abs(point.y - y) < EPS);
    }

    template<class T>
    Line2<T>::Line2(Point2<T> const& _begin, Point2<T> const& _end)
    {
        // RVA 0x83CDE0
        A = _begin.y - _end.y;
        B = _end.x - _begin.x;
        C = 0.0f - (_begin.x * A + B * _begin.y);
        Normalize();
    }

    template<class T>
    Line2<T>::Line2()
    {
    }

    template<class T>
    void Line2<T>::Normalize()
    {
        // RVA 0x83C0E0 - NOTE: C is included in the length, so the normal (A, B) does not come out
        // unit length. signedDist divides by |(A, B)| again, which is why distances still work.
        float const len = static_cast<float>(std::sqrt(double(A) * A + double(B) * B + double(C) * C));
        float const invLen = static_cast<float>(1.0 / len);
        A = invLen * A;
        B = invLen * B;
        C = invLen * C;
    }

    template<class T>
    T Line2<T>::pointWeight(Point2<T> const& point) const
    {
        // RVA 0x83B820
        return point.y * B + point.x * A + C;
    }

    template<class T>
    T Line2<T>::signedDist(Point2<T> const& point) const
    {
        // RVA 0x83C140
        return static_cast<T>(pointWeight(point) / std::sqrt(double(B) * B + double(A) * A));
    }

    template<class T>
    T Line2<T>::Dist(Point2<T> const& point) const
    {
        // RVA 0x83CE90
        return std::fabs(signedDist(point));
    }

    template<class T>
    bool Line2<T>::isPointOn(Point2<T> const& point) const
    {
        // RVA 0x83DB60
        return Dist(point) < EPS;
    }

    template<class T>
    std::vector<Point2<T>>& Polygon2<T>::getPoints()
    {
        // RVA 0x83B5E0
        return m_Points;
    }

    template<class T>
    std::vector<Point2<T>> const& Polygon2<T>::getPoints() const
    {
        // RVA 0x83B5D0
        return m_Points;
    }

    template<class T>
    bool Polygon2<T>::isPointInside(Point2<T> const& point)
    {
        // RVA 0x83EE40 - an even-odd ray cast towards a fixed far point, walking the edges from the
        // last one back. A point on an edge counts as inside.
        static Point2<T> const Inf(50000.0f, 49999.0f);
        Segment2<T> const ray(point, Inf);

        // NOTE: the counter is a char in the original; only its parity is used, so wrapping is harmless.
        char intCount = 0;
        for (int j = static_cast<int>(m_Points.size()) - 1; j >= 0; --j)
        {
            int next = j + 1;
            if (next == static_cast<int>(m_Points.size()))
            {
                next = 0;
            }

            Segment2<T> const segment(m_Points[j], m_Points[next]);
            if (SegmentsIntersect(ray, segment))
            {
                ++intCount;
            }
            if (segment.isPointOn(point))
            {
                return true;
            }
        }
        return (intCount & 1) != 0;
    }

    template<class T>
    Segment2<T>::Segment2(Point2<T> const& _begin, Point2<T> const& _end) : begin(_begin), end(_end)
    {
        // RVA 0x83B730
    }

    template<class T>
    Segment2<T>::Segment2()
    {
    }

    template<class T>
    bool Segment2<T>::isPointOn(Point2<T> const& Point) const
    {
        // RVA 0x83EA20 - a degenerate segment is treated as a point.
        if (Abs(end.x - begin.x) < EPS && Abs(end.y - begin.y) < EPS)
        {
            return Abs(begin.x - Point.x) < EPS && Abs(begin.y - Point.y) < EPS;
        }

        Line2<T> const line(begin, end);
        if (!line.isPointOn(Point))
        {
            return false;
        }
        return geom1d::Segment1<T>(begin.x, end.x).isPointOn(Point.x) &&
            geom1d::Segment1<T>(begin.y, end.y).isPointOn(Point.y);
    }

    template<class T>
    AABB2<T>::AABB2(Segment2<T> const& s)
    {
        // RVA 0x83C1E0
        pMin.x = s.begin.x < s.end.x ? s.begin.x : s.end.x;
        pMin.y = s.begin.y < s.end.y ? s.begin.y : s.end.y;
        pMax.x = s.begin.x >= s.end.x ? s.begin.x : s.end.x;
        pMax.y = s.begin.y >= s.end.y ? s.begin.y : s.end.y;
    }

    template<class T>
    AABB2<T>::AABB2()
    {
    }

    template<class T>
    bool AABBIntersects(AABB2<T> const& a, AABB2<T> const& b)
    {
        // RVA 0x83CED0
        return geom1d::SegmentsIntersect(
                   geom1d::Segment1<T>(a.pMin.x, a.pMax.x), geom1d::Segment1<T>(b.pMin.x, b.pMax.x)) &&
            geom1d::SegmentsIntersect(geom1d::Segment1<T>(a.pMin.y, a.pMax.y), geom1d::Segment1<T>(b.pMin.y, b.pMax.y));
    }

    template<class T>
    bool SegmentsIntersect(Segment2<T> const& seg1, Segment2<T> const& seg2)
    {
        // RVA 0x83DBB0 - the boxes must overlap, and each segment's ends must lie on opposite sides
        // of (or on) the other segment's line.
        if (!AABBIntersects(AABB2<T>(seg1), AABB2<T>(seg2)))
        {
            return false;
        }

        Line2<T> const line1(seg1.begin, seg1.end);
        T const w1 = line1.pointWeight(seg2.begin);
        T const w2 = line1.pointWeight(seg2.end);
        if (!((w1 <= EPS && w2 >= -EPS) || (w1 >= -EPS && w2 <= EPS)))
        {
            return false;
        }

        Line2<T> const line2(seg2.begin, seg2.end);
        T const w3 = line2.pointWeight(seg1.begin);
        T const w4 = line2.pointWeight(seg1.end);
        return (w3 <= EPS && w4 >= -EPS) || (w3 >= -EPS && w4 <= EPS);
    }

    template class Point2<float>;
    template class Line2<float>;
    template class Polygon2<float>;
    template class Segment2<float>;
    template class AABB2<float>;
    template bool AABBIntersects<float>(AABB2<float> const&, AABB2<float> const&);
    template bool SegmentsIntersect<float>(Segment2<float> const&, Segment2<float> const&);
}  // namespace geom2d
