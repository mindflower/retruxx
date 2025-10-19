#include "trimesh.h"

namespace ai
{
    TriMesh::~TriMesh()
    {
        throw std::logic_error("Not implemented");
    }

    ai::TriMesh* TriMesh::CreateObject(dxSpace* space, const void* trimeshVertices, int numVertices,
        const int* trimeshIndices, int numIndices, void(* movedCallback)(dxGeom*), int vertexStride)
    {
        throw std::logic_error("Not implemented");
    }

    TriMesh::TriMesh(dxGeom* const geomId, void(*movedCalback)(dxGeom*)) : Geom(geomId, movedCalback)
    {
        throw std::logic_error("Not implemented");
    }
}
