#include <decals.h>
#include <m3dapp.h>
#include <core/kernel.h>
#include <math/matrix.h>

namespace m3d
{
    rend::IHlslShader* DecalsList::m_decalsVs = nullptr;
    rend::IHlslShader* DecalsList::m_decalsPs = nullptr;
    CMatrix DecalsList::m_identityMatrix;

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
        if (m_decalsVs)
        {
            m_decalsVs->Release();
            m_decalsVs = nullptr;
        }
        m_decalsVs = Application::g_pApp->m_renderer->NewHlslShader("data/shaders/decals.vs", "DecalsVS", rend::IHlslShader::VS_1_1);
        if (!m_decalsVs)
        {
            SYS_ERROR("m_decalsVs");
        }

        if (m_decalsPs)
        {
            m_decalsPs->Release();
            m_decalsPs = nullptr;
        }
        m_decalsPs = Application::g_pApp->m_renderer->NewHlslShader("data/shaders/decals.ps", "DecalsPS", rend::IHlslShader::PS_1_1);
        if (!m_decalsPs)
        {
            SYS_ERROR("m_decalsPs");
        }

        memset(&m_identityMatrix, 0, sizeof(m_identityMatrix));
        m_identityMatrix._44 = 1.0;
        m_identityMatrix._33 = 1.0;
        m_identityMatrix._22 = 1.0;
        m_identityMatrix._11 = 1.0;
    }

    short DecalsList::GetMatrixId(CMatrix*)
    {
        throw std::logic_error("Not implemented");
    }
}
