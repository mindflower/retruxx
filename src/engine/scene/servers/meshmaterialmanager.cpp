#include "meshmaterialmanager.h"

namespace m3d
{
    MeshMaterialManager::~MeshMaterialManager()
    {
        throw std::logic_error("Not implemented");
    }

    void MeshMaterialManager::Init(CStr const&, CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    MeshMaterialManager::MeshMaterialManager()
    {
        throw std::logic_error("Not implemented");
    }

    void MeshMaterialManager::Release()
    {
        throw std::logic_error("Not implemented");
    }

    DSurfaceMaterial& MeshMaterialManager::GetMaterial(SgNode&, AnimatedModel::Mesh&)
    {
        throw std::logic_error("Not implemented");
    }
}
