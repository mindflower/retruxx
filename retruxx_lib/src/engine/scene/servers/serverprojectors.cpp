#include <m3dapp.h>
#include <scene/servers/serverprojectors.h>

struct ProjectorStats
{
    /* 0x0000 */ int numProjToRender;
    /* 0x0004 */ int numCellsRendered;
    /* 0x0008 */ int numModelsRendered;
    /* 0x000c */ int curFrame;

    void Zero()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ProjectorStats()
    {
        this->curFrame = 0;
        this->numModelsRendered = 0;
        this->numProjToRender = 0;
        this->numCellsRendered = 0;
    }
}; /* size: 0x0010 */

namespace m3d
{
    ProjectorsServer::~ProjectorsServer()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int ProjectorsServer::GetItemProperty(int, int, void*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int ProjectorsServer::SetItemProperty(int, int, void*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int ProjectorsServer::SaveAllLoadedEntities(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int ProjectorsServer::Init()
    {
        m_valid = true;
        return 1;
    }

    ProjectorsServer::ProjectorsServer()
    {
        auto id = Application::g_pApp->GetProfilerStack().AddProfiler("projectors", 0x1E);
        if (id < Application::g_pApp->GetProfilerStack().GetNumProfilers())
        {
            m_profiler = Application::g_pApp->GetProfilerStack().GetProfiler(id);
        }
        m_attenuationTex = Application::g_pApp->m_renderer->AddTexture("data\\textures\\attenuation_256.tga", 4);
        Application::g_pApp->m_renderer->SetTextureParameter(m_attenuationTex, rend::TM_TEX_FILTER, 1);
        Application::g_pApp->m_renderer->SetTextureParameter(m_attenuationTex, rend::TM_WRAP_S, 3);
        Application::g_pApp->m_renderer->SetTextureParameter(m_attenuationTex, rend::TM_WRAP_T, 3);

        m_texBiasMat._12 = 0.0;
        m_texBiasMat._13 = 0.0;
        m_texBiasMat._14 = 0.0;
        m_texBiasMat._21 = 0.0;
        m_texBiasMat._23 = 0.0;
        m_texBiasMat._24 = 0.0;
        m_texBiasMat._31 = 0.0;
        m_texBiasMat._32 = 0.0;
        m_texBiasMat._34 = 0.0;
        m_texBiasMat._44 = 1.0;
        m_texBiasMat._11 = 0.5;
        m_texBiasMat._33 = 0.5;
        m_texBiasMat._41 = 0.5;
        m_texBiasMat._42 = 0.5;
        m_texBiasMat._43 = 0.5;
        m_texBiasMat._22 = -0.5;

        memset(&m_texProjMat, 0, sizeof(m_texProjMat));
        m_texProjMat._34 = 1.0;
        m_texProjMat._33 = 1.001001;
        m_texProjMat._43 = -1.001001;
        m_texProjMat._11 = 1.0 / tan(0.5235987901687622);
        m_texProjMat._22 = m_texProjMat._11;

        m_texProjBiasMat._11 = (((m_texBiasMat._31 * m_texProjMat._13) + (m_texBiasMat._21 * m_texProjMat._12))
            + (m_texBiasMat._11 * m_texProjMat._11))
            + (m_texProjMat._14 * m_texBiasMat._41);
        m_texProjBiasMat._12 = (((m_texBiasMat._42 * m_texProjMat._14) + (m_texBiasMat._12 * m_texProjMat._11)) + (m_texBiasMat._22 * m_texProjMat._12))
            + (m_texBiasMat._32 * m_texProjMat._13);
        m_texProjBiasMat._13 = (((m_texBiasMat._13 * m_texProjMat._11) + (m_texProjMat._14 * m_texBiasMat._43))
            + (m_texBiasMat._23 * m_texProjMat._12))
            + (m_texBiasMat._33 * m_texProjMat._13);
        m_texProjBiasMat._14 = (((m_texBiasMat._34 * m_texProjMat._13) + (m_texBiasMat._44 * m_texProjMat._14)) + (m_texProjMat._11 * m_texBiasMat._14))
            + (m_texBiasMat._24 * m_texProjMat._12);
        m_texProjBiasMat._21 = (((m_texBiasMat._31 * m_texProjMat._23) + (m_texBiasMat._11 * m_texProjMat._21)) + (m_texProjMat._24 * m_texBiasMat._41))
            + (m_texBiasMat._21 * m_texProjMat._22);
        m_texProjBiasMat._22 = (((m_texBiasMat._12 * m_texProjMat._21) + (m_texProjMat._22 * m_texBiasMat._22)) + (m_texProjMat._24 * m_texBiasMat._42))
            + (m_texProjMat._23 * m_texBiasMat._32);
        m_texProjBiasMat._23 = (((m_texBiasMat._13 * m_texProjMat._21) + (m_texProjMat._24 * m_texBiasMat._43)) + (m_texProjMat._23 * m_texBiasMat._33))
            + (m_texProjMat._22 * m_texBiasMat._23);
        m_texProjBiasMat._24 = (((m_texBiasMat._44 * m_texProjMat._24) + (m_texBiasMat._34 * m_texProjMat._23)) + (m_texProjMat._22 * m_texBiasMat._24))
            + (m_texBiasMat._14 * m_texProjMat._21);
        m_texProjBiasMat._31 = (((m_texBiasMat._31 * m_texProjMat._33) + (m_texBiasMat._11 * m_texProjMat._31)) + (m_texProjMat._34 * m_texBiasMat._41))
            + (m_texProjMat._32 * m_texBiasMat._21);
        m_texProjBiasMat._32 = (((m_texProjMat._32 * m_texBiasMat._22) + (m_texBiasMat._12 * m_texProjMat._31))
            + (m_texProjMat._34 * m_texBiasMat._42))
            + (m_texProjMat._33 * m_texBiasMat._32);
        m_texProjBiasMat._33 = (((m_texProjMat._34 * m_texBiasMat._43) + (m_texProjMat._33 * m_texBiasMat._33)) + (m_texBiasMat._13 * m_texProjMat._31))
            + (m_texProjMat._32 * m_texBiasMat._23);
        m_texProjBiasMat._34 = (((m_texBiasMat._44 * m_texProjMat._34) + (m_texBiasMat._34 * m_texProjMat._33)) + (m_texProjMat._32 * m_texBiasMat._24))
            + (m_texBiasMat._14 * m_texProjMat._31);
        m_texProjBiasMat._41 = (((m_texBiasMat._31 * m_texProjMat._43) + (m_texBiasMat._11 * m_texProjMat._41)) + (m_texProjMat._44 * m_texBiasMat._41))
            + (m_texProjMat._42 * m_texBiasMat._21);
        m_texProjBiasMat._42 = (((m_texProjMat._42 * m_texBiasMat._22) + (m_texBiasMat._12 * m_texProjMat._41)) + (m_texProjMat._44 * m_texBiasMat._42))
            + (m_texProjMat._43 * m_texBiasMat._32);
        m_texProjBiasMat._43 = (((m_texProjMat._43 * m_texBiasMat._33) + (m_texProjMat._44 * m_texBiasMat._43)) + (m_texBiasMat._13 * m_texProjMat._41))
            + (m_texProjMat._42 * m_texBiasMat._23);
        m_texProjBiasMat._44 = (((m_texBiasMat._44 * m_texProjMat._44) + (m_texBiasMat._34 * m_texProjMat._43)) + (m_texProjMat._42 * m_texBiasMat._24))
            + (m_texBiasMat._14 * m_texProjMat._41);

        memset(&m_attenMat, 0, sizeof(m_attenMat));
        m_attenMat._31 = 0.0099999998;
        m_attenMat._41 = 0.039999999;
    }

    int ProjectorsServer::Release()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int ProjectorsServer::AddItem(char const*, char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ProjectorsServer::RenderItem(int id, void* params)
    {
        static ProjectorStats stats;
        m_profiler->StartCountdown();
        switch (id)
        {
        case -4:
        {
            break;
        }
        default:
            RETRUXX_NOT_IMPLEMENTED;
        }
        m_profiler->EndCountdown();
    }

    int ProjectorsServer::RemoveItem(int id)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ProjectorsServer::AddItemsList(retruxx::vector<m3d::DataServer::ServerItem>& itemslist)
    {
        // TODO: implement ProjectorsServer::AddItemsList
        //for (auto& item : itemslist)
        //{
        //    RETRUXX_NOT_IMPLEMENTED;
        //}
    }
}
