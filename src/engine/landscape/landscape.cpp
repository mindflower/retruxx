#include "roads/road.h"
#include <geomobject.h>
#include <landscape.h>
#include <core/kernel.h>

namespace m3d
{
    void Landscape::Register()
    {
        g_Kernel->AddClass(&RoadNode::m_classRoadNode);
        g_Kernel->AddClass(&GeomObject::m_classGeomObject);
        g_Kernel->AddClass(&GeomObjectWater::m_classGeomObjectWater);
        g_Kernel->AddClass(&GeomObjectLandscape::m_classGeomObjectLandscape);
        g_Kernel->AddClass(&GeomObjectStatics::m_classGeomObjectStatics);
        g_Kernel->AddClass(&GeomObjectRoad::m_classGeomObjectRoad);
        g_Kernel->AddClass(&GeomObjectPassCell::m_classGeomObjectPassCell);
    }
}
