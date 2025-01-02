#include <core/stringm3d.h>
#include "thirdparty/containers.h"

namespace ai
{
    class Path;

    void StrToStringVector(CStr const&, retruxx::vector<CStr>&);
    void StrToFloatVector(CStr const&, retruxx::vector<float>&);
    void StrToIntVector(CStr const&, retruxx::vector<int>&);
    bool GetPathItem(Path const*, unsigned int, CVector&);
    void DebugCircle(CVector const&, float, unsigned int);
}
