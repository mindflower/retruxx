#include "utils.h"
#include "path.h"
#include <sstream>
#include <stdexcept>
#include "thirdparty/injecttools.h"

namespace ai
{
    void StrToStringVector(CStr const& str, oldstd::vector<CStr>& stringVector)
    {
        //TODO: check this
        stringVector.clear();
        if (!str.empty())
        {
            std::istringstream ss(str.c_str());
            std::string s;
            while(getline(ss, s, ' '))
            {
                stringVector.push_back(s.c_str());
            }
        }
    }

    void StrToFloatVector(CStr const& str, oldstd::vector<float>& floatVector)
    {
        //TODO: check this
        floatVector.clear();
        if (!str.empty())
        {
            std::istringstream ss(str.c_str());
            std::string s;
            while (getline(ss, s, ' '))
            {
                floatVector.push_back(atof(s.c_str()));
            }
        }
    }

    void StrToIntVector(CStr const& str, oldstd::vector<int>& intVector)
    {
        //TODO: check this
        intVector.clear();
        if (!str.empty())
        {
            std::istringstream ss(str.c_str());
            std::string s;
            while (getline(ss, s, ' '))
            {
                intVector.push_back(atoi(s.c_str()));
            }
        }
    }

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION(0x006A9D60, GetPathItem)
    bool GetPathItem(Path const*, unsigned int, CVector&)
    {
        throw std::logic_error("Not implemented");
    }

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION(0x006AAB20, DebugCircle)
    void DebugCircle(CVector const&, float, unsigned int)
    {
    }
}
