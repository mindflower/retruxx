#include "utils.h"
#include "path.h"
#include <sstream>

#include "landscape.h"
#include "math/vector.h"
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

    bool GetPathItem(Path const* pPath, unsigned int itemNum, CVector& point)
    {
        if (!pPath || pPath->GetSearchStatus())
        {
            return false;
        }
        pPath->GetItem(itemNum, &point.x, &point.z);
        return true;
    }

    void DebugCircle(CVector const&, float, unsigned int)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
    PointBase<float> clampIntoLandscape(PointBase<float> const& point)
    {
        auto result = point;
        auto v4 = pServer->GetLevelSize() - 5.0;
        if (result.x < 5.0)
            result.x = 5.0;
        if (result.x > v4)
            result.x = v4;
        auto v5 = ai::pServer->GetLevelSize() - 5.0;
        if (result.y < 5.0)
            result.y = 5.0;
        if (result.y > v5)
            result.y = v5;
        return result;
    }
}  // namespace ai
