#include <core/stringm3d.h>
#include "thirdparty/stl/vector.hpp"

namespace ai
{
    class Path;

    void StrToStringVector(CStr const&, oldstd::vector<CStr>&);
    void StrToFloatVector(CStr const&, oldstd::vector<float>&);
    void StrToIntVector(CStr const&, oldstd::vector<int>&);
    bool GetPathItem(Path const*, unsigned int, CVector&);
    void DebugCircle(CVector const&, float, unsigned int);
}
