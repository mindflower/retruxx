#pragma once
#include <server/objects/temporarylocation.h>

namespace ai
{
    class EngineOilLocation : public TemporaryLocation
    {
    public:
        static inline m3d::Class m_classEngineOilLocation;
    };
}
