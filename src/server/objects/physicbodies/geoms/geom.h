#pragma once

namespace m3d
{
    namespace cmn
    {
        class XmlNode;
        class XmlFile;
    }
}

class CVector;
class Aabb;
struct dxBody;
struct dxGeom;
struct dxSpace;
class Quaternion;

namespace ai
{
    enum GeomType
    {
        GEOM_TYPE_NONE = 0x0,
        GEOM_TYPE_BOX = 0x1,
        GEOM_TYPE_SPHERE = 0x2,
        GEOM_TYPE_CYLINDER = 0x3,
        GEOM_TYPE_RAY = 0x4,
        GEOM_TYPE_TRIMESH = 0x5,
        GEOM_TYPE_FROM_MODEL = 0x6,
    };

    class Geom
    {
    public:
        class CellAabb
        {
        public:
            CellAabb();

        private:
            int x0;
            int z0;
            int x1;
            int z1;
        };

    public:
        Quaternion GetRotation() const ;
        void SetBody(dxBody * const);
        dxSpace * GetSpace() const ;
        void UnlinkFromCollisionCells(int);
        void RelinkToSpace(dxSpace *);
        void SetRotation(Quaternion const &);
        virtual ~Geom();
        dxGeom * GetGeomId() const ;
        void Disable();
        void LinkToCollisionCells(int,CellAabb *);
        CVector GetPosition() const ;
        CellAabb GetCollisionCellAabb() const ;
        void * GetData() const ;
        int GetGeomClass() const ;
        CellAabb CountCellAabb() const ;
        Aabb GetAabb() const ;
        void UnlinkFromBody();
        void SetData(void *);
        void SetPosition(CVector const &);
        void Enable();
        void RelinkToCollisionCells(int);
        bool IsEnabled() const ;
        void CheckCollisionCells();
        void SetDirection(CVector const &);
        virtual void DumpPhysicInfo(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;

    protected:
        Geom(dxGeom* const geomId,void (__fastcall* movedCallback)(dxGeom *));

    private:
        //Geom_vtbl *__vftable /*VFT*/;
        dxGeom *m_geomId;
        CellAabb m_curAabb;
    };
}
