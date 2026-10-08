#pragma once
#include "vector.h"

class Segment
{
private:
    /* 0x0000 */ CVector m_begin;
    /* 0x000c */ CVector m_end;

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
}; /* size: 0x0018 */
