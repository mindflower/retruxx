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

public:
    T x0;
    T y0;
    T width;
    T height;
};

template <class T>
BoundsBase<T> BoundsBase<T>::SizeRect() const
{
    throw std::logic_error("Not implemented");
}

template <class T>
int BoundsBase<T>::IsPtInBounds(PointBase<T> const&) const
{
    throw std::logic_error("Not implemented");
}

template <class T>
T BoundsBase<T>::Height() const
{
    throw std::logic_error("Not implemented");
}

template <class T>
T BoundsBase<T>::Width() const
{
    throw std::logic_error("Not implemented");
}

template <class T>
PointBase<T> BoundsBase<T>::BottomLeft() const
{
    throw std::logic_error("Not implemented");
}

template <class T>
void BoundsBase<T>::Inflate(T, T)
{
    throw std::logic_error("Not implemented");
}

template <class T>
T BoundsBase<T>::Bottom() const
{
    throw std::logic_error("Not implemented");
}

template <class T>
T BoundsBase<T>::Top() const
{
    throw std::logic_error("Not implemented");
}

template <class T>
int BoundsBase<T>::Empty() const
{
    throw std::logic_error("Not implemented");
}

template <class T>
void BoundsBase<T>::Offset(PointBase<T> const&)
{
    throw std::logic_error("Not implemented");
}

template <class T>
void BoundsBase<T>::Zero()
{
    throw std::logic_error("Not implemented");
}

template <class T>
T BoundsBase<T>::Right() const
{
    throw std::logic_error("Not implemented");
}

template <class T>
T BoundsBase<T>::CenterX() const
{
    throw std::logic_error("Not implemented");
}

template <class T>
T BoundsBase<T>::Left() const
{
    throw std::logic_error("Not implemented");
}

template <class T>
void BoundsBase<T>::CenterIn(BoundsBase<T> const&)
{
    throw std::logic_error("Not implemented");
}

template <class T>
PointBase<T> BoundsBase<T>::BottomRight() const
{
    throw std::logic_error("Not implemented");
}

template <class T>
T BoundsBase<T>::CenterY() const
{
    throw std::logic_error("Not implemented");
}

template <class T>
PointBase<T> BoundsBase<T>::TopLeft() const
{
    throw std::logic_error("Not implemented");
}

template <class T>
BoundsBase<T>::~BoundsBase()
{
    throw std::logic_error("Not implemented");
}

template <class T>
BoundsBase<T>::BoundsBase(int)
{
    throw std::logic_error("Not implemented");
}

template <class T>
BoundsBase<T>::BoundsBase(BoundsBase<T> const&)
{
    throw std::logic_error("Not implemented");
}

template <class T>
BoundsBase<T>::BoundsBase(T, T)
{
    throw std::logic_error("Not implemented");
}

template <class T>
BoundsBase<T>::BoundsBase()
{
    throw std::logic_error("Not implemented");
}

template <class T>
BoundsBase<T>::BoundsBase(T, T, T, T)
{
    throw std::logic_error("Not implemented");
}

template <class T>
BoundsBase<T>::BoundsBase(PointBase<T> const&, PointBase<T> const&)
{
    throw std::logic_error("Not implemented");
}

template <class T>
BoundsBase<T> BoundsBase<T>::Intersect(BoundsBase<T> const&) const
{
    throw std::logic_error("Not implemented");
}

template <class T>
PointBase<T> BoundsBase<T>::TopRight() const
{
    throw std::logic_error("Not implemented");
}
