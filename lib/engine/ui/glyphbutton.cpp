#include <m3dapp.h>
#include <ui/glyphbutton.h>
#include <ui/frame.h>
#include <ui/ui_srv.h>

namespace m3d
{
    namespace ui
    {
        namespace
        {
            // Gap between the glyph and the caption.
            float const GLYPH_TEXT_SPACE = 5.0f;

            // Colour index the disabled state paints in.
            unsigned const CLR_DISABLED = 3;

            // The stock buttons take their control id from the message-box return
            // code they stand for, and their caption from a "^name^" markup token.
            struct StockGlyph
            {
                char const* m_name;
                unsigned m_id;
            };

            StockGlyph const STOCK_GLYPHS[] = {
                {"ok", MBX_RET_OK},
                {"cancel", MBX_RET_CANCEL},
                {"yes", MBX_RET_YES},
                {"no", MBX_RET_NO},
            };
        }  // namespace

        RT_CLASS_EXPORTS_BEGIN(GlyphButton)
        RT_CLASS_EXPORTS_END;
        RT_CLASS_DEFINE(GlyphButton);

        GlyphButton::GlyphButton()
        {
            // RVA 0x79CDA0
            m_glyphTexture.SetInvalid();
            m_glyphTextureDisabled.SetInvalid();
            m_glyphHeight = GetGfxServer()->GetGlyphHeight();
            m_space = GLYPH_TEXT_SPACE;
        }

        GlyphButton::GlyphButton(GlyphButton const&)
        {
            // RVA 0x79CDF0 - the shipped copy constructor copies nothing from rhs
            // and, unlike the default one, leaves the glyph height and the spacing
            // uninitialised; only the two texture handles are made safe to release.
            m_glyphTexture.SetInvalid();
            m_glyphTextureDisabled.SetInvalid();
        }

        GlyphButton::~GlyphButton()
        {
            // RVA 0x79C7C0
            if (m_glyphTexture.IsValid())
            {
                Application::g_pApp->m_renderer->ReleaseTexture(m_glyphTexture);
            }
            if (m_glyphTextureDisabled.IsValid())
            {
                Application::g_pApp->m_renderer->ReleaseTexture(m_glyphTextureDisabled);
            }
        }

        Object* GlyphButton::Clone()
        {
            // RVA 0x79D360
            return new GlyphButton(*this);
        }

        Object* GlyphButton::CreateObject()
        {
            return new GlyphButton;
        }

        Class* GlyphButton::GetBaseClass()
        {
            return RT_CLASS_LOCAL(ButtonWnd);
        }

        Class* GlyphButton::GetClass() const
        {
            return RT_CLASS_LOCAL(GlyphButton);
        }

        float GlyphButton::GetGlyphHeight()
        {
            // RVA 0x79C790
            return m_glyphHeight;
        }

        void GlyphButton::SetGlyphHeight(float height)
        {
            // RVA 0x79C7A0
            m_glyphHeight = height;
        }

        rend::TexHandle GlyphButton::GetGlyphTexture() const
        {
            // RVA 0x79CD80
            return m_glyphTexture;
        }

        rend::TexHandle GlyphButton::GetGlyphTextureDisabled() const
        {
            // RVA 0x79CD90
            return m_glyphTextureDisabled;
        }

        void GlyphButton::SetGlyphTexture(rend::TexHandle tex, rend::TexHandle texDisabled)
        {
            // RVA 0x79C9F0
            if (m_glyphTexture.IsValid())
            {
                Application::g_pApp->m_renderer->ReleaseTexture(m_glyphTexture);
            }
            m_glyphTexture = tex;
            if (m_glyphTexture.IsValid())
            {
                Application::g_pApp->m_renderer->ReferenceTexture(m_glyphTexture);
            }

            if (m_glyphTextureDisabled.IsValid())
            {
                Application::g_pApp->m_renderer->ReleaseTexture(m_glyphTextureDisabled);
            }
            m_glyphTextureDisabled = texDisabled;
            if (m_glyphTextureDisabled.IsValid())
            {
                Application::g_pApp->m_renderer->ReferenceTexture(m_glyphTextureDisabled);
            }
        }

        int GlyphButton::SetGlyphTexture(CStr const& fileName, CStr const& fileNameDisabled)
        {
            // RVA 0x79C920 - only the enabled glyph decides success.
            if (m_glyphTexture.IsValid())
            {
                Application::g_pApp->m_renderer->ReleaseTexture(m_glyphTexture);
            }
            m_glyphTexture = Application::g_pApp->m_renderer->AddTexture(fileName, 4);
            if (m_glyphTexture.IsValid())
            {
                Application::g_pApp->m_renderer->ReferenceTexture(m_glyphTexture);
            }

            if (m_glyphTextureDisabled.IsValid())
            {
                Application::g_pApp->m_renderer->ReleaseTexture(m_glyphTextureDisabled);
            }
            m_glyphTextureDisabled = Application::g_pApp->m_renderer->AddTexture(fileNameDisabled, 4);
            if (m_glyphTextureDisabled.IsValid())
            {
                Application::g_pApp->m_renderer->ReferenceTexture(m_glyphTextureDisabled);
            }

            return m_glyphTexture.IsValid();
        }

        int GlyphButton::CreateGlyphButton(BoundsBase<float> const& rc, GlyphButtonType type)
        {
            // RVA 0x79CE20 - one of the stock buttons: caption, glyph and control
            // id all follow from the type.
            CStr caption;
            CStr glyphName;
            unsigned id = 0;
            if (type != GBTYPE_USER)
            {
                auto const& stock = STOCK_GLYPHS[type];
                caption = CStr("^") + stock.m_name + "^";
                glyphName = stock.m_name;
                id = stock.m_id;
            }

            m_glyphTexture = GetGfxServer()->GetGlyph(glyphName);
            m_glyphTextureDisabled.SetInvalid();
            if (m_glyphTexture.IsValid())
            {
                Application::g_pApp->m_renderer->ReferenceTexture(m_glyphTexture);
            }
            return ButtonWnd::Create(caption, 0, rc, id);
        }

        int GlyphButton::CreateGlyphButton(unsigned style, BoundsBase<float> const& rc, GlyphButtonType type,
                                           CStr const& caption, unsigned id, rend::TexHandle glyph,
                                           rend::TexHandle glyphDisabled)
        {
            // RVA 0x79D280 - a request that carries no id, no caption and a glyph
            // of its own for a stock type is treated as "just build the stock
            // button", with the style applied afterwards.
            if (id == static_cast<unsigned>(-1) && glyph.IsValid() && caption.empty() && type != GBTYPE_USER)
            {
                if (!CreateGlyphButton(rc, type))
                {
                    return 0;
                }
                if (style)
                {
                    m_style = style;
                }
                return 1;
            }

            if (!ButtonWnd::Create(caption, style, rc, id))
            {
                return 0;
            }
            m_glyphTexture = glyph;
            if (m_glyphTexture.IsValid())
            {
                Application::g_pApp->m_renderer->ReferenceTexture(m_glyphTexture);
            }
            m_glyphTextureDisabled = glyphDisabled;
            if (m_glyphTextureDisabled.IsValid())
            {
                Application::g_pApp->m_renderer->ReferenceTexture(m_glyphTextureDisabled);
            }
            return 1;
        }

        int GlyphButton::CreateGlyphButton(unsigned style, BoundsBase<float> const& rc, GlyphButtonType type,
                                           CStr const& caption, unsigned id, CStr glyphFile, CStr glyphFileDisabled)
        {
            // RVA 0x79C850
            auto const glyph = Application::g_pApp->m_renderer->AddTexture(glyphFile, 4);
            auto const glyphDisabled = Application::g_pApp->m_renderer->AddTexture(glyphFileDisabled, 4);
            return CreateGlyphButton(style, rc, type, caption, id, glyph, glyphDisabled);
        }

        void GlyphButton::AdjustBoundsToFit(float maxW)
        {
            // RVA 0x79D3C0 - grows or shrinks the button around its centre so the
            // frame, the glyph and the caption all fit.
            auto const textSz = GetGfxServer()->MeasureText(m_caption, m_defFont, m_textWrap, maxW);

            float frameW = 0.0f;
            if ((m_style & WS_NOFRAME) == 0)
            {
                if (auto* pane = GetGfxServer()->GetPane(m_paneName))
                {
                    if (pane->m_frame[0])
                    {
                        frameW = static_cast<float>(pane->m_frame[0]->m_barUsedWidth);
                    }
                }
            }

            auto const glyphW = m_glyphTexture.IsValid() ? m_glyphHeight : 0.0f;
            auto const space = m_glyphTexture.IsValid() ? m_space : 0.0f;
            auto const frameW2 = frameW * 2.0f;

            auto const& edges = m_clientEdges;
            auto const wantW = edges[0] + frameW2 + space + glyphW + textSz.x + edges[2];

            auto contentH = textSz.y;
            if (m_glyphTexture.IsValid() && m_glyphHeight > textSz.y)
            {
                contentH = m_glyphHeight;
            }
            auto const wantH = edges[3] + frameW2 + edges[1] + contentH;

            auto const dx = (wantW - m_bounds.width) * 0.5f;
            auto const dy = (wantH - m_bounds.height) * 0.5f;
            m_bounds.x0 = m_bounds.x0 - dx;
            m_bounds.y0 = m_bounds.y0 - dy;
            m_bounds.width = dx * 2.0f + m_bounds.width;
            m_bounds.height = dy * 2.0f + m_bounds.height;
        }

        int GlyphButton::OnPaint(DrawInfo const& di)
        {
            // RVA 0x79CA80 - a button without a glyph is an ordinary button.
            if (!m_glyphTexture.IsValid())
            {
                return ButtonWnd::OnPaint(di);
            }

            auto const bDisabled = (m_style & WS_DISABLE) != 0 || (m_style & WS_GRAYED) != 0;
            if ((m_style & WS_NOFRAME) == 0)
            {
                ButtonWnd::OnNcPaint(di, bDisabled ? CLR_DISABLED : m_curClr);
            }

            auto const textSz = GetGfxServer()->MeasureText(m_caption, m_defFont, m_textWrap, m_bounds.width);
            // A button with no caption puts the glyph dead centre.
            auto const space = textSz.x != 0.0 ? m_space : 0.0f;

            BoundsBase<float> glyphB;
            glyphB.x0 = (di.m_clientRect.width - (m_glyphHeight + space + textSz.x)) * 0.5f;
            glyphB.y0 = (di.m_clientRect.height - m_glyphHeight) * 0.5f;
            glyphB.width = m_glyphHeight;
            glyphB.height = m_glyphHeight;

            // The glyph is drawn against the client area rather than the whole
            // window, so the frame does not clip it.
            DrawInfo di0 = di;
            di0.m_originalRect = di0.m_clientRect;
            di0.m_clippedRect = di0.m_clientClippedRect;

            if (bDisabled)
            {
                if (m_glyphTextureDisabled.IsValid())
                {
                    GetGfxServer()->AddImagedRect(di0, glyphB, m_curClr, m_glyphTextureDisabled);
                }
                else
                {
                    GetGfxServer()->AddImagedRect(di0, glyphB, CLR_DISABLED, m_glyphTexture);
                }
            }
            else
            {
                GetGfxServer()->AddImagedRect(di0, glyphB, m_curClr, m_glyphTexture);
            }

            PointBase<float> at;
            at.x = m_space + m_glyphHeight + glyphB.x0;
            at.y = (di.m_clientRect.height - textSz.y) * 0.5f;
            CStr const text = (bDisabled ? m_strTextColorDisabled : m_strTextColor) + m_caption;
            GetGfxServer()->AddText(di, at, text, m_defFont, m_textWrap, TF_LEFT);
            return 1;
        }
    }  // namespace ui
}  // namespace m3d
