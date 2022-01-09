#include <decals.h>
#include <math/matrix.h>

namespace m3d
{
    void DecalsList::ReleaseShaders()
    {
        throw std::logic_error("Not implemented");
    }

    DecalsList::~DecalsList()
    {
        throw std::logic_error("Not implemented");
    }

    unsigned DecalsList::GetNumDecals()
    {
        throw std::logic_error("Not implemented");
    }

    void DecalsList::RecalcDecalsForMesh(GeometryInfo const&)
    {
        throw std::logic_error("Not implemented");
    }

    DecalsList::DecalsList()
    {
        throw std::logic_error("Not implemented");
    }

    DecalInfo const& DecalsList::GetDecal(int)
    {
        throw std::logic_error("Not implemented");
    }

    void DecalsList::Render(rend::TexHandle)
    {
        throw std::logic_error("Not implemented");
    }

    void DecalsList::AddDecal(DecalSource const&, GeometryInfo const&)
    {
        throw std::logic_error("Not implemented");
    }

    void DecalsList::LoadShaders()
    {
        throw std::logic_error("Not implemented");
    }

    short DecalsList::GetMatrixId(CMatrix*)
    {
        throw std::logic_error("Not implemented");
    }
}
