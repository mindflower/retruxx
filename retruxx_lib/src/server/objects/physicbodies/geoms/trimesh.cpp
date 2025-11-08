#include "trimesh.h"
#include <ode/collision_trimesh.h>

namespace ai
{
    TriMesh::~TriMesh()
    {
        dGeomTriMeshDataDestroy(m_trimeshDataId);
    }

    ai::TriMesh* TriMesh::CreateObject(dxSpace* space, const void* trimeshVertices, int numVertices,
        const int* trimeshIndices, int numIndices, void(* movedCallback)(dxGeom*), int vertexStride)
    {
        auto trimeshData = dGeomTriMeshDataCreate();
        if (vertexStride < 0)
        {
            vertexStride = 12;
        }

        dGeomTriMeshDataBuildSingle(trimeshData, trimeshVertices, vertexStride, numVertices, trimeshIndices, numIndices, 12);

        auto geom = dCreateTriMesh(space, trimeshData, nullptr, nullptr, nullptr);
        auto trimesh = new TriMesh(geom, movedCallback);
        trimesh->m_trimeshDataId = trimeshData;
        return trimesh;
    }

    TriMesh::TriMesh(dxGeom* const geomId, void(*movedCalback)(dxGeom*)) : Geom(geomId, movedCalback)
    {
    }
}
