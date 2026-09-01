#include <ui/textbox.h>
#include <ui/scroll.h>
#include <ui/ui_srv.h>
#include <m3dapp.h>

namespace m3d
{
    namespace ui
    {
        RT_CLASS_EXPORTS_BEGIN(TextBoxWnd)
        RT_CLASS_EXPORTS_END;
        RT_CLASS_DEFINE(TextBoxWnd);

        Class* TextBoxWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(FormattedStringsListBoxWnd);
        }

        Object* TextBoxWnd::CreateObject()
        {
            return new TextBoxWnd;
        }

        int TextBoxWnd::SetText(CStr const& caption)
        {
            auto res = Wnd::SetText(caption);
            RecalcLayout();
            return res;
        }

        CStr TextBoxWnd::GetText() const
        {
            return m_caption;
        }

        Object* TextBoxWnd::Clone()
        {
            return new TextBoxWnd;
        }

        int TextBoxWnd::SetVScrollToPos(unsigned pos)
        {
            if (!m_scrollVWnd)
            {
                return 0;
            }
            if (pos > 100)
            {
                pos = 100;
            }
            m_scrollVWnd->SetCurPos(m_scrollVWnd->GetMaxPos() * static_cast<float>(pos) * 0.01f);
            return 1;
        }

        Class* TextBoxWnd::GetClass() const
        {
            return RT_CLASS_LOCAL(TextBoxWnd);
        }

        int TextBoxWnd::Create(CStr const& capt, unsigned style, BoundsBase<float> const& rc, unsigned id)
        {
            if (!ListBoxWnd<m3d::ui::FormattedLine>::Create(rc, style, id))
                return 0;
            m_drawFlags |= 5u;
            RecalcLayout();
            SetText(capt);
            return 1;
        }

        int TextBoxWnd::Create(BoundsBase<float> const& rc, unsigned flags, int id)
        {
            auto result = ListBoxWnd<m3d::ui::FormattedLine>::Create(rc, flags, id);
            if (result)
            {
                m_drawFlags |= 5u;
                RecalcLayout();
                return 1;
            }
            return result;
        }

        TextBoxWnd::~TextBoxWnd() = default;

        void TextBoxWnd::RecalcLayout()
        {
            retruxx::vector<m3d::ui::FormattedLine> linesOfText;

            BoundsBase<float> const clientBounds = GetClientBounds();

            DrawInfo di{};
            di.m_clientRect.x0 = 0.0f;
            di.m_clientRect.y0 = 0.0f;
            di.m_clientRect.width = clientBounds.width;
            di.m_clientRect.height = clientBounds.height;
            di.m_clientClippedRect = di.m_clientRect;

            Wnd::m_gfx->SetFont(m_defFont);

            float x;
            if (m_textFormat == TF_CENTER)
            {
                x = m_bounds.width * 0.5f;
            }
            else if (m_textFormat == TF_RIGHT)
            {
                x = m_bounds.width;
            }
            else
            {
                x = 0.0f;
            }

            PointBase<float> at;
            at.x = x;
            at.y = 0.0f;

            M3D_APP->FormatText(linesOfText, at, m_caption, di, TW_WORD_WRAP, m_textFormat);

            RemoveAllItems();

            unsigned int const color =
                ((m_style & 2) != 0 || (m_style & 0x80000) != 0) ? m_textColorDisabled : m_textColor;
            for (auto& line : linesOfText)
            {
                line.m_color = color;
                AddItem(line);
            }
        }

        int TextBoxWnd::OnPaint(DrawInfo const& di)
        {
            return ListBoxWnd<m3d::ui::FormattedLine>::OnPaint(di);
        }

        TextBoxWnd::TextBoxWnd(TextBoxWnd const&) : TextBoxWnd()
        {
        }

        TextBoxWnd::TextBoxWnd()
        {
            m_textFormat = TF_FULL;
        }
    }  // namespace ui
}  // namespace m3d
