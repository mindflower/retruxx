#include "trimesh.h"
#include <ode/collision_trimesh.h>

namespace ai
{
    TriMesh::~TriMesh()
    {
        RETRUXX_NOT_IMPLEMENTED;
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

        auto trimesh = dCreateTriMesh(space, trimeshData, nullptr, nullptr, nullptr);
        return new TriMesh(trimesh, movedCallback);
    }

    TriMesh::TriMesh(dxGeom* const geomId, void(*movedCalback)(dxGeom*)) : Geom(geomId, movedCalback)
    {
    }
}
