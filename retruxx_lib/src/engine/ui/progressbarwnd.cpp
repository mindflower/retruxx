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
            //TODO: check and refactor this
            auto maxBar = this->GetMaxBarRect();
            auto valueInPixel = GetValueInPixel();
            auto v4 = m_orientation == ORIENTATION_LEFT_TO_RIGHT || m_orientation == ORIENTATION_RIGHT_TO_LEFT;
            auto v5 = m_orientation == ORIENTATION_RIGHT_TO_LEFT || m_orientation == ORIENTATION_BOTTOM_TO_TOP;

            float* v6 = nullptr;
            float* v7 = nullptr;
            if (v4)
            {
                v6 = &maxBar.x0;
                v7 = &maxBar.width;
            }
            else
            {
                v6 = &maxBar.y0;
                v7 = &maxBar.height;
            }
            if (v5)
                *v6 = *v7 - valueInPixel;
            *v7 = valueInPixel;
            return maxBar;
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

        void ProgressBarWnd::CalcTexCoordinates(float& u0, float& v0, float& u1, float& v1) const
        {
            throw std::logic_error("Not implemented");
            u0 = 0.0;
            v0 = 0.0;
            u1 = 1.0;
            v1 = 1.0;
            //auto curNumOfSteps = 0;
            //if (this->m_minValue == this->m_maxValue)
            //{
            //    curNumOfSteps = 0;
            //}
            //else
            //{
            //    auto value = (this->m_curValue - this->m_minValue) / ((this->m_maxValue - this->m_minValue) / this->m_numOfSteps);
            //    if (value > 0.001)
            //    {
            //        throw std::logic_error("Not implemented");
            //    }
            //    else
            //    {
            //        curNumOfSteps = 0;
            //    }
            //}
            //auto barRect = GetBarRect();
            //float* left = nullptr;
            //float* right = nullptr;
            //auto horizontal = m_orientation == ORIENTATION_LEFT_TO_RIGHT || m_orientation == ORIENTATION_RIGHT_TO_LEFT;
            //auto vert = false;
            //if (m_orientation == ORIENTATION_RIGHT_TO_LEFT || m_orientation == ORIENTATION_BOTTOM_TO_TOP)
            //    vert = true;
            //if (horizontal)
            //{
            //    left = &u0;
            //    right = &u1;
            //
            //}
            //else
            //{
            //    left = &v0;
            //    right = &v1;
            //}
            //auto x = 0;
            //auto y = 0;
            //M3D_RENDERER->GetDims(this->m_barTexture, x, y);
            //auto texDim = x;
            //if (!horizontal)
            //    texDim = y;
            //float perc = 0.0;
            //if (m_textureStyle == TEXTURE_CLAMP)
            //{
            //    float cur = 0.0;
            //    float max = 0.0;
            //    if (m_numOfSteps <= 1)
            //    {
            //        cur = this->m_curValue - this->m_minValue;
            //        max = this->m_maxValue - this->m_minValue;
            //    }
            //    else
            //    {
            //        cur = curNumOfSteps;
            //        max = m_numOfSteps;
            //    }
            //    perc = cur / max;
            //}
            //else
            //{
            //    throw std::logic_error("Not implemented");
            //    if (this->m_textureStyle != TEXTURE_REPEAT)
            //        //goto LABEL_30;
            //    if (m_numOfSteps <= 1)
            //    {
            //        //perc = *v21 / texDim;
            //        //goto LABEL_30;
            //    }
            //    perc = curNumOfSteps;
            //}
            //u1 = perc;
            //if (vert)
            //{
            //    throw std::logic_error("Not implemented");
            //    //*v31 = 1.0 - perc;
            //    //*v20 = 1.0;
            //}
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
            //TODO: check and refactor this
            m3d::ui::ProgressBarWnd* v1; // ecx
            double result; // st7
            float v3; // xmm1_4
            float v4; // xmm0_4
            m3d::ui::ProgressBarWnd::Orientation v5; // eax
            float value; // [esp+4h] [ebp-28h]
            float v7; // [esp+8h] [ebp-24h]
            char v8[16]; // [esp+Ch] [ebp-20h] BYREF
            char v9[16]; // [esp+1Ch] [ebp-10h] BYREF

            if (this->m_numOfSteps <= 1)
            {
                v3 = 0.0;
                value = 0.0;
                if (this->m_minValue != this->m_maxValue)
                {
                    v4 = (this->m_curValue - this->m_minValue) / (this->m_maxValue - this->m_minValue);
                    value = v4;
                    if (v4 < 0.0 || (v3 = 1.0, v4 > 1.0))
                        value = v3;
                }
                v5 = this->m_orientation;
                if (v5 == ORIENTATION_LEFT_TO_RIGHT || v5 == ORIENTATION_RIGHT_TO_LEFT)
                    result = this->GetMaxBarRect().width * value;
                else
                    result = this->GetMaxBarRect().height * value;
            }
            else
            {
                v7 = m3d::ui::ProgressBarWnd::GetCurNumOfSteps();
                result = m3d::ui::ProgressBarWnd::GetSizeOfStepInPixel() * v7;
            }
            return result;
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
            return GetClientBounds();
        }
    }
}
