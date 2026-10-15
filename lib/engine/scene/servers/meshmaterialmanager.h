#pragma once
#include <skelmodel.h>
#include <core/stringm3d.h>

namespace m3d
{
    class SgNode;
    struct DSurfaceMaterial;

    class MeshMaterialManager
    {
    public:
        MeshMaterialManager(m3d::MeshMaterialManager const&);
        MeshMaterialManager();
        ~MeshMaterialManager();
        void Init(CStr const& LogoFileName, CStr const& BelongsToLogoFileName);
        void Release();
        m3d::DSurfaceMaterial& GetMaterial(m3d::SgNode& Node, m3d::AnimatedModel::Mesh& Mh);

    protected:
        m3d::AnimatedModel* m_pLogos;
        retruxx::map<int, int, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, int>>> m_mapBelongToLogo;
    };
}  // namespace m3d
