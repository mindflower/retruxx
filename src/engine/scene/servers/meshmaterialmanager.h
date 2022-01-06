#pragma once
#include <skelmodel.h>
#include <core/stringm3d.h>

namespace m3d
{
    class SgNode;
    class DSurfaceMaterial;

    class MeshMaterialManager
    {
    public:
        ~MeshMaterialManager();
        void Init(CStr const &,CStr const &);
        MeshMaterialManager();
        void Release();
        DSurfaceMaterial& GetMaterial(SgNode &,AnimatedModel::Mesh &);

    private:
        AnimatedModel *m_pLogos;
        std::map<int,int> m_mapBelongToLogo;
    };
}
