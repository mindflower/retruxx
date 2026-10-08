#define _USE_MATH_DEFINES
#include "utils.h"

#include "config.h"
#include "path.h"
#include <algorithm>
#include <sstream>

#include "landscape.h"
#include "m3dapp.h"
#include "math/vector.h"
#include "server.h"
#include "world.h"
#include "dynamicquestmanager.h"
#include "relationship.h"
#include "objects/base/compositeobj.h"
#include "objects/base/objcontainer.h"
#include "objects/base/prototypemanager.h"
#include "objects/player.h"
#include "objects/town.h"
#include "objects/vehicle.h"
#include "core/kernel.h"
#include <cmath>

namespace ai
{
    namespace
    {
        bool IsFloatChar(char c)
        {
            return (c >= '0' && c <= '9') || c == '.' || c == '-';
        }

        bool IsIntChar(char c)
        {
            return c >= '0' && c <= '9';
        }

        // Shared scan of StrToFloatVector and StrToIntVector (inlined in both in the shipped code): each run of
        // number characters is parsed with sscanf.
        // NOTE: trailing non-number characters after the last number yield one extra 0 (sscanf of an empty
        // string leaves the value at 0), e.g. "1 2 " gives {1, 2, 0}.
        template <typename T>
        void ScanNumbers(CStr const& str, retruxx::vector<T>& numbers, char const* format, bool (*isNumberChar)(char))
        {
            numbers.clear();
            char const* p = str.c_str();
            while (*p)
            {
                while (*p && !isNumberChar(*p))
                {
                    ++p;
                }
                T number = 0;
                sscanf(p, format, &number);
                numbers.push_back(number);

                while (*p && isNumberChar(*p))
                {
                    ++p;
                }
            }
        }
    }  // namespace

    void StrToStringVector(CStr const& str, retruxx::vector<CStr>& stringVector)
    {
        // RVA 0x6AB070 - splits at spaces; runs of spaces count as one separator and leading ones are skipped.
        // NOTE: trailing spaces yield one extra empty string at the end.
        stringVector.clear();
        char const* p = str.c_str();
        while (*p)
        {
            while (*p == ' ')
            {
                ++p;
            }
            char const* const word = p;
            while (*p && *p != ' ')
            {
                ++p;
            }
            stringVector.push_back(CStr(word, static_cast<int>(p - word)));
        }
    }

    void StrToFloatVector(CStr const& str, retruxx::vector<float>& floatVector)
    {
        // RVA 0x6AAF20 - digits, '.' and '-' make up a number; anything else separates.
        ScanNumbers(str, floatVector, "%f", IsFloatChar);
    }

    void StrToIntVector(CStr const& str, retruxx::vector<int>& intVector)
    {
        // RVA 0x6AADB0 - only digits make up a number; anything else separates.
        // NOTE: '-' is a separator here, so negative numbers come out positive.
        ScanNumbers(str, intVector, "%d", IsIntChar);
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
        // RVA 0x6AAB20 - a horizontal circle of 40 segments, drawn on the ground.
        CVector prev(center.x + radius, center.y, center.z);
        for (int i = 1; i <= 40; ++i)
        {
            double const angle = i * static_cast<double>(0.15707964f);  // 2 * pi / 40
            float const c = static_cast<float>(cos(angle));
            float const s = static_cast<float>(sin(angle));
            // NOTE: the shipped code adds radius * 0 to the height (a leftover of a rotated circle).
            CVector const cur(center.x + c * radius, center.y + radius * 0.0f, center.z + s * radius);
            DebugLineOnGround(cur, prev, 0.5f, color);
            prev = cur;
        }
    }

    // RVA 0x6A9E70
    int DebugText(CVector const& p1, float size, float, unsigned int color, CStr const& OutStr)
    {
        // NOTE: the third argument is never read.
        CVector const org = M3D_RENDERER->Project(p1 - M3D_RENDERER->MatGetOrgInv());
        M3D_APP->SetFont(CStr("Tahoma"), size, 0, M3D_APP->m_codePage.CodePage);
        M3D_RENDERER->PushFog(false);
        M3D_APP->DrawTextAbs(org.x, org.y, color, OutStr, 0, -1);
        M3D_RENDERER->PopFog();
        return 1;
    }

    void DebugLine(CVector const& pp1, CVector const& pp2, unsigned int color)
    {
        M3D_APP->DrawLine(pp1, pp2, color);
    }

    void DebugLineOnGround(CVector const& p1, CVector const& p2, float hover, unsigned int color)
    {
        // RVA 0x6AA840 - both ends are put `hover` above the landscape.
        CVector pp1 = p1;
        CVector pp2 = p2;
        pp1.y = M3D_ENGINE_CFG.GetHeight(pp1.x, pp1.z) + hover;
        pp2.y = M3D_ENGINE_CFG.GetHeight(pp2.x, pp2.z) + hover;
        M3D_RENDERER->SetTexture(0, m3d::rend::TexHandle(), -1.0);
        M3D_APP->DrawLine(pp1, pp2, color);
    }

    CVector GetGroundPos(CVector const& pos, bool withCollisions, bool forVehicle)
    {
        // RVA 0x6A9F80
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

    CVector GetGroundPos(CVector2 const& pos, bool withCollisions)
    {
        // RVA 0x6AA000 - the 2D point is (x, z); never uses the vehicle collision mode.
        return GetGroundPos(CVector(pos.x, 0.0f, pos.y), withCollisions, false);
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

    void DecToleranceWhenDamageFromPlayerInflicted(Obj const* victim, float partOfHealth)
    {
        // RVA 0x6AB1C0
        M3D_ASSERT(victim);

        Vehicle* playerVehicle = thePlayer->GetVehicle();
        if (!playerVehicle)
        {
            return;
        }

        int const victimBelong = victim->GetBelong();
        int const playerBelong = playerVehicle->GetBelong();

        eTolerance const before = theRelationship->CheckTolerance(victimBelong, playerBelong);
        theRelationship->IncTolerance(victimBelong, playerBelong, partOfHealth * -4.0f);
        eTolerance const after = theRelationship->CheckTolerance(victimBelong, playerBelong);

        // Only react on the transition into hostility, and only for a clan that was
        // not hostile by default to begin with.
        if (theRelationship->CheckDefaultTolerance(victimBelong, playerBelong) <= RS_ENEMY || before <= RS_ENEMY ||
            after > RS_ENEMY)
        {
            return;
        }

        victim->CauseEvent(GE_RELATION_CHANGED, 0.0f, m3d::AIParam(playerBelong), m3d::AIParam());
        thePlayer->CauseEvent(GE_RELATION_CHANGED, 0.0f, m3d::AIParam(victimBelong), m3d::AIParam());

        // Offer a way back: the first town that still tolerates the player hands out
        // a peace quest for the clan they just turned hostile.
        for (auto* obj : *theObjects)
        {
            if (obj && obj->IsKindOf(&Town::m_classTown) &&
                theRelationship->CheckTolerance(obj->GetBelong(), playerVehicle->GetBelong()) > RS_ENEMY)
            {
                DynamicQuestManager::CreateQuest(DynamicQuestManager::TYPE_PEACE, victim->GetId(), obj->GetId());
                return;
            }
        }
    }

    CompositeObj* CreateBrokenObj(PhysicObj* obj, CStr const& destroyedModelName, m3d::SgNode* toAccept)
    {
        // RVA 0x7D3C80 - spawns a "BrokenModel" composite in obj's place that removes itself after a minute.
        // NOTE: the created object is used without checking that it exists.
        int const objId = theObjects->CreateNewObject(thePrototypeManager->GetPrototypeId(CStr("BrokenModel")), "", -1, -1);
        auto* const brokenObj = static_cast<CompositeObj*>(theObjects->GetEntityByObjId(objId));
        float const mass = obj->GetMass();
        Quaternion const rot = obj->GetRotation();
        brokenObj->Init(destroyedModelName, obj->GetPosition(), rot, mass, toAccept);
        theObjects->AddObjToPostCollideList(brokenObj);
        brokenObj->SetDeadTimer(60000, true);
        return brokenObj;
    }

    cmpByDistToOrg::cmpByDistToOrg(CVector const& Org) : m_Org(Org)
    {
    }

    bool cmpByDistToOrg::operator()(int const& objId1, int const& objId2) const
    {
        // RVA 0x9FD8C0 - NOTE: neither object is checked for null or for being a PhysicObj.
        PhysicObj* obj1 = static_cast<PhysicObj*>(theObjects->GetEntityByObjId(objId1));
        PhysicObj* obj2 = static_cast<PhysicObj*>(theObjects->GetEntityByObjId(objId2));
        CVector const pos1 = obj1->GetPosition();
        CVector const pos2 = obj2->GetPosition();
        double const x1 = double(pos1.x) - m_Org.x;
        double const y1 = double(pos1.y) - m_Org.y;
        double const z1 = double(pos1.z) - m_Org.z;
        float const x2 = pos2.x - m_Org.x;
        float const y2 = pos2.y - m_Org.y;
        float const z2 = pos2.z - m_Org.z;
        return std::sqrt(double(z2) * z2 + double(y2) * y2 + double(x2) * x2) > std::sqrt(z1 * z1 + y1 * y1 + x1 * x1);
    }

    void sortPhysicObjsByDistance(retruxx::vector<int>& vct, CVector const& org)
    {
        // RVA 0x9FEA80 - NOTE: std::sort is not stable, so the order of equally distant objects
        // may differ from the shipped STL's.
        std::sort(vct.begin(), vct.end(), cmpByDistToOrg(org));
    }

    Quaternion GetRotationByDirection(CVector const& direction)
    {
        // RVA 0x625840 - the rotation that turns +z towards direction: a heading about y followed
        // by an elevation about x. The zero terms of the quaternion product are kept so the result
        // matches the original to the bit.
        double const halfHeading = std::atan2(direction.x, direction.z) * 0.5;
        float const headingSin = static_cast<float>(std::sin(halfHeading));
        float const headingCos = static_cast<float>(std::cos(halfHeading));
        double const halfElevation = -std::asin(direction.y) * 0.5;
        float const elevationSin = static_cast<float>(std::sin(halfElevation));
        float const elevationCos = static_cast<float>(std::cos(halfElevation));

        Quaternion result;
        result.x = headingCos * elevationSin + elevationCos * 0.0f + headingSin * 0.0f;
        result.y = elevationCos * headingSin + elevationSin * 0.0f + headingCos * 0.0f;
        result.z = headingCos * 0.0f + elevationCos * 0.0f - headingSin * elevationSin;
        result.w = elevationCos * headingCos - elevationSin * 0.0f - headingSin * 0.0f;
        return result;
    }
}  // namespace ai
