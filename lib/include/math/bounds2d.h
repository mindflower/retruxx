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
    BoundsBase(int);
    BoundsBase(BoundsBase<T> const&);
    BoundsBase(T, T);
    BoundsBase() = default;
    BoundsBase(T, T, T, T);
    BoundsBase(PointBase<T> const&, PointBase<T> const&);
    BoundsBase<T> Intersect(BoundsBase<T> const&) const;
    PointBase<T> TopRight() const;

public:
    T x0;
    T y0;
    T width;
    T height;
};

template <class T>
BoundsBase<T> BoundsBase<T>::SizeRect() const
{
    // RVA 0x437650 - the extents moved to the origin.
    BoundsBase<T> res;
    res.x0 = static_cast<T>(0);
    res.y0 = static_cast<T>(0);
    res.width = width;
    res.height = height;
    return res;
}

template <class T>
int BoundsBase<T>::IsPtInBounds(PointBase<T> const& p) const
{
    // RVA 0x4450B0 (float) / 0x45BC10 (int). Left/top inclusive, right/bottom
    // exclusive, and the far edges are computed as origin + extent.
    return p.x >= x0 && (width + x0) > p.x && p.y >= y0 && (height + y0) > p.y;
}

template <class T>
T BoundsBase<T>::Height() const
{
    // RVA 0x435530
    return height;
}

template <class T>
T BoundsBase<T>::Width() const
{
    // RVA 0x437640
    return width;
}

template <class T>
PointBase<T> BoundsBase<T>::BottomLeft() const
{
    // RVA 0x4E5A80
    return PointBase<T>(x0, height + y0);
}

template <class T>
void BoundsBase<T>::Inflate(T a, T b)
{
    // RVA 0x43F580 (float) / 0x6B9040 (int) - grows on all four sides.
    x0 = x0 - a;
    y0 = y0 - b;
    width = width + a * static_cast<T>(2);
    height = height + b * static_cast<T>(2);
}

template <class T>
T BoundsBase<T>::Bottom() const
{
    // RVA 0x4355A0
    return height + y0;
}

template <class T>
T BoundsBase<T>::Top() const
{
    // RVA 0x41CF90
    return y0;
}

template <class T>
int BoundsBase<T>::Empty() const
{
    // ExMachina 1.02 NoCD RVA 0x491C50 (the float instantiation emitted for
    // ZnayuKakProdatWnd::MakeBottomPanel). The shipped code really does compute
    // (x0 + width) - x0 and y0 - (y0 + height) rather than just the extents, so
    // the rounding of those expressions is reproduced here. Note that the second
    // term is negated relative to the first, which is why an empty rect compares
    // equal to zero on both.
    return static_cast<T>((this->width + this->x0) - this->x0) == static_cast<T>(0)
        && static_cast<T>(this->y0 - (this->height + this->y0)) == static_cast<T>(0);
}

template <class T>
void BoundsBase<T>::Offset(PointBase<T> const& p)
{
    // RVA 0x440E30 (float) / 0x6B9020 (int) - moves the origin, keeps extents.
    x0 = p.x + x0;
    y0 = p.y + y0;
}

template <class T>
void BoundsBase<T>::Zero()
{
    // RVA 0x673AE0
    x0 = static_cast<T>(0);
    y0 = static_cast<T>(0);
    width = static_cast<T>(0);
    height = static_cast<T>(0);
}

template <class T>
T BoundsBase<T>::Right() const
{
    // RVA 0x43FEA0
    return width + x0;
}

template <class T>
T BoundsBase<T>::CenterX() const
{
    // RVA 0x4DCF20 (float: (x0 * 2 + width) * 0.5) / 0x584B00 (int: the same
    // expression with a signed halve, so integer bounds truncate toward zero).
    return (x0 + x0 + width) / static_cast<T>(2);
}

template <class T>
T BoundsBase<T>::Left() const
{
    // RVA 0x41CFA0
    return x0;
}

template <class T>
void BoundsBase<T>::CenterIn(BoundsBase<T> const& rc)
{
    // RVA 0x4B7290. NOTE: the shipped code centres against rc's EXTENTS only -
    // rc.x0 and rc.y0 are never added - so the result is relative to rc's
    // origin. Callers pass a client rect, which starts at the origin anyway.
    x0 = (rc.width - width) / static_cast<T>(2);
    y0 = (rc.height - height) / static_cast<T>(2);
}

template <class T>
PointBase<T> BoundsBase<T>::BottomRight() const
{
    // RVA 0x445060
    return PointBase<T>(width + x0, height + y0);
}

template <class T>
T BoundsBase<T>::CenterY() const
{
    // RVA 0x4375F0 (float) / 0x584B10 (int)
    return (y0 + y0 + height) / static_cast<T>(2);
}

template <class T>
PointBase<T> BoundsBase<T>::TopLeft() const
{
    // RVA 0x41CD70
    return PointBase<T>(x0, y0);
}

template <class T>
BoundsBase<T>::BoundsBase(int)
{
    // RVA 0x611910 - the argument is ignored; everything zeroes.
    x0 = static_cast<T>(0);
    y0 = static_cast<T>(0);
    width = static_cast<T>(0);
    height = static_cast<T>(0);
}

template <class T>
BoundsBase<T>::BoundsBase(BoundsBase<T> const& rhs) :
    x0(rhs.x0),
    y0(rhs.y0),
    width(rhs.width),
    height(rhs.height)
{
}

template <class T>
BoundsBase<T>::BoundsBase(T a, T b)
{
    this->x0 = 0.0;
    this->y0 = 0.0;
    this->width = a;
    this->height = b;
}

template <class T>
BoundsBase<T>::BoundsBase(T x, T y, T xx, T yy)
{
    x0 = x;
    y0 = y;
    width = xx - x;
    height = yy - y;
}

template <class T>
BoundsBase<T>::BoundsBase(PointBase<T> const& topLeft, PointBase<T> const& bottomRight)
{
    // RVA 0x445080 - corners in, extents stored.
    x0 = topLeft.x;
    y0 = topLeft.y;
    width = bottomRight.x - x0;
    height = bottomRight.y - y0;
}

template <class T>
BoundsBase<T> BoundsBase<T>::Intersect(BoundsBase<T> const& b) const
{
    // RVA 0x46E3A0 - the overlap of the two rectangles; all zero when they do not touch.
    T const bx1 = b.width + b.x0;
    T const x1 = width + x0;
    T const by1 = b.height + b.y0;
    T const y1 = height + y0;
    BoundsBase<T> result;
    if (x0 > bx1 || b.x0 > x1 || y0 > by1 || b.y0 > y1)
    {
        result.x0 = 0.0;
        result.y0 = 0.0;
        result.width = 0.0;
        result.height = 0.0;
        return result;
    }
    T const left = x0 <= b.x0 ? b.x0 : x0;
    T const right = bx1 <= x1 ? bx1 : x1;
    T const top = y0 <= b.y0 ? b.y0 : y0;
    T const bottom = by1 <= y1 ? by1 : y1;
    result.x0 = left;
    result.y0 = top;
    result.width = right - left;
    result.height = bottom - top;
    return result;
}

template <class T>
PointBase<T> BoundsBase<T>::TopRight() const
{
    // RVA 0x4E5A60
    return PointBase<T>(width + x0, y0);
}
