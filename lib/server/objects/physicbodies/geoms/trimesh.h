#pragma once
#include "geom.h"

namespace ai
{
    class TriMesh : public ai::Geom
    {
    public:
        virtual ~TriMesh() override;
        static ai::TriMesh* CreateObject(
            dxSpace* space,
            void const* trimeshVertices,
            int numVertices,
            int const* trimeshIndices,
            int numIndices,
            void (*movedCallback)(dxGeom*),
            int vertexStride);

    private:
        TriMesh(dxGeom* const geomId, void (*movedCalback)(dxGeom*));
        dxTriMeshData* m_trimeshDataId;
    };
}  // namespace ai
