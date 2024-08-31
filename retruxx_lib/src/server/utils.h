#include <core/stringm3d.h>
#include <vector>

namespace ai
{
    class Path;

    void StrToStringVector(CStr const&, std::vector<CStr>&);
    void StrToFloatVector(CStr const&, std::vector<float>&);
    void StrToIntVector(CStr const&, std::vector<int>&);
    bool GetPathItem(Path const*, unsigned int, CVector&);
    void DebugCircle(CVector const&, float, unsigned int);
}
