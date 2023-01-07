#include <m3dapp.h>
#include <core/ini.h>
#include <ui/button.h>

namespace m3d
{
    namespace ui
    {
        RT_CLASS_EXPORTS_BEGIN(ButtonWnd)
    	RT_CLASS_EXPORTS_END;

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
            m_isImaged = 0;
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
            return m_isImaged != 0;
        }

        ButtonWnd::~ButtonWnd()
        {
            ReleaseTextures();
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
            M3D_APP->m_renderer->ReleaseTexture(m_image);
            M3D_APP->m_renderer->ReleaseTexture(m_imageMouseIn);
            M3D_APP->m_renderer->ReleaseTexture(m_imageMouseDown);
            M3D_APP->m_renderer->ReleaseTexture(m_imageDisabled);
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

        RT_CLASS_EXPORTS_BEGIN(CheckWnd)
    	RT_CLASS_EXPORTS_END;

        RT_CLASS_DEFINE(CheckWnd);

        Object* CheckWnd::CreateObject()
        {
            return new CheckWnd;
        }

        Class* CheckWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(ButtonWnd);
        }

        Class* CheckWnd::GetClass() const
        {
            return RT_CLASS_LOCAL(CheckWnd);
        }

        void CheckWnd::SetCheck(int chk)
        {
            this->m_isChecked = chk;
            this->m_paneName = chk ? m_checkedPaneName : m_uncheckedPaneName;
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

        int CheckWnd::ReadFromXmlNode(cmn::XmlFile* file, cmn::XmlNode* node)
        {
            //TODO: check this and refactor
            int result; // eax
            int v5; // ebx
            const char* v6; // eax

            result = ButtonWnd::ReadFromXmlNode(file, node);
            if (result)
            {
                m3d::SafeStrAttrib(this->m_checkedPaneName, node, "checkedPaneName");
                m3d::SafeStrAttrib(this->m_uncheckedPaneName, node, "uncheckedPaneName");
                v5 = 0;
                if (!node->IsEmpty())
                {
                    v6 = node->GetAttribute("isChecked");
                    if (v6)
                        v5 = atoi(v6);
                }
                SetCheck(v5);
                result = 1;
            }
            return result;
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

        int CheckWnd::Create(CStr const& caption, unsigned style, BoundsBase<float> const& rc, unsigned id)
        {
            //TODO: check this and refactor
            unsigned int v6; // eax

            if (!CreateWnd(caption, style, rc, id))
                return 0;
            this->m_style |= 4u;
            v6 = this->m_style;
            v6 &= 0xFBu;
            this->m_textWrap = TW_WORD_WRAP;
            this->m_isChecked = 0;
            this->m_style = v6;
            this->m_paneName = this->m_uncheckedPaneName;
            return 1;
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
        }

        CheckWnd::CheckWnd(CheckWnd const&)
        {
            throw std::logic_error("Not implemented");
        }
    }
}
