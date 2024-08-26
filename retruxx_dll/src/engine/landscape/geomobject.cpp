#include <geomobject.h>
#include <stdexcept>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(GeomObject)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(GeomObject);

    RT_CLASS_EXPORTS_BEGIN(GeomObjectLandscape)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(GeomObjectLandscape);

    RT_CLASS_EXPORTS_BEGIN(GeomObjectStatics)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(GeomObjectStatics);

    RT_CLASS_EXPORTS_BEGIN(GeomObjectRoad)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(GeomObjectRoad);

    RT_CLASS_EXPORTS_BEGIN(GeomObjectPassCell)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(GeomObjectPassCell);

    RT_CLASS_EXPORTS_BEGIN(GeomObjectWater)
	RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(GeomObjectWater);

    void GeomObject::IncEnabledCellsCount()
    {
        throw std::logic_error("Not implemented");
    }

    void GeomObject::SetBounds(PointBase<int> const&, PointBase<int> const&)
    {
        throw std::logic_error("Not implemented");
    }

    Object* GeomObject::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    PointBase<int> const& GeomObject::GetEndCell()
    {
        throw std::logic_error("Not implemented");
    }

    PointBase<int> const& GeomObject::GetStartCell()
    {
        throw std::logic_error("Not implemented");
    }

    Class* GeomObject::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    GeomObject::~GeomObject()
    {
        throw std::logic_error("Not implemented");
    }

    Class* GeomObject::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    Object* GeomObject::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    void GeomObject::SetGeom(dxGeom*)
    {
        throw std::logic_error("Not implemented");
    }

    void GeomObject::SetEnabled(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void GeomObject::DecEnabledCellsCount()
    {
        throw std::logic_error("Not implemented");
    }

    void GeomObject::Release()
    {
        throw std::logic_error("Not implemented");
    }

    dxGeom* GeomObject::GetGeom() const
    {
        throw std::logic_error("Not implemented");
    }

    void GeomObject::SetMayBeEnabled(bool)
    {
        throw std::logic_error("Not implemented");
    }

    GeomObject::GeomObject()
    {
        throw std::logic_error("Not implemented");
    }

    GeomObject::GeomObject(GeomObject const&)
    {
        throw std::logic_error("Not implemented");
    }

    GeomObjectLandscape::~GeomObjectLandscape()
    {
        throw std::logic_error("Not implemented");
    }

    Class* GeomObjectLandscape::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    Object* GeomObjectLandscape::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    Object* GeomObjectLandscape::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    Class* GeomObjectLandscape::GetBaseClass()
    {
        return RT_CLASS_LOCAL(GeomObject);
    }

    GeomObjectLandscape::GeomObjectLandscape()
    {
        throw std::logic_error("Not implemented");
    }

    GeomObjectLandscape::GeomObjectLandscape(GeomObjectLandscape const&)
    {
        throw std::logic_error("Not implemented");
    }

    Class* GeomObjectStatics::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    GeomObjectStatics::~GeomObjectStatics()
    {
        throw std::logic_error("Not implemented");
    }

    Class* GeomObjectStatics::GetBaseClass()
    {
        return RT_CLASS_LOCAL(GeomObject);
    }

    Object* GeomObjectStatics::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    Object* GeomObjectStatics::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    GeomObjectStatics::GeomObjectStatics()
    {
        throw std::logic_error("Not implemented");
    }

    GeomObjectStatics::GeomObjectStatics(GeomObjectStatics const&)
    {
        throw std::logic_error("Not implemented");
    }

    GeomObjectRoad::~GeomObjectRoad()
    {
        throw std::logic_error("Not implemented");
    }

    Object* GeomObjectRoad::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    void GeomObjectRoad::SetRoadNode(RoadNode*)
    {
        throw std::logic_error("Not implemented");
    }

    Class* GeomObjectRoad::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    RoadNode* GeomObjectRoad::GetRoadNode() const
    {
        throw std::logic_error("Not implemented");
    }

    Class* GeomObjectRoad::GetBaseClass()
    {
        return RT_CLASS_LOCAL(GeomObject);
    }

    Object* GeomObjectRoad::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    GeomObjectRoad::GeomObjectRoad(GeomObjectRoad const&)
    {
        throw std::logic_error("Not implemented");
    }

    GeomObjectRoad::GeomObjectRoad()
    {
        throw std::logic_error("Not implemented");
    }

    GeomObjectWater::~GeomObjectWater()
    {
        throw std::logic_error("Not implemented");
    }

    Class* GeomObjectWater::GetBaseClass()
    {
        return RT_CLASS_LOCAL(GeomObject);
    }

    Object* GeomObjectWater::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    Object* GeomObjectWater::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    Class* GeomObjectWater::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    GeomObjectWater::GeomObjectWater()
    {
        throw std::logic_error("Not implemented");
    }

    GeomObjectWater::GeomObjectWater(GeomObjectWater const&)
    {
        throw std::logic_error("Not implemented");
    }

    Object* GeomObjectPassCell::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    Class* GeomObjectPassCell::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    Class* GeomObjectPassCell::GetBaseClass()
    {
        return RT_CLASS_LOCAL(GeomObject);
    }

    Object* GeomObjectPassCell::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    GeomObjectPassCell::~GeomObjectPassCell()
    {
        throw std::logic_error("Not implemented");
    }

    GeomObjectPassCell::GeomObjectPassCell(GeomObjectPassCell const&)
    {
        throw std::logic_error("Not implemented");
    }

    GeomObjectPassCell::GeomObjectPassCell()
    {
        throw std::logic_error("Not implemented");
    }
}
