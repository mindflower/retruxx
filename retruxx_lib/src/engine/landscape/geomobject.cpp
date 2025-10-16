#include <geomobject.h>
#include <stdexcept>

extern "C"
{
#include <ode/collision.h>
}

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

    void GeomObject::SetBounds(PointBase<int> const& startCell, PointBase<int> const& endCell)
    {
        this->m_startCell = startCell;
        this->m_endCell = endCell;
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

    void GeomObject::SetGeom(dxGeom* geom)
    {
        this->m_geom = geom;
        dGeomSetCategoryBits(geom, 1u);
        dGeomSetCollideBits(this->m_geom, 0xFFFFFFFE);
        m_geom = this->m_geom;
        if (m_geom)
            dGeomSetData(m_geom, this);
        if (!this->m_enabledCellsCount)
            dGeomDisable(this->m_geom);
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
        return this->m_geom;
    }

    void GeomObject::SetMayBeEnabled(bool)
    {
        throw std::logic_error("Not implemented");
    }

    GeomObject::GeomObject()
    {
        this->m_startCell.x = 0;
        this->m_startCell.y = 0;
        this->m_endCell.x = -1;
        this->m_endCell.y = -1;
        this->m_geom = 0;
        this->m_TriData = 0;
        this->m_Vertices = 0;
        this->m_Indices = 0;
        this->m_enabledCellsCount = 0;
        this->m_needToDeleteInUnlink = 1;
        this->m_bMayBeEnabled = 1;
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
        return new GeomObjectLandscape;
    }

    Class* GeomObjectLandscape::GetBaseClass()
    {
        return RT_CLASS_LOCAL(GeomObject);
    }

    GeomObjectLandscape::GeomObjectLandscape()
    {
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
        return new GeomObjectStatics;
    }

    Object* GeomObjectStatics::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    GeomObjectStatics::GeomObjectStatics()
    {
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
        return new GeomObjectWater;
    }

    Object* GeomObjectWater::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    Class* GeomObjectWater::GetClass() const
    {
        return RT_CLASS_LOCAL(GeomObjectWater);
    }

    GeomObjectWater::GeomObjectWater()
    {
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
