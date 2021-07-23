#pragma once
#include <vector>

namespace geom2d
{
    template<class T>
    class Point2
    {
    public:
        bool operator!=(Point2<T> const&) const;
        Point2();
        Point2(T, T);
        bool operator==(Point2<T> const&) const;

    private:
        T x;
        T y;
    };

    template<class T>
    class Line2
    {
    public:
        T Dist(Point2<T> const&) const;
        void Normalize();
        T signedDist(Point2<T> const&) const;
        Line2(Point2<T> const&, Point2<T> const&);
        bool isPointOn(Point2<T> const&) const;
        T pointWeight(Point2<T> const&) const;

    private:
        float A;
        float B;
        float C;
    };

    template<class T>
    class Polygon2
    {
    public:
        std::vector<Point2<T>> const& getPoints() const;
        std::vector<Point2<T>>& getPoints();
        bool isPointInside(Point2<T> const&);

    private:
        std::vector<Point2<T>> m_Points;
    };

    template<class T>
    class Segment2
    {
    public:
        Segment2(Point2<T> const&, Point2<T> const&);
        bool isPointOn(Point2<T> const&) const;

    private:
        Point2<T> begin;
        Point2<T> end;
    };

    template<class T>
    class AABB2
    {
    public:
        AABB2(Segment2<T> const&);

    private:
        Point2<T> pMin;
        Point2<T> pMax;
    };
}
