#include <m3dapp.h>
#include <core/ini.h>
#include <core/log.h>
#include <ui/progressbarwnd.h>
#include <ui/ui_srv.h>

namespace m3d
{
    namespace ui
    {
        RT_CLASS_EXPORTS_BEGIN(ProgressBarWnd)
    	RT_CLASS_EXPORTS_END;
        RT_CLASS_DEFINE(ProgressBarWnd);

        unsigned ProgressBarWnd::GetBarColor() const
        {
            throw std::logic_error("Not implemented");
        }

        float ProgressBarWnd::GetMaxValue() const
        {
            throw std::logic_error("Not implemented");
        }

        void ProgressBarWnd::SetCurValue(float curValue)
        {
            m_curValue = curValue;
            if (m_minValue > m_curValue)
            {
                m_curValue = m_minValue;
            }
            if (m_curValue > m_maxValue)
            {
                m_curValue = m_maxValue;
            }
        }

        rend::TexHandle ProgressBarWnd::GetBarTexture() const
        {
            throw std::logic_error("Not implemented");
        }

        void ProgressBarWnd::SetTextStyle(TextStyle)
        {
            throw std::logic_error("Not implemented");
        }

        void ProgressBarWnd::SetOrientation(Orientation)
        {
            throw std::logic_error("Not implemented");
        }

        int ProgressBarWnd::ReadFromXmlNode(cmn::XmlFile* xmlFile, cmn::XmlNode* xmlNode)
        {
            auto res = Wnd::ReadFromXmlNode(xmlFile, xmlNode);
            if (!res)
            {
                return res;
            }
            SafeClrAttrib(m_barColor, xmlNode, "barColor");
            int numOfSteps = 0;
            SafeIntAttrib(numOfSteps, xmlNode, "numOfSteps");
            SetNumOfSteps(numOfSteps);
            m_orientation = ORIENTATION_LEFT_TO_RIGHT;
            SafeEnumAttrib(m_orientation, xmlNode, "orientation");
            CStr barTexture;
            SafeStrAttrib(barTexture, xmlNode, "barTexture");
            if (!barTexture.empty())
            {
                SetBarTexture(barTexture);
            }
            int textStyle = m_textStyle;
            SafeIntAttrib(textStyle, xmlNode, "textStyle");
            if (textStyle < 0 || textStyle > 2)
            {
                M3D_LOG_INFO("ProgressBarWnd::ReadFromXmlNode warning - invalid text style " + CStr(textStyle));
            }
            else
            {
                m_textStyle = static_cast<TextStyle>(textStyle);
            }
            int textureStyle = m_textureStyle;
            SafeIntAttrib(textureStyle, xmlNode, "textureStyle");
            if (textureStyle < 0 || textureStyle > 1)
            {
                M3D_LOG_INFO("ProgressBarWnd::ReadFromXmlNode warning - invalid texture style " + CStr(textureStyle));
            }
            else
            {
                m_textureStyle = static_cast<TextureStyle>(textureStyle);
            }
            return 1;
        }

        int ProgressBarWnd::GetNumOfSteps() const
        {
            throw std::logic_error("Not implemented");
        }

        void ProgressBarWnd::SetBarColor(unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        void ProgressBarWnd::SetMaxValue(float maxValue)
        {
            m_maxValue = maxValue;
            if (m_minValue > m_curValue)
            {
                m_curValue = m_minValue;
            }
            if (m_curValue > m_maxValue)
            {
                m_curValue = m_maxValue;
            }
        }

        Object* ProgressBarWnd::Clone()
        {
            throw std::logic_error("Not implemented");
        }

        float ProgressBarWnd::GetCurValue() const
        {
            throw std::logic_error("Not implemented");
        }

        void ProgressBarWnd::SetBarTexture(rend::TexHandle barTexture)
        {
            Application::g_pApp->m_renderer->ReleaseTexture(m_barTexture);
            m_barTexture = barTexture;
            if (barTexture.IsValid())
            {
                Application::g_pApp->m_renderer->ReferenceTexture(m_barTexture);
                Application::g_pApp->m_renderer->SetTextureParameter(m_barTexture, rend::TM_WRAP_S, 1);
                Application::g_pApp->m_renderer->SetTextureParameter(m_barTexture, rend::TM_WRAP_T, 1);
            }
        }

        void ProgressBarWnd::SetBarTexture(CStr const& fileName)
        {
            if (!fileName.empty())
            {
                auto tex = Application::g_pApp->m_renderer->AddTexture(fileName, 4);
                SetBarTexture(tex);
                Application::g_pApp->m_renderer->ReleaseTexture(tex);
            }
        }

        ProgressBarWnd::TextStyle ProgressBarWnd::GetTextStyle() const
        {
            throw std::logic_error("Not implemented");
        }

        Class* ProgressBarWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(Wnd);
        }

        ProgressBarWnd::Orientation ProgressBarWnd::GetOrientation() const
        {
            throw std::logic_error("Not implemented");
        }

        void ProgressBarWnd::SetNumOfSteps(int numOfSteps)
        {
            m_numOfSteps = numOfSteps;
            if (m_numOfSteps < 1)
            {
                m_numOfSteps = 1;
            }
        }

        Class* ProgressBarWnd::GetClass() const
        {
            return RT_CLASS_LOCAL(ProgressBarWnd);
        }

        ProgressBarWnd::TextureStyle ProgressBarWnd::GetTextureStyle() const
        {
            throw std::logic_error("Not implemented");
        }

        Object* ProgressBarWnd::CreateObject()
        {
            return new ProgressBarWnd;
        }

        float ProgressBarWnd::GetMinValue() const
        {
            throw std::logic_error("Not implemented");
        }

        ProgressBarWnd::~ProgressBarWnd()
        {
            throw std::logic_error("Not implemented");
        }

        void ProgressBarWnd::SetTextureStyle(TextureStyle)
        {
            throw std::logic_error("Not implemented");
        }

        int ProgressBarWnd::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        void ProgressBarWnd::SetMinValue(float minValue)
        {
            m_minValue = minValue;
            if (minValue > m_curValue)
            {
                m_curValue = minValue;
            }
            if (m_curValue > m_maxValue)
            {
                m_curValue = m_maxValue;
            }
        }

        float ProgressBarWnd::GetSizeOfStepInPixel() const
        {
            throw std::logic_error("Not implemented");
        }

        int ProgressBarWnd::GetCurNumOfSteps() const
        {
            throw std::logic_error("Not implemented");
        }

        CStr ProgressBarWnd::GetStringValue() const
        {
            throw std::logic_error("Not implemented");
        }

        BoundsBase<float> ProgressBarWnd::GetBarRect() const
        {
            throw std::logic_error("Not implemented");
        }

        int ProgressBarWnd::OnPaint(DrawInfo const& di)
        {
            m_caption = {};
            auto res = Wnd::OnPaint(di);
            if (res)
            {
                auto barRect = GetBarRect();
                if (m_barTexture.IsValid())
                {
                    float u0 = 0.0;
                    float v0 = 0.0;
                    float u1 = 1.0;
                    float v1 = 1.0;
                    CalcTexCoordinates(u0, v0, u1, v1);
                    GetGfxServer()->AddImagedRectGeneral(di, barRect, m_curClr, m_barTexture, u0, v0, u1, v1);
                }
                else
                {
                    GetGfxServer()->AddFlatAxialQuad(di, barRect, m_barColor);
                }
                if (m_textStyle)
                {
                    SetText(GetStringValue());
                    DrawWndText(di);
                }
                return 1;
            }
            return res;
        }

        bool ProgressBarWnd::IsInversed() const
        {
            throw std::logic_error("Not implemented");
        }

        void ProgressBarWnd::CalcTexCoordinates(float&, float&, float&, float&) const
        {
            throw std::logic_error("Not implemented");
        }

        float ProgressBarWnd::GetMaxValueInPixel() const
        {
            throw std::logic_error("Not implemented");
        }

        bool ProgressBarWnd::IsHorizontal() const
        {
            throw std::logic_error("Not implemented");
        }

        float ProgressBarWnd::GetValueInPixel() const
        {
            throw std::logic_error("Not implemented");
        }

        ProgressBarWnd::ProgressBarWnd(ProgressBarWnd const&)
        {
            throw std::logic_error("Not implemented");
        }

        ProgressBarWnd::ProgressBarWnd()
        {
            m_barColor = GetGfxServer()->GetColor(1);
        }

        BoundsBase<float> ProgressBarWnd::GetMaxBarRect() const
        {
            throw std::logic_error("Not implemented");
        }
    }
}
