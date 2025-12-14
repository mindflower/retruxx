#include "utils.h"
#include "math/vector.h"
#include "math/matrix.h"
#include <cmath>

namespace utils
{
    bool FloatEquals(float const a, float const b, float const epsilon)
    {
        return std::abs(a - b) < epsilon;
    }

    bool VectorsEqual(CVector const& a, CVector const& b)
    {
        return FloatEquals(a.x, b.x) && FloatEquals(a.y, b.y) && FloatEquals(a.z, b.z);
    }

    bool MatrixEquals(CMatrix const& a, CMatrix const& b)
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                if (!FloatEquals(a.m[i][j], b.m[i][j]))
                {
                    return false;
                }
            }
        }
        return true;
    }
}  // namespace utils
