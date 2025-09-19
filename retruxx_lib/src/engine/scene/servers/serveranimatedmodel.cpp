#include <config.h>
#include <m3dapp.h>
#include <core/kernel.h>
#include <scene/servers/serveranimatedmodel.h>
#include <core/ini.h>
#include <client.h>
#include <world.h>

namespace m3d
{
    extern CClient* pClient;
}

namespace m3d
{
    void AnimatedModelsServer::PostLoad()
    {
        for (auto& model : m_models)
        {
            throw retruxx::logic_error("Not implemented");
        }
    }

    int AnimatedModelsServer::AddItem(char const* params, char const* id)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModelsServer::RemoveItem(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool AnimatedModelsServer::IsBonePresentsInModel(char const*, char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModelsServer::RenderItem(int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModelsServer::UnregisterNode(SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool AnimatedModelsServer::ReportServerInfo(char const*)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModelsServer::GetBoneMatrixByNameFromModelName(char const*, CStr const&, CMatrix&, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModelsServer::SaveAllLoadedEntities(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModelsServer::Release()
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModelsServer::RenderNodeSet(SgNode**, unsigned, RenderNodeInfo)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModelsServer::RenderTransparents(SgNode**, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModelsServer::RenderShadowVolumesSet(SgNode**, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    AnimatedModelsServer::~AnimatedModelsServer()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModelsServer::GenerateImpostorsIfNeeded()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModelsServer::GetItemProperty(int id, int prop, void* dest)
    {
        if (DataServer::GetItemProperty(id, prop, dest))
        {
            return 1;
        }
        if (id == -1)
        {
            return 0;
        }
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModelsServer::RegisterNode(SgNode* node)
    {
        AnimInfo* anim = nullptr;
        node->GetProperty(1u, &anim);
        anim = new AnimInfo;
        node->SetProperty(1u, &anim);

        int modelId = -1;
        node->GetProperty(4360u, &modelId);
        if (modelId >= 0)
        {
            if (modelId < m_models.size())
            {
                auto* model = reinterpret_cast<DynamicModel*>(&m_models[modelId]);
                auto& animModel = model->m_mdl[0];
                if (anim->m_forModel != animModel)
                {
                    anim->Release();
                    anim->CreateFor(animModel);
                }

                auto modelEffectList = new ModelEffectList(model);
                node->SetProperty(2u, &modelEffectList);

                m3d::Configuration* cfg = nullptr;
                node->GetProperty(8707u, &cfg);

                animModel->FromCfgNum(*cfg);
                animModel->CalculateMeshes(*cfg);

                void* temp = nullptr;
                node->GetProperty(8706u, &temp);
                node->SetProperty(8706u, &temp);

                bool useImpostors = true;
                node->GetProperty(8720u, &useImpostors);
                if (useImpostors)
                {
                    node->SetProperty(8720u, &model->m_useImpostors);
                }

                m3d::TransparencyType tt = TT_NONE;
                node->GetServerItemProperty(2u, &tt);
                node->SetTransparencyType(tt);
            }
        }

        if (!anim->IsEmpty())
        {
            // TODO: check this
            node->GetGraph()->LinkThinkNode(node);
        }
    }

    void AnimatedModelsServer::UpdateItem(int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    namespace
    {
        void __fastcall DefineSkinsToLoad(m3d::LoadSkins& skinsToLoad, CStr const& paramsStr)
        {
            retruxx::string view(paramsStr.c_str(), paramsStr.length());
            const auto skinsPos = view.find("skins:");
            const auto postSkinsPos = skinsPos + 6;
            const auto semicolonPos = view.find(";", postSkinsPos);
            if (skinsPos == retruxx::string::npos || semicolonPos == retruxx::string::npos)
            {
                return;
            }

            const auto params = view.substr(postSkinsPos, semicolonPos - postSkinsPos);
            retruxx::vector<CStr> tokens;
            Tokenize(params.c_str(), tokens, "(), ;\t");

            skinsToLoad.loadAllSkins = tokens.empty();

            for (const auto& token : tokens)
            {
                skinsToLoad.loadSkins.insert(strToInt(token));
            }
        }

    }

    void AnimatedModelsServer::AddItemsList(retruxx::vector<ServerItem>& itemslist)
    {
        auto startTime = GetTickCount();
        if (itemslist.empty())
        {
            return;
        }

        retruxx::vector<LoadSkins> skinsToLoad(itemslist.size());
        for (int i = 0; i < skinsToLoad.size(); ++i)
        {
            if (!itemslist[i].m_params.empty())
            {
                DefineSkinsToLoad(skinsToLoad[i], itemslist[i].m_params);
            }
        }

        for (const auto& model : m_models)
        {
            throw retruxx::logic_error("Not implemented");
            for (auto& item : itemslist)
            {
                if (model.m_name == item.m_id)
                {
                    item.m_fileWasRead = true;
                    throw retruxx::logic_error("Not implemented");
                    //TODO: check this!!!!!!
                    M3D_APP->m_cachedSoundIDs.insert(model.m_name);


                }
            }
        }
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModelsServer::RenderMesh(SgAnimatedModelNode*, AnimatedModel::Mesh&, rend::IEffect*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModelsServer::RenderMesh(AnimInfo*, AnimatedModel::Mesh&, rend::IEffect*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModelsServer::RenderModelForImpostor(AnimatedModel*, float, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModelsServer::UpdateGlobalRenderingParams()
    {
        throw retruxx::logic_error("Not implemented");
    }
}

ModelEffectList::ModelEffectList(DynamicModel* meta) :
    m_dynModel(meta)
{
}
