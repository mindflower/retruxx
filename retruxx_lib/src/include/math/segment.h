#pragma once
#include "vector.h"

class Segment
{
public:
    Segment(CVector const&, CVector const&);
    CVector const& begin() const;
    CVector const getDirection() const;
    float length() const;
    CVector const& end() const;

private:
    CVector m_begin;
    CVector m_end;
};
