#pragma once

namespace m3d
{
    class MeshMaterialManager
    {
    public:
        ~MeshMaterialManager();
        void Init(class CStr const &,class CStr const &);
        MeshMaterialManager();
        void Release();
        DSurfaceMaterial & GetMaterial(SgNode &,AnimatedModel::Mesh &);
    protected:
    private:
        AnimatedModel *m_pLogos;
        std::map<int,int> m_mapBelongToLogo;
    };
}
