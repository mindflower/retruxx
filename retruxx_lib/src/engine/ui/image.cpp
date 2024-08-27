#include <m3dapp.h>
#include <core/ini.h>
#include <ui/image.h>

namespace m3d
{
    namespace ui
    {
        RT_CLASS_EXPORTS_BEGIN(ImageWnd)
    	RT_CLASS_EXPORTS_END;

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
                M3D_RENDERER->ReleaseTexture(m_texture);
            }
            m_textureName = {};
            m_texture = tex;
            M3D_RENDERER->ReferenceTexture(m_texture);
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
                M3D_RENDERER->ReleaseTexture(m_texture);
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

        int ImageWnd::OnPaint(DrawInfo const& di)
        {
            auto bounds = GetClientBounds();
            GetGfxServer()->AddImagedRect(di, bounds, m_curClr, m_texture);
            if (m_paneFlags)
            {
                auto color = m_curClr;
                if ((m_style & 2) != 0 || (m_style & 0x80000) != 0)
                    color = 3;
                auto v5 = this->GetBounds();
                BoundsBase<float> rect;
                rect.x0 = 0.0;
                rect.y0 = 0.0;
                rect.width = v5.width;
                rect.height = v5.height;
                GetGfxServer()->AddFlatAxialPane0(di, rect, color, m_paneFlags, this->m_paneName, m_bgFlags);
            }
            return 1;
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
