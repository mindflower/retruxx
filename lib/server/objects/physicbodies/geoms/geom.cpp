#include "geom.h"
#include <stdexcept>
#include <math/aabb.h>
#include <math/quaternion.h>
#include <math/vector.h>
#include <ode/collision.h>

#include "core/debugcounter.h"
#include "server/objects/physicbodies/physichelpers.h"
#include "core/log.h"
#include "server/objects/base/physicobj.h"
#include <server/server.h>

#include "level.h"
#include "world.h"

namespace ai
{
    Geom::CellAabb::CellAabb()
    {
        this->x0 = 0;
        this->z0 = 0;
        this->x1 = -1;
        this->z1 = -1;
    }

    Quaternion Geom::GetRotation() const
    {
        float dq[4];

        // TODO: check order
        dGeomGetQuaternion(this->m_geomId, dq);

        Quaternion result;
        result.x = dq[1];
        result.y = dq[2];
        result.z = dq[3];
        result.w = dq[0];
        return result;
    }

    void Geom::SetBody(dxBody* const body)
    {
        // RVA 0x5CE1A0
        dGeomSetBody(m_geomId, body);
    }

    dxSpace* Geom::GetSpace() const
    {
        // RVA 0x6025C0
        return dGeomGetSpace(m_geomId);
    }

    void Geom::UnlinkFromCollisionCells(int physicObjId)
    {
        if (physicObjId != -1)
        {
            auto& landscape = ai::pServer->GetWorld()->GetLandscape();
            for (auto x0 = this->m_curAabb.x0; x0 <= this->m_curAabb.x1; ++x0)
            {
                for (auto j = this->m_curAabb.z0; j <= this->m_curAabb.z1; ++j)
                {
                    auto CollisionCellItem = landscape.GetCollisionCellItem(x0, j);
                    if (CollisionCellItem)
                        CollisionCellItem->ErasePhysicObjId(physicObjId);
                }
            }
        }
    }

    void Geom::RelinkToSpace(dxSpace* newSpace)
    {
        // RVA 0x616E10 - ODE will not move a geom between spaces on its own, so it has to
        // be taken out of the old one first.
        if (dGeomGetSpace(m_geomId))
        {
            dSpaceRemove(dGeomGetSpace(m_geomId), m_geomId);
        }
        dSpaceAdd(newSpace, m_geomId);
    }

    void Geom::SetRotation(Quaternion const& q)
    {
        float dq[4];
        dq[0] = q.w;
        dq[1] = q.x;
        dq[2] = q.y;
        dq[3] = q.z;
        dGeomSetQuaternion(m_geomId, dq);
    }

    Geom::~Geom()
    {
        dGeomDestroy(m_geomId);
    }

    dxGeom* Geom::GetGeomId() const
    {
        return m_geomId;
    }

    void Geom::Disable()
    {
        // RVA 0x5CE1B0
        dGeomDisable(m_geomId);
    }

    void Geom::LinkToCollisionCells(int physicObjId, CellAabb* newAabb)
    {
        if (physicObjId != -1)
        {
            ai::Geom::CellAabb result;
            auto v4 = newAabb;
            if (!newAabb)
            {
                result = ai::Geom::CountCellAabb();
                v4 = &result;
            }
            this->m_curAabb.x0 = v4->x0;
            this->m_curAabb.z0 = v4->z0;
            this->m_curAabb.x1 = v4->x1;
            this->m_curAabb.z1 = v4->z1;

            auto& landscape = ai::pServer->GetWorld()->GetLandscape();
            auto x0 = this->m_curAabb.x0;
            for (; x0 <= this->m_curAabb.x1; ++x0)
            {
                for (auto j = this->m_curAabb.z0; j <= this->m_curAabb.z1; ++j)
                {
                    auto CollisionCellItem = landscape.GetCollisionCellItem(x0, j);
                    if (CollisionCellItem)
                        CollisionCellItem->InsertPhysicObjId(physicObjId);
                }
            }
        }
    }

    CVector Geom::GetPosition() const
    {
        auto* pos = dGeomGetPosition(m_geomId);

        CVector result;
        result.x = pos[0];
        result.y = pos[1];
        result.z = pos[2];
        return result;
    }

    Geom::CellAabb Geom::GetCollisionCellAabb() const
    {
        // RVA 0x756BB0 - the cells this geom was last linked into, not a fresh count.
        return m_curAabb;
    }

    void* Geom::GetData() const
    {
        // RVA 0x6025A0
        return dGeomGetData(m_geomId);
    }

    int Geom::GetGeomClass() const
    {
        // RVA 0x616E60
        return dGeomGetClass(m_geomId);
    }

    Geom::CellAabb Geom::CountCellAabb() const
    {
        ai::PhysicObj::GetRelinksToCollisionCounter()->IncI();

        float aabb[6];
        dGeomGetAABB(this->m_geomId, aabb);

        const float VISCELL_EDGE_LENGTH_INV = 0.0078125;

        auto x0 = (int)(float)(VISCELL_EDGE_LENGTH_INV * aabb[0]);
        auto x1 = (int)(float)(VISCELL_EDGE_LENGTH_INV * aabb[1]);

        if (x0 > x1)
        {
            M3D_LOG_ERR("Error: incorrect cell AABB for geom: x0 = " + CStr(x0) + ", x1 = " + CStr(x1));
            x0 = x1;
        }


        auto z0 = (int)(float)(VISCELL_EDGE_LENGTH_INV * aabb[4]);
        auto z1 = (int)(float)(VISCELL_EDGE_LENGTH_INV * aabb[5]);
        if (z0 > z1)
        {
            M3D_LOG_ERR("Error: incorrect cell AABB for geom: z0 = " + CStr(z0) + ", z1 = " + CStr(z1));
            z0 = z1;
        }

        auto m_pWorld = ai::pServer->GetWorld();
        auto v8 = m_pWorld->m_level->land_size - 1;
        if (x0 < 0)
            x0 = 0;
        if (x0 > v8)
            x0 = m_pWorld->m_level->land_size - 1;
        if (x1 < 0)
            x1 = 0;
        if (x1 > v8)
            x1 = m_pWorld->m_level->land_size - 1;
        if (z0 < 0)
            z0 = 0;
        if (z0 > v8)
            z0 = m_pWorld->m_level->land_size - 1;
        if (z1 < 0)
            z1 = 0;
        if (z1 > v8)
            z1 = v8;

        ai::Geom::CellAabb result;
        result.x0 = x0;
        result.z0 = z0;
        result.x1 = x1;
        result.z1 = z1;
        return result;
    }

    Aabb Geom::GetAabb() const
    {
        float aabb[6];
        dGeomGetAABB(this->m_geomId, aabb);
        auto res_4 = aabb[2];
        auto v3 = aabb[4];

        Aabb result;
        result.m_box[0] = aabb[0];
        result.m_box[1] = res_4;
        auto v4 = v3;
        auto v5 = aabb[1];
        result.m_box[2] = v4;
        auto v6 = v5;
        auto v7 = aabb[3];
        result.m_box[3] = v6;
        auto v8 = v7;
        auto v9 = aabb[5];
        result.m_box[4] = v8;
        result.m_box[5] = v9;
        return result;
    }

    void Geom::UnlinkFromBody()
    {
        dGeomUnlinkFromBody(this->m_geomId);
    }

    void Geom::SetData(void* ptr)
    {
        // RVA 0x6025B0
        dGeomSetData(m_geomId, ptr);
    }

    void Geom::SetPosition(CVector const& vec)
    {
        dGeomSetPosition(m_geomId, vec.x, vec.y, vec.z);
    }

    void Geom::Enable()
    {
        // RVA 0x616E50
        dGeomEnable(m_geomId);
    }

    void Geom::RelinkToCollisionCells(int physicObjId)
    {
        auto newAabb = ai::Geom::CountCellAabb();
        if (newAabb.x0 != this->m_curAabb.x0
            || newAabb.z0 != this->m_curAabb.z0
            || newAabb.x1 != this->m_curAabb.x1
            || newAabb.z1 != this->m_curAabb.z1)
        {
            ai::Geom::UnlinkFromCollisionCells(physicObjId);
            ai::Geom::LinkToCollisionCells(physicObjId, &newAabb);
        }
    }

    bool Geom::IsEnabled() const
    {
        // RVA 0x756BE0
        return dGeomIsEnabled(m_geomId) != 0;
    }

    void Geom::CheckCollisionCells()
    {
        auto& landscape = pServer->GetWorld()->GetLandscape();
        for (auto x = m_curAabb.x0; x < m_curAabb.x1; ++x)
        {
            for (auto z = m_curAabb.z0; z < m_curAabb.z1; ++x)
            {
                auto* item = landscape.GetCollisionCellItem(x, z);
                if (item)
                {
                    item->m_bMustCheck = true;
                }
            }
        }
    }

    void Geom::SetDirection(CVector const& direction)
    {
        // RVA 0x5D3FE0
        SetDirectionToObject(*this, direction);
    }

    void Geom::DumpPhysicInfo(m3d::cmn::XmlFile*, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x756CA0 - a debug dump of what ODE currently holds for this geom.
        dReal const* const posArray = dGeomGetPosition(m_geomId);
        xmlNode->SetAttribute(
            "Position", CStr(CVector(posArray[0], posArray[1], posArray[2])).c_str());

        // ODE stores a quaternion as w, x, y, z.
        float dq[4];
        dGeomGetQuaternion(m_geomId, dq);
        xmlNode->SetAttribute("Rotation", CStr(Quaternion(dq[1], dq[2], dq[3], dq[0])).c_str());

        xmlNode->SetAttribute("Enabled", CStr(static_cast<int>(dGeomIsEnabled(m_geomId) != 0)).c_str());
        // NOTE: the space is dumped as the raw pointer value, which is only good for
        // telling two dumps apart within one run.
        xmlNode->SetAttribute(
            "Space", CStr(reinterpret_cast<unsigned>(dGeomGetSpace(m_geomId))).c_str());
    }

    Geom::Geom(dxGeom* const geomId, void(* movedCallback)(dxGeom*))
    {
        m_geomId = geomId;
        m_curAabb.x0 = 0;
        m_curAabb.z0 = 0;
        m_curAabb.x1 = -1;
        m_curAabb.z1 = -1;
        dGeomSetMovedCallback(geomId, movedCallback);
        dGeomSetCategoryBits(geomId, 0xFFFFu);
        dGeomSetCollideBits(geomId, 0xFFFFu);
    }
}
