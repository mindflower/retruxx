#include "utils.h"
#include "path.h"
#include <sstream>

#include "landscape.h"
#include "math/vector.h"
#include "thirdparty/injecttools.h"
#include "server.h"
#include "world.h"

namespace ai
{
    void StrToStringVector(CStr const& str, retruxx::vector<CStr>& stringVector)
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

    void StrToFloatVector(CStr const& str, retruxx::vector<float>& floatVector)
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

    void StrToIntVector(CStr const& str, retruxx::vector<int>& intVector)
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION(0x006AAB20, DebugCircle)
    void DebugCircle(CVector const&, float, unsigned int)
    {
    }

    CVector GetGroundPos(CVector const& pos, bool withCollisions, bool forVehicle)
    {
        CVector a1;
        auto m_pWorld = ai::pServer->GetWorld();
        auto withCollisionsa = pos.z;
        if (!withCollisions)
        {
            auto Height = m_pWorld->GetLandscape().GetHeight(pos.x, pos.z, -1, 1);
            auto x = pos.x;
            a1.y = Height;
            a1.x = x;
        }
        else
        {
            auto HeightWithCollisions = m_pWorld->GetLandscape().GetHeightWithCollisions(pos.x, pos.z, forVehicle);
            auto v9 = pos.x;
            a1.y = HeightWithCollisions;
            a1.x = v9;
        }
        a1.z = withCollisionsa;
        return a1;
    }

    CVector GetGroundPos(CVector2 const&, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
