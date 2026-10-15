#pragma once
#include <vector>

#include "geom1d.h"

namespace geom2d
{
    template<class T>
    class Point2
    {
    public:
        T x;
        T y;
        Point2(T x_, T y_);
        Point2();
        bool operator==(Point2<T> const& point) const;
        bool operator!=(Point2<T> const& point) const;
    };

    template<class T>
    class Line2
    {
    public:
        T A;
        T B;
        T C;
        Line2(Point2<T> const& _begin, Point2<T> const& _end);
        Line2();
        void Normalize();
        T pointWeight(Point2<T> const& point) const;
        T signedDist(Point2<T> const& point) const;
        T Dist(Point2<T> const& point) const;
        bool isPointOn(Point2<T> const& point) const;
    };

    template<class T>
    class Polygon2
    {
    public:
        std::vector<Point2<T>> m_Points;
        std::vector<Point2<T>>& getPoints();
        std::vector<Point2<T>> const& getPoints() const;
        bool isPointInside(Point2<T> const& point);
    };

    template<class T>
    class Segment2
    {
    public:
        Point2<T> begin;
        Point2<T> end;
        Segment2(Point2<T> const& _begin, Point2<T> const& _end);
        Segment2();
        bool isPointOn(Point2<T> const& Point) const;
    };

    template<class T>
    class AABB2
    {
    public:
        Point2<T> pMin;
        Point2<T> pMax;
        AABB2(Segment2<T> const& s);
        AABB2();
    };

    template<class T>
    bool AABBIntersects(AABB2<T> const& a, AABB2<T> const& b);

    template<class T>
    bool SegmentsIntersect(Segment2<T> const& seg1, Segment2<T> const& seg2);
}  // namespace geom2d
