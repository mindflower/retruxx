#pragma once

template<class T>
class PointBase
{
public:
    PointBase() = default;

    PointBase(T xx, T yy) :
        x(xx),
        y(yy)
    {
    }
    PointBase<T>& operator+=(PointBase<T> const&);
    PointBase<T> operator-() const;

public:
    T x{};
    T y{};
};
