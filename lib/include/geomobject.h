#pragma once
#include <core/clazz.h>
#include <math/point2d.h>
#include <math/quaternion.h>
#include <math/vector.h>

struct CMatrix;
struct dxTriMeshData;
struct dxGeom;

namespace m3d
{
    class RoadNode;

    struct Triangle
    {
        int index[3];
    };

    struct GeometryInfo
    {
        void* mesh;
        unsigned int numIndices;
        m3d::Triangle* indices;
        unsigned char* vertices;
        unsigned int vertexStride;
        CMatrix* transform;
        void* oldMesh;
    };

    struct DecalData
    {
        CVector pos;
        CVector normal;
        CVector tangent;
        m3d::GeometryInfo toPutOn;
    };

    class GeomObject : public m3d::Object
    {
        friend class Landscape;

    protected:
        GeomObject();
        GeomObject(m3d::GeomObject const& rhs);

    public:
        virtual ~GeomObject() override;
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classGeomObject;
        CVector m_translation;
        Quaternion m_rotation;
        dxTriMeshData* m_TriData;
        CVector* m_Vertices;
        int* m_Indices;
        bool m_needToDeleteInUnlink;
        void Release();
        void IncEnabledCellsCount();
        void DecEnabledCellsCount();
        void SetGeom(dxGeom* geom);
        dxGeom* GetGeom() const;
        void SetEnabled(bool enabled);
        void SetMayBeEnabled(bool bMayBeEbabled);
        void SetBounds(PointBase<int> const& startCell, PointBase<int> const& endCell);
        PointBase<int> const& GetStartCell();
        PointBase<int> const& GetEndCell();

    private:
        dxGeom* m_geom;
        bool m_bMayBeEnabled;
        PointBase<int> m_startCell;
        PointBase<int> m_endCell;
        int m_enabledCellsCount;
    };

    class GeomObjectLandscape : public GeomObject
    {
    public:
        virtual ~GeomObjectLandscape();
        virtual Class* GetClass() const;
        virtual Object* Clone();
        static Object* CreateObject();
        static Class* GetBaseClass();

    protected:
        GeomObjectLandscape();
        GeomObjectLandscape(GeomObjectLandscape const&);

    public:
        RT_CLASS_DECLARE(GeomObjectLandscape);
    };

    class GeomObjectStatics : public m3d::GeomObject
    {
    protected:
        GeomObjectStatics();

    public:
        virtual ~GeomObjectStatics() override;
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classGeomObjectStatics;
    };

    class GeomObjectRoad : public GeomObject
    {
    public:
        virtual ~GeomObjectRoad();
        static Object* CreateObject();
        void SetRoadNode(RoadNode*);
        virtual Class* GetClass() const;
        RoadNode* GetRoadNode() const;
        static Class* GetBaseClass();
        virtual Object* Clone();

    protected:
        GeomObjectRoad(GeomObjectRoad const&);
        GeomObjectRoad();

    public:
        RT_CLASS_DECLARE(GeomObjectRoad);

    private:
        RoadNode* m_roadNode;
    };

    class GeomObjectWater : public GeomObject
    {
    public:
        virtual ~GeomObjectWater();
        static Class* GetBaseClass();
        static Object* CreateObject();
        virtual Object* Clone();
        virtual Class* GetClass() const;

    protected:
        GeomObjectWater();
        GeomObjectWater(GeomObjectWater const&);

    public:
        RT_CLASS_DECLARE(GeomObjectWater);
    };

    class GeomObjectPassCell : public GeomObject
    {
    public:
        static Object* CreateObject();
        virtual Class* GetClass() const;
        static Class* GetBaseClass();
        virtual Object* Clone();
        virtual ~GeomObjectPassCell();

    protected:
        GeomObjectPassCell(GeomObjectPassCell const&);
        GeomObjectPassCell();

    public:
        RT_CLASS_DECLARE(GeomObjectPassCell);
    };
}  // namespace m3d
