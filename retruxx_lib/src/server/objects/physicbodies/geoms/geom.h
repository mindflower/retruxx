#pragma once
#include "core/ini.h"
#include "engine/ode/sources/collision_kernel.h"
#include "math/aabb.h"
#include "math/quaternion.h"
#include "math/vector.h"

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
    private:
        Geom(ai::Geom const&);

    public:
        struct CellAabb
        {
            /* 0x0000 */ int x0;
            /* 0x0004 */ int z0;
            /* 0x0008 */ int x1;
            /* 0x000c */ int z1;
            CellAabb();
            ai::Geom::CellAabb& operator+=(ai::Geom::CellAabb const&);
        }; /* size: 0x0010 */

        virtual ~Geom() /* 0x00 */;
        dxGeom* GetGeomId() const;
        CVector GetPosition() const;
        Quaternion GetRotation() const;
        void SetPosition(CVector const& vec);
        void SetRotation(Quaternion const& q);
        void SetDirection(CVector const& direction);
        void* GetData() const;
        void SetData(void* ptr);
        void SetBody(dxBody* const body);
        void UnlinkFromBody();
        dxSpace* GetSpace() const;
        void RelinkToSpace(dxSpace* newSpace);
        void Enable();
        void Disable();
        bool IsEnabled() const;
        Aabb GetAabb() const;
        int GetGeomClass() const;
        ai::Geom::CellAabb CountCellAabb() const;
        void LinkToCollisionCells(int physicObjId, ai::Geom::CellAabb* newAabb);
        void UnlinkFromCollisionCells(int physicObjId);
        void RelinkToCollisionCells(int physicObjId);
        void CheckCollisionCells();
        ai::Geom::CellAabb GetCollisionCellAabb() const;
        virtual void DumpPhysicInfo(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const /* 0x04 */;

    protected:
        Geom(dxGeom* const geomId, void (*movedCallback)(dxGeom*));
        /* 0x0004 */ dxGeom* m_geomId;

    private:
        /* 0x0008 */ ai::Geom::CellAabb m_curAabb;
    }; /* size: 0x0018 */
}  // namespace ai
