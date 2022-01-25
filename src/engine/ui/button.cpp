#include <m3dapp.h>
#include <core/ini.h>
#include <ui/button.h>

namespace m3d
{
    namespace ui
    {
        RT_CLASS_DEFINE(ButtonWnd);

        Object* ButtonWnd::CreateObject()
        {
            return new ButtonWnd;
        }

        Class* ButtonWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(Wnd);
        }

        int ButtonWnd::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        rend::TexHandle ButtonWnd::GetImageDown() const
        {
            throw std::logic_error("Not implemented");
        }

        Class* ButtonWnd::GetClass() const
        {
            return RT_CLASS_LOCAL(ButtonWnd);
        }

        void ButtonWnd::SetRegular()
        {
            throw std::logic_error("Not implemented");
        }

        int ButtonWnd::SetImaged(rend::TexHandle, rend::TexHandle, rend::TexHandle, rend::TexHandle)
        {
            throw std::logic_error("Not implemented");
        }

        rend::TexHandle ButtonWnd::GetImageRegular() const
        {
            throw std::logic_error("Not implemented");
        }

        Object* ButtonWnd::Clone()
        {
            throw std::logic_error("Not implemented");
        }

        bool ButtonWnd::IsImaged() const
        {
            throw std::logic_error("Not implemented");
        }

        ButtonWnd::~ButtonWnd()
        {
            throw std::logic_error("Not implemented");
        }

        int ButtonWnd::ReadFromXmlNode(cmn::XmlFile* xmlFile, cmn::XmlNode* xmlNode)
        {
            auto res = Wnd::ReadFromXmlNode(xmlFile, xmlNode);
            if (!res)
            {
                return res;
            }
            m_isImaged = 0;
            SafeIntAttrib(m_isImaged, xmlNode, "btnIsImaged");
            if (m_isImaged)
            {
                CStr image;
                SafeStrAttrib(image, xmlNode, "btnImage");
                if (!image.empty())
                {
                    m_image = Application::g_pApp->m_renderer->AddTexture(image, 4);
                }

                CStr imageDown;
                SafeStrAttrib(imageDown, xmlNode, "btnImageDown");
                if (!imageDown.empty())
                {
                    m_imageMouseDown = Application::g_pApp->m_renderer->AddTexture(imageDown, 4);
                }

                CStr imageIn;
                SafeStrAttrib(imageIn, xmlNode, "btnImageIn");
                if (!imageIn.empty())
                {
                    m_imageMouseIn = Application::g_pApp->m_renderer->AddTexture(imageIn, 4);
                }

                CStr imageDis;
                SafeStrAttrib(imageDis, xmlNode, "btnImageDisabled");
                if (!imageDis.empty())
                {
                    m_imageDisabled = Application::g_pApp->m_renderer->AddTexture(imageDis, 4);
                }

                if (!m_imageMouseDown.IsValid())
                {
                    m_imageMouseDown = m_image;
                    Application::g_pApp->m_renderer->ReferenceTexture(m_image);
                }
                if (!m_imageMouseIn.IsValid())
                {
                    m_imageMouseIn = m_image;
                    Application::g_pApp->m_renderer->ReferenceTexture(m_image);
                }
                if (!m_imageDisabled.IsValid())
                {
                    m_imageDisabled = m_image;
                    Application::g_pApp->m_renderer->ReferenceTexture(m_image);
                }
            }
            return 1;
        }

        rend::TexHandle ButtonWnd::GetImageDisabled() const
        {
            throw std::logic_error("Not implemented");
        }

        rend::TexHandle ButtonWnd::GetImageIn() const
        {
            throw std::logic_error("Not implemented");
        }

        float ButtonWnd::GetFrameWidth() const
        {
            throw std::logic_error("Not implemented");
        }

        int ButtonWnd::Create(CStr const& caption, unsigned style, BoundsBase<float> const& rc, unsigned id)
        {
            auto const res = Wnd::Create(caption, style, rc, id);
            if (!res)
            {
                return res;
            }
            m_style |= 4;
            return 1;
        }

        int ButtonWnd::OnMouseOut()
        {
            throw std::logic_error("Not implemented");
        }

        int ButtonWnd::OnLoosingFocus()
        {
            throw std::logic_error("Not implemented");
        }

        int ButtonWnd::OnMouseIn()
        {
            throw std::logic_error("Not implemented");
        }

        ButtonWnd::ButtonWnd(ButtonWnd const&)
        {
            throw std::logic_error("Not implemented");
        }

        void ButtonWnd::ReleaseTextures()
        {
            throw std::logic_error("Not implemented");
        }

        void ButtonWnd::OnNcPaint(DrawInfo const&, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        int ButtonWnd::OnPaint(DrawInfo const&)
        {
            throw std::logic_error("Not implemented");
        }

        int ButtonWnd::OnMouseButton0(unsigned, PointBase<float> const&)
        {
            throw std::logic_error("Not implemented");
        }

        int ButtonWnd::OnKey(unsigned short, unsigned char, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        ButtonWnd::ButtonWnd()
        {
            m_style = 278016;
            m_textWrap = TW_NOWRAP;
            m_textFormat = TF_CENTER;
            m_paneName = "defaultBtn";
            m_paneFlags = 7;
        }

        int ButtonWnd::OnObtainingFocus()
        {
            throw std::logic_error("Not implemented");
        }

        Object* CheckWnd::CreateObject()
        {
            throw std::logic_error("Not implemented");
        }

        Class* CheckWnd::GetBaseClass()
        {
            throw std::logic_error("Not implemented");
        }

        Class* CheckWnd::GetClass() const
        {
            throw std::logic_error("Not implemented");
        }

        void CheckWnd::SetCheck(int)
        {
            throw std::logic_error("Not implemented");
        }

        CheckWnd::~CheckWnd()
        {
            throw std::logic_error("Not implemented");
        }

        CStr const& CheckWnd::GetCheckedPaneName() const
        {
            throw std::logic_error("Not implemented");
        }

        Object* CheckWnd::Clone()
        {
            throw std::logic_error("Not implemented");
        }

        int CheckWnd::GetCheck() const
        {
            throw std::logic_error("Not implemented");
        }

        void CheckWnd::SetUncheckedPane(CStr const&)
        {
            throw std::logic_error("Not implemented");
        }

        int CheckWnd::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        CStr const& CheckWnd::GetUncheckedPaneName() const
        {
            throw std::logic_error("Not implemented");
        }

        void CheckWnd::SetCheckedPane(CStr const&)
        {
            throw std::logic_error("Not implemented");
        }

        int CheckWnd::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        int CheckWnd::Create(CStr const&, unsigned, BoundsBase<float> const&, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        int CheckWnd::OnMouseButton0(unsigned, PointBase<float> const&)
        {
            throw std::logic_error("Not implemented");
        }

        void CheckWnd::DrawWndText(DrawInfo const&)
        {
            throw std::logic_error("Not implemented");
        }

        BoundsBase<float> CheckWnd::GetIcoBounds() const
        {
            throw std::logic_error("Not implemented");
        }

        void CheckWnd::OnNcPaint(DrawInfo const&, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        CheckWnd::CheckWnd()
        {
            throw std::logic_error("Not implemented");
        }

        CheckWnd::CheckWnd(CheckWnd const&)
        {
            throw std::logic_error("Not implemented");
        }
    }
}
