#include <m3dapp.h>
#include <core/ini.h>
#include <ui/frame.h>

namespace m3d
{
    namespace ui
    {
        Frame::~Frame()
        {
            ReleaseTextures();
        }

        void Frame::ReleaseTextures()
        {
            for (auto& tex : m_textures)
            {
                if (tex.IsValid())
                {
                    Application::g_pApp->m_renderer->ReleaseTexture(tex);
                }
            }
        }

        Frame::Frame()
        {
        }

        int Frame::ReadFromXmlNode(cmn::XmlNode* node)
        {
            SafeStrAttrib(m_name, node, "name");
            if (m_name.empty())
            {
                return 0;
            }
            SafeIntAttrib(m_barTexWidth, node, "barTexWidth");

            float ratio = 0.0;
            SafeFloatAttrib(ratio, node, "barUsedWidthRatio");
            m_barUsedWidth = static_cast<int>(ratio * m_barTexWidth);

            SafeIntAttrib(m_cornerSize, node, "cornerSize");
            SafeIntAttrib(m_cornerRoundSize, node, "roundCornerSize");
            SafeIntAttrib(m_barRepeat, node, "barRepeat");

            auto const wrapParam = 2 * (m_barRepeat == 0) + 1;

            CStr texName;
            SafeStrAttrib(texName, node, "bar_l");
            if (!texName.empty())
            {
                m_textures[0] = Application::g_pApp->m_renderer->AddTexture(texName, 4);
                Application::g_pApp->m_renderer->SetTextureParameter(m_textures[0], rend::TM_WRAP_S, 3);
                Application::g_pApp->m_renderer->SetTextureParameter(m_textures[0], rend::TM_WRAP_T, wrapParam);
            }
            SafeStrAttrib(texName, node, "bar_t");
            if (!texName.empty())
            {
                m_textures[1] = Application::g_pApp->m_renderer->AddTexture(texName, 4);
                Application::g_pApp->m_renderer->SetTextureParameter(m_textures[1], rend::TM_WRAP_S, wrapParam);
                Application::g_pApp->m_renderer->SetTextureParameter(m_textures[1], rend::TM_WRAP_T, 3);
            }
            SafeStrAttrib(texName, node, "bar_r");
            if (!texName.empty())
            {
                m_textures[2] = Application::g_pApp->m_renderer->AddTexture(texName, 4);
                Application::g_pApp->m_renderer->SetTextureParameter(m_textures[2], rend::TM_WRAP_S, 3);
                Application::g_pApp->m_renderer->SetTextureParameter(m_textures[2], rend::TM_WRAP_T, wrapParam);
            }
            SafeStrAttrib(texName, node, "bar_b");
            if (!texName.empty())
            {
                m_textures[3] = Application::g_pApp->m_renderer->AddTexture(texName, 4);
                Application::g_pApp->m_renderer->SetTextureParameter(m_textures[3], rend::TM_WRAP_S, wrapParam);
                Application::g_pApp->m_renderer->SetTextureParameter(m_textures[3], rend::TM_WRAP_T, 3);
            }
            SafeStrAttrib(texName, node, "corner_tl");
            if (!texName.empty())
            {
                m_textures[4] = Application::g_pApp->m_renderer->AddTexture(texName, 4);
                Application::g_pApp->m_renderer->SetTextureParameter(m_textures[4], rend::TM_WRAP_S, 3);
                Application::g_pApp->m_renderer->SetTextureParameter(m_textures[4], rend::TM_WRAP_T, 3);
            }
            SafeStrAttrib(texName, node, "corner_tr");
            if (!texName.empty())
            {
                m_textures[5] = Application::g_pApp->m_renderer->AddTexture(texName, 4);
                Application::g_pApp->m_renderer->SetTextureParameter(m_textures[5], rend::TM_WRAP_S, 3);
                Application::g_pApp->m_renderer->SetTextureParameter(m_textures[5], rend::TM_WRAP_T, 3);
            }
            SafeStrAttrib(texName, node, "corner_bl");
            if (!texName.empty())
            {
                m_textures[6] = Application::g_pApp->m_renderer->AddTexture(texName, 4);
                Application::g_pApp->m_renderer->SetTextureParameter(m_textures[6], rend::TM_WRAP_S, 3);
                Application::g_pApp->m_renderer->SetTextureParameter(m_textures[6], rend::TM_WRAP_T, 3);
            }
            SafeStrAttrib(texName, node, "corner_br");
            if (!texName.empty())
            {
                m_textures[7] = Application::g_pApp->m_renderer->AddTexture(texName, 4);
                Application::g_pApp->m_renderer->SetTextureParameter(m_textures[7], rend::TM_WRAP_S, 3);
                Application::g_pApp->m_renderer->SetTextureParameter(m_textures[7], rend::TM_WRAP_T, 3);
            }
            SafeStrAttrib(texName, node, "tabbtn_corner_tl");
            if (!texName.empty())
            {
                m_textures[8] = Application::g_pApp->m_renderer->AddTexture(texName, 4);
                Application::g_pApp->m_renderer->SetTextureParameter(m_textures[8], rend::TM_WRAP_S, 3);
                Application::g_pApp->m_renderer->SetTextureParameter(m_textures[8], rend::TM_WRAP_T, 3);
            }
            SafeStrAttrib(texName, node, "tabbtn_corner_tr");
            if (!texName.empty())
            {
                m_textures[9] = Application::g_pApp->m_renderer->AddTexture(texName, 4);
                Application::g_pApp->m_renderer->SetTextureParameter(m_textures[9], rend::TM_WRAP_S, 3);
                Application::g_pApp->m_renderer->SetTextureParameter(m_textures[9], rend::TM_WRAP_T, 3);
            }
            SafeStrAttrib(texName, node, "tabbtn_corner_bl");
            if (!texName.empty())
            {
                m_textures[10] = Application::g_pApp->m_renderer->AddTexture(texName, 4);
                Application::g_pApp->m_renderer->SetTextureParameter(m_textures[10], rend::TM_WRAP_S, 3);
                Application::g_pApp->m_renderer->SetTextureParameter(m_textures[10], rend::TM_WRAP_T, 3);
            }
            SafeStrAttrib(texName, node, "tabbtn_corner_br");
            if (!texName.empty())
            {
                m_textures[11] = Application::g_pApp->m_renderer->AddTexture(texName, 4);
                Application::g_pApp->m_renderer->SetTextureParameter(m_textures[11], rend::TM_WRAP_S, 3);
                Application::g_pApp->m_renderer->SetTextureParameter(m_textures[11], rend::TM_WRAP_T, 3);
            }
            return 1;
        }

        BackGround::~BackGround()
        {
            if (m_texture.IsValid())
            {
                Application::g_pApp->m_renderer->ReleaseTexture(m_texture);
            }
        }

        int BackGround::ReadFromXmlNode(cmn::XmlNode* node)
        {
            SafeStrAttrib(m_name, node, "name");
            if (m_name.empty())
            {
                return 0;
            }
            SafeIntAttrib(m_repeatU, node, "bgRepeatU");
            SafeIntAttrib(m_repeatV, node, "bgRepeatV");
            auto const repeatUParam = 2 * (m_repeatU == 0) + 1;
            auto const repeatVParam = 2 * (m_repeatV == 0) + 1;

            CStr texName;
            SafeStrAttrib(texName, node, "file");
            if (!texName.empty())
            {
                m_texture = Application::g_pApp->m_renderer->AddTexture(texName, 4);
                Application::g_pApp->m_renderer->SetTextureParameter(m_texture, rend::TM_WRAP_S, repeatUParam);
                Application::g_pApp->m_renderer->SetTextureParameter(m_texture, rend::TM_WRAP_T, repeatVParam);
            }
            return 1;
        }

        BackGround::BackGround()
        {
        }

        Pane::~Pane()
        {
        }

        Pane::Pane()
        {
        }

        int Pane::ReadFromXmlNode(cmn::XmlNode* node, std::vector<BackGround*> const& gfxBgs, std::vector<Frame*> const& gfxFrames)
        {
            SafeStrAttrib(m_name, node, "name");
            if (m_name.empty())
            {
                return 0;
            }
            CStr texName;
            const char* frameIds[4] = { "frameOut", "frameDown", "frameOver", "frameDisable" };
            for (size_t i =0; i<4; ++i)
            {
                SafeStrAttrib(texName, node, frameIds[i]);
                auto it = std::find_if(cbegin(gfxFrames), cend(gfxFrames), [&texName](auto* frame)
                {
                    return texName == frame->m_name;
                });
                if (it != cend(gfxFrames))
                {
                    m_frame[i] = *it;
                }
            }

            const char* bgIds[4] = { "bgOut", "bgDown", "bgOver", "bgDisable" };
            for (size_t i = 0; i < 4; ++i)
            {
                SafeStrAttrib(texName, node, bgIds[i]);
                auto it = std::find_if(cbegin(gfxBgs), cend(gfxBgs), [&texName](auto* frame)
                {
                    return texName == frame->m_name;
                });
                if (it != cend(gfxBgs))
                {
                    m_bg[i] = *it;
                }
            }

            SafeBoolAttrib(m_bInnerBg, node, "innerBg");
            return 1;
        }

        float ScrollPane::GetWidth() const
        {
            throw std::logic_error("Not implemented");
        }

        int ScrollPane::ReadFromXmlNode(m3d::cmn::XmlNode* node)
        {
            SafeStrAttrib(m_name, node, "name");
            if (m_name.empty())
            {
                return 0;
            }
            CVector2 vec;
            SafeVector2Attrib(vec, node, "scrollThumbSize");
            m_thumbSize.x = vec.x;
            m_thumbSize.y = vec.y;
            
            SafeVector2Attrib(vec, node, "scrollBtnSize");
            m_btnSize.x = vec.x;
            m_btnSize.y = vec.y;

            SafeFloatAttrib(m_space, node, "scrollSpace");

            CStr texName;
            SafeStrAttrib(texName, node, "scrollThumb");
            if (!texName.empty())
            {
                m_thumbTex = Application::g_pApp->m_renderer->AddTexture(texName, 4);
                Application::g_pApp->m_renderer->SetTextureParameter(m_thumbTex, rend::TM_WRAP_S, 3);
                Application::g_pApp->m_renderer->SetTextureParameter(m_thumbTex, rend::TM_WRAP_T, 3);
            }
            SafeStrAttrib(m_barPaneName, node, "scrollBarPane");
            SafeStrAttrib(m_prevButtonPaneName, node, "scrollBtnPrevPane");
            SafeStrAttrib(m_nextButtonPaneName, node, "scrollBtnNextPane");
            return 1;
        }

        ScrollPane::~ScrollPane()
        {
            if (m_thumbTex.IsValid())
            {
                Application::g_pApp->m_renderer->ReleaseTexture(m_thumbTex);
            }
        }

        ScrollPane::ScrollPane()
        {
        }
    }
}
