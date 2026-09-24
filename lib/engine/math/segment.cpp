#include "math/segment.h"

#include <cmath>

// NOTE: the default and copy constructors, lengthSq and the non-const begin/end are declared in
// the PDB but never instantiated in the shipped binary; their bodies are the obvious ones.

Segment::Segment(Segment const& other) : m_begin(other.m_begin), m_end(other.m_end)
{
}

Segment::Segment(CVector const& _begin, CVector const& _end) : m_begin(_begin), m_end(_end)
{
    // RVA 0x7E0B60
}

Segment::Segment()
{
}

float Segment::length() const
{
    // RVA 0x602500
    double const dy = double(m_end.y) - m_begin.y;
    double const dz = double(m_end.z) - m_begin.z;
    double const dx = double(m_end.x) - m_begin.x;
    return static_cast<float>(std::sqrt(dz * dz + dx * dx + dy * dy));
}

float Segment::lengthSq() const
{
    float const dx = m_end.x - m_begin.x;
    float const dy = m_end.y - m_begin.y;
    float const dz = m_end.z - m_begin.z;
    return dx * dx + dy * dy + dz * dz;
}

CVector& Segment::begin()
{
    return m_begin;
}

CVector const& Segment::begin() const
{
    // RVA 0x602530
    return m_begin;
}

CVector& Segment::end()
{
    return m_end;
}

CVector const& Segment::end() const
{
    // RVA 0x7E0B90
    return m_end;
}

CVector const Segment::getDirection() const
{
    // RVA 0x602540 - not normalized.
    return CVector(m_end.x - m_begin.x, m_end.y - m_begin.y, m_end.z - m_begin.z);
}
