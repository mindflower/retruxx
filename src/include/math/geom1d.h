#pragma once

namespace geom1d
{
    //TODO: add static functions

    template<class T>
    class Segment1
    {
    public:
        Segment1(T const&, T const&);
        bool isPointOn(T const&) const;

    private:
        T begin;
        T end;
    };
}
