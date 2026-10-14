#pragma once
#include "vector.h"

class Segment
{
private:
    CVector m_begin;
    CVector m_end;

public:
    Segment(Segment const&);
    Segment(CVector const& _begin, CVector const& _end);
    Segment();
    float length() const;
    float lengthSq() const;
    CVector& begin();
    CVector const& begin() const;
    CVector& end();
    CVector const& end() const;
    CVector const getDirection() const;
};
