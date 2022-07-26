#include <m3dapp.h>
#include <core/ini.h>
#include <ui/image.h>

namespace m3d
{
    namespace ui
    {
        RT_CLASS_DEFINE(ImageWnd);

        Object* ImageWnd::CreateObject()
        {
            return new ImageWnd;
        }

        Class* ImageWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(Wnd);
        }

        int ImageWnd::SetImage(CStr const&)
        {
            throw std::logic_error("Not implemented");
        }

        int ImageWnd::SetImage(rend::TexHandle tex)
        {
            if (m_texture.IsValid())
            {
                m3d::Application::g_pApp->m_renderer->ReleaseTexture(m_texture);
            }
            m_texture = tex;
            m3d::Application::g_pApp->m_renderer->ReferenceTexture(m_texture);
            return m_texture.IsValid();
        }

        Class* ImageWnd::GetClass() const
        {
            return RT_CLASS_LOCAL(ImageWnd);
        }

        ImageWnd::~ImageWnd()
        {
            if (m_texture.IsValid())
            {
                m3d::Application::g_pApp->m_renderer->ReleaseTexture(m_texture);
            }
        }

        rend::TexHandle ImageWnd::GetImage() const
        {
            return m_texture;
        }

        Object* ImageWnd::Clone()
        {
            throw std::logic_error("Not implemented");
        }

        int ImageWnd::ReadFromXmlNode(cmn::XmlFile* xmlFile, cmn::XmlNode* xmlNode)
        {
            auto res = Wnd::ReadFromXmlNode(xmlFile, xmlNode);
            if (!res)
            {
                return res;
            }
            SafeStrAttrib(m_textureName, xmlNode, "image");
            if (!m_textureName.empty())
            {
                m_texture = Application::g_pApp->m_renderer->AddTexture(m_textureName, 4);
                Application::g_pApp->m_renderer->SetTextureParameter(m_texture, rend::TM_WRAP_S, 3);
                Application::g_pApp->m_renderer->SetTextureParameter(m_texture, rend::TM_WRAP_T, 3);
            }
            return 1;
        }

        int ImageWnd::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        int ImageWnd::CreateImageWnd(BoundsBase<float> const&, CStr const&)
        {
            throw std::logic_error("Not implemented");
        }

        int ImageWnd::CreateImageWnd(BoundsBase<float> const&, rend::TexHandle)
        {
            throw std::logic_error("Not implemented");
        }

        int ImageWnd::Create(CStr const& caption, unsigned style, BoundsBase<float> const& rc, unsigned id)
        {
            return Wnd::Create(caption, style, rc, id);
        }

        int ImageWnd::OnPaint(DrawInfo const&)
        {
            throw std::logic_error("Not implemented");
        }

        ImageWnd::ImageWnd()
        {
            m_paneFlags = 0;
        }

        ImageWnd::ImageWnd(ImageWnd const&)
        {
            throw std::logic_error("Not implemented");
        }
    }
}
