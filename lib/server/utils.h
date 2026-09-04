#include "math/point2d.h"

#include <core/stringm3d.h>
#include "retruxx/common.h"

namespace ai
{
    class Path;

    void StrToStringVector(CStr const&, retruxx::vector<CStr>&);
    void StrToFloatVector(CStr const&, retruxx::vector<float>&);
    void StrToIntVector(CStr const&, retruxx::vector<int>&);
    CStr StringVectorToStr(retruxx::vector<CStr> const&);
    CStr IntVectorToStr(retruxx::vector<int> const&);
    CStr FloatVectorToStr(retruxx::vector<float> const&);
    bool GetPathItem(Path const*, unsigned int, CVector&);
    void DebugCircle(CVector const&, float, unsigned int);
    int DebugText(CVector const&, float, float, unsigned int, CStr const&);
    int DebugText(CVector const&, float, float, unsigned int, CStr const&);
    void DebugLine(CVector const&, CVector const&, unsigned int);
    void DebugLineOnGround(CVector const&, CVector const&, float, unsigned int);
    CVector GetGroundPos(CVector const&, bool, bool);
    CVector GetGroundPos(CVector2 const&, bool);
    PointBase<float> clampIntoLandscape(PointBase<float> const&);
}
