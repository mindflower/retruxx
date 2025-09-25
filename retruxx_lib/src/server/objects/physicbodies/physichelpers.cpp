#include "physichelpers.h"

namespace ai
{
    CollisionInfo::CollisionInfo(const ai::CollisionInfo& info)
    {
        // TODO: implement CollisionInfo::CollisionInfo
        //throw std::logic_error("Not implemented");
    }

    CollisionInfo::CollisionInfo()
    {
        Init();
    }

    void CollisionInfo::Init()
    {
        // TODO: implement CollisionInfo::Init
        //throw std::logic_error("Not implemented");
    }

    int ai::RoughSign(float value)
    {
        if (value > 0.000001)
            return 1;
        if (value >= -0.000001)
            return 0;
        return -1;
    }
}

