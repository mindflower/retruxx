#define _USE_MATH_DEFINES
#include "utils.h"

#include "config.h"
#include "path.h"
#include <sstream>

#include "landscape.h"
#include "m3dapp.h"
#include "math/vector.h"
#include "server.h"
#include "world.h"
#include "core/kernel.h"
#include <cmath>

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
            while (getline(ss, s, ' '))
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

    CStr StringVectorToStr(retruxx::vector<CStr> const& stringVector)
    {
        // The inverse of StrToStringVector: a single space-separated run.
        CStr res;
        for (size_t i = 0; i < stringVector.size(); ++i)
        {
            if (i != 0)
            {
                res += CStr(" ");
            }
            res += stringVector[i];
        }
        return res;
    }

    CStr IntVectorToStr(retruxx::vector<int> const& intVector)
    {
        CStr res;
        for (size_t i = 0; i < intVector.size(); ++i)
        {
            if (i != 0)
            {
                res += CStr(" ");
            }
            res += CStr(intVector[i]);
        }
        return res;
    }

    CStr FloatVectorToStr(retruxx::vector<float> const& floatVector)
    {
        CStr res;
        for (size_t i = 0; i < floatVector.size(); ++i)
        {
            if (i != 0)
            {
                res += CStr(" ");
            }
            res += CStr(floatVector[i]);
        }
        return res;
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

    void DebugCircle(CVector const& center, float radius, unsigned int color)
    {
        // TODO: generated code DebugCircle
        int const SEGMENTS = 40;                          // Number of line segments to approximate circle
        float const ANGLE_STEP = 2.0f * M_PI / SEGMENTS;  // Angle between segments

        // Start with the first point (at angle 0)
        CVector previousPoint;
        previousPoint.x = center.x + radius;  // cos(0) = 1, sin(0) = 0
        previousPoint.y = center.y;
        previousPoint.z = center.z;

        // Draw circle using line segments
        for (int i = 1; i <= SEGMENTS; ++i)
        {
            float angle = i * ANGLE_STEP;
            float cosAngle = cos(angle);
            float sinAngle = sin(angle);

            // Calculate current point on circle
            CVector currentPoint;
            currentPoint.x = center.x + (cosAngle * radius);
            currentPoint.y = center.y;
            currentPoint.z = center.z + (sinAngle * radius);  // Circle in XZ plane

            // Draw line segment from previous point to current point
            ai::DebugLineOnGround(currentPoint, previousPoint, 0.5, color);

            // Current point becomes previous point for next iteration
            previousPoint = currentPoint;
        }
    }

    int DebugText(CVector const&, float, float, unsigned int, CStr const&)
    {
        // TODO: implement DebugText
        // RETRUXX_NOT_IMPLEMENTED;
        return 1;
    }

    void DebugLine(CVector const& pp1, CVector const& pp2, unsigned int color)
    {
        M3D_APP->DrawLine(pp1, pp2, color);
    }

    void DebugLineOnGround(CVector const& p1, CVector const& p2, float hover, unsigned int color)
    {
        // TODO: check and refactor
        CVector pp1;
        CVector pp2;
        auto x = p1.x;
        auto y = p1.y;
        pp1.z = p1.z;
        auto z = p2.z;
        pp1.x = x;
        auto v7 = p2.x;
        pp1.y = y;
        auto v8 = p2.y;
        pp2.z = z;
        pp2.x = v7;
        pp2.y = v8;

        auto v10 = M3D_ENGINE_CFG.GetHeight(pp1.x, pp1.z);
        pp1.y = v10 + hover;
        auto v12 = M3D_ENGINE_CFG.GetHeight(pp2.x, pp2.z);
        auto v13 = v12 + hover;
        pp2.y = v13;

        M3D_RENDERER->SetTexture(0, {}, -1.0);
        M3D_APP->DrawLine(pp1, pp2, color);
    }

    CVector GetGroundPos(CVector const& pos, bool withCollisions, bool forVehicle)
    {
        CVector res = pos;
        if (!withCollisions)
        {
            res.y = ai::pServer->GetWorld()->GetLandscape().GetHeight(pos.x, pos.z, -1, 1);
        }
        else
        {
            res.y = ai::pServer->GetWorld()->GetLandscape().GetHeightWithCollisions(pos.x, pos.z, forVehicle);
        }
        return res;
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
