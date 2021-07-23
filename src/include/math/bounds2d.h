#pragma once

template<class T>
class BoundsBase
{
public:
    BoundsBase SizeRect() const;
    int IsPtInBounds(PointBase<T> const&) const;
    T Height() const;
    T Width() const;
    PointBase<T> BottomLeft() const;
    void Inflate(T, T);
    T Bottom() const;
    T Top() const;
    int Empty() const;
    void Offset(PointBase<T> const&);
    void Zero();
    T Right() const;
    T CenterX() const;
    T Left() const;
    void CenterIn(BoundsBase<T> const&);
    PointBase<T> BottomRight() const;
    T CenterY() const;
    PointBase<T> TopLeft() const;
    ~BoundsBase();
    BoundsBase(int);
    BoundsBase(BoundsBase<T> const&);
    BoundsBase(T, T);
    BoundsBase();
    BoundsBase(T, T, T, T);
    BoundsBase(PointBase<T> const&, PointBase<T> const&);
    BoundsBase<T> Intersect(BoundsBase<T> const&) const;
    PointBase<T> TopRight() const;

private:
    T x0;
    T y0;
    T width;
    T height;
};
