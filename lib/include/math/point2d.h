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

// RVA 0x4B3770 (float) / 0x59C800 (int)
template <class T>
PointBase<T>& PointBase<T>::operator+=(const PointBase<T>& rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}

// No instantiation of this one survives in the shipped binary; it mirrors
// operator+= component for component.
template <class T>
PointBase<T>& PointBase<T>::operator-=(const PointBase<T>& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

// RVA 0x46E470 (float) / 0x6E9010 (int)
template <class T>
PointBase<T> PointBase<T>::operator-() const
{
    return PointBase<T>(static_cast<T>(0) - x, static_cast<T>(0) - y);
}
