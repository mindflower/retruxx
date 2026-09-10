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
    RETRUXX_NOT_IMPLEMENTED;
}

template <class T>
int BoundsBase<T>::IsPtInBounds(PointBase<T> const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

template <class T>
T BoundsBase<T>::Height() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

template <class T>
T BoundsBase<T>::Width() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

template <class T>
PointBase<T> BoundsBase<T>::BottomLeft() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

template <class T>
void BoundsBase<T>::Inflate(T, T)
{
    RETRUXX_NOT_IMPLEMENTED;
}

template <class T>
T BoundsBase<T>::Bottom() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

template <class T>
T BoundsBase<T>::Top() const
{
    RETRUXX_NOT_IMPLEMENTED;
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
void BoundsBase<T>::Offset(PointBase<T> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

template <class T>
void BoundsBase<T>::Zero()
{
    RETRUXX_NOT_IMPLEMENTED;
}

template <class T>
T BoundsBase<T>::Right() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

template <class T>
T BoundsBase<T>::CenterX() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

template <class T>
T BoundsBase<T>::Left() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

template <class T>
void BoundsBase<T>::CenterIn(BoundsBase<T> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

template <class T>
PointBase<T> BoundsBase<T>::BottomRight() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

template <class T>
T BoundsBase<T>::CenterY() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

template <class T>
PointBase<T> BoundsBase<T>::TopLeft() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

template <class T>
BoundsBase<T>::BoundsBase(int)
{
    RETRUXX_NOT_IMPLEMENTED;
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
BoundsBase<T>::BoundsBase(PointBase<T> const&, PointBase<T> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

template <class T>
BoundsBase<T> BoundsBase<T>::Intersect(BoundsBase<T> const& b) const
{
    //TODO: check this and refactor
    T v3; // xmm2_4
    T v4; // xmm1_4
    T v5; // xmm4_4
    T v6; // xmm0_4
    T v7; // xmm3_4
    T v8; // xmm2_4
    BoundsBase<T> result; // eax

    v3 = b.width + b.x0;
    if (this->x0 > v3
        || (v4 = this->width + this->x0, b.x0 > v4)
        || (v5 = b.height + b.y0, this->y0 > v5)
        || (v6 = this->height + this->y0, b.y0 > v6))
    {
        result.x0 = 0.0;
        result.y0 = 0.0;
        result.width = 0.0;
        result.height = 0.0;
    }
    else
    {
        v7 = this->x0;
        if (this->x0 <= b.x0)
            v7 = b.x0;
        if (v3 <= v4)
            v4 = b.width + b.x0;
        v8 = this->y0;
        if (v8 <= b.y0)
            v8 = b.y0;
        if (v5 <= v6)
            v6 = b.height + b.y0;
        result.x0 = v7;
        result.y0 = v8;
        result.width = v4 - v7;
        result.height = v6 - v8;
    }
    return result;
}

template <class T>
PointBase<T> BoundsBase<T>::TopRight() const
{
    RETRUXX_NOT_IMPLEMENTED;
}
