#pragma once
#include <deque>
#include <geomobject.h>
#include <vector>
#include <math/vector4.h>
#include <math/vector2.h>
#include <renderer/i_renderer.h>

namespace m3d
{
    class DecalSource
    {
    private:
        CVector center;
        CVector normal;
        CVector tangent;
        float width;
        float height;
        float depth;
    };

    class DecalInfo
    {
    private:
        DecalSource source;
        void* mesh;
        unsigned __int16 vbOffset;
        unsigned int ibOffset;
        unsigned __int16 sizeInVb;
        unsigned int sizeInIb;
        CMatrix* mat;
    };

    class Decal
    {
    public:
        void Init(DecalSource const&, GeometryInfo const&);

    private:
        bool AddPolygon(long, CVector const*);
        void ClipMesh(GeometryInfo const&);
        long ClipPolygon(long, CVector const*, CVector*) const;
        static long __fastcall ClipPolygonAgainstPlane(CVector4 const&, long, CVector const*, CVector*);

    private:
        CVector decalCenter;
        CVector decalNormal;
        CVector4 leftPlane;
        CVector4 rightPlane;
        CVector4 bottomPlane;
        CVector4 topPlane;
        CVector4 frontPlane;
        CVector4 backPlane;
        unsigned __int16 decalVertexCount;
        unsigned int decalTriangleCount;
        CVector vertexArray[100];
        CVector2 texcoordArray[100];
        Triangle triangleArray[100];
    };

    class DecalsList
    {
    public:
        static void __fastcall ReleaseShaders();
        ~DecalsList();
        unsigned int GetNumDecals();
        void RecalcDecalsForMesh(GeometryInfo const &);
        DecalsList();
        DecalInfo const & GetDecal(int);
        void Render(rend::TexHandle);
        void AddDecal(DecalSource const &,GeometryInfo const &);
        static void __fastcall LoadShaders();

    private:
        short GetMatrixId(CMatrix *);

    private:
        Decal m_workDecal;
        std::deque<DecalInfo> m_decals;
        unsigned int m_numVerts;
        unsigned int m_numIds;
        unsigned __int16 m_vbStart;
        unsigned int m_ibStart;
        rend::IbPoolField m_IbPoolField;
        rend::VbPoolField m_VbPoolField;
        std::vector<CMatrix *> m_transforms;
        std::vector<CMatrix> m_invBindtransforms;
    };
}
