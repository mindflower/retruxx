#pragma once
#include <vector>

namespace geom1d
{
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

namespace geom2d
{
    template<class T>
    class Point2
    {
    public:
        /* 0x0000 */ T x;
        /* 0x0004 */ T y;
        Point2(T x_, T y_);
        Point2();
        bool operator==(Point2<T> const& point) const;
        bool operator!=(Point2<T> const& point) const;
    }; /* size: 0x0008 */

    template<class T>
    class Line2
    {
    public:
        /* 0x0000 */ T A;
        /* 0x0004 */ T B;
        /* 0x0008 */ T C;
        Line2(Point2<T> const& _begin, Point2<T> const& _end);
        Line2();
        void Normalize();
        T pointWeight(Point2<T> const& point) const;
        T signedDist(Point2<T> const& point) const;
        T Dist(Point2<T> const& point) const;
        bool isPointOn(Point2<T> const& point) const;
    }; /* size: 0x000c */

    template<class T>
    class Polygon2
    {
    public:
        /* 0x0000 */ std::vector<Point2<T>> m_Points;
        std::vector<Point2<T>>& getPoints();
        std::vector<Point2<T>> const& getPoints() const;
        bool isPointInside(Point2<T> const& point);
    }; /* size: 0x0010 */

    template<class T>
    class Segment2
    {
    public:
        /* 0x0000 */ Point2<T> begin;
        /* 0x0008 */ Point2<T> end;
        Segment2(Point2<T> const& _begin, Point2<T> const& _end);
        Segment2();
        bool isPointOn(Point2<T> const& Point) const;
    }; /* size: 0x0010 */

    template<class T>
    class AABB2
    {
    public:
        /* 0x0000 */ Point2<T> pMin;
        /* 0x0008 */ Point2<T> pMax;
        AABB2(Segment2<T> const& s);
        AABB2();
    }; /* size: 0x0010 */

    template<class T>
    bool AABBIntersects(AABB2<T> const& a, AABB2<T> const& b);

    template<class T>
    bool SegmentsIntersect(Segment2<T> const& seg1, Segment2<T> const& seg2);
}  // namespace geom2d
