#pragma once
#include <vector>
#include <core/stringm3d.h>

namespace ai
{
    class Obj;

    class PassageData
    {
    public:
        void SetPositionToPlayerVehicle();
        void PutPassedObjectsToObjContainer();
        PassageData();
        void TakeNeededObjectsFromObjContainer();
        void Dump();
        ~PassageData();

    public:
        CStr m_mapName;
        CStr m_locationName;
        int m_angle;
        unsigned int m_fadingStartTime;
        std::vector<Obj *> m_passingObjects;
    };

    inline PassageData* thePassageData = nullptr;
}
