#pragma once

template<class T>
class PointBase
{
public:
    PointBase(T, T);
    PointBase(PointBase<T> const&);
    PointBase();
    PointBase<T>& operator+=(PointBase<T> const&);
    PointBase<T> const& operator=(PointBase<T> const&);
    PointBase<T> operator-() const;

private:
    float x;
    float y;
};
