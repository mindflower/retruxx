#include <config.h>
#include <m3dapp.h>
#include <core/kernel.h>
#include <scene/servers/serveranimatedmodel.h>

namespace m3d
{
    void AnimatedModelsServer::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModelsServer::AddItem(char const*, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModelsServer::RemoveItem(int)
    {
        throw std::logic_error("Not implemented");
    }

    bool AnimatedModelsServer::IsBonePresentsInModel(char const*, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModelsServer::RenderItem(int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModelsServer::UnregisterNode(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    bool AnimatedModelsServer::ReportServerInfo(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModelsServer::Init()
    {
        auto const logoFileName = g_Kernel->GetEngineCfg().m_loadFromGAM.GetB() ?
            "data\\models\\Logos\\Logos.gam" : "data\\models\\Logos\\Logos.sam";
        m_MeshMaterialManager.Init(logoFileName, g_Kernel->GetEngineCfg().m_pathToBelongsToLogos.GetS());

        m_impostorVs = Application::g_pApp->m_renderer->NewHlslShader("data/shaders/impostorTest_vs11.vs", "ImpostorVS", rend::IHlslShader::VS_1_1);
        if (!m_impostorVs)
        {
            return 0;
        }

        m_impostorPs = Application::g_pApp->m_renderer->NewHlslShader("data/shaders/impostorTest_ps11.ps", "ImpostorPS", rend::IHlslShader::PS_1_1);
        if (!m_impostorPs)
        {
            return 0;
        }

        m_valid = true;
        return 1;
    }

    CVector AnimatedModelsServer::GetBoundSizes(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModelsServer::GetBoneMatrixByNameFromModelName(char const*, CStr const&, CMatrix&, bool)
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModelsServer::SaveAllLoadedEntities(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModelsServer::Release()
    {
        throw std::logic_error("Not implemented");
    }

    AnimatedModelsServer::AnimatedModelsServer()
    {
        auto idx = M3D_APP->GetProfilerStack().AddProfiler("animated", 0x1E);
        if (idx < M3D_APP->GetProfilerStack().GetNumProfilers())
        {
            m_profiler = M3D_APP->GetProfilerStack().GetProfiler(idx);
        }
        idx = M3D_APP->GetDbgCounterStack().AddCounter("nodes");
        if (idx < M3D_APP->GetDbgCounterStack().GetNumCounters())
        {
            m_countNodes = M3D_APP->GetDbgCounterStack().GetCounter(idx);
            m_countNodes->SetI(0);
        }
        idx = M3D_APP->GetDbgCounterStack().AddCounter("meshes");
        if (idx < M3D_APP->GetDbgCounterStack().GetNumCounters())
        {
            m_countMeshes = M3D_APP->GetDbgCounterStack().GetCounter(idx);
            m_countMeshes->SetI(0);
        }
    }

    int AnimatedModelsServer::SetItemProperty(int, int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModelsServer::RenderNodeSet(SgNode**, unsigned, RenderNodeInfo)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModelsServer::RenderTransparents(SgNode**, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModelsServer::RenderShadowVolumesSet(SgNode**, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    AnimatedModelsServer::~AnimatedModelsServer()
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModelsServer::GenerateImpostorsIfNeeded()
    {
        //TODO: implement AnimatedModelsServer::GenerateImpostorsIfNeeded
        return 0;
        throw std::logic_error("Not implemented");
    }

    int AnimatedModelsServer::GetItemProperty(int, int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModelsServer::RegisterNode(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModelsServer::UpdateItem(int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModelsServer::AddItemsList(std::vector<ServerItem>&)
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModelsServer::RenderMesh(SgAnimatedModelNode*, AnimatedModel::Mesh&, rend::IEffect*)
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModelsServer::RenderMesh(AnimInfo*, AnimatedModel::Mesh&, rend::IEffect*)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModelsServer::RenderModelForImpostor(AnimatedModel*, float, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModelsServer::UpdateGlobalRenderingParams()
    {
        throw std::logic_error("Not implemented");
    }
}
