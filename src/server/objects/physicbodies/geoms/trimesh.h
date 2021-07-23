#pragma once
#include "geom.h"

struct dxTriMeshData;

namespace ai
{
    class TriMesh :  public Geom
    {
    public:
        static TriMesh * __fastcall CreateObject(dxSpace *,void const *,int,int const *,int,void (__fastcall*)(dxGeom *),int);
        virtual ~TriMesh();

    private:
        TriMesh(dxGeom * const,void (__fastcall*)(dxGeom *));
        dxTriMeshData *m_trimeshDataId;
    };
}
