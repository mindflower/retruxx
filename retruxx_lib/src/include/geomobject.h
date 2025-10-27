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
        /* 0x0000 */ void* mesh;
        /* 0x0004 */ unsigned int numIndices;
        /* 0x0008 */ m3d::Triangle* indices;
        /* 0x000c */ unsigned char* vertices;
        /* 0x0010 */ unsigned int vertexStride;
        /* 0x0014 */ CMatrix* transform;
        /* 0x0018 */ void* oldMesh;
    }; /* size: 0x001c */

    struct DecalData
    {
        /* 0x0000 */ CVector pos;
        /* 0x000c */ CVector normal;
        /* 0x0018 */ CVector tangent;
        /* 0x0024 */ m3d::GeometryInfo toPutOn;
    }; /* size: 0x0040 */

    class GeomObject : public m3d::Object
    {
    protected:
        GeomObject();
        GeomObject(const m3d::GeomObject& rhs);

    public:
        virtual ~GeomObject() override /* 0x00 */;
        virtual m3d::Object* Clone() override /* 0x04 */;
        static m3d::Object* __fastcall CreateObject();
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x34 */;
        static m3d::Class m_classGeomObject;
        CVector m_translation;
        Quaternion m_rotation;
        /* 0x0050 */ dxTriMeshData* m_TriData;
        /* 0x0054 */ CVector* m_Vertices;
        /* 0x0058 */ int* m_Indices;
        /* 0x005c */ bool m_needToDeleteInUnlink;
        void Release();
        void IncEnabledCellsCount();
        void DecEnabledCellsCount();
        void SetGeom(dxGeom* geom);
        dxGeom* GetGeom() const;
        void SetEnabled(bool enabled);
        void SetMayBeEnabled(bool bMayBeEbabled);
        void SetBounds(const PointBase<int>& startCell, const PointBase<int>& endCell);
        const PointBase<int>& GetStartCell();
        const PointBase<int>& GetEndCell();
        /* 0x005d */ char Padding_169[3];

    private:
        /* 0x0060 */ dxGeom* m_geom;
        /* 0x0064 */ bool m_bMayBeEnabled;
        /* 0x0065 */ char Padding_170[3];
        PointBase<int> m_startCell;
        PointBase<int> m_endCell;
        /* 0x0078 */ int m_enabledCellsCount;
    }; /* size: 0x007c */

    class GeomObjectLandscape :  public GeomObject
    {
    public:
        virtual ~GeomObjectLandscape();
        virtual Class * GetClass() const ;
        virtual Object * Clone();
        static Object * CreateObject();
        static Class * GetBaseClass();

    protected:
        GeomObjectLandscape();
        GeomObjectLandscape(GeomObjectLandscape const &);

    public:
        RT_CLASS_DECLARE(GeomObjectLandscape);
    };

    class GeomObjectStatics : public m3d::GeomObject
    {

    protected:
        GeomObjectStatics();
        GeomObjectStatics(const m3d::GeomObjectStatics& rhs);

    public:
        virtual  ~GeomObjectStatics() override /* 0x00 */;
        virtual m3d::Object* Clone() override /* 0x04 */;
        static m3d::Object* __fastcall CreateObject();
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x34 */;
        static m3d::Class m_classGeomObjectStatics;
    }; /* size: 0x007c */

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
}
