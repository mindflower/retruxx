#include "utils.h"
#include <sstream>
#include <stdexcept>

namespace ai
{
    void StrToStringVector(CStr const& str, std::vector<CStr>& stringVector)
    {
        //TODO: check this
        stringVector.clear();
        if (!str.empty())
        {
            std::istringstream ss(str.c_str());
            std::string s;
            while(getline(ss, s, ' '))
            {
                stringVector.emplace_back(s.c_str());
            }
        }
    }

    void StrToFloatVector(CStr const& str, std::vector<float>& floatVector)
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
}
