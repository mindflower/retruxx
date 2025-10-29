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
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomObject::SetBounds(PointBase<int> const& startCell, PointBase<int> const& endCell)
    {
        this->m_startCell = startCell;
        this->m_endCell = endCell;
    }

    Object* GeomObject::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    PointBase<int> const& GeomObject::GetEndCell()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    PointBase<int> const& GeomObject::GetStartCell()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Class* GeomObject::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    GeomObject::~GeomObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Class* GeomObject::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Object* GeomObject::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomObject::DecEnabledCellsCount()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomObject::Release()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    dxGeom* GeomObject::GetGeom() const
    {
        return this->m_geom;
    }

    void GeomObject::SetMayBeEnabled(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    GeomObjectLandscape::~GeomObjectLandscape()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Class* GeomObjectLandscape::GetClass() const
    {
        return RT_CLASS_LOCAL(GeomObjectLandscape);
    }

    Object* GeomObjectLandscape::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    Class* GeomObjectStatics::GetClass() const
    {
        return RT_CLASS_LOCAL(GeomObjectStatics);
    }

    GeomObjectStatics::~GeomObjectStatics()
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    GeomObjectStatics::GeomObjectStatics()
    {
    }

    GeomObjectStatics::GeomObjectStatics(GeomObjectStatics const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GeomObjectRoad::~GeomObjectRoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Object* GeomObjectRoad::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomObjectRoad::SetRoadNode(RoadNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Class* GeomObjectRoad::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RoadNode* GeomObjectRoad::GetRoadNode() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Class* GeomObjectRoad::GetBaseClass()
    {
        return RT_CLASS_LOCAL(GeomObject);
    }

    Object* GeomObjectRoad::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GeomObjectRoad::GeomObjectRoad(GeomObjectRoad const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GeomObjectRoad::GeomObjectRoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GeomObjectWater::~GeomObjectWater()
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    Object* GeomObjectPassCell::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Class* GeomObjectPassCell::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Class* GeomObjectPassCell::GetBaseClass()
    {
        return RT_CLASS_LOCAL(GeomObject);
    }

    Object* GeomObjectPassCell::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GeomObjectPassCell::~GeomObjectPassCell()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GeomObjectPassCell::GeomObjectPassCell(GeomObjectPassCell const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GeomObjectPassCell::GeomObjectPassCell()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
