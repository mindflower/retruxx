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
            return m_barColor;
        }

        float ProgressBarWnd::GetMaxValue() const
        {
            return m_maxValue;
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
            return m_barTexture;
        }

        void ProgressBarWnd::SetTextStyle(TextStyle textStyle)
        {
            m_textStyle = textStyle;
        }

        void ProgressBarWnd::SetOrientation(Orientation orientation)
        {
            m_orientation = orientation;
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
            return m_numOfSteps;
        }

        void ProgressBarWnd::SetBarColor(unsigned barColor)
        {
            m_barColor = barColor;
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
            return new ProgressBarWnd(*this);
        }

        float ProgressBarWnd::GetCurValue() const
        {
            return m_curValue;
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
            return m_textStyle;
        }

        Class* ProgressBarWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(Wnd);
        }

        ProgressBarWnd::Orientation ProgressBarWnd::GetOrientation() const
        {
            return m_orientation;
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
            return m_textureStyle;
        }

        Object* ProgressBarWnd::CreateObject()
        {
            return new ProgressBarWnd;
        }

        float ProgressBarWnd::GetMinValue() const
        {
            return m_minValue;
        }

        ProgressBarWnd::~ProgressBarWnd()
        {
            if (m_barTexture.IsValid())
            {
                M3D_RENDERER->ReleaseTexture(m_barTexture);
            }
        }

        void ProgressBarWnd::SetTextureStyle(TextureStyle textureStyle)
        {
            m_textureStyle = textureStyle;
        }

        int ProgressBarWnd::WriteToXmlNode(cmn::XmlFile* file, cmn::XmlNode* writeTo)
        {
            // RVA 0x674DA0
            if (!Wnd::WriteToXmlNode(file, writeTo))
            {
                return 0;
            }

            CStr clr;
            clr.format("%08x", m_barColor);
            writeTo->SetAttribute("barColor", clr.c_str());
            writeTo->SetAttribute("numOfSteps", CStr(m_numOfSteps).c_str());
            writeTo->SetAttribute("orientation", CStr(static_cast<int>(m_orientation)).c_str());

            CStr barTexFile;
            Application::g_pApp->m_renderer->GetTextureName(m_barTexture, barTexFile);
            writeTo->SetAttribute("barTexture", barTexFile.c_str());

            writeTo->SetAttribute("textStyle", CStr(static_cast<int>(m_textStyle)).c_str());
            writeTo->SetAttribute("textureStyle", CStr(static_cast<int>(m_textureStyle)).c_str());
            return 1;
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
            if (m_orientation == ORIENTATION_LEFT_TO_RIGHT || m_orientation == ORIENTATION_RIGHT_TO_LEFT)
                return GetMaxBarRect().width / (float)m_numOfSteps;
            else
                return GetMaxBarRect().height / (float)m_numOfSteps;
        }

        int ProgressBarWnd::GetCurNumOfSteps() const
        {
            if (m_minValue == m_maxValue)
            {
                return 0;
            }
            // RVA 0x724EC0 - a value just above the minimum still shows one step. The step count rounds to nearest
            // (a bare fistp), not down.
            float const fSteps = (m_curValue - m_minValue) / ((m_maxValue - m_minValue) / static_cast<float>(m_numOfSteps));
            if (fSteps <= 0.001f)
            {
                return 0;
            }
            int const result = static_cast<int>(lrintf(fSteps));
            return result ? result : 1;
        }

        CStr ProgressBarWnd::GetStringValue() const
        {
            // RVA 0x725520 - "cur/max", rounded to nearest for the integer style (a bare fistp).
            if (m_textStyle == TEXT_INTEGER)
            {
                return CStr(static_cast<int>(lrintf(m_curValue))) + "/" + CStr(static_cast<int>(lrintf(m_maxValue)));
            }
            if (m_textStyle == TEXT_FLOAT)
            {
                return CStr(m_curValue) + "/" + CStr(m_maxValue);
            }
            return CStr();
        }

        BoundsBase<float> ProgressBarWnd::GetBarRect() const
        {
            // RVA 0x7257C0 - the filled part of the bar, measured along whichever
            // axis the orientation runs and anchored at the far end when inversed.
            auto rect = GetMaxBarRect();
            auto const valueInPixel = GetValueInPixel();

            float& origin = IsHorizontal() ? rect.x0 : rect.y0;
            float& size = IsHorizontal() ? rect.width : rect.height;

            if (IsInversed())
            {
                origin = size - valueInPixel;
            }
            size = valueInPixel;
            return rect;
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
            // RVA 0x724D30
            return m_orientation == ORIENTATION_RIGHT_TO_LEFT || m_orientation == ORIENTATION_BOTTOM_TO_TOP;
        }

        void ProgressBarWnd::CalcTexCoordinates(float& u0, float& v0, float& u1, float& v1) const
        {
            // RVA 0x724FB0 - the bar texture's coordinates along the bar: stretched over it, clamped to the filled
            // fraction, or repeated (once per step, or once per texture length); an inversed bar runs from the far
            // end.
            u0 = 0.0f;
            v0 = 0.0f;
            u1 = 1.0f;
            v1 = 1.0f;
            int const curNumOfSteps = GetCurNumOfSteps();
            int const maxNumOfSteps = m_numOfSteps;
            BoundsBase<float> const barRect = GetBarRect();
            bool const horizontal = IsHorizontal();
            float& start = horizontal ? u0 : v0;
            float& end = horizontal ? u1 : v1;
            float const barLength = horizontal ? barRect.width : barRect.height;

            int texWidth = 0;
            int texHeight = 0;
            Application::g_pApp->m_renderer->GetDims(m_barTexture, texWidth, texHeight);
            int const texLength = horizontal ? texWidth : texHeight;

            if (m_textureStyle == TEXTURE_CLAMP)
            {
                end = maxNumOfSteps > 1 ? static_cast<float>(curNumOfSteps) / static_cast<float>(maxNumOfSteps)
                                        : (m_curValue - m_minValue) / (m_maxValue - m_minValue);
            }
            else if (m_textureStyle == TEXTURE_REPEAT)
            {
                end = maxNumOfSteps > 1 ? static_cast<float>(curNumOfSteps) : barLength / static_cast<float>(texLength);
            }
            if (IsInversed())
            {
                start = 1.0f - end;
                end = 1.0f;
            }
        }

        float ProgressBarWnd::GetMaxValueInPixel() const
        {
            // RVA 0x724E80
            return IsHorizontal() ? GetMaxBarRect().width : GetMaxBarRect().height;
        }

        bool ProgressBarWnd::IsHorizontal() const
        {
            // RVA 0x724D10
            return m_orientation == ORIENTATION_LEFT_TO_RIGHT || m_orientation == ORIENTATION_RIGHT_TO_LEFT;
        }

        float ProgressBarWnd::GetValueInPixel() const
        {
            // RVA 0x725440 - a stepped bar snaps to whole steps, a plain one
            // scales linearly between min and max.
            if (m_numOfSteps > 1)
            {
                return GetSizeOfStepInPixel() * static_cast<float>(GetCurNumOfSteps());
            }

            float value = 0.0f;
            if (m_minValue != m_maxValue)
            {
                value = (m_curValue - m_minValue) / (m_maxValue - m_minValue);
                if (value < 0.0f)
                {
                    value = 0.0f;
                }
                else if (value > 1.0f)
                {
                    value = 1.0f;
                }
            }
            return (IsHorizontal() ? GetMaxBarRect().width : GetMaxBarRect().height) * value;
        }

        ProgressBarWnd::ProgressBarWnd(ProgressBarWnd const& rhs)
        {
            // ok
        }

        ProgressBarWnd::ProgressBarWnd()
        {
            m_barColor = GetGfxServer()->GetColor(1);
        }

        BoundsBase<float> ProgressBarWnd::GetMaxBarRect() const
        {
            return GetClientBounds();
        }
    }
}
