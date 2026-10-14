#pragma once
#include "vector.h"

class Segment
{
private:
    CVector m_begin;
    CVector m_end;

public:
    Segment(const Segment&);
    Segment(const CVector& _begin, const CVector& _end);
    Segment();
    float length() const;
    float lengthSq() const;
    CVector& begin();
    const CVector& begin() const;
    CVector& end();
    const CVector& end() const;
    const CVector getDirection() const;
};
