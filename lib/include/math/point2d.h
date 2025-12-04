#pragma once

template<class T>
class PointBase
{
public:
    T x{};
    T y{};

    PointBase(T a, T b) :
        x(a),
        y(b)
    {
    }

    PointBase() = default;

    PointBase<T>& operator+=(const PointBase<T>& rhs);
    PointBase<T>& operator-=(const PointBase<T>&);
    PointBase<T> operator-() const;
};
