#include <m3dapp.h>
#include <stdexcept>
#include <scene/servers/serversprites.h>
#include <client.h>
#include <config.h>
#include <landscape.h>
#include <world.h>
#include <core/kernel.h>
#include <core/log.h>
#include <renderer/i_renderer.h>

namespace m3d
{
    namespace
    {
        // What SgSpriteNode hands to RenderItem (SpritesServer::RenderItem::RenderInfo in the PDB).
        struct RenderInfo
        {
            /* 0x0000 */ CVector* m_org;
            /* 0x0004 */ float m_scaleX;
            /* 0x0008 */ float m_scaleY;
            /* 0x000c */ float m_rotZ;
            /* 0x0010 */ unsigned int m_color;
            /* 0x0014 */ unsigned int m_mode;
            /* 0x0018 */ float m_tsc;
        }; /* size: 0x001c */

        // A sprite item is nothing but a texture; its handle is kept in the model's pointer.
        rend::TexHandle& TextureOf(DataServer::Model& model)
        {
            return reinterpret_cast<rend::TexHandle&>(model.m_ptr);
        }
    }  // namespace

    int SpritesServer::GetItemProperty(int id, int prop, void* dest)
    {
        // RVA 0x76AFA0
        return DataServer::GetItemProperty(id, prop, dest);
    }

    int SpritesServer::SetItemProperty(int id, int prop, void* src)
    {
        // RVA 0x76AFB0
        return DataServer::SetItemProperty(id, prop, src);
    }

    SpritesServer::~SpritesServer()
    {
        // RVA 0x76B870
        Release();
    }

    int SpritesServer::Release()
    {
        // RVA 0x76B630
        m_valid = false;
        for (auto& model : m_models)
        {
            M3D_RENDERER->ReleaseTexture(TextureOf(model));
        }
        m_models = retruxx::vector<Model>();
        return 1;
    }

    void SpritesServer::RenderItem(int id, void* params)
    {
        // RVA 0x76B2F0 - a textured quad laid over the landscape.
        m_profiler->StartCountdown();

        auto* const renderer = M3D_RENDERER;
        if (id == -2)
        {
            overlayStart();
            renderer->SetCull(rend::M3DCULL_CW, false);
            rend::TexHandle noTexture;
            renderer->SetTexture(1, noTexture, -1.0);
            renderer->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
            renderer->SetStageState(0, rend::BM_ALPHA, rend::TS_TEXTURE);
            renderer->SetStageState(1, rend::BM_COLOR, rend::TS_NONE);
            renderer->SetStageState(1, rend::BM_ALPHA, rend::TS_NONE);
            renderer->SetAlphaTest(M3D_ENGINE_CFG.m_alphaTestWorld.GetI());
            renderer->SetBlend(rend::BM_DCOLOR_0, false);
            CMatrix identity;
            identity.identity();
            renderer->MatMul(identity);
        }
        else if (id == -3)
        {
            overlayStop();
            renderer->SetAlphaTest(0);
            renderer->SetBlend(rend::BM_NONE, false);
        }
        else if (id != -4)
        {
            auto const* const ri = static_cast<RenderInfo const*>(params);
            rend::TexHandle texture = TextureOf(m_models[id]);
            switch (ri->m_mode)
            {
            case 0:
                // Darkening, like scorch marks.
                renderer->SetBlend(rend::BM_DCOLOR_0, false);
                renderer->SetStageState(0, rend::BM_COLOR, rend::TS_TEX_ADDSIGNED_DIFF);
                break;
            case 1:
                // Additive.
                renderer->SetBlend(rend::BM_1_1, false);
                renderer->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
                break;
            case 2:
                // Alpha blended.
                renderer->SetBlend(rend::BM_ALPHA, false);
                renderer->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
                renderer->SetAlphaTest(1);
                break;
            default:
                break;
            }
            CVector const scale(ri->m_scaleX * 1.5f, ri->m_scaleY * 1.5f, 0.0f);
            renderer->SetTexture(0, texture, -1.0);
            pClient->GetWorld().GetLandscape().drawSpriteOverlayed(ri->m_color, *ri->m_org, scale, ri->m_rotZ);
        }

        m_profiler->EndCountdown();
    }

    int SpritesServer::SaveAllLoadedEntities(char const*)
    {
        // RVA 0x76AFC0
        return 1;
    }

    int SpritesServer::RemoveItem(int)
    {
        // RVA 0x76AF90
        return 1;
    }

    int SpritesServer::AddItem(char const* params, char const* id)
    {
        // RVA 0x76B6D0
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
            // NOTE: the message is indexed by the protocol value, so it loses its first characters.
            M3D_LOG_INFO(CStr(&"protocol is not supported "[proto]));
            return -1;
        }

        char const* const filename = &params[paramsPos];
        rend::TexHandle texture = M3D_RENDERER->AddTexture(CStr(filename), 0);
        // NOTE: the first wrap mode is set on the protocol value read as a texture handle instead of on the texture.
        M3D_RENDERER->SetTextureParameter(reinterpret_cast<rend::TexHandle const&>(proto), rend::TM_WRAP_S, 3);
        M3D_RENDERER->SetTextureParameter(texture, rend::TM_WRAP_T, 3);

        Model model(nullptr, filename, filename, id);
        TextureOf(model) = texture;
        m_models.push_back(model);
        return m_models.empty() ? -1 : static_cast<int>(m_models.size()) - 1;
    }

    SpritesServer::SpritesServer()
    {
        auto id = Application::g_pApp->GetProfilerStack().AddProfiler("sprites", 0x1E);
        if (id < Application::g_pApp->GetProfilerStack().GetNumProfilers())
        {
            m_profiler = Application::g_pApp->GetProfilerStack().GetProfiler(id);
        }
    }

    int SpritesServer::Init()
    {
        m_valid = true;
        return 1;
    }

    void SpritesServer::AddItemsList(retruxx::vector<m3d::DataServer::ServerItem>& itemsList)
    {
        // RVA 0x76AFE0
        for (unsigned i = 0; i < itemsList.size(); ++i)
        {
            if (m_fnLoadCallback)
            {
                m_fnLoadCallback(100 * i / itemsList.size(), m_fnLoadCallbackData);
            }
            CStr const fn(itemsList[i].m_filename);
            CStr const itemId(itemsList[i].m_id);
            if (AddItem(fn.c_str(), itemId.c_str()) == -1)
            {
                M3D_LOG_INFO("DataServer: cannot read " + fn + " id = " + itemId);
            }
        }
    }
}
