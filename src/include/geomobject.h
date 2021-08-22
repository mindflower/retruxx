#pragma once
#include <core/clazz.h>
#include <math/point2d.h>
#include <math/quaternion.h>
#include <math/vector.h>

class CMatrix;
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
        Triangle* indices;
        unsigned __int8* vertices;
        unsigned int vertexStride;
        CMatrix* transform;
        void* oldMesh;
    };

    class GeomObject : public Object
    {
    public:
        static inline Class m_classGeomObject;

    public:
        void IncEnabledCellsCount();
        void SetBounds(PointBase<int> const&, PointBase<int> const&);
        virtual Object* Clone();
        PointBase<int> const& GetEndCell();
        PointBase<int> const& GetStartCell();
        static Class* __fastcall GetBaseClass();
        virtual ~GeomObject();
        virtual Class* GetClass() const;
        static Object* __fastcall CreateObject();
        void SetGeom(dxGeom*);
        void SetEnabled(bool);
        void DecEnabledCellsCount();
        void Release();
        dxGeom* GetGeom() const;
        void SetMayBeEnabled(bool);

    protected:
        GeomObject();
        GeomObject(GeomObject const&);

    private:
        CVector m_translation;
        Quaternion m_rotation;
        dxTriMeshData* m_TriData;
        CVector* m_Vertices;
        int* m_Indices;
        bool m_needToDeleteInUnlink;
        dxGeom* m_geom;
        bool m_bMayBeEnabled;
        PointBase<int> m_startCell;
        PointBase<int> m_endCell;
        int m_enabledCellsCount;
    };

    class GeomObjectLandscape :  public GeomObject
    {
    public:
        static inline Class m_classGeomObjectLandscape;

    public:
        virtual ~GeomObjectLandscape();
        virtual Class * GetClass() const ;
        virtual Object * Clone();
        static Object * __fastcall CreateObject();
        static Class * __fastcall GetBaseClass();

    protected:
        GeomObjectLandscape();
        GeomObjectLandscape(GeomObjectLandscape const &);
    };

    class GeomObjectStatics : public GeomObject
    {
    public:
        static inline Class m_classGeomObjectStatics;

    public:
        virtual Class* GetClass() const;
        virtual ~GeomObjectStatics();
        static Class* __fastcall GetBaseClass();
        static Object* __fastcall CreateObject();
        virtual Object* Clone();

    protected:
        GeomObjectStatics();
        GeomObjectStatics(GeomObjectStatics const&);

    };

    class GeomObjectRoad : public GeomObject
    {
    public:
        static inline Class m_classGeomObjectRoad;

    public:
        virtual ~GeomObjectRoad();
        static Object* __fastcall CreateObject();
        void SetRoadNode(RoadNode*);
        virtual Class* GetClass() const;
        RoadNode* GetRoadNode() const;
        static Class* __fastcall GetBaseClass();
        virtual Object* Clone();

    protected:
        GeomObjectRoad(GeomObjectRoad const&);
        GeomObjectRoad();

    private:
        RoadNode* m_roadNode;
    };

    class GeomObjectWater : public GeomObject
    {
    public:
        static inline Class m_classGeomObjectWater;

    public:
        virtual ~GeomObjectWater();
        static Class* __fastcall GetBaseClass();
        static Object* __fastcall CreateObject();
        virtual Object* Clone();
        virtual Class* GetClass() const;

    protected:
        GeomObjectWater();
        GeomObjectWater(GeomObjectWater const&);
    };

    class GeomObjectPassCell : public GeomObject
    {
    public:
        static inline Class m_classGeomObjectPassCell;

    public:
        static Object* __fastcall CreateObject();
        virtual Class* GetClass() const;
        static Class* __fastcall GetBaseClass();
        virtual Object* Clone();
        virtual ~GeomObjectPassCell();

    protected:
        GeomObjectPassCell(GeomObjectPassCell const&);
        GeomObjectPassCell();
    };
}
