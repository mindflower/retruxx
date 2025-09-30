#include <m3dapp.h>
#include <core/ini.h>
#include <ui/button.h>

#include "config.h"
#include "core/kernel.h"
#include "ui/frame.h"

#include "thirdparty/injecttools.h"
#include <thread>

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
            if (m_isImaged)
            {
                return 0.0;
            }
            return Wnd::GetFrameWidth();
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
            m_isInside = 0;
            return Wnd::OnMouseOut();
        }

        int ButtonWnd::OnLoosingFocus()
        {
            throw std::logic_error("Not implemented");
        }

        int ButtonWnd::OnMouseIn()
        {
            m_isInside = 1;
            return Wnd::OnMouseIn();
        }

        ButtonWnd::ButtonWnd(ButtonWnd const&)
        {
            //throw std::logic_error("Not implemented");
        }

        void ButtonWnd::ReleaseTextures()
        {
            M3D_APP->m_renderer->ReleaseTexture(m_image);
            M3D_APP->m_renderer->ReleaseTexture(m_imageMouseIn);
            M3D_APP->m_renderer->ReleaseTexture(m_imageMouseDown);
            M3D_APP->m_renderer->ReleaseTexture(m_imageDisabled);
        }

        void ButtonWnd::OnNcPaint(DrawInfo const& di, unsigned clr)
        {
            if (m_isImaged)
            {
                rend::TexHandle tex;
                if ((m_style & 2) != 0)
                {
                    tex = m_imageDisabled;
                }
                else if ((m_mouseDown & 1) != 0)
                {
                    tex = m_imageMouseDown;
                }
                else if (m_isInside)
                {
                    tex = m_imageMouseIn;
                }
                else
                {
                    tex = m_image;
                }
                auto rect = GetBounds();
                rect.x0 = 0.0;
                rect.y0 = 0.0;
                GetGfxServer()->AddImagedRect(di, rect, clr, tex);
            }
            else
            {
                PaneFlagBg bgFlag = PANE_FLAG_BG_OUT;
                if ((m_style & 2) != 0)
                {
                    bgFlag = PANE_FLAG_BG_DISABLE;
                }
                else if ((m_mouseDown & 1) != 0)
                {
                    bgFlag = PANE_FLAG_BG_DOWN;
                }
                else
                {
                    bgFlag = m_isInside != 0 ? PANE_FLAG_BG_OVER : PANE_FLAG_BG_OUT;
                }
                auto rect = GetBounds();
                rect.x0 = 0.0;
                rect.y0 = 0.0;
                GetGfxServer()->AddFlatAxialPane0(di, rect, clr, m_paneFlags, m_paneName, bgFlag);
            }
        }

        int ButtonWnd::OnPaint(DrawInfo const& di)
        {
            //TODO: check and refactor this
            if ((m_style & 0x40) == 0)
            {
                auto clr = m_curClr;
                if ((m_style & 2) == 0 && (m_style & 0x80000) == 0)
                {
                    DrawNonClient(di, clr);
                    DrawWndText(di);
                    return 1;
                }
                if (m_isImaged)
                {
                    if (m_imageDisabled.IsValid())
                    {
                        if (m_image == m_imageDisabled)
                        {
                            clr = 3;
                        }
                        DrawNonClient(di, clr);
                        DrawWndText(di);
                        return 1;
                    }
                }
                else
                {
                    auto pane = GetGfxServer()->GetPane(m_paneName);
                    if (pane)
                    {
                        if (pane->m_frame[3])
                        {
                            DrawNonClient(di, clr);
                            DrawWndText(di);
                            return 1;
                        }
                        if (pane->m_bg[3] == 0)
                        {
                            clr = 3;
                        }
                        DrawNonClient(di, clr);
                        DrawWndText(di);
                        return 1;
                    }
                }
                clr = 3;
                DrawNonClient(di, clr);
                DrawWndText(di);
                return 1;
            }
            DrawWndText(di);
            return 1;
        }

        //RETRUXX_DLL_INJECT_VIRTUAL_FUNCTION(0x006B1670, ButtonWnd::OnMouseButton0)
        int ButtonWnd::OnMouseButton0(unsigned state, PointBase<float> const& at)
        {
            //PointBase<float> firstClick;
            //if (GetStation()->CheckForMouseDblClick(GetStation(), at, state, firstClick))
            //{
            //    return 0;
            //}
            if (!state || !m_isSounded)
            {
                return Wnd::OnMouseButton0(state, at);
            }
            if (!M3D_KERNEL->GetEngineCfg().m_snd_Enable.GetB())
            {
                return Wnd::OnMouseButton0(state, at);
            }
            GetGfxServer()->PlayControlSound("CONTROL_SOUND_BUTTON_CLICK_DEFAULT", nullptr);
            return Wnd::OnMouseButton0(state, at);
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
            m_isChecked = chk;
            m_paneName = chk ? m_checkedPaneName : m_uncheckedPaneName;
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
            return m_isChecked;
        }

        void CheckWnd::SetUncheckedPane(CStr const&)
        {
            throw std::logic_error("Not implemented");
        }

        int CheckWnd::ReadFromXmlNode(cmn::XmlFile* file, cmn::XmlNode* node)
        {
            if (ButtonWnd::ReadFromXmlNode(file, node))
            {
                m3d::SafeStrAttrib(m_checkedPaneName, node, "checkedPaneName");
                m3d::SafeStrAttrib(m_checkedPaneName, node, "uncheckedPaneName");
                auto isChecked = 0;
                if (!node->IsEmpty())
                {
                    auto isCheckedAttr = node->GetAttribute("isChecked");
                    if (isCheckedAttr)
                        isChecked = atoi(isCheckedAttr);
                }
                SetCheck(isChecked);
                return 1;
            }
            return 0;
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
            if (!Wnd::CreateWnd(caption, style, rc, id))
            {
                return 0;
            }
            m_style |= 0x00040000;
            m_textWrap = TW_WORD_WRAP;
            m_isChecked = 0;
            m_style &= 0xFBFFFFFF;
            m_paneName = m_uncheckedPaneName;
            return 1;
        }

        int CheckWnd::OnMouseButton0(unsigned state, PointBase<float> const& at)
        {
            if (!state)
            {
                return ButtonWnd::OnMouseButton0(state, at);
            }
            SetCheck(m_isChecked == 0);
            return ButtonWnd::OnMouseButton0(state, at);
        }

        void CheckWnd::DrawWndText(DrawInfo const& di)
        {
            throw std::logic_error("Not implemented");
        }

        BoundsBase<float> CheckWnd::GetIcoBounds() const
        {
            throw std::logic_error("Not implemented");
        }

        void CheckWnd::OnNcPaint(DrawInfo const& di, unsigned clr)
        {
            auto const origBounds = GetBounds();
            auto fakeBounds = origBounds;
            fakeBounds.width = origBounds.height;
            SetBounds(fakeBounds, true);
            ButtonWnd::OnNcPaint(di, clr);
            SetBounds(origBounds, true);
        }

        CheckWnd::CheckWnd()
        {
            m_paneFlags = 1;
            m_textFormat = TF_LEFT;
        }

        CheckWnd::CheckWnd(CheckWnd const&)
        {
            throw std::logic_error("Not implemented");
        }
    }
}
