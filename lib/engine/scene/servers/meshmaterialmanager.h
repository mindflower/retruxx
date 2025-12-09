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
        MeshMaterialManager(const m3d::MeshMaterialManager&);
        MeshMaterialManager();
        ~MeshMaterialManager();
        void Init(const CStr& LogoFileName, const CStr& BelongsToLogoFileName);
        void Release();
        m3d::DSurfaceMaterial& GetMaterial(m3d::SgNode& Node, m3d::AnimatedModel::Mesh& Mh);

    protected:
        /* 0x0000 */ m3d::AnimatedModel* m_pLogos;
        /* 0x0004 */ retruxx::map<int, int, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, int> > > m_mapBelongToLogo;
    }; /* size: 0x0010 */
}
