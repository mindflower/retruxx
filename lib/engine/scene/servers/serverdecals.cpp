#include <decals.h>
#include <m3dapp.h>
#include <stdexcept>
#include <core/ini.h>
#include <core/log.h>
#include <core/profilerstack.h>
#include <core/ref_ptr.h>
#include <geomobject.h>
#include <scene/nodes/sgnode.h>
#include <scene/servers/serverdecals.h>

namespace m3d
{
    namespace
    {
        // Property ids the decal nodes and the server trade in.
        int const DECALS_LIST = 0x2900;   // 10496 - fetch a node's DecalsList
        int const ADD_DECAL = 0x2901;     // 10497 - stamp a new decal on a node
        int const RECALC_DECALS = 0x2902; // 10498 - re-fit decals to a new mesh

        // The void* every DataServer::Model carries is, for this server, the
        // decal's description as read from its .xml.
        struct DecalProto
        {
            /* 0x0000 */ float m_width = 0.1f;
            /* 0x0004 */ float m_height = 0.1f;
            /* 0x0008 */ float m_depth = 0.1f;
            /* 0x000c */ rend::TexHandle m_texture;
        };

        // What an SgDecalsNode hands the server for the decal properties: the
        // payload plus the node the decals live on.
        struct DecalParams
        {
            /* 0x0000 */ void* m_data;
            /* 0x0004 */ SgNode* m_node;
        };

        // And what it hands RenderItem.
        struct DecalRenderParams
        {
            /* 0x0000 */ CMatrix* m_transform;
            /* 0x0004 */ SgNode* m_node;
        };

        DecalsList* GetDecalsList(SgNode* node)
        {
            DecalsList* decals = nullptr;
            node->GetProperty(DECALS_LIST, &decals);
            return decals;
        }
    }  // namespace

    int DecalsServer::AddItem(char const* params, char const* id)
    {
        // RVA 0x75E570
        int const existing = GetItemByName(id, false);
        if (existing != -1)
        {
            return existing;
        }

        Proto proto;
        int paramsPos;
        ParseProto(params, &proto, &paramsPos);
        if (proto != PROTO_FILE)
        {
            M3D_LOG_INFO("Protocol is not supported: " + CStr(proto));
            return -1;
        }

        char const* fileName = params + paramsPos;

        CStr errorStr;
        ref_ptr<cmn::XmlFile> xmlFile = ReadXmlFile(fileName, &errorStr);
        if (!xmlFile)
        {
            M3D_LOG_INFO("DecalsServer:: cannot load " + CStr(fileName) + " err: " + errorStr);
            // NOTE: the shipped code returns 0 here, not -1, so a decal whose
            // file is missing silently aliases whichever decal happens to be
            // first in the table instead of being reported as a failure.
            return 0;
        }

        auto* decalProto = new DecalProto();

        ref_ptr<cmn::XmlNode> node = xmlFile->CreateNode();
        xmlFile->GetFirstChild(node, "Decal");
        if (node->IsEmpty())
        {
            M3D_LOG_INFO("DecalsServer:: decal descriptor missing in " + CStr(fileName));
            delete decalProto;
            return -1;
        }

        // Anything the descriptor leaves out keeps the 0.1 default.
        SafeFloatAttrib(decalProto->m_width, node, "width");
        SafeFloatAttrib(decalProto->m_height, node, "height");
        SafeFloatAttrib(decalProto->m_depth, node, "depth");

        CStr texName;
        SafeStrAttrib(texName, node, "texture");
        decalProto->m_texture = M3D_RENDERER->AddTexture(texName, 2);

        m_models.push_back(Model(decalProto, fileName, fileName, id));
        return m_models.size() - 1;
    }

    int DecalsServer::RemoveItem(int)
    {
        // RVA 0x75CE80 - decal prototypes live for the whole level, so there is
        // nothing to take out one at a time.
        return 1;
    }

    void DecalsServer::RenderItem(int id, void* params)
    {
        // RVA 0x75DB30 - the three negative ids bracket a batch of decals: -2
        // sets the render state up, -3 tears it down and -4 is a no-op marker.
        m_profiler->StartCountdown();
        if (id == -2)
        {
            CMatrix identity;
            memset(&identity, 0, sizeof(identity));
            identity._11 = 1.0f;
            identity._22 = 1.0f;
            identity._33 = 1.0f;
            identity._44 = 1.0f;
            M3D_RENDERER->MatMul(identity);

            M3D_RENDERER->PushCull(rend::M3DCULL_CCW);
            M3D_RENDERER->PushZbState(rend::ZB_NOWRITE);
            M3D_RENDERER->PushBlend(rend::BM_ALPHA);
            M3D_RENDERER->PushFog(true);
            M3D_RENDERER->PushLighting(false);

            M3D_RENDERER->SetAlphaTest(0);
            M3D_RENDERER->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
            M3D_RENDERER->SetStageState(0, rend::BM_ALPHA, rend::TS_MODULATE);
            M3D_RENDERER->SetStageState(1, rend::BM_COLOR, rend::TS_NONE);
            M3D_RENDERER->SetStageState(1, rend::BM_ALPHA, rend::TS_NONE);
        }
        else if (id == -3)
        {
            // Popped in the reverse order they were pushed.
            M3D_RENDERER->PopLighting();
            M3D_RENDERER->PopFog();
            M3D_RENDERER->PopBlend();
            M3D_RENDERER->PopZbState();
            M3D_RENDERER->PopCull();
        }
        else
        {
            if (id != -4)
            {
                auto const* decalProto = static_cast<DecalProto const*>(m_models[id].m_ptr);
                auto const* renderParams = static_cast<DecalRenderParams const*>(params);

                DecalsList* decals = GetDecalsList(renderParams->m_node);

                M3D_RENDERER->MatPush(*renderParams->m_transform);
                decals->Render(decalProto->m_texture);
                M3D_RENDERER->MatPop(true);
            }
        }
        m_profiler->EndCountdown();
    }

    int DecalsServer::Init()
    {
        m_valid = true;
        return 1;
    }

    int DecalsServer::SaveAllLoadedEntities(char const*)
    {
        // RVA 0x75CEA0 - decals are never written back out.
        return 1;
    }

    DecalsServer::~DecalsServer()
    {
        // RVA 0x75EBB0 - NOTE: the shaders go before Release(), which is the
        // reverse of the order the constructor brings them up in.
        DecalsList::ReleaseShaders();
        DecalsServer::Release();
    }

    int DecalsServer::Release()
    {
        // RVA 0x75E420
        m_valid = false;

        for (auto& model : m_models)
        {
            auto* decalProto = static_cast<DecalProto*>(model.m_ptr);
            M3D_RENDERER->ReleaseTexture(decalProto->m_texture);
            delete decalProto;
        }
        m_models.clear();

        return 1;
    }

    int DecalsServer::SetItemProperty(int id, int prop, void* src)
    {
        // RVA 0x75D9D0
        m_profiler->StartCountdown();

        if (prop == ADD_DECAL)
        {
            auto const* decalParams = static_cast<DecalParams const*>(src);
            DecalsList* decals = GetDecalsList(decalParams->m_node);

            auto const* decalProto = static_cast<DecalProto const*>(m_models[id].m_ptr);
            auto const* decalData = static_cast<DecalData const*>(decalParams->m_data);

            // Where to stamp comes from the caller, how big from the prototype.
            DecalSource source;
            source.center = decalData->pos;
            source.normal = decalData->normal;
            source.tangent = decalData->tangent;
            source.width = decalProto->m_width;
            source.height = decalProto->m_height;
            source.depth = decalProto->m_depth;

            decals->AddDecal(source, decalData->toPutOn);
        }
        else if (prop == RECALC_DECALS)
        {
            auto const* decalParams = static_cast<DecalParams const*>(src);
            DecalsList* decals = GetDecalsList(decalParams->m_node);
            decals->RecalcDecalsForMesh(*static_cast<GeometryInfo const*>(decalParams->m_data));
        }

        int const result = DataServer::SetItemProperty(id, prop, src);

        m_profiler->EndCountdown();
        return result;
    }

    DecalsServer::DecalsServer()
    {
        auto id = Application::g_pApp->GetProfilerStack().AddProfiler("decals", 0x1E);
        if (id < Application::g_pApp->GetProfilerStack().GetNumProfilers())
        {
            m_profiler = Application::g_pApp->GetProfilerStack().GetProfiler(id);
        }
        DecalsList::LoadShaders();
    }

    int DecalsServer::GetItemProperty(int id, int prop, void* dest)
    {
        // RVA 0x75CE90 - nothing decal specific to read back.
        return DataServer::GetItemProperty(id, prop, dest);
    }

    void DecalsServer::AddItemsList(retruxx::vector<ServerItem>& itemsList)
    {
        // RVA 0x75D6C0
        for (unsigned i = 0; i < itemsList.size(); ++i)
        {
            if (m_fnLoadCallback)
            {
                // NOTE: integer arithmetic, so the reported progress steps in
                // whole percent and stays at 0 until 1/100th of the list is in.
                m_fnLoadCallback(i * 100 / itemsList.size(), m_fnLoadCallbackData);
            }

            CStr const fn = itemsList[i].m_filename;
            CStr const id = itemsList[i].m_id;
            if (AddItem(fn.c_str(), id.c_str()) == -1)
            {
                M3D_LOG_INFO("DecalsServer: cannot read " + fn + " id = " + id);
            }
        }
    }
}
