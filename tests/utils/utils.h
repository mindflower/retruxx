#pragma once

struct CVector;
struct CMatrix;

namespace utils
{
    bool FloatEquals(float a, float b, float epsilon = 1e-6f);
    bool VectorsEqual(CVector const& a, CVector const& b);
    bool MatrixEquals(CMatrix const& a, CMatrix const& b);
}  // namespace utils
